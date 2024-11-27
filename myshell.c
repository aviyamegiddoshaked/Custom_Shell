

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LENGTH 100 
#define MAX_COMMANDS 100   
// note to self: environment variable for system PATH
#define PATH "PATH"      

// Global variables for storing command history and process IDs
char* command_list[MAX_COMMANDS]; 
int command_count = 0; 
int process_history[MAX_COMMANDS]; 


void set_path_variable(int argc, char *argv[]) {
    // get current PATH variable
    char *currentPath = getenv(PATH); 
    for (int i = 1; i < argc; ++i) {
        strcat(currentPath, ":"); //  colon to separate paths
        strcat(currentPath, argv[i]); 
    }
    setenv(PATH, currentPath, 1); 
}


void add_command_to_history(const char* cmd) {
    if (command_count < MAX_COMMANDS) {
        // duplicate the command string and add it to the history
        command_list[command_count] = strdup(cmd);
        if (command_list[command_count] != NULL) {
            command_count++; 
        } else {
            // memory duplication error
            perror("strdup failed"); 
        }
    } else {
        fprintf(stderr, "History limit reached. Cannot store more commands.\n");
    }
}


void display_command_history() {
    for (int i = 0; i < command_count; i++) {
        printf("%s\n", command_list[i]); // Print each command
    }
}


void run_command(char* input) {
    //array to store command arguments
    char* args[MAX_COMMANDS];
    int args_count = 0; 

    // tokenize the input string into arguments
    args[args_count] = strtok(input, " ");
    while (args[args_count] != NULL) {
        args_count++;
        args[args_count] = strtok(NULL, " ");
    }

    // if no command is entered, return early
    if (args_count == 0) return;
    args[args_count] = NULL; 
    // handle built-in "cd" command
    if (strcmp(args[0], "cd") == 0) {
        if (args_count > 1) {
            if (chdir(args[1]) == -1) {
                perror("chdir failed"); // handle invalid directory
            } else {
                add_command_to_history("cd"); 
            }
        } else {
            fprintf(stderr, "cd: missing argument\n"); 
        }
        return;
    }

    // "history" command
    if (strcmp(args[0], "history") == 0) {
        add_command_to_history("history"); 
        display_command_history(); 
        return;
    }

    // "pwd" command
    if (strcmp(args[0], "pwd") == 0) {
        char cwd[MAX_LENGTH]; 
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd); 
            add_command_to_history("pwd");
        } else {
            perror("getcwd failed"); 
        }
        return;
    }

    // "exit" command
    if (strcmp(args[0], "exit") == 0) {
        exit(0); 
    }

    // for other commands : create a new process and execute the command
    pid_t pid = fork(); 
    if (pid < 0) {
        perror("fork failed"); 
    } else if (pid == 0) {
        // execute the command in child process
        if (execvp(args[0], args) == -1) {
            perror("exec failed"); 
            exit(1);
        }
    } else {
        // wait for child process to complete to complete parent process
        waitpid(pid, NULL, 0);
        add_command_to_history(input);
    }
}

int main(int argc, char *argv[]) {
    char* command_list[MAX_COMMANDS];
    set_path_variable(argc, argv);
    char input[MAX_LENGTH]; 
    while (1) {
        // for user input
        printf("$ "); // Prompt for user input
        fflush(stdout); 

        if (fgets(input, MAX_LENGTH, stdin) == NULL) {
            // handles error
            perror("fgets failed"); 
            continue;
        }

        input[strcspn(input, "\n")] = 0;

        run_command(input);
    }

    // free memory allocated for command history
    for (int i = 0; i < command_count; i++) {
        free(command_list[i]);
    }

    return 0;
}
