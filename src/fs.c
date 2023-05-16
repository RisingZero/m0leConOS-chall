#include "fs.h"

typedef struct file_s {
    int size;
    char *data;
} file_t;

typedef struct file_wrap_s {
    char name[25];
    file_t* file;
} file_wrap_t;

file_wrap_t files[10];
long last_rm = NULL;

void (*executable_func)(void *arg, int);

int init_fs() {
    for (int i = 0; i < 10; i++) {
        files[i].file = NULL;
    }
    reset_executable();
    mprotect(files[0].file->data, 4096, PROT_READ | PROT_EXEC);
    executable_func("Filesystem initialized, try me!\n", 32);
    mprotect(files[0].file->data, 4096, PROT_READ);

    return 0;
}

int ls() {
    for (int i = 0; i < 10; i++) {
        if (files[i].file == NULL) {
            continue;
        }
        printf("%s ", files[i].name);
    }
    printf("\n");

    return 0;
}

int touch(char *filename) {
    int cnt = 0, idx = -1;
    for (int i = 1; i < 10; i++) {
        if (files[i].file != NULL)
            cnt++;
        else {
            idx = i;
            break;
        }
    }

    if (cnt >= 9) {
        printf("Too many files, delete something first!\n");
        return 1;
    }

    files[idx].file = malloc(sizeof(file_t));
    if (files[idx].file == NULL)
        exit(1);

    if (last_rm == files[idx].file) {
        long tmp = files[idx].file;
        files[idx].file = malloc(sizeof(file_t));
        free(tmp);
    }

    strncpy(files[idx].name, filename, 25);
    files[idx].name[24] = '\0';
    files[idx].file->size = 0;
    
    files[idx].file->data = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (files[idx].file->data == MAP_FAILED)
        exit(1);

    last_rm = files[idx].file;

    printf("Created %s, give me some things to put inside it:\n> ", files[idx].name);
    files[idx].file->size = (int)read(STDIN_FILENO, files[idx].file->data, 4096);
    printf("Ok your file has been created!\n");

    return 0;
}

int rm(char *filename) {
    int idx = -1;

    for (int i = 1; i < 10; i++) {
        if (files[i].file == NULL) {
            continue;
        }
        if (strcmp(files[i].name, filename) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("Mhh, can't seem to find your file :C\n");
        return 1;
    }

    if (last_rm == files[idx].file)
       return 1;

    munmap(files[idx].file->data, 4096);
    free(files[idx].file);
    last_rm = files[idx].file;
    files[idx].file = NULL;

    printf("File deleted!\n");
    
    return 0;
}

int cp(char *src, char *dest) {
    int idxold = -1;

    for (int i = 0; i < 10; i++) {
        if (files[i].file == NULL) {
            continue;
        }
        if (strcmp(files[i].name, src) == 0) {
            idxold = i;
            break;
        }
    }

    if (idxold == -1) {
        printf("Mhh, can't seem to find your file :C\n");
        return 1;
    }

    int cnt = 0, idxnew = -1;
    for (int i = 1; i < 10; i++) {
        if (files[i].file != NULL)
            cnt++;
        else {
            idxnew = i;
            break;
        }
    }

    if (cnt >= 9) {
        printf("Too many files, delete something first!\n");
        return 1;
    }

    files[idxnew].file = malloc(sizeof(file_t));
    if (files[idxnew].file == NULL)
        exit(1);

    if (last_rm == files[idxnew].file) {
        long tmp = files[idxnew].file;
        files[idxnew].file = malloc(sizeof(file_t));
        free(tmp);
    }

    strncpy(files[idxnew].name, dest, 25);
    files[idxnew].name[24] = '\0';
    files[idxnew].file->size = files[idxold].file->size;
    
    files[idxnew].file->data = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (files[idxnew].file->data == MAP_FAILED)
        exit(1);

    memcpy(files[idxnew].file->data, files[idxold].file->data, files[idxold].file->size);
    
    printf("Ok your file has been copied!\n");

    return 0;
}

int ln(char *src, char *dest) {
    int idxold = -1;

    for (int i = 0; i < 10; i++) {
        if (files[i].file == NULL) {
            continue;
        }
        if (strcmp(files[i].name, src) == 0) {
            idxold = i;
            break;
        }
    }

    if (idxold == -1) {
        printf("Mhh, can't seem to find your file :C\n");
        return 1;
    }

    int cnt = 0, idxnew = -1;
    for (int i = 1; i < 10; i++) {
        if (files[i].file != NULL)
            cnt++;
        else {
            idxnew = i;
            break;
        }
    }

    if (cnt >= 9) {
        printf("Too many files, delete something first!\n");
        return 1;
    }

    strncpy(files[idxnew].name, dest, 25);
    files[idxnew].name[24] = '\0';
    files[idxnew].file = files[idxold].file;

    return 0;
}

int reset_executable() {
    files[0].file = malloc(sizeof(file_t));
    if (files[0].file == NULL)
        exit(1);

    files[0].file->data = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (files[0].file->data == MAP_FAILED)
        exit(1);

    strncpy(files[0].name, "m0lecat", 25);
    
    int fd = open("./m0lecat", O_RDONLY);
    if (fd == -1)
        exit(1);
    files[0].file->size = (int)read(fd, (void *)files[0].file->data, 4096);
    mprotect(files[0].file->data, 4096, PROT_READ);
    executable_func = (void (*)(void *, int))files[0].file->data;
    close(fd);

    return 0;
}

int check_executable() {
    if (files[0].file != NULL && files[0].file->data != NULL && executable_func != NULL)
        return 1;
    return 0;
}

int run_executable() {
    char arg[50];
    printf("Give $1 for the executable to run\n> ");
    read(STDIN_FILENO, arg, 50);
    arg[strcspn(arg, "\n")] = 0;
    getchar();

    int idx = -1;

    for (int i = 0; i < 10; i++) {
        if (files[i].file == NULL) {
            continue;
        }
        if (strcmp(files[i].name, arg) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        file_t real_file;
        real_file.data = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
        if (real_file.data == MAP_FAILED)
            exit(1);

        int rfd = open(arg, O_RDONLY);
        if (rfd == -1) { 
            printf("Mhh, can't seem to find your file :C\n");
            return 1;
        }
        real_file.size = (int)read(rfd, (void *)real_file.data, 4096);
        close(rfd);

        mprotect(executable_func, 4096, PROT_READ | PROT_EXEC);
        if (strstr(real_file.data, "ptm{") != NULL) {
            executable_func("Nice try.\n", 10);
            return 1;
        }
        executable_func(real_file.data, real_file.size);

        munmap(real_file.data, 4096);
    } else {
        mprotect(executable_func, 4096, PROT_READ | PROT_EXEC);
        executable_func((void *)files[idx].file->data, files[idx].file->size);
    }
    mprotect(executable_func, 4096, PROT_READ);
    printf("\n");
    return 0;
}

int help() {
    printf("Available commands:\nls, reset, touch, cp, rm, help, ");
    if (check_executable())
        printf("m0lecat, ");
        
    printf("exit\n");

    return 0;
}