#include "structs.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//request_t* set_request(int code, char message_box[32], char pipe_name[256]) {
//    request_t *request = NULL;
//    request->code = (uint8_t)code;
//    if (realpath(pipe_name, request->pipe_path) == NULL) { return NULL; }
//    strcpy(request->box_name, message_box);
//
//    return request;
//}
//
void build_message(uint8_t code, char message[1024], char* buffer) {
    // use memcpy to copy the paramaters to the buffer
    memcpy(buffer, &code, sizeof(int)); 
    memcpy(buffer + sizeof(int), message, strlen(message));

    return ;
}

void build_request(uint8_t code, char pipe_name[256], char message_box[32], char* buffer) {
    // use memcpy to copy the paramaters to the buffer
    memcpy(buffer, &code, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t), pipe_name, strlen(pipe_name) + 1);
    memcpy(buffer + sizeof(uint8_t) + strlen(pipe_name) + 1, message_box, strlen(message_box) + 1);

    return;    
}

void build_answer_to_list(uint8_t code, uint8_t last, char box_name[32], uint64_t box_size, uint64_t n_pubs, uint64_t n_subs, char *buffer) {
    // use memcpy to copy the paramaters to the buffer
    memcpy(buffer, &code, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t), &last, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t) + sizeof(uint8_t), box_name, strlen(box_name) + 1);
    memcpy(buffer + sizeof(uint8_t) + sizeof(uint8_t) + strlen(box_name) + 1, &box_size, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint8_t) + sizeof(uint8_t) + strlen(box_name) + 1 + sizeof(uint64_t), &n_pubs, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint8_t) + sizeof(uint8_t) + strlen(box_name) + 1 + sizeof(uint64_t) + sizeof(uint64_t), &n_subs, sizeof(uint64_t));

    return;
}

void build_answer_to_box(uint8_t code, int32_t return_code, const char *error_message, char* buffer) {
    // use memcpy to copy the paramaters to the buffer
    memcpy(buffer, &code, sizeof(uint8_t));
    memcpy(buffer + sizeof(uint8_t), &return_code, sizeof(int32_t));
    memcpy(buffer + sizeof(uint8_t) + sizeof(int32_t), error_message, strlen(error_message) + 1);

    return;
}
