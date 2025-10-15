| **`timer_run()`** | Cập nhật (giảm đếm) tất cả các Software Timer đang hoạt động. Hàm này được gọi trong ngắt 10ms. | ```c
void timer_run (){
    for (int i = 0; i < NO_OF_TIMERS; i++) {
        if( timer_counter[i] > 0){
            timer_counter[i] --;
            if( timer_counter[i] == 0) timer_flag[i] = 1;
        }
    }
}
``` |

---

## 2. Hàm Cập nhật Dữ liệu Đồng hồ

Hàm này được gọi mỗi giây trong vòng lặp `while(1)` để tính toán và lưu trữ giá trị giờ/phút vào `led_buffer`.

| Hàm | Mục đích | Source Code (từ `main.c`) |
| :--- | :--- | :--- |
| **`updateClockBuffer()`** | Cập nhật mảng `led_buffer` dựa trên biến `hour` và `minute`. | ```c
void updateClockBuffer(void) {
    // 1. Xử lý Giờ (HH)
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;
    // 2. Xử lý Phút (MM)
    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}
``` |

---

## 3. Hàm Quét và Hiển thị LED 7-Segment

Đây là hàm thực hiện quá trình quét LED, nó phải được trình bày vì đã chuyển hoàn toàn khỏi ngắt và được gọi bởi `timer_flag[2]` trong `main()`.

| Hàm | Mục đích | Source Code (từ `main.c`) |
| :--- | :--- | :--- |
| **`update7SEG()`** | Quét (tắt/bật chân Enable) và hiển thị giá trị từ `led_buffer` lên LED 7-segment tương ứng với `index`. | ```c
void update7SEG(int index) {
    // 1. TẮT TẤT CẢ các chân Enable
    HAL_GPIO_WritePin(GPIOA, E1_Pin | E2_Pin | E3_Pin | E4_Pin, GPIO_PIN_SET);
    // 2. Cài đặt các chân Segment (PB0-PB6) với giá trị của LED hiện tại
    display7SEG(led_buffer[index % MAX_LED]);
    // 3. BẬT chỉ chân Enable tương ứng với index
    HAL_GPIO_WritePin(GPIOA, LED_ENABLE_PINS[index % MAX_LED], GPIO_PIN_RESET);
}
``` |

---

## 4. Báo cáo Chính (Nơi tập trung logic)

Bạn đã có phần này, nhưng đây là cấu trúc cuối cùng để xác nhận tất cả logic đã được chuyển.

| Phần | Logic Đã Chuyển Sang `while(1)` |
| :--- | :--- |
| **`HAL_TIM_PeriodElapsedCallback`** | ```c
/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    timer_run(); // CHỈ CÒN DUY NHẤT LỆNH NÀY
}
/* USER CODE END 4 */
``` |
| **`main()` (`while(1)`)** | Báo cáo cần trình bày phần `while(1)` hoàn chỉnh với 3 khối `if (timer_flag[index] == 1)` để thực thi tất cả logic (Clock, Blink, Scan). |
