@echo off
REM Build and Run Script for Concurrent Programming Examples

echo ========================================
echo Concurrent Programming in C++ - Builder
echo ========================================
echo.

:menu
echo Select a program to compile and run:
echo.
echo Lab 1 - Threading Fundamentals:
echo   1. Simple Threads - Basic thread creation
echo   2. Thread Array - Managing threads in arrays
echo   3. Random Threads - Dynamic thread creation
echo   4. Thread Class - OOP approach to threading
echo.
echo Lab 2 - Synchronization:
echo   5. Mutex Synchronization - Reader-writer pattern
echo   6. Process Management - Cross-platform
echo.
echo Lab 3 - IPC:
echo   7. Card Game - Cross-platform
echo.
echo   8. Clean all compiled files
echo   0. Exit
echo.

set /p choice="Enter your choice (0-8): "

if "%choice%"=="1" goto tp1_1
if "%choice%"=="2" goto tp1_2
if "%choice%"=="3" goto tp1_3
if "%choice%"=="4" goto tp1_4
if "%choice%"=="5" goto tp2_1
if "%choice%"=="6" goto tp2_cross
if "%choice%"=="7" goto tp3_cross
if "%choice%"=="8" goto clean
if "%choice%"=="0" goto end
echo Invalid choice. Please try again.
echo.
goto menu

:tp1_1
echo.
echo Compiling lab1/simple_threads.cpp...
g++ -std=c++17 -pthread lab1/simple_threads.cpp -o lab1/simple_threads.exe
if %errorlevel% equ 0 (
    echo Compilation successful! Running...
    echo.
    lab1\simple_threads.exe
) else (
    echo Compilation failed!
)
echo.
pause
goto menu

:tp1_2
echo.
echo Compiling lab1/thread_array.cpp...
g++ -std=c++17 -pthread lab1/thread_array.cpp -o lab1/thread_array.exe
if %errorlevel% equ 0 (
    echo Compilation successful! Running...
    echo.
    lab1\thread_array.exe
) else (
    echo Compilation failed!
)
echo.
pause
goto menu

:tp1_3
echo.
echo Compiling lab1/random_threads.cpp...
g++ -std=c++17 -pthread lab1/random_threads.cpp -o lab1/random_threads.exe
if %errorlevel% equ 0 (
    echo Compilation successful! Running...
    echo.
    lab1\random_threads.exe
) else (
    echo Compilation failed!
)
echo.
pause
goto menu

:tp1_4
echo.
echo Compiling lab1/thread_class.cpp...
g++ -std=c++17 -pthread lab1/thread_class.cpp -o lab1/thread_class.exe
if %errorlevel% equ 0 (
    echo Compilation successful! Running...
    echo.
    lab1\thread_class.exe
) else (
    echo Compilation failed!
)
echo.
pause
goto menu

:tp2_1
echo.
echo Compiling lab2/mutex_synchronization.cpp...
g++ -std=c++17 -pthread lab2/mutex_synchronization.cpp -o lab2/mutex_synchronization.exe
if %errorlevel% equ 0 (
    echo Compilation successful! Running...
    echo.
    lab2\mutex_synchronization.exe
) else (
    echo Compilation failed!
)
echo.
pause
goto menu

:tp2_cross
echo.
echo Compiling lab2/process_management.cpp (cross-platform)...
g++ -std=c++17 -pthread lab2/process_management.cpp -o lab2/process_management.exe
if %errorlevel% equ 0 (
    echo Compilation successful! Running...
    echo.
    lab2\process_management.exe
) else (
    echo Compilation failed!
)
echo.
pause
goto menu

:tp3_cross
echo.
echo Compiling lab3/card_game.cpp (cross-platform)...
g++ -std=c++17 -pthread lab3/card_game.cpp -o lab3/card_game.exe
if %errorlevel% equ 0 (
    echo Compilation successful! Running...
    echo.
    lab3\card_game.exe
) else (
    echo Compilation failed!
)
echo.
pause
goto menu



:clean
echo.
echo Cleaning compiled files...
del /Q lab1\*.exe 2>nul
del /Q lab2\*.exe 2>nul
del /Q lab3\*.exe 2>nul
echo Clean complete!
echo.
pause
goto menu

:end
echo.
echo Goodbye!
exit /b 0
