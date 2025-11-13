# ⏱️ Microcontroller - Lab 4: Cooperative Scheduler

This project implements a high-efficiency, non-preemptive cooperative scheduler for an STM32 microcontroller. The scheduler itself is the core component, designed for O(1) performance in its update routine. Its functionality is demonstrated by concurrently running five periodic tasks (blinking LEDs) at different intervals.

## 📋 Specifications

### 1. Hardware Requirements
* **STM32 Microcontroller:**
* **5 Single LEDs**
* **1 Hardware Timer:** TIM2 is used to generate the 10ms system tick.

### 2. Functional Requirements

The core of this lab is the scheduler library (`scheduler.c` and `scheduler.h`). The system is built to meet the following functional and performance criteria.

---
#### 💠 Scheduler API
The scheduler provides a public API for adding, deleting, and running tasks :
* **`void SCH_Init(void)`:** Initializes all internal data structures, including the Task Pool and the Node Pool for the linked list.

* **uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD):** Adds a new task to the scheduler. It takes a function pointer, an initial delay, and a periodic interval. It returns a `TaskID` for future reference.

* **`uint8_t SCH_Delete_Task(uint32_t taskID)`:** Safely removes a task from the scheduler using its TaskID.

* **`void SCH_Update(void)`:** The core "tick" function, designed to be called by the 10ms timer ISR. It updates the delay of the next task to run.

* **`void SCH_Dispatch_Tasks(void)`:** The main "runner" function, placed in the `while(1)` super-loop. It checks for tasks that are ready to run (where `RunMe > 0`) and executes them

---
#### 💠 Performance Requirement (O(1) Update)
A critical design constraint for this lab is that the SCH_Update() function must have **O(1) complexity**.
* **Design:** This is achieved by separating task storage from scheduling. A `g_TaskPool` array holds all task data, while a sorted Linked List (`g_TaskListHead`) acts as the run-queue.

* **Mechanism:** This list stores relative delays (the time to wait after the previous task runs) .

* **Result:** The `SCH_Update()` ISR only needs to check and decrement the `Delay` of the single head node. This avoids an O(n) search through all tasks, ensuring minimal time is spent in the interrupt routine.

---
#### 💠 Demonstration Application (main.c)
To prove the scheduler's functionality, the `main.c` file implements 5 concurrent tasks as required by the lab.
* **System Tick:** `TIM2` is configured to generate a 10ms interrupt.

* **ISR Hook:** The `HAL_TIM_PeriodElapsedCallback` function calls `SCH_Update()` on every 10ms tick.

* **Main Loop:** The `while(1)` super-loop continuously calls `SCH_Dispatch_Tasks()` to execute any tasks that are due.

* **Concurrent Tasks:** 5 LED-blinking tasks are added to the scheduler with different periods:

Task 1: 0.5s (50 ticks)

Task 2: 1.0s (100 ticks)

Task 3: 1.5s (150 ticks)

Task 4: 2.0s (200 ticks)

Task 5: 2.5s (250 ticks)
