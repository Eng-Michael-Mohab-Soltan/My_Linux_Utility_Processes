#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("use it like this: %s <source> <destination>\n", argv[0]);
        return 0;
    }

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0) {
        printf("couldn't open source file\n");
        return 0;
    }

    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        printf("couldn't create destination file\n");
        close(src_fd);
        return 0;
    }

    char buf[1024];
    int bytes;
    while ((bytes = read(src_fd, buf, sizeof(buf))) > 0) {
        write(dest_fd, buf, bytes);
    }

    close(src_fd);
    close(dest_fd);

    if (remove(argv[1]) != 0) {
        printf("couldn't delete source file\n");
    }

    printf("file moved\n");
    return 0;
}

