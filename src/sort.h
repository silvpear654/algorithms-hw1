/* 세 정렬 구현의 공개 인터페이스. */
#ifndef SORT_H
#define SORT_H

/* a[0..n-1]을 제자리에서 오름차순으로 정렬한다 (버블 정렬). */
void bubbleSort(int a[], int n);

/* a[0..n-1]을 제자리에서 오름차순으로 정렬한다 (퀵 정렬, Lomuto 파티션). */
void quickSort(int a[], int n);

/* a[0..n-1]을 제자리에서 오름차순으로 정렬한다
 * (블록 정렬: 블록 단위 삽입 정렬 + 회전 기반 제자리 병합). */
void blockSort(int a[], int n);

#endif /* SORT_H */
