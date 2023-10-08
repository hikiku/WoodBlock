// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <ExampleCommon/MyMacro.h>
#include <WoodBlock.h>

class VoterDiscarderFBType : public BasicFBType {
 public:
  static const char* FB_TYPE_NAME;
  static const char* IV_STATE;
  static const char* IE_VOTED;
  static const char* IE_READY;

  VoterDiscarderFBType() : BasicFBType(VoterDiscarderFBType::FB_TYPE_NAME) {
    addInputVariable<Bool>(VoterDiscarderFBType::IV_STATE);

    {
      const char* inVariableNames[] = {VoterDiscarderFBType::IV_STATE};
      addEventInput(VoterDiscarderFBType::IE_VOTED, inVariableNames,
                    ARRAY_SIZE(inVariableNames));
      addEventInput(VoterDiscarderFBType::IE_READY, inVariableNames,
                    ARRAY_SIZE(inVariableNames));
    }
  }
  ~VoterDiscarderFBType() {}

  void executeEventInput(const EventInput& inEvent) {
    if (inEvent.getName().equals(VoterDiscarderFBType::IE_VOTED)) {
      Vi<Bool>* ivState =
          (Vi<Bool>*)findInputVariableByName(VoterDiscarderFBType::IV_STATE);
      const EventInput* ieVoted = &inEvent;
      if (ivState) {
        BOOL* state = ivState->getDataBox().getData();
        if (state) {
          WB_OUT(
              "%s \tProcess: \tEVENT_INPUT \t%s \t\tWITH \tState \t(* %s, "
              "\t\tline:%d *) \r\n",
              getName().c_str(), ieVoted->getName().c_str(),
              (*state) ? "true" : "false", __LINE__);
        }
      }
    } else if (inEvent.getName().equals(VoterDiscarderFBType::IE_READY)) {
      Vi<Bool>* ivState =
          (Vi<Bool>*)findInputVariableByName(VoterDiscarderFBType::IV_STATE);
      const EventInput* ieReady = &inEvent;
      if (ivState) {
        BOOL* state = ivState->getDataBox().getData();
        if (state) {
          WB_OUT(
              "%s \tProcess: \tEVENT_INPUT \t%s \t\tWITH \tState \t(* %s, "
              "\t\tline:%d *) \r\n",
              getName().c_str(), ieReady->getName().c_str(),
              (*state) ? "true" : "false", __LINE__);
        }
      }
    } else {
      WB_OUT("TODO: Don't deal event(%s), line:%d !!!!!!!! \r\n",
                    inEvent.getName().c_str(), __LINE__);
    }
  }
};

const char* VoterDiscarderFBType::FB_TYPE_NAME = "VoterDiscarderFBType";
const char* VoterDiscarderFBType::IV_STATE = "State";
const char* VoterDiscarderFBType::IE_VOTED = "Voted";
const char* VoterDiscarderFBType::IE_READY = "Ready";