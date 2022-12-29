#include "logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#define MAX_MESSAGE_SIZE 1024

// Structure to hold the state of a client session
typedef struct {
  int client_fd; // File descriptor for the connection to the client
  char client_pipe[256]; // Name of the client's named pipe
  char message_box[256]; // Name of the message box the client is subscribed to
  pthread_t thread; // Thread handle for the session thread
} session_t;

// Linked list node for storing published messages
typedef struct message_node {
  char *message; // Pointer to the message data
  size_t size; // Size of the message data
  struct message_node *next; // Pointer to the next node in the list
} message_node_t;

// Structure to hold the state of a message box
typedef struct {
  char name[256]; // Name of the message box
  message_node_t *messages; // Linked list of published messages
  pthread_mutex_t mutex; // Mutex to protect access to the message list
  pthread_cond_t cond; // Condition variable to signal new messages
} message_box_t;

// Function prototypes
int start_server(const char *server_pipe, int max_sessions);
void *session_thread(void *arg);
int publish_message(const char *message_box, const char *message, size_t size);
int add_message(message_box_t *box, const char *message, size_t size);
void delete_messages(message_node_t *node);

// Implementation of the start_server function
int start_server(const char *server_pipe, int max_sessions) {
  // Create the server's named pipe
  if (mkfifo(server_pipe, 0666) < 0 && errno != EEXIST) {
    perror("Error creating named pipe");
    return -1;
  }

  // Open the server's named pipe for reading
  int server_fd = open(server_pipe, O_RDONLY);
  if (server_fd < 0) {
    perror("Error opening named pipe for reading");
    return -1;
  }

  // Set up an array to hold the session state for each active session
  session_t sessions;

if (message_box == NULL) {
// Create a new message box
message_box_t *box = malloc(sizeof(message_box_t));
if (box == NULL) {
perror("Error allocating memory");
return -1;
}
strncpy(box->name, message_box, sizeof(box->name));
box->messages = NULL;
pthread_mutex_init(&box->mutex, NULL);
pthread_cond_init(&box->cond, NULL);
message_box = box;
HASH_ADD_STR(message_boxes, name, message_box);
}

// Add the message to the message box's message list
if (add_message(message_box, message, size) < 0) {
return -1;
}

// Notify all subscribers of the new message
pthread_mutex_lock(&message_box->mutex);
pthread_cond_broadcast(&message_box->cond);
pthread_mutex_unlock(&message_box->mutex);

return 0;
}

// Implementation of the add_message function
int add_message(message_box_t *box, const char *message, size_t size) {
// Allocate a new message node
message_node_t *node = malloc(sizeof(message_node_t));
if (node == NULL) {
perror("Error allocating memory");
return -1;
}

// Allocate memory for the message data
node->message = malloc(size);
if (node->message == NULL) {
perror("Error allocating memory");
free(node);
return -1;
}

// Copy the message data into the new node
memcpy(node->message, message, size);
node->size = size;
node->next = NULL;

// Add the new node to the message box's message list
pthread_mutex_lock(&box->mutex);
message_node_t *tail = box->messages;
if (tail == NULL) {
box->messages = node;
} else {
while (tail->next != NULL) {
tail = tail->next;

// Open the client's named pipe for writing
int client_fd = open(client_pipe, O_WRONLY);
if (client_fd < 0) {
perror("Error opening named pipe for writing");
return -1;
}

// Write the message to the client's named pipe
if (write(client_fd, message, size) < 0) {
perror("Error writing to named pipe");
return -1;
}

close(client_fd);
return 0;
}

// Implementation of the delete_messages function
void delete_messages(message_node_t *node) {
if (node == NULL) {
return;
}
delete_messages(node->next);
free(node->message);
free(node);
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    fprintf(stderr, "usage: mbroker <pipename>\n");
    WARN("unimplemented"); // TODO: implement
    return -1;
}
