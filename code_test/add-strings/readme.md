# 字符串相加

## 问题链接
[LeetCode 415. 字符串相加](https://leetcode.com/problems/add-strings/)

## 问题描述
给定两个字符串形式的非负整数 `num1` 和 `num2`，计算它们的和并同样以字符串形式返回。

你不能使用任何内建的用于处理大整数的库（比如 BigInteger），也不能直接将输入的字符串转换为整数形式。

示例：
```
输入：num1 = "11", num2 = "123"
输出："134"

输入：num1 = "456", num2 = "77"
输出："533"

输入：num1 = "0", num2 = "0"
输出："0"
```

## 解题思路
1. 模拟加法运算过程：
   - 从两个字符串的末尾开始，逐位相加
   - 记录进位值
   - 将每一位的计算结果添加到结果字符串中

2. 关键点：
   - 处理两个字符串长度不同的情况
   - 注意进位的处理
   - 最后需要反转结果字符串

## 代码实现
```cpp
class Solution {
public:
    string addStrings(string num1, string num2) {
        string result = "";
        int carry = 0;  // 进位值
        int i = num1.length() - 1;  // num1的末位索引
        int j = num2.length() - 1;  // num2的末位索引
        
        // 从末位开始，逐位相加
        while (i >= 0 || j >= 0 || carry > 0) {
            // 获取当前位的数字，如果已经没有数字则用0代替
            int x = i >= 0 ? num1[i] - '0' : 0;
            int y = j >= 0 ? num2[j] - '0' : 0;
            
            // 计算当前位的和（包括进位）
            int sum = x + y + carry;
            
            // 更新进位值和当前位的结果
            carry = sum / 10;
            result += (sum % 10 + '0');
            
            // 移动指针
            i--;
            j--;
        }
        
        // 反转结果字符串
        reverse(result.begin(), result.end());
        return result;
    }
};
```

## 复杂度分析
- 时间复杂度：O(max(n, m))，其中 n 和 m 分别是两个字符串的长度
- 空间复杂度：O(max(n, m))，需要存储结果字符串

## 扩展：相关问题
1. 字符串相乘
2. 二进制求和
3. 两数相加（链表版本）

## 扩展问题详解

### 1. 字符串相乘
#### 问题链接
[LeetCode 43. 字符串相乘](https://leetcode.com/problems/multiply-strings/)

#### 问题描述
给定两个以字符串形式表示的非负整数 `num1` 和 `num2`，返回 `num1` 和 `num2` 的乘积，它们的乘积也表示为字符串形式。

示例：
```
输入: num1 = "123", num2 = "456"
输出: "56088"
```

#### 解题思路
1. 模拟竖式乘法的过程：
   - 将每一位相乘
   - 处理进位
   - 最后将所有结果相加
让我们以 "123" × "456" 为例，详细说明计算过程：

```
    1 2 3     <- num1
  × 4 5 6     <- num2
-----------
    7 3 8     <- 123 × 6
  6 1 5 0     <- 123 × 50
4 9 2 0 0     <- 123 × 400
-----------
5 6 0 8 8     <- 最终结果
```

2. 关键实现细节：
   - 结果的长度最多为 num1.length + num2.length
   - 使用数组存储中间结果，避免字符串频繁拼接
   - 每位相乘的结果位置是确定的：
     * i位数字 × j位数字的结果影响 [i+j, i+j+1] 位置
     * 例如：个位×个位影响结果的个位和十位

具体过程示例（以 "123" × "456" 为例）：
```
1. 初始化结果数组 [0,0,0,0,0,0]（长度为3+3=6）

2. 3×6=18:
   [0,0,0,0,1,8]

3. 3×5=15:
   [0,0,0,1,6,8]

4. 3×4=12:
   [0,0,1,3,6,8]

5. 2×6=12:
   [0,0,1,4,8,8]

6. 2×5=10:
   [0,0,2,4,8,8]

7. 2×4=8:
   [0,0,5,4,8,8]

8. 1×6=6:
   [0,0,5,5,8,8]

9. 1×5=5:
   [0,0,5,6,8,8]

10. 1×4=4:
    [0,0,5,6,0,8,8]

最终结果：56088（去掉前导零）
```

3. 优化技巧：
   - 特殊情况处理：如果任一数字为0，直接返回"0"
   - 使用数组而不是字符串存储中间结果，提高效率
   - 从个位开始计算，这样更容易处理进位
   - 最后处理前导零

4. 注意事项：
   - 需要正确处理进位
   - 结果可能比任何一个输入数字都长
   - 要注意去除结果中的前导零
   - 所有计算都要在字符串层面进行，不能转换为整数
```cpp
class Solution {
public:
    string multiply(string num1, string num2) {
        if (num1 == "0" || num2 == "0") return "0";
        
        int n1 = num1.length(), n2 = num2.length();
        vector<int> result(n1 + n2, 0);
        
        // 从个位数开始逐位相乘
        for (int i = n1 - 1; i >= 0; i--) {
            for (int j = n2 - 1; j >= 0; j--) {
                int mul = (num1[i] - '0') * (num2[j] - '0');
                int p1 = i + j, p2 = i + j + 1;  // p1:高位, p2:低位
                int sum = mul + result[p2];
                
                result[p2] = sum % 10;
                result[p1] += sum / 10;
            }
        }
        
        // 构建结果字符串
        string str;
        int i = 0;
        while (i < result.size() && result[i] == 0) i++;
        while (i < result.size()) str += result[i++] + '0';
        
        return str.empty() ? "0" : str;
    }
};
```

### 2. 二进制求和
#### 问题链接
[LeetCode 67. 二进制求和](https://leetcode.com/problems/add-binary/)

#### 问题描述
给你两个二进制字符串 `a` 和 `b`，以二进制字符串的形式返回它们的和。

示例：
```
输入: a = "11", b = "1"
输出: "100"

输入: a = "1010", b = "1011"
输出: "10101"
```

#### 解题思路
- 类似于字符串相加，但是是在二进制下进行运算
- 需要注意进位是在2进制下处理

```cpp
class Solution {
public:
    string addBinary(string a, string b) {
        string result;
        int carry = 0;
        int i = a.length() - 1;
        int j = b.length() - 1;
        
        while (i >= 0 || j >= 0 || carry > 0) {
            int sum = carry;
            if (i >= 0) sum += a[i--] - '0';
            if (j >= 0) sum += b[j--] - '0';
            
            // 在二进制下处理进位
            carry = sum / 2;
            result += (sum % 2 + '0');
        }
        
        reverse(result.begin(), result.end());
        return result;
    }
};
```

### 3. 两数相加（链表版本）
#### 问题链接
[LeetCode 2. 两数相加](https://leetcode.com/problems/add-two-numbers/)

#### 问题描述
给你两个非空的链表，表示两个非负的整数。它们每位数字都是按照逆序的方式存储的，并且每个节点只能存储一位数字。请你将两个数相加，并以相同形式返回一个表示和的链表。

示例：
```
输入：l1 = [2,4,3], l2 = [5,6,4]
输出：[7,0,8]
解释：342 + 465 = 807
```

#### 解题思路
- 同时遍历两个链表，逐位相加
- 处理进位
- 注意链表已经是逆序存储，不需要反转结果

```cpp
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* dummy = new ListNode(0);
        ListNode* curr = dummy;
        int carry = 0;
        
        while (l1 || l2 || carry) {
            int sum = carry;
            if (l1) {
                sum += l1->val;
                l1 = l1->next;
            }
            if (l2) {
                sum += l2->val;
                l2 = l2->next;
            }
            
            carry = sum / 10;
            curr->next = new ListNode(sum % 10);
            curr = curr->next;
        }
        
        return dummy->next;
    }
};
```

### 总结
字符串数字运算是一类重要的编程问题，主要涉及以下几个要点：

1. 模拟运算过程：需要理解和模拟实际的数学运算过程
2. 进位处理：在不同进制下正确处理进位
3. 字符和数字转换：灵活运用字符和数字之间的转换
4. 结果处理：注意前导零和结果的存储形式

这类问题不仅考察基本的编程能力，还能帮助理解数字在计算机中的表示和处理方式。同时，这些解法也常常用于处理大数运算的场景。
