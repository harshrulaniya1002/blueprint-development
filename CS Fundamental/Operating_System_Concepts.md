# Operating System Concepts - Complete Interview Guide

---

# Table of Contents

1. [Introduction to Operating Systems](#1-introduction-to-operating-systems)
2. [System Calls](#2-system-calls)
3. [Process Management](#3-process-management)
4. [CPU Scheduling](#4-cpu-scheduling)
5. [Threads & Multithreading](#5-threads--multithreading)
6. [Process Synchronization](#6-process-synchronization)
7. [Deadlocks](#7-deadlocks)
8. [Memory Management](#8-memory-management)
9. [Virtual Memory](#9-virtual-memory)
10. [File Systems](#10-file-systems)
11. [Disk Scheduling](#11-disk-scheduling)
12. [I/O Management](#12-io-management)
13. [Important Interview Questions](#13-important-interview-questions)

---

# 1. Introduction to Operating Systems

## What is an Operating System?

An **Operating System (OS)** is system software that acts as an intermediary between the user and hardware. It manages hardware resources and provides services for application programs.

```
+---------------------------+
|     User Applications     |
+---------------------------+
|     Operating System      |
+---------------------------+
|   Computer Hardware       |
+---------------------------+
```

## Functions of an OS

| Function | Description | Example |
|----------|-------------|---------|
| **Process Management** | Creates, schedules, terminates processes | Running Chrome + VS Code simultaneously |
| **Memory Management** | Allocates/deallocates RAM | Loading a program into memory |
| **File Management** | Creates, deletes, organizes files | NTFS, ext4 file systems |
| **I/O Management** | Controls input/output devices | Printer spooling |
| **Security** | Authentication, access control | Login passwords, file permissions |

## Types of Operating Systems

- **Batch OS**: Jobs collected in batches, no user interaction. Example: Early IBM mainframes.
- **Multiprogramming OS**: Multiple programs in memory, CPU switches on I/O wait.
- **Multitasking/Time-Sharing**: CPU time shared using time quantum. Example: UNIX, Linux.
- **Real-Time OS**: Hard RTOS (missile systems), Soft RTOS (video streaming).
- **Distributed OS**: Multiple machines work together. Example: Google infrastructure.

## Kernel Types

| Type | Description | Example | Pros | Cons |
|------|-------------|---------|------|------|
| **Monolithic** | All services in kernel space | Linux | Fast | Crash in one module crashes all |
| **Microkernel** | Minimal kernel; services in user space | QNX | Stable | Slower |
| **Hybrid** | Mix of both | Windows NT, macOS | Balanced | Complex |

## User Mode vs Kernel Mode

| Feature | User Mode | Kernel Mode |
|---------|-----------|-------------|
| **Privilege** | Restricted | Full access |
| **Mode bit** | 1 | 0 |
| **Crash impact** | Only that process | Entire system |

When a process makes a **system call**, it switches User Mode → Kernel Mode → User Mode.

---

# 2. System Calls

A **system call** is a programmatic way for a process to request a service from the kernel.

### Process Control
| Call | Description |
|------|-------------|
| `fork()` | Create child process |
| `exec()` | Replace process with new program |
| `wait()` | Parent waits for child |
| `exit()` | Terminate process |

```c
int pid = fork();
if (pid == 0) {
    printf("Child PID: %d\n", getpid());
} else if (pid > 0) {
    printf("Parent PID: %d, Child: %d\n", getpid(), pid);
    wait(NULL);
}
```

**`fork()` returns**: 0 to child, child's PID to parent, -1 on failure.

### File Management
`open()`, `read()`, `write()`, `close()`, `lseek()`

### Communication
`pipe()`, `shmget()`, `mmap()`, `socket()`

---

# 3. Process Management

## Process Memory Layout

```
High Address
+------------------+
|      Stack       |  ← Local variables (grows down)
|        ↓         |
|        ↑         |
|       Heap       |  ← Dynamic memory (grows up)
+------------------+
|      BSS         |  ← Uninitialized globals
+------------------+
|      Data        |  ← Initialized globals
+------------------+
|      Text        |  ← Code (read-only)
+------------------+
Low Address
```

## Stack vs Heap

| Stack | Heap |
|-------|------|
| Auto alloc/dealloc | Manual (malloc/free) |
| LIFO, fast | No order, slower |
| Limited (1-8 MB) | Limited by RAM |
| Local variables | Dynamic objects |

## Process States

```
New → Ready → Running → Terminated
              ↕
           Waiting
```

| State | Description |
|-------|-------------|
| **New** | Being created |
| **Ready** | Waiting for CPU |
| **Running** | Executing |
| **Waiting** | Waiting for I/O |
| **Terminated** | Finished |

## Process Control Block (PCB)

Contains: PID, State, Program Counter, CPU Registers, Scheduling Info, Memory Info, I/O Info.

## Context Switching

Saving state of current process (into PCB) and restoring state of next process. **Pure overhead** — typical time: 1–10 μs.

## Zombie vs Orphan Process

- **Zombie**: Child terminated, parent hasn't called `wait()`. Fix: parent calls `wait()`.
- **Orphan**: Parent terminated. Adopted by **init (PID 1)**.

## Inter-Process Communication (IPC)

| Method | Speed | Sync Needed? |
|--------|-------|-------------|
| **Shared Memory** | Fast | Yes (semaphores) |
| **Message Passing** | Slower | No |
| **Pipes** | Moderate | No |
| **Sockets** | Network | No |
| **Signals** | Async | N/A |

---

# 4. CPU Scheduling

## Scheduling Criteria

| Criteria | Goal |
|----------|------|
| **CPU Utilization** | Keep CPU busy (40-90%) |
| **Throughput** | Processes completed per unit time |
| **Turnaround Time** | Submission to completion |
| **Waiting Time** | Time in ready queue |
| **Response Time** | Request to first response |

## Preemptive vs Non-Preemptive

| Preemptive | Non-Preemptive |
|------------|----------------|
| OS can remove process from CPU | Process runs until it yields |
| Round Robin, SRTF | FCFS, SJF |

## Algorithms

### 1. FCFS (Non-preemptive)
```
Process  Arrival  Burst
P1       0        24
P2       1        3
P3       2        3

Gantt: |----P1----|--P2--|--P3--|
       0         24     27     30

Avg WT = (0 + 23 + 25)/3 = 16.0
```
**Problem**: Convoy Effect.

### 2. SJF (Non-preemptive)
Selects smallest burst. **Optimal** for avg waiting time. **Problem**: Starvation.

### 3. SRTF (Preemptive SJF)
At each arrival, compare remaining times, preempt if shorter job arrives.

### 4. Round Robin (Preemptive, time quantum q)
```
q=4:  Process  Burst
      P1       5
      P2       3
      P3       8

Gantt: |-P1-|P2-|--P3--|P1|--P3--|
       0    4   7     11 12     16
```
- q too large → FCFS
- q too small → too many context switches
- Typical q: 10-100 ms

### 5. Priority Scheduling
Highest priority first. **Problem**: Starvation. **Solution**: Aging.

### 6. Multilevel Feedback Queue
- Multiple queues with different quantum sizes.
- Processes move between queues (prevents starvation).
- Most general and flexible.

---

# 5. Threads & Multithreading

## Process vs Thread

| Feature | Process | Thread |
|---------|---------|--------|
| **Memory** | Separate address space | Shares process memory |
| **Creation** | Slow (~ms) | Fast (~μs) |
| **Communication** | IPC needed | Direct shared memory |
| **Context switch** | Expensive | Cheap |
| **Crash** | Only that process | Entire process |

## Thread Types
- **User-Level**: Managed by library. Fast but one block = all block.
- **Kernel-Level**: Managed by OS. True parallelism.

## Models
- **Many-to-One**: Many user threads → 1 kernel thread.
- **One-to-One**: Each user thread → 1 kernel thread (Linux).
- **Many-to-Many**: M user → N kernel threads.

---

# 6. Process Synchronization

## Race Condition

```c
int counter = 5;
// Thread A: counter++    Thread B: counter--
// Result could be 4, 5, or 6!
```

## Critical Section Requirements
1. **Mutual Exclusion**: Only one process in CS at a time.
2. **Progress**: No unnecessary blocking.
3. **Bounded Waiting**: Limit on wait time.

## Synchronization Tools

### Mutex
```c
pthread_mutex_lock(&lock);
// CRITICAL SECTION
pthread_mutex_unlock(&lock);
```

### Semaphore
- **Binary (0/1)**: Like mutex.
- **Counting (0-N)**: Controls N resource instances.

```c
sem_wait(&sem);   // P operation (decrement)
// CRITICAL SECTION
sem_post(&sem);   // V operation (increment)
```

## Classic Problems

### 1. Producer-Consumer
```
Semaphores: mutex=1, empty=N, full=0

Producer:                  Consumer:
  wait(empty)               wait(full)
  wait(mutex)               wait(mutex)
  add item                  remove item
  signal(mutex)             signal(mutex)
  signal(full)              signal(empty)
```

### 2. Readers-Writers
- Multiple readers OR one writer. First reader locks writers out. Last reader unlocks.

### 3. Dining Philosophers
- 5 philosophers, 5 forks. Each needs 2 to eat.
- Naive: all grab left fork → **DEADLOCK**.
- Fix: limit to 4 at table, or asymmetric pickup order.

---

# 7. Deadlocks

## Four Necessary Conditions (ALL must hold)

1. **Mutual Exclusion**: Non-sharable resource.
2. **Hold and Wait**: Hold one, wait for another.
3. **No Preemption**: Can't forcibly take resources.
4. **Circular Wait**: P1→P2→P3→P1.

**Mnemonic**: **M-H-N-C**

## Handling Strategies

### 1. Prevention (Break one condition)
- **Circular Wait**: Order resources, request in order.
- **Hold and Wait**: Request all at once.

### 2. Avoidance: Banker's Algorithm

```
5 processes, 3 resources: A=10, B=5, C=7

         Alloc    Max     Need    Available
P0       0 1 0   7 5 3   7 4 3    3 3 2
P1       2 0 0   3 2 2   1 2 2
P2       3 0 2   9 0 2   6 0 0
P3       2 1 1   2 2 2   0 1 1
P4       0 0 2   4 3 3   4 3 1

Safe Sequence: <P1, P3, P0, P2, P4> ✓
```

### 3. Detection & Recovery
- Detect cycles. Kill processes or preempt resources.

### 4. Ignore (Ostrich Algorithm)
- Used by Linux/Windows. Assume deadlocks are rare. Reboot if needed.

---

# 8. Memory Management

## Logical vs Physical Address

- **Logical**: Generated by CPU (virtual).
- **Physical**: Actual RAM location.
- **MMU** translates: Physical = Base + Logical.

## Fragmentation

| Internal | External |
|----------|----------|
| Wasted space inside partition | Free memory not contiguous |
| Fixed partitioning | Variable partitioning |

## Allocation Strategies
- **First Fit**: First hole that fits (fastest, generally best).
- **Best Fit**: Smallest sufficient hole.
- **Worst Fit**: Largest hole.

## Paging

Divides logical memory into **pages**, physical into **frames** (same size).

```
Logical Address = Page Number (p) + Offset (d)
Physical = Frame[p] + d

Page size = 4KB = 2^12 → offset = 12 bits
32-bit address → page number = 20 bits → 2^20 pages
```

### TLB (Translation Lookaside Buffer)
Hardware cache for page table. 

```
EAT = hit% × (TLB + mem) + miss% × (TLB + 2×mem)
Example: 90% hit, TLB=10ns, mem=100ns
EAT = 0.9×110 + 0.1×210 = 120ns
```

### Multi-level Page Tables
For large address spaces. 2-level splits page number into outer + inner.

## Segmentation

Variable-sized segments (code, data, stack). Address = (segment#, offset).

---

# 9. Virtual Memory

Allows processes larger than physical memory. Pages loaded **on demand**.

## Demand Paging

Page loaded only when accessed. If not in memory → **Page Fault**.

```
Page Fault: trap → find free frame → load from disk → update table → restart instruction

EAT = (1-p)×mem + p×page_fault_time
p=0.001, mem=200ns, fault=8ms:
EAT = 0.999×200 + 0.001×8000000 = 8199.8ns (40x slowdown!)
```

## Page Replacement Algorithms

### 1. FIFO
Replace oldest page. **Suffers Belady's Anomaly** (more frames → more faults possible).

### 2. Optimal (OPT)
Replace page not used for longest future time. **Not implementable** — benchmark only.

### 3. LRU (Least Recently Used)
Replace least recently accessed. **No Belady's Anomaly**. Near-optimal.

### 4. Clock (Second Chance)
Reference bit per page. Sweep: bit=0 → replace, bit=1 → set 0 and skip.

| Algorithm | Faults | Belady's? | Practical? |
|-----------|--------|-----------|------------|
| FIFO | High | YES | Simple |
| Optimal | Min | No | No |
| LRU | Good | No | Expensive |
| Clock | Good | No | Efficient |

## Thrashing

Process spends more time paging than executing. 

**Solutions**: Working Set Model (track recent pages), Page Fault Frequency monitoring.

---

# 10. File Systems

## File Allocation Methods

### 1. Contiguous
```
[File | Start | Length]
+ Fast access  - External fragmentation
```

### 2. Linked
```
Block → Block → Block → null
+ No fragmentation  - Slow random access
```

### 3. Indexed (inode-based)
```
Index block stores pointers to data blocks.
UNIX inode: 12 direct + single/double/triple indirect
Max size with 4KB blocks: ~4TB
```

## Hard Link vs Soft Link

| Hard Link | Soft Link |
|-----------|-----------|
| Same inode | Points to path |
| Can't cross filesystem | Can cross |
| Delete original → still works | Becomes dangling |

## Free Space Management
- **Bit Vector**: 1 bit per block (0=free, 1=used).
- **Linked List**: Free blocks chained.

---

# 11. Disk Scheduling

**Access Time = Seek Time + Rotational Latency + Transfer Time**

Example: Head at 53, queue: 98, 183, 37, 122, 14, 124, 65, 67

| Algorithm | Total Seek | Starvation? |
|-----------|-----------|-------------|
| **FCFS** | 640 | No |
| **SSTF** | 236 | Yes |
| **SCAN** | ~236 | No |
| **C-SCAN** | ~382 | No (uniform) |
| **LOOK/C-LOOK** | Best practical | No |

- **SSTF**: Closest track first (greedy).
- **SCAN**: Elevator — sweep one direction, reverse.
- **C-SCAN**: Sweep one direction, jump back.
- **LOOK**: Like SCAN but only goes to last request.

---

# 12. I/O Management

## I/O Methods

| Method | Description | CPU Usage |
|--------|-------------|-----------|
| **Polling** | CPU repeatedly checks device status | Wastes CPU |
| **Interrupt-driven** | Device signals CPU when ready | Better |
| **DMA** | Device transfers directly to memory | Best |

## Spooling
Buffering for slow devices (e.g., printer). Jobs queued to disk, processed later.

---

# 13. Important Interview Questions

### Q1: What is the difference between process and thread?
Process has separate memory space; threads share memory. Threads are lightweight, cheaper to create/switch.

### Q2: What is a deadlock? How to prevent it?
All 4 conditions (M-H-N-C) must hold. Break any one: e.g., order resources to prevent circular wait.

### Q3: What is virtual memory?
Allows processes larger than RAM by using disk as extension. Uses demand paging.

### Q4: Explain thrashing.
Too many processes → not enough frames → excessive page faults → CPU utilization drops. Fix: reduce degree of multiprogramming.

### Q5: Difference between paging and segmentation?

| Paging | Segmentation |
|--------|-------------|
| Fixed-size pages | Variable-size segments |
| No external frag | No internal frag |
| Invisible to programmer | Visible (code, data, stack) |

### Q6: What is a semaphore vs mutex?
Mutex: binary lock, owned by thread. Semaphore: counter, no ownership, can allow N threads.

### Q7: Explain Banker's Algorithm.
Deadlock avoidance. Before granting a resource, check if system remains in safe state (a sequence where all processes can finish).

### Q8: What is context switching?
Saving current process state to PCB, loading next process state. Overhead — no useful work done.

### Q9: What are zombie and orphan processes?
Zombie: child terminated, parent didn't `wait()`. Orphan: parent terminated, adopted by init.

### Q10: FCFS vs SJF vs Round Robin?
FCFS: simple, convoy effect. SJF: optimal avg wait, starvation. RR: fair, time quantum based.

### Q11: What is a page fault?
Accessing a page not in memory triggers a trap. OS loads page from disk, updates page table, restarts instruction.

### Q12: What is the difference between internal and external fragmentation?
Internal: wasted space inside allocated block. External: total free space sufficient but not contiguous.

### Q13: Explain fork() system call.
Creates child process (copy of parent). Returns 0 to child, child PID to parent. Uses Copy-on-Write for efficiency.

### Q14: What is starvation? How to solve?
Low-priority process never gets CPU. Solution: **Aging** — gradually increase priority.

### Q15: What is DMA?
Direct Memory Access. Device transfers data to/from memory without CPU involvement. CPU only initiates and gets interrupted on completion.

### Q16: Explain RAID levels.

| Level | Description | Min Disks |
|-------|-------------|-----------|
| RAID 0 | Striping, no redundancy | 2 |
| RAID 1 | Mirroring | 2 |
| RAID 5 | Striping + distributed parity | 3 |
| RAID 6 | Striping + double parity | 4 |
| RAID 10 | Mirror + Stripe | 4 |

### Q17: What is a spin lock?
A lock where the thread busy-waits (spins) checking if lock is available. Good for short critical sections on multiprocessors.

### Q18: Explain Copy-on-Write (COW).
After fork(), parent and child share same pages. Only when one writes, the page is copied. Saves memory and time.

### Q19: What is a system call vs library call?
System call: request to kernel (mode switch). Library call: user-space function (may internally use system calls).

### Q20: Belady's Anomaly?
In FIFO page replacement, increasing frames can increase page faults. Does NOT occur in LRU or Optimal.

---

# 14. Detailed Real-World Case Study: Google Chrome Browser

This case study traces what happens when you open Chrome and browse the web, showing how **every major OS concept** comes into play.

## Step 1: Launching Chrome — Process Creation

When you double-click Chrome:
1. **fork()** — The shell/explorer creates a child process.
2. **exec()** — Child's memory is replaced with Chrome's binary.
3. **PCB created** — PID=5001, State=Ready, Priority=Normal.
4. **Memory layout** created: Text (code ~150MB), Data (globals), Heap (will grow), Stack (calls).

## Step 2: Opening Tabs — Multiprocess Architecture

Chrome uses a **multi-process model**. Each tab = separate process:

```
Browser Process (PID 5001)
    ├── Tab 1: Gmail      (PID 5010)
    ├── Tab 2: YouTube    (PID 5011)
    ├── Tab 3: Google Docs (PID 5012)
    └── GPU Process       (PID 5020)
```

**Why separate processes per tab?**
- **Isolation**: YouTube crash → Gmail survives.
- **Security**: Malicious site can't access another tab's memory.
- Each tab has its own PCB, own address space.

If Chrome used a single process (like old browsers), one bad tab crashes everything.

## Step 3: Inside a Tab — Threads

Each tab process uses **multiple threads**:

| Thread | Role |
|--------|------|
| **Main Thread** | Runs JavaScript, handles DOM updates |
| **Render Thread** | Paints pixels (layout, compositing) |
| **Network Thread** | Fetches resources (HTTP requests) |
| **I/O Thread** | Reads/writes disk (cache, downloads) |
| **Worker Threads** | Background tasks (service workers) |

**Shared among threads**: Code segment, data segment, heap (JS objects).
**Separate per thread**: Stack, registers, program counter.

> **Interview Question**: *"Why processes for tabs but threads within a tab?"*
> **Answer**: Processes give **isolation and security** between tabs. Threads within a tab give **performance** (shared memory, fast communication) for rendering, networking, and JS that must work together tightly.

## Step 4: CPU Scheduling in Action

Your system runs: Chrome (3 tabs), VS Code, Spotify, OS services.

Scheduler uses **Multilevel Feedback Queue**:

```
Queue 0 (q=8ms)  [Highest] → Keyboard/mouse interrupt handlers
Queue 1 (q=16ms) [High]    → Chrome's active tab (foreground)
Queue 2 (q=32ms) [Medium]  → VS Code, Spotify
Queue 3 (FCFS)   [Low]     → Background updates, indexing
```

**When you type in Chrome:**
1. Keyboard → **hardware interrupt** → OS switches to kernel mode.
2. Interrupt handler (Queue 0) processes keystroke.
3. Chrome's main thread (Queue 1) scheduled → updates search bar.
4. Spotify (Queue 2) **preempted** briefly but resumes (you don't notice).
5. Windows Update (Queue 3) gets least CPU time.

## Step 5: Paging & Frames in Action

Your PC: **8 GB RAM**, **4 KB page/frame size**:
```
Total frames = 8 GB / 4 KB = 2,097,152 frames
```

Chrome's YouTube tab (PID 5011) uses ~500 MB:
```
Pages needed = 500 MB / 4 KB = 128,000 pages
```

**Page table for YouTube tab:**

| Page | Content | Frame | In RAM? |
|------|---------|-------|---------|
| Page 0 | V8 engine code start | Frame 45,201 | Yes |
| Page 1 | V8 engine code cont. | Frame 102,887 | Yes |
| Page 500 | YouTube HTML DOM | Frame 8,442 | Yes |
| Page 800 | Cached video buffer | Frame 200,105 | Yes |
| Page 1200 | Old tab content | --- | **No (on disk)** |
| Page 5000 | Unused heap | --- | **Not loaded** |

**Key insight**: Pages are NOT contiguous! Page 0 → Frame 45,201, Page 1 → Frame 102,887. Page table handles this transparently.

### Page Fault Scenario

You scroll down on YouTube, browser needs Page 1200 (on disk):

```
1. CPU accesses Page 1200 → valid bit = 0
2. PAGE FAULT! CPU traps to OS.
3. OS finds free frame (Frame 77,543).
4. OS reads Page 1200 from SSD swap → Frame 77,543.
5. Page table updated: Page 1200 → Frame 77,543, valid=1.
6. Instruction restarted → content renders.
7. Cost: ~8 ms (SSD) vs 200 ns (RAM) = 40,000× slower!
```

## Step 6: Segmentation in Action

YouTube tab's virtual memory is logically divided:

```
+-------------------------------------------+
| Segment 0: Code (V8 engine, 150 MB)       |  Read-only, SHARED across tabs
+-------------------------------------------+
| Segment 1: Data (Global vars, 20 MB)      |
+-------------------------------------------+
| Segment 2: Heap (DOM, JS objects, 300 MB)  |  Grows upward dynamically
+-------------------------------------------+
| Segment 3: Stack (Call frames, 8 MB)       |  Grows downward
+-------------------------------------------+
| Segment 4: Shared Libs (libc, GPU, 50 MB)  |
+-------------------------------------------+
```

**Segment Table:**

| Seg # | Name | Base (Physical) | Limit |
|-------|------|----------------|-------|
| 0 | Code | 0x00400000 | 150 MB |
| 1 | Data | 0x10000000 | 20 MB |
| 2 | Heap | 0x20000000 | 300 MB (grows) |
| 3 | Stack | 0x7FFE0000 | 8 MB (grows down) |
| 4 | Shared Libs | 0x7F000000 | 50 MB |

**What segmentation enables:**
- **Code sharing**: 3 YouTube tabs → V8 engine code loaded ONCE, shared (saves 300 MB!).
- **Stack overflow detection**: Recursive function exceeds 8 MB → offset ≥ limit → **Segmentation Fault**.
- **Heap growth**: JS creates objects → OS expands Segment 2 via `brk()`/`mmap()`.

### Segmentation Fault — Real Example
```c
int *ptr = NULL;
*ptr = 42;  // Access address 0x00000000
// Address 0x0 belongs to no valid segment
// → OS generates SIGSEGV (signal 11)
// → Process killed: "Segmentation fault (core dumped)"
```

### Segmentation Address Translation Example

Given the segment table above:

```
Translate (1, 150):  Segment 1 (Data), offset 150
  150 < Limit[1]=200  ✓
  Physical = Base[1] + 150 = 0x10000000 + 150 = 0x10000096

Translate (2, 350):  Segment 2 (Stack), offset 350
  BUT if Limit[2] = 300: 350 ≥ 300 → SEGMENTATION FAULT!

Translate (0, 100):  Segment 0 (Code), offset 100
  100 < 150MB ✓
  Physical = 0x00400000 + 100 = 0x00400064
```

## Step 7: Synchronization — Producer-Consumer in Chrome

Network Thread downloads video chunks, Render Thread displays them:

```c
// This is exactly the Producer-Consumer problem!
pthread_mutex_t buffer_lock;
sem_t data_available;

// Network Thread (Producer):
void* network_thread(void* arg) {
    while (downloading) {
        chunk = download_video_chunk();
        pthread_mutex_lock(&buffer_lock);
        add_to_buffer(chunk);
        pthread_mutex_unlock(&buffer_lock);
        sem_post(&data_available);       // Signal: data ready
    }
}

// Render Thread (Consumer):
void* render_thread(void* arg) {
    while (playing) {
        sem_wait(&data_available);       // Wait for data
        pthread_mutex_lock(&buffer_lock);
        chunk = get_from_buffer();
        pthread_mutex_unlock(&buffer_lock);
        render_frame(chunk);
    }
}
```

## Step 8: Deadlock Scenario in Chrome

Render thread and Network thread need two resources:

```
Render Thread                Network Thread
  Holds: GPU buffer            Holds: Network socket
  Wants: Network socket        Wants: GPU buffer
  
  → Circular Wait → DEADLOCK!
```

Chrome avoids this by:
- **Resource ordering**: Always acquire GPU before Socket.
- **Timeouts**: If lock not acquired in 5 seconds, release all and retry.

## Step 9: Virtual Memory & Thrashing

50 Chrome tabs open, only 8 GB RAM:

| Process | Virtual Mem | RAM Used |
|---------|-------------|----------|
| Tabs 1-10 (active) | 5 GB | 3 GB |
| Tabs 11-50 (background) | 20 GB | 1 GB (swapped) |
| VS Code | 2 GB | 800 MB |
| Spotify | 500 MB | 200 MB |
| OS + Services | 3 GB | 2 GB |
| **Total** | **30.5 GB** | **7 GB (+1 free)** |

Switch to Tab 35 (swapped to disk):
1. Massive page faults loading Tab 35's pages.
2. OS evicts pages from other tabs using **LRU**.
3. Rapidly switching tabs → **THRASHING**.
4. OS detects via Page Fault Frequency → **suspends background tabs**.

This is why Chrome shows "Aw, Snap!" and your PC slows with too many tabs!

---

# 15. Paging — Extended Examples

## Example 1: Full Address Translation

```
Given:
  Logical address space = 64 bytes (6 bits)
  Page size = 16 bytes = 2^4
  Physical memory = 128 bytes (7 bits)

Calculations:
  Offset bits = 4 (since page size = 2^4)
  Page number bits = 6 - 4 = 2 → 4 pages (0-3)
  Frame number bits = 7 - 4 = 3 → 8 frames (0-7)

Page Table:
  Page 0 → Frame 5
  Page 1 → Frame 2
  Page 2 → Frame 7
  Page 3 → Frame 0

Translate logical address 21:
  Binary: 010101
  Page = 01 (=1), Offset = 0101 (=5)
  Frame[1] = 2
  Physical = 2 × 16 + 5 = 37
  Binary: 0100101

Translate logical address 48:
  Page = 48/16 = 3, Offset = 48%16 = 0
  Frame[3] = 0
  Physical = 0 × 16 + 0 = 0

Translate logical address 60:
  Page = 60/16 = 3, Offset = 60%16 = 12
  Frame[3] = 0
  Physical = 0 × 16 + 12 = 12
```

## Example 2: Page Table Size Calculation

```
Given:
  Virtual address space = 32 bits (4 GB)
  Page size = 4 KB = 2^12
  Page table entry size = 4 bytes

Number of pages = 2^32 / 2^12 = 2^20 = 1,048,576 pages
Page table size = 2^20 × 4 bytes = 4 MB per process!

Problem: If 100 processes → 400 MB just for page tables!
Solution: Multi-level page tables or inverted page table.
```

## Example 3: Two-Level Page Table

```
32-bit address, 4KB pages, 4-byte entries:

Single-level: 2^20 entries → 4 MB page table (wasteful!)

Two-level:
  |  10 bits  |  10 bits  |  12 bits  |
  | Outer PT  | Inner PT  |  Offset   |

  Outer page table: 2^10 = 1024 entries = 4 KB
  Each inner page table: 2^10 entries = 4 KB
  Only allocate inner tables as needed!

If process uses only 1 MB of address space:
  Single-level: still 4 MB page table
  Two-level: 4 KB (outer) + 4 KB (one inner) = 8 KB!
  → 500× memory savings!
```

---

# 16. Segmentation — Extended Examples

## Example 1: Complete Segmentation Walkthrough

```
A process has 5 segments:

Segment Table:
+--------+----------+--------+-------+
| Seg #  | Name     | Base   | Limit |
+--------+----------+--------+-------+
|   0    | Code     | 2000   | 800   |
|   1    | Data     | 3500   | 500   |
|   2    | Stack    | 5000   | 1000  |
|   3    | Heap     | 7000   | 2000  |
|   4    | Shared   | 10000  | 300   |
+--------+----------+--------+-------+

Physical Memory:
0         2000    2800  3500 4000  5000      6000  7000         9000  10000 10300
|  Free   | Seg 0 |Free| Seg 1  | Seg 2     |Free| Seg 3            | Seg 4|

Address Translations:
  (0, 500) → Base[0]+500 = 2000+500 = 2500 ✓ (500 < 800)
  (1, 499) → Base[1]+499 = 3500+499 = 3999 ✓ (499 < 500)
  (1, 500) → 500 ≥ 500 → SEGFAULT! (right at the boundary!)
  (2, 999) → Base[2]+999 = 5000+999 = 5999 ✓ (barely fits)
  (3, 2001) → 2001 ≥ 2000 → SEGFAULT!
  (4, 0)   → Base[4]+0 = 10000 ✓
```

## Example 2: Segment Sharing

```
Two processes running same program (e.g., two instances of vim):

Process A:                    Process B:
Seg 0 (Code)  → Base=2000    Seg 0 (Code)  → Base=2000  ← SAME! (shared)
Seg 1 (Data)  → Base=3500    Seg 1 (Data)  → Base=8000  ← Different
Seg 2 (Stack) → Base=5000    Seg 2 (Stack) → Base=9000  ← Different

Code segment is shared (read-only) → saves memory!
Data and stack are separate → processes don't interfere.
```

## Example 3: Segmented Paging (Intel x86)

```
Logical Address:
  (Segment Selector, Offset)
       ↓
  Segment Table → Linear Address (base + offset)
       ↓
  Page Table → Physical Address

Example:
  Segment 2 base = 0x00400000, offset = 0x1234
  Linear address = 0x00401234
  Page number = 0x00401 (top 20 bits)
  Page offset = 0x234 (bottom 12 bits)
  Frame[0x00401] = 0x8F320
  Physical = 0x8F320234

Modern x86-64: Segmentation mostly disabled, paging only.
```

---

# 17. Quick Reference Formulas

```
EAT (with TLB)  = h × (t_TLB + t_mem) + (1-h) × (t_TLB + 2×t_mem)
EAT (paging)    = (1-p) × t_mem + p × t_fault

Page number     = ⌊logical_address / page_size⌋
Offset          = logical_address mod page_size
Physical addr   = frame_number × frame_size + offset

Num pages       = ⌈address_space / page_size⌉
Num frames      = physical_memory / frame_size

Turnaround Time = Completion Time - Arrival Time
Waiting Time    = Turnaround Time - Burst Time

Disk Access     = Seek Time + Rotational Latency + Transfer Time
```

---

*Comprehensive OS document with real-world examples. Covers all major topics for interview preparation. Good luck!*
