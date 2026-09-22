/* 실행: make run-c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sort.h"

typedef void (*SortFn)(int[], int);

typedef struct {
    const char *name;
    SortFn fn;
} Algorithm;

static int isSorted(const int a[], int n) {
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) {
            return 0;
        }
    }
    return 1;
}

static double measure(SortFn fn, const int data[], int n) {
    int *a = malloc((size_t)n * sizeof(int));
    memcpy(a, data, (size_t)n * sizeof(int));

    clock_t start = clock();
    fn(a, n);
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;

    if (!isSorted(a, n)) {
        fprintf(stderr, "정렬 결과가 올바르지 않습니다\n");
        exit(1);
    }
    free(a);
    return elapsed;
}

int main(void) {
    int sample[] = {6, 8, 5, 9, 10, 1, 7, 2, 4, 3};
    int sampleN = (int)(sizeof(sample) / sizeof(sample[0]));

    Algorithm algorithms[] = {
        {"버블 정렬", bubbleSort},
        {"퀵 정렬", quickSort},
        {"블록 정렬", blockSort},
    };
    int numAlgorithms = (int)(sizeof(algorithms) / sizeof(algorithms[0]));

    for (int i = 0; i < numAlgorithms; i++) {
        int copy[10];
        memcpy(copy, sample, sizeof(sample));
        algorithms[i].fn(copy, sampleN);
        printf("%s:", algorithms[i].name);
        for (int j = 0; j < sampleN; j++) {
            printf(" %d", copy[j]);
        }
        printf("\n");
    }

    printf("\n%8s |  %10s |  %10s |  %10s  (초)\n", "n", "버블 정렬", "퀵 정렬", "블록 정렬");
    int sizes[] = {1000, 5000, 10000, 20000, 40000};
    int numSizes = (int)(sizeof(sizes) / sizeof(sizes[0]));

    srand(42);
    for (int s = 0; s < numSizes; s++) {
        int size = sizes[s];
        int *data = malloc((size_t)size * sizeof(int));
        for (int i = 0; i < size; i++) {
            data[i] = rand() % 1000000;
        }

        printf("%8d |", size);
        for (int i = 0; i < numAlgorithms; i++) {
            double t = measure(algorithms[i].fn, data, size);
            printf("  %10.4f |", t);
        }
        printf("\n");

        free(data);
    }

    return 0;
}
