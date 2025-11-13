/*
 * scheduler.h
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h" // Hoặc "stdint.h"

// --- Định nghĩa Hằng số ---
#define SCH_MAX_TASKS	40

// --- Định nghĩa Kiểu dữ liệu (Struct) ---

typedef struct {
	// Con trỏ trỏ đến hàm (task) cần thực thi
	void (*pTask)(void);

	// Thời gian chờ (delay) TƯƠNG ĐỐI
	// (Chỉ dùng cho logic nội bộ của linked list)
	uint32_t Delay;

	// Chu kỳ lặp lại (tính bằng "tick")
	uint32_t Period;

	// Cờ (flag) báo hiệu task đã sẵn sàng để chạy
	uint8_t RunMe;

	// ID của Task (là index trong Task Pool)
	uint32_t TaskID;

	// FIX LỖI "TRÔI THỜI GIAN" (Timing Drift):
	// Thêm trường này để lưu thời điểm TUYỆT ĐỐI mà task
	// được lên lịch để chạy.
	uint32_t TargetTime;

} sTask;

// --- Khai báo hàm (Function Prototypes) ---

void SCH_Init(void);

void SCH_Update(void);

void SCH_Dispatch_Tasks(void);

uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD);

uint8_t SCH_Delete_Task(uint32_t taskID);

#endif /* INC_SCHEDULER_H_ */
