#include "logging.h"
#include "operations.h"
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
#include <signal.h>


// CODES
// 1 - register publisher
// 2 - register subscriber
// 3 - create message box
// 4 - answer to creating a box
// 5- request to delete a message box
// 6 - answer to deleting a message box
// 7 - request to list all boxes
// 8 - answer to list all boxes
// 9 - message from publisher to publish
// 10 - message from subscriber to receiv

char pub_pipe[MAX_PIPE_NAME];
char box1_name[MAX_BOX_NAME];
int sub_fd = 0;

int send_buffer(int fd, char buffer[], size_t size) {
    if(write(fd, buffer, size) < 0) {
        return -1;
    }
    return 0;
}

int start_server(char pipe_name[MAX_PIPE_NAME]/*, int max_sessions*/) {


    if(tfs_init(NULL)) {
        printf("Error initializing the tfs\n");
        exit(1);
    }

    strcpy(global_pipe_name, pipe_name);

    // create a pipe with pipe_name
    if(mkfifo(pipe_name, 0666) == -1) {
        printf("Error creating the fifo\n");
        exit(1);
    }


    // open the pipe
    int pipe_fd = open(pipe_name, O_RDONLY);
    if(pipe_fd < 0) {
        printf("Error opening named pipe for reading\n");
        exit(1);
    }

    return pipe_fd;
}

int w8_for_activity(int pipe_fd) {
    //wait for data to be available in register_pipe
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(pipe_fd, &read_set);
    int available_data = select(pipe_fd + 1, &read_set, NULL, NULL, NULL);
    if(available_data < 0) {
        printf("Error in select\n");
        exit(1);
    }

    return available_data;
}

void pub_req_handle(char request[MAX_REQUEST_SIZE]) {
    //get the pipe name
    char pipe_name[MAX_PIPE_NAME];
    memcpy(pipe_name, request + sizeof(uint8_t), MAX_PIPE_NAME);
    strcpy(pub_pipe, pipe_name);


    //get the box name
    char box_name[MAX_BOX_NAME];
    memcpy(box_name, request + sizeof(uint8_t) + MAX_PIPE_NAME, MAX_BOX_NAME);
    strcpy(box1_name, box_name);

    //open the pipe
    int pipe_fd = open(pipe_name, O_RDONLY);
    if(pipe_fd < 0) {
        printf("Error opening named pipe for reading\n");
        exit(1);
    }
}

void sub_req_handle(char request[MAX_REQUEST_SIZE]) {
    //get the pipe name
    char pipe_name[MAX_PIPE_NAME];
    memcpy(pipe_name, request + sizeof(uint8_t), MAX_PIPE_NAME);


    //get the box name
    char box_name[MAX_BOX_NAME];
    memcpy(box_name, request + sizeof(uint8_t) + MAX_PIPE_NAME, MAX_BOX_NAME);

    //open the box
    int box_fd = tfs_open(box_name, TFS_O_APPEND);
    if(box_fd < 0) {
        printf("Error opening the box\n");
        exit(1);
    }

    //open the pipe
    int pipe_fd = open(pipe_name, O_WRONLY);
    sub_fd = pipe_fd;
    if(pipe_fd < 0) {
        printf("Error opening named pipe for writing\n");
        exit(1);
    }
}

void publish(char request[MAX_REQUEST_SIZE]) {
    //get the message
    char message[MAX_MESSAGE_SIZE];
    memcpy(message, request + sizeof(uint8_t), MAX_MESSAGE_SIZE);

    //open the box
    int box_fd = tfs_open(box1_name, TFS_O_APPEND);
    if(box_fd < 0) {
        printf("Error opening the box\n");
        exit(1);
    }

    char buffer[MAX_BUFFER_MESSAGE];
    build_message(10, message, buffer);
    //send messsage trought the sub_pipe
    if(send_buffer(sub_fd, buffer, MAX_BUFFER_MESSAGE) < 0) {
        printf("Error sending the message to the subscriber\n");
        exit(1);
    }

    //write the message
    ssize_t bytes_written = tfs_write(box_fd, message, MAX_MESSAGE_SIZE);
    if(bytes_written < 0) {
        printf("Error writing to the box\n");
        exit(1);
    }

    tfs_close(box_fd);
}

void create_box(char request[MAX_REQUEST_SIZE]) {
    //get the box name
    char box_name[MAX_BOX_NAME];
    char pipe_name[MAX_PIPE_NAME];
    memcpy(pipe_name, request + sizeof(uint8_t), MAX_PIPE_NAME);
    memcpy(box_name, request + sizeof(uint8_t) + MAX_PIPE_NAME, MAX_BOX_NAME);

    //open the pipe if its not open already
    int pipe_fd = open(pipe_name, O_WRONLY);
    if(pipe_fd < 0) {
        printf("Error opening named pipe for writing\n");
        exit(1);
    }

    int box_fd = tfs_open(box_name, TFS_O_CREAT);
    tfs_close(box_fd);
    if(box_fd < 0) {
        //send answer to client
        char answer[MAX_ANSWER_SIZE];
        build_answer_to_box(4, -1, "Error creating the box", answer);
        if(send_buffer(pipe_fd, answer, MAX_ANSWER_SIZE) == -1) {
            printf("Error sending answer to client\n");
            exit(1);
        }
        tfs_close(box_fd);
    }

    char answer[MAX_ANSWER_SIZE];
    build_answer_to_box(4, 0, "\0", answer);
    send_buffer(pipe_fd, answer, MAX_ANSWER_SIZE);
}

void treat_request(char request[MAX_REQUEST_SIZE]) {
    //get the code of the request to chose next behaviour
    uint8_t code;
    memcpy(&code, request, sizeof(uint8_t));

    switch(code) {
        default:
            break;
        case 1:
            //register publisher
            pub_req_handle(request);
            printf("Done Processing Publisher\n");
            break;
        case 2:
            //register subscriber
            sub_req_handle(request);
            printf("Done Processing Subscriber\n");
            break;
        case 3:
            //create message box
            create_box(request);
            printf("Done Processing Create Box\n");
            break;
        case 5:
            //request to delete a message box
            break;
        case 7:
            //request to list all boxes
            break;
        case 9:
            //message from publisher to publish
            publish(request);
            printf("DOne Processing Publish\n");
            break;
    }   
}

//int main with args
//format: ./mbroker <pipe_name> <max_sessions>
int main(int argc, char **argv) {

    signal(SIGINT, INT_handler);
    // check the number of arguments
    if(argc != 3) {
        printf("Wrong number of arguments\n");
        exit(1);
    }

    int pipe_fd = start_server(argv[1]/*, atoi(argv[2])*/);

    while(true) {

        if(w8_for_activity(pipe_fd) > 0) {
            // read the request from the pipe
            char request[MAX_REQUEST_SIZE];
            if(read(pipe_fd, request, sizeof(request)) < 0) {
                printf("Error reading from the pipe\n");
                exit(1);
            }

            treat_request(request);
        }
    }

    return 0;
}