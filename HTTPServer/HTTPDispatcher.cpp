#include "HTTPDispatcher.h"

const char* HTTPDispatcher::_not_found_message =
"<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"<title>404 Page not found</title>\n"
"</head>\n"
"<body>\n"
"<h1>Page not found!</h1>\n"
"</body>\n"
"</html>";

void HTTPDispatcher::addHandler(HTTPRequestHandler* handle, string path)
{
  if(!handle)
    return;
  _handler.insert(std::pair<string, HTTPRequestHandler*>(path, handle));
}

void HTTPDispatcher::dispatch(HTTPRequest& request, HTTPResponse& response)
{
  HTTPRequestHandler* handler;
  printf("Request path |%s|\n", request.getPath().c_str());
  map<string, HTTPRequestHandler*>::iterator it = _handler.find(request.getPath());
  if(it == _handler.end())//404
  {
    response.setRespCode(HTTP_NotFound);
    response.setData(_not_found_message);
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
