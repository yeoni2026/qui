#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define DATA_SIZE 128
#define MAX_TOKEN 10
typedef enum {SUCCESS, FILE_NOT_FOUND, FILE_READ_FAILED} res_code;

static void get_data_path(char *dest, size_t size);
res_code load_command(char *buffer, int index);
void change_command(char *buffer, int index);

int main(int argc, char *argv[]){ 
    if (argc == 1) {        
        // 사용법 가이드 출력
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s <num>          Execute shortcut\n", argv[0]);
        fprintf(stderr, "  %s <num> <cmd>    Register shortcut\n", argv[0]);
        
        char buffer[DATA_SIZE];

        fprintf(stderr, "\n[qui shortcuts]\n");
        for (int i = 0; i <= 9; ++i) {
            res_code rc = load_command(buffer, i);
            
            if (rc == FILE_NOT_FOUND) {
                for (int j = 0; j < 10; ++j){
                    fprintf(stderr, "  %d: (empty)\n", j);
                }
                break;
            } 
            else if (rc == FILE_READ_FAILED) {
                fprintf(stderr, "Error: Failed to read data from file.\n");
                exit(EXIT_FAILURE);
            } 
            else {
                buffer[DATA_SIZE - 1] = '\0';

                if (buffer[0] == '\0') {
                    fprintf(stderr, "  %d: (empty)\n", i);
                } else {
                    fprintf(stderr, "  %d: %s\n", i, buffer);
                }
            }
        }
        return 0;
    }
    else if (argc == 2){
        if (strlen(argv[1]) != 1 || !(argv[1][0] >= '0' && argv[1][0] <= '9')){
            fprintf(stderr, "Error: Invalid index '%s'. Must be a number between 0 and 9.\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        int index = (int)(strtol(argv[1], NULL, 10));
        char buffer[DATA_SIZE];
        
        res_code rc = load_command(buffer, index);
        if (rc == FILE_NOT_FOUND) {
            fprintf(stderr, "Error: Data file not found.\n");
            exit(EXIT_FAILURE);
        } else if (rc == FILE_READ_FAILED) {
            fprintf(stderr, "Error: Failed to read data from file.\n");
            exit(EXIT_FAILURE);
        }

        if (buffer[0] == '\0'){
            fprintf(stderr, "Shortcut %d is not set yet.\n", index);
            exit(EXIT_FAILURE);
        }

        fprintf(stderr, "%s\n", buffer);
        printf("%s", buffer);

        return 0;
    }
    else {
        if (strlen(argv[1]) != 1 || !(argv[1][0] >= '0' && argv[1][0] <= '9')){
            fprintf(stderr, "Error: Invalid index '%s'. Must be a number between 0 and 9.\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        int index = (int)(strtol(argv[1], NULL, 10));
        
        char buffer[DATA_SIZE];
        int buf_idx = 0;
        for (int i = 2; i < argc; ++i){
            int len = strlen(argv[i]);
            int has_space = (strchr(argv[i], ' ') != NULL) && (argc > 3);

            if (buf_idx + len + (has_space ? 2 : 0) + 2 >= DATA_SIZE) {
                fprintf(stderr, "Error: Command exceeds maximum length (%d bytes).\n", DATA_SIZE);
                exit(EXIT_FAILURE);
            }

            if (has_space) {
                buffer[buf_idx] = '"';
                ++buf_idx;
            }

            strcpy(buffer + buf_idx, argv[i]);
            buf_idx += len;

            if (has_space) {
                buffer[buf_idx] = '"';
                ++buf_idx;
            }

            if (i < argc - 1) {
                buffer[buf_idx] = ' ';
                ++buf_idx;
            }
        }
        buffer[buf_idx] = '\0';

        change_command(buffer, index);

        return 0;
    }
}

static void get_data_path(char *dest, size_t size) {
    const char *home = getenv("HOME");
    if (home) {
        snprintf(dest, size, "%s/.qui_data", home);
    } else {
        snprintf(dest, size, ".qui_data");
    }
}

res_code load_command(char *buffer, int index){
    char path[PATH_MAX];
    get_data_path(path, sizeof(path));

    FILE* fp = fopen(path, "rb");
    if (fp == NULL) return FILE_NOT_FOUND;

    fseek(fp, DATA_SIZE * index, SEEK_SET);
    if (fread(buffer, sizeof(char), DATA_SIZE, fp) < DATA_SIZE) {
        return FILE_READ_FAILED;
    }
    fclose(fp);

    return SUCCESS;
}

void change_command(char *buffer, int index){
    char path[PATH_MAX];
    get_data_path(path, sizeof(path));
    
    FILE* fp = fopen(path, "rb+");
    if (fp == NULL) {
        fp = fopen(path, "wb+");
        if (fp == NULL) {
            perror("파일 생성 실패");
            exit(EXIT_FAILURE);
        }

        char empty[DATA_SIZE * 10] = {0};
        fwrite(empty, sizeof(char), DATA_SIZE * 10, fp);
    }

    fseek(fp, DATA_SIZE * index, SEEK_SET);
    fwrite(buffer, sizeof(char), DATA_SIZE, fp);
    fclose(fp);

    return;
}