// Define task and input
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <queue>
#include <iomanip>

using namespace std;

struct Task {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;

    // For metrics
    int startTime = -1;
    int finishTime = -1;
};

// Global sync primitives
mutex mtx;
condition_variable cv;
bool ready = false;

// Thread Function for Task Execution
void executeTask(Task &t) {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [] { return ready; }); // wait until scheduler signals

    t.startTime = chrono::duration_cast<chrono::milliseconds>(
                      chrono::high_resolution_clock::now().time_since_epoch()
                  ).count();

    cout << "[START] Task " << t.id
         << " (Priority: " << t.priority
         << ", Burst: " << t.burstTime << " ms)\n";

    this_thread::sleep_for(chrono::milliseconds(t.burstTime)); // simulate work

    t.finishTime = chrono::duration_cast<chrono::milliseconds>(
                       chrono::high_resolution_clock::now().time_since_epoch()
                   ).count();

    cout << "[END]   Task " << t.id << " completed.\n";
}

// Step 2: Implement Scheduling Algorithms
// FCFS Scheduling

void fcfsScheduling(vector<Task>& tasks) {
    sort(tasks.begin(), tasks.end(), [](Task a, Task b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    cout << "\n--- FCFS Scheduling ---\n";
    for (auto &task : tasks) {
        if (currentTime < task.arrivalTime) currentTime = task.arrivalTime;
        cout << "Task " << task.id << " started at " << currentTime 
             << " and finished at " << currentTime + task.burstTime << endl;
        currentTime += task.burstTime;
    }
}

// SJF Scheduling
void sjfScheduling(vector<Task> tasks) {
    sort(tasks.begin(), tasks.end(), [](Task a, Task b) {
        return a.burstTime < b.burstTime;
    });

    int currentTime = 0;
    cout << "\n--- SJF Scheduling ---\n";
    for (auto &task : tasks) {
        if (currentTime < task.arrivalTime) currentTime = task.arrivalTime;
        cout << "Task " << task.id << " started at " << currentTime 
             << " and finished at " << currentTime + task.burstTime << endl;
        currentTime += task.burstTime;
    }
}

// Round Robin Quantum = 2
void roundRobinScheduling(vector<Task> tasks, int quantum = 2) {
    queue<Task> q;
    int currentTime = 0;
    cout << "\n--- Round Robin Scheduling ---\n";

    for (auto &task : tasks) q.push(task);

    while (!q.empty()) {
        Task t = q.front(); q.pop();

        int execTime = min(quantum, t.burstTime);
        cout << "Task " << t.id << " executed from " << currentTime 
             << " to " << currentTime + execTime << endl;

        currentTime += execTime;
        t.burstTime -= execTime;

        if (t.burstTime > 0) q.push(t); // re-queue
    }
}

// Priority Scheduler with Aging (Multithreaded)
void prioritySchedulingWithAging(vector<Task> tasks) {
    cout << "\n--- Priority Scheduling with Aging (Multithreaded) ---\n";

    vector<thread> threads;
    vector<int> waitingTimes, turnaroundTimes;

    auto globalStart = chrono::high_resolution_clock::now();

    // sort initially by arrival time
    sort(tasks.begin(), tasks.end(), [](Task a, Task b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    while (!tasks.empty()) {
        // select tasks that have arrived
        vector<Task> readyQueue;
        for (auto &t : tasks) {
            if (t.arrivalTime <= currentTime) readyQueue.push_back(t);
        }

        if (readyQueue.empty()) {
            currentTime++;
            continue; // wait until a task arrives
        }

        // aging step: increment waiting task priorities
        for (auto &t : tasks) {
            if (t.arrivalTime <= currentTime) t.priority++;
        }

        // pick the highest priority task
        auto it = max_element(tasks.begin(), tasks.end(), [](Task a, Task b) {
            return a.priority < b.priority;
        });

        Task current = *it;
        tasks.erase(it);

        // launch this task in a thread
        threads.emplace_back(executeTask, ref(current));

        // signal all threads to start
        {
            lock_guard<mutex> lock(mtx);
            ready = true;
        }
        cv.notify_all();

        // simulate advancing time
        currentTime += current.burstTime;

        // metrics
        int waitingTime = currentTime - current.arrivalTime - current.burstTime;
        int turnaroundTime = currentTime - current.arrivalTime;
        waitingTimes.push_back(waitingTime);
        turnaroundTimes.push_back(turnaroundTime);

        // formatted row
        cout << setw(5) << current.id
             << setw(10) << current.arrivalTime
             << setw(10) << current.burstTime
             << setw(10) << waitingTime
             << setw(10) << turnaroundTime << endl;
    }

    // wait for completion
    for (auto &th : threads) th.join();

    auto globalEnd = chrono::high_resolution_clock::now();
    auto totalTime = chrono::duration_cast<chrono::milliseconds>(globalEnd - globalStart).count();

    // calculate averages
    double avgWT = accumulate(waitingTimes.begin(), waitingTimes.end(), 0.0) / waitingTimes.size();
    double avgTAT = accumulate(turnaroundTimes.begin(), turnaroundTimes.end(), 0.0) / turnaroundTimes.size();

    cout << "\nAll tasks completed.\n";
    cout << "Total Dispatch Time = " << totalTime << " ms\n";
    cout << "Average Waiting Time = " << avgWT << " ms\n";
    cout << "Average Turnaround Time = " << avgTAT << " ms\n";
}

// Main Function
int main() {
    vector<Task> tasks;
    // Generate 12 tasks with small burst times (1–5 ms) to keep dispatch <50ms
    for (int i = 1; i <= 12; i++) {
        tasks.push_back({i, i * 2, (i * 3) % 5 + 1, i % 4 + 1});
    }

    cout << left << setw(5) << "ID" 
         << setw(10) << "Arrival" 
         << setw(10) << "Burst" 
         << setw(10) << "Waiting" 
         << setw(10) << "TAT" << endl;
    cout << "---------------------------------------------\n";

    fcfsScheduling(tasks);
    sjfScheduling(tasks);
    roundRobinScheduling(tasks, 2);
    prioritySchedulingWithAging(tasks);

    return 0;
}