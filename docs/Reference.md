# HomeSpan Library Reference

The HomeSpan Library is invoked by including *HomeSpan.h* in your Arduino sketch as follows:

```C++
#include "HomeSpan.h"
```

## *homeSpan*

At runtime this HomeSpan will create a global **object** named `homeSpan` that supports the following methods:

* `void begin(Category catID, const char *displayName, const char *hostNameBase, const char *modelName)` 
  * initializes HomeSpan
  * **must** be called at the beginning of each sketch before any other HomeSpan functions and is typically placed near the top of the Arduino `setup()` method, but **after** `Serial.begin()` so that initialization diagnostics can be output to the Serial Monitor
  * all arguments are **optional**
    * *catID* - the HAP Category HomeSpan broadcasts for pairing to HomeKit.  Default is Category::Lighting.  See [HomeSpan Accessory Categories](Categories.md) for a complete list
    * *displayName* - the MDNS display name broadcast by HomeSpan.  Default is "HomeSpan Server"
    * *hostNameBase* - the full MDNS host name is broadcast by HomeSpan as *hostNameBase-DeviceID*.local, where DeviceID is a unique 6-byte code generated automatically by HomeSpan.  Default is "HomeSpan"
    * *modelName* - the HAP model name HomeSpan broadcasts for pairing to HomeKit.  Default is "HomeSpan-ESP32"
  * example: `homeSpan.begin(Category::Fans, "Living Room Ceiling Fan");`
 
 * `void poll()`
   * checks for HAP requests, local commands, and device activity
   * **must** be called repeatedly in each sketch and is typically placed at the top of the Arduino `loop()` method
   
The following **optional** `homeSpan` methods override various HomeSpan initialization parameters used in `begin()`, and therefore **should** be called before `begin()` to take effect.  If a method is *not* called, HomeSpan uses the default parameter indicated below:

* `void setControlPin(uint8_t pin)`
  * sets the ESP32 pin to use for the HomeSpan Control Button (default=21)
  
* `void setStatusPin(uint8_t pin)`
  * sets the ESP32 pin to use for the HomeSpan Status LED (default=13).  There is also a corresponding `getStatusPin()` method that returns this pin number

* `void setApSSID(const char *ssid)`
  * sets the SSID (network name) of the HomeSpan Setup Access Point (default="HomeSpan-Setup")
  
* `void setApPassword(const char *pwd)`
  * sets the password of the HomeSpan Setup Access Point (default="homespan")
  
* `void setApTimeout(uint16_t nSec)`
  * sets the duration (in seconds) that the HomeSpan Setup Access Point, once activated, stays alive before timing out (default=300 seconds)
  
* `void setCommandTimeout(uint16_t nSec)`
  * sets the duration (in seconds) that the HomeSpan End-User Command Mode, once activated, stays alive before timing out (default=120 seconds)
  
* `void setLogLevel(uint8_t level)`
  * sets the logging level for diagnostic messages, where:
    * 0 = top-level status messages only (default),
    * 1 = all status messages, and
    * 2 = all status messages plus all HAP communication packets to and from the HomeSpan device
  * this parameter can also be changed at runtime via the [HomeSpan CLI](CLI.md)
  
* `void setMaxConnections(uint8_t nCon)`
  * sets the desired maximum number of HAP Controllers that can be simultaneously connected to HomeSpan (default=8)
  * due to limitations of the ESP32 Arduino library, HomeSpan will override *nCon* if it exceed the following internal limits:
    * if OTA is not enabled, *nCon* will be reduced to 8 if it has been set to a value greater than 8
    * if OTA is enabled, *nCon* will be reduced to 7 if it has been set to a value greater than 7
  * if you add code to a sketch that uses it own network resources, you will need to determine how many TCP sockets your code may need to use, and use this method to reduce the maximum number of connections available to HomeSpan accordingly
  
* `void setPortNum(uint16_t port)`
  * sets the TCP port number used for communication between HomeKit and HomeSpan (default=80)
  
* `void setHostNameSuffix(const char *suffix)`
  * sets the suffix HomeSpan appends to *hostNameBase* to create the full hostName
  * if not specified, the default is for HomeSpan to append a dash "-" followed the 6-byte Accessory ID of the HomeSpan device
  * setting *suffix* to a null string "" is permitted
  * example: `homeSpan.begin(Category::Fans, "Living Room Ceiling Fan", "LivingRoomFan");` will yield a default *hostName* of the form *LivingRoomFan-A1B2C3D4E5F6.local*.  Calling `homeSpan.setHostNameSuffix("v2")` prior to `homeSpan.begin()` will instead yield a *hostName* of *LivingRoomFanv2.local*
  
* `void setQRID(const char *id)`
  * changes the Setup ID, which is used for pairing a device with a [QR Code](QRCodes.md), from the HomeSpan default to *id*
  * the HomeSpan default is "HSPN" unless permanently changed for the device via the [HomeSpan CLI](CLI.md) using the 'Q' command
  * *id* must be exactly 4 alphanumeric characters (0-9, A-Z, and a-z).  If not, the request to change the Setup ID is silently ignored and the default is used instead
  
* `void enableOTA(boolean auth=true)`
  * enables [Over-the-Air (OTA) Updating](OTA.md) of a HomeSpan device, which is otherwise disabled
  * HomeSpan OTA requires an authorizing password unless *auth* is specified and set to *false*
  * the default OTA password for new HomeSpan devices is "homespan-ota"
  * this can be changed via the [HomeSpan CLI](CLI.md) using the 'O' command
  
* `void setSketchVersion(const char *sVer)`
  * sets the version of a HomeSpan sketch to *sVer*, which can be any arbitrary character string
  * if unspecified, HomeSpan uses "n/a" as the default version text
  * HomeSpan displays the version of the sketch in the Arduino IDE Serial Monitor upon start-up
  * HomeSpan also includes both the version of the sketch, as well as the version of the HomeSpan library used to compile the sketch, as part of its HAP MDNS broadcast.  This data is *not* used by HAP.  Rather, it is for informational purposes and allows you to identify the version of a sketch for a device that is updated via [OTA](OTA.md), rather than connected to a computer
  
* `const char *getSketchVersion()`
  * returns the version of a HomeSpan sketch, as set using `void setSketchVersion(const char *sVer)`, or "n/a" if not set
  
* `void setWifiCallback(void (*func)(void))`
  * Sets an optional user-defined callback function, *func*, to be called by HomeSpan upon start-up just after WiFi connectivity has been established.  This one-time call to *func* is provided for users that are implementing other network-related services as part of their sketch, but that cannot be started until WiFi connectivity is established.  The function *func* must be of type *void* and have no arguments
  
## *SpanAccessory(uint32_t aid)*

Creating an instance of this **class** adds a new HAP Accessory to the HomeSpan HAP Database.

  * every HomeSpan sketch requires at least one Accessory
  * a sketch can contain a maximum of 41 Accessories per sketch (if exceeded, a runtime error will the thrown and the sketch will halt)
  * there are no associated methods
  * the argument *aid* is optional.
  
    * if specified and *not* zero, the Accessory ID is set to *aid*.
    * if unspecified, or equal to zero, the Accessory ID will be set to one more than the ID of the previously-instantiated Accessory, or to 1 if this is the first Accessory.
    * the first Accessory instantiated must always have an ID=1 (which is the default if *aid* is unspecified).
    * setting the *aid* of the first Accessory to anything but 1 throws an error during initialization.
    
  * you must call `homeSpan.begin()` before instantiating any Accessories
  * example: `new SpanAccessory();`
  
## *SpanService()*

This is a **base class** from which all HomeSpan Services are derived, and should **not** be directly instantiated.  Rather, to create a new Service instantiate one of the HomeSpan Services defined in the [Service](ServiceList.md) namespace.  No arguments are needed.

* instantiated Services are added to the HomeSpan HAP Database and associated with the last Accessory instantiated
* instantiating a Service without first instantiating an Accessory throws an error during initialization
* example: `new Service::MotionSensor();`

The following methods are supported:

* `SpanService *setPrimary()`
  * specifies that this is the primary Service for the Accessory.  Returns a pointer to the Service itself so that the method can be chained during instantiation. 
  * example: `(new Service::Fan)->setPrimary();`
  
* `SpanService *setHidden()`
  * specifies that this is hidden Service for the Accessory.  Returns a pointer to the Service itself so that the method can be chained during instantiation.
  * note this does not seem to have any affect on the Home App.  Services marked as hidden still appear as normal
  
* `SpanService *addLink(SpanService *svc)`
  * adds *svc* as a Linked Service.  Returns a pointer to the calling Service itself so that the method can be chained during instantiation.
  * note that Linked Services are only applicable for select HAP Services.  See Apple's [HAP-R2](https://developer.apple.com/support/homekit-accessory-protocol/) documentation for full details.
  * example: `(new Service::Faucet)->addLink(new Service::Valve)->addLink(new Service::Valve);` (links two Valves to a Faucet)
  
* `virtual boolean update()`
  * HomeSpan calls this method upon receiving a request from a HomeKit Controller to update one or more Characteristics associated with the Service.  Users should override this method with code that implements that requested updates using one or more of the SpanCharacteristic methods below.  Method **must** return *true* if update succeeds, or *false* if not.
  
* `virtual void loop()`
  * HomeSpan calls this method every time `homeSpan.poll()` is executed.  Users should override this method with code that monitors for state changes in Characteristics that require HomeKit Controllers to be notified using one or more of the SpanCharacteristic methods below.
  
* `virtual void button(int pin, int pressType)`
  * HomeSpan calls this method whenever a SpanButton() object associated with the Service is triggered.  Users should override this method with code that implements any actions to be taken in response to the SpanButton() trigger using one or more of the SpanCharacteristic methods below.
    * *pin* - the ESP32 pin associated with the SpanButton() object
    * *pressType* - 
      * 0=single press (SpanButton::SINGLE)
      * 1=double press (SpanButton::DOUBLE)
      * 2=long press (SpanButton::LONG)
    
## *SpanCharacteristic(value)*
  
This is a **base class** from which all HomeSpan Characteristics are derived, and should **not** be directly instantiated.  Rather, to create a new Characteristic instantiate one of the HomeSpan Characteristics defined in the [Characteristic](ServiceList.md) namespace.

* instantiated Characteristics are added to the HomeSpan HAP Database and associated with the last Service instantiated
* instantiating a Characteristic without first instantiating a Service throws an error during initialization
* a single, optional argument is used to set the initial value of the Characteristic at startup
* throws a runtime warning if value is outside of the min/max range for the Characteristic, where min/max is either the HAP default, or any new values set via a call to `setRange()`
* example: `new Characteristic::Brightness(50);`

The following methods are supported:

* `type T getVal<T>()`
  * a template method that returns the **current** value of the Characteristic, after casting into the type *T* specified (e.g. *int*, *double*, etc.).  If template parameter is excluded, value will be cast to *int*.
  * example with template specified: `double temp = Characteristic::CurrentTemperature->getVal<double>();`
  * example with template excluded : `int tilt = Characteristic::CurrentTiltAngle->getVal();`

* `type T getNewVal<T>()`
  * a template method that returns the desired **new** value to which a HomeKit Controller has requested to the Characteristic be updated.  Same casting rules as for `getVal<>()`
  
* `boolean updated()`
  * returns *true* if a HomeKit Controller has requested an update to the value of the Characteristic, otherwise *false*.  The requested value itself can retrieved with `getNewVal<>()`
  
* `void setVal(value)`
  * sets the value of the Characteristic to *value*, and notifies all HomeKit Controllers of the change
  * works with any integer, boolean, or floating-based numerical *value*, though HomeSpan will convert *value* into the appropriate type for each Characteristic (e.g. calling `setValue(5.5)` on an integer-based Characteristic results in *value*=5)
  * throws a runtime warning if *value* is outside of the min/max range for the Characteristic, where min/max is either the HAP default, or any new min/max range set via a prior call to `setRange()`
  * *value* is **not** restricted to being an increment of the step size; for example it is perfectly valid to call `setVal(43.5)` after calling `setRange(0,100,5)` on a floating-based Characteristic even though 43.5 does does not align with the step size specified.  The Home App will properly retain the value as 43.5, though it will round to the nearest step size increment (in this case 45) when used in a slider graphic (such as setting the temperature of a thermostat)
  
* `int timeVal()`
  * returns time elapsed (in millis) since value of the Characteristic was last updated (whether by `setVal()` or as the result of a successful update request from a HomeKit Controller)

* `SpanCharacteristic *setRange(min, max, step)`
  * overrides the default HAP range for a Characteristic with the *min*, *max*, and *step* parameters specified
  * *step* is optional; if unspecified (or set to a non-positive number), the default HAP step size remains unchanged
  * works with any integer or floating-based parameters, though HomeSpan will recast the parameters into the appropriate type for each Characteristic (e.g. calling `setRange(50.5,70.3,0.5)` on an integer-based Characteristic results in *min*=50, *max*=70, and *step*=0)
  * an error is thrown if:
    * called on a Characteristic that does not suport range changes, or
    * called more than once on the same Characteristic
  * returns a pointer to the Characteristic itself so that the method can be chained during instantiation
  * example: `(new Characteristic::Brightness(50))->setRange(10,100,5);`
  
## *SpanButton(int pin, uint16_t longTime, uint16_t singleTime, uint16_t doubleTime)*

Creating an instance of this **class** attaches a pushbutton handler to the ESP32 *pin* specified.

* instantiated Buttons are associated with the last Service instantiated
* instantiating a Button without first instantiating a Service throws an error during initialization

The first argument is required; the rest are optional:
* *pin* - the ESP32 pin to which a one pole of a normally-open pushbutton will be connected; the other pole is connected to ground
* *longTime* - the minimum time (in millis) required for the button to be pressed and held to trigger a Long Press (default=2000 ms)
* *singleTime* - the minimum time (in millis) required for the button to be pressed to trigger a Single Press (default=5 ms)
* *doubleTime* -  the maximum time (in millis) allowed between two Single Presses to qualify as a Double Press (default=200 ms)
  
Trigger Rules:
* If button is pressed and continuously held, a Long Press will be triggered every longTime ms until the button is released
* If button is pressed for more than singleTime ms but less than longTime ms and then released, a Single Press will be triggered, UNLESS the button is pressed a second time within doubleTime ms AND held again for at least singleTime ms, in which case a DoublePress will be triggered;  no further events will occur until the button is released
* If singleTime>longTime, only Long Press triggers can occur
* If doubleTime=0, Double Presses cannot occur
  
HomeSpan automatically calls the `button(int pin, int pressType)` method of a Service upon a trigger event in any Button associated with that Service, where *pin* is the ESP32 pin to which the pushbutton is connected, and *pressType* is an integer that can also be represented by the enum constants indicated:
  * 0=single press (SpanButton::SINGLE)
  * 1=double press (SpanButton::DOUBLE)
  * 2=long press (SpanButton::LONG)  
  
HomeSpan will report a warning, but not an error, during initialization if the user had not overridden the virtual button() method for a Service contaning one or more Buttons; triggers of those Buttons will simply ignored.

## *#define REQUIRED VERSION(major,minor,patch)*

If REQUIRED is defined in the main sketch prior to including the HomeSpan library with `#include "HomeSpan.h"`, HomeSpan will throw a compile-time error unless the version of the library included is equal to, or later than, the version specified using the VERSION macro.  Example:

```C++
#define REQUIRED VERISON(2,1,3)   // throws a compile-time error unless HomeSpan library used is version 2.1.3 or later
```
---

## *SpanRange(int min, int max, int step)*

Creating an instance of this **class** overrides the default HAP range for a Characteristic with the *min*, *max*, and *step* values specified.

* instantiated Ranges are added to the HomeSpan HAP Database and associated with the last Characteristic instantiated
* instantiating a Range without first instantiating a Characteristic throws an error during initialization
* example: `new Characteristic::Brightness(50); new SpanRange(10,100,5);`
* this is a legacy function that is limited to integer-based parameters, and has been re-coded to simply call the more generic `setRange(min, max, step)` method
* **please use** `setRange(min, max, step)` **for all new sketches**

---

[↩️](README.md) Back to the Welcome page
