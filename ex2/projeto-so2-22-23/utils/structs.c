#include "structs.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void build_message(uint8_t code, const char *message, char* buffer) {
    // use memcpy to copy the paramaters to the buffer
    memcpy(buffer, &code, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t), message, sizeof(message)+ 1); 
    return ;
}

void build_request(uint8_t code, const char *pipe_name, const char *message_box, char* buffer) {
    // use memcpy to copy the paramaters to the buffer
    memcpy(buffer, &code, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t), pipe_name, sizeof(pipe_name) + 1);
    memcpy(buffer + sizeof(uint8_t) + sizeof(pipe_name) + 1, message_box, sizeof(message_box) + 1);

    return;    
}

void build_answer_to_list(uint8_t code, uint8_t last, const char *box_name, uint64_t box_size, uint64_t n_pubs, uint64_t n_subs, char *buffer) {
    // use memcpy to copy the paramaters to the buffer
    memcpy(buffer, &code, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t), &last, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t) + sizeof(uint8_t), box_name, sizeof(box_name) + 1);
    memcpy(buffer + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(box_name) + 1, &box_size, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(box_name) + 1 + sizeof(uint64_t), &n_pubs, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(box_name) + 1 + sizeof(uint64_t) + sizeof(uint64_t), &n_subs, sizeof(uint64_t));

    return;
}

void build_answer_to_box(uint8_t code, int32_t return_code, const char *error_message, char* buffer) {
    // use memcpy to copy the paramaters to the buffer
    memcpy(buffer, &code, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t), &return_code, sizeof(int32_t));
    memcpy(buffer + sizeof(uint8_t) + sizeof(int32_t), error_message, sizeof(error_message) + 1);

    return;
}
