#include "structs.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void build_message(uint8_t code, char message[MAX_MESSAGE_SIZE], char* buffer) {
    // use memcpy to copy the paramaters to the buffer
    memcpy(buffer, &code, sizeof(u_int8_t)); 
    memcpy(buffer + sizeof(u_int8_t), message, MAX_MESSAGE_SIZE);

    return ;
}

void build_request(uint8_t code, char pipe_name[MAX_PIPE_NAME], char message_box[MAX_BOX_NAME], char* buffer) {
    // use memcpy to copy the paramaters to the buffer
    memcpy(buffer, &code, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t), pipe_name, MAX_PIPE_NAME);
    memcpy(buffer + sizeof(uint8_t) + MAX_PIPE_NAME, message_box, MAX_BOX_NAME);

    return;    
}

void build_answer_to_list(uint8_t code, uint8_t last, char box_name[MAX_BOX_NAME], uint64_t box_size, uint64_t n_pubs, uint64_t n_subs, char *buffer) {
    // use memcpy to copy the paramaters to the buffer
    memcpy(buffer, &code, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t), &last, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t) + sizeof(uint8_t), box_name, MAX_BOX_NAME);
    memcpy(buffer + sizeof(uint8_t) + sizeof(uint8_t) + MAX_BOX_NAME, &box_size, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint8_t) + sizeof(uint8_t) + MAX_BOX_NAME + sizeof(uint64_t), &n_pubs, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint8_t) + sizeof(uint8_t) + MAX_BOX_NAME + sizeof(uint64_t) + sizeof(uint64_t), &n_subs, sizeof(uint64_t));

    return;
}

void build_answer_to_box(uint8_t code, int32_t return_code, const char *error_message, char* buffer) {
    // use memcpy to copy the paramaters to the buffer
    memcpy(buffer, &code, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t), &return_code, sizeof(int32_t));
    memcpy(buffer + sizeof(uint8_t) + sizeof(int32_t), error_message, strlen(error_message) + 1);

    return;
}
