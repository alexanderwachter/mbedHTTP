#include "HTTPResponse.h"
#include "HTTPTypes.h"

void HTTPResponse::setData(const char* data)
{
  _data = data;
  if(_data)
    _content_length = strlen(_data);
}

void HTTPResponse::send(TCPSocket* socket)
{
  //TODO: response code map
  map<string, string>::iterator it;
  string header = "HTTP/1.1 " + to_string(_resp_code) + " OK\r\n";
  header += "Content-Length: " + to_string(_content_length) + "\r\n";
  for(it = _response_header_fields.begin(); it != _response_header_fields.end(); it++)
  {
    header += it->first + ": " + it->second + "\r\n";
  }
  header += "\r\n";
  socket->send(header.c_str(), header.length());
  if(_data)
    socket->send(_data, _content_length);
}
