/*
 * scheduler.c
 *
 * PHIÊN BẢN FIX CUỐI CÙNG (V3 - Fix Timing Drift):
 * - Dùng "Task Pool" (Mảng) để lưu Task.
 * - Dùng "Linked List" (Danh sách liên kết) để lập lịch.
 * - Dùng "g_TickCount" và "TargetTime" để chống trôi thời gian.
 *
 * KẾT QUẢ:
 * - SCH_Update()     : O(1)
 * - SCH_Delete_Task(): O(n) (an toàn)
 * - SCH_Add_Task()   : O(n)
 */

#include "scheduler.h"
#include <stdlib.h> // Để dùng NULL

// --- Định nghĩa cấu trúc nội bộ ---

// 1. Cấu trúc cho một Node trong danh sách liên kết
typedef struct sNode {
	uint32_t Delay;      // Delay TƯƠNG ĐỐI
	sTask* pTaskData;    // Con trỏ trỏ đến Task thật trong Pool
	struct sNode* pNext;
} sTaskNode;


// --- Biến nội bộ (Private Variables) ---

// FIX LỖI TRÔI (1): Thêm bộ đếm tick toàn cục
static uint32_t g_TickCount = 0;

/**
 * @brief Mảng (Pool) để LƯU TRỮ Task. TaskID là index của mảng này.
 */
static sTask g_TaskPool[SCH_MAX_TASKS];

/**
 * @brief Một Pool để cấp phát các Node cho Linked List.
 */
static sTaskNode g_TaskNodePool[SCH_MAX_TASKS];

/**
 * @brief Con trỏ đầu (Head) của danh sách liên kết.
 */
static sTaskNode* g_TaskListHead = NULL;


// --- Các hàm hỗ trợ nội bộ (Private Helper Functions) ---

/**
 * @brief Lấy một Node mới từ Pool (g_TaskNodePool).
 * Trả về NULL nếu hết Node.
 */
static sTaskNode* Get_New_Node(void) {
	for (int i = 0; i < SCH_MAX_TASKS; i++) {
		if (g_TaskNodePool[i].pTaskData == NULL) {
			return &g_TaskNodePool[i];
		}
	}
	return NULL; // Hết Node
}

/**
 * @brief Trả một Node về Pool (để tái sử dụng).
 */
static void Free_Node(sTaskNode* pNode) {
	if (pNode == NULL) return;
	pNode->Delay = 0;
	pNode->pTaskData = NULL;
	pNode->pNext = NULL;
}

/**
 * @brief Chèn một node mới vào danh sách liên kết (đã sắp xếp).
 * Đây là hàm O(n).
 */
static void Insert_Node_Sorted(sTaskNode* pNewNode) {
	if (g_TaskListHead == NULL || pNewNode->Delay < g_TaskListHead->Delay) {
		if (g_TaskListHead != NULL) {
			g_TaskListHead->Delay -= pNewNode->Delay;
		}
		pNewNode->pNext = g_TaskListHead;
		g_TaskListHead = pNewNode;
	}
	else {
		sTaskNode* pCurrent = g_TaskListHead;
		uint32_t newDelay = pNewNode->Delay;
		newDelay -= pCurrent->Delay;

		while (pCurrent->pNext != NULL && newDelay >= pCurrent->pNext->Delay) {
			pCurrent = pCurrent->pNext;
			newDelay -= pCurrent->Delay;
		}
		pNewNode->Delay = newDelay;
		if (pCurrent->pNext != NULL) {
			pCurrent->pNext->Delay -= newDelay;
		}
		pNewNode->pNext = pCurrent->pNext;
		pCurrent->pNext = pNewNode;
	}
}

// --- Cài đặt hàm Public (theo scheduler.h) ---

/**
 * @brief Khởi tạo 2 Pool (Task và Node) và Linked List Head.
 */
void SCH_Init(void) {
	// FIX LỖI TRÔI (2): Reset g_TickCount
	g_TickCount = 0;

	for (uint32_t i = 0; i < SCH_MAX_TASKS; i++) {
		g_TaskPool[i].pTask = NULL;
		g_TaskPool[i].Period = 0;
		g_TaskPool[i].RunMe = 0;
		g_TaskPool[i].TaskID = i;
		g_TaskPool[i].TargetTime = 0; // Khởi tạo trường mới

		g_TaskNodePool[i].pTaskData = NULL;
		g_TaskNodePool[i].pNext = NULL;
		g_TaskNodePool[i].Delay = 0;
	}
	g_TaskListHead = NULL;
}

/**
 * @brief Thêm một task mới vào bộ lập lịch.
 */
uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
	uint32_t taskID = 0;

	// 1. Tìm slot trống trong Task Pool (g_TaskPool)
	while (taskID < SCH_MAX_TASKS && g_TaskPool[taskID].pTask != NULL) {
		taskID++;
	}
	if (taskID == SCH_MAX_TASKS) {
		return SCH_MAX_TASKS; // Lỗi: Hết slot Task
	}

	// 3. Lấy một Node rảnh từ Node Pool (g_TaskNodePool)
	sTaskNode* pNewNode = Get_New_Node();
	if (pNewNode == NULL) {
		return SCH_MAX_TASKS; // Lỗi: Hết slot Node
	}

	// 4. Điền thông tin vào Task Pool (Slot [taskID])
	g_TaskPool[taskID].pTask = pFunction;
	g_TaskPool[taskID].Period = PERIOD;
	g_TaskPool[taskID].RunMe = 0;

	// FIX LỖI TRÔI (3): Tính toán và lưu TargetTime
	g_TaskPool[taskID].TargetTime = g_TickCount + DELAY;

	// 5. Điền thông tin vào Node (để chèn vào Linked List)
	pNewNode->Delay = DELAY;
	pNewNode->pTaskData = &g_TaskPool[taskID]; // Trỏ đến Task
	pNewNode->pNext = NULL;

	// 6. Chèn Node vào Linked List (đã sắp xếp)
	Insert_Node_Sorted(pNewNode);

	return taskID;
}

/**
 * @brief Cập nhật thời gian chờ của các task.
 * Đây là hàm O(1).
 */
void SCH_Update(void) {
	// FIX LỖI TRÔI (4): Tăng g_TickCount mỗi lần Update
	g_TickCount++;

	if (g_TaskListHead == NULL) {
		return;
	}

	// (Đã fix lỗi mất tick: Bỏ check RunMe)

	if (g_TaskListHead->Delay > 0) {
		g_TaskListHead->Delay--;
	}

	if (g_TaskListHead->Delay == 0) {
		g_TaskListHead->pTaskData->RunMe = 1;
	}
}

/**
 * @brief Thực thi các task đã đến hạn.
 */
void SCH_Dispatch_Tasks(void) {

	// (Đã fix lỗi mất tick: Dùng "while")
	while (g_TaskListHead != NULL && g_TaskListHead->pTaskData->RunMe > 0) {

		// 2. Lấy thông tin task
		sTask* pTaskToRun = g_TaskListHead->pTaskData;
		void (*pFunction)() = pTaskToRun->pTask;
		uint32_t Period = pTaskToRun->Period;
		uint32_t TaskID = pTaskToRun->TaskID;

		// 3. Lấy Node đầu ra khỏi danh sách
		sTaskNode* pNodeToFree = g_TaskListHead;
		g_TaskListHead = g_TaskListHead->pNext;

		pTaskToRun->RunMe = 0;
		(*pFunction)();
		Free_Node(pNodeToFree);

		if (Period > 0) {
			// (Đã fix lỗi rò rỉ: giải phóng slot cũ)
			g_TaskPool[TaskID].pTask = NULL;

			// FIX LỖI TRÔI (5): Tính newDelay dựa trên TargetTime
			// thay vì chỉ dùng Period.
			uint32_t newDelay = pTaskToRun->TargetTime + Period - g_TickCount;

			// Thêm lại task với delay đã được bù đắp
			SCH_Add_Task(pFunction, newDelay, Period);
		}
		else {
			// Task one-shot, dọn dẹp
			g_TaskPool[TaskID].pTask = NULL;
		}
	}
}

/**
 * @brief Xóa một task khỏi bộ lập lịch (An toàn).
 * Đây là hàm O(n).
 */
uint8_t SCH_Delete_Task(uint32_t taskID) {
	if (taskID >= SCH_MAX_TASKS || g_TaskPool[taskID].pTask == NULL) {
		return 0; // Lỗi
	}

	sTask* pTaskToDelete = &g_TaskPool[taskID];
	sTaskNode* pCurrent = g_TaskListHead;
	sTaskNode* pPrevious = NULL;

	while (pCurrent != NULL && pCurrent->pTaskData != pTaskToDelete) {
		pPrevious = pCurrent;
		pCurrent = pCurrent->pNext;
	}

	if (pCurrent != NULL) {
		if (pPrevious == NULL) {
			g_TaskListHead = pCurrent->pNext;
		}
		else {
			pPrevious->pNext = pCurrent->pNext;
		}
		if (pCurrent->pNext != NULL) {
			pCurrent->pNext->Delay += pCurrent->Delay;
		}
		Free_Node(pCurrent);
	}

	pTaskToDelete->pTask = NULL;
	pTaskToDelete->Period = 0;
	pTaskToDelete->RunMe = 0;
	pTaskToDelete->TargetTime = 0; // Reset trường mới

	return 1; // Thành công
}
