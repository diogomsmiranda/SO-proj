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
int connect(publisher_t *publisher, char register_name[MAX_PIPE_NAME], char pipe_name[MAX_PIPE_NAME], char message_box[MAX_BOX_NAME]);
int publish(publisher_t *publisher, char message[MAX_MESSAGE_SIZE]);
int disconnect(publisher_t *publisher);

int connect(publisher_t *publisher, char register_name[MAX_PIPE_NAME], char pipe_name[MAX_PIPE_NAME], char message_box[MAX_BOX_NAME]) {


    // Open the register pipe
    publisher->server_fd = open(register_name, O_WRONLY);
    if(publisher->server_fd < 0) {
        printf("Error opening register pipe for writing\n");
        close(publisher->server_fd);
        return -1;
    }

    strcpy(publisher->message_box,message_box);

    char request[MAX_REQUEST_SIZE];
    build_request(1, pipe_name, message_box, request);

    if(write(publisher->server_fd,request,sizeof(request)) < 0) {
        printf("Error writing in the register pipe\n");
        return -1;
    }

    //make the fifo
    if(mkfifo(pipe_name, 0666) < 0) {
        printf("Error creating the fifo\n");
        return -1;
    }

    publisher->pipe_fd = open(pipe_name, O_WRONLY);
    if(publisher->pipe_fd < 0) {
        printf("Error opening named pipe for writing\n");
        return -1;
    }


    return 0;
}

int publish(publisher_t *publisher, char message[MAX_MESSAGE_SIZE]) {
    //create a message and send it to the pipe
    char buffer[MAX_MESSAGE_SIZE];
    build_message(9, message, buffer);
    if(write(publisher->pipe_fd,buffer,sizeof(buffer)) < 0) {
        printf("Error writing in the pipe\n");
        return -1;
    }

    return 0;
}

int wait_message(char message[MAX_MESSAGE_SIZE]) {
    //wait for a message from the user
    printf("|\n");
    fgets(message, MAX_MESSAGE_SIZE, stdin);
    // limitate the message with a \0
    message[strlen(message)-1] = '\0';

    return 0;
}

int disconnect(publisher_t *publisher) { return close(publisher->pipe_fd); }


int main(int argc, char **argv) {

    printf(".\n");

    signal(SIGINT, INT_handler);

    // check the number of arguments
    if(argc != 4) {
        printf("Wrong number of arguments\n");
        exit(1);
    }

    publisher_t publisher;

    //get the arguments from the command

    char register_name[MAX_PIPE_NAME];
    char pipe_name[MAX_PIPE_NAME];
    char message_box[MAX_BOX_NAME];

    strcpy(register_name, argv[1]);
    strcpy(pipe_name, argv[2]);
    strcpy(global_pipe_name, argv[2]);
    strcpy(message_box, argv[3]);

    printf(".\n");

    if(connect(&publisher, register_name, pipe_name, message_box) < 0) {
        printf("Error connecting to the server\n");
        return -1;
    }

    printf(".\n");

    char message[MAX_MESSAGE_SIZE];

    while(true) {

        wait_message(message);


        if(publish(&publisher, message) < 0) {
            printf("Error publishing the message\n");
            break;
        }

        // clear message
        memset(message, 0, MAX_MESSAGE_SIZE);
    }
    

    return 0;
}
