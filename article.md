# Building a smart weighing scale

### How did we get into this?
Looking at the increasing demand of online grocery shopping, we decided to get into that market and make our contribution. The people in this industry are used to the conventional tools they've been using for last few decades. So, we introduced them to the idea of smarter, less power consuming tools which can also connect to the internet and help make their lives a bit easier. 
After interacting with these online delivery companies we got a better picture of what they need and what we can provide.So, came the idea of smart weighing scales. They required these machines to do the following:
	
* keep track of everything that's measured on them.
* help maintain a better inventory.
* make them more remote.
* more power efficient / longer battery life.

Keeping all these features in mind we came up with two variants: Weighing Scales with WiFi and BLE.
After further research, development, feild testing and lots of discussions we decided to go ahead with the BLE alternative. It is much more power efficient, cheaper and met all the requirements we were looking for.

### Hardware design and development.
For low power BLE, there is no better option in the market other than Nordic Semiconductor's nRF series. So, based on requirements, availability and price we went with nRF52840 based module "MDBT42Q". One can find all the design files and a more technical documentation in our GitHub repo [here](https://github.com/IoTReady/weighing_scale_nrf).

So, basically a weighing scale has a load cell which helps determine the weight of an object put on it. Then, we use an Op-Amp circuit to get a better signal of the same and using an ADC (Analog-to-Digital Converter) we calculate the final weight of the object. We used the ADC on the nRF52840 chip and for the Amplifier circuit we tried three different options:

 * LPV821
 * LTC6915
 * HX711

And, after further testing and comparison of all three we decided to go with "HX711" circuit. It gave us the most stable results and power consumption was also the least compared to the rest. We are sharing all three designs in our Open-source repo, but we only provide more technically data on HX711 circuit for now.

Next requirement was for a display showing weight, battery life and charging status of the device. So, to keep it low power we tried out E-Ink displays. And we were amazed on how much less current they draw compared to the conventional LED displays. So we went with a 1.54 inch 200x200 pixel [E-Ink Display](https://www.e-paper-display.com/products_detail/productId=513.html) from Good Display.

Here is some data on the power consumption of the device:

User can install any 3.7V Li-Ion battery, as per their requirement. It can be charged via the micro-USB port on the device.

Finally, we have a Tare Button. It is a push button user can press to define their own zero reference and measure anything with respect to that.

### Connecting to the Internet.
As we decided to go with BLE, there was no way for the device to directly connect to the internet. Hence, we had to develop a mobile App which wil be used by the person taking care of all the weighing of things and packaging them. User will have to only connect the device to the mobile App once, and then just go on with their regular process. The device will keep sending all the weight data measured on it over BLE and keep recording it, which will then be sent to the internet(company database or inventory tracking application) from the smart phone.

This way we record all the data with minimal power consumption by the device.