/*
 * DeskControl.cpp
 *
 *  Created on: 14 Jun 2023
 *      Author: jondurrant
 */

#include "DeskControl.h"

DeskControl::DeskControl() {
	// TODO Auto-generated constructor stub

}

DeskControl::~DeskControl() {
	// TODO Auto-generated destructor stub
}

/***
 * Setup the Rotary Encocder Agent
 * @param agent - Rotary encoder agent
 */
void DeskControl::initRotEnc(RotEncAgent *agent){
	pRotEnc = agent;
	pRotEnc->setListener(this);
}

/***
 * Setup the Switches
 * @param sw1 - GPIO number
 * @param sw2 - GPIO number
 * @param sw3 - GPIO number
 * @param sw4 - GPIO number
 */
void DeskControl::initSws(uint8_t sw1, uint8_t sw2, uint8_t sw3, uint8_t sw4){
	xSw1 = sw1;
	xSw2 = sw2;
	xSw3 = sw3;
	xSw4 = sw4;

	pSwitch[0]  = new SwitchMgr(xSw1);
	pSwitch[1]  = new SwitchMgr(xSw2);
	pSwitch[2]  = new SwitchMgr(xSw3);
	pSwitch[3]  = new SwitchMgr(xSw4);
	for (int i=0; i < NUM_SWITCHES; i++){
		pSwitch[i]->setObserver(this);
	}
}


/***
 * Initialise with desk object
 * @param desk
 */
void DeskControl::initDesk(Desk *desk){
	pDesk = desk;
}


/***
 * Handle a short press from the switch
 * @param gp - GPIO number of the switch
 */
void DeskControl::handleShortPress(uint8_t gp){
	if (gp == xSw1){
		pDesk->preset(DESK_PRESET_0);
	}
	if (gp == xSw2){
		pDesk->preset(DESK_PRESET_1);
	}
	if (gp == xSw3){
		pDesk->preset(DESK_PRESET_2);
	}
	if (gp == xSw4){
		pDesk->preset(DESK_PRESET_3);
	}

}

/***
 * Handle a short press from the switch
 * @param gp - GPIO number of the switch
 */
void DeskControl::handleLongPress(uint8_t gp){
	handleShortPress(gp);
}

/***
 * Notify of short Press
 * @param rotEnv - Ref to RotEnc in case multiple being used
 */
void DeskControl::shortPress(void * rotEnc){
	xMode ++;

	if (xMode > 3){
		xMode = 0;
	}
}

/***
 * Notify of long Press
 * @param rotEnv - Ref to RotEnc in case multiple being used
 */
void DeskControl::longPress(void * rotEnc){
	shortPress(rotEnc);
}

/***
 * Notification of rotation
 * @param clockwise - Direction. True if clockwise
 * @param pos - Current possition (within 360)
 * @param rotEnc - Ref to RotEnc in case multiple being used
 */
void DeskControl::rotate(bool clockwise, int16_t pos, void * rotEnc){
	switch(xMode){
	case 0:{
		if (!clockwise){
			xBri+=5;
			if (xBri > 255){
				xBri = 255;
			}
		} else {
			xBri-=5;
			if (xBri < 0){
				xBri = 0;
			}
		}
		break;
	}
	case 1:{
		if (!clockwise){
			xHue+=5;
			if (xHue > 255){
				xHue = 255;
			}
		} else {
			xHue-=5;
			if (xHue < 0){
				xHue = 0;
			}
		}
		break;
	}
	case 2:{
		if (!clockwise){
			xSat+=5;
			if (xSat > 255){
				xSat = 255;
			}
		} else {
			xSat-=5;
			if (xSat < 0){
				xSat = 0;
			}
		}
		break;
	}
	case 3: {
		if (!clockwise){
			xVal+=5;
			if (xVal > 255){
				xVal = 255;
			}
		} else {
			xVal-=5;
			if (xVal < 0){
				xVal = 0;
			}
		}
		break;
	}
	}

	if (xMode > 0){
		pDesk->full(xHue, xSat, xVal);
		printf("MANUAL %d, %d, %d\n", xHue, xSat, xVal);
	} else {
		pDesk->bright(xBri);
		printf("BRIGHT %d\n", xBri);
	}

}
