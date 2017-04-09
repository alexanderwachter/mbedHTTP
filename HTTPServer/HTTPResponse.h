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
 
#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "mbed.h"
#include "TCPSocket.h"
#include "HTTPTypes.h"
#include <string> 
#include <map>
using std::string;
using std::map;

struct RespCodeMap
{
  HTTPResponseCode code;
  const char* str;
};

class HTTPResponse
{
public:
  HTTPResponse(): _data(nullptr), _content_length(0), _resp_code(HTTP_Ok){}
  virtual ~HTTPResponse(){}

  void setRespCode(HTTPResponseCode code){_resp_code = code;}
  void setData(const char* data);
  void send(TCPSocket* socket);
  const char* getResponseCodeStr(HTTPResponseCode code);

private:
  const char* _data;
  static const RespCodeMap _resp_code_map[];
  map<string, string> _response_header_fields;
  uint _content_length;
  HTTPResponseCode _resp_code;
};
 
#endif
 
