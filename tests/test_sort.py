"""유닛 테스트 — 표준 라이브러리의 unittest만 쓴다.

실행: make test-py
"""

import sys
import unittest
from pathlib import Path

# src/를 import 경로에 넣는다. 패키지로 만들지 않아도 되도록.
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "src"))

from sort import bubble_sort, quick_sort, block_sort  # noqa: E402

ALGORITHMS = {
    "bubble_sort": bubble_sort,
    "quick_sort": quick_sort,
    "block_sort": block_sort,
}


class TestSortAlgorithms(unittest.TestCase):
    def test_shuffled(self):
        for name, fn in ALGORITHMS.items():
            with self.subTest(name=name):
                a = [6, 8, 5, 9, 10, 1, 7, 2, 4, 3]
                self.assertEqual(fn(a), [1, 2, 3, 4, 5, 6, 7, 8, 9, 10])

    def test_already_sorted(self):
        for name, fn in ALGORITHMS.items():
            with self.subTest(name=name):
                a = [1, 2, 3, 4, 5]
                self.assertEqual(fn(a), [1, 2, 3, 4, 5])

    def test_reversed(self):
        for name, fn in ALGORITHMS.items():
            with self.subTest(name=name):
                a = [5, 4, 3, 2, 1]
                self.assertEqual(fn(a), [1, 2, 3, 4, 5])

    def test_duplicates(self):
        for name, fn in ALGORITHMS.items():
            with self.subTest(name=name):
                a = [3, 1, 3, 1, 2]
                self.assertEqual(fn(a), [1, 1, 2, 3, 3])

    def test_single(self):
        for name, fn in ALGORITHMS.items():
            with self.subTest(name=name):
                self.assertEqual(fn([42]), [42])

    def test_empty(self):
        for name, fn in ALGORITHMS.items():
            with self.subTest(name=name):
                self.assertEqual(fn([]), [])

    def test_sorts_in_place(self):
        for name, fn in ALGORITHMS.items():
            with self.subTest(name=name):
                a = [3, 1, 2]
                fn(a)
                self.assertEqual(a, [1, 2, 3])

    def test_large_array_spanning_blocks(self):
        # 블록 정렬의 블록 크기(32)보다 커서 병합(rotate + 이분 탐색) 경로를
        # 실제로 거치는지 확인한다.
        a = [(i * 37 + 5) % 97 for i in range(200)]
        for name, fn in ALGORITHMS.items():
            with self.subTest(name=name):
                self.assertEqual(fn(a[:]), sorted(a))


if __name__ == "__main__":
    unittest.main()
