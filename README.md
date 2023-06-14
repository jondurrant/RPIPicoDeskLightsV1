# RPIPicoDeskLightsV1

Use a Raspberry PI Pico and WS2812B LEDs to have an underdesk lighting system.
Fitted to my stand up desk.

## Hardware

Reuses the control board build for my [Video [Turntable rig](https://github.com/jondurrant/RPIPicoVideoTurntableMk1)

Four switches to operate preset colour and illumination levels.

Rotary encoder to set Brightness or manual set a HSV colour.

## Software

Raspberry PI Pico s/w based on Pico SDK in C++.

Key Libraries:

+ [Freertos](https://www.freertos.org/)
+ [PicoLED](https://github.com/ForsakenNGS/PicoLED)

## Techniques

 Based on approaches taught in my Udemy training courses:
 
 + [FreeRTOS on Raspberry PI PICO](https://www.udemy.com/course/freertos-on-rpi-pico/?referralCode=C5A9A19C93919A9DA294)
 + [Raspberry PI PICO Micro Projects (C++)](https://www.udemy.com/course/rpi-pico-microprojects-c1/?referralCode=2F48111FD8290C72D4C7)