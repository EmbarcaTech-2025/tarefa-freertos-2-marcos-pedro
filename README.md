# Tarefa: Roteiro de FreeRTOS #2 - EmbarcaTech 2025

Autores: Marcos Vinícius e Pedro Teixeira

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Brasília, junho de 2025

# Don't Touch Security System

## Project Objective

The main objective of this project is to develop a "Don't Touch" security system using the Raspberry Pi Pico W microcontroller with the FreeRTOS operating system. This involves mastering concepts such as audio signal acquisition to detect noise and using semaphores to coordinate tasks, ensuring one starts only after another has finished. A key aspect of this project is understanding the fundamentals of a Real-Time Operating System (RTOS).

The system interacts with the user through physical buttons and an analog joystick, provides visual feedback via an LED matrix and an OLED display, and delivers auditory feedback through buzzers.

## Used Components

The project utilizes the BitDogLab, a custom board based on the Raspberry Pi Pico W, which incorporates various external components:

* **Microcontroller:** Raspberry Pi Pico W (RP2040)
* **Audio Input:** Electret Microphone Module (Analog Input)
* **Audio Output:**
    * Passive Buzzer A
    * Passive Buzzer B
* **User Input Buttons:**
    * Button A
    * Button B
* **Display:** 0.96-inch OLED display (128x64 pixels, I2C communication)
* **LED Matrix:** 5x5 WS2812B (Neopixel) RGB LED matrix
* **Joystick**: Analog Joystick (KY023) with switch

## Pinout of the Devices Used

The following table details the connections between the Raspberry Pi Pico W and the components on the custom BitDogLab board:

| Raspberry Pi Pico W GPIO | Component Connected | Notes |
| :--- | :--- | :--- |
| **GPIO 5** | Button A | User input, active low (internal pull-up) |
| **GPIO 6** | Button B | User input, active low (internal pull-up) |
| **GPIO 7** | LED Matrix | Data-in for the 5x5 WS2812B RGB matrix |
| **GPIO 10** | Buzzer B | Passive buzzer |
| **GPIO 14** | OLED SDA | I2C Data Line (for SSD1306 display) |
| **GPIO 15** | OLED SCL | I2C Clock Line (for SSD1306 display) |
| **GPIO 21** | Buzzer A | Passive buzzer (via transistor) |
| **GPIO 22** | Joystick Button | User input, active low (internal pull-up) |
| **GPIO 26 (ADC0)**| Joystick Y-axis | Analog input from joystick |
| **GPIO 27 (ADC1)**| Joystick X-axis | Analog input from joystick |
| **GPIO 28 (ADC2)**| Microphone Input | Analog input from electret microphone module |

## Images and/or videos of the project in operation

<!-- TODO: Adicionar link do projeto funcionando -->

*[Link to video demonstration of the project in operation]()* 

## Expected Results

### System States

* **Arming State:**
    * Upon startup or after disarming, pressing Button A prompts the user to set a password via the OLED display.
    * The password is created using Joystick movements (up, down, right, left), with each movement shown on the OLED screen.
    * The password has a maximum length of 8 characters but can be shorter if the user presses Button B to confirm the entry.
    * Once the password is set, the system transitions to the Armed State.

* **Armed State:**
    * In this state, a Noise Detector task continuously monitors the microphone's input level.
    * The OLED display shows a message reminding the user to be quiet, such as "System Armed. Do not make loud noises."
    * If the user enters the correct password without the microphone detecting a loud noise, the system disarms and returns to the initial state.
    * If the detected sound level exceeds a predefined threshold, the system triggers the Alarm State.

* **Alarm State:**
    * When triggered, the Alarm Task starts running.
    * The LED matrix flashes red.
    * A warning message blinks on the OLED display.
    * The buzzers beep continuously.
    * The alarm will only stop when the correct password is entered.
    * Upon correct password entry, the system returns to the Armed State.

### Performance

* The system is expected to run smoothly with no noticeable lag, leveraging the real-time capabilities of FreeRTOS for responsive and concurrent task management.

### Obtained Results
* The project is working as expected.

### Video Demonstration
* Link: https://youtube.com/shorts/6OT5NVtpVHg?si=DcrUbvRkyqFOZecQ

## 📜 Licença
GNU GPL-3.0.