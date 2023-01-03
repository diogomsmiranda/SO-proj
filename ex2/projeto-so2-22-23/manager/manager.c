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

static void print_usage() {
    fprintf(stderr, "usage: \n"
                    "   manager <register_pipe_name> create <box_name>\n"
                    "   manager <register_pipe_name> remove <box_name>\n"
                    "   manager <register_pipe_name> list\n");
}

int connect(manager_t *manager, const char *register_name, const char *pipe_name);
int create_box(manager_t *manager, const char *message_box, const char *pipe_name);
int remove_box(manager_t *manager, const char *message_box, const char *pipe_name);
int list_boxes(manager_t *manager, const char *pipe_name);

int connect(manager_t *manager, const char *register_name, const char *pipe_name) {

    manager->server_fd = open(register_name, O_WRONLY);
    if(manager->server_fd < 0) {
        WARN("Error opening register pipe for writing");
        return -1;
    }

    manager->pipe_fd = open(pipe_name, O_RDONLY);
    if(manager->pipe_fd < 0) {
        WARN("Error opening named pipe for reading");
        return -1;
    }

    return 0;
}

int create_box(manager_t *manager, const char *message_box, const char *pipe_name) {
    char request[MAX_REQUEST_SIZE];
    build_request(3,pipe_name,message_box, request);

    if(write(manager->pipe_fd,request,sizeof(request)) < 0) {
        WARN("Error writing in the named pipe");
        return -1;
    }

    return 0;
}

int remove_box(manager_t *manager, const char *message_box, const char *pipe_name) {

    char request[MAX_REQUEST_SIZE];
    build_request(5,pipe_name,message_box,request);

    if(write(manager->pipe_fd,request,sizeof(request)) < 0) {
        WARN("Error writing in the named pipe");
        return -1;
    }

    return 0;
}

int list_boxes(manager_t *manager,const char *pipe_name) {

    char request[MAX_REQUEST_SIZE];
    build_request(7, pipe_name, NULL, request);

    if(write(manager->pipe_fd,request,sizeof(request)) < 0) {
        WARN("Error writing in the named pipe");
        return -1;
    }

    return 0;
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    print_usage();
    WARN("unimplemented"); // TODO: implement
    return -1;
}
