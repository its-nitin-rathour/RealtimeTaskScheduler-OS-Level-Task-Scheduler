# 🕒 Realtime Task Scheduler – OS-Level Process Management

A **multithreaded task scheduler** implemented in **C++** to simulate **OS-level process scheduling**.  
This project demonstrates **priority-based scheduling with aging**, **First Come First Serve (FCFS)**, **Shortest Job First (SJF)**, and **Round Robin (RR)** algorithms.  
It measures **dispatch time, waiting time, and turnaround time** while ensuring **race-free execution** using `mutex` and `condition_variable`.

Engineered a multithreaded task scheduler in C++ with priority-based scheduling and aging to simulate OS-level process management.  Handled 10+ concurrent tasks using 4 worker threads, ensuring race-free execution with mutexes and condition vari- ables. • Achieved &lt;50ms average task dispatch time with optimized scheduling and modular design.
---

## 🚀 Features
- ✅ **Multithreaded execution** – each task runs in its own thread.  
- ✅ **Scheduling Algorithms Implemented**:
  - First Come First Serve (FCFS)  
  - Shortest Job First (SJF)  
  - Round Robin (time quantum based)  
  - Priority Scheduling with **Aging** (to avoid starvation)  
- ✅ **Race-free execution** using `std::mutex` and `std::condition_variable`.  
- ✅ **Performance metrics**:
  - Total Dispatch Time  
  - Average Dispatch Time per Task  
  - Execution Order Visualization  
- ✅ Handles **10+ concurrent tasks** with average dispatch time **<50ms**.

---

## 🛠 Tech Stack
- **Language:** C++17  
- **Concurrency:** `std::thread`, `std::mutex`, `std::condition_variable`  
- **Build Tool:** g++ / clang++  

---

## 📂 Project Structure
```
Realtime-Task-Scheduler/
│── scheduler.cpp       # Main implementation
│── README.md           # Documentation
│── LICENSE             # (optional)
```

---

## ▶️ How to Run

### 1. Clone the Repository
```bash
git clone https://github.com/your-username/realtime-task-scheduler.git
cd realtime-task-scheduler
```

### 2. Compile
```bash
g++ scheduler.cpp -o scheduler -pthread -std=c++17
```

### 3. Run
```bash
./scheduler
```

---

## 📝 Example Output
```
--- FCFS Scheduling ---
Task 1 started at 0 and finished at 20
Task 2 started at 20 and finished at 35
...

--- Priority Scheduling with Aging (Multithreaded) ---
[START] Task 3 (Priority: 3, Burst: 30 ms)
[END]   Task 3 completed.
...
All tasks completed.
Total Dispatch Time = 115 ms
Average Dispatch Time per Task = 28.7 ms
```

---

## 📚 Learning Outcomes
- Gained hands-on experience with **process scheduling algorithms**.  
- Understood **OS concepts** like **synchronization, starvation, and aging**.  
- Applied **multithreading and synchronization primitives** in real-world scenarios.  
- Optimized scheduling to ensure **low-latency task dispatch**.  

---

## 💡 Future Improvements
- Add **GUI visualization** for Gantt charts.  
- Extend to **I/O bound tasks** simulation.  
- Integrate **dynamic task arrival** during runtime.  

---

## 👤 Author
**Mansi Gupta**  
- 🎓 M.Tech Student – Software Engineering  
- 💼 Aspiring Routing/IP Engineer  
- 🌐 [LinkedIn](https://www.linkedin.com/in/gupta-mansi-ktl/) | [GitHub](https://github.com/Mansi2202)  

---
