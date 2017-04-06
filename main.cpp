#include "mbed.h"
#include "rtos.h"
#include "EthernetInterface.h"
#include "TCPServer.h"
#include "TCPSocket.h"
#include "HTTPRequestHandler.h"

DigitalIn user_sw(USER_BUTTON);
Serial pc(USBTX, USBRX, 115200);
EthernetInterface net;
TCPServer srv;

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

    srv.accept(&client_sock, &clienr_addr);
    printf("accept %s:%d\n", clienr_addr.get_ip_address(), clienr_addr.get_port());
    nsapi_size_or_error_t read;
    HTTPRequestHandler req_handler(&client_sock);
    req_handler.read();
  }

}
