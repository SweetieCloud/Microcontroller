# 🚦 Microcontroller - Lab 3: Buttons & Switches (Traffic Light)

This project is an application for a traffic light system at a crossroad, implemented on an STM32 microcontroller. The system includes multiple modes for normal operation and on-the-fly modification of light timings.

This project fulfills the requirements of Lab 3 from the Microcontroller course (HCMUT).

## 📋 Specifications

### 1. Hardware Requirements
* **12 LEDs:** 4 red, 4 amber, and 4 green.
* **4 Seven-Segment LEDs:** Used to display time, with 2 displays allocated for each road.
* **3 Buttons:**
    1.  **Mode Button:** To select the operational mode.
    2.  **Modify Button:** To modify the time value for the selected light.
    3.  **Set Button:** To confirm and set the chosen value.

### 2. Functional Requirements
The application has 4 modes, controlled by the **Mode Button**. [cite_start]Pressing this button cycles through the modes from 1 to 4 and back to 1 [cite: 1462-1463].

---

#### 💠 Mode 1: Normal Mode
* [cite_start]The traffic light application runs normally, cycling through red, amber, and green phases for both roads[cite: 1464].

---

#### 💠 Mode 2: Modify Red LED Duration
[cite_start]This mode allows the user to change the time duration for the red LEDs[cite: 1465].
* [cite_start]**Indicator:** All single red LEDs blink at 2 Hz[cite: 1467].
* **Display:**
    * [cite_start]Two 7-segment LEDs display the current mode (e.g., "02")[cite: 1469].
    * [cite_start]The other two 7-segment LEDs display the time value being modified[cite: 1468].
* **Controls:**
    * The **Modify Button** (Button 2) increases the time duration value. [cite_start]The value is within the range of 1-99 [cite: 1470-1471].
    * [cite_start]The **Set Button** (Button 3) saves the new value[cite: 1472].

---

#### 💠 Mode 3: Modify Amber LED Duration
[cite_start]This mode is similar to Mode 2, but allows the user to change the time duration for the amber LEDs[cite: 1473].
* **Indicator:** All single amber LEDs blink at 2 Hz.
* **Controls:** Same as Mode 2, but applies to the amber light duration.

---

#### 💠 Mode 4: Modify Green LED Duration
[cite_start]This mode is similar to Mode 2, but allows the user to change the time duration for the green LEDs[cite: 1474].
* **Indicator:** All single green LEDs blink at 2 Hz.
* **Controls:** Same as Mode 2, but applies to the green light duration.
