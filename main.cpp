#include "mbed.h"
#include "rtos.h"
#include "EthernetInterface.h"
#include "TCPServer.h"
#include "TCPSocket.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "HTTPDispatcher.h"

// #include "HTTPDispatcher.h"

DigitalIn user_sw(USER_BUTTON);
Serial pc(USBTX, USBRX, 115200);
EthernetInterface net;
TCPServer srv;

class TestHandler : public HTTPRequestHandle
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
  
}
void TestHandler::doPut(HTTPRequest& request, HTTPResponse& response)
{
  
}
void TestHandler::doDelete(HTTPRequest& request, HTTPResponse& response)
{
  
}

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

  srv.open(&net);
  srv.bind(net.get_ip_address(), 80);
  srv.listen(5);
  while(true)
  {
    SocketAddress clienr_addr;
    TCPSocket client_sock;
    HTTPRequest req(&client_sock);
    HTTPDispatcher disp;
    HTTPResponse resp;
    TestHandler test_h;
    int ret;

    disp.addHandler(&test_h, "/test.html");
    srv.accept(&client_sock, &clienr_addr);
    printf("accept %s:%d\n", clienr_addr.get_ip_address(), clienr_addr.get_port());
    if((ret = req.read()))
    {
      printf("Reqest handler error %d\n", ret);
      continue;
    }
    disp.dispatch(req, resp);
    resp.send(&client_sock);
  }

}
