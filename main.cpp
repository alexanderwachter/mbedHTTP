#include "mbed.h"
#include "rtos.h"
#include "EthernetInterface.h"
#include "HTTPServer.h"

DigitalIn user_sw(USER_BUTTON);
Serial pc(USBTX, USBRX, 115200);
EthernetInterface net;

class TestHandler : public HTTPRequestHandler
{
public:
  TestHandler(){}
  virtual ~TestHandler(){}
  void doGet(HTTPRequest&, HTTPResponse&);
  void doPost(HTTPRequest&, HTTPResponse&);
  void doPut(HTTPRequest&, HTTPResponse&);
  void doDelete(HTTPRequest&, HTTPResponse&);
  static const char* _message;
};

const char* TestHandler::_message = 
"<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"<title>Mbed HTTPServer test page</title>\n"
"</head>\n"
"<body>\n"
"<h1>It Works</h1>\n"
"</body>\n"
"</html>";

void TestHandler::doGet(HTTPRequest& request, HTTPResponse& response)
{
  response.setData(_message);
}

void TestHandler::doPost(HTTPRequest& request, HTTPResponse& response)
{
  response.setRespCode(HTTP_MethodNotAllowed);
}

void TestHandler::doPut(HTTPRequest& request, HTTPResponse& response)
{
  response.setRespCode(HTTP_MethodNotAllowed);
}

void TestHandler::doDelete(HTTPRequest& request, HTTPResponse& response)
{
  response.setRespCode(HTTP_MethodNotAllowed);
}

TestHandler test_h;
DigitalOut led1(LED1);

int main() 
{
  printf("Start eth\n");
  net.set_network("10.0.0.10", "255.0.0.0", "10.0.0.1");
  if(net.connect())
  {
    printf("Faild to connect");
    return -1;
  }
  const char *ip = net.get_ip_address();
  printf("IP address is: %s\n", ip ? ip : "No IP");
  HTTPServer http_server(&net);
  http_server.runThread();
  http_server.addHandler(&test_h, "/test.html");
  while(true)
  {
    led1 = !led1;
    wait(0.5);
  }

}
