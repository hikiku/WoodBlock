// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#include <WoodBlock.h>

#include <ExampleCommon/OccupySensorFBType.h>
#include <ExampleCommon/RelayFBType.h>
#include <ExampleCommon/WebPortalFBType.h>

// For user extend data type
extern bool extend_check4ConnectDataType(unsigned int outDataType,
                                         unsigned int inDataType) {
  return false;
}

FBNetwork fbNetwork;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  fbNetwork.attachFBInstance(FBInstance::create<RelayFBType>("Relay"));
  fbNetwork.attachFBInstance(
      FBInstance::create<OccupySensorFBType>("OccupySensor"));
  fbNetwork.attachFBInstance(FBInstance::create<WebPortalFBType>(
      "WebPortal", /*hasEventInput*/ true, /*hasEventOutput*/ true));
  {
    const char* outVariableNames[] = {OccupySensorFBType::OV_STATUS};
    const char* inVariableNames[] = {WebPortalFBType::IV_STATUS};
    fbNetwork.connect("OccupySensor", OccupySensorFBType::OE_OCCUPY,
                      outVariableNames, ARRAY_SIZE(outVariableNames),
                      "WebPortal", WebPortalFBType::IE_OCCUPY, inVariableNames,
                      ARRAY_SIZE(inVariableNames));
  }
  {
    const char* outVariableNames[] = {WebPortalFBType::OV_ONOFF};
    const char* inVariableNames[] = {RelayFBType::IV_ONOFF};
    fbNetwork.connect("WebPortal", WebPortalFBType::OE_CONTROL,
                      outVariableNames, ARRAY_SIZE(outVariableNames), "Relay",
                      RelayFBType::IE_CONTROL, inVariableNames,
                      ARRAY_SIZE(inVariableNames));
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  fbNetwork.fetchExternalEvents();
  delay(1000);
}
