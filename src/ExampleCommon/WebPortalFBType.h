// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WoodBlock.h>
#include "MyMacro.h"

class WebPortalFBType : public SIFBType {
 public:
  static const char* FB_TYPE_NAME;
  static const char* IV_STATUS;
  static const char* IE_OCCUPY;

  static const char* TV_ONOFF;
  static const char* OV_ONOFF;
  static const char* OE_CONTROL;

  WebPortalFBType(bool hasEventInput = true, bool hasEventOutput = true)
      : SIFBType(WebPortalFBType::FB_TYPE_NAME),
        hasEventInput(hasEventInput),
        hasEventOutput(hasEventOutput) {
    if (hasEventInput) {
      /*Vi<Bool>* ivStatus =*/addInputVariable<Bool>(
          WebPortalFBType::IV_STATUS);

      {
        const char* inVariableNames[] = {WebPortalFBType::IV_STATUS};
        /*EventInput* ieOccupy =*/
        addEventInput(WebPortalFBType::IE_OCCUPY, inVariableNames,
                      ARRAY_SIZE(inVariableNames));
      }
    }

    if (hasEventOutput) {
      Vt<Bool>* tvOnOff = addInternalVariable<Bool>(WebPortalFBType::TV_ONOFF);
      tvOnOff->getDataBox().setData(true);

      /*Vo<Bool>* ovOnOff =*/addOutputVariable<Bool>(WebPortalFBType::OV_ONOFF);

      {
        const char* outVariableNames[] = {WebPortalFBType::OV_ONOFF};
        /*EventOutput* oeControl =*/addEventOutput(
            WebPortalFBType::OE_CONTROL, outVariableNames,
            ARRAY_SIZE(outVariableNames));
      }
    }
  }
  ~WebPortalFBType() {}

  void executeEventInput(const EventInput& inEvent) {
    if (hasEventInput) {
      if (inEvent.getName().equals(WebPortalFBType::IE_OCCUPY)) {
        const EventInput* ieOccupy = &inEvent;
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
        return;
      }
    }

    Serial.printf("TODO: Don't deal event(%s), line:%d !!!!!!!!\n",
                  inEvent.getName().c_str(), __LINE__);
  }
  bool captureAndExecuteServiceInterfaceInEvent() {
    if (hasEventOutput) {
      static long unsigned int lasttime =
          0;  // time stamp, millisecond  //Fixed bug: std::time(0) is
              // changed with SNTP response!
      long unsigned int time =
          millis();  //(uint32_t)std::time(0); //Fixed bug: std::time(0) is
                     // changed with SNTP response!

      if (lasttime == 0) {
        lasttime = time + 5 * 1000;
      }

      if (time - lasttime > 10 * 1000) {
        EventOutput* oeControl =
            (EventOutput*)findEventOutputByName(WebPortalFBType::OE_CONTROL);
        Vo<Bool>* ovOnOff =
            (Vo<Bool>*)findOutputVariableByName(WebPortalFBType::OV_ONOFF);
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
          generateEventOutput(*oeControl);
        }
        lasttime = time;
        return true;
      }
    }
    return false;
  }

 private:
  bool hasEventInput;
  bool hasEventOutput;
};
const char* WebPortalFBType::FB_TYPE_NAME = "WebPortalFBType";
const char* WebPortalFBType::IV_STATUS = "Status";
const char* WebPortalFBType::IE_OCCUPY = "Occupy";
const char* WebPortalFBType::TV_ONOFF = "OnOff";
const char* WebPortalFBType::OV_ONOFF = "OnOff";
const char* WebPortalFBType::OE_CONTROL = "Control";