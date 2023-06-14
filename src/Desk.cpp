/*
 * Desk.cpp
 *
 *  Created on: 14 Jun 2023
 *      Author: jondurrant
 */

#include "Desk.h"

#define DESK_QUEUE_LEN 10

//Queue cmd structure
struct DeskCmd {
	DeskPart 	part;
	uint8_t 	hue;
	uint8_t 	sat;
	uint8_t 	val;
};

//Type def for the queue command
typedef struct DeskCmd DeskCmdT;


Desk::Desk() {

	xController.setBrightness(255);
	xController.fill( PicoLed::RGB(255, 255, 255) );

	xCmdQ = xQueueCreate( DESK_QUEUE_LEN, sizeof(DeskCmdT));
	if (xCmdQ == NULL){
		printf("ERROR: Unable to create Queue\n");
	}

}

Desk::~Desk() {
	// TODO Auto-generated destructor stub
}

/***
 * Get the static depth required in words
 * @return - words
 */
configSTACK_DEPTH_TYPE Desk::getMaxStackSize(){
	return 100;
}

/***
 * Run loop for the agent.
 */
void Desk::run(){
	BaseType_t res;
	DeskCmdT cmd;
	for (;;){
		res = xQueueReceive(xCmdQ, (void *)&cmd, 0);
		if (res == pdTRUE){
			switch(cmd.part){
			case FULL_DESK:{
				xController.fill( PicoLed::HSV(cmd.hue, cmd.sat, cmd.val) );
				break;
			}
			case LEFT_DESK:{
				xController.fill(
						PicoLed::HSV(cmd.hue, cmd.sat, cmd.val),
						LEFT_START, LEFT_LEN);
				break;
			}
			case RIGHT_DESK:{
				xController.fill(
						PicoLed::HSV(cmd.hue, cmd.sat, cmd.val),
						RIGHT_START, RIGHT_LEN);
				break;
			}
			case UNDER_KB_DESK:{
				xController.fill(
						PicoLed::HSV(cmd.hue, cmd.sat, cmd.val),
						UNDER_KB_START, UNDER_KB_LEN);
				break;
			}
			case OVER_KB_DESK:{
				xController.fill(
						PicoLed::HSV(cmd.hue, cmd.sat, cmd.val),
						OVER_KB_START, OVER_KB_LEN);
				break;
			}
			case BACK_KB_DESK:{
				xController.fill(
						PicoLed::HSV(cmd.hue, cmd.sat, cmd.val),
						BACK_KB_START, BACK_KB_LEN);
				break;
			}
			case UNDER_DESK:{
				xController.fill(
						PicoLed::HSV(cmd.hue, cmd.sat, cmd.val),
						LEFT_START, LEFT_LEN);
				xController.fill(
						PicoLed::HSV(cmd.hue, cmd.sat, cmd.val),
						RIGHT_START, RIGHT_LEN);
				xController.fill(
						PicoLed::HSV(cmd.hue, cmd.sat, cmd.val),
						UNDER_KB_START, UNDER_KB_LEN);
				xController.fill(
						PicoLed::HSV(cmd.hue, cmd.sat, cmd.val),
						BACK_KB_START, BACK_KB_LEN);
				break;
			}
			}

		}


		xController.show();
		vTaskDelay(100);
	}
}


void Desk::full(uint8_t hue, uint8_t sat, uint8_t val){
	mode(FULL_DESK, hue, sat, val);
}

void Desk::mode(DeskPart part, uint8_t hue, uint8_t sat, uint8_t val){
	BaseType_t res;

	DeskCmdT cmd;
	cmd.part = part;
	cmd.hue = hue;
	cmd.sat	 = sat;
	cmd.val  = val;

	if (xCmdQ != NULL){
		res = xQueueSendToBack(xCmdQ, (void *)&cmd, 0);
		if (res != pdTRUE){
			printf("WARNING: Queue is full\n");
		}
	}
}

void Desk::preset(DeskPreset preset){
	switch(preset){
	case DESK_PRESET_0:{
		//WHITE
		mode(FULL_DESK, 0, 0, 255);
		break;
	}
	case DESK_PRESET_1:{
		//
		mode(UNDER_DESK, 160, 255, 90);
		mode(OVER_KB_DESK, 0, 0, 255);
		break;

	}
	case DESK_PRESET_2:{
		mode(UNDER_DESK, 0, 255, 90);
		mode(OVER_KB_DESK, 0, 0, 255);
		break;
	}
	case DESK_PRESET_3:{
		mode(UNDER_DESK, 0, 0, 255);
		mode(OVER_KB_DESK, 0, 0, 255);
		break;
	}
	default:{
		mode(FULL_DESK, 0, 0, 0);
		break;
	}
	}
}

void Desk::bright(uint8_t bri){
	xController.setBrightness(bri);
}
