// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#include <WoodBlock.h>

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

// For user extend data type
extern bool extend_check4ConnectDataType(unsigned int outDataType,
                                         unsigned int inDataType) {
  return false;
}

class WebPortal : public SIFBType {
 public:
  WebPortal()
      : SIFBType("WebPortal"),
        ovOnOff(nullptr),
        oeControl(nullptr),
        onOff(nullptr) {
    ovOnOff = addOutVariable<Bool>("OnOff");
    {
      const char* outVariableNames[] = {"OnOff"};
      oeControl = addOutEvent("Control", outVariableNames,
                              ARRAY_SIZE(outVariableNames));
    }
  }
  ~WebPortal() {}

  void executeEventInput(EventInput& inEvent) {
    Serial.printf("TODO: Don't deal event(%s), line:%d !!!!!!!!\n",
                  inEvent.getName().c_str(), __LINE__);
  }
  bool captureAndExecuteServiceInterfaceInEvent() {
    static long unsigned int lasttime =
        0;  // time stamp, millisecond  //Fixed bug: std::time(0) is changed
            // with SNTP response!
    long unsigned int time =
        millis();  //(uint32_t)std::time(0); //Fixed bug: std::time(0) is
                   // changed with SNTP response!

    if (lasttime == 0) {
      lasttime = time + 5 * 1000;
    }

    if (time - lasttime > 10 * 1000) {
      if (ovOnOff) {
        onOff = !onOff;
        ovOnOff->getDataBox().setData(onOff);
      }
      if (oeControl) {
        Serial.printf(
            "%s \tGenerate: \tEVENT_OUTPUT \t%s \tWITH \tOnOff \t(* %s, "
            "\tline:%d *)\n",
            getName().c_str(), oeControl->getName().c_str(),
            onOff ? "true" : "false", __LINE__);
        generateOutEvent(*oeControl);
      }
      lasttime = time;
      return true;
    }
    return false;
  }

 private:
  Vo<Bool>* ovOnOff;
  EventOutput* oeControl;
  bool onOff;
};

class Relay : public BasicFBType {
 public:
  Relay() : BasicFBType("Relay") {
    ivOnOff = addInVariable<Bool>("OnOff");
    {
      const char* inVariableNames[] = {"OnOff"};
      ieControl =
          addInEvent("Control", inVariableNames, ARRAY_SIZE(inVariableNames));
    }
  }
  ~Relay() {}

  void executeEventInput(EventInput& inEvent) {
    if (inEvent.getName().equals("Control")) {
      if (ivOnOff) {
        BOOL* onOff = ivOnOff->getDataBox().getData();
        if (onOff) {
          Serial.printf(
              "%s \t\tProcess: \tEVENT_INPUT \t%s \tWITH \tOnOff \t(* %s, "
              "\tline:%d *) \n",
              getName().c_str(), inEvent.getName().c_str(),
              (*onOff) ? "true" : "false", __LINE__);
        }
      }
    } else {
      Serial.printf("TODO: Don't deal event(%s), line:%d !!!!!!!!\n",
                    inEvent.getName().c_str(), __LINE__);
    }
  }

 private:
  Vi<Bool>* ivOnOff;
  EventInput* ieControl;
};

FBNetwork fbNetwork;

Vi<Time> myTime();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  FBInstance* relay = FBInstance::create<Relay>("Relay");
  FBInstance* webPortal = FBInstance::create<WebPortal>("WebPortal");
  fbNetwork.attachFBInstance(relay);
  fbNetwork.attachFBInstance(webPortal);

  {
    const char* outVariableNames[] = {"OnOff"};
    const char* inVariableNames[] = {"OnOff"};
    fbNetwork.connect("WebPortal", "Control", outVariableNames,
                      ARRAY_SIZE(outVariableNames), "Relay", "Control",
                      inVariableNames, ARRAY_SIZE(inVariableNames));
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  fbNetwork.fetchExternalEvents();

  delay(1000);
}
