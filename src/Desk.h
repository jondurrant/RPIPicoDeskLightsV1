/*
 * Desk.h
 *
 * Manage the desk LED Setup
 *
 *  Created on: 14 Jun 2023
 *      Author: jondurrant
 */

#ifndef SRC_DESK_H_
#define SRC_DESK_H_

#include <pico/stdlib.h>
#include "Agent.h"
#include "queue.h"
#include <PicoLed.hpp>

enum DeskPart {FULL_DESK, LEFT_DESK, RIGHT_DESK, UNDER_KB_DESK, OVER_KB_DESK, BACK_KB_DESK, UNDER_DESK};

enum DeskPreset {DESK_PRESET_0, DESK_PRESET_1, DESK_PRESET_2, DESK_PRESET_3, DESK_PRESET_OFF };

class Desk : public Agent{
public:
	Desk();
	virtual ~Desk();

	/***
	 * Control all the desk lamps as a set
	 * @param hue
	 * @param sat
	 * @param val
	 */
	void full(uint8_t hue, uint8_t sat, uint8_t val);

	/***
	 * Control a set of the lamps
	 * @param part
	 * @param hue
	 * @param sat
	 * @param val
	 */
	void mode(DeskPart part, uint8_t hue, uint8_t sat, uint8_t val);

	/***
	 * Activate preset
	 * @param preset
	 */
	void preset(DeskPreset preset);

	/***
	 * Set Brightness
	 * @param bri
	 */
	void bright(uint8_t bri);


protected:


	/***
	 * Get the static depth required in words
	 * @return - words
	 */
	virtual configSTACK_DEPTH_TYPE getMaxStackSize();

	/***
	 * Task main run loop
	 */
	virtual void run();

	//GPIO PAD for LED
	uint8_t xWS2812BPad = 26;

	PicoLed::PicoLedController xController= PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, 26,
			146, PicoLed::FORMAT_GRB);;

	//Queue of commands
	QueueHandle_t xCmdQ;


	const uint LED_LENGTH 		=  146;
	const uint LEFT_START 		= 0;
	const uint LEFT_LEN 		= 21;
	const uint RIGHT_START 		= 125;
	const uint RIGHT_LEN 		= 21;
	const uint UNDER_KB_START 	= 21;
	const uint UNDER_KB_LEN		= 36;
	const uint OVER_KB_START	= 91;
	const uint OVER_KB_LEN		= 34;
	const uint BACK_KB_START	= 57;
	const uint BACK_KB_LEN		= 34;


};

#endif /* SRC_DESK_H_ */
