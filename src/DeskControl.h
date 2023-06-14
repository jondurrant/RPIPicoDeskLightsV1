/*
 * DeskControl.h
 *
 * Desk Control
 * Set Switches to be preset
 * RotEnc to adjust Brightness, Hue, Sat, Val
 *
 *  Created on: 14 Jun 2023
 *      Author: jondurrant
 */

#ifndef SRC_DESKCONTROL_H_
#define SRC_DESKCONTROL_H_

#include "SwitchObserver.h"
#include "SwitchMgr.h"
#include "RotEncListener.h"
#include "RotEncAgent.h"
#include "Desk.h"

#define NUM_SWITCHES 4

class DeskControl : public SwitchObserver, public RotEncListener {
public:
	DeskControl();
	virtual ~DeskControl();

	/***
	 * Setup the Rotary Encocder Agent
	 * @param agent - Rotary encoder agent
	 */
	void initRotEnc(RotEncAgent *agent);

	/***
	 * Setup the Switches
	 * @param sw1 - GPIO number
	 * @param sw2 - GPIO number
	 * @param sw3 - GPIO number
	 * @param sw4 - GPIO number
	 */
	void initSws(uint8_t sw1, uint8_t sw2, uint8_t sw3, uint8_t sw4);


	/***
	 * Initialise with desk object
	 * @param desk
	 */
	void initDesk(Desk *desk);


	/***
	 * Handle a short press from the switch
	 * @param gp - GPIO number of the switch
	 */
	virtual void handleShortPress(uint8_t gp);

	/***
	 * Handle a short press from the switch
	 * @param gp - GPIO number of the switch
	 */
	virtual void handleLongPress(uint8_t gp);

	/***
	 * Notify of short Press
	 * @param rotEnv - Ref to RotEnc in case multiple being used
	 */
	virtual void shortPress(void * rotEnc);

	/***
	 * Notify of long Press
	 * @param rotEnv - Ref to RotEnc in case multiple being used
	 */
	virtual void longPress(void * rotEnc);

	/***
	 * Notification of rotation
	 * @param clockwise - Direction. True if clockwise
	 * @param pos - Current possition (within 360)
	 * @param rotEnc - Ref to RotEnc in case multiple being used
	 */
	virtual void rotate(bool clockwise, int16_t pos, void * rotEnc);

private:
	RotEncAgent * pRotEnc = NULL;
	Desk * pDesk = NULL;
	SwitchMgr *pSwitch[NUM_SWITCHES];
	uint8_t xSw1, xSw2, xSw3, xSw4;

	//WS2812B LED parameters
	int16_t xHue 	= 0;
	int16_t xSat 	= 0;
	int16_t xVal 	= 0;
	int16_t xBri 	= 255;
	uint8_t xMode	= 0;




};

#endif /* SRC_DESKCONTROL_H_ */
