/*
Copyright (c) 2017 Alexander Wachter (alexander [at] wachter [dot] cloud)
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
 
/**
HTTP Request Handler header file.
*/
 
#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include "mbed.h"
#include "TCPSocket.h"
 
#include <string>
using std::string;
 
#include <map>
using std::map;
 
///HTTP Server's generic request handler
class HTTPRequestHandler
{
public:
  HTTPRequestHandler(TCPSocket* socket): _socket(socket), _content_length(0){}
  virtual ~HTTPRequestHandler(){}
  int read();
  string toString();
//protected:
  /*
  virtual void doGet(map<string,string> param) = 0;
  virtual void doPost(map<string,string> param) = 0;
  virtual void doPut(map<string,string> param) = 0;
  virtual void doHead(map<string,string> param) = 0;
  virtual void onClose(map<string,string> param) = 0; //Connection is closing
  virtual void close(); //Close socket and destroy data
  */

  typedef enum 
  {
    GET,        /*!< GET request */
    POST,       /*!< POST request */
    PUT,        /*!< PUT request */
    OPTIONS,    /*!< OPTIONS request */
    HEAD,       /*!< HEAD request */
    DELETE,     /*!< DELETE request */
    TRACE,      /*!< TRACE request */
    CONNECT     /*!< CONNECT request */
  } RequestMethod;

protected:
  map<string, string>& getHeaderFields() const;
  string& getPath() const;
  int setData(const char* buf, int len);

private:
  TCPSocket* _socket;
  map<string, string> _reqest_header_fields;
  map<string, string> _request_data;
  map<string, string> _url_param;
  string _path;
  uint _content_length;
  RequestMethod _method;
  static map<string, RequestMethod> _method_mapping;

  static map<string, RequestMethod> create_method_map()
        {
          map<string, RequestMethod> _map;
          _map["GET"] = GET;
          _map["POST"] = POST;
          _map["PUT"] = PUT;
          _map["OPTIONS"] = OPTIONS;
          _map["HEAD"] = HEAD;
          _map["DELETE"] = DELETE;
          _map["TRACE"] = TRACE;
          _map["CONNECT"] = CONNECT;
          return _map;
        }

  int fillBuffer(char* buffer,const int buffer_len, char* end_ptr);
  int parseMethod(char* buffer);
  int parseVersion(char* buffer);
  int parseURL(char* buffer);
  int parseFields(char* buffer);
  int parseData(char* buffer, map<string, string>& data_map);
};
 
#endif
