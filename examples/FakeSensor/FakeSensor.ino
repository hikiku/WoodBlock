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

  WebPortal() : SIFBType(WebPortal::FB_TYPE_NAME) {
    /*Vi<Bool>* ivStatus =*/addInVariable<Bool>(WebPortal::IV_STATUS);

    {
      const char* outVariableNames[] = {WebPortal::IV_STATUS};
      /*EventInput* ieOccupy =*/
      addInEvent(WebPortal::IE_OCCUPY, outVariableNames,
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
    return false;
  }
};
const char* WebPortal::FB_TYPE_NAME = "WebPortal";
const char* WebPortal::IV_STATUS = "Status";
const char* WebPortal::IE_OCCUPY = "Occupy";

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

FBNetwork fbNetwork;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

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
}

void loop() {
  // put your main code here, to run repeatedly:

  fbNetwork.fetchExternalEvents();
  delay(1000);
}
