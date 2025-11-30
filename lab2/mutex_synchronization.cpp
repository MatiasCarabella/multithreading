#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <vector>
#include <stdexcept>

using std::cerr;
using std::cin;
using std::cout;
using std::exception;
using std::invalid_argument;
using std::lock_guard;
using std::mt19937;
using std::mutex;
using std::random_device;
using std::thread;
using std::uniform_int_distribution;
using std::vector;

constexpr int MAX_SLEEP_MS = 2000;

// Shared variable between writer and reader threads
int sharedCounter = 0;

// Mutex to synchronize access to the shared variable
mutex counterMutex;

// Writer thread: increments the shared counter
void writerThread(int id) {
    try {
        cout << "Writer thread " << id << " started\n";

        // Generate random sleep time between 0 and 2 seconds
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, MAX_SLEEP_MS);
        int sleepTime = dist(gen);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

        // Increment shared counter with mutex protection
        {
            lock_guard<mutex> guard(counterMutex);
            ++sharedCounter;
        }
    } catch (const exception& e) {
        cerr << "Exception in writer thread " << id << ": " << e.what() << "\n";
    }
}

// Reader thread: reads and displays the shared counter value
void readerThread(int id) {
    try {
        cout << "Reader thread " << id << " started\n";

        // Generate random sleep time between 0 and 2 seconds
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, MAX_SLEEP_MS);
        int sleepTime = dist(gen);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

        // Read and display shared counter with mutex protection
        {
            lock_guard<mutex> guard(counterMutex);
            cout << "Shared counter value: " << sharedCounter << "\n";
        }
    } catch (const exception& e) {
        cerr << "Exception in reader thread " << id << ": " << e.what() << "\n";
    }
}

int main() {
    try {
        int writerCount, readerCount;

        // Get number of writer threads
        cout << "Enter number of writer threads (N): ";
        cin >> writerCount;
        if (cin.fail() || writerCount < 0) {
            throw invalid_argument("Invalid input. Please enter positive integers.");
        }

        // Get number of reader threads
        cout << "Enter number of reader threads (M): ";
        cin >> readerCount;
        if (cin.fail() || readerCount < 0) {
            throw invalid_argument("Invalid input. Please enter positive integers.");
        }

        // Use vectors for automatic memory management
        vector<thread> writerThreads;
        vector<thread> readerThreads;
        
        writerThreads.reserve(writerCount);
        readerThreads.reserve(readerCount);

        // Create writer threads
        for (int i = 0; i < writerCount; ++i) {
            writerThreads.emplace_back(writerThread, i);
        }

        // Create reader threads
        for (int i = 0; i < readerCount; ++i) {
            readerThreads.emplace_back(readerThread, i);
        }

        // Wait for all writer threads to complete
        for (auto& thread : writerThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        // Wait for all reader threads to complete
        for (auto& thread : readerThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        cout << "Execution completed.\n";

    } catch (const exception& e) {
        cerr << "Exception in main: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
