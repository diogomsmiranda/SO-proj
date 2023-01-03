#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define MAX_REQUEST_SIZE 296
#define MAX_MESSAGE_SIZE 1024
#define MAX_BOX_NAME 32

typedef struct {
    int pipe_fd; //file descriptor for the name pipe
    int server_fd; // file descriptor for the register pipe
} manager_t;

typedef struct {
    int pipe_fd; //file descriptor for the name pipe
    int server_fd; // file descriptor for the register pipe
    char message_box[32]; //name of the message box
} publisher_t;

typedef struct {
    int pipe_fd; //file descriptor for the name pipe
    int server_fd; // file descriptor for the register pipe
    char message_box[32]; //name of the message box
} subscriber_t;

void build_request(uint8_t code, char pipe_name[256], char message_box[32], char *buffer);

void build_answer_to_list(uint8_t code, uint8_t last, char box_name[32], uint64_t box_size, uint64_t n_pubs, uint64_t n_subs, char *buffer);

void build_answer_to_box(uint8_t code, int32_t return_code, const char *error_message, char *buffer);

void build_message(uint8_t code, char message[1024], char *buffer);

#endif