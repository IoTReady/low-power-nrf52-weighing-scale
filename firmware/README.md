Here one can find the main application with all library files(for e-ink display, and weighing scale) in the "Application" folder.
To setup the Nordic SDK and bootloader files follow the links in "Bootloader" folder. Also, the "documentation" folder has some good technical data.

To make changes in the firmware go to "weighing_scale_nrf/firmware/application/w_scale_all/" and make necessary changes.
To build the output files, go to the same location and use this command to generate final output files(.hex):
```
make release
```

Then use the following to flash and debug the firmware:
```
make upload
```

This is a custom makefile, so make sure all Debugger setings are proper. Like, make sure the Debugge name and Serial Number are correct.
These can be set in this [makefile](https://github.com/IoTReady/weighing_scale_nrf/blob/master/firmware/application/w_scale_all/Makefile).
