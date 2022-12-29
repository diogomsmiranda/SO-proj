#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint8_t code;
    char pipe_path[256];
    char box_name[32];
} request_t;

typedef struct {
    uint8_t code;
    char content[1024];
} message_t;

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

request_t* set_request(int code, const char *message_box, const char *pipe_name);
message_t* set_message(int code, const char *content);
#endif