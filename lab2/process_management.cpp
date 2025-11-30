#include <iostream>
#include <random>
#include <chrono>
#include <thread>

// Platform detection
#ifdef _WIN32
    #include <windows.h>
    #define PLATFORM_WINDOWS
#else
    #include <unistd.h>
    #include <sys/wait.h>
    #include <cstdlib>
    #include <ctime>
    #define PLATFORM_UNIX
#endif

using std::cerr;
using std::cin;
using std::cout;

constexpr int MAX_SLEEP_MS = 2000;

// Shared variable (behavior differs by platform)
int sharedCounter = 0;

#ifdef PLATFORM_WINDOWS

// Windows implementation using threads (since CreateProcess is complex for this demo)
#include <mutex>
#include <vector>

std::mutex counterMutex;

void writerProcess(int id) {
    cout << "Writer process " << id << " started (Windows thread)\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, MAX_SLEEP_MS);
    int sleepTime = dist(gen);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

    {
        std::lock_guard<std::mutex> lock(counterMutex);
        ++sharedCounter;
    }
    
    cout << "Writer process " << id << " completed\n";
}

void readerProcess(int id) {
    cout << "Reader process " << id << " started (Windows thread)\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, MAX_SLEEP_MS);
    int sleepTime = dist(gen);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

    {
        std::lock_guard<std::mutex> lock(counterMutex);
        cout << "Reader process " << id << " - Counter value: " << sharedCounter << "\n";
    }
}

int runProcessManagement(int writerCount, int readerCount) {
    cout << "\n=== Running on Windows (using threads) ===\n\n";
    
    std::vector<std::thread> threads;

    // Create writer threads
    for (int i = 0; i < writerCount; ++i) {
        threads.emplace_back(writerProcess, i);
    }

    // Create reader threads
    for (int i = 0; i < readerCount; ++i) {
        threads.emplace_back(readerProcess, i);
    }

    // Wait for all threads
    for (auto& t : threads) {
        t.join();
    }

    cout << "\nExecution completed.\n";
    return 0;
}

#else

// Unix/Linux implementation using fork()
void writerProcess(int id) {
    cout << "Writer process " << id << " started (Unix fork)\n";

    int sleepTime = rand() % (MAX_SLEEP_MS + 1);
    usleep(sleepTime * 1000);

    ++sharedCounter;
    
    exit(0);
}

void readerProcess(int id) {
    cout << "Reader process " << id << " started (Unix fork)\n";

    int sleepTime = rand() % (MAX_SLEEP_MS + 1);
    usleep(sleepTime * 1000);

    cout << "Reader process " << id << " - Counter value: " << sharedCounter << "\n";
    
    exit(0);
}

int runProcessManagement(int writerCount, int readerCount) {
    cout << "\n=== Running on Unix/Linux (using fork) ===\n\n";
    
    srand(static_cast<unsigned int>(time(nullptr)));

    // Create writer processes
    for (int i = 0; i < writerCount; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            writerProcess(i);
        } else if (pid < 0) {
            cerr << "Error creating writer process " << i << "\n";
        }
    }

    // Create reader processes
    for (int i = 0; i < readerCount; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            readerProcess(i);
        } else if (pid < 0) {
            cerr << "Error creating reader process " << i << "\n";
        }
    }

    // Wait for all child processes
    int status;
    while (wait(&status) > 0);

    cout << "\nExecution completed.\n";
    return 0;
}

#endif

int main() {
    int writerCount, readerCount;

    cout << "=== Cross-Platform Process Management Demo ===\n";
    
    #ifdef PLATFORM_WINDOWS
    cout << "Detected: Windows\n";
    #else
    cout << "Detected: Unix/Linux\n";
    #endif

    cout << "\nEnter number of writer processes (N): ";
    cin >> writerCount;
    if (cin.fail() || writerCount < 0) {
        cerr << "Invalid input\n";
        return 1;
    }

    cout << "Enter number of reader processes (M): ";
    cin >> readerCount;
    if (cin.fail() || readerCount < 0) {
        cerr << "Invalid input\n";
        return 1;
    }

    return runProcessManagement(writerCount, readerCount);
}
