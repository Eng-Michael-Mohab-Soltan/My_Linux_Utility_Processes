#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    char work_dir[1024];
    
    if (getcwd(work_dir, sizeof(work_dir)) != 0) {
        printf("Current directory: %s\n", work_dir);
    } else {
        printf("getcwd");
    }

    return 0;
}

