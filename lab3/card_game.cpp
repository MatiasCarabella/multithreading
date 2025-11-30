#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

// Platform detection
#ifdef _WIN32
    #define PLATFORM_WINDOWS
    #include <thread>
    #include <mutex>
    #include <condition_variable>
    #include <chrono>
#else
    #define PLATFORM_UNIX
    #include <unistd.h>
    #include <sys/wait.h>
    #include <cstdlib>
    #include <ctime>
#endif

using std::cerr;
using std::cin;
using std::cout;
using std::fixed;
using std::setprecision;
using std::setw;
using std::vector;

constexpr int MIN_PLAYERS = 2;
constexpr int MAX_PLAYERS = 10;
constexpr float WINNING_SCORE = 7.5f;

struct Player {
    int id;
    float score;
    bool standing;
    bool busted;
};

#ifdef PLATFORM_WINDOWS

// Windows implementation using threads
#include <thread>
#include <mutex>
#include <condition_variable>

using std::condition_variable;
using std::lock_guard;
using std::mutex;
using std::thread;
using std::unique_lock;

struct GameState {
    vector<Player> players;
    vector<float> deck;
    mutex mtx;
    condition_variable cv;
    int currentPlayer;
    bool gameOver;
    float currentCard;
    bool hasCard;
    
    GameState(int playerCount) 
        : players(playerCount), 
          deck{1, 2, 3, 4, 5, 6, 7, 0.5, 0.5, 0.5},
          currentPlayer(0),
          gameOver(false),
          currentCard(0),
          hasCard(false) {
        for (int i = 0; i < playerCount; ++i) {
            players[i] = {i, 0, false, false};
        }
    }
};

void playerThread(int id, GameState& state) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    while (true) {
        unique_lock<mutex> lock(state.mtx);
        
        state.cv.wait(lock, [&]() { 
            return state.gameOver || 
                   (state.currentPlayer == id && state.hasCard);
        });
        
        if (state.gameOver) break;
        
        Player& player = state.players[id];
        
        if (state.hasCard && state.currentPlayer == id) {
            player.score += state.currentCard;
            state.hasCard = false;
            
            cout << "Player " << id << " received: " << fixed 
                 << setprecision(1) << state.currentCard 
                 << " (Total: " << player.score << ")\n";
            
            if (player.score > WINNING_SCORE) {
                player.busted = true;
                cout << "Player " << id << " BUSTED!\n";
            } else {
                int decision = gen() % 2;
                if (decision == 1 || player.score == WINNING_SCORE) {
                    player.standing = true;
                    cout << "Player " << id << " stands\n";
                }
            }
            
            state.cv.notify_all();
        }
    }
}

void dealerThread(GameState& state) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    cout << "\n=== Game Starting (Windows - Threads) ===\n\n";
    
    while (true) {
        unique_lock<mutex> lock(state.mtx);
        
        bool allDone = true;
        for (const auto& player : state.players) {
            if (!player.standing && !player.busted) {
                allDone = false;
                break;
            }
        }
        
        if (allDone) {
            state.gameOver = true;
            state.cv.notify_all();
            break;
        }
        
        bool foundPlayer = false;
        for (size_t i = 0; i < state.players.size(); ++i) {
            int playerIdx = (state.currentPlayer + i) % state.players.size();
            if (!state.players[playerIdx].standing && !state.players[playerIdx].busted) {
                state.currentPlayer = playerIdx;
                foundPlayer = true;
                break;
            }
        }
        
        if (!foundPlayer) continue;
        
        Player& player = state.players[state.currentPlayer];
        if (!state.hasCard) {
            state.currentCard = state.deck[gen() % state.deck.size()];
            state.hasCard = true;
            
            state.cv.notify_all();
            state.cv.wait(lock, [&]() { return !state.hasCard || state.gameOver; });
        }
        
        state.currentPlayer = (state.currentPlayer + 1) % state.players.size();
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int runGame(int playerCount) {
    GameState state(playerCount);
    vector<thread> playerThreads;

    for (int i = 0; i < playerCount; ++i) {
        playerThreads.emplace_back(playerThread, i, std::ref(state));
    }

    thread dealer(dealerThread, std::ref(state));
    dealer.join();
    
    for (auto& t : playerThreads) {
        t.join();
    }

    // Display results
    float bestScore = -1;
    int winnerId = -1;

    cout << "\n=== Results ===\n";
    cout << "Player | Score  | Status\n";
    cout << "-----------------------------\n";

    for (const auto& player : state.players) {
        cout << setw(6) << player.id << " | " 
             << setw(6) << fixed << setprecision(1) << player.score << " | ";

        if (player.busted) {
            cout << "Busted\n";
        } else {
            cout << "Standing\n";
            if (player.score > bestScore) {
                bestScore = player.score;
                winnerId = player.id;
            }
        }
    }

    if (winnerId != -1) {
        cout << "\n🎉 Player " << winnerId << " wins with " << bestScore << " points!\n";
    } else {
        cout << "\nNo winner.\n";
    }

    return 0;
}

#else

// Unix/Linux implementation using pipes
constexpr int READ_END = 0;
constexpr int WRITE_END = 1;

void playerProcess(int id, int readPipe, int writePipe) {
    srand(time(nullptr) + id);
    float score = 0;
    bool standing = false;

    while (!standing && score <= WINNING_SCORE) {
        float card;
        read(readPipe, &card, sizeof(float));
        score += card;

        int decision;
        if (score > WINNING_SCORE) {
            decision = 2;
        } else {
            decision = rand() % 2;
        }

        if (decision == 1) standing = true;
        write(writePipe, &decision, sizeof(int));
        if (decision == 2) break;
    }

    close(readPipe);
    close(writePipe);
}

void startGame(int playerCount, const vector<int>& readPipes, const vector<int>& writePipes) {
    vector<Player> players(playerCount);
    vector<float> deck = {1, 2, 3, 4, 5, 6, 7, 0.5, 0.5, 0.5};
    srand(time(nullptr));

    cout << "\n=== Game Starting (Unix - Pipes) ===\n\n";

    for (int i = 0; i < playerCount; ++i) {
        players[i] = {i, 0, false, false};
    }

    bool gameOver = false;
    while (!gameOver) {
        for (int i = 0; i < playerCount; ++i) {
            if (!players[i].standing && !players[i].busted) {
                float card = deck[rand() % deck.size()];
                write(writePipes[i], &card, sizeof(float));

                int decision;
                read(readPipes[i], &decision, sizeof(int));
                players[i].score += card;

                if (decision == 1) {
                    players[i].standing = true;
                } else if (decision == 2) {
                    players[i].busted = true;
                }
            }
        }

        gameOver = true;
        for (const auto& player : players) {
            if (!player.standing && !player.busted) {
                gameOver = false;
                break;
            }
        }
    }

    // Display results
    float bestScore = -1;
    int winnerId = -1;

    cout << "\n=== Results ===\n";
    cout << "Player | Score  | Status\n";
    cout << "-----------------------------\n";

    for (const auto& player : players) {
        cout << setw(6) << player.id << " | " 
             << setw(6) << fixed << setprecision(1) << player.score << " | ";

        if (player.busted) {
            cout << "Busted\n";
        } else {
            cout << "Standing\n";
            if (player.score > bestScore) {
                bestScore = player.score;
                winnerId = player.id;
            }
        }
    }

    if (winnerId != -1) {
        cout << "\n🎉 Player " << winnerId << " wins with " << bestScore << " points!\n";
    } else {
        cout << "\nNo winner.\n";
    }

    for (int pipe : readPipes) close(pipe);
    for (int pipe : writePipes) close(pipe);
}

int runGame(int playerCount) {
    vector<int> playerToDealerPipes[2];
    vector<int> dealerToPlayerPipes[2];
    vector<pid_t> playerPids;

    for (int i = 0; i < playerCount; ++i) {
        int pipePlayerToDealer[2];
        int pipeDealerToPlayer[2];
        
        if (pipe(pipePlayerToDealer) == -1 || pipe(pipeDealerToPlayer) == -1) {
            cerr << "Error creating pipes\n";
            return 1;
        }

        playerToDealerPipes[READ_END].push_back(pipePlayerToDealer[READ_END]);
        playerToDealerPipes[WRITE_END].push_back(pipePlayerToDealer[WRITE_END]);
        dealerToPlayerPipes[READ_END].push_back(pipeDealerToPlayer[READ_END]);
        dealerToPlayerPipes[WRITE_END].push_back(pipeDealerToPlayer[WRITE_END]);

        pid_t pid = fork();

        if (pid == -1) {
            cerr << "Error creating child process\n";
            return 1;
        } else if (pid == 0) {
            close(pipePlayerToDealer[READ_END]);
            close(pipeDealerToPlayer[WRITE_END]);
            playerProcess(i, pipeDealerToPlayer[READ_END], pipePlayerToDealer[WRITE_END]);
            return 0;
        } else {
            close(pipePlayerToDealer[WRITE_END]);
            close(pipeDealerToPlayer[READ_END]);
            playerPids.push_back(pid);
        }
    }

    startGame(playerCount, playerToDealerPipes[READ_END], dealerToPlayerPipes[WRITE_END]);

    for (pid_t pid : playerPids) {
        waitpid(pid, nullptr, 0);
    }

    return 0;
}

#endif

int main() {
    int playerCount;
    
    cout << "=== Seven and a Half (Cross-Platform) ===\n";
    
    #ifdef PLATFORM_WINDOWS
    cout << "Detected: Windows\n";
    #else
    cout << "Detected: Unix/Linux\n";
    #endif
    
    do {
        cout << "\nEnter number of players (" << MIN_PLAYERS << "-" 
             << MAX_PLAYERS << "): ";
        cin >> playerCount;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            playerCount = 0;
        }
        
        if (playerCount < MIN_PLAYERS || playerCount > MAX_PLAYERS) {
            cout << "Invalid number.\n";
        }
    } while (playerCount < MIN_PLAYERS || playerCount > MAX_PLAYERS);

    return runGame(playerCount);
}
