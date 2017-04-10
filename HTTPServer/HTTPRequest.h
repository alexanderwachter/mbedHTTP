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
 
#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "mbed.h"
#include "TCPSocket.h"
#include "HTTPTypes.h"
#include <string>
#include <map>
using std::string;
using std::map;
 
///HTTP Server's generic request handler
class HTTPRequest
{
public:
  HTTPRequest(TCPSocket* socket): _socket(socket), _content_length(0){}
  virtual ~HTTPRequest(){}

  string getPath(){return _path;}
  RequestMethod getMethod() {return _method;}
  map<string, string> _reqest_header_fields;
  map<string, string> _request_data;
  map<string, string> _url_param;
  int read();

private:
  TCPSocket* _socket;
  string _path;
  uint _content_length;
  RequestMethod _method;
  static map<string, RequestMethod> _method_mapping;
  int fillBuffer(char* buffer,const int buffer_len, char* end_ptr);
  int parseMethod(char* buffer);
  int parseVersion(char* buffer);
  int parseURL(char* buffer);
  int parseFields(char* buffer);
  int parseData(char* buffer, map<string, string>& data_map);
};
 
#endif
