// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WoodBlock.h>
#include "MyMacro.h"

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

  void executeEventInput(const EventInput& inEvent) {
    if (inEvent.getName().equals(RelayFBType::IE_CONTROL)) {
      Vi<Bool>* ivOnOff = (Vi<Bool>*)findInputVariableByName(RelayFBType::IV_ONOFF);
      const EventInput* ieControl = &inEvent;
      if (ivOnOff) {
        BOOL* onOff = ivOnOff->getDataBox().getData();
        if (onOff) {
          WB_OUT(
              "%s \t\tProcess: \tEVENT_INPUT \t%s \tWITH \tOnOff \t(* %s, "
              "\tline:%d *) \r\n",
              getName().c_str(), ieControl->getName().c_str(),
              (*onOff) ? "true" : "false", __LINE__);
        }
      }
    } else {
      WB_OUT("TODO: Don't deal event(%s), line:%d !!!!!!!! \r\n",
                    inEvent.getName().c_str(), __LINE__);
    }
  }
};
const char* RelayFBType::FB_TYPE_NAME = "RelayFBType";
const char* RelayFBType::IV_ONOFF = "OnOff";
const char* RelayFBType::IE_CONTROL = "Control";