#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        printf("Couldn't open source file!\n");
        return 1;
    }

    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd2 < 0) {
        printf("Couldn't open destination file!\n");
        close(fd1);
        return 1;
    }

    char text[1024];
    int bytes;
    while (bytes = read(fd1, text, sizeof(text)) > 0) {
        write(fd2, text, bytes);
    }

    close(fd1);
    close(fd2);
    printf("Copy done!\n");
    return 0;
}

