"""실행: make run-py"""

import random
import time

from sort import bubble_sort, quick_sort, block_sort

ALGORITHMS = [
    ("버블 정렬", bubble_sort),
    ("퀵 정렬", quick_sort),
    ("블록 정렬", block_sort),
]

SIZES = [100, 500, 1000, 2000, 4000]


def measure(sort_fn, data):
    a = data[:]
    start = time.perf_counter()
    sort_fn(a)
    elapsed = time.perf_counter() - start
    if a != sorted(data):
        raise AssertionError("정렬 결과가 올바르지 않습니다")
    return elapsed


def main():
    sample = [6, 8, 5, 9, 10, 1, 7, 2, 4, 3]
    for name, fn in ALGORITHMS:
        print(f"{name}:", " ".join(str(x) for x in fn(sample[:])))

    print()
    print(f"{'n':>8} | {'버블 정렬':>10} | {'퀵 정렬':>10} | {'블록 정렬':>10}  (초)")
    random.seed(42)
    for n in SIZES:
        data = [random.randint(0, 1_000_000) for _ in range(n)]
        times = [measure(fn, data) for _, fn in ALGORITHMS]
        print(f"{n:>8} | " + " | ".join(f"{t:>10.4f}" for t in times))


if __name__ == "__main__":
    main()
