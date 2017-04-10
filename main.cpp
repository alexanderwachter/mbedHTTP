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
  static string page =
  "<!DOCTYPE html>\n"
  "<html>\n"
  "<head>\n"
  "<title>Mbed HTTPServer test page</title>\n"
  "</head>\n"
  "<body>\n"
  "<h1>POST Data:</h1>\n";

  for(auto data_it = request._request_data.begin(); data_it != request._request_data.end(); data_it++)
  {
    page += "<p>" + data_it->first + ": " + data_it->second + "</p>";
  }
  page += "</body>\n"
  "</html>";
  response.setData(page.c_str());
}

void TestHandler::doPut(HTTPRequest& request, HTTPResponse& response)
{
  response.setRespCode(HTTP_MethodNotAllowed);
}

void TestHandler::doDelete(HTTPRequest& request, HTTPResponse& response)
{
  response.setRespCode(HTTP_MethodNotAllowed);
}

char _message[30];

class DataHandler : public HTTPRequestHandler
{
public:
  DataHandler(){}
  virtual ~DataHandler(){}
  void doGet(HTTPRequest&, HTTPResponse&);
  void doPost(HTTPRequest&, HTTPResponse&);
  void doPut(HTTPRequest& request, HTTPResponse& response){ response.setRespCode(HTTP_MethodNotAllowed); };
  void doDelete(HTTPRequest&, HTTPResponse&);
  int _data;
private:
};

void DataHandler::doGet(HTTPRequest& request, HTTPResponse& response)
{
  string msg;
  msg = to_string(_data);
  msg += "unit";
  strcpy(_message, msg.c_str());
  response.setData(_message);
};
void DataHandler::doPost(HTTPRequest& request, HTTPResponse& response)
{
  auto data_it = request._request_data.find("data");
  if(data_it == request._request_data.end())
  {
    response.setRespCode(HTTP_NotModified);
    return;
  }
  _data = atoi(data_it->second.c_str());
}

void DataHandler::doDelete(HTTPRequest& request, HTTPResponse& response)
{
  _data = 0;
}

TestHandler test_h;
DataHandler data_h;
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
  http_server.addHandler(&data_h, "/data");
  while(true)
  {
    led1 = !led1;
    wait(0.5);
  }

}
