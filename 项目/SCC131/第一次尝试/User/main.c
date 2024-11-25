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
 * @brief       画粗线
 * @param       x1,y1: 起点坐标
 * @param       x2,y2: 终点坐标
 * @param       size : 线条粗细程度
 * @param       color: 线的颜色
 */
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;

    if (x1 < size || x2 < size || y1 < size || y2 < size)
        return;

    delta_x = x2 - x1; /* 计算坐标增量 */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)
    {
        incx = 1; /* 设置单步方向 */
    }
    else if (delta_x == 0)
    {
        incx = 0; /* 垂直线 */
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
        incy = 0; /* 水平线 */
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
        distance = delta_x; /* 选取基本增量坐标轴 */
    else
        distance = delta_y;

    for (t = 0; t <= distance + 1; t++) /* 画线输出 */
    {
        lcd_fill_circle(row, col, size, color); /* 画点 */
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
#define ORIGIN_X 40     // X轴原点坐标
#define ORIGIN_Y 350	    // Y轴原点坐标
#define AXIS_LENGTH 400 // 轴长度
#define GRID_SIZE 20    // 网格大小
#define WAVE_COLOR RED  // 波形颜色
#define BG_COLOR WHITE  // 背景色
#define MOVE_STEP 2     // 每次移动的像素数(必须能被AXIS_LENGTH整除)

float g_amplitude = 50.0f;   // 振幅，可调节
float g_frequency = 12.0f;    // 周期，可调节
float g_phase = 0.0f;        // 相位，用于移动
uint16_t g_wave_buffer[AXIS_LENGTH];  // 波形缓冲区

// 绘制坐标轴
void draw_coordinate_system(void) {
    // 清屏
    lcd_fill(0, 0, lcddev.width, lcddev.height, BG_COLOR);
    
    // 绘制X轴
    lcd_draw_line(ORIGIN_X, ORIGIN_Y, ORIGIN_X + AXIS_LENGTH, ORIGIN_Y, BLACK);
    // X轴箭头
    lcd_draw_line(ORIGIN_X + AXIS_LENGTH, ORIGIN_Y, ORIGIN_X + AXIS_LENGTH - 10, ORIGIN_Y - 5, BLACK);
    lcd_draw_line(ORIGIN_X + AXIS_LENGTH, ORIGIN_Y, ORIGIN_X + AXIS_LENGTH - 10, ORIGIN_Y + 5, BLACK);
    
    // 绘制Y轴
    lcd_draw_line(ORIGIN_X, ORIGIN_Y - AXIS_LENGTH/2, ORIGIN_X, ORIGIN_Y + AXIS_LENGTH/2, BLACK);
    // Y轴箭头
    lcd_draw_line(ORIGIN_X, ORIGIN_Y - AXIS_LENGTH/2, ORIGIN_X - 5, ORIGIN_Y - AXIS_LENGTH/2 + 10, BLACK);
    lcd_draw_line(ORIGIN_X, ORIGIN_Y - AXIS_LENGTH/2, ORIGIN_X + 5, ORIGIN_Y - AXIS_LENGTH/2 + 10, BLACK);
    
    // 绘制刻度线
    for(int i = 1; i < AXIS_LENGTH/GRID_SIZE; i++) {
        // X轴刻度
        lcd_draw_line(ORIGIN_X + i*GRID_SIZE, ORIGIN_Y-3, 
                     ORIGIN_X + i*GRID_SIZE, ORIGIN_Y+3, BLACK);
        
        // Y轴刻度
        lcd_draw_line(ORIGIN_X-3, ORIGIN_Y - i*GRID_SIZE/2, 
                     ORIGIN_X+3, ORIGIN_Y - i*GRID_SIZE/2, BLACK);
        lcd_draw_line(ORIGIN_X-3, ORIGIN_Y + i*GRID_SIZE/2, 
                     ORIGIN_X+3, ORIGIN_Y + i*GRID_SIZE/2, BLACK);
    }
}

// 重绘X轴和刻度线
void redraw_x_axis(void) {
    // 重绘X轴
    lcd_draw_line(ORIGIN_X, ORIGIN_Y, ORIGIN_X + AXIS_LENGTH, ORIGIN_Y, BLACK);
    
    // 重绘X轴刻度线
    for(int i = 1; i < AXIS_LENGTH/GRID_SIZE; i++) {
        lcd_draw_line(ORIGIN_X + i*GRID_SIZE, ORIGIN_Y-3, 
                     ORIGIN_X + i*GRID_SIZE, ORIGIN_Y+3, BLACK);
    }
}

// 计算波形值
int calculate_wave_point(float x, float phase) {
    return ORIGIN_Y - (int)(g_amplitude * sinf((x/GRID_SIZE) * g_frequency * PI + phase));
}

// 初始化波形缓冲区
void init_wave_buffer(void) {
    int i;
    for(i = 0; i < AXIS_LENGTH; i++) {
        g_wave_buffer[i] = calculate_wave_point((float)i, g_phase);
    }
}

// 将波形数据左移
void shift_wave_buffer(void) {
    int i;
    
    // 移动现有数据
    for(i = 0; i < AXIS_LENGTH - MOVE_STEP; i++) {
        g_wave_buffer[i] = g_wave_buffer[i + MOVE_STEP];
    }
    
    // 计算新的点
    float phase = g_phase + ((float)AXIS_LENGTH/GRID_SIZE) * g_frequency * PI;
    for(i = AXIS_LENGTH - MOVE_STEP; i < AXIS_LENGTH; i++) {
        g_wave_buffer[i] = calculate_wave_point((float)i, g_phase);
    }
}

// 显示波形
void display_wave(void) {
    int i;
    int prev_y = g_wave_buffer[0];
    
    // 清除波形区域(保护坐标轴)
    lcd_fill(ORIGIN_X + 1, ORIGIN_Y - g_amplitude - 2,
             ORIGIN_X + AXIS_LENGTH - 1, ORIGIN_Y + g_amplitude + 2, BG_COLOR);
    
    // 重绘X轴和刻度
    redraw_x_axis();
    
    // 绘制波形
    for(i = 1; i < AXIS_LENGTH; i++) {
        lcd_draw_line(ORIGIN_X + i - 1, prev_y,
                     ORIGIN_X + i, g_wave_buffer[i], WAVE_COLOR);
        prev_y = g_wave_buffer[i];
    }
}

// 显示参数
void show_parameters(void) {
    char str[20];
    // 显示振幅
    sprintf(str, "A=%.1f", g_amplitude);
    lcd_show_string(10, 10, 100, 16, 16, str, BLUE);
    
    // 显示频率
    sprintf(str, "f=%.1f", g_frequency);
    lcd_show_string(10, 30, 100, 16, 16, str, BLUE);
}

// 设置振幅
void set_amplitude(float amp) {
    g_amplitude = amp;
}

// 设置频率
void set_frequency(float freq) {
    g_frequency = freq;
}

// 运行波形显示
void run_sine_wave(void) {
    // 初始化显示
    draw_coordinate_system();
    show_parameters();
    init_wave_buffer();
    
    while(1) {
        // 更新相位
        g_phase += 0.1f;
        if(g_phase >= 2*PI) {
            g_phase -= 2*PI;
        }
        
        // 移动并显示波形
        shift_wave_buffer();
        display_wave();
        
				if(key_scan(0) == KEY2_PRES) {
						lcd_clear(WHITE);
            break;  // 如果KEY2按下,退出循环
        }
				
        // 控制移动速度
        delay_ms(50);
    }
}

// 主函数示例
void sine_wave_demo(void) {
    // 设置初始参数
    set_amplitude(50.0f);  // 设置振幅
    set_frequency(1.0f);   // 设置频率
    
    // 启动动画
    run_sine_wave();
}
const uint8_t TEXT_TO_SEND[] = {"Part 2"};  /* 要循环发送的字符串 */
#define SEND_BUF_SIZE       (sizeof(TEXT_TO_SEND) + 2) * 200
	
#define RECV_BUF_SIZE   128
uint8_t g_recvbuf[RECV_BUF_SIZE];   // 接收缓冲区
uint32_t g_recvlen = 0;             // 接收到的数据长度
uint8_t g_sendbuf[SEND_BUF_SIZE];           /* 发送数据缓冲区 */

extern DMA_HandleTypeDef  g_dma_handle;     /* DMA句柄 */
extern UART_HandleTypeDef g_uart1_handle;   /* UART句柄 */

uint8_t g_led_state = 0;  // LED状态,0表示关闭,1表示打开

static void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        if (g_recvlen < RECV_BUF_SIZE)
        {
            g_recvlen++;
        }
        HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t*)&g_recvbuf[g_recvlen], 1);
        
        if (g_recvbuf[g_recvlen-1] == '\n')  // 接收到完整指令
        {   
            if (strncmp((const char*)g_recvbuf, "start", 5) == 0)
            {
                printf("Received command: start\r\n");
                HAL_UART_Transmit_DMA(&g_uart1_handle, g_sendbuf, SEND_BUF_SIZE);
            }
            else if (strncmp((const char*)g_recvbuf, "led on", 6) == 0)
            {
                printf("Received command: led on\r\n");
                g_led_state = 1;  // 标记LED状态为打开
                LED0(1);         // 点亮LED
            }
            else if (strncmp((const char*)g_recvbuf, "led off", 7) == 0)
            {
                printf("Received command: led off\r\n");
                g_led_state = 0;  // 标记LED状态为关闭
                LED0(0);        // 关闭LED
            }
            
            memset(g_recvbuf, 0, RECV_BUF_SIZE); // 清空接收缓冲区
            g_recvlen = 0;
        }
    }
}


int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
    led_init();                             /* 初始化LED */
    lcd_init();                             /* 初始化LCD */
    key_init();                             /* 初始化按键 */
		dma_init(DMA1_Channel4);                /* 初始化串口1 TX DMA */
		

    lcd_show_string(30, 50, 400, 32, 32, "STM32F103ZET6", RED);
    lcd_show_string(30, 90, 400, 32, 32, "Yaojingxi 23722032", RED);
    lcd_show_string(30, 130, 400, 32, 32, "CS2301", RED);
	   lcd_show_string(30, 170, 400, 32, 32, "Part 1:A moving Sin wave showing", BLUE);
		//Part 1:显示基础信息报告展示

    delay_ms(5000);
		//延时大概十秒进入Part2：1清屏
		lcd_clear(WHITE);
		//开始绘制图形
		sine_wave_demo();
		
		uint8_t  key = 0;
    uint16_t i, k;
    uint16_t len;
    uint8_t  mask = 0;
    float pro = 0;          /* 进度 */
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

        if (key == KEY0_PRES)       /* KEY0按下 */
        {
            lcd_show_string(30, 150, 200, 3, 16, "   %", BLUE);    /* 显示百分号 */
            
            HAL_UART_Transmit_DMA(&g_uart1_handle, g_sendbuf, SEND_BUF_SIZE);
            /* 等待DMA传输完成，此时我们来做另外一些事情，比如点灯  
             * 实际应用中，传输数据期间，可以执行另外的任务 
             */

            while (1)
            {
                if ( __HAL_DMA_GET_FLAG(&g_dma_handle, DMA_FLAG_TC4))   /* 等待 DMA1_Channel4 传输完成 */
                {
                    __HAL_DMA_CLEAR_FLAG(&g_dma_handle, DMA_FLAG_TC4);
                    HAL_UART_DMAStop(&g_uart1_handle);                  /* 传输完成以后关闭串口DMA */
                    break;
                }

                pro = __HAL_DMA_GET_COUNTER(&g_dma_handle); /* 得到当前还剩余多少个数据 */
                len = SEND_BUF_SIZE;        /* 总长度 */
                pro = 1 - (pro / len);      /* 得到百分比 */
                pro *= 100;                 /* 扩大100倍 */
                lcd_show_num(30, 150, pro, 3, 16, BLUE);
            } 
						delay_ms(1000);
						lcd_clear(WHITE); 
            lcd_show_num(30, 150, 100, 3, 16, BLUE);    /* 显示100% */
            lcd_show_string(30, 130, 200, 16, 16, "Transimit Finished!", BLUE); /* 提示传送完成 */
						delay_ms(1000);
						
    }
}
}