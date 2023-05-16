#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "fs.h"

enum command command_interpreter(int return_code, int executable_available);
int command_executer(enum command cmd);

int main(int argc, char* argv[]) {

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    enum command cmd;
    int err_code = 0;

    init_fs();

    while (1) {
        cmd = command_interpreter(err_code, check_executable());
        err_code = command_executer(cmd);
    }

    return 0;
}

enum command command_interpreter(int return_code, int executable_available) {
    int ok = 0;
    char buffer[50];
    while (ok == 0) {
        ok = 1;
        if (return_code == 0)
            printf("\033[1;32mroot@m0leCon\033[0m:# ");
        else
            printf("\033[1;31mroot@m0leCon\033[0m:# "); 
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "ls") == 0) {
            return cmd_ls;
        } else if (strcmp(buffer, "reset") == 0) {
            return cmd_reset;
        } else if (strcmp(buffer, "touch") == 0) {
            return cmd_touch;
        } else if (strcmp(buffer, "cp") == 0) {
            return cmd_cp;
        } else if (strcmp(buffer, "ln") == 0) {
            return cmd_ln;
        } else if (strcmp(buffer, "rm") == 0) {
            return cmd_rm;
        } else if (executable_available == 1 && strcmp(buffer, "m0lecat") == 0) {
            return cmd_m0lecat;
        } else if (strcmp(buffer, "help") == 0) {
            return cmd_help;
        } else if (strcmp(buffer, "exit") == 0) {
            return cmd_exit;
        } else {
            printf("Unknown command. Please use the command help to see which commands are available.\n");
            ok = 0;
            return_code = 1;
        }
    }
}

int command_executer(enum command cmd) {
    char buffer1[25], buffer2[25];
    switch (cmd) {
        case cmd_ls:
            return ls();
        case cmd_reset:
            return reset_executable();
        case cmd_touch:
            printf("Filename: ");
            read(STDIN_FILENO, buffer1, 25);
            buffer1[strcspn(buffer1, "\n")] = 0;
            return touch(buffer1);
        case cmd_cp:
            printf("Filename from: ");
            read(STDIN_FILENO, buffer1, 25);
            buffer1[strcspn(buffer1, "\n")] = 0;
            printf("Filename to: ");
            read(STDIN_FILENO, buffer2, 25);
            buffer2[strcspn(buffer2, "\n")] = 0;
            return cp(buffer1, buffer2);
        case cmd_ln:
            printf("Filename from: ");
            read(STDIN_FILENO, buffer1, 25);
            buffer1[strcspn(buffer1, "\n")] = 0;
            printf("Filename to: ");
            read(STDIN_FILENO, buffer2, 25);
            buffer2[strcspn(buffer2, "\n")] = 0;
            return ln(buffer1, buffer2);
        case cmd_rm:
            printf("Filename: ");
            read(STDIN_FILENO, buffer1, 25);
            buffer1[strcspn(buffer1, "\n")] = 0;
            return rm(buffer1);
        case cmd_m0lecat:
            return run_executable();
        case cmd_help:
            return help();
        case cmd_exit:
            exit(0);
        default:
            return 1;
    }
}