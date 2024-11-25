#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TOUCH/touch.h"
#include "./BSP/DMA/dma.h"
#include "./BSP/ADC/adc.h"
#include "math.h"
#include "string.h"



/*
 * @brief       ������
 * @param       x1,y1: �������
 * @param       x2,y2: �յ�����
 * @param       size : ������ϸ�̶�
 * @param       color: �ߵ���ɫ
 */
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;

    if (x1 < size || x2 < size || y1 < size || y2 < size)
        return;

    delta_x = x2 - x1; /* ������������ */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)
    {
        incx = 1; /* ���õ������� */
    }
    else if (delta_x == 0)
    {
        incx = 0; /* ��ֱ�� */
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0; /* ˮƽ�� */
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
        distance = delta_x; /* ѡȡ�������������� */
    else
        distance = delta_y;

    for (t = 0; t <= distance + 1; t++) /* ������� */
    {
        lcd_fill_circle(row, col, size, color); /* ���� */
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

#define PI 3.14159f
#define ORIGIN_X 40     // X��ԭ������
#define ORIGIN_Y 350	    // Y��ԭ������
#define AXIS_LENGTH 400 // �᳤��
#define GRID_SIZE 20    // �����С
#define WAVE_COLOR RED  // ������ɫ
#define BG_COLOR WHITE  // ����ɫ
#define MOVE_STEP 2     // ÿ���ƶ���������(�����ܱ�AXIS_LENGTH����)

float g_amplitude = 50.0f;   // ������ɵ���
float g_frequency = 12.0f;    // ���ڣ��ɵ���
float g_phase = 0.0f;        // ��λ�������ƶ�
uint16_t g_wave_buffer[AXIS_LENGTH];  // ���λ�����

// ����������
void draw_coordinate_system(void) {
    // ����
    lcd_fill(0, 0, lcddev.width, lcddev.height, BG_COLOR);
    
    // ����X��
    lcd_draw_line(ORIGIN_X, ORIGIN_Y, ORIGIN_X + AXIS_LENGTH, ORIGIN_Y, BLACK);
    // X���ͷ
    lcd_draw_line(ORIGIN_X + AXIS_LENGTH, ORIGIN_Y, ORIGIN_X + AXIS_LENGTH - 10, ORIGIN_Y - 5, BLACK);
    lcd_draw_line(ORIGIN_X + AXIS_LENGTH, ORIGIN_Y, ORIGIN_X + AXIS_LENGTH - 10, ORIGIN_Y + 5, BLACK);
    
    // ����Y��
    lcd_draw_line(ORIGIN_X, ORIGIN_Y - AXIS_LENGTH/2, ORIGIN_X, ORIGIN_Y + AXIS_LENGTH/2, BLACK);
    // Y���ͷ
    lcd_draw_line(ORIGIN_X, ORIGIN_Y - AXIS_LENGTH/2, ORIGIN_X - 5, ORIGIN_Y - AXIS_LENGTH/2 + 10, BLACK);
    lcd_draw_line(ORIGIN_X, ORIGIN_Y - AXIS_LENGTH/2, ORIGIN_X + 5, ORIGIN_Y - AXIS_LENGTH/2 + 10, BLACK);
    
    // ���ƿ̶���
    for(int i = 1; i < AXIS_LENGTH/GRID_SIZE; i++) {
        // X��̶�
        lcd_draw_line(ORIGIN_X + i*GRID_SIZE, ORIGIN_Y-3, 
                     ORIGIN_X + i*GRID_SIZE, ORIGIN_Y+3, BLACK);
        
        // Y��̶�
        lcd_draw_line(ORIGIN_X-3, ORIGIN_Y - i*GRID_SIZE/2, 
                     ORIGIN_X+3, ORIGIN_Y - i*GRID_SIZE/2, BLACK);
        lcd_draw_line(ORIGIN_X-3, ORIGIN_Y + i*GRID_SIZE/2, 
                     ORIGIN_X+3, ORIGIN_Y + i*GRID_SIZE/2, BLACK);
    }
}

// �ػ�X��Ϳ̶���
void redraw_x_axis(void) {
    // �ػ�X��
    lcd_draw_line(ORIGIN_X, ORIGIN_Y, ORIGIN_X + AXIS_LENGTH, ORIGIN_Y, BLACK);
    
    // �ػ�X��̶���
    for(int i = 1; i < AXIS_LENGTH/GRID_SIZE; i++) {
        lcd_draw_line(ORIGIN_X + i*GRID_SIZE, ORIGIN_Y-3, 
                     ORIGIN_X + i*GRID_SIZE, ORIGIN_Y+3, BLACK);
    }
}

// ���㲨��ֵ
int calculate_wave_point(float x, float phase) {
    return ORIGIN_Y - (int)(g_amplitude * sinf((x/GRID_SIZE) * g_frequency * PI + phase));
}

// ��ʼ�����λ�����
void init_wave_buffer(void) {
    int i;
    for(i = 0; i < AXIS_LENGTH; i++) {
        g_wave_buffer[i] = calculate_wave_point((float)i, g_phase);
    }
}

// ��������������
void shift_wave_buffer(void) {
    int i;
    
    // �ƶ���������
    for(i = 0; i < AXIS_LENGTH - MOVE_STEP; i++) {
        g_wave_buffer[i] = g_wave_buffer[i + MOVE_STEP];
    }
    
    // �����µĵ�
    float phase = g_phase + ((float)AXIS_LENGTH/GRID_SIZE) * g_frequency * PI;
    for(i = AXIS_LENGTH - MOVE_STEP; i < AXIS_LENGTH; i++) {
        g_wave_buffer[i] = calculate_wave_point((float)i, g_phase);
    }
}

// ��ʾ����
void display_wave(void) {
    int i;
    int prev_y = g_wave_buffer[0];
    
    // �����������(����������)
    lcd_fill(ORIGIN_X + 1, ORIGIN_Y - g_amplitude - 2,
             ORIGIN_X + AXIS_LENGTH - 1, ORIGIN_Y + g_amplitude + 2, BG_COLOR);
    
    // �ػ�X��Ϳ̶�
    redraw_x_axis();
    
    // ���Ʋ���
    for(i = 1; i < AXIS_LENGTH; i++) {
        lcd_draw_line(ORIGIN_X + i - 1, prev_y,
                     ORIGIN_X + i, g_wave_buffer[i], WAVE_COLOR);
        prev_y = g_wave_buffer[i];
    }
}

// ��ʾ����
void show_parameters(void) {
    char str[20];
    // ��ʾ���
    sprintf(str, "A=%.1f", g_amplitude);
    lcd_show_string(10, 10, 100, 16, 16, str, BLUE);
    
    // ��ʾƵ��
    sprintf(str, "f=%.1f", g_frequency);
    lcd_show_string(10, 30, 100, 16, 16, str, BLUE);
}

// �������
void set_amplitude(float amp) {
    g_amplitude = amp;
}

// ����Ƶ��
void set_frequency(float freq) {
    g_frequency = freq;
}

// ���в�����ʾ
void run_sine_wave(void) {
    // ��ʼ����ʾ
    draw_coordinate_system();
    show_parameters();
    init_wave_buffer();
    
    while(1) {
        // ������λ
        g_phase += 0.1f;
        if(g_phase >= 2*PI) {
            g_phase -= 2*PI;
        }
        
        // �ƶ�����ʾ����
        shift_wave_buffer();
        display_wave();
        
				if(key_scan(0) == KEY2_PRES) {
						lcd_clear(WHITE);
            break;  // ���KEY2����,�˳�ѭ��
        }
				
        // �����ƶ��ٶ�
        delay_ms(50);
    }
}

// ������ʾ��
void sine_wave_demo(void) {
    // ���ó�ʼ����
    set_amplitude(50.0f);  // �������
    set_frequency(1.0f);   // ����Ƶ��
    
    // ��������
    run_sine_wave();
}
const uint8_t TEXT_TO_SEND[] = {"Part 2"};  /* Ҫѭ�����͵��ַ��� */
#define SEND_BUF_SIZE       (sizeof(TEXT_TO_SEND) + 2) * 200
	
#define RECV_BUF_SIZE   128
uint8_t g_recvbuf[RECV_BUF_SIZE];   // ���ջ�����
uint32_t g_recvlen = 0;             // ���յ������ݳ���
uint8_t g_sendbuf[SEND_BUF_SIZE];           /* �������ݻ����� */

extern DMA_HandleTypeDef  g_dma_handle;     /* DMA��� */
extern UART_HandleTypeDef g_uart1_handle;   /* UART��� */

uint8_t g_led_state = 0;  // LED״̬,0��ʾ�ر�,1��ʾ��

static void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        if (g_recvlen < RECV_BUF_SIZE)
        {
            g_recvlen++;
        }
        HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t*)&g_recvbuf[g_recvlen], 1);
        
        if (g_recvbuf[g_recvlen-1] == '\n')  // ���յ�����ָ��
        {   
            if (strncmp((const char*)g_recvbuf, "start", 5) == 0)
            {
                printf("Received command: start\r\n");
                HAL_UART_Transmit_DMA(&g_uart1_handle, g_sendbuf, SEND_BUF_SIZE);
            }
            else if (strncmp((const char*)g_recvbuf, "led on", 6) == 0)
            {
                printf("Received command: led on\r\n");
                g_led_state = 1;  // ���LED״̬Ϊ��
                LED0(1);         // ����LED
            }
            else if (strncmp((const char*)g_recvbuf, "led off", 7) == 0)
            {
                printf("Received command: led off\r\n");
                g_led_state = 0;  // ���LED״̬Ϊ�ر�
                LED0(0);        // �ر�LED
            }
            
            memset(g_recvbuf, 0, RECV_BUF_SIZE); // ��ս��ջ�����
            g_recvlen = 0;
        }
    }
}


int main(void)
{
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��, 72Mhz */
    delay_init(72);                         /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */
		dma_init(DMA1_Channel4);                /* ��ʼ������1 TX DMA */
		

    lcd_show_string(30, 50, 400, 32, 32, "STM32F103ZET6", RED);
    lcd_show_string(30, 90, 400, 32, 32, "Yaojingxi 23722032", RED);
    lcd_show_string(30, 130, 400, 32, 32, "CS2301", RED);
	   lcd_show_string(30, 170, 400, 32, 32, "Part 1:A moving Sin wave showing", BLUE);
		//Part 1:��ʾ������Ϣ����չʾ

    delay_ms(5000);
		//��ʱ���ʮ�����Part2��1����
		lcd_clear(WHITE);
		//��ʼ����ͼ��
		sine_wave_demo();
		
		uint8_t  key = 0;
    uint16_t i, k;
    uint16_t len;
    uint8_t  mask = 0;
    float pro = 0;          /* ���� */
		short temp;
				
		lcd_show_string(30, 50, 400, 32, 32, "STM32F103ZET6", RED);
    lcd_show_string(30, 90, 400, 32, 32, "DMA + LED", RED);
    lcd_show_string(30, 130, 400, 32, 32, "Yaojingxi 23722032", RED);
    lcd_show_string(30, 170, 400, 32, 32, "You can use key0", RED);
		delay_ms(1000);
		lcd_clear(WHITE);
		
    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)       /* KEY0���� */
        {
            lcd_show_string(30, 150, 200, 3, 16, "   %", BLUE);    /* ��ʾ�ٷֺ� */
            
            HAL_UART_Transmit_DMA(&g_uart1_handle, g_sendbuf, SEND_BUF_SIZE);
            /* �ȴ�DMA������ɣ���ʱ������������һЩ���飬������  
             * ʵ��Ӧ���У����������ڼ䣬����ִ����������� 
             */

            while (1)
            {
                if ( __HAL_DMA_GET_FLAG(&g_dma_handle, DMA_FLAG_TC4))   /* �ȴ� DMA1_Channel4 ������� */
                {
                    __HAL_DMA_CLEAR_FLAG(&g_dma_handle, DMA_FLAG_TC4);
                    HAL_UART_DMAStop(&g_uart1_handle);                  /* ��������Ժ�رմ���DMA */
                    break;
                }

                pro = __HAL_DMA_GET_COUNTER(&g_dma_handle); /* �õ���ǰ��ʣ����ٸ����� */
                len = SEND_BUF_SIZE;        /* �ܳ��� */
                pro = 1 - (pro / len);      /* �õ��ٷֱ� */
                pro *= 100;                 /* ����100�� */
                lcd_show_num(30, 150, pro, 3, 16, BLUE);
            } 
						delay_ms(1000);
						lcd_clear(WHITE); 
            lcd_show_num(30, 150, 100, 3, 16, BLUE);    /* ��ʾ100% */
            lcd_show_string(30, 130, 200, 16, 16, "Transimit Finished!", BLUE); /* ��ʾ������� */
						delay_ms(1000);
						
    }
}
}