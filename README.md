# Smart Weighing Scale

Smart Weighing Scale with low current consumption, and hence a high battery backup.

## Block Diagram

## Circuit

### Power Supply
![image]()
The nrf Based Weighing Scale operates on 3.3V, and is powered by a 3.7V Li-ion battery.
We use a p-MOSFET(IRLML6401) for reverse polarity protection, and a LDO(Low Drop Out) Regulator(TPS7A0233PDBV) to get a stable 3.3V output supply.

### Battery Charging
![image]()
We use a Li-ion/Li-polymer linear battery charger IC(BQ21040) for charging the battery via a micro-USB oulet. The led indicator stops glowing when the battery is fully charged.

### nRF Module
![image]()
We are using MDBT42Q nRF52840 based module as our main MCU and BLE module. It is a very low current consuming and high performance module.
It needs a 3.3V power supply, and we are using its internal DC/DC regulator which is set by connecting a 10uH and 18nH inductor at DCC and DEC4 pins as shown here. We also use an external 32.768 kHz external crystal. For programming this module we need a Segger JLink Debugger, which is available as a separate device or one could use an on-board debugger from any of the Nordic Dev-boards like PCA400, etc...
We kept a few GPIOs out for future use or if need to integrate anything else with the weighing scale.

![image]()
To measure the battery level we use this voltage divider circuit and measure it use the on board ADC pins.

![image]()
Tare Button is a push button for using the tare functionality. It basically resets the displayed weight on scale back to zero. The Tare button can also be used to measure multiple items in the same container. Simply add you first item, note the weight, press Tare and you are ready to weigh your next item.

### ADC Circuit: HX711
![image]()
For measuring the weighing scale we use a Load Cell which gives an anlog output, as the weight changes. For high accuracy we use HX711 24-bit ADC chip, which is designed specifically for industrial weighing scales and other industrial control applications.

![image]() ![image]()
To power the HX711 we need a 5V supply, so we use a Boost regulator(LM2703) to step-up the 3.7V from the battery supply and use an LDO(LP2985-5.0) to get a stable noise free 5V supply for HX711.

The data from the Load Cell is received via four pins HX711_IN+, HX711_IN-, HX711_AVDD and GND. This data is processed by the ADC chip and an output is send via serial interface to the nRF module. We can control the output data rate as either 10Hz or 80Hz by controlling the HX711_RATE pin state.
There is no programming needed for the internal registers. All controls to the HX711 are through the pins.

### E-Ink Display
![image]()
We decided to go with an E-Ink display as it is very low power consuming, reflection resistant, and was useful for the application we were targeting.
This is a 1.54" 200x200 pixel [E-Ink display](https://www.e-paper-display.com/products_detail/productId=513.html) from Good Display.