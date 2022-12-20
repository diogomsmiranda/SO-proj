#include "fs/operations.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


uint8_t const file_contents[] = "AAA!";
char const target_path0[] = "/f0";
char const target_path1[] = "/f1";
char const target_path2[] = "/f2";
char const target_path3[] = "/f3";
char const target_path4[] = "/f4";

void assert_contents_ok(char const *path) {
    int f = tfs_open(path, 0);
    assert(f != -1);

    uint8_t buffer[sizeof(file_contents)];
    assert(tfs_read(f, buffer, sizeof(buffer)) == sizeof(buffer));
    assert(memcmp(buffer, file_contents, sizeof(buffer)) == 0);

    assert(tfs_close(f) != -1);
}

void assert_empty_file(char const *path) {
    int f = tfs_open(path, 0);
    assert(f != -1);

    uint8_t buffer[sizeof(file_contents)];
    assert(tfs_read(f, buffer, sizeof(buffer)) == 0);

    assert(tfs_close(f) != -1);
}

void write_contents(char const *path) {
    int f = tfs_open(path, 0);
    assert(f != -1);

    assert(tfs_write(f, file_contents, sizeof(file_contents)) ==
           sizeof(file_contents));

    assert(tfs_close(f) != -1);
}

void *thread1() {
    assert(tfs_copy_from_external_fs("tests/thread.txt", target_path0) != -1);
    assert(tfs_link(target_path0,target_path1) != -1);
    assert(tfs_unlink(target_path1) != -1);
    sleep(1);   // force thread 2 & 3 to finish writing so it can check if content was read.
    assert_contents_ok(target_path4);
    return NULL;
}

void *thread2() {
    assert(tfs_unlink(target_path1) == -1);
    assert(tfs_copy_from_external_fs("tests/nonexistent.txt", target_path2) == -1);
    assert(tfs_copy_from_external_fs("tests/thread.txt", target_path2) != -1);
    assert(tfs_sym_link(target_path2,target_path3) != -1);
    return NULL;
}

void *thread3() {
    assert(tfs_copy_from_external_fs("tests/thread.txt", target_path4) != -1);
    assert(tfs_link(target_path4,target_path1) != -1);
    write_contents(target_path1);
    return NULL;
}

int main() {
    pthread_t tid[3];

    assert(tfs_init(NULL) != -1);

    if (pthread_create(&tid[0], NULL, thread1, NULL) != 0) {
        assert(1 == -1);    // error in creating thread. ABORT.
    }
    if (pthread_create(&tid[1], NULL, thread2, NULL) != 0) {
        assert(1 == -1);    // error in creating thread. ABORT.
    }
    if (pthread_create(&tid[2], NULL, thread3, NULL) != 0) {
        assert(1 == -1);    // error in creating thread. ABORT.
    }
    // join the threads
    for (int i = 0; i < 3; i++) {
        if (pthread_join(tid[i], NULL) != 0) {
            assert(1 == -1);    // error in creating thread. ABORT.
        }
    }

    assert(tfs_destroy() != -1);

    printf("Successful test.\n");

    return 0;
}
