//
//  main.cpp
//  doublePackge
//
//  Created by 无铭 on 2024/4/24.
//

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <math.h>
#include <functional>

#define Test_solution1
#define Test_solution2

using namespace std;

// MARK: - 定义物品
struct Thing {
    int value;
    int weight;
    
    Thing(int value = 0, int weight = 0): value(value), weight(weight){}
};

// MARK: 定义物品



// MARK: - 定义所有物品
class Things {
protected:
    vector<Thing> allThings;               // 所有物品
public:
    Things();
};

Things :: Things(){
    FILE *fp = NULL;
    string fileName = "weight.txt";
    cout << "正在从文件中读取数据。。。" << endl;
    fp = fopen(fileName.c_str(), "r");
    if(!fp){
        cerr << "打开文件失败" << endl;
        exit(-1);
    }
    char str1[15], str2[15];
    fscanf(fp, "%s %s", str1, str2);
    int value, weight;
    while(fscanf(fp, "%d %d", &value, &weight) > 0) {
        allThings.push_back(Thing(value, weight));
    }
}

// MARK: 定义所有物品



// MARK: - 定义解法 1
class solution1_dp: Things {
private:
    const int bagOneSize;               // 背包1的容量
    const int bagTwoSize;               // 背包2的容量
    vector<vector<vector<int>>> dp;     // dp 大小为 bagOneSize * bagTwoSize * 物品数量
    vector<int> ans;
    
public:
    solution1_dp(int bagOneSize = 10, int bagTwoSize = 8);
    void print();                       // 输出所有解
    void constructDp();                 // 构造 dp
    void constructAns();                // 构造 答案
    void solve();                       // 解决问题
    
#ifndef Test_solution1
    void display(){
        for(int i = 1; i <= allThings.size(); ++i){
            for(int j = 1; j <= bagOneSize; ++j){
                for(int k = 1; k <=  bagTwoSize; ++k){
                    cout << dp[i][j][k] << "\t";
                }
                cout << endl;
            }
            cout << "\n ------------- \n\n";
        }
    }
#endif
};

// MARK: - 构造函数
solution1_dp :: solution1_dp(int bagOneSize, int bagTwoSize): bagOneSize(bagOneSize), bagTwoSize(bagTwoSize){
    int cnt = (int)this->allThings.size();
    // 初始化
    dp.resize(cnt + 1);
    for(int i = 0; i <= cnt; ++i){
        dp[i].resize(bagOneSize + 1);
        for(int j = 0; j <= bagOneSize; ++j)
            dp[i][j].resize(bagTwoSize + 1, 0);
    }
    
    for(int i = 1; i <= cnt; ++i){
        for(int j = 1; j <= bagOneSize; ++j){
            if(j < allThings[i-1].weight)
                dp[i][j][0] = dp[i-1][j][0];
            else
                dp[i][j][0] = max(dp[i-1][j][0], dp[i-1][j-allThings[i-1].weight][0] + allThings[i-1].value);
        }
    }
    for(int i = 1; i <= cnt; ++i){
        for(int j = 1; j <= bagTwoSize; ++j){
            if(j < allThings[i-1].weight)
                dp[i][0][j] = dp[i-1][0][j];
            else
                dp[i][0][j] = max(dp[i-1][0][j], dp[i-1][0][j-allThings[i-1].weight] + allThings[i-1].value);
        }
    }
}
// MARK: 构造函数


// MARK: - 构造 Dp
void solution1_dp :: constructDp(){
    int cnt = (int)allThings.size();
    // 自底向上构造解
    for(int i = 1; i <= cnt; ++i){
        for(int j = 1; j <= bagOneSize; ++j){
            for(int k = 1; k <= bagTwoSize; ++k){
                // 两个背包都放不下时
                if(j < allThings[i-1].weight && k < allThings[i-1].weight)
                    dp[i][j][k] = dp[i-1][j][k];
                // 背包 2 放不下，就放背包 1
                else if (j >= allThings[i-1].weight && k < allThings[i-1].weight)
                    dp[i][j][k] = max(dp[i-1][j][k], dp[i-1][j-allThings[i-1].weight][k] + allThings[i-1].value);
                // 背包 1 放不下，就放背包 2
                else if (j < allThings[i-1].weight && k >= allThings[i-1].weight)
                    dp[i][j][k] = max(dp[i-1][j][k], dp[i-1][j][k-allThings[i-1].weight] + allThings[i-1].value);
                else {
                    int _k = max(1, k-allThings[i-1].weight);
                    int _j = max(1, j-allThings[i-1].weight);
                    dp[i][j][k] = max(dp[i-1][j][k], max(dp[i-1][j][_k], dp[i-1][_j][k]) + allThings[i-1].value);
                }
            }
        }
    }
}
// MARK: 构造 Dp

// MARK: - 构造 Ans
void solution1_dp :: constructAns(){
    int j = bagOneSize, k = bagTwoSize, n = (int)allThings.size();
    for(size_t d = n; d > 0; --d){
        if(dp[d][j][k] != dp[d-1][j][k]){
            int _j = j - this->allThings[d-1].weight;
            int _k = k - this->allThings[d-1].weight;
            if(_j < 0 && _k < 0)
                break;
            else if(_j < 0 && _k >= 0){
                ans[d-1] = 2;
                k -= this->allThings[d-1].weight;
            } else if(_j >= 0 && _k < 0){
                ans[d-1] = 1;
                j -= this->allThings[d-1].weight;
            } else {
                if(dp[d-1][_j][k] > dp[d-1][j][_k]){
                    ans[d-1] = 1;
                    j = j - allThings[d-1].weight;
                } else {
                    ans[d-1] = 2;
                    k = k - allThings[d-1].weight;
                }
            }
        } else {
            ans[d-1] = 0;
        }
    }
}
// MARK: 构造 Ans

// MARK: - 解决问题
void solution1_dp :: solve() {
    size_t size = allThings.size();
    if(ans.size() < size) {
        this->constructDp();
        ans.resize(size);
        this->constructAns();
    }
    cout << "最多能容纳价值为 " << dp[size][bagOneSize][bagTwoSize] << " 的物品\n";
    cout << "拾取物品的策略为：";
    Thing bagOneSum, bagTwoSum;
    for(int i = 0; i < size; ++i){
        cout << ans[i] << " ";
        int value = allThings[i].value;
        int weight = allThings[i].weight;
        if(ans[i] == 1){
            bagOneSum.value += value;
            bagOneSum.weight += weight;
        } else if(ans[i] == 2){
            bagTwoSum.value += value;
            bagTwoSum.weight += weight;
        } else {
            continue;
        }
    }
    cout << endl;
    
    cout << "其中背包 1 所选取的物品的总重量为：" << bagOneSum.weight << "\t\t总价值为：" << bagOneSum.value << endl;
    cout << "其中背包 2 所选取的物品的总重量为：" << bagTwoSum.weight << "\t\t总价值：" << bagTwoSum.value << endl;
}
// MARK: 解决问题
// MARK: -------------



// MARK: - 定义解法 2
class solution2_pso: Things {
private:
    const int bagOneSize;               // 背包1的容量
    const int bagTwoSize;               // 背包2的容量
    // 粒子群构造
    const int N;                        // 粒子个数
    const int maxIter;                  // 最大迭代次数
    const double c1;                    // 粒子的速度更新权重 -- 个体
    const double c2;                    // 粒子的速度更新权重 -- 群体
    const int dim;                      // 问题的维度
    vector<vector<double>> X;           // 粒子的位置 [0, dim) 为第一个背包， [dim, 2dim) 为第二个背包
    vector<vector<double>> V;           // 粒子的速度 [0, dim) 为第一个背包， [dim, 2dim) 为第二个背包
    vector<int> fitness;                // 粒子适应度
    vector<int> pbest;                  // 粒子的个体最大适应度
    vector<vector<double>> pbestPosition;// 粒子的个体最大适应度对应的位置 [0, dim) 为第一个背包， [dim, 2dim) 为第二个背包
    int gbest;                          // 粒子的全局最大适应度
    int gbestIndex;                     // 粒子的全局最大适应度对应的下标
    vector<int> ans;
    
    
public:
    solution2_pso(const int &bagOneSize = 10, const int &bagTwoSize = 8, const int N = 100, const int maxIter = 100, const double c1 = 1.5, const double c2 = 0.5, int dim = 5);   // 构造函数
    void updateFitness();              // 更新适应度
    void updatePbest();                // 更新粒子个体最大适应度及其对应位置
    void updateGbest();                // 更新粒子全体最大适应度及其对应下标
    void updateV(const int &iter);     // 更新粒子的速度
    void updateX();                    // 更新粒子的位置
    void getAns();                     // 获取 Ans
    void solve();                      // 解决问题
};

// MARK: - 构造函数
solution2_pso :: solution2_pso(const int &bagOneSize, const int &bagTwoSize, const int N, const int maxIter, const double c1, const double c2, const int dim): bagOneSize(bagOneSize), bagTwoSize(bagTwoSize), N(N), maxIter(maxIter), c1(c1), c2(c2), dim((int)this->allThings.size()){
    srand(unsigned(time(0)));
    rand();
    this->gbest = 0;
    this->gbestIndex = 0;
    this->fitness.resize(this->N);
    this->pbest.resize(this->N);
    this->X.resize(this->N);
    this->V.resize(this->N);
    this->pbestPosition.resize(this->N);
    
    for(int i = 0; i < this->N; ++i){
        this->X[i].resize(this->dim * 2);
        this->V[i].resize(this->dim * 2);
        this->pbestPosition[i].resize(this->dim * 2, 0);
        for(int d = 0; d < this->dim * 2; ++d){
            this->X[i][d] = (double)rand()/RAND_MAX;
            this->V[i][d] = (double)rand()/RAND_MAX * 10 - 5;
        }
    }
}

// MARK: - 更新适应度
void solution2_pso :: updateFitness(){
    for(int i = 0; i < this->N; ++i){
        int sumValue = 0, sumWeight1 = 0, sumWeight2 = 0;
        for(int d = 0; d < this->dim; ++d){
            if(X[i][d] > 0.5 && X[i][dim+d] > 0.5){
                sumValue = 0;
                break;
            } else if(X[i][d] > 0.5 && X[i][dim+d] <= 0.5) {
                sumWeight1 += this->allThings[d].weight;
                sumValue += this->allThings[d].value;
            } else if(X[i][dim+d] > 0.5 && X[i][d] <= 0.5) {
                sumWeight2 += this->allThings[d].weight;
                sumValue += this->allThings[d].value;
            }
            if(sumWeight1 > bagOneSize || sumWeight2 > bagTwoSize){
                sumValue = 0;
                break;
            }
        }
        this->fitness[i] = sumValue;
    }
}

// MARK: - 更新个体最大适应度
void solution2_pso :: updatePbest(){
    for(int i = 0; i < this->N; ++i){
        if(this->pbest[i] < this->fitness[i]){
            this->pbest[i] = this->fitness[i];
            for(int d = 0; d < this->dim * 2; ++d)
                this->pbestPosition[i][d] = this->X[i][d];
        }
    }
}

// MARK: - 更新全体最大适应度
void solution2_pso :: updateGbest(){
    for(int i = 0; i < this->N; ++i){
        if(this->gbest < this->pbest[i]){
            this->gbest = this->pbest[i];
            this->gbestIndex = i;
        }
    }
}

// MARK: - 更新速度
void solution2_pso :: updateV(const int &iter){
    for(int i = 0; i < this->N; ++i){
        for(int d = 0; d < this->dim * 2; ++d){
            V[i][d] = 1/double(iter) * V[i][d] + c1 * double(rand())/RAND_MAX * (pbestPosition[i][d]-X[i][d]) + c2 * double(rand())/RAND_MAX * (pbestPosition[gbestIndex][d]-X[i][d]);
            if(V[i][d] > 5)
                V[i][d] = 5;
            else if(V[i][d] < -5)
                V[i][d] = -5;
        }
    }
}

// MARK: - 更新位置
void solution2_pso :: updateX(){
    for(int i = 0; i < this->N; ++i)
        for(int d = 0; d < this->dim * 2; ++d){
            X[i][d] = 1/(1+exp(-V[i][d]));
        }
}

// MARK: - 获取 Ans
void solution2_pso :: getAns(){
    int size = (int)allThings.size();
    if(this->ans.size() < size){
        this->ans.resize(size);
        for(int iter = 1; iter < this->maxIter; ++iter){
            this->updateFitness();
            this->updatePbest();
            this->updateGbest();
            this->updateV(iter);
            this->updateX();
        }
    }
    // 开始构造 Ans
    if(this->gbestIndex >= 0){
        for(int d = 0; d < this->dim; ++d){
            if(pbestPosition[gbestIndex][d] > 0.5)
                this->ans[d] = 1;
            if(pbestPosition[gbestIndex][dim+d] > 0.5)
                this->ans[d] = 2;
            if(pbestPosition[gbestIndex][d] <= 0.5 && pbestPosition[gbestIndex][dim+d] <= 0.5)
                ans[d] = 0;
        }
    } else {
        ans.resize(size, 0);
    }
}
// MARK: 获取 Ans

// MARK: - 解决问题
void solution2_pso :: solve(){
    int size = (int)allThings.size();
    this->getAns();
    cout << "最多能容纳价值为 " << this->gbest << " 的物品\n";
    cout << "拾取物品的策略为：";
    Thing bagOneSum, bagTwoSum;
    for(int i = 0; i < size; ++i){
        cout << ans[i] << " ";
        int value = allThings[i].value;
        int weight = allThings[i].weight;
        if(ans[i] == 1){
            bagOneSum.value += value;
            bagOneSum.weight += weight;
        } else if(ans[i] == 2){
            bagTwoSum.value += value;
            bagTwoSum.weight += weight;
        } else {
            continue;
        }
    }
    cout << endl;
    cout << "其中背包 1 所选取的物品的总重量为：" << bagOneSum.weight << "\t\t总价值为：" << bagOneSum.value << endl;
    cout << "其中背包 2 所选取的物品的总重量为：" << bagTwoSum.weight << "\t\t总价值：" << bagTwoSum.value << endl;
}

// MARK: 定义解法 2
int main(int argc, const char * argv[]) {
    int bagOneSize = 203, bagTwoSize = 156;
    clock_t t1, t2, t3;
#ifdef Test_solution1
    cout << "解法1: ";
    t1 = clock();
    solution1_dp a(bagOneSize, bagTwoSize);
    a.solve();
    t2 = clock();
    cout << "DP 耗时：" << double(t2 - t1)/CLOCKS_PER_SEC << endl;
    cout << "-----------" << endl;
#endif
    
#ifdef Test_solution2
    cout << " 解法2: ";
    solution2_pso(bagOneSize, bagTwoSize).solve();
    t3 = clock();
    cout << "PSO 耗时：" << double(t3 - t2)/CLOCKS_PER_SEC << endl;
    cout << "-----------" << endl;
#endif
    return 0;
}
