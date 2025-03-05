#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <vector>
#include "../skiplist.h"

const int NUM_THREADS = 100;       // 线程数量
const int TEST_COUNT = 100000;  // 测试次数

// 线程函数：插入元素
void insertElement(SkipList<int, std::string>& skipList, int start, int end) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, TEST_COUNT - 1);

    for (int i = start; i < end; ++i) {
        skipList.insert_element(dis(gen), "a");
    }
}

// 线程函数：查找元素
void getElement(SkipList<int, std::string>& skipList, int start, int end) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, TEST_COUNT - 1);

    for (int i = start; i < end; ++i) {
        skipList.search_element(dis(gen));
    }
}

int main() {
    SkipList<int, std::string> skipList(18);  // 创建跳表，最大层数为 18

    // 测试插入操作
    {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        int chunkSize = TEST_COUNT / NUM_THREADS;

        for (int i = 0; i < NUM_THREADS; ++i) {
            int startIdx = i * chunkSize;
            int endIdx = (i == NUM_THREADS - 1) ? TEST_COUNT : startIdx + chunkSize;
            threads.emplace_back(insertElement, std::ref(skipList), startIdx, endIdx);
        }

        for (auto& t : threads) {
            t.join();
        }

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "Insert elapsed: " << elapsed.count() << " seconds\n";
    }

    // 测试查找操作
    {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        int chunkSize = TEST_COUNT / NUM_THREADS;

        for (int i = 0; i < NUM_THREADS; ++i) {
            int startIdx = i * chunkSize;
            int endIdx = (i == NUM_THREADS - 1) ? TEST_COUNT : startIdx + chunkSize;
            threads.emplace_back(getElement, std::ref(skipList), startIdx, endIdx);
        }

        for (auto& t : threads) {
            t.join();
        }

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "Search elapsed: " << elapsed.count() << " seconds\n";
    }

    return 0;
}