#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define DATA_SIZE 128
#define MAX_TOKEN 10
#define SHORTCUT_NUM 9 // 1부터 9까지
typedef enum {SUCCESS, FILE_NOT_FOUND, FILE_READ_FAILED} res_code;

void get_data_path(char *dest, size_t size);
void load_command(char *buffer, int index);
void change_command(char *buffer, int index);
int find_empty(void);
void init_datafile(char *path);


int main(int argc, char *argv[]){ 
    if (argc == 1) {        
        // 사용법 가이드 출력
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s <num>          Execute shortcut\n", argv[0]);
        fprintf(stderr, "  %s <num> <cmd>    Register shortcut\n", argv[0]);
        
        char buffer[DATA_SIZE];

        fprintf(stderr, "\n[qui shortcuts]\n");
        for (int i = 1; i <= 9; ++i) {
            load_command(buffer, i);
        
            buffer[DATA_SIZE - 1] = '\0';
            if (buffer[0] == '\0') {
                fprintf(stderr, "  %d: (empty)\n", i);
            } else {
                fprintf(stderr, "  %d: %s\n", i, buffer);
            }
        }
        return 0;
    }
    else if (argc == 2){
        if (strcmp(argv[1], "clean") == 0){
            char path[PATH_MAX];
            get_data_path(path, sizeof(path));
            init_datafile(path);
            return 0;
        }
        else if (strlen(argv[1]) == 1 && argv[1][0] >= '1' && argv[1][0] <= '9'){
            fprintf(stderr, "Error: Invalid index '%s'. Must be a number between 1 and 9.\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        int index = (int)(strtol(argv[1], NULL, 10));
        char buffer[DATA_SIZE];
        
        load_command(buffer, index);

        if (buffer[0] == '\0'){
            fprintf(stderr, "Shortcut %d is not set yet.\n", index);
            exit(EXIT_FAILURE);
        }

        fprintf(stderr, "%s\n", buffer);
        printf("%s", buffer);

        return 0;
    }
    else {
        int index;
        if (strcmp(argv[1], "auto") == 0){
            if ((index = find_empty()) == -1){
                fprintf(stderr, "Error: All slots (1-9) are full. Use 'qui clean' to free up space.\n");
                exit(EXIT_FAILURE);
            }
            fprintf(stderr, "Command assigned to slot %d. Run with 'qui %d'.\n", index, index);
        }
        else if (strlen(argv[1]) == 1 && argv[1][0] >= '1' && argv[1][0] <= '9'){
            index = (int)(strtol(argv[1], NULL, 10));
        }
        else {
            fprintf(stderr, "Error: Invalid index '%s'. Must be a number between 1 and 9.\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        
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

void get_data_path(char *dest, size_t size) {
    const char *home = getenv("HOME");
    if (home) {
        snprintf(dest, size, "%s/.qui_data", home);
    } else {
        snprintf(dest, size, ".qui_data");
    }
}

void load_command(char *buffer, int index) {
    char path[PATH_MAX];
    get_data_path(path, sizeof(path));

    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        init_datafile(path);
        fp = fopen(path, "rb");
    }

    if (fseek(fp, DATA_SIZE * (index - 1), SEEK_SET) != 0) {
        fprintf(stderr, "qui: failed to seek data file\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    if (fread(buffer, sizeof(char), DATA_SIZE, fp) < DATA_SIZE) {
        fprintf(stderr, "qui: failed to read data file\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);
}

void change_command(char *buffer, int index){
    char path[PATH_MAX];
    get_data_path(path, sizeof(path));
    
    FILE* fp = fopen(path, "rb+");
    if (fp == NULL) init_datafile(path);

    fseek(fp, DATA_SIZE * (index - 1), SEEK_SET);
    fwrite(buffer, sizeof(char), DATA_SIZE, fp);
    fclose(fp);

    return;
}

int find_empty(void) {
    char path[PATH_MAX];
    get_data_path(path, sizeof(path));

    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        init_datafile(path);
        fp = fopen(path, "rb");
    }

    char buffer[DATA_SIZE];
    for (int i = 1; i <= SHORTCUT_NUM; ++i) {
        if (fread(buffer, sizeof(char), DATA_SIZE, fp) < DATA_SIZE) {
            fprintf(stderr, "qui: failed to read data file\n");
            fclose(fp);
            exit(EXIT_FAILURE);
        }

        // 빈 슬롯 발견 시 파일 닫고 바로 인덱스 반환
        if (buffer[0] == '\0') {
            fclose(fp);
            return i;
        }
    }

    fclose(fp);
    return -1; // 모든 슬롯(1~SHORTCUT_NUM)이 꽉 찼을 때
}

void init_datafile(char *path){
    FILE *fp = fopen(path, "wb+");
    if (fp == NULL) {
        fprintf(stderr, "Error: Failed to create data file\n");
        exit(EXIT_FAILURE);
    }

    char empty[DATA_SIZE * SHORTCUT_NUM] = {0};
    fwrite(empty, sizeof(char), DATA_SIZE * SHORTCUT_NUM, fp);

    fclose(fp);
}