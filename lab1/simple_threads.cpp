#include <iostream>
#include <thread>
#include <string>
#include <chrono>

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
    // Create three threads with different messages, delays, and repetition counts
    thread thread1(&printGreeting, "I am A", 100, 10);
    thread thread2(&printGreeting, "\tI am B", 150, 15);
    thread thread3(&printGreeting, "\t\tI am C", 300, 5);
    
    // Wait for all threads to complete
    thread1.join();
    thread2.join();
    thread3.join();
    
    cout << "End\n";
    return 0;
}
