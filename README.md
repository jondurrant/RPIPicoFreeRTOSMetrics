# RPIPicoFreeRTOSMetrics
Metric Analysis of FreeRTOS behaviour on a Web Dashboard

Add a [Mongoose](https://mongoose.ws/) Task to a [FreeRTOS Kernel](https://www.freertos.org/) application and then be able to see running tasks, stack and heap usage in near time on a Web Console using [Google Charts](https://developers.google.com/chart).


## Dashboard
The Dashboard is simple "index.html" in the Dashboard folder. Note as I load this from filesystem on host computer you will need to grant "Access-Control-Allow-Origin *" to the page. I've used the Google Extension [Allow CORS: Access-Control-Allow-Origin](https://chromewebstore.google.com/detail/allow-cors-access-control/lhobafahddgcelffkeicbaginigeejlf?hl=en) to do this.

## Example Firmware
An example FreeRTOS KErnel application is in the folder examples.

The stats Web Service framework is provided through the class WebStats.h/.cpp

To build you will need to have the following Environment Variables defined:
+ PICO_SDK_PATH point to your SDK Install location
+ PICO_FREERTOS point to a copy of FreeRTOS Kernenl
+ WIFI_SSID - The Wifi to join to
+ WIFI_PASSWORD - Wifi password

The Mongoose library has been included as a submodule. Please clone with the "--recurse-submodules" switch.

Build using the normal Pico SDK framework build process:
```
cd example
mkdir build
cd build
cmake ..
make # or ninja on Windows
```



