# HomeSpan Tutorials

The HomeSpan library includes 16 tutorial sketches of increasing complexity that take you through all the functions and features of HomeSpan.  The sketches are extensively annotated, and you'll even learn a lot about HomeKit itself by working through all the examples.  If you've already loaded HomeSpan into your Arduino IDE, the tutorials will be found under *File → Examples → HomeSpan*.  Each sketch is ready to be compiled and uploaded to your ESP32 device so you can see them in action.  Alternatively, you can explore just the code within GitHub by clicking on any of titles below.  Note: you may want to first read through the [HomeSpan API Overview](Overview.md) before exploring the tutorials.  They will probably make a lot more sense if you do!

> :heavy_check_mark: Each example is designed to be operated after pairing your ESP32 to HomeKit so you can control HomeSpan from the Home App on your iPhone, iPad, or Mac.  In principle, once you configure and pair your device to HomeKit, your Home App should automatically reflect all changes in your configuration whenever you upload a different tutorial.  However, in practice this is not always the case as it seems HomeKit sometimes caches information about devices, which means what you see in your Home App may not be fully in sync with your sketch.  If this occurs, unpairing and then re-pairing the ESP32 device usually fixes the issue.  If not, you may have to reset the ID on the ESP32 device so that HomeKit thinks it is a new device and will not use any cached data.  This is very easy to do - see the [HomeSpan Command-Line Interface (CLI)](CLI.md) page for details.

### [Example 1 - SimpleLightBulb](../examples/01-SimpleLightBulb)
This first example introduces the HomeSpan library and demonstrates how to implement a simple on/off light control using a combination of HomeSpan Accessory, Service, and Characteristic objects.  Once this sketch has been uploaded to your HomeSpan device and the device is paired to your home, a new "lightbulb" tile will appear in the Home App of your iPhone, iPad, or Mac. Though the tile will be fully operational (i.e. you can change the status of the lightbulb from "on" or "off"), we won't yet connect an actual light or LED to the HomeSpan device, so nothing real will light up.  Instead, in this and the next few examples, we'll focus on learning about the different ways HomeKit controls can be configured.  Starting in Example 5, we'll connect an LED to the device and introduce the methods that actually turn the LED on and off from your Home App.  HomeSpan API topics covered in this example include:

* the `homeSpan` global object, and it's `begin()` and `poll()` methods
* referencing HomeSpan categories defined in the `Categories::` namespace
* instantiating a new `SpanAccessory`
* instantiating HomeSpan Services and Characteristics defined in the `Service::` and `Characteristic::` namespaces

### [Example 2 - TwoSimpleLightBulbs](../examples/02-TwoSimpleLightBulbs)
Example 2 expands on Example 1 by implementing two LightBulbs, each as their own Accessory.

### [Example 3 - CeilingFanWithLight](../examples/03-CeilingFanWithLight)
Example 3 shows how adding multiple Services to a single Accessory allows us to create a multi-featured Accessory, such as a ceiling fan wih a ceiling light.

### [Example 4 - AdvancedCeilingFan](../examples/04-AdvancedCeilingFan)
Example 4 expands on Example 3 by adding Characteristics to set fan speed, fan rotation direction, and light brightness.  New HomeSpan API topics covered in this example include:

* using `setRange()` to set the allowable range and increment values for a Characteristic

### [Example 5 - WorkingLED](../examples/05-WorkingLED)
Example 5 expands on Example 2 by adding in the code needed to actually control LEDs connected to the ESP32 from HomeKit. In Example 2 we built out all the functionality to create a "Tile" Acessories inside HomeKit that displayed an on/off light, but these control did not actually operate anything on the ESP32.  To operate actual devices HomeSpan needs to be programmed to respond to "update" requests from HomeKit by performing some form of operation.  New HomeSpan API topics covered in this example include:

* creating derived device-specific Service structures (classes) from a base HomeSpan Service class
* placing derived Service classes in their own \*.h files for readability and portability
* implementing the virtual `update()` method for your derived Services
* saving references to Characteristic objects with a `SpanCharacteristic *` pointer
* retrieving new and updated Characteristic values with the `getVal()` and `getNewVal()` methods

### [Example 6 - DimmableLED](../examples/06-DimmableLED)
Example 6 changes Example 5 so that LED #2 is now dimmable, instead of just on/off.  New HomeSpan API topics covered in this example include:

* implementing pulse-width-modulation on any ESP32 pin by instantiating a `PwmPin()` object
* setting the PWM level to control the brightness of an LED using the PwmPin `set()` method
* storing similar derived Service classes in the same \*.h file for ease of use

### [Example 7 - IdentifyRoutines](../examples/07-IdentifyRoutines)
Example 7 uses the encapsulation techniques illustrated in Examples 5 and 6 to derive an easier-to-use Identify Service from HomeSpan's AccessoryInformation Service.  The example includes the implementation of an `update()` method that responds to HomeKit requests writing to the Identify Characteristic.  New HomeSpan API topics covered in this example include:

* storing dissimilar derived Service classes in the different \*.h files for better portability

### [Example 8 - Bridges](../examples/08-Bridges)
Example 8 is functionally identical to Example 7, except that instead of defining two Accessories (one for the on/off LED and one for the dimmable LED), we define three Accessories, where the first acts as a HomeKit Bridge.

### [Example 9 - MessageLogging](../examples/09-MessageLogging)
Example 9 illustrates how to add log messages to your HomeSpan sketch.  The code is identical to Example 8 except for the inclusion of new log messages.  New HomeSpan API topics covered in this example include:

* using the `LOG1()` and `LOG2()` macros to create log messages for different log levels
* setting the initial log level for a sketch with the `homeSpan.setLogLevel()` method

### [Example 10 - RGB_LED](../examples/10-RGB_LED)
Example 10 illustrates how to control an RGB LED to set any color and brightness.  New HomeSpan API topics covered in this example include:

* converting HomeKit Hue/Saturation/Brightness levels to Red/Green/Blue levels using `PwmPin::HSVtoRGB()`
* using the optional template functionality of `getVal()`, such as `getVal<float>()`

### [Example 11 - ServiceOptions](../examples/11-ServiceOptions)
This example explores how the Name Characteristic can be used to create different naming schemes for multi-Service Accessories, and how these appear in the Home App depending on how you display the Accessory tile.  New HomeSpan API topics covered in this example include:

* setting the primary Service for an Accessory with the `setPrimary()` method

### [Example 12 - ServiceLoops](../examples/12-ServiceLoops)
Example 12 introduces HomeKit *Event Notifications* to implement two new accessories - a Temperature Sensor and an Air Quality Sensor.  Of course we won't actually have these physical devices attached to the ESP32 for the purpose of this example, but we will simulate "reading" their properties on a periodic basis, and notify HomeKit of any changed values.  New HomeSpan API topics covered in this example include:

* implementing the virtual `loop()` method in a derived Service
* keeping track of elapsed time since the last update of a Characteristic with the `timeVal()` method
* setting the value of a Characteristic and triggering an Event Notification with the `setVal()` method

### [Example 13 - TargetStates](../examples/13-TargetStates)
Example 13 we demonstrate the simultaneous use of both the `update()` and `loop()` methods by implementing two new Services: a Garage Door Opener and a motorized Window Shade.  Both examples showcase HomeKit's Target-State/Current-State framework.

### [Example 14 - EmulatedPushButtons](../examples/14-EmulatedPushButtons)
Example 14 demonstrates how you can use the `setVal()` and `timeVal()` methods inside a Service's `loop()` method to create a tile in the Home App that emulates a pushbutton switch.  In this example pressing the tile in the Home App will cause it to turn on, blink an LED 3 times, and then turn off (just like a real pushbutton might do).

### [Example 15 - RealPushButtons](../examples/15-RealPushButtons)
This example introduces HomeSpan functionality that lets you easily connect real pushbuttons to any pin on your ESP32 device.  These pushbuttons can then be used to  manually control any appliance connected to the device, such as a lamp or fan.  In this example we implement 3 pushbuttons to control the power, brightness, and a "favorites" setting of an LED, using a combination of single, double, and long button presses.  Event Notifications are sent back to HomeKit using the `setVal()` method after each pushbutton press so that the Home App tiles immediately reflect your manual changes to the power and brightness of the LED.  New HomeSpan API topics covered in this example include:

* creating pushbutton objects on any ESP32 pin with `SpanButton()`
* implementing the virtual `button()` method in a derived Service
* parsing SINGLE, DOUBLE, and LONG button presses

### [Example 16 - ProgrammableSwitches](../examples/16-ProgrammableSwitches)
Example 16 does not introduce any new HomeSpan functionality, but instead showcases a unique feature of HomeKit that you can readily access with HomeSpan.  In all prior examples we used the ESP32 to control a local appliance - something connected directly to the ESP32 device.  We've then seen how you can control the device via HomeKit's iOS or MacOS Home App, or by the addition of local pushbuttons connected directly to the ESP32 device. In this example we do the opposite, and use pushbuttons connected to the ESP32 to control OTHER HomeKit devices of any type.  To do so, we use HomeKit's Stateless Programmable Switch Service.

### [Example 17 - LinkedServices](../examples/17-LinkedServices)
Example 17 introduces the HAP concept of Linked Services and demonstrates how they are used through the implementation of a multi-head Shower.  This example also illustrates some different coding styles that showcase the power and flexibility of HomeSpan's C++ *structure-based* design paradigm.  New HomeSpan API topics covered in this example include:

* creating Linked Services using the `addLink()` method

---

[↩️](README.md) Back to the Welcome page
