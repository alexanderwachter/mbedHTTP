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
HTTP Server file.
*/

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "mbed.h"
#include "TCPSocket.h"
#include <NetworkInterface.h>
#include <Thread.h>
#include <string>
#include "HTTPDispatcher.h"

class HTTPRequestHandler;

class HTTPServer
{
public:
  HTTPServer(NetworkInterface* net);
  virtual ~HTTPServer(){}
  void runThread();
  void killThread();
  void addHandler(HTTPRequestHandler* handler, std::string path) {_dispatcher.addHandler(handler, path);}
  void accept_blocking();
  static void accept_blocking(HTTPServer* srv);

private:
  NetworkInterface* _net;
  TCPServer _srv;
  TCPSocket _client_sock;
  HTTPDispatcher _dispatcher;
  Thread _thread;
  bool _run;
};

#endif
