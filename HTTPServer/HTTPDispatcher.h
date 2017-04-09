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
 
#ifndef HTTP_DISPATCHER_H
#define HTTP_DISPATCHER_H

#include "mbed.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include <string>
#include <map>
using std::string;
using std::map;

class HTTPRequest;

class HTTPRequestHandle
{
public:
  HTTPRequestHandle(){}
  virtual ~HTTPRequestHandle(){}
  virtual void doGet(HTTPRequest&, HTTPResponse&) = 0;
  virtual void doPost(HTTPRequest&, HTTPResponse&) = 0;
  virtual void doPut(HTTPRequest&, HTTPResponse&) = 0;
  virtual void doDelete(HTTPRequest&, HTTPResponse&) = 0;
};

class HTTPDispatcher
{
public:
  HTTPDispatcher(){}
  virtual ~HTTPDispatcher(){}
  void addHandler(HTTPRequestHandle* handle, string path);
  void dispatch(HTTPRequest& request, HTTPResponse& response);

private:
  map<string, HTTPRequestHandle*> _handler;
};
 
#endif
 
