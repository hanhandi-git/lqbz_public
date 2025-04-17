### 合并两个有序数组
给定两个有序整数数组 nums1 和 nums2，将 nums2 合并到 nums1 中，使得合并后的数组同样有序。

#### 问题本质
本质上是要将两个有序数组合并成一个有序数组。

#### 算法步骤
1. 使用三个指针，分别指向 nums1、nums2 和合并后数组的末尾。
2. 从后往前比较 nums1 和 nums2 的元素，将较大的元素放入合并数组的末尾。
3. 如果 nums2 还有剩余元素，直接将其复制到 nums1 的开头。

#### 代码实现要点
```cpp
void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
    // 从后往前合并
    int i = m - 1; // nums1 的最后一个元素索引
    int j = n - 1; // nums2 的最后一个元素索引
    int k = m + n - 1; // 合并后数组的最后一个元素索引

    while (i >= 0 && j >= 0) {
        if (nums1[i] > nums2[j]) {
            nums1[k--] = nums1[i--];
        } else {
            nums1[k--] = nums2[j--];
        }
    }

    // 处理 nums2 中剩余的元素
    while (j >= 0) {
        nums1[k--] = nums2[j--];
    }
}
```

#### 复杂度分析
- 时间复杂度：O(m + n)
  * 需要遍历两个数组的所有元素。
- 空间复杂度：O(1)
  * 只使用了常数级别的额外空间。
