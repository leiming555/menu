#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "Timer.h"
#include "Encoder.h"
#include "Key.h"
#include "OLED.h"
#include "Menu.h"
#include "LED.h"
#include "stepmotor.h"
#include "ASRPRO.h"
#include "Serial.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"

int Power_Off(void); // 关机
volatile u8 curtainBusy = 0; // 0表示空闲，1表示正在执行窗帘命令
volatile u8 mode=0;
typedef enum {
    CMD_NONE = 0,
    CMD_LIGHT_ON,
    CMD_LIGHT_OFF,
    // 可以扩展其它命令
} LightCommand_t;

typedef enum {
    CMD_NONE2 = 0,
    CMD_CURTAIN_ON,
    CMD_CURTAIN_OFF,
    // 可以扩展其它命令
} CurtainCommand_t;



QueueHandle_t xLightQueue;
QueueHandle_t xCurtainQueue;




void MenuTask(void *pvParameters)
{
    for (;;) {
			 Menu_RunMainMenu();		
        // 当用户在菜单中选择“灯打开”
        if (mode==6/*检测到菜单打开灯命令*/)
					{
            LightCommand_t cmd = CMD_LIGHT_ON;
            xQueueSend(xLightQueue, &cmd, portMAX_DELAY);
           }
				if (mode==0/*检测到菜单关闭灯命令*/) 
					{
            LightCommand_t cmd = CMD_LIGHT_OFF;
            xQueueSend(xLightQueue, &cmd, portMAX_DELAY);
           }
				if ((mode == 1 || mode == 2) && (curtainBusy == 0)) 
         {
    CurtainCommand_t cmd;
    if (mode == 1)
        cmd = CMD_CURTAIN_ON;
    else
        cmd = CMD_CURTAIN_OFF;
    xQueueSend(xCurtainQueue, &cmd, portMAX_DELAY);
          }

				
        // 添加适当的延时
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void SpeechTask(void *pvParameters)
{
    for (;;) {
			  mode = RxFlag_deal();
         mode_deal();
        // 根据语音指令决定灯状态
        if ( mode==0/*检测到语音命令：灯关闭*/) {
            LightCommand_t cmd = CMD_LIGHT_OFF;
            xQueueSend(xLightQueue, &cmd, portMAX_DELAY);
        }
        // 延时或等待事件
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}




void LightControlTask(void *pvParameters)
{
    LightCommand_t cmd;
    for (;;) {
        // 阻塞等待命令（例如等待 100 ms 内无命令时返回 CMD_NONE 也可以）
        if (xQueueReceive(xLightQueue, &cmd, portMAX_DELAY) == pdPASS) {
            switch (cmd) {
                case CMD_LIGHT_ON:
                    // 调用打开灯的函数
                    Red_on();
                    break;
                case CMD_LIGHT_OFF:
                    // 调用关闭灯的函数
                    Red_off();
                    break;
                default:
                    break;
            }
        }
    }
}

void CurtainControlTask(void *pvParameters)
{
    CurtainCommand_t cmd;
    for (;;) {
        if (xQueueReceive(xCurtainQueue, &cmd, portMAX_DELAY) == pdPASS) {
            // 执行动作前挂起接收其它命令
            if (curtainBusy == 1)
                continue;
            curtainBusy = 1;

            switch (cmd) {
                case CMD_CURTAIN_ON:
                    MOTOR_Direction_Angle(1, 0, 90, 1);
                    MOTOR_STOP();
                    break;
                case CMD_CURTAIN_OFF:
                    MOTOR_Direction_Angle(0, 0, 90, 1);
                    MOTOR_STOP();
                    break;
                default:
                    break;
            }
            // 任务挂起，直到外部条件恢复
            vTaskSuspend(NULL);

            // 当任务恢复后，清除状态
            curtainBusy = 0;
        }
    }
}




int main(void)
{
  LED_Init();
	Timer_Init();
	OLED_Init();
	Encoder_Init();
	Key_Init();
	MOTOR_Init();
	Serial_Init();  //串口1初始化,可打印调试
	ASRPRO_Init();  //语音助手所用串口初始化
	
    // 创建队列，长度例如 10
    xLightQueue = xQueueCreate(10, sizeof(LightCommand_t));
    xCurtainQueue = xQueueCreate(10, sizeof(CurtainCommand_t));
    // 创建任务，优先级相同或适当配置
    xTaskCreate(MenuTask, "MenuTask", 256, NULL, 2, NULL);
    xTaskCreate(SpeechTask, "SpeechTask", 256, NULL, 2, NULL);
    xTaskCreate(LightControlTask, "LightCtl", 256, NULL, 2, NULL);
    xTaskCreate(CurtainControlTask, "CurtainCtl", 256, NULL, 2, NULL);
    // 启动调度器
    vTaskStartScheduler();

    // 永远不应到达这里
    for (;;) {}
	
	
	
	
	
	

}











































int Power_Off(void) // 关机
{
	/*关闭外设区*******/
	OLED_Clear();
	OLED_Update();
	/*******关闭外设区*/

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // Enables or disables the Low Speed APB (APB1) peripheral clock.
	PWR_WakeUpPinCmd(ENABLE);

	PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI); // SIOP模式
	SystemInit();

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, DISABLE); // Enables or disables the Low Speed APB (APB1) peripheral clock.
	PWR_WakeUpPinCmd(DISABLE);

	Key_Reset_All();
	Key_Reset_All();

	return 1;
}

StaticTask_t	IdleTaskTCB;
StackType_t		IdleTaskStack[configMINIMAL_STACK_SIZE];
void vApplicationGetIdleTaskMemory( StaticTask_t 	**ppxIdleTaskTCBBuffer, 
									StackType_t 	**ppxIdleTaskStackBuffer, 
									uint32_t 		*pulIdleTaskStackSize )
{
	*ppxIdleTaskTCBBuffer = &IdleTaskTCB;
	*ppxIdleTaskStackBuffer = IdleTaskStack;	
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;

}

