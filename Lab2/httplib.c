#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "httplib.h"

#define LIBHTTP_REQUEST_MAX_SIZE 8192

void http_fatal_error(char *message) {
  fprintf(stderr, "%s\n", message);
  exit(ENOBUFS);
}

//提取请求的url(eg./index.html) or method(GET/POST)
struct http_request *http_request_parse(int fd) {
  struct http_request *request = (struct http_request*)malloc(sizeof(struct http_request));
  if (!request) http_fatal_error("Malloc failed");

  char *read_buffer = (char*)malloc(LIBHTTP_REQUEST_MAX_SIZE + 1);
  if (!read_buffer) http_fatal_error("Malloc failed");

  int bytes_read = read(fd, read_buffer, LIBHTTP_REQUEST_MAX_SIZE);
  read_buffer[bytes_read] = '\0'; /* Always null-terminate. */

  char *read_start, *read_end;
  size_t read_size;

  do {
    /* Read in the HTTP method: "[A-Z]*" */
    read_start = read_end = read_buffer;
    while (*read_end >= 'A' && *read_end <= 'Z') read_end++;
    read_size = read_end - read_start;
    if (read_size == 0) break;
    request->method = (char*)malloc(read_size + 1);
    memcpy(request->method, read_start, read_size);
    request->method[read_size] = '\0';

    /* Read in a space character. */
    read_start = read_end;
    if (*read_end != ' ') break;
    read_end++;

    /* Read in the path: "[^ \n]*" */
    read_start = read_end;
    while (*read_end != '\0' && *read_end != ' ' && *read_end != '\n') read_end++;
    read_size = read_end - read_start;
    if (read_size == 0) break;
    request->path = (char*)malloc(read_size + 1);
    memcpy(request->path, read_start, read_size);
    request->path[read_size] = '\0';

    /* Read in HTTP version and rest of request line: ".*" */
    read_start = read_end;
    while (*read_end != '\0' && *read_end != '\n') read_end++;
    if (*read_end != '\n') break;
    read_end++;

    free(read_buffer);
    return request;
  } while (0);

  /* An error occurred. */
  free(request);
  free(read_buffer);
  return NULL;

}

//test
void accept_request(int fd)
{
    struct http_request *request = (struct http_request*)malloc(sizeof(struct http_request));
    request=http_request_parse(fd);


    printf("method:%s\n",request->method);
    printf("url:%s\n",request->path);
}