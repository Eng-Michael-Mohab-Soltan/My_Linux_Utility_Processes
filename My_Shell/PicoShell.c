#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 2048
const char* prompt = "mosh";

int main() {
    char input[BUFFER_SIZE];
    
    while (1) {
        printf("%s > ", prompt);
        fflush(stdout);
        
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        if (*input == '\0') {
            continue;
        }
        
        if (strncmp(input, "echo ", 5) == 0) {
            printf("%s\n", input + 5);
        } else if (strcmp(input, "exit") == 0) {
            printf("Exiting...\n");
            break;
        } else if (strncmp(input, "cd ", 3) == 0) {
            char *directory = input + 3;
            if (chdir(directory) == -1) {
                perror("cd error");
            }
        } else if (strcmp(input, "pwd") == 0) {
            char cwd[BUFFER_SIZE];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            } else {
                perror("getcwd error");
            }
        } else {
            pid_t child_pid = fork();
            if (child_pid == -1) {
                perror("Fork failed");
                continue;
            } else if (child_pid == 0) {
                char *args[BUFFER_SIZE / 2];
                char *token = strtok(input, " ");
                int index = 0;
                
                while (token != NULL) {
                    args[index++] = token;
                    token = strtok(NULL, " ");
                }
                args[index] = NULL;
                
                if (execvp(args[0], args) == -1) {
                    perror("Command execution failed");
                    exit(EXIT_FAILURE);
                }
            }
            
            int status;
            if (waitpid(child_pid, &status, 0) == -1) {
                perror("waitpid error");
            }
        }
    }
    return 0;
}
