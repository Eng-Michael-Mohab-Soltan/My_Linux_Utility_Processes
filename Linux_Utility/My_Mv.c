#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0) {
        printf("Error: Couldn't open source file '%s'\n", argv[1]);
        return 1;
    }

    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        printf("Error: Couldn't create destination file '%s'\n", argv[2]);
        close(src_fd);
        return 1;
    }

    char buf[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buf, sizeof(buf))) > 0) {
        ssize_t bytes_written = write(dest_fd, buf, bytes_read);
        if (bytes_written != bytes_read) {
            printf("Error: Failed to write to destination file\n");
            close(src_fd);
            close(dest_fd);
            return 1;
        }
    }

    if (bytes_read < 0) {
        printf("Error: Failed to read from source file\n");
        close(src_fd);
        close(dest_fd);
        return 1;
    }

    close(src_fd);
    close(dest_fd);

    if (remove(argv[1]) != 0) {
        printf("Error: Couldn't delete source file '%s'\n", argv[1]);
        return 1;
    }

    printf("File moved successfully\n");
    return 0;
}
