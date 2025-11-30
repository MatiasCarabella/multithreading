#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <array>

using std::array;
using std::cout;
using std::string;
using std::thread;

// Prints a greeting message multiple times with a delay between each print
void printGreeting(const string& message, int delayMs, int repetitions) {
    for (int i = 0; i < repetitions; ++i) {
        cout << message << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

int main() {
    constexpr int THREAD_COUNT = 3;
    array<thread, THREAD_COUNT> threads;
    
    // Create threads using array indexing
    threads[0] = thread(&printGreeting, "I am A", 100, 10);
    threads[1] = thread(&printGreeting, "\tI am B", 150, 15);
    threads[2] = thread(&printGreeting, "\t\tI am C", 300, 5);

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    cout << "End\n";
    return 0;
}
