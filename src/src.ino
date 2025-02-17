
// This is a placeholder .ino file that allows you to easily edit the contents of this library using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.

#include "HomeSpan.h"

void setup() {
 
  Serial.begin(115200);
 
  homeSpan.setLogLevel(1);
  
  homeSpan.setHostNameSuffix("-lamp1");
  homeSpan.setPortNum(1201);
//  homeSpan.setMaxConnections(6);
//  homeSpan.setQRID("One1");
//  homeSpan.enableOTA();
  homeSpan.setSketchVersion("Test 1.3.1");
  homeSpan.setWifiCallback(wifiEstablished);

  homeSpan.begin(Category::Lighting,"HomeSpan Lamp Server","homespan");

  new SpanAccessory();                                  // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments

    new Service::AccessoryInformation();                    // HAP requires every Accessory to implement an AccessoryInformation Service, which has 6 required Characteristics
      new Characteristic::Name("HomeSpan Test");                // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories                                                            
      new Characteristic::Manufacturer("HomeSpan");             // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("HSL-123");              // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("HSL Test");                    // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision(HOMESPAN_VERSION);   // Firmware of the Accessory (arbitrary text string, and can be the same for every Accessory)  
      new Characteristic::Identify();                           // Create the required Identify
  
//    new Service::HAPProtocolInformation();                  // Create the HAP Protcol Information Service  
      new Characteristic::Version("1.1.0");                     // Set the Version Characteristic to "1.1.0" as required by HAP

    new Service::LightBulb();
//      new Characteristic::On();
      new Characteristic::Brightness();
      new Characteristic::Name("Light 1");
    new Service::LightBulb();
      new Characteristic::On(2);
      (new Characteristic::Brightness(150))->setRange(0,140,5);
      new Characteristic::Name("Light 2");
    (new Service::Switch())->setPrimary();
      new Characteristic::On();
      new Characteristic::Name("Switch 3");
      new SpanButton(17);

} // end of setup()

//////////////////////////////////////

void loop(){

  homeSpan.poll();

} // end of loop()

//////////////////////////////////////

void wifiEstablished(){
  Serial.print("IN CALLBACK FUNCTION\n\n");
}
