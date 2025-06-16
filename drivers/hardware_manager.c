#include "hardware_manager.h"

bool init_hardware(ssd1306_t *disp) {
    bool success = _init_display(disp);
    if(success) {
        _init_buttons();
        _init_leds();
        adc_init();
        _init_mic();
        _init_joystick();
        _init_audio();
        _init_matrix_led(MATRIX_LED_PIN);
    }

    return success;
}

// --- Private Helper Functions ---
static bool _init_display(ssd1306_t *disp) {
    i2c_init(I2C_PORT, 400 * 1000); // 400 kHz for I2C communication
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    disp->external_vcc = false; // Internal VCC
    bool success = ssd1306_init(disp, OLED_WIDTH, OLED_HEIGHT, SSD1306_I2C_ADDR, I2C_PORT);
    
    return success;
}

static void _init_buttons() {
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
}

static void _init_leds() {
    gpio_init(RED_LED_PIN);
    gpio_init(GREEN_LED_PIN);
    gpio_init(BLUE_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);
}

static void _init_mic() {
    adc_gpio_init(28);
}

static void _init_joystick() {
    gpio_init(JOYSTICK_SW_PIN);
    gpio_set_dir(JOYSTICK_SW_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_SW_PIN);

    // ADC initialization for joystick
    adc_init();
    adc_gpio_init(JOYSTICK_VRY_PIN); // VRy
    adc_gpio_init(JOYSTICK_VRX_PIN); // VRx
}

static void _init_audio() {
    // Buzzer A
    gpio_set_function(BUZZER_A_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_A_PIN);
    uint channel_num = pwm_gpio_to_channel(BUZZER_A_PIN);

    pwm_set_wrap(slice_num, 255);
    pwm_set_clkdiv(slice_num, 1.0f); 
    pwm_set_chan_level(slice_num, channel_num, 0); 
    pwm_set_enabled(slice_num, true);

    // Buzzer B
    gpio_set_function(BUZZER_B_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(BUZZER_B_PIN);
    channel_num = pwm_gpio_to_channel(BUZZER_B_PIN);

    pwm_set_wrap(slice_num, 255);
    pwm_set_clkdiv(slice_num, 1.0f); 
    pwm_set_chan_level(slice_num, channel_num, 0); 
    pwm_set_enabled(slice_num, true);
}

struct pixel_t {
  uint8_t G, R, B;
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;

npLED_t leds[MATRIX_LED_COUNT];

PIO np_pio;
uint sm;

static void _init_matrix_led(uint pin) {
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true);
  }

  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  for (uint i = 0; i < MATRIX_LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

void npClear() {
  for (uint i = 0; i < MATRIX_LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

void npWrite() {
  for (uint i = 0; i < MATRIX_LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100);
}