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
  static const char* IV_STATUS;
  static const char* IE_OCCUPY;

  static const char* TV_ONOFF;
  static const char* OV_ONOFF;
  static const char* OE_CONTROL;

  WebPortal() : SIFBType(WebPortal::FB_TYPE_NAME) {
    /*Vi<Bool>* ivStatus =*/addInVariable<Bool>(WebPortal::IV_STATUS);

    {
      const char* outVariableNames[] = {WebPortal::IV_STATUS};
      /*EventInput* ieOccupy =*/
      addInEvent(WebPortal::IE_OCCUPY, outVariableNames,
                 ARRAY_SIZE(outVariableNames));
    }

    Vt<Bool>* tvOnOff = addInternalVariable<Bool>(WebPortal::TV_ONOFF);
    tvOnOff->getDataBox().setData(true);

    /*Vo<Bool>* ovOnOff =*/addOutVariable<Bool>(WebPortal::OV_ONOFF);

    {
      const char* outVariableNames[] = {WebPortal::OV_ONOFF};
      /*EventOutput* oeControl =*/addOutEvent(WebPortal::OE_CONTROL,
                                              outVariableNames,
                                              ARRAY_SIZE(outVariableNames));
    }
  }
  ~WebPortal() {}

  void executeEventInput(EventInput& inEvent) {
    if (inEvent.getName().equals(WebPortal::IE_OCCUPY)) {
      EventInput* ieOccupy = &inEvent;
      Vi<Bool>* ivStatus =
          (Vi<Bool>*)findInVariableByName(WebPortal::IV_STATUS);
      if (ivStatus) {
        BOOL* status = ivStatus->getDataBox().getData();
        if (status) {
          Serial.printf(
              "%s \tProcess: \tEVENT_INPUT \t%s \t\tWITH \tStatus \t(* %s, "
              "\tline:%d *) \n",
              getName().c_str(), ieOccupy->getName().c_str(),
              (*status) ? "true" : "false", __LINE__);
        }
      }
    } else {
      Serial.printf("TODO: Don't deal event(%s), line:%d !!!!!!!!\n",
                    inEvent.getName().c_str(), __LINE__);
    }
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
          (EventOutput*)findOutEventByName(WebPortal::OE_CONTROL);
      Vo<Bool>* ovOnOff = (Vo<Bool>*)findOutVariableByName(WebPortal::OV_ONOFF);
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
const char* WebPortal::IV_STATUS = "Status";
const char* WebPortal::IE_OCCUPY = "Occupy";
const char* WebPortal::TV_ONOFF = "OnOff";
const char* WebPortal::OV_ONOFF = "OnOff";
const char* WebPortal::OE_CONTROL = "Control";

class OccupySensor : public SIFBType {
 public:
  static const char* FB_TYPE_NAME;
  static const char* TV_STATUS;
  static const char* OV_STATUS;
  static const char* OE_OCCUPY;

  OccupySensor() : SIFBType(OccupySensor::FB_TYPE_NAME) {
    Vt<Bool>* tvStatus = addInternalVariable<Bool>(OccupySensor::TV_STATUS);
    tvStatus->getDataBox().setData(false);

    /*Vo<Bool>* ovStatus =*/addOutVariable<Bool>(OccupySensor::OV_STATUS);

    {
      const char* outVariableNames[] = {OccupySensor::OV_STATUS};
      /*EventOutput* oeOccupy =*/
      addOutEvent(OccupySensor::OE_OCCUPY, outVariableNames,
                  ARRAY_SIZE(outVariableNames));
    }
  }
  ~OccupySensor() {}

  void executeEventInput(EventInput& inEvent) {
    Serial.printf("TODO: Don't deal event(%s), line:%d\n",
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
      lasttime = time;
    }

    if (time - lasttime > 10 * 1000) {
      EventOutput* oeOccupy =
          (EventOutput*)findOutEventByName(OccupySensor::OE_OCCUPY);
      Vo<Bool>* ovStatus =
          (Vo<Bool>*)findOutVariableByName(OccupySensor::OV_STATUS);
      Vt<Bool>* tvStatus =
          (Vt<Bool>*)findInternalVariableByName(OccupySensor::TV_STATUS);
      if (oeOccupy && ovStatus && tvStatus) {
        BOOL status = *(tvStatus->getDataBox().getData());
        tvStatus->getDataBox().setData(!status);
        ovStatus->getDataBox().setData(status);

        Serial.printf(
            "%s \tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \tStatus \t(* %s, "
            "\tline:%d *)\n",
            getName().c_str(), oeOccupy->getName().c_str(),
            status ? "true" : "false", __LINE__);
        generateOutEvent(*oeOccupy);
      }
      lasttime = time;
      return true;
    }
    return false;
  }
};
const char* OccupySensor::FB_TYPE_NAME = "OccupySensor";
const char* OccupySensor::TV_STATUS = "Status";
const char* OccupySensor::OV_STATUS = "Status";
const char* OccupySensor::OE_OCCUPY = "Occupy";

class Relay : public BasicFBType {
 public:
  static const char* FB_TYPE_NAME;
  static const char* IV_ONOFF;
  static const char* IE_CONTROL;

  Relay() : BasicFBType(Relay::FB_TYPE_NAME) {
    /*Vi<Bool>* ivOnOff =*/addInVariable<Bool>(Relay::IV_ONOFF);

    {
      const char* inVariableNames[] = {Relay::IV_ONOFF};
      /*EventInput* ieControl =*/addInEvent(Relay::IE_CONTROL, inVariableNames,
                                            ARRAY_SIZE(inVariableNames));
    }
  }
  ~Relay() {}

  void executeEventInput(EventInput& inEvent) {
    if (inEvent.getName().equals(Relay::IE_CONTROL)) {
      Vi<Bool>* ivOnOff = (Vi<Bool>*)findInVariableByName(Relay::IV_ONOFF);
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
  fbNetwork.attachFBInstance(FBInstance::create<OccupySensor>("OccupySensor"));
  fbNetwork.attachFBInstance(FBInstance::create<WebPortal>("WebPortal"));
  {
    const char* outVariableNames[] = {OccupySensor::OV_STATUS};
    const char* inVariableNames[] = {WebPortal::IV_STATUS};
    fbNetwork.connect("OccupySensor", OccupySensor::OE_OCCUPY, outVariableNames,
                      ARRAY_SIZE(outVariableNames), "WebPortal",
                      WebPortal::IE_OCCUPY, inVariableNames,
                      ARRAY_SIZE(inVariableNames));
  }
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
