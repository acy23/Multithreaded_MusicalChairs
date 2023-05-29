#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include "IntQueueHW6.h"
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

mutex mtx;             // Mutex for synchronization
int numPlayers;             // Number of players in the game

string getCurrentTime() {
    auto currentTime = chrono::system_clock::now();
    time_t currentTime_t = chrono::system_clock::to_time_t(currentTime);

    tm currentTm;
#ifdef _WIN32
    localtime_s(&currentTm, &currentTime_t);  // For Windows
#else
    localtime_r(&currentTime_t, &currentTm);  // For non-Windows systems
#endif

    int hours = currentTm.tm_hour;
    int minutes = currentTm.tm_min;
    int seconds = currentTm.tm_sec;

    // Create a string representing the current time in the desired format
    ostringstream oss;
    oss << setfill('0') << setw(2) << hours << ":"
        << setfill('0') << setw(2) << minutes << ":"
        << setfill('0') << setw(2) << seconds;

    return oss.str();
}



void threadFunction(int threadId, IntQueueHW6 &queue) {
    // Acquire the lock on the mutex
    unique_lock<mutex> lock(mtx);

    // Access the shared queue
    // Perform the desired operations (e.g., enqueue, dequeue)
    if (queue.isFull()) {
        
        cout << "Player " << threadId << " couldn't capture a chair." << endl;
        return;
    }
    queue.enqueue(threadId);
    cout << "Player " << threadId << " captured a chair at " << getCurrentTime() << "." << endl;

    // Release the lock on the mutex
    lock.unlock();
}

int main()
{
    cout << "Welcome to Musical Chairs game!" << endl;
    cout << "Enter the number of players: ";
    cin >> numPlayers;
    cout << "Game Start!" << endl;


    IntQueueHW6 chairQueue(numPlayers - 1);  // Queue to store the chairs
    vector<thread> threads;
    for (int i = 0; i < numPlayers; ++i) {
        threads.emplace_back(threadFunction, i, ref(chairQueue));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
