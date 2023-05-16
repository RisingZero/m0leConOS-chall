#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

enum command {
    cmd_ls,
    cmd_reset,
    cmd_touch,
    cmd_cp,
    cmd_ln,
    cmd_rm,
    cmd_m0lecat,
    cmd_help,
    cmd_exit
};

int init_fs();
int ls();
int touch(char *filename);
int rm(char *filename);
int cp(char *src, char *dest);
int ln(char *src, char* dest);
int reset_executable();
int check_executable();
int run_executable();
int help();
