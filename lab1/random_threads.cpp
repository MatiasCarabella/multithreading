#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <random>
#include <array>

using std::array;
using std::cout;
using std::mt19937;
using std::random_device;
using std::thread;
using std::uniform_int_distribution;

constexpr int THREAD_COUNT = 15;
constexpr int MIN_DELAY_MS = 100;
constexpr int MAX_DELAY_MS = 1000;
constexpr int MIN_REPETITIONS = 5;
constexpr int MAX_REPETITIONS = 20;

// Prints a thread identification message multiple times with a delay
void printThreadMessage(int threadId, int delayMs, int repetitions) {
    for (int i = 0; i < repetitions; ++i) {
        cout << "I am thread " << threadId << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

int main() {
    array<thread, THREAD_COUNT> threads;

    // Random number generation setup
    random_device randomDevice;
    mt19937 generator(randomDevice());
    uniform_int_distribution<> delayDistribution(MIN_DELAY_MS, MAX_DELAY_MS);
    uniform_int_distribution<> repetitionDistribution(MIN_REPETITIONS, MAX_REPETITIONS);

    // Create and launch threads with random parameters
    for (int i = 0; i < THREAD_COUNT; ++i) {
        int delay = delayDistribution(generator);
        int repetitions = repetitionDistribution(generator);
        threads[i] = thread(&printThreadMessage, i + 1, delay, repetitions);
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    cout << "End\n";
    return 0;
}
