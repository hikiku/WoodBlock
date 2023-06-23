// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#include <WoodBlock.h>

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

// For user extend data type
extern bool extend_check4ConnectDataType(unsigned int outDataType, unsigned int inDataType)
{
  return false;
}

class WebPortal : public WoodServiceInterfaceBlock
{
public:
  WebPortal(const char *name)
      : WoodServiceInterfaceBlock(name),
        ivStatus(nullptr), ieOccupy(nullptr),
        ovOnOff(nullptr), oeControl(nullptr), onOff(nullptr)
  {
    ivStatus = addInVariable<WoodBoolDataBox>("Status");
    {
      const char *outVariableNames[] = {"Status"};
      ieOccupy = addInEvent("Occupy", outVariableNames, ARRAY_SIZE(outVariableNames));
    }

    ovOnOff = addOutVariable<WoodBoolDataBox>("OnOff");
    {
      const char *outVariableNames[] = {"OnOff"};
      oeControl = addOutEvent("Control", outVariableNames, ARRAY_SIZE(outVariableNames));
    }
  }
  ~WebPortal() {}

  void executeInEvent(WoodInEvent &inEvent)
  {
    if (inEvent.getName().equals("Occupy"))
    {
      if (ivStatus)
      {
        BOOL *status = ivStatus->getDataBox().getData();
        if (status)
        {
          Serial.printf("%s \tProcess: \tEVENT_INPUT \t%s \t\tWITH \tStatus \t(* %s, \tline:%d *) \n",
                        getName().c_str(), inEvent.getName().c_str(), (*status) ? "true" : "false", __LINE__);
        }
      }
    }
    else
    {
      Serial.printf("TODO: Don't deal event(%s), line:%d !!!!!!!!\n", inEvent.getName().c_str(), __LINE__);
    }
  }
  bool captureAndExecuteServiceInterfaceInEvent()
  {
    static long unsigned int lasttime = 0; // time stamp, millisecond  //Fixed bug: std::time(0) is changed with SNTP response!
    long unsigned int time = millis();     //(uint32_t)std::time(0); //Fixed bug: std::time(0) is changed with SNTP response!

    if (lasttime == 0)
    {
      lasttime = time + 5 * 1000;
    }

    if (time - lasttime > 10 * 1000)
    {
      if (ovOnOff)
      {
        onOff = !onOff;
        ovOnOff->getDataBox().setData(onOff);
      }
      if (oeControl)
      {
        Serial.printf("%s \tGenerate: \tEVENT_OUTPUT \t%s \tWITH \tOnOff \t(* %s, \tline:%d *)\n",
                      getName().c_str(), oeControl->getName().c_str(), onOff ? "true" : "false", __LINE__);
        generateOutEvent(*oeControl);
      }
      lasttime = time;
      return true;
    }
    return false;
  }

private:
  WoodInDataImpl<WoodBoolDataBox> *ivStatus;
  WoodInEvent *ieOccupy;

  WoodOutDataImpl<WoodBoolDataBox> *ovOnOff;
  WoodOutEvent *oeControl;
  bool onOff;
};

class OccupySensor : public WoodServiceInterfaceBlock
{
public:
  OccupySensor(const char *name)
      : WoodServiceInterfaceBlock(name), ovStatus(nullptr), oeOccupy(nullptr), status(false)
  {
    ovStatus = addOutVariable<WoodBoolDataBox>("Status");
    {
      const char *outVariableNames[] = {"Status"};
      oeOccupy = addOutEvent("Occupy", outVariableNames, ARRAY_SIZE(outVariableNames));
    }
  }
  ~OccupySensor() {}

  void executeInEvent(WoodInEvent &inEvent)
  {
    Serial.printf("TODO: Don't deal event(%s), line:%d\n", inEvent.getName().c_str(), __LINE__);
  }
  bool captureAndExecuteServiceInterfaceInEvent()
  {
    static long unsigned int lasttime = 0; // time stamp, millisecond  //Fixed bug: std::time(0) is changed with SNTP response!
    long unsigned int time = millis();     //(uint32_t)std::time(0); //Fixed bug: std::time(0) is changed with SNTP response!

    if (lasttime == 0)
    {
      lasttime = time;
    }

    if (time - lasttime > 10 * 1000)
    {
      if (ovStatus)
      {
        status = !status;
        ovStatus->getDataBox().setData(status);
      }
      if (oeOccupy)
      {
        Serial.printf("%s \tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \tStatus \t(* %s, \tline:%d *)\n",
                      getName().c_str(), oeOccupy->getName().c_str(), status ? "true" : "false", __LINE__);
        generateOutEvent(*oeOccupy);
      }
      lasttime = time;
      return true;
    }
    return false;
  }

private:
  WoodOutDataImpl<WoodBoolDataBox> *ovStatus;
  WoodOutEvent *oeOccupy;
  bool status;
};

class Relay : public WoodBlock
{
public:
  Relay(const char *name) : WoodBlock(name)
  {
    ivOnOff = addInVariable<WoodBoolDataBox>("OnOff");
    {
      const char *inVariableNames[] = {"OnOff"};
      ieControl = addInEvent("Control", inVariableNames, ARRAY_SIZE(inVariableNames));
    }
  }
  ~Relay() {}

  void executeInEvent(WoodInEvent &inEvent)
  {
    if (inEvent.getName().equals("Control"))
    {
      if (ivOnOff)
      {
        BOOL *onOff = ivOnOff->getDataBox().getData();
        if (onOff)
        {
          Serial.printf("%s \t\tProcess: \tEVENT_INPUT \t%s \tWITH \tOnOff \t(* %s, \tline:%d *) \n",
                        getName().c_str(), inEvent.getName().c_str(), (*onOff) ? "true" : "false", __LINE__);
        }
      }
    }
    else
    {
      Serial.printf("TODO: Don't deal event(%s), line:%d !!!!!!!!\n", inEvent.getName().c_str(), __LINE__);
    }
  }

private:
  WoodInDataImpl<WoodBoolDataBox> *ivOnOff;
  WoodInEvent *ieControl;
};

Relay relay("Relay");
OccupySensor occupySensor("OccupySensor");
WebPortal webPortal("WebPortal");

WoodBlockContainer blockContainer;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  blockContainer.hostWoodBlock(relay);
  blockContainer.hostWoodBlock(occupySensor);
  blockContainer.hostWoodBlock(webPortal);

  {
    const char *outVariableNames[] = {"Status"};
    const char *inVariableNames[] = {"Status"};

    blockContainer.connect("OccupySensor", "Occupy", outVariableNames, ARRAY_SIZE(outVariableNames),
                           "WebPortal", "Occupy", inVariableNames, ARRAY_SIZE(inVariableNames));
  }
  {
    const char *outVariableNames[] = {"OnOff"};
    const char *inVariableNames[] = {"OnOff"};
    blockContainer.connect("WebPortal", "Control", outVariableNames, ARRAY_SIZE(outVariableNames),
                           "Relay", "Control", inVariableNames, ARRAY_SIZE(inVariableNames));
  }
}

void loop()
{
  // put your main code here, to run repeatedly:

  occupySensor.fetchExternalEvents();
  webPortal.fetchExternalEvents();

  delay(1000);
}
