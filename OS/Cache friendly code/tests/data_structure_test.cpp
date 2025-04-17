#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace chrono;

/**
 * @brief 测试AoS和SoA性能差异
 */
class DataStructureTester {
private:
    // AoS结构
    struct Particle {
        float x, y, z;    // 位置
        float vx, vy, vz; // 速度
        float ax, ay, az; // 加速度
        float m;          // 质量
        float radius;     // 半径
        float density;    // 密度
    };
    
    // SoA结构
    struct ParticleSystem {
        vector<float> x, y, z;    // 位置
        vector<float> vx, vy, vz; // 速度
        vector<float> ax, ay, az; // 加速度
        vector<float> m;          // 质量
        vector<float> radius;     // 半径
        vector<float> density;    // 密度
        
        ParticleSystem(size_t n) : 
            x(n), y(n), z(n),
            vx(n), vy(n), vz(n),
            ax(n), ay(n), az(n),
            m(n), radius(n), density(n) {}
    };
    
    vector<Particle> aos_particles;
    ParticleSystem soa_particles;
    const size_t particle_count;
    const size_t iterations;
    
public:
    DataStructureTester(size_t count = 1000000, size_t iter = 1000) 
        : particle_count(count), iterations(iter), 
          soa_particles(count) {
        
        // 初始化数据
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<float> dis(-1.0, 1.0);
        
        aos_particles.resize(count);
        for(size_t i = 0; i < count; i++) {
            // 初始化AoS
            aos_particles[i].x = dis(gen);
            aos_particles[i].y = dis(gen);
            aos_particles[i].z = dis(gen);
            aos_particles[i].vx = dis(gen);
            aos_particles[i].vy = dis(gen);
            aos_particles[i].vz = dis(gen);
            aos_particles[i].ax = dis(gen);
            aos_particles[i].ay = dis(gen);
            aos_particles[i].az = dis(gen);
            aos_particles[i].m = dis(gen);
            aos_particles[i].radius = dis(gen);
            aos_particles[i].density = dis(gen);
            
            // 初始化SoA
            soa_particles.x[i] = aos_particles[i].x;
            soa_particles.y[i] = aos_particles[i].y;
            soa_particles.z[i] = aos_particles[i].z;
            soa_particles.vx[i] = aos_particles[i].vx;
            soa_particles.vy[i] = aos_particles[i].vy;
            soa_particles.vz[i] = aos_particles[i].vz;
            soa_particles.ax[i] = aos_particles[i].ax;
            soa_particles.ay[i] = aos_particles[i].ay;
            soa_particles.az[i] = aos_particles[i].az;
            soa_particles.m[i] = aos_particles[i].m;
            soa_particles.radius[i] = aos_particles[i].radius;
            soa_particles.density[i] = aos_particles[i].density;
        }
    }
    
    void runTest() {
        cout << "开始数据结构性能测试...\n" << endl;
        
        auto aos_time = testAoS();
        auto soa_time = testSoA();
        
        cout << "\n测试结果汇总:" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "数据结构" << setw(15) << "耗时(ms)" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "AoS" << setw(15) << aos_time << endl;
        cout << setw(20) << "SoA" << setw(15) << soa_time << endl;
        cout << "----------------------------------------" << endl;
        
        if(aos_time > 0) {
            double speedup = aos_time / soa_time;
            cout << "SoA性能提升: " << fixed << setprecision(2) 
                 << speedup << "x" << endl;
        }
    }
    
private:
    double testAoS() {
        cout << "测试AoS结构..." << endl;
        
        auto start = high_resolution_clock::now();
        volatile double total = 0;
        
        // 更复杂的计算模式
        for(size_t iter = 0; iter < iterations; iter++) {
            for(const auto& p : aos_particles) {
                // 计算动能
                float ke = 0.5f * p.m * (p.vx * p.vx + p.vy * p.vy + p.vz * p.vz);
                // 计算势能
                float pe = p.m * (p.x * p.x + p.y * p.y + p.z * p.z);
                // 计算加速度
                float a = sqrt(p.ax * p.ax + p.ay * p.ay + p.az * p.az);
                // 计算体积
                float volume = (4.0f/3.0f) * 3.14159f * p.radius * p.radius * p.radius;
                
                total += ke + pe + a + volume * p.density;
            }
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
    
    double testSoA() {
        cout << "测试SoA结构..." << endl;
        
        auto start = high_resolution_clock::now();
        volatile double total = 0;
        
        // 对应的SoA版本计算
        for(size_t iter = 0; iter < iterations; iter++) {
            for(size_t i = 0; i < particle_count; i++) {
                // 计算动能
                float ke = 0.5f * soa_particles.m[i] * (
                    soa_particles.vx[i] * soa_particles.vx[i] +
                    soa_particles.vy[i] * soa_particles.vy[i] +
                    soa_particles.vz[i] * soa_particles.vz[i]
                );
                // 计算势能
                float pe = soa_particles.m[i] * (
                    soa_particles.x[i] * soa_particles.x[i] +
                    soa_particles.y[i] * soa_particles.y[i] +
                    soa_particles.z[i] * soa_particles.z[i]
                );
                // 计算加速度
                float a = sqrt(
                    soa_particles.ax[i] * soa_particles.ax[i] +
                    soa_particles.ay[i] * soa_particles.ay[i] +
                    soa_particles.az[i] * soa_particles.az[i]
                );
                // 计算体积
                float volume = (4.0f/3.0f) * 3.14159f * 
                    soa_particles.radius[i] * soa_particles.radius[i] * soa_particles.radius[i];
                
                total += ke + pe + a + volume * soa_particles.density[i];
            }
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
};

int main() {
    DataStructureTester tester;
    tester.runTest();
    return 0;
} 