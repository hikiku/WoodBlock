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
  static const char* FB_TYPE_NAME;
  static const char* TV_ONOFF;
  static const char* OV_ONOFF;
  static const char* OE_CONTROL;

  WebPortal() : SIFBType(WebPortal::FB_TYPE_NAME) {
    Vt<Bool>* tvOnOff = addInternalVariable<Bool>(WebPortal::TV_ONOFF);
    tvOnOff->getDataBox().setData(true);

    /*Vo<Bool>* ovOnOff =*/addOutputVariable<Bool>(WebPortal::OV_ONOFF);

    {
      const char* outVariableNames[] = {WebPortal::OV_ONOFF};
      /*EventOutput* oeControl =*/addEventOutput(WebPortal::OE_CONTROL,
                                              outVariableNames,
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
      EventOutput* oeControl =
          (EventOutput*)findEventOutputByName(WebPortal::OE_CONTROL);
      Vo<Bool>* ovOnOff = (Vo<Bool>*)findOutputVariableByName(WebPortal::OV_ONOFF);
      Vt<Bool>* tvOnOff =
          (Vt<Bool>*)findInternalVariableByName(WebPortal::TV_ONOFF);
      if (tvOnOff && ovOnOff && oeControl) {
        BOOL onOff = *(tvOnOff->getDataBox().getData());
        tvOnOff->getDataBox().setData(!onOff);
        ovOnOff->getDataBox().setData(onOff);
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
};
const char* WebPortal::FB_TYPE_NAME = "WebPortal";
const char* WebPortal::TV_ONOFF = "OnOff";
const char* WebPortal::OV_ONOFF = "OnOff";
const char* WebPortal::OE_CONTROL = "Control";

class Relay : public BasicFBType {
 public:
  static const char* FB_TYPE_NAME;
  static const char* IV_ONOFF;
  static const char* IE_CONTROL;

  Relay() : BasicFBType(Relay::FB_TYPE_NAME) {
    /*Vi<Bool>* ivOnOff =*/addInputVariable<Bool>(Relay::IV_ONOFF);

    {
      const char* inVariableNames[] = {Relay::IV_ONOFF};
      /*EventInput* ieControl =*/addEventInput(Relay::IE_CONTROL, inVariableNames,
                                            ARRAY_SIZE(inVariableNames));
    }
  }
  ~Relay() {}

  void executeEventInput(EventInput& inEvent) {
    if (inEvent.getName().equals(Relay::IE_CONTROL)) {
      Vi<Bool>* ivOnOff = (Vi<Bool>*)findInputVariableByName(Relay::IV_ONOFF);
      EventInput* ieControl = &inEvent;
      if (ivOnOff) {
        BOOL* onOff = ivOnOff->getDataBox().getData();
        if (onOff) {
          Serial.printf(
              "%s \t\tProcess: \tEVENT_INPUT \t%s \tWITH \tOnOff \t(* %s, "
              "\tline:%d *) \n",
              getName().c_str(), ieControl->getName().c_str(),
              (*onOff) ? "true" : "false", __LINE__);
        }
      }
    } else {
      Serial.printf("TODO: Don't deal event(%s), line:%d !!!!!!!!\n",
                    inEvent.getName().c_str(), __LINE__);
    }
  }
};
const char* Relay::FB_TYPE_NAME = "Relay";
const char* Relay::IV_ONOFF = "OnOff";
const char* Relay::IE_CONTROL = "Control";

FBNetwork fbNetwork;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  fbNetwork.attachFBInstance(FBInstance::create<Relay>("Relay"));
  fbNetwork.attachFBInstance(FBInstance::create<WebPortal>("WebPortal"));
  {
    const char* outVariableNames[] = {WebPortal::OV_ONOFF};
    const char* inVariableNames[] = {Relay::IV_ONOFF};
    fbNetwork.connect("WebPortal", WebPortal::OE_CONTROL, outVariableNames,
                      ARRAY_SIZE(outVariableNames), "Relay", Relay::IE_CONTROL,
                      inVariableNames, ARRAY_SIZE(inVariableNames));
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  fbNetwork.fetchExternalEvents();
  delay(1000);
}
