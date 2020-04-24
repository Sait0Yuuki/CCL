#ifndef HTTPLIB_H
#define HTTPLIB_H

struct http_request {
  char *method;
  char *path;
};

struct http_request *http_request_parse(int fd);
void accept_request(int fd);

#endif