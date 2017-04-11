#include "HTTPServer.h"
#include <SocketAddress.h>
#include "HTTPDispatcher.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include <NetworkInterface.h>
#include <Thread.h>

HTTPServer::HTTPServer(NetworkInterface* net) : _run(false)
{
  _net = net;
  if(!net)
    return;
  printf("open tcp server\n");
  _srv.open(net);
  _srv.bind(net->get_ip_address(), 80);
  _srv.listen(1);
}

void HTTPServer::runThread()
{
  if(!_run)
    _thread.start(callback(accept_blocking, this));
  _run = true;
}

void HTTPServer::killThread()
{
  _run = false;
  _thread.terminate();
  _thread.join();
}

void HTTPServer::accept_blocking()
{
  accept_blocking(this);
}

void HTTPServer::accept_blocking(HTTPServer* srv)
{
  SocketAddress clienr_addr;
  int ret;
  printf("listening\n");
  while(srv->_run)
  {
    HTTPRequest req(&srv->_client_sock);
    HTTPResponse resp;

    srv->_srv.accept(&(srv->_client_sock), &clienr_addr);
    printf("accept %s:%d\n", clienr_addr.get_ip_address(), clienr_addr.get_port());
    if((ret = req.read()))
      resp.setRespCode(HTTP_BadRequest);
    else
      srv->_dispatcher.dispatch(req, resp);
    resp.send(&(srv->_client_sock));
    srv->_client_sock.close();
  }
}
