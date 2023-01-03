#include "logging.h"
#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_MESSAGE_SIZE 1033
#define MAX_BOX_NAME 32

// Functions
int connect(publisher_t *publisher, const char *register_name, const char *pipe_name, const char *message_box);
int publish(publisher_t *publisher, const char *message);
int disconnect(publisher_t *publisher);

int connect(publisher_t *publisher, const char *register_name, const char *pipe_name, const char *message_box) {
    // Open the register pipe
    publisher->server_fd = open(register_name, O_WRONLY);
    if(publisher->server_fd < 0) {
        WARN("Error opening register pipe for writing");
        close(publisher->server_fd);
        return -1;
    }

    strcpy(publisher->message_box,message_box);

    char request[MAX_REQUEST_SIZE];
    build_request(1, pipe_name, message_box, request);

    if(write(publisher->server_fd,request,sizeof(request)) < 0) {
        WARN("Error writing in the register pipe");
        return -1;
    }

    publisher->pipe_fd = open(pipe_name, O_WRONLY);
    if(publisher->pipe_fd < 0) {
        return -1;
    }

    return 0;
}

int publish(publisher_t *publisher, const char *message) {
    //create a message and send it to the pipe
    char buffer[MAX_MESSAGE_SIZE];
    build_message(9, message, buffer);
    if(write(publisher->pipe_fd,buffer,sizeof(buffer)) < 0) {
        WARN("Error writing in the pipe");
        return -1;
    }

    return 0;
}

int disconnect(publisher_t *publisher) { return close(publisher->pipe_fd); }


int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    fprintf(stderr, "usage: pub <register_pipe_name> <box_name>\n");
    WARN("unimplemented"); // TODO: implement
    return -1;
}
