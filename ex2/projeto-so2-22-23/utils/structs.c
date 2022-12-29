#include "structs.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

request_t* set_request(int code, const char *message_box, const char *pipe_name) {
    request_t *request;
    request->code = code;
    if (realpath(pipe_name, request->pipe_path) == NULL) { return -1; }
    strcpy(request->box_name, message_box);

    return request;
}

message_t* set_message(int code, const char *message) {
    message_t *msg;
    msg->code = code;
    strcpy(msg->content, message);

    return msg;
}
