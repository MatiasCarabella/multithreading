#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <random>
#include <array>
#include <memory>

using std::array;
using std::cout;
using std::make_unique;
using std::mt19937;
using std::random_device;
using std::thread;
using std::uniform_int_distribution;
using std::unique_ptr;

constexpr int THREAD_COUNT = 15;
constexpr int MIN_DELAY_MS = 100;
constexpr int MAX_DELAY_MS = 1000;
constexpr int MIN_REPETITIONS = 5;
constexpr int MAX_REPETITIONS = 20;

// Encapsulates the behavior of a thread process
class ThreadProcess {
private:
    int id_;
    int delayMs_;
    int repetitions_;

public:
    ThreadProcess(int id, int delayMs, int repetitions) 
        : id_(id), delayMs_(delayMs), repetitions_(repetitions) {}

    // Executes the thread's main behavior
    void execute() {
        for (int i = 0; i < repetitions_; ++i) {
            cout << "I am thread " << id_ << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs_));
        }
    }
};

int main() {
    array<thread, THREAD_COUNT> threads;
    array<unique_ptr<ThreadProcess>, THREAD_COUNT> processes;

    // Random number generation setup
    random_device randomDevice;
    mt19937 generator(randomDevice());
    uniform_int_distribution<> delayDistribution(MIN_DELAY_MS, MAX_DELAY_MS);
    uniform_int_distribution<> repetitionDistribution(MIN_REPETITIONS, MAX_REPETITIONS);

    // Create and launch threads with random parameters
    for (int i = 0; i < THREAD_COUNT; ++i) {
        int delay = delayDistribution(generator);
        int repetitions = repetitionDistribution(generator);
        processes[i] = std::make_unique<ThreadProcess>(i + 1, delay, repetitions);
        threads[i] = thread(&ThreadProcess::execute, processes[i].get());
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    cout << "End\n";
    return 0;
}
