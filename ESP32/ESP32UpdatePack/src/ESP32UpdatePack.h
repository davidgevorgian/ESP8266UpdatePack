/*
	ver1.01 [09/08/2017]
	- small optimized
	ver2.0 [05/10/17]
	- new magic byte = $A7
	- add command [4] Fire OnState function
	ver2.1 [august 18]
	- transfer from esp8266 to ESP32

*/

#ifndef __ESP32_Update_Pack__
#define __ESP32_Update_Pack__

#include <Arduino.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <Update.h>

#define UPK_ERROR_OK            (0)
#define UPK_ERROR_ABORT_UPLOAD  (1)
#define UPK_ERROR_BAD_PACKAGE   (2)
#define UPK_ERROR_FILE_TYPE     (3)
#define UPK_ERROR_FILE_CREATE   (4)
#define UPK_ERROR_ROM_BEGIN     (5)
#define UPK_ERROR_ROM_WRITE     (6)
#define UPK_ERROR_FILE_WRITE    (7)
#define UPK_ERROR_ROM_END       (8)
#define UPK_ERROR_BAD_LOGIN     (9)

#define UPK_STATE_START		      (1)
#define UPK_STATE_FINISHED      (2)
#define UPK_STATE_ABORT		      (3)
#define UPK_STATE_ERROR		      (4)

#define FS_FileName_Length      (32)
#define UPK_MAGIC_BYTE          (0xA7)

static const char ESP32_UP_msg_get[] PROGMEM = "<html><body>Update Package<br><form method='POST' action='' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form></body></html>";
static const char ESP32_UP_msg_ok[] PROGMEM = "<html><head><script>window.setTimeout(function(){top.location.href='/'},10000);</script></head><body>Done. Restart, please wait ...</body></html>";

class ESP32UpdatePack
{
  private:
    WebServer *_server;
    char *_username;
    char *_password;

    File     u_file;
    uint8_t  u_buf[FS_FileName_Length];
    uint8_t  u_buflen;
    uint8_t  u_step;
    uint8_t  u_ftype;
    uint32_t u_fsize;
    struct{
       uint8_t StateOn : 1; 
    }Flags;
    void (*FOnState)(uint8_t) = NULL;

  public:
  	uint8_t  error;
     void OnState(void (*Function)(uint8_t)){FOnState = Function;}

    ESP32UpdatePack(WebServer *server);

    void set(const char *urlpath, const char* username, const char* password) {set(urlpath, (char*)username, (char*)password);}
    void set(const char *urlpath, char* username, char* password);
    void set(const char *urlpath) {set(urlpath, (char*)NULL, (char*)NULL);}
    void set() {set("/update", (char*)NULL, (char*)NULL);}
    void fupload(uint8_t passwordcheck);
    void abort();
};
#endif
