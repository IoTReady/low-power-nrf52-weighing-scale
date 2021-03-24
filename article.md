# Building a low power, smart weighing scale

### How did we get into this?

Online grocery shopping is booming. We were approached by a new startup to help them build a tracking infrastructure. A key componet of this infrastructre was a weighing station that could be combined with a bar-code printer and a reader to automatically identify, weigh and update information on Stock Keeping Units (SKU's) as they flowed through the grocery processing flow.

Conventional scales have been manufactured for a long time. Its also possible to fairly easily integrate them with scanners and printers. However in this case more was needed:
 
* Identify everything that's weighed on them and be able to update information on the item
* Integrate the item information into the startups workflow management system
* Enable the placement of these devices where needed by the layout of processing stations, and the ability to move them around as required
* Ensure that these devices were capable of handling power interruptions

Keeping all these features in mind we came up with two variants: Weighing Scales with WiFi or BLE.
After further research, development, feild testing and lots of discussions we decided to go ahead with the BLE alternative. It is much more power efficient, cheaper and met all the requirements we were looking for.

### Hardware design and development

For low power, Bluetooth Low Energy (BLE), there is no better option in the market other than Nordic Semiconductor's nRF series. So, based on requirements, availability and price we went with nRF52840 based module "MDBT42Q". One can find all the design files and a more technical documentation in our GitHub repo [here](https://github.com/IoTReady/weighing_scale_nrf).

Basically a weighing scale has a load cell which helps determine the weight of an object put on it. The load-cell signal uses an op-amp circuit to get a improve the signal and an ADC (Analog-to-Digital Converter) to calculate the final weight of the object. We used the ADC on the nRF52840 chip and for the Amplifier circuit we tried three different options:

* LPV821
* LTC6915
* HX711

After further testing and comparison of all three we decided to go with "HX711" circuit. It gave us the most stable results and power consumption was also the least expensive compared to the rest. We are sharing all three designs in our open-source repo, but we only provide more technical data on HX711 circuit for now.

The next requirement was for a display showing weight, battery life and charging status of the device. To keep it low power we tried out E-Ink displays. They met all our expectations on how much less current they draw compared to conventional LED displays. We settled on a 1.54 inch 200x200 pixel [E-Ink Display](https://www.e-paper-display.com/products_detail/productId=513.html) from Good Display.

Here is some data on the power consumption of the device:

Powering the device is simple. Users can install any 3.7V Li-Ion battery, as per their requirement. It can be charged via the micro-USB port on the device.

Finally, we have a Tare Button. It is a push button user can press to define their own zero reference and measure anything with respect to that.

### Connecting to the Internet

As we decided to go with BLE, there was no way for the device to directly connect to the internet. Hence, we developed a small mobile App which functions as a background gateway between the weighing scale and the factory intranet. A user will link to the device with the mobile App once. The device will keep sending all the weight data measured on it over BLE in addition to recording it, which will then be sent to the internet(company database or inventory tracking application) from the smart phone.

This way we record all the data with minimal power consumption by the device.
