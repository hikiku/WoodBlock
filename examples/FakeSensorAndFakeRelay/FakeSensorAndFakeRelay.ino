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

class WebPortalFBType : public SIFBType {
 public:
  static const char* FB_TYPE_NAME;
  static const char* IV_STATUS;
  static const char* IE_OCCUPY;

  static const char* TV_ONOFF;
  static const char* OV_ONOFF;
  static const char* OE_CONTROL;

  WebPortalFBType() : SIFBType(WebPortalFBType::FB_TYPE_NAME) {
    /*Vi<Bool>* ivStatus =*/addInputVariable<Bool>(WebPortalFBType::IV_STATUS);

    {
      const char* outVariableNames[] = {WebPortalFBType::IV_STATUS};
      /*EventInput* ieOccupy =*/
      addEventInput(WebPortalFBType::IE_OCCUPY, outVariableNames,
                 ARRAY_SIZE(outVariableNames));
    }

    Vt<Bool>* tvOnOff = addInternalVariable<Bool>(WebPortalFBType::TV_ONOFF);
    tvOnOff->getDataBox().setData(true);

    /*Vo<Bool>* ovOnOff =*/addOutputVariable<Bool>(WebPortalFBType::OV_ONOFF);

    {
      const char* outVariableNames[] = {WebPortalFBType::OV_ONOFF};
      /*EventOutput* oeControl =*/addEventOutput(WebPortalFBType::OE_CONTROL,
                                              outVariableNames,
                                              ARRAY_SIZE(outVariableNames));
    }
  }
  ~WebPortalFBType() {}

  void executeEventInput(EventInput& inEvent) {
    if (inEvent.getName().equals(WebPortalFBType::IE_OCCUPY)) {
      EventInput* ieOccupy = &inEvent;
      Vi<Bool>* ivStatus =
          (Vi<Bool>*)findInputVariableByName(WebPortalFBType::IV_STATUS);
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
          (EventOutput*)findEventOutputByName(WebPortalFBType::OE_CONTROL);
      Vo<Bool>* ovOnOff = (Vo<Bool>*)findOutputVariableByName(WebPortalFBType::OV_ONOFF);
      Vt<Bool>* tvOnOff =
          (Vt<Bool>*)findInternalVariableByName(WebPortalFBType::TV_ONOFF);
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
const char* WebPortalFBType::FB_TYPE_NAME = "WebPortalFBType";
const char* WebPortalFBType::IV_STATUS = "Status";
const char* WebPortalFBType::IE_OCCUPY = "Occupy";
const char* WebPortalFBType::TV_ONOFF = "OnOff";
const char* WebPortalFBType::OV_ONOFF = "OnOff";
const char* WebPortalFBType::OE_CONTROL = "Control";

class OccupySensorFBType : public SIFBType {
 public:
  static const char* FB_TYPE_NAME;
  static const char* TV_STATUS;
  static const char* OV_STATUS;
  static const char* OE_OCCUPY;

  OccupySensorFBType() : SIFBType(OccupySensorFBType::FB_TYPE_NAME) {
    Vt<Bool>* tvStatus = addInternalVariable<Bool>(OccupySensorFBType::TV_STATUS);
    tvStatus->getDataBox().setData(false);

    /*Vo<Bool>* ovStatus =*/addOutputVariable<Bool>(OccupySensorFBType::OV_STATUS);

    {
      const char* outVariableNames[] = {OccupySensorFBType::OV_STATUS};
      /*EventOutput* oeOccupy =*/
      addEventOutput(OccupySensorFBType::OE_OCCUPY, outVariableNames,
                  ARRAY_SIZE(outVariableNames));
    }
  }
  ~OccupySensorFBType() {}

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
          (EventOutput*)findEventOutputByName(OccupySensorFBType::OE_OCCUPY);
      Vo<Bool>* ovStatus =
          (Vo<Bool>*)findOutputVariableByName(OccupySensorFBType::OV_STATUS);
      Vt<Bool>* tvStatus =
          (Vt<Bool>*)findInternalVariableByName(OccupySensorFBType::TV_STATUS);
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
const char* OccupySensorFBType::FB_TYPE_NAME = "OccupySensorFBType";
const char* OccupySensorFBType::TV_STATUS = "Status";
const char* OccupySensorFBType::OV_STATUS = "Status";
const char* OccupySensorFBType::OE_OCCUPY = "Occupy";

class RelayFBType : public BasicFBType {
 public:
  static const char* FB_TYPE_NAME;
  static const char* IV_ONOFF;
  static const char* IE_CONTROL;

  RelayFBType() : BasicFBType(RelayFBType::FB_TYPE_NAME) {
    /*Vi<Bool>* ivOnOff =*/addInputVariable<Bool>(RelayFBType::IV_ONOFF);

    {
      const char* inVariableNames[] = {RelayFBType::IV_ONOFF};
      /*EventInput* ieControl =*/addEventInput(RelayFBType::IE_CONTROL, inVariableNames,
                                            ARRAY_SIZE(inVariableNames));
    }
  }
  ~RelayFBType() {}

  void executeEventInput(EventInput& inEvent) {
    if (inEvent.getName().equals(RelayFBType::IE_CONTROL)) {
      Vi<Bool>* ivOnOff = (Vi<Bool>*)findInputVariableByName(RelayFBType::IV_ONOFF);
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
const char* RelayFBType::FB_TYPE_NAME = "RelayFBType";
const char* RelayFBType::IV_ONOFF = "OnOff";
const char* RelayFBType::IE_CONTROL = "Control";

FBNetwork fbNetwork;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  fbNetwork.attachFBInstance(FBInstance::create<RelayFBType>("Relay"));
  fbNetwork.attachFBInstance(FBInstance::create<OccupySensorFBType>("OccupySensor"));
  fbNetwork.attachFBInstance(FBInstance::create<WebPortalFBType>("WebPortal"));
  {
    const char* outVariableNames[] = {OccupySensorFBType::OV_STATUS};
    const char* inVariableNames[] = {WebPortalFBType::IV_STATUS};
    fbNetwork.connect("OccupySensor", OccupySensorFBType::OE_OCCUPY, outVariableNames,
                      ARRAY_SIZE(outVariableNames), "WebPortal",
                      WebPortalFBType::IE_OCCUPY, inVariableNames,
                      ARRAY_SIZE(inVariableNames));
  }
  {
    const char* outVariableNames[] = {WebPortalFBType::OV_ONOFF};
    const char* inVariableNames[] = {RelayFBType::IV_ONOFF};
    fbNetwork.connect("WebPortal", WebPortalFBType::OE_CONTROL, outVariableNames,
                      ARRAY_SIZE(outVariableNames), "Relay", RelayFBType::IE_CONTROL,
                      inVariableNames, ARRAY_SIZE(inVariableNames));
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  fbNetwork.fetchExternalEvents();
  delay(1000);
}
