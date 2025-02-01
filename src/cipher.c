#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"

void file_read(char* f_name, int* code);
void file_in(char* in);
void file_write(char* f_name, char* data);
void dir_in(char* d_name);
void encrypt_files(const char* directory);
void caesar_cipher(char* f_data, int key);
void caesar_key(int* key, int* code);
void des_encrypt(char* directory);

int main(void) {
    int input = 0;
    char f_name[50] = {0};
    char dir[100] = {0};
    char data[100] = {0};
    int flag = 1;
    int* code = &flag;
    while (scanf("%1d", &input)) {
        switch (input) {
            case 1:
                file_in(f_name);
                file_read(f_name, code);
                printf("\n");
                break;
            case 2:
                if (flag == 0) {
                    scanf("%*c");
                    fgets(data, 100, stdin);
                    file_write(f_name, data);
                    file_read(f_name, code);
                    printf("\n");
                } else
                    printf("n/a\n");
                break;
            case 3:
                scanf("%99s", dir);
                encrypt_files(dir);
                printf("\n");
                break;
            case 4:
                scanf("%99s", dir);
                des_encrypt(dir);
                printf("\n");
                break;
            case -1:
                break;
            default:
                printf("n/a\n");
        }
        if (input == -1) break;
    }
    return 0;
}

void file_read(char* f_name, int* code) {
    int f = 0;

#ifdef LOGGER
    FILE* log_file = log_init("cipher.log");
    if (log_file == NULL) {
        logcat(log_file, "READ FILE: ERROR LOG", ERROR);
        f = 1;
    }
#endif

    FILE* fp = fopen(f_name, "r");
    if (fp == NULL) {
        printf("n/a");
        f = 1;
        *code = 1;

#ifdef LOGGER
        logcat(log_file, "READ FILE: File not found", ERROR);
#endif
    }
    if (f != 1) {
        int count = 0;
        int c = 0;
        while ((c = fgetc(fp)) != EOF) {
            putchar(c);
            count++;
        }
        *code = 0;
        if (c == EOF && count == 0) printf("n/a");
#ifdef LOGGER
        logcat(log_file, "READ FILE: Read successfully", INFO);
#endif
    }
    if (*code != 1) {
        fclose(fp);

#ifdef LOGGER
        log_close(log_file);
#endif
    }
}

void file_in(char* in) { scanf("%50s", in); }

void file_write(char* f_name, char* data) {
#ifdef LOGGER
    FILE* log_file = log_init("cipher.log");
    if (log_file == NULL) {
        logcat(log_file, "READ FILE: ERROR LOG", ERROR);
    }

#endif

    if (*f_name != 0) {
        FILE* fp = fopen(f_name, "a+");
        fputs(data, fp);
        fclose(fp);
#ifdef LOGGER
        logcat(log_file, "APPEND FILE: Append successfully", INFO);
#endif
    } else {
        printf("n/a");
#ifdef LOGGER
        logcat(log_file, "APPEND FILE: File not found", ERROR);
#endif
    }
#ifdef LOGGER
    log_close(log_file);
#endif
}

void encrypt_files(const char* directory) {
    int code = 0;
    int* codeptr = &code;
    int c = 0;
    int* key = &c;
    DIR* dir = opendir(directory);

#ifdef LOGGER
    FILE* log_file = log_init("cipher.log");
    if (log_file == NULL) {
        logcat(log_file, "CAESAR ENCRYPT FILE: ERROR LOG", ERROR);
    }
#endif

    if (dir == NULL) {
        code = 1;
#ifdef LOGGER
        logcat(log_file, "ENCRYPTING FILES IN DIRECTORY: Failed to open directory", ERROR);
#endif
    }
    if (!code) {
        caesar_key(key, codeptr);
    }
    if (!code) {
        struct dirent* ent;

        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] == '.') continue;

            char file_path[2000];
            snprintf(file_path, 1500, "%s/%s", directory, ent->d_name);

            if (strstr(ent->d_name, ".c") != NULL) {
                FILE* file = fopen(file_path, "r+");
                if (file != NULL) {
                    fseek(file, 0, SEEK_END);
                    long file_size = ftell(file);
                    fseek(file, 0, SEEK_SET);

                    char* buffer = malloc(file_size + 1);
                    fread(buffer, 1, file_size, file);
                    buffer[file_size] = '\0';

                    caesar_cipher(buffer, c);

                    fseek(file, 0, SEEK_SET);
                    fwrite(buffer, 1, file_size, file);
                    fclose(file);
#ifdef LOGGER
                    logcat(log_file, "ENCRYPTING FILES IN DIRECTORY: Encrypted .c file", INFO);
#endif

                    free(buffer);
                }
            } else if (strstr(ent->d_name, ".h") != NULL) {
                FILE* file = fopen(file_path, "w");
                if (file != NULL) {
                    fclose(file);
#ifdef LOGGER
                    logcat(log_file, "ENCRYPTING FILES IN DIRECTORY: Cleared .h file", INFO);
#endif
                }
            }
        }
        closedir(dir);
    } else
        printf("n/a");
}

void caesar_key(int* key, int* code) {
#ifdef LOGGER
    FILE* log_file = log_init("cipher.log");
    if (log_file == NULL) {
        logcat(log_file, "CAESAR ENCRYPT FILE: ERROR LOG", ERROR);
    }
#endif

    if (scanf("%d", key) != 1 || getchar() != '\n') {
#ifdef LOGGER
        logcat(log_file, "ENCRYPTING FILES IN DIRECTORY: Invalid shift value", ERROR);
#endif
        *code = 1;
    }
#ifdef LOGGER
    log_close(log_file);
#endif
}

void caesar_cipher(char* f_data, int key) {
    for (int i = 0; f_data[i] != '\0'; i++) {
        if (isalpha(f_data[i])) {
            char base = islower(f_data[i]) ? 'a' : 'A';
            f_data[i] = (f_data[i] - base + key) % 26 + base;
        }
    }
}

void des_encrypt(char* directory) {
#ifdef LOGGER
    FILE* log_file = log_init("cipher.log");
    if (log_file == NULL) {
        logcat(log_file, "ENCRYPTING FILES IN DIRECTORY DEC: ERROR LOG", ERROR);
    }
#endif
    int code = 0;
    DIR* dir = opendir(directory);
    if (dir == NULL) {
#ifdef LOGGER
        logcat(log_file, "ENCRYPTING FILES IN DIRECTORY DEC: Failed to open directory", ERROR);
#endif
        code = 1;
    }
    if (!code) {
        struct dirent* ent;

        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] == '.') continue;
            char file_path[2000];
            snprintf(file_path, 1500, "%s/%s", directory, ent->d_name);
            if (strstr(ent->d_name, ".c") != NULL) {
                char command[5000];
                char* ssl1 = "openssl des-cbc -in ";
                char* ssl2 = " -out ";
                snprintf(command, 5000, "%s%s%s%s", ssl1, file_path, ssl2, file_path);
                system(command);
#ifdef LOGGER
                logcat(log_file, "ENCRYPTING FILES IN DIRECTORY DEC: Encrypted DEC .c file", INFO);
#endif
            } else if (strstr(ent->d_name, ".h") != NULL) {
                FILE* file = fopen(file_path, "w");
                if (file != NULL) {
                    fclose(file);
#ifdef LOGGER
                    logcat(log_file, "ENCRYPTING FILES IN DIRECTORY DEC: Cleared .h file", INFO);
#endif
                }
            }
        }
        closedir(dir);

    } else {
        printf("n/a");
    }
#ifdef LOGGER
    log_close(log_file);
#endif
}
