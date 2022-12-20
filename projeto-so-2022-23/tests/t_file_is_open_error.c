#include "fs/operations.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>

char *str_ext_file = "BBB!";
char *path = "/f1";

int main() {
    assert(tfs_init(NULL) != -1);
    int fd = tfs_open(path, TFS_O_CREAT);
    assert(fd != -1);
    assert(tfs_unlink(path) == -1);
    assert(tfs_close(fd) != -1);
    assert(tfs_unlink(path) != -1);

    assert(tfs_destroy(NULL) != -1);

    printf("Successful test.\n");
    return 0;
}