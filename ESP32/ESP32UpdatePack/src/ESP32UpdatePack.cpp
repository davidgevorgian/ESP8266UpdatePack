/* 
	
 ver 2.1

 PAKAGE FORMAT
 [1B] - magic byte = 0xA7
 [file 1]
 [file 2]
 .....
 [file N]

 -- file --
 [1B] - type (0 - ROM, 1 - SPIFFS add file, 2 - SPIFFS del file, 3 - SPIFFS format, 4 - Disable OnState feedback)
 if type (1, 2)
    [32B] - file name
 if type (0,1)
    [4B] - File size
    [xB] - File body
*/

#include "ESP32UpdatePack.h"

ESP32UpdatePack::ESP32UpdatePack(WebServer *server)
{
  SPIFFS.begin();
  _server   = server;
  _username = NULL;
  _password = NULL;
}

void ESP32UpdatePack::set(const char *urlpath, char* username, char* password)
{
  _username = username;
  _password = password;

  _server->on(urlpath, HTTP_GET, [&](){
      if ((_username != NULL) && (_password != NULL) && (strlen(_password)) && (!_server->authenticate(_username, _password))) return _server->requestAuthentication();
      _server->send_P(200, "text/html", ESP32_UP_msg_get);
    });

  _server->on(urlpath, HTTP_POST, [&]()
    {
      String msg = "<html><body>Update ERROR[" + String(error) + "].</body></html>";
      _server->sendHeader("Connection", "close");      
      if (!error) _server->send_P(200, "text/html", ESP32_UP_msg_ok);
             else _server->send(200, "text/html", msg);
      delay(1000);
      ESP.restart();
    }, [&](){fupload(1);});
}

void ESP32UpdatePack::fupload(uint8_t passwordcheck)

{
	 size_t u_ptr;
      HTTPUpload& upload = _server->upload();
      if(upload.status == UPLOAD_FILE_START){
        error = UPK_ERROR_OK;
        if ((passwordcheck) && (_username != NULL) && (_password != NULL) && (strlen(_password)) && (!_server->authenticate(_username, _password))) error = UPK_ERROR_BAD_LOGIN;
        u_step = 0;
        u_buflen = 0;
        Flags.StateOn = 1; //By default State Feedback ON
        if ((Flags.StateOn) && (FOnState)) FOnState(UPK_STATE_START);
      }else if((!error) && (upload.status == UPLOAD_FILE_WRITE)){
        u_ptr = 0;
        if (u_step == 0)
        {
          //check magic byte
          if (upload.buf[0] != UPK_MAGIC_BYTE) {error = UPK_ERROR_BAD_PACKAGE; return;}
          u_step = 1;
          u_ptr++;
        }
        while (u_ptr < upload.currentSize)
        {
          if (u_step == 1){ //Get File type
            u_ftype = upload.buf[u_ptr++];
            if (u_ftype == 0) {u_step = 3;} //Sketch Flash File
            else if (u_ftype == 1) {u_step = 2;} //SPIFFS add/create File
            else if (u_ftype == 2) {u_step = 2;} //SPIFFS delete File
            else if (u_ftype == 3) { //SPIFFS format
              SPIFFS.end();
              SPIFFS.format();
              SPIFFS.begin();
              u_step = 1; //Next File
            }else if (u_ftype == 4) { //Disable State Feedback
              Flags.StateOn = 0;
              u_step = 1; //Next File
            }else {error = UPK_ERROR_FILE_TYPE; break;}

          }else if (u_step == 2){ //Get File Name
            if ((upload.currentSize - u_ptr + u_buflen) >= FS_FileName_Length)
            {
              memcpy(&u_buf[u_buflen], &upload.buf[u_ptr], FS_FileName_Length - u_buflen);
              if (u_ftype == 1){
                u_file = SPIFFS.open((char*)u_buf, "w+"); //Open SPIFFS file
                if (!u_file) {error = UPK_ERROR_FILE_CREATE; break;}
                u_step = 3;
              }else if (u_ftype == 2){
                SPIFFS.remove((char*)u_buf); //Delete SPIFFS file
                u_step = 1; //Next File
              }
              u_ptr += FS_FileName_Length - u_buflen;
              u_buflen = 0;
            }else{
              //save remainder
              u_buflen = upload.currentSize - u_ptr;
              memcpy(&u_buf, &upload.buf[u_ptr], u_buflen);
              u_ptr += u_buflen;
            }
          }else if (u_step == 3){ //Get File Size
            if ((upload.currentSize - u_ptr + u_buflen) >= sizeof(u_fsize))
            {
              memcpy(&u_buf[u_buflen], &upload.buf[u_ptr], sizeof(u_fsize) - u_buflen);
              memcpy(&u_fsize, &u_buf, sizeof(u_fsize));
              u_ptr += sizeof(u_fsize) - u_buflen;
              u_buflen = 0;
              if ((u_ftype == 0) && (!Update.begin(UPDATE_SIZE_UNKNOWN))) {error = UPK_ERROR_ROM_BEGIN; break;}
              u_step = 4; //Start write file
            }else{
              //save remainder
              u_buflen = upload.currentSize - u_ptr;
              memcpy(&u_buf, &upload.buf[u_ptr], u_buflen);
              u_ptr += u_buflen;
            }
          }else if (u_step == 4){ //Write File
            if (u_fsize)
            {
              size_t chnk = u_fsize;
              if (chnk > (upload.currentSize - u_ptr)) chnk = upload.currentSize - u_ptr;
              if (u_ftype == 0){
                if (Update.write(&upload.buf[u_ptr], chnk) != chnk) {error = UPK_ERROR_ROM_WRITE; break;}
              }else if (u_ftype == 1){
                if (u_file.write(&upload.buf[u_ptr], chnk) != chnk) {error = UPK_ERROR_FILE_WRITE; break;}
              }
              u_ptr += chnk;
              u_fsize -= chnk;
            }            
            if (u_fsize == 0){
              if (u_ftype == 0){
                if ((!Update.end(true)) || Update.hasError()) {error = UPK_ERROR_ROM_END; break;}
              }else if (u_ftype == 1){
                u_file.close();
              }
              u_step = 1; //Next File
            }
          }
        }
      }else if(upload.status == UPLOAD_FILE_END){
        if ((Flags.StateOn) && (FOnState)) FOnState(UPK_STATE_FINISHED);
      }else if(upload.status == UPLOAD_FILE_ABORTED){
        if ((Flags.StateOn) && (FOnState)) FOnState(UPK_STATE_ABORT);
        error = UPK_ERROR_ABORT_UPLOAD;
        Flags.StateOn = 0; //Disable Feedback
      }else if ((error) && (Flags.StateOn) && (FOnState)) {
        FOnState(UPK_STATE_ERROR);
        Flags.StateOn = 0; //Disable Feedback
      }
}

void ESP32UpdatePack::abort()
{
    _server->client().stop();
}
