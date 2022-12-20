#include "fs/operations.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


uint8_t const file_contents[] = "AAA!";
char const target_path1[] = "/f1";
char const link_path1[] = "/l1";
char const target_path2[] = "/f2";
char const link_path2[] = "/l2";
char const target_path3[] = "/f3";
char const link_path3[] = "/l3";
char const link_path4[] = "/l4";

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
    assert(tfs_sym_link(target_path1, link_path1) != -1);
    assert_empty_file(link_path1);
    sleep(1);
    assert_contents_ok(target_path1);
    assert(tfs_unlink(link_path1) != -1);
    sleep(1);
    assert(tfs_link(target_path1, link_path1) == -1);

    return NULL;
}

void *thread2() {
    assert(tfs_sym_link(target_path1, link_path2) != -1);
    write_contents(link_path2);
    sleep(1);
    assert(tfs_unlink(link_path2) == -1);

    return NULL;
}

void *thread3() {
    assert(tfs_sym_link(link_path2, link_path3) != -1);
    assert_contents_ok(link_path3);
    assert(tfs_unlink(link_path2) != -1);
    assert(tfs_link(link_path2, link_path3) == -1);

    return NULL;
}

void *thread4() {
    assert(tfs_sym_link(target_path1,link_path3) != -1);
    sleep(1);
    assert(tfs_unlink(target_path1) != -1);
    assert(tfs_unlink(link_path3) != -1);
    assert(tfs_open(target_path1, TFS_O_APPEND) == -1);
    return NULL;
}

int main() {
    pthread_t tid[4];

    assert(tfs_init(NULL) != -1);

        // Write to symlink and read original file
    {
        int f = tfs_open(target_path1, TFS_O_CREAT);
        assert(f != -1);
        assert(tfs_close(f) != -1);

        assert_empty_file(target_path1); // sanity check
    }

    if (pthread_create(&tid[0], NULL, thread1, NULL) != 0) {
        assert(1 == -1); // error creating thread. ABORTS.
    }
    if (pthread_create(&tid[1], NULL, thread2, NULL) != 0) {
        assert(1 == -1); // error creating thread. ABORTS.
    }
    if (pthread_create(&tid[2], NULL, thread3, NULL) != 0) {
        assert(1 == -1); // error creating thread. ABORTS.
    }
    if(pthread_create(&tid[3], NULL, thread4, NULL) != 0) {
        assert(1 == -1); // error creating threads. ABORTS.
    }
    // join the threads
    for (int i = 0; i < 4; i++) {
        if (pthread_join(tid[i], NULL) != 0) {
            assert(1 == -1); // error joining threads. ABORTS.
        }
    }

    assert(tfs_destroy() != -1);
    printf("Successful test.\n");
    return 0;
}
