#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define MAX_REQUEST_SIZE 296
#define MAX_MESSAGE_SIZE 1024
#define MAX_ANSWER_SIZE 1064
#define MAX_BOX_NAME 32
#define MAX_PIPE_NAME 256
#define MAX_BUFFER_MESSAGE 1032

typedef struct {
    int pipe_fd; //file descriptor for the name pipe
    int server_fd; // file descriptor for the register pipe
} manager_t;

typedef struct {
    int pipe_fd; //file descriptor for the name pipe
    int server_fd; // file descriptor for the register pipe
    char message_box[MAX_BOX_NAME]; //name of the message box
} publisher_t;

typedef struct {
    int pipe_fd; //file descriptor for the name pipe
    int server_fd; // file descriptor for the register pipe
    char message_box[MAX_BOX_NAME]; //name of the message box
} subscriber_t;

char global_pipe_name[MAX_PIPE_NAME];


void INT_handler(int signum);

void build_request(uint8_t code, char pipe_name[MAX_PIPE_NAME], char message_box[MAX_BOX_NAME], char *buffer);

void build_answer_to_list(uint8_t code, uint8_t last, char box_name[MAX_BOX_NAME], uint64_t box_size, uint64_t n_pubs, uint64_t n_subs, char *buffer);

void build_answer_to_box(uint8_t code, int32_t return_code, const char *error_message, char *buffer);

void build_message(uint8_t code, char message[MAX_MESSAGE_SIZE], char *buffer);

#endif