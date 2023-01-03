#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define MAX_REQUEST_SIZE 296

typedef struct {
    uint8_t code;
    char pipe_path[256];
    char box_name[32];
} request_t;


// list the boxes answer
// [ code = 8 (uint8_t) ] | [ last (uint8_t) ] |
// [ box_name (char[32]) ] | [ box_size (uint64_t) ] | [ n_publishers (uint64_t) ] | [ n_subscribers (uint64_t) ]
typedef struct {
    uint8_t code;
    uint8_t last;
    char box_name[32];
    uint64_t box_size;
    uint64_t n_pubs;
    uint64_t n_subs;
} answer_to_list_t;

typedef struct {
    uint8_t code;
    int32_t return_code;
    char error_message[1024];
} answer_to_box_t;

typedef struct {
    int pipe_fd; //file descriptor for the name pipe
    int server_fd; // file descriptor for the register pipe
} manager_t;

typedef struct {
    int pipe_fd; //file descriptor for the name pipe
    int server_fd; // file descriptor for the register pipe
    char *message_box; //name of the message box
} publisher_t;

typedef struct {
    int pipe_fd; //file descriptor for the name pipe
    int server_fd; // file descriptor for the register pipe
    char *message_box; //name of the message box
} subscriber_t;

void build_request(uint8_t code, const char *message_box, const char *pipe_name, char *buffer);

void build_answer_to_list(uint8_t code, uint8_t last, const char *box_name, uint64_t box_size, uint64_t n_pubs, uint64_t n_subs, char *buffer);

void build_answer_to_box(uint8_t code, int32_t return_code, const char *error_message, char *buffer);

void build_message(uint8_t code, const char *message, char *buffer);

#endif