"""정렬 세 가지 구현 — 버블 정렬(배움), 퀵 정렬(배움), 블록 정렬(안 배움)."""


def bubble_sort(a):
    """a를 제자리에서 오름차순으로 정렬한다."""
    n = len(a)
    for i in range(n - 1):
        swapped = False
        # 한 번 훑을 때마다 가장 큰 값이 뒤로 밀려 자리를 잡는다.
        for j in range(n - 1 - i):
            if a[j] > a[j + 1]:
                a[j], a[j + 1] = a[j + 1], a[j]
                swapped = True
        # 한 바퀴 동안 교환이 없었다면 이미 정렬된 것이다.
        if not swapped:
            return a
    return a


def _partition(a, lo, hi):
    pivot = a[hi]
    i = lo
    for j in range(lo, hi):
        if a[j] <= pivot:
            a[i], a[j] = a[j], a[i]
            i += 1
    a[i], a[hi] = a[hi], a[i]
    return i


def _quick_sort(a, lo, hi):
    # 재귀 대신 반복문 + 작은 쪽 먼저 재귀로 처리해 재귀 깊이를 O(log n)으로 묶는다.
    while lo < hi:
        p = _partition(a, lo, hi)
        if p - lo < hi - p:
            _quick_sort(a, lo, p - 1)
            lo = p + 1
        else:
            _quick_sort(a, p + 1, hi)
            hi = p - 1


def quick_sort(a):
    """a를 제자리에서 오름차순으로 정렬한다 (퀵 정렬, Lomuto 파티션)."""
    _quick_sort(a, 0, len(a) - 1)
    return a


# 블록 하나를 처음 정렬할 때 쓰는 크기. WikiSort류 구현처럼 n에 비례하게
# 잡지 않고 상수로 고정한다 — 블록 하나의 삽입 정렬 비용을 입력 크기와
# 무관하게 유지하기 위해서다.
_BLOCK_SIZE = 32


def _insertion_sort(a, lo, hi):
    for i in range(lo + 1, hi):
        key = a[i]
        j = i - 1
        while j >= lo and a[j] > key:
            a[j + 1] = a[j]
            j -= 1
        a[j + 1] = key


def _reverse(a, lo, hi):
    hi -= 1
    while lo < hi:
        a[lo], a[hi] = a[hi], a[lo]
        lo += 1
        hi -= 1


def _rotate(a, start, mid, end):
    # 세 번 뒤집기로 [start,mid)와 [mid,end)의 순서를 맞바꾼다.
    # 추가 배열 없이 제자리에서 두 구간을 회전시키는 표준 기법이다.
    _reverse(a, start, mid)
    _reverse(a, mid, end)
    _reverse(a, start, end)


def _lower_bound(a, lo, hi, value):
    while lo < hi:
        mid = (lo + hi) // 2
        if a[mid] < value:
            lo = mid + 1
        else:
            hi = mid
    return lo


def _merge_in_place(a, lo, mid, hi):
    # 정렬된 a[lo:mid]와 a[mid:hi]를 추가 배열 없이 제자리에서 합친다.
    # i는 아직 합치지 못한 왼쪽 구간의 시작, j는 아직 합치지 못한 오른쪽
    # 구간의 시작이다. a[i]가 a[j]보다 크면, a[i]보다 작은 오른쪽 원소들의
    # 구간을 이분 탐색으로 한 번에 찾아 회전으로 옮긴다 (한 칸씩 밀지 않는다).
    i, j = lo, mid
    while i < j and j < hi:
        if a[i] <= a[j]:
            i += 1
        else:
            k = _lower_bound(a, j, hi, a[i])
            _rotate(a, i, j, k)
            i += k - j
            j = k


def block_sort(a):
    """a를 제자리에서 오름차순으로 정렬한다
    (블록 정렬: 블록 단위 삽입 정렬 + 회전 기반 제자리 병합)."""
    n = len(a)
    for start in range(0, n, _BLOCK_SIZE):
        _insertion_sort(a, start, min(start + _BLOCK_SIZE, n))

    width = _BLOCK_SIZE
    while width < n:
        for start in range(0, n, 2 * width):
            mid = min(start + width, n)
            end = min(start + 2 * width, n)
            if mid < end:
                _merge_in_place(a, start, mid, end)
        width *= 2
    return a
