#include "HTTPResponse.h"
#include "HTTPTypes.h"

const RespCodeMap HTTPResponse::_resp_code_map[] =
  {
    {HTTP_Ok, "OK"},
    {HTTP_Created, "Created"},
    {HTTP_Accepted, "Accepted"},
    {HTTP_NoContent, "No Content"},
    {HTTP_BadRequest, "Bad Request"},
    {HTTP_Unauthorized, "Unauthorized"},
    {HTTP_Forbidden, "Forbidden"},
    {HTTP_NotFound, "Not Found"},
    {HTTP_MethodNotAllowed, "Method Not Allowed"},
    {HTTP_InternalServerError, "Internal Server Error"},
    {HTTP_HTTPVersionNotSupported, "HTTP Version Not Supported"}
  };

void HTTPResponse::setData(const char* data)
{
  _data = data;
  if(_data)
    _content_length = strlen(_data);
}

const char* HTTPResponse::getResponseCodeStr(HTTPResponseCode code)
{
  uint i;
  for(i=0; i < sizeof(_resp_code_map)/sizeof(RespCodeMap); i++)
  {
    if(_resp_code_map[i].code == code)
      return _resp_code_map[i].str;
  }
  return nullptr;
}

void HTTPResponse::send(TCPSocket* socket)
{
  printf("send response\n");
  map<string, string>::iterator fields_it;
  const char* resp_code_str = getResponseCodeStr(_resp_code);
  if(!resp_code_str)
  {
    _resp_code = HTTP_InternalServerError;
    resp_code_str = getResponseCodeStr(_resp_code);
  }
  string header = "HTTP/1.1 " + to_string(_resp_code) + " " + string(resp_code_str) + "\r\n";
  header += "Content-Length: " + to_string(_content_length) + "\r\n";
  for(fields_it = _response_header_fields.begin(); fields_it != _response_header_fields.end(); fields_it++)
  {
    header += fields_it->first + ": " + fields_it->second + "\r\n";
  }
  header += "\r\n";
  socket->send(header.c_str(), header.length());
  if(_data && _content_length)
    socket->send(_data, _content_length);
}
