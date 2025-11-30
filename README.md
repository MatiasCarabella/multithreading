# Concurrent Programming in C++

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-green?style=flat)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Unix-lightgrey?style=flat)
![Threads](https://img.shields.io/badge/concurrency-threads%20%7C%20processes-blue?style=flat)

A comprehensive collection of C++ programs demonstrating concurrent programming concepts, including multithreading, process management, inter-process communication (IPC), and synchronization primitives.

## 📋 Overview

This repository contains practical implementations of concurrent programming patterns in C++, organized into three main sections covering fundamental to advanced topics in parallel computing.

## 🗂️ Project Structure

### Lab 1: Threading Fundamentals
Basic multithreading examples demonstrating thread creation, management, and execution patterns.

- **simple_threads.cpp** - Simple thread creation with multiple concurrent threads
- **thread_array.cpp** - Thread management using arrays
- **random_threads.cpp** - Dynamic thread creation with random parameters
- **thread_class.cpp** - Object-oriented approach to thread management using classes

### Lab 2: Synchronization & Process Management
Advanced examples showcasing thread synchronization and process-based concurrency.

- **mutex_synchronization.cpp** - Thread synchronization using mutexes (reader-writer pattern)
- **process_management.cpp** - 🌐 Cross-platform process management (auto-detects OS)

### Lab 3: Inter-Process Communication
Complex application demonstrating IPC using pipes and threads.

- **card_game.cpp** - 🌐 Cross-platform "Seven and a Half" game (auto-detects OS)

## 🚀 Getting Started

### Prerequisites

- C++ compiler with C++17 support or higher (GCC 7+, Clang 5+, MSVC 2017+)
- POSIX-compliant system for process-based examples (Linux, macOS, Unix)
- Basic understanding of concurrent programming concepts

### Compilation

Each program can be compiled individually:

```bash
# For threading examples (Lab 1 & Lab 2/mutex_synchronization)
g++ -std=c++17 -pthread lab1/simple_threads.cpp -o simple_threads

# For process-based examples (Lab 2/process_fork & Lab 3)
g++ -std=c++17 lab2/process_fork.cpp -o process_fork
```

### Running Examples

```bash
# Clone the repository
git clone https://github.com/MatiasCarabella/multithreading-cpp.git
cd multithreading-cpp

# Compile and run an example
g++ -std=c++17 -pthread lab1/simple_threads.cpp -o simple_threads
./simple_threads
```

## 🎯 Key Concepts Demonstrated

- **Thread Creation & Management** - Using `std::thread` for concurrent execution
- **Synchronization Primitives** - Mutexes, lock guards, condition variables
- **Cross-Platform Development** - Automatic OS detection with preprocessor directives
- **Process Management** - Platform-specific implementations (`fork()` vs threads)
- **Inter-Process Communication** - Pipes (Unix) and shared memory (Windows)
- **Modern C++ Features** - Smart pointers, RAII, C++17 standards
- **Clean Code Practices** - SOLID principles and readable code structure

## 📚 Learning Path

1. Start with **Lab 1** to understand basic threading concepts
2. Progress to **Lab 2** for synchronization and process management
3. Explore **Lab 3** for advanced IPC patterns

## 🛠️ Best Practices Applied

- No `using namespace std` - Explicit namespace usage for clarity
- Smart pointers (`std::unique_ptr`) instead of raw pointers
- RAII for resource management
- `constexpr` for compile-time constants
- Modern C++ random number generation
- Exception handling and error checking
- Clear naming conventions and documentation

## 📖 Additional Resources

- [C++ Reference - Threading](https://en.cppreference.com/w/cpp/thread)
- [C++ Concurrency in Action](https://www.manning.com/books/c-plus-plus-concurrency-in-action-second-edition)
- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👤 Author

**Matías Carabella**

## 🤝 Contributing

Contributions, issues, and feature requests are welcome! Feel free to check the issues page.

---

⭐ If you find this repository helpful, please consider giving it a star!
