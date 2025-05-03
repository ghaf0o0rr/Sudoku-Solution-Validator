#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9
#define SUB 3

int sudoku[SIZE][SIZE];
int valid[27] = {0};

typedef struct {
    int row;
    int col;
    int type;
    int index;
} parameters;

void *check(void *param) {
    parameters *p = (parameters *)param;
    int seen[SIZE + 1] = {0};
    if (p->type == 0) {
        for (int j = 0; j < SIZE; ++j) {
            int num = sudoku[p->row][j];
            if (num < 1 || num > 9 || seen[num]) pthread_exit(NULL);
            seen[num] = 1;
        }
    } else if (p->type == 1) {
        for (int i = 0; i < SIZE; ++i) {
            int num = sudoku[i][p->col];
            if (num < 1 || num > 9 || seen[num]) pthread_exit(NULL);
            seen[num] = 1;
        }
    } else {
        int rStart = (p->row / SUB) * SUB;
        int cStart = (p->col / SUB) * SUB;
        for (int i = 0; i < SUB; ++i)
            for (int j = 0; j < SUB; ++j) {
                int num = sudoku[rStart + i][cStart + j];
                if (num < 1 || num > 9 || seen[num]) pthread_exit(NULL);
                seen[num] = 1;
            }
    }
    valid[p->index] = 1;
    pthread_exit(NULL);
}

int main(void) {
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (scanf("%d", &sudoku[i][j]) != 1) return 1;

    pthread_t tid[27];
    parameters *data[27];
    int idx = 0;

    for (int r = 0; r < SIZE; ++r) {
        data[idx] = malloc(sizeof(parameters));
        *data[idx] = (parameters){.row = r, .col = 0, .type = 0, .index = idx};
        pthread_create(&tid[idx], NULL, check, data[idx]);
        ++idx;
    }

    for (int c = 0; c < SIZE; ++c) {
        data[idx] = malloc(sizeof(parameters));
        *data[idx] = (parameters){.row = 0, .col = c, .type = 1, .index = idx};
        pthread_create(&tid[idx], NULL, check, data[idx]);
        ++idx;
    }

    for (int r = 0; r < SIZE; r += SUB)
        for (int c = 0; c < SIZE; c += SUB) {
            data[idx] = malloc(sizeof(parameters));
            *data[idx] = (parameters){.row = r, .col = c, .type = 2, .index = idx};
            pthread_create(&tid[idx], NULL, check, data[idx]);
            ++idx;
        }

    for (int i = 0; i < 27; ++i) pthread_join(tid[i], NULL);
    for (int i = 0; i < 27; ++i) free(data[i]);

    for (int i = 0; i < 27; ++i)
        if (!valid[i]) {
            printf("invalid\n");
            return 0;
        }

    printf("valid\n");
    return 0;
}
