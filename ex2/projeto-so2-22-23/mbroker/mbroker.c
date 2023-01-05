#include "logging.h"
#include "operations.h"
#include "structs.h"

#include "state.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>


//int main with args
//format: ./mbroker <pipe_name> <max_sessions>
int main(int argc, char **argv) {
    tfs_init(NULL);

    // create a pipe with pipe_name
    if(mkfifo(argv[1], 0666) == -1) {
        warn("Error creating the fifo");
        exit(1);
    }

    // open the pipe
    int pipe_fd = open(argv[1], O_RDONLY);
    if(pipe_fd < 0) {
        warn("Error opening named pipe for reading");
        exit(1);
    }


    while(true) {
        //read the request from the pipe
        char request[MAX_REQUEST_SIZE];
        if(read(pipe_fd, request, sizeof(request)) < 0) {
            warn("Error reading from the named pipe");
            exit(1);
        }

        // get the code of the request and act accordingly
        u_int8_t code;
        memcpy(code, request, sizeof(u_int8_t));
        

    }

    return 0;
}