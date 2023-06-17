#include "WoodBlock.h"

class WebPortal: public WoodBlock
{
  // TODO:
};

class Relay: public WoodBlock
{
  // TODO:
};

class OccupySensor: public WoodBlock
{
  // TODO:
};

Relay relay;
OccupySensor occupySensor;
WebPortal webPortal;

void setup() {
  // put your setup code here, to run once:

  // TODO: INIT!
  {
    const char * outVariables[] = {"occupied"};
    const char * inVariables[] = {"occupied"};
    CONNECT(occupySensor, "status", outVariables, sizeof(outVariables)/sizeof(outVariables(0)),
            webPortal, "occupyStatus", inVariables, sizeof(inVariables)/sizeof(inVariables(0)));
  }

  {
    const char * outVariables[] = {"relayOnOff"};
    const char * inVariables[] = {"onOff"};
    CONNECT(webPortal, "relayControl", outVariables, sizeof(outVariables)/sizeof(outVariables(0)),
            webPortal, "control", inVariables, sizeof(inVariables)/sizeof(inVariables(0)));
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  occupySensor.fetchExternalEvents();
  webPortal.fetchExternalEvents();
}
