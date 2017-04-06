#ifndef HTTP_TYPES_H
#define HTTP_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

  enum http_error {
    HTTP_OK                =  0,     /*!< no error */
    HTTP_ERROR_PARSING     = -1,     /*!< syntax error while parsing */
    HTTP_ERROR_METHOD      = -2,     /*!< wrong or unknown method */
    HTTP_ERROR_VERSION     = -3,     /*!< unsupported version */
    HTTP_ERROR_BUFFER      = -4      /*!< buffer overflow (line to long) */
};

#ifdef __cplusplus
}
#endif

#endif
