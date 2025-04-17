# 接雨水

## 问题链接
[LeetCode 42. 接雨水](https://leetcode.com/problems/trapping-rain-water/)

## 问题描述
给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。

### 一维接雨水图解
```
示例 1: height = [0,1,0,2,1,0,1,3,2,1,2,1]

按高度绘制的柱状图：
                     __
         __         |  |
   __    |  |__    |  |__    __
__|  |__ |     |__ |     |__|  |__
[0,1,0,2,1,0,1,3,2,1,2,1]

积水后的状态（~表示水）：
                     __
         __    ~    |  |
   __    |  |__    |  |__    __
__|  |__ |     |__ |     |__|  |__
[0,1,0,2,1,0,1,3,2,1,2,1]

计算过程：
位置 2: 可以存储 1 单位水
位置 5: 可以存储 2 单位水
位置 6: 可以存储 1 单位水
位置 9: 可以存储 1 单位水
位置 10: 可以存储 1 单位水
总计: 6 单位水
```

```
示例 2: height = [4,2,0,3,2,5]

按高度绘制的柱状图：
            __
__          |  |
|  |    __  |  |
|  |    |  ||  |
|  |__  |  ||  |
[4,2,0,3,2,5]

积水后的状态（~表示水）：
            __
__    ~  ~  |  |
|  |  ~ __  |  |
|  |~ ~|  | |  |
|  |__|  |_|  |
[4,2,0,3,2,5]

计算过程：
位置 2: 可以存储 2 单位水
位置 3: 可以存储 1 单位水
位置 4: 可以存储 2 单位水
总计: 9 单位水
```

## 解题思路
这道题有多种解法，每种解法都有其独特的思维角度和实现方式。让我们深入分析每种方法：

### 1. 动态规划法
#### 核心思想
- 对于每个位置i，能接的雨水量取决于两个因素：
  1. 左边最高的柱子高度leftMax[i]
  2. 右边最高的柱子高度rightMax[i]
- 该位置能接的雨水量 = min(leftMax[i], rightMax[i]) - height[i]

#### 实现步骤
1. 预处理得到每个位置左右两侧的最大高度：
   ```
   对于位置i：
   leftMax[i] = max(leftMax[i-1], height[i])
   rightMax[i] = max(rightMax[i+1], height[i])
   ```

2. 遍历每个位置计算可以接的雨水量：
   ```
   对于每个位置i：
   water[i] = min(leftMax[i], rightMax[i]) - height[i]
   ```

#### 图解分析
```
示例：height = [0,1,0,2,1,0,1,3,2,1,2,1]

1. 计算leftMax数组：
位置:    0  1  2  3  4  5  6  7  8  9  10 11
height:  0  1  0  2  1  0  1  3  2  1  2  1
leftMax: 0  1  1  2  2  2  2  3  3  3  3  3

2. 计算rightMax数组：
位置:     0  1  2  3  4  5  6  7  8  9  10 11
height:   0  1  0  2  1  0  1  3  2  1  2  1
rightMax: 3  3  3  3  3  3  3  3  2  2  2  1

3. 计算每个位置能接的水量：
位置i=2为例：
- leftMax[2] = 1
- rightMax[2] = 3
- height[2] = 0
- water[2] = min(1,3) - 0 = 1
```

### 2. 双指针法
#### 核心思想
- 使用左右两个指针从两端向中间移动
- 维护左右两侧已遍历部分的最大高度
- 根据较小的一侧来计算当前位置能接的水量

#### 实现步骤
1. 初始化左右指针和左右最大高度：
   ```
   left = 0, right = n-1
   leftMax = rightMax = 0
   ```

2. 当left < right时：
   - 如果height[left] < height[right]：
     * 如果height[left] >= leftMax，更新leftMax
     * 否则可以接水：water += leftMax - height[left]
     * left++
   - 否则：
     * 如果height[right] >= rightMax，更新rightMax
     * 否则可以接水：water += rightMax - height[right]
     * right--

#### 图解分析
```
示例：height = [0,1,0,2,1,0,1,3,2,1,2,1]

初始状态：
left=0, right=11
leftMax=0, rightMax=0

步骤1：
height[0]=0 < height[11]=1
water += 0-0 = 0
leftMax = 0
left++

步骤2：
height[1]=1 < height[11]=1
leftMax = 1
left++

... 以此类推
```

### 3. 单调栈法
#### 核心思想
- 使用栈维护一个单调递减的高度序列
- 当遇到一个较大的高度时，说明找到了一个可能的凹槽
- 计算凹槽能接的雨水量 = 凹槽宽度 * (min(左边界高度,右边界高度) - 凹槽底部高度)

#### 实现步骤
1. 维护单调栈：
   - 当前高度小于等于栈顶高度时，入栈
   - 当前高度大于栈顶高度时，说明找到了一个凹槽

2. 处理凹槽：
   - 弹出栈顶元素，这是凹槽的底部
   - 计算左右边界（当前元素和新的栈顶元素）
   - 计算这个凹槽能接的水量

#### 图解分析
```
示例：height = [0,1,0,2,1,0,1,3,2,1,2,1]

步骤1：遍历到height[0]=0
栈：[0]

步骤2：遍历到height[1]=1
1 > 0，找到凹槽
栈：[1]

步骤3：遍历到height[2]=0
0 < 1，入栈
栈：[1,2]

步骤4：遍历到height[3]=2
2 > 0，找到凹槽
计算位置2的积水量：min(1,2)-0 = 1
栈：[3]

... 以此类推
```

### 算法选择建议
1. 如果对空间复杂度要求不严格：
   - 选择动态规划，代码直观易懂
   - 时间复杂度O(n)，空间复杂度O(n)

2. 如果要求优化空间复杂度：
   - 选择双指针法，可以实现O(1)空间复杂度
   - 时间复杂度仍然是O(n)

3. 如果需要处理类似的单调性问题：
   - 选择单调栈法，这种思维方式可以推广到其他问题
   - 时间复杂度O(n)，空间复杂度O(n)

## 代码实现

### 1. 动态规划解法
```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        if (height.empty()) return 0;
        
        int n = height.size();
        vector<int> leftMax(n), rightMax(n);
        
        // 计算左侧最大高度
        leftMax[0] = height[0];
        for (int i = 1; i < n; i++) {
            leftMax[i] = max(leftMax[i-1], height[i]);
        }
        
        // 计算右侧最大高度
        rightMax[n-1] = height[n-1];
        for (int i = n-2; i >= 0; i--) {
            rightMax[i] = max(rightMax[i+1], height[i]);
        }
        
        // 计算接水量
        int water = 0;
        for (int i = 0; i < n; i++) {
            water += min(leftMax[i], rightMax[i]) - height[i];
        }
        
        return water;
    }
};
```

### 2. 双指针解法
```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int left = 0, right = height.size() - 1;
        int leftMax = 0, rightMax = 0;
        int water = 0;
        
        while (left < right) {
            if (height[left] < height[right]) {
                if (height[left] >= leftMax) {
                    leftMax = height[left];
                } else {
                    water += leftMax - height[left];
                }
                left++;
            } else {
                if (height[right] >= rightMax) {
                    rightMax = height[right];
                } else {
                    water += rightMax - height[right];
                }
                right--;
            }
        }
        
        return water;
    }
};
```

### 3. 单调栈解法
```cpp
class Solution {
public:
    int trap_stack(vector<int>& height) {
        // 单调栈存储的是下标，栈中下标对应的高度是单调递减的
        stack<int> st;
        int water = 0;
        
        // 遍历每个柱子
        for (int i = 0; i < height.size(); i++) {
            // 当栈不为空且当前柱子高度大于栈顶柱子高度时
            // 说明找到了一个可以接雨水的凹槽
            while (!st.empty() && height[i] > height[st.top()]) {
                // 取出凹槽底部的高度
                int top = st.top();
                st.pop();
                
                // 如果栈为空，说明左边没有柱子了，无法接水
                if (st.empty()) break;
                
                // 计算凹槽的宽度：当前柱子下标 - 左边柱子下标 - 1
                int distance = i - st.top() - 1;
                // 计算凹槽的高度：两边柱子的最小高度 - 凹槽底部高度
                int bounded_height = min(height[i], height[st.top()]) - height[top];
                // 累加这个凹槽可以接的水量
                water += distance * bounded_height;
            }
            // 将当前柱子入栈，继续维护单调递减特性
            st.push(i);
        }
        
        return water;
    }
};
```

### 单调栈解法详解
单调栈解法的核心思想是维护一个单调递减的高度序列，通过栈来处理每个位置可能形成的凹槽。
1. 为什么要用单调栈
- 单调栈可以帮助我们找到左右两边第一个比当前高度高的柱子
- 这正是计算积水需要的信息

2. 栈内元素的含义
- 栈中存储的是下标而不是高度
- 栈中的元素对应的高度是单调递减的

3. 计算过程的关键步骤
- 当找到一个比栈顶元素高的柱子时,就可以计算积水
- 计算宽度和高度的方法
- 为什么要判断栈是否为空

4. 整体的思路流程
- 维护单调递减栈
- 当出现更高柱子时计算积水
- 最后剩余的递减序列不会有积水
#### 工作原理
1. **栈的维护**：
   - 栈中存储的是柱子的索引（而不是高度）
   - 保持栈中的柱子高度是单调递减的
   - 当遇到一个较高的柱子时，说明可能形成凹槽

2. **凹槽的形成**：
   ```
   当前柱子: i
   栈顶柱子（凹槽底部）: top
   次栈顶柱子（左边界）: st.top()
   右边界: 当前柱子i

        |
   |    |
   |  _ |   <- 当前位置i
   |_|  |   <- top（凹槽底部）
   ```

3. **积水量计算**：
   - 凹槽宽度 = 右边界位置 - 左边界位置 - 1
   - 凹槽高度 = min(左边界高度, 右边界高度) - 凹槽底部高度
   - 该凹槽积水量 = 宽度 * 高度

#### 示例分析
以输入 `[4,2,0,3,2,5]` 为例：
```
步骤1: 初始状态
4 |    |
3 |    |         |
2 |    |__    |  |
1 |    |  |   |  |
0 |____|__|___|__|
  [4,   2,  0,  3]  栈:[0]（索引）

步骤2: 遇到高度3
4 |    |
3 |    |    |      <- 可以接水
2 |    |__  |  
1 |    |  | |  
0 |____|__|_|__
  [4,   2,  0,  3]  栈:[0,1]

步骤3: 计算积水
- 凹槽底部(top) = 2
- 左边界 = 2
- 右边界 = 3
- 宽度 = 3 - 2 - 1 = 0
- 高度 = min(2, 3) - 0 = 2
- 积水量 = 0 * 2 = 2
```

#### 代码实现要点
```cpp
int trap_stack(vector<int>& height) {
    stack<int> st;  // 存储索引的栈
    int water = 0;
    
    for (int i = 0; i < height.size(); i++) {
        // 当前高度大于栈顶高度时，可能形成凹槽
        while (!st.empty() && height[i] > height[st.top()]) {
            int top = st.top();  // 凹槽底部
            st.pop();
            
            if (st.empty()) break;  // 没有左边界
            
            // 计算凹槽大小
            int distance = i - st.top() - 1;  // 宽度
            int bounded_height = min(height[i], height[st.top()]) - height[top];  // 高度
            water += distance * bounded_height;
        }
        st.push(i);
    }
    return water;
}
```

#### 优缺点分析
优点：
- 可以一次遍历就得到结果
- 适合处理需要找左右边界的问题
- 可以方便地处理多个凹槽的情况

缺点：
- 需要额外的栈空间
- 实现相对复杂
- 不如双指针解法直观

## 复杂度分析

### 动态规划解法
- 时间复杂度：O(n)，需要遍历数组三次
- 空间复杂度：O(n)，需要两个数组存储左右最大高度

### 双指针解法
- 时间复杂度：O(n)，只需要遍历一次数组
- 空间复杂度：O(1)，只需要常数空间

### 单调栈解法
- 时间复杂度：O(n)，每个元素最多被压入和弹出栈一次
- 空间复杂度：O(n)，栈的大小最大为n

## 解题技巧
1. 理解积水的条件：当前位置能积水的量取决于其左右两侧最大高度的较小值
2. 选择合适的解法：
   - 如果对空间要求不严格，可以使用动态规划
   - 如果要求空间复杂度为O(1)，使用双指针法
   - 如果需要处理类似的单调性问题，可以考虑单调栈
