#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
const char* shell_prompt = "mosh";

int main() {
    char buffer[BUFFER_SIZE];
    
    while (1) {
        printf("%s > ", shell_prompt);
        fflush(stdout);

        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        if (*buffer == '\0') {
            continue;
        }

        // Handle "export" command
        if (strncmp(buffer, "export ", 7) == 0) {
            char *name = buffer + 7;
            char *value = strchr(name, '=');
            if (value) {
                *value = '\0';
                value++;
                if (setenv(name, value, 1) == -1) {
                    perror("setenv");
                }
            } else {
                fprintf(stderr, "Invalid export format. Use name=value.\n");
            }
            continue;
        }

        // Handle variable assignment
        if (strchr(buffer, '=')) {
            char *name = strtok(buffer, "=");
            char *value = strtok(NULL, "");
            if (name && value) {
                if (setenv(name, value, 1) == -1) {
                    perror("setenv");
                }
            } else {
                fprintf(stderr, "Invalid assignment format. Use name=value.\n");
            }
            continue;
        }

        // Handle environment variable expansion
        char expanded[BUFFER_SIZE] = "";
        char *start = buffer;
        char *dollar;

        while ((dollar = strchr(start, '$')) != NULL) {
            strncat(expanded, start, dollar - start);
            char *name = dollar + 1;
            char *end = strpbrk(name, " \t");
            char var_name[BUFFER_SIZE];
            
            if (end) {
                strncpy(var_name, name, end - name);
                var_name[end - name] = '\0';
                start = end;
            } else {
                strcpy(var_name, name);
                start = dollar + strlen(dollar);
            }

            char *value = getenv(var_name);
            if (value) {
                strcat(expanded, value);
            } else {
                fprintf(stderr, "Variable %s not found.\n", var_name);
            }
        }
        strcat(expanded, start);

        // Fork and execute the command
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            char *args[BUFFER_SIZE / 2];
            char *token = strtok(expanded, " ");
            int i = 0;
            
            while (token) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }

        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
        }
    }
    return 0;
}
