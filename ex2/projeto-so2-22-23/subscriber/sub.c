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

// Functions
int connect(subscriber_t *subscriber, char register_name[MAX_PIPE_NAME], char pipe_name[MAX_PIPE_NAME], char message_box[MAX_BOX_NAME]);
int getMessages(subscriber_t *subscriber, char *buffer, size_t size);
int disconnect(subscriber_t *subscriber);

int connect(subscriber_t *subscriber, char register_name[MAX_PIPE_NAME], char pipe_name[MAX_PIPE_NAME], char message_box[MAX_BOX_NAME]) {
    subscriber->server_fd = open(register_name, O_WRONLY);
    if (subscriber->server_fd < 0) {
        WARN("Error opening register pipe");
        close(subscriber->server_fd);
        return -1;
    }

    // Send a request to the regsiter pipe
    char request[MAX_REQUEST_SIZE];
    build_request(2, pipe_name, message_box, request);
    if (write(subscriber->server_fd, request, sizeof(request)) < 0) {
        WARN("Error writing in the register pipe");
        return -1;
    }


    // Open the named pipe for reading
    subscriber->pipe_fd = open(pipe_name, O_RDONLY | O_NONBLOCK);
    if (subscriber->pipe_fd < 0) {
        WARN("Error opening named pipe");
        return -1;
    }

    
    return 0;
}

// DUVIDOSO
int getMessages(subscriber_t *subscriber, char *buffer, size_t size) {
    // Use file descriptor sets for good practice even though we only putting 1 fd in it
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(subscriber->pipe_fd, &read_fds);
    if(select(subscriber->pipe_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
        WARN("Error selecting");
        return -1;
    }
    
    //read the last message from the pipe
    if (read(subscriber->pipe_fd, buffer, size) < 0) {
        WARN("Error reading from pipe");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    fprintf(stderr, "usage: sub <register_pipe> <pipe_name> <box_name>\n");
    WARN("unimplemented"); // TODO: implement
    return -1;
}