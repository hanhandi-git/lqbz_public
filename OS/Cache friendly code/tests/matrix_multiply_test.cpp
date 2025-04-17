#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
using namespace std::chrono;

/**
 * @brief 矩阵乘法性能测试类
 */
class MatrixMultiplyTester {
private:
    vector<float> A, B, C1, C2;
    const int N;
    const int BLOCK_SIZE;
    
public:
    MatrixMultiplyTester(int size = 1024, int block_size = 32) 
        : N(size), BLOCK_SIZE(block_size) {
        // 初始化矩阵
        A.resize(N * N);
        B.resize(N * N);
        C1.resize(N * N);
        C2.resize(N * N);
        
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<float> dis(-1.0, 1.0);
        
        for(int i = 0; i < N * N; i++) {
            A[i] = dis(gen);
            B[i] = dis(gen);
        }
    }
    
    void runTest() {
        cout << "开始矩阵乘法测试 (矩阵大小: " << N << "x" << N << ")\n" << endl;
        
        // 测试传统实现
        auto naive_time = testNaiveMultiply();
        
        // 测试分块优化实现
        auto blocked_time = testBlockedMultiply();
        
        // 验证结果
        bool correct = verifyResults();
        
        // 输出结果
        cout << "\n测试结果汇总:" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "实现方式" << setw(15) << "耗时(ms)" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "传统实现" << setw(15) << naive_time << endl;
        cout << setw(20) << "分块优化" << setw(15) << blocked_time << endl;
        cout << "----------------------------------------" << endl;
        cout << "结果验证: " << (correct ? "正确" : "错误") << endl;
        
        if(naive_time > 0) {
            double speedup = naive_time / blocked_time;
            cout << "性能提升: " << fixed << setprecision(2) << speedup << "x" << endl;
        }
    }
    
private:
    double testNaiveMultiply() {
        cout << "测试传统矩阵乘法..." << endl;
        
        auto start = high_resolution_clock::now();
        
        for(int i = 0; i < N; i++)
            for(int j = 0; j < N; j++) {
                float sum = 0;
                for(int k = 0; k < N; k++)
                    sum += A[i*N + k] * B[k*N + j];
                C1[i*N + j] = sum;
            }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
    
    double testBlockedMultiply() {
        cout << "测试分块矩阵乘法..." << endl;
        
        auto start = high_resolution_clock::now();
        
        for(int i0 = 0; i0 < N; i0 += BLOCK_SIZE)
            for(int j0 = 0; j0 < N; j0 += BLOCK_SIZE)
                for(int k0 = 0; k0 < N; k0 += BLOCK_SIZE)
                    
                    // 处理一个块
                    for(int i = i0; i < min(i0+BLOCK_SIZE, N); i++)
                        for(int j = j0; j < min(j0+BLOCK_SIZE, N); j++) {
                            float sum = (k0 == 0) ? 0 : C2[i*N + j];
                            for(int k = k0; k < min(k0+BLOCK_SIZE, N); k++)
                                sum += A[i*N + k] * B[k*N + j];
                            C2[i*N + j] = sum;
                        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
    
    bool verifyResults() {
        const float epsilon = 1e-5;
        for(int i = 0; i < N * N; i++) {
            if(abs(C1[i] - C2[i]) > epsilon) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    MatrixMultiplyTester tester;
    tester.runTest();
    return 0;
} 