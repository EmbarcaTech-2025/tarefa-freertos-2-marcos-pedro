#include "arming.h"

int direction_arr[8];
int answer_arr[8];

int draw(int x, int y, char item)
{
    ssd1306_draw_char(disp, x, y, 1, item);
    return 5;
}

int security_path(int x, int y, int arr[])
{
    int val = 0;

    adc_select_input(0);
    uint vlr_y = adc_read();

    adc_select_input(1);
    uint vlr_x = adc_read();

    if (vlr_x > 3500) // Movido para direita
    {
        val = draw(x, y, '>');
        arr[x/5] = 1;
    }

    else if (vlr_x < 600) // Movido para a esquerda
    {
        val = draw(x, y, '<');
        arr[x/5] = 2;
    }

    else if (vlr_y > 3500) // Movido para cima
    {
        val = draw(x, y, '^');
        arr[x/5] = 3;
    }

    else if (vlr_y < 600) // Movido para baixo
    {
        val = draw(x, y, 'v');
        arr[x/5] = 4;
    }

    ssd1306_show(disp);
    return val;
}

void vTaskArming(void *pvParameters)
{

    // ADC initialization for joystick
    adc_gpio_init(JOYSTICK_VRY_PIN); // VRy
    adc_gpio_init(JOYSTICK_VRX_PIN); // VRx

    bool disarmed = true;
    bool arming = false;
    bool armed = false;

    memset(answer_arr, 0, 16);

    ssd1306_clear(disp);
    ssd1306_show(disp);

    uint y = 16;
    uint x = 0;

    for (;;)
    {
        while (disarmed)
        {
            x = 0;
            y = 16;
            ssd1306_draw_string(disp, 10, 8, 1, "Click any button");
            ssd1306_draw_string(disp, 32, 16, 1, "to start");

            ssd1306_show(disp);

            if (!gpio_get(BUTTON_A_PIN) || !gpio_get(BUTTON_B_PIN))
            {
                memset(direction_arr, 0, 8);
                disarmed = false;
                arming = true;

                ssd1306_clear(disp);
                ssd1306_draw_string(disp, 0, 0, 1, "Create a password:");
                ssd1306_show(disp);

                x = 0;
            }
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        while (arming)
        {

            if (!gpio_get(BUTTON_A_PIN) || !gpio_get(BUTTON_B_PIN) || x >= 35)
            {
                arming = false;
                armed = true;

                memset(answer_arr, 0, 8);
                ssd1306_clear(disp);

                ssd1306_draw_string(disp, 0, 16, 1, "Password Set!");
                ssd1306_show(disp);

                vTaskDelay(pdMS_TO_TICKS(1000));

                ssd1306_clear(disp);
                ssd1306_show(disp);

                x = 0;

                //Começar a ouvir
                xSemaphoreGive(xArmedSemaphore);
                vTaskResume(noise_detector_handle);

                break;
            }

            x += security_path(x, y, direction_arr);
            vTaskDelay(pdMS_TO_TICKS(250));
        }

        while (armed)
        {
            x += security_path(x, y, answer_arr);

            if (!gpio_get(BUTTON_A_PIN) || !gpio_get(BUTTON_B_PIN) || x >= 35)
            {
                bool check = true;
                for(int i = 0; i < 8; i++)
                {
                    if(answer_arr[i] != direction_arr[i]) check = false;
                }

                if(check)
                {
                    armed = false;
                    disarmed = true;
                    //Terminar o alarde
                    vTaskSuspend(noise_detector_handle);
                    vTaskSuspend(alarm_handle);
                    ssd1306_clear(disp);
                    npClear();
                    npWrite();

                    uint slice_num_a = pwm_gpio_to_slice_num(BUZZER_A_PIN);
                    uint channel_num_a = pwm_gpio_to_channel(BUZZER_A_PIN);

                    uint slice_num_b = pwm_gpio_to_slice_num(BUZZER_B_PIN);
                    uint channel_num_b = pwm_gpio_to_channel(BUZZER_B_PIN);

                    pwm_set_chan_level(slice_num_a, channel_num_a, 0);
                    pwm_set_chan_level(slice_num_b, channel_num_b, 0);
                    ssd1306_show(disp);
                }

                else
                {
                    memset(answer_arr, 0, 8);
                    ssd1306_clear(disp);
                    ssd1306_show(disp);
                    x = 0;
                }
            }
            vTaskDelay(pdMS_TO_TICKS(250));
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}