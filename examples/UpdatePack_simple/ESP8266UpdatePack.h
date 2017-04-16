/*
MIT License

Copyright (c) [2017] [David Gevorgian]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __ESP8266_Update_Pack__
#define __ESP8266_Update_Pack__

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "FS.h"

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

#define FS_FileName_Length 32

class ESP8266UpdatePack
{
  private:
    static const char *_msg_get;              //HTTP Get message
    static const char *_msg_ok;               //HTTP Post when update is done with no errors
    ESP8266WebServer *_server;
    char *_username;
    char *_password;

    File     u_file;                          //For SPIFFS files (new/rewrite/delete...)
    uint8_t  u_buf[FS_FileName_Length];       //For remaind bytes from receive data
    uint8_t  u_buflen;                        //Count remaind bytes in buffer
    uint8_t  u_step;                          //Package file step
    uint8_t  u_ftype;                         //Package file type or command
    uint32_t u_fsize;                         //Package filse size for save
    uint8_t  error;                           //Just error

  public:
    ESP8266UpdatePack();

    void set(ESP8266WebServer *server, const char *urlpath, const char* username, const char* password) {set(server, urlpath, (char*)username, (char*)password);}
    void set(ESP8266WebServer *server, const char *urlpath, char* username, char* password);
    void set(ESP8266WebServer *server, const char *urlpath) {set(server, urlpath, (char*)NULL, (char*)NULL);}
    void set(ESP8266WebServer *server) {set(server, "/update", (char*)NULL, (char*)NULL);}
};
#endif
