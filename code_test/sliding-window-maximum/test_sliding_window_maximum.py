import unittest
from typing import List

class Solution:
    def maxSlidingWindow(self, nums: List[int], k: int) -> List[int]:
        # 这里实现你的解决方案
        pass

class TestSlidingWindowMaximum(unittest.TestCase):
    def setUp(self):
        self.solution = Solution()

    def test_example_1(self):
        nums = [1, 3, -1, -3, 5, 3, 6, 7]
        k = 3
        expected = [3, 3, 5, 5, 6, 7]
        self.assertEqual(self.solution.maxSlidingWindow(nums, k), expected)

    def test_example_2(self):
        nums = [1]
        k = 1
        expected = [1]
        self.assertEqual(self.solution.maxSlidingWindow(nums, k), expected)

    def test_empty_input(self):
        nums = []
        k = 0
        expected = []
        self.assertEqual(self.solution.maxSlidingWindow(nums, k), expected)

    def test_large_window(self):
        nums = [1, 3, -1, -3, 5, 3, 6, 7]
        k = 8
        expected = [7]
        self.assertEqual(self.solution.maxSlidingWindow(nums, k), expected)

    def test_negative_numbers(self):
        nums = [-7, -8, 7, 5, 7, 1, 6, 0]
        k = 4
        expected = [7, 7, 7, 7, 7]
        self.assertEqual(self.solution.maxSlidingWindow(nums, k), expected)

if __name__ == '__main__':
    unittest.main()