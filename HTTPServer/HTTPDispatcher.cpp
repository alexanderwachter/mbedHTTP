#include "HTTPDispatcher.h"

void HTTPDispatcher::addHandler(HTTPRequestHandle* handle, string path)
{
  if(!handle)
    return;
  _handler.insert(std::pair<string, HTTPRequestHandle*>(path, handle));
}

void HTTPDispatcher::dispatch(HTTPRequest& request, HTTPResponse& response)
{
  HTTPRequestHandle* handler;
  printf("Request path |%s|\n", request.getPath().c_str());
  map<string, HTTPRequestHandle*>::iterator it = _handler.find(request.getPath());
  if(it == _handler.end())//404
  {
    response.setRespCode(HTTP_NotFound);
    return;
  }
  else
    handler = it->second;

  switch(request.getMethod())
  {
    case GET:
      handler->doGet(request, response);
      break;
    case POST:
      handler->doPost(request, response);
      break;
    case PUT:
      handler->doPut(request, response);
      break;
    case DELETE:
      handler->doDelete(request, response);
      break;
    default:
      response.setRespCode(HTTP_NotFound);
      break;
  }
}
