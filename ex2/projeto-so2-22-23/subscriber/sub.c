#include "logging.h"
#include "structs.h"

#include <stdbool.h>
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

    // Create the named pipe
    if (mkfifo(pipe_name, 0666) < 0) {
        WARN("Error creating named pipe");
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
int getMessages(subscriber_t *subscriber, char buffer[], size_t size) {
    // Use file descriptor sets for good practice even though we only putting 1 fd in it
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(subscriber->pipe_fd, &read_fds);
    int available_data = select(subscriber->pipe_fd + 1, &read_fds, NULL, NULL, NULL);
    if(available_data < 0) {
        WARN("Error selecting");
        return -1;
    } else if (available_data == 0) {
        WARN("No data available");
        return 0;
    }
    
    //read the last message from the pipe
    if (read(subscriber->pipe_fd, buffer, size) < 0) {
        WARN("Error reading from pipe");
        return -1;
    }
    return 1;
}

int main(int argc, char **argv) {
    // Creete the subscriber
    subscriber_t subscriber;
    
    // format: sub <register_pipe> <pipe_name> <box_name>
    // arguments for initialization of the subscriber
    char register_name[MAX_PIPE_NAME];
    char pipe_name[MAX_PIPE_NAME];
    char message_box[MAX_BOX_NAME];
    strcpy(register_name, argv[1]);
    strcpy(pipe_name, argv[2]);
    strcpy(message_box, argv[3]);


    // Connect to the server
    if (connect(&subscriber, register_name, pipe_name, message_box) < 0) {
        WARN("Error connecting to the server");
        return -1;
    }

    char buffer[MAX_BUFFER_MESSAGE];
    while (true) {
        if (getMessages(&subscriber, buffer, sizeof(buffer)) < 0) {
            WARN("Error getting messages");
            break;
        }
        char message[MAX_MESSAGE_SIZE];
        //get the message in the buffer without the code
        memcpy(message,buffer+sizeof(uint8_t),MAX_MESSAGE_SIZE);

        //print the message
        printf("%s\n", message);
        
        // clear the buffer
        memset(buffer, 0, sizeof(buffer));
    }


    
    return -1;
}