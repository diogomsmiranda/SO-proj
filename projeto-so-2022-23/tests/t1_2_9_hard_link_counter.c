#include "fs/operations.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char const target_path[] = "/f1";
char const link_path[] = "/l1";

int main() {
    assert(tfs_init(NULL) != -1);

    // Write to hardlink and read original file
    {
        int f = tfs_open(target_path, TFS_O_CREAT);
        assert(f != -1);
        assert(tfs_close(f) != -1);
    }

    // file is created (1 hard link exists)
    int num_links = tfs_link_counter(target_path);
    assert(num_links == 1);

    assert(tfs_link(target_path, link_path) != -1);

    // hardlink to file is created (+1 hardlink = 2)
    num_links = tfs_link_counter(target_path);
    assert(num_links == 2);

    // unlink the hard link and check number of hardlinks (-1 hardlink = 1)
    tfs_unlink(link_path);
    num_links = tfs_link_counter(target_path);
    assert(num_links == 1);

    // unlink the file (delete) and check number of hardlinks (-1 hardlink = 0)
    tfs_unlink(target_path);
    num_links = tfs_link_counter(target_path);
    assert(num_links == 0);


    printf("Successful test.\n");

    return 0;
}
