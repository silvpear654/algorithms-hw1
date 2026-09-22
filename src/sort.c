#include "sort.h"

void bubbleSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        /* 한 번 훑을 때마다 가장 큰 값이 뒤로 밀려 자리를 잡는다. */
        for (int j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
                swapped = 1;
            }
        }
        /* 한 바퀴 동안 교환이 없었다면 이미 정렬된 것이다. */
        if (!swapped) {
            return;
        }
    }
}

static int partition(int a[], int lo, int hi) {
    int pivot = a[hi];
    int i = lo;
    for (int j = lo; j < hi; j++) {
        if (a[j] <= pivot) {
            int tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            i++;
        }
    }
    int tmp = a[i];
    a[i] = a[hi];
    a[hi] = tmp;
    return i;
}

static void quickSortRange(int a[], int lo, int hi) {
    /* 재귀 대신 반복문 + 작은 쪽 먼저 재귀로 처리해 재귀 깊이를 O(log n)으로 묶는다. */
    while (lo < hi) {
        int p = partition(a, lo, hi);
        if (p - lo < hi - p) {
            quickSortRange(a, lo, p - 1);
            lo = p + 1;
        } else {
            quickSortRange(a, p + 1, hi);
            hi = p - 1;
        }
    }
}

void quickSort(int a[], int n) {
    quickSortRange(a, 0, n - 1);
}

/* 블록 하나를 처음 정렬할 때 쓰는 크기. WikiSort류 구현처럼 n에 비례하게
 * 잡지 않고 상수로 고정한다 — 블록 하나의 삽입 정렬 비용을 입력 크기와
 * 무관하게 유지하기 위해서다. */
#define BLOCK_SIZE 32

static void insertionSortRange(int a[], int lo, int hi) {
    for (int i = lo + 1; i < hi; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= lo && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

static void reverseRange(int a[], int lo, int hi) {
    hi--;
    while (lo < hi) {
        int tmp = a[lo];
        a[lo] = a[hi];
        a[hi] = tmp;
        lo++;
        hi--;
    }
}

/* 세 번 뒤집기로 [start,mid)와 [mid,end)의 순서를 맞바꾼다. 추가 배열 없이
 * 제자리에서 두 구간을 회전시키는 표준 기법이다. */
static void rotateRange(int a[], int start, int mid, int end) {
    reverseRange(a, start, mid);
    reverseRange(a, mid, end);
    reverseRange(a, start, end);
}

static int lowerBound(const int a[], int lo, int hi, int value) {
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] < value) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

/* 정렬된 a[lo..mid)와 a[mid..hi)를 추가 배열 없이 제자리에서 합친다.
 * i는 아직 합치지 못한 왼쪽 구간의 시작, j는 아직 합치지 못한 오른쪽 구간의
 * 시작이다. a[i]가 a[j]보다 크면, a[i]보다 작은 오른쪽 원소들의 구간을
 * 이분 탐색으로 한 번에 찾아 회전으로 옮긴다 (한 칸씩 밀지 않는다). */
static void mergeInPlace(int a[], int lo, int mid, int hi) {
    int i = lo;
    int j = mid;
    while (i < j && j < hi) {
        if (a[i] <= a[j]) {
            i++;
        } else {
            int k = lowerBound(a, j, hi, a[i]);
            rotateRange(a, i, j, k);
            i += k - j;
            j = k;
        }
    }
}

void blockSort(int a[], int n) {
    for (int start = 0; start < n; start += BLOCK_SIZE) {
        int end = start + BLOCK_SIZE < n ? start + BLOCK_SIZE : n;
        insertionSortRange(a, start, end);
    }
    for (int width = BLOCK_SIZE; width < n; width *= 2) {
        for (int start = 0; start < n; start += 2 * width) {
            int mid = start + width < n ? start + width : n;
            int end = start + 2 * width < n ? start + 2 * width : n;
            if (mid < end) {
                mergeInPlace(a, start, mid, end);
            }
        }
    }
}
