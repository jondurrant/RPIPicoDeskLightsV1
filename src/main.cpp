/***
 * Control Turntable and Led Lights
 *
 * Uses FreeRTOS Task
 * Jon Durrant
 * 15-Aug-2022
 */


#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#include "BlinkAgent.h"

#include <PicoLed.hpp>
#include "OledDisplay.h"

#include "RotEncListener.h"
#include "RotEncAgent.h"
#include "Desk.h"
#include "DeskControl.h"

#include <VirtualStrip.hpp>



//Standard Task priority
#define TASK_PRIORITY		( tskIDLE_PRIORITY + 1UL )

//LED PAD to use
#define LED_PAD			2
#define NEOPIXEL1_PAD	26
#define NEOPIXEL2_PAD	27

//Rotary Encoder
#define ROTENC_SW_PAD	4
#define ROTENC_A_PAD	3
#define ROTENC_B_PAD	5
#define ROTENC_COUNT	20


//Switches
#define SW1_PAD			6
#define SW2_PAD			7
#define SW3_PAD			8
#define SW4_PAD			9


//OLED I2C
#define OLED_SDA_PAD	14
#define OLED_SDL_PAD	15


/***
 * Debug function to look at Task Stats
 */
void runTimeStats(   ){
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime;


   // Get number of takss
   uxArraySize = uxTaskGetNumberOfTasks();
   printf("Number of tasks %d\n", uxArraySize);

   //Allocate a TaskStatus_t structure for each task.
   pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

   if( pxTaskStatusArray != NULL ){
      // Generate raw status information about each task.
      uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
                                 uxArraySize,
                                 &ulTotalRunTime );

	 // Print stats
	 for( x = 0; x < uxArraySize; x++ )
	 {
		 printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
				pxTaskStatusArray[ x ].xTaskNumber ,
				pxTaskStatusArray[ x ].uxCurrentPriority ,
				pxTaskStatusArray[ x ].uxBasePriority ,
				pxTaskStatusArray[ x ].usStackHighWaterMark ,
				pxTaskStatusArray[ x ].pcTaskName
				);
	 }


      // Free array
      vPortFree( pxTaskStatusArray );
   } else {
	   printf("Failed to allocate space for stats\n");
   }

   //Get heap allocation information
   HeapStats_t heapStats;
   vPortGetHeapStats(&heapStats);
   printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
		   heapStats.xAvailableHeapSpaceInBytes,
		   heapStats.xNumberOfFreeBlocks,
		   heapStats.xNumberOfSuccessfulAllocations,
		   heapStats.xNumberOfSuccessfulFrees
		   );
}


/***
 * Main task to boot the other Agents
 * @param params - unused
 */
void mainTask(void *params){
	BlinkAgent blink(LED_PAD);

	printf("Boot task started\n");


	blink.start("Blink", TASK_PRIORITY);

	Desk desk;
	desk.start("Desk", TASK_PRIORITY);

	RotEncAgent rotEnc(
				ROTENC_SW_PAD,
				ROTENC_A_PAD,
				ROTENC_B_PAD,
				ROTENC_COUNT);
	rotEnc.start("RotEnc", TASK_PRIORITY);

	DeskControl control;
	control.initRotEnc(&rotEnc);
	control.initSws(SW1_PAD, SW2_PAD, SW3_PAD, SW4_PAD);
	control.initDesk(&desk);

	for (;;){
		vTaskDelay(2000);
	}

}



void old(){
	BlinkAgent blink(LED_PAD);

	printf("Boot task started\n");


	blink.start("Blink", TASK_PRIORITY);

/*
	uint8_t sw = 26;
	gpio_init(sw);

	gpio_set_dir(sw, GPIO_OUT);


	for(;;){
		gpio_put(sw, 1);
		vTaskDelay(2);
		gpio_put(sw, 0);
		vTaskDelay(2);
	}
*/

	//auto ledStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, NEOPIXEL2_PAD, 10, PicoLed::FORMAT_GRB);


	int max = 146;
	auto ledStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, NEOPIXEL1_PAD, max, PicoLed::FORMAT_GRB);
	ledStrip.setBrightness(255);
	ledStrip.fill( PicoLed::RGB(150, 150, 255) );
	ledStrip.show();


	/*
	auto ledStrip2 = PicoLed::addLeds<PicoLed::WS2812B>(pio1, 0, NEOPIXEL2_PAD, 10, PicoLed::FORMAT_GRB);
	ledStrip2.setBrightness(255);
	ledStrip2.fill( PicoLed::RGB(255, 50, 50) );
	ledStrip2.show();
	*/


	RotEncAgent rotEnc(
			ROTENC_SW_PAD,
			ROTENC_A_PAD,
			ROTENC_B_PAD,
			ROTENC_COUNT);
	rotEnc.start("RotEnc", TASK_PRIORITY);






	int count =0;
	for (;;){
		switch(count++){
		case 0:{
			ledStrip.fill( PicoLed::RGB(255, 255, 255) );
			break;
		}
		case 1:{
			//LEFT
			ledStrip.fill( PicoLed::RGB(0, 0, 0) );
			ledStrip.fill(PicoLed::RGB(0, 255, 0), 0, 21);
			break;
		}
		case 2:{
			//Under KB
			ledStrip.fill( PicoLed::RGB(0, 0, 0) );
			ledStrip.fill(PicoLed::RGB(80, 255, 80), 21, 36);
			break;
		}
		case 3:{
			// KB Back
			ledStrip.fill( PicoLed::RGB(0, 0, 0) );
			ledStrip.fill(PicoLed::RGB(255, 80, 80), 57, 34);
			break;
		}
		case 4:{
			// KB Front
			ledStrip.fill( PicoLed::RGB(0, 0, 0) );
			ledStrip.fill(PicoLed::RGB(255, 120, 40), 91, 34);
			break;
		}
		case 5:{
			// RIGHT
			ledStrip.fill( PicoLed::RGB(0, 0, 0) );
			ledStrip.fill(PicoLed::RGB(50, 255, 50), 125, 21);
			break;
		}
		default: {
			count = 0;
		}
		}

		for (int i=0; i < 20; i++){
			ledStrip.show();
			vTaskDelay(100);
		}
	}




	for (;;){
		printf("Count = %d\n", count);
		ledStrip.fill( PicoLed::RGB(150, 150, 255) );

		//Left 0 to 20 (21)
		ledStrip.fill(PicoLed::RGB(50, 255, 50), 0, 21);

		//Under KB 21 to 57 (36)
		ledStrip.fill(PicoLed::RGB(80, 255, 80), 21, 36);

		//KB Back 58 to 90 (32)
		ledStrip.fill(PicoLed::RGB(255, 80, 80), 58, 32);

		//KB Front 91 to 124 (33)
		ledStrip.fill(PicoLed::RGB(255, 120, 40), 91, 33);

		//Right 124 to 146 (22)
		ledStrip.fill(PicoLed::RGB(50, 255, 50), 124, 22);

		ledStrip.setPixelColor(count++, PicoLed::RGB(255, 0, 0));
		ledStrip.show();
		vTaskDelay(1000);

		printf("Count = %d\n", count);
		ledStrip.fill( PicoLed::RGB(255, 150, 150) );
		ledStrip.setPixelColor(count++, PicoLed::RGB(255, 0, 0));
		ledStrip.show();
		vTaskDelay(1000);

		if (count > max){
			count = 0;
		}

	}


}




/***
 * Launch the tasks and scheduler
 */
void vLaunch( void) {

	//Start blink task
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 500, NULL, TASK_PRIORITY, &task);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

/***
 * Main
 * @return
 */
int main( void )
{
	//Setup serial over USB and give a few seconds to settle before we start
    stdio_init_all();
    sleep_ms(2000);
    printf("GO\n");

    //Start tasks and scheduler
    const char *rtos_name = "FreeRTOS";
    printf("Starting %s on core 0:\n", rtos_name);
    vLaunch();


    return 0;
}
