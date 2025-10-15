# Exercise 10: LED Matrix Animation Optimization 

## 1. Description and Objective

The primary goal of this exercise is to implement a **Animation** on the 8x8 LED Matrix. This task addresses the requirement of creating animation by using an advanced technique: **Double Buffering** synchronized with the display's vertical blanking interval (**VSync - Vertical Synchronization**). This mechanism is crucial to prevent **tearing** (visual flickering or data mix-up) when updating the matrix data during the high-frequency column scanning process.

### Key Implementation Techniques

* **Double Buffering:** Uses two separate memory buffers (`current_display_buffer` and `current_draw_buffer`) to decouple drawing/calculation from display refreshing.
* **VSync Synchronization:** The calculation for the next frame is performed asynchronously, but the final buffer swap is delayed until a **safe moment**: immediately after the last column (Column 7) has been fully displayed.
* **Animation Logic:** The animation implemented is a **Circular Right Shift** of the LED data, simulating a persistent scrolling text effect.

---

## 2. Solution and Code Description

The application logic is primarily contained within the Timer Interrupt Service Routine (ISR) to handle all display and animation timing via dedicated counters, demonstrating a high-load, non-blocking ISR structure.

### Private Variables (PV)

Key variables used for Double Buffering and synchronization.

```c
// Two pointers manage the memory: one for display, one for drawing.
uint8_t *current_display_buffer = matrix_buffer_A;
uint8_t *current_draw_buffer = matrix_buffer_B;

// Flag used for VSync: set to 1 when a new frame is calculated and ready to swap
volatile uint8_t frame_is_ready = 0;
int animation_counter = 20;
```
### `Function updateScrollingAnimation()`

void updateScrollingAnimation() {
    for (int i = 0; i < MAX_LED_MATRIX; i++) {
        // Circular right shift: (>> 1) shifts right, while (| << 7) wraps the MSB to the LSB
        current_draw_buffer[i] = (current_display_buffer[i] >> 1) | (current_display_buffer[i] << 7);
    }
    // Signal that the new frame is ready for display (VSync signal)
    frame_is_ready = 1;
}

### `HAL_TIM_PeriodElapsedCallback (Interrupt Handler)`
```c
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    timer_run();
    scan_counter--;
    if(scan_counter <= 0){
        scan_counter = 25;
        index_led = (index_led + 1) % MAX_LED;
        update7SEG(index_led);
    }

    blink_counter--;
    if(blink_counter <= 0){
        blink_counter = 100;
        HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }

    animation_counter--;
    if(animation_counter <= 0){
        animation_counter = 20;
        if (frame_is_ready == 0) {
            updateScrollingAnimation();
        }
    }

    updateLEDMatrix(index_led_matrix);

    if (frame_is_ready == 1 && index_led_matrix == 7) {
        uint8_t *temp = current_display_buffer;
        current_display_buffer = current_draw_buffer;
        current_draw_buffer = temp;
        frame_is_ready = 0;
    }

    index_led_matrix = (index_led_matrix + 1) % MAX_LED_MATRIX;
}
```
