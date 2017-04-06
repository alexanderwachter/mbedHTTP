#include "HTTPRequestHandler.h"
#include <string.h>
#include "HTTPTypes.h"

map<string, HTTPRequestHandler::RequestMethod> HTTPRequestHandler::_method_mapping = create_method_map();


int HTTPRequestHandler::parseURL(char* buffer)
{
  string str(buffer);
  _path += "/" + str;
  return HTTP_OK;
}

int HTTPRequestHandler::parseMethod(char* buffer)
{
  string str(buffer);
  //TODO: maybe use a more efficient data type
  map<string, RequestMethod>::iterator it = _method_mapping.find(str);
  if(it != _method_mapping.end())
  {
    _method = it->second;
    return HTTP_OK;
  }
  return HTTP_ERROR_METHOD;
}

int HTTPRequestHandler::parseVersion(char* buffer)
{
  if(strcmp(buffer, "HTTP/1.1") && strcmp(buffer, "HTTP/1.0"))
    return HTTP_ERROR_VERSION;
  return HTTP_OK;
}

int HTTPRequestHandler::parseFields(char* buffer)
{
  char* name = buffer;
  char* value;
  buffer = strchr(buffer, ':');
  if(!buffer)
    return HTTP_ERROR_PARSING;
  *buffer = '\0';
  buffer += 2;
  value = buffer;
  if(!strcmp(name, "Content-Length"))
    _content_length = atoi(value);
  _reqest_header_fields.insert(std::pair<string, string>(name, value));
  return HTTP_OK;
}

int HTTPRequestHandler::parseData(char* buffer, map<string, string>& data_map)
{
  char* name = buffer;
  char* value;
  value = strchr(buffer, '=');
  *value = '\0';
  value++;
  data_map.insert(std::pair<string, string>(name, value));
  return HTTP_OK;
}

int HTTPRequestHandler::fillBuffer(char* buffer, int buffer_len, char* start_ptr)
{
  char* buffer_end = buffer + buffer_len - 1;
  nsapi_size_or_error_t read_len;
  if(start_ptr != buffer_end)
  {
    //preserve the unprocessed data
    size_t len = buffer_end - start_ptr;
    memcpy(buffer, start_ptr, len);
    start_ptr = buffer + len;
    buffer_len -= len;
    //buffer ist to small for this line
    if(buffer_len < 2)
      return HTTP_ERROR_BUFFER;
  }
  else
    start_ptr = buffer;
  read_len = _socket->recv(start_ptr, buffer_len - 1);
  if(read_len <= 0)
    return read_len;
  *(start_ptr + read_len) = '\0';
  return read_len;
}

int HTTPRequestHandler::read()
{
  char buffer[100];
  char* start_ptr = buffer;
  char* end_ptr = buffer + sizeof(buffer) - 1;
  int ret;
  int remaining_data = 1;
  typedef enum{URL_PATH, URL_PARAM, URL_FIN} url_state_t;
  url_state_t url_state = URL_PATH;
  

  _socket->set_blocking(true);
  _socket->set_timeout(1500);

  fillBuffer(buffer, sizeof(buffer), end_ptr);
  end_ptr = buffer;

  while(*(++end_ptr) && *end_ptr != ' ');
  *end_ptr = '\0';
  if((ret = parseMethod(start_ptr)) != HTTP_OK)
    return ret;
  start_ptr = ++end_ptr;

  while(true)
  {
    while(*end_ptr && *end_ptr != ' ' && *end_ptr != '/' && *end_ptr != '?' && *end_ptr != '&')
      end_ptr++;
    if(*end_ptr == '\0')
    {
      ret = fillBuffer(buffer, sizeof(buffer), start_ptr);
      if(ret <= 0)
        return ret;
      start_ptr = buffer;
      end_ptr = buffer;
      continue;
    }
    switch(url_state)
    {
      case URL_PATH:
        if(*end_ptr == ' ')
          url_state = URL_FIN;
        if(*end_ptr == '?')
          url_state = URL_PARAM;
        *end_ptr = '\0';
        if(*start_ptr)
          parseURL(start_ptr);
        break;
      case URL_PARAM:
        if(*end_ptr == ' ')
          url_state = URL_FIN;
        *end_ptr = '\0';
        parseData(start_ptr, _url_param);
        break;
      case URL_FIN:
        break;
    }
    start_ptr = ++end_ptr;
    if(url_state == URL_FIN)
      break;
  }
  while(true)
  {
    while(*(++end_ptr) && !(*(end_ptr-1) == '\r' && *end_ptr == '\n'));
    if(*end_ptr == '\0')
    {
      ret = fillBuffer(buffer, sizeof(buffer), start_ptr);
      if(ret <= 0)
        return ret;
      start_ptr = buffer;
      end_ptr = buffer;
      continue;
    }
    *(end_ptr-1) = '\0';
    if((ret = parseVersion(start_ptr)) != HTTP_OK)
      return ret;
    start_ptr = ++end_ptr;
    break;
  }
  while(true)
  {
    while(*(++end_ptr) && !(*(end_ptr-1) == '\r' && *end_ptr == '\n'));
    if(*end_ptr == '\0')
    {
      ret = fillBuffer(buffer, sizeof(buffer), start_ptr);
      if(ret <= 0)
        return ret;
      start_ptr = buffer;
      end_ptr = buffer;
      continue;
    }
    if(end_ptr - start_ptr == 1)
      break;
    *(end_ptr-1) = '\0';  //terminate c string
    ret = parseFields(start_ptr);
    if(ret != HTTP_OK)
      return ret;
    start_ptr = ++end_ptr;
  }
  start_ptr = ++end_ptr;
  if(!((_method == POST || _method == PUT) && _content_length))
    return HTTP_OK;
  while(true)
  {
    while(*(++end_ptr) && *end_ptr != '&');
    remaining_data = _content_length - (end_ptr - start_ptr);
    if(*end_ptr == '\0' && remaining_data)
    {
      ret = fillBuffer(buffer, sizeof(buffer), start_ptr);
      if(ret <= 0)
        return ret;
      start_ptr = buffer;
      end_ptr = buffer;
      continue;
    }
    *end_ptr = '\0';  //terminate c string
    ret = parseData(start_ptr, _request_data);
    if(ret < 0)
      return ret;
    _content_length -= end_ptr - start_ptr;
    if(!remaining_data)
      break;
    _content_length--; //the '&'
    start_ptr = ++end_ptr;
  }
  return HTTP_OK;
}
