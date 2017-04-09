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
  typedef enum 
  {
    GET,        /*!< GET request */
    POST,       /*!< POST request */
    PUT,        /*!< PUT request */
    OPTIONS,    /*!< OPTIONS request */
    HEAD,       /*!< HEAD request */
    DELETE,     /*!< DELETE request */
    TRACE,      /*!< TRACE request */
    CONNECT     /*!< CONNECT request */
  } RequestMethod;
  
typedef enum {
    HTTP_Continue                       = 100,  // 100
    HTTP_SwitchingProtocols             = 101,  // 101
    HTTP_Ok                             = 200,  // 200
    HTTP_Created                        = 201,  // 201
    HTTP_Accepted                       = 202,  // 202
    HTTP_NonAuthoritativeInformation    = 203,  // 203
    HTTP_NoContent                      = 204,  // 204
    HTTP_ResetContent                   = 205,  // 205
    HTTP_PartialContent                 = 206,  // 206
    HTTP_MultipleChoices                = 300,  // 300
    HTTP_MovedPermanently               = 301,  // 301
    HTTP_Found                          = 302,  // 302
    HTTP_SeeOther                       = 303,  // 303
    HTTP_NotModified                    = 304,  // 304
    HTTP_UseProxy                       = 305,  // 305
    HTTP_TemporaryRedirect              = 307,  // 307
    HTTP_BadRequest                     = 400,  // 400
    HTTP_Unauthorized                   = 401,  // 401
    HTTP_PaymentRequired                = 402,  // 402
    HTTP_Forbidden                      = 403,  // 403
    HTTP_NotFound                       = 404,  // 404
    HTTP_MethodNotAllowed               = 405,  // 405
    HTTP_NotAcceptable                  = 406,  // 406
    HTTP_ProxyAuthRequired              = 407,  // 407
    HTTP_RequestTimeOut                 = 408,  // 408
    HTTP_Conflict                       = 409,  // 409
    HTTP_Gone                           = 410,  // 410
    HTTP_LengthRequired                 = 411,  // 411
    HTTP_PreconditionFailed             = 412,  // 412
    HTTP_RequestEntityTooLarge          = 413,  // 413
    HTTP_RequestURITooLarge             = 414,  // 414
    HTTP_UnsupportedMediaType           = 415,  // 415
    HTTP_RequestedRangeNotSatisfiable   = 416,  // 416
    HTTP_ExpectationFailed              = 417,  // 417
    HTTP_InternalServerError            = 500,  // 500
    HTTP_NotImplemented                 = 501,  // 501
    HTTP_BadGateway                     = 502,  // 502
    HTTP_ServiceUnavailable             = 503,  // 503
    HTTP_GatewayTimeout                 = 504,  // 504
    HTTP_HTTPVersionNotSupported        = 505,  // 505
} HTTPResponseCode;
#ifdef __cplusplus
}
#endif

#endif
