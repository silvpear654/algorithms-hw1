/* 유닛 테스트 — 외부 프레임워크 없이 표준 C만 쓴다.
 * 실행: make test-c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"

static int cmpInt(const void *x, const void *y) {
    return *(const int *)x - *(const int *)y;
}

typedef void (*SortFn)(int[], int);

typedef struct {
    const char *name;
    SortFn fn;
} Algorithm;

static int checks = 0;
static int failures = 0;

static void printArray(const char *label, const int a[], int n) {
    printf("      %s:", label);
    for (int i = 0; i < n; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");
}

/* input을 정렬한 결과가 want와 같은지 본다. input은 건드리지 않고 복사본으로 정렬한다. */
static void expectSorted(const char *algoName, const char *caseName, SortFn fn,
                          const int input[], const int want[], int n) {
    checks++;
    /* 테스트 배열은 전부 이 크기 안에 들어온다 (가장 큰 케이스가 블록 정렬의
     * 병합 경로를 확인하려고 블록 크기(32)보다 크게 잡은 200개짜리 배열이다). */
    int copy[200];
    memcpy(copy, input, (size_t)n * sizeof(int));
    fn(copy, n);
    if (n > 0 && memcmp(copy, want, (size_t)n * sizeof(int)) != 0) {
        failures++;
        printf("FAIL  [%s] %s\n", algoName, caseName);
        printArray("got ", copy, n);
        printArray("want", want, n);
        return;
    }
    printf("ok    [%s] %s\n", algoName, caseName);
}

int main(void) {
    Algorithm algorithms[] = {
        {"버블 정렬", bubbleSort},
        {"퀵 정렬", quickSort},
        {"블록 정렬", blockSort},
    };
    int numAlgorithms = (int)(sizeof(algorithms) / sizeof(algorithms[0]));

    for (int k = 0; k < numAlgorithms; k++) {
        const char *algoName = algorithms[k].name;
        SortFn fn = algorithms[k].fn;

        {
            int a[] = {6, 8, 5, 9, 10, 1, 7, 2, 4, 3};
            const int want[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            expectSorted(algoName, "섞인 배열", fn, a, want, 10);
        }
        {
            int a[] = {1, 2, 3, 4, 5};
            const int want[] = {1, 2, 3, 4, 5};
            expectSorted(algoName, "이미 정렬된 배열", fn, a, want, 5);
        }
        {
            int a[] = {5, 4, 3, 2, 1};
            const int want[] = {1, 2, 3, 4, 5};
            expectSorted(algoName, "역순 배열", fn, a, want, 5);
        }
        {
            int a[] = {3, 1, 3, 1, 2};
            const int want[] = {1, 1, 2, 3, 3};
            expectSorted(algoName, "중복이 있는 배열", fn, a, want, 5);
        }
        {
            int a[] = {42};
            const int want[] = {42};
            expectSorted(algoName, "원소 하나", fn, a, want, 1);
        }
        {
            /* n = 0이면 배열을 건드리지 않는다. 초기화해 두어야 경고가 없다. */
            int a[1] = {0};
            const int want[1] = {0};
            expectSorted(algoName, "빈 배열", fn, a, want, 0);
        }
        {
            /* 블록 정렬의 블록 크기(32)보다 커서 병합(rotate + 이분 탐색)
             * 경로를 실제로 거치는지 확인한다. */
            enum { BIG_N = 200 };
            int a[BIG_N];
            int want[BIG_N];
            for (int i = 0; i < BIG_N; i++) {
                a[i] = (i * 37 + 5) % 97;
            }
            memcpy(want, a, sizeof(a));
            qsort(want, BIG_N, sizeof(int), cmpInt);
            expectSorted(algoName, "블록 경계를 넘는 큰 배열", fn, a, want, BIG_N);
        }
    }

    printf("\n%d checks, %d failures\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
