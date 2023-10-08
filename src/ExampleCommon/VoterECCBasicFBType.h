// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WoodBlock.h>
#include "MyMacro.h"

class VoterECCBasicFBType : public ECCBasicFBType {
 public:
  static const char* FB_TYPE_NAME;

  static const char* IE_VOTE;
  static const char* IE_RESET;
  static const char* IV_A;
  static const char* IV_B;
  static const char* IV_C;

  static const char* OE_VOTED;
  static const char* OE_READY;
  static const char* OV_STATE;

  static const char* A_VoteAlg;
  static const char* A_ResetAlg;

  static const char* ES_Ready;
  static const char* ES_Vote;
  static const char* ES_VotedPos;
  static const char* ES_Reset;

  VoterECCBasicFBType() : ECCBasicFBType(VoterECCBasicFBType::FB_TYPE_NAME) {
    // Input
    {
      addInputVariable<Bool>(VoterECCBasicFBType::IV_A);
      addInputVariable<Bool>(VoterECCBasicFBType::IV_B);
      addInputVariable<Bool>(VoterECCBasicFBType::IV_C);
      {
        const char* inVariableNames[] = {VoterECCBasicFBType::IV_A,
                                         VoterECCBasicFBType::IV_B,
                                         VoterECCBasicFBType::IV_C};
        addEventInput(VoterECCBasicFBType::IE_VOTE, inVariableNames,
                      ARRAY_SIZE(inVariableNames));
        addEventInput(VoterECCBasicFBType::IE_RESET);  // TODO:!!!!!
      }
    }

    // Output
    {
      addOutputVariable<Bool>(VoterECCBasicFBType::OV_STATE);
      {
        const char* outVariableNames[] = {VoterECCBasicFBType::OV_STATE};
        addEventOutput(VoterECCBasicFBType::OE_VOTED, outVariableNames,
                       ARRAY_SIZE(outVariableNames));
        addEventOutput(VoterECCBasicFBType::OE_READY, outVariableNames,
                       ARRAY_SIZE(outVariableNames));
      }
    }

    // ECC
    {
      addAlgorithm(VoterECCBasicFBType::A_VoteAlg,
                   std::bind(&VoterECCBasicFBType::VoteAlg, this));
      addAlgorithm(VoterECCBasicFBType::A_ResetAlg,
                   std::bind(&VoterECCBasicFBType::ResetAlg, this));

      addECState(VoterECCBasicFBType::ES_Ready);
      addECState(VoterECCBasicFBType::ES_Vote);
      addECState(VoterECCBasicFBType::ES_VotedPos);
      addECState(VoterECCBasicFBType::ES_Reset);

      addECAction(VoterECCBasicFBType::ES_Vote, VoterECCBasicFBType::A_VoteAlg,
                  VoterECCBasicFBType::OE_VOTED);
      addECAction(VoterECCBasicFBType::ES_Reset,
                  VoterECCBasicFBType::A_ResetAlg,
                  VoterECCBasicFBType::OE_READY);

      // addECTransitionWithEvent()
      addECTransition(VoterECCBasicFBType::ES_Ready,
                      VoterECCBasicFBType::ES_Vote,
                      VoterECCBasicFBType::IE_VOTE);
      // addECTransitionWithBoolExpression()
      addECTransition(VoterECCBasicFBType::ES_Vote,
                      VoterECCBasicFBType::ES_VotedPos, [this]() {
                        Vo<Bool>* State = (Vo<Bool>*)findOutputVariableByName(
                            VoterECCBasicFBType::OV_STATE);
                        if (State) {
                          BOOL* state = State->getDataBox().getData();
                          if (state) {
                            return *state;
                          }
                        }
                        return false;
                      });
      // addECTransitionWithUnconditional();
      addECTransition(VoterECCBasicFBType::ES_Vote,
                      VoterECCBasicFBType::ES_Ready);
      // addECTransitionWithEvent()
      addECTransition(VoterECCBasicFBType::ES_VotedPos,
                      VoterECCBasicFBType::ES_Reset,
                      VoterECCBasicFBType::IE_RESET);
      // addECTransitionWithUnconditional()
      addECTransition(VoterECCBasicFBType::ES_Reset,
                      VoterECCBasicFBType::ES_Ready);
    }
  }
  ~VoterECCBasicFBType() {}

  //   void executeEventInput(const EventInput& inEvent) {
  //     if (inEvent.getName().equals(VoterECCBasicFBType::IE_VOTE)) {
  //       EventInput* ieOccupy = &inEvent;
  //       Vi<Bool>* ivStatus =
  //           (Vi<Bool>*)findInputVariableByName(VoterECCBasicFBType::IV_A);
  //       if (ivStatus) {
  //         BOOL* status = ivStatus->getDataBox().getData();
  //         if (status) {
  //           WB_OUT(
  //               "%s \tProcess: \tEVENT_INPUT \t%s \t\tWITH \tStatus \t(* %s,
  //               "
  //               "\tline:%d *) \r\n",
  //               getName().c_str(), ieOccupy->getName().c_str(),
  //               (*status) ? "true" : "false", __LINE__);
  //         }
  //       }
  //       return;
  //     }

  //     WB_OUT("TODO: Don't deal event(%s), line:%d !!!!!!!! \r\n",
  //                   inEvent.getName().c_str(), __LINE__);
  //   }
  //   bool captureAndExecuteServiceInterfaceInEvent() {
  //     static long unsigned int lasttime =
  //         0;  // time stamp, millisecond  //Fixed bug: std::time(0) is
  //             // changed with SNTP response!
  //     long unsigned int time =
  //         millis();  //(uint32_t)std::time(0); //Fixed bug: std::time(0) is
  //                    // changed with SNTP response!

  //     if (lasttime == 0) {
  //       lasttime = time + 5 * 1000;
  //     }

  //     if (time - lasttime > 10 * 1000) {
  //       EventOutput* oeControl =
  //           (EventOutput*)findEventOutputByName(VoterECCBasicFBType::OE_VOTED);
  //       Vo<Bool>* ovOnOff =
  //           (Vo<Bool>*)findOutputVariableByName(VoterECCBasicFBType::OV_STATE);
  //       Vt<Bool>* tvOnOff =
  //           (Vt<Bool>*)findInternalVariableByName(VoterECCBasicFBType::TV_ONOFF);
  //       if (tvOnOff && ovOnOff && oeControl) {
  //         BOOL onOff = *(tvOnOff->getDataBox().getData());
  //         tvOnOff->getDataBox().setData(!onOff);
  //         ovOnOff->getDataBox().setData(onOff);
  //         WB_OUT(
  //             "%s \tGenerate: \tEVENT_OUTPUT \t%s \tWITH \tOnOff \t(* %s, "
  //             "\tline:%d *)\n",
  //             getName().c_str(), oeControl->getName().c_str(),
  //             onOff ? "true" : "false", __LINE__);
  //         generateEventOutput(*oeControl);
  //       }
  //       lasttime = time;
  //       return true;
  //     }

  //     return false;
  //   }

 private:
  // TODO: Merge to class Aogorithm!
  void VoteAlg() {
    // TODO: vi<T>() is Aogorithm's member function !
    /*const*/ Bool& A = vi<Bool>(VoterECCBasicFBType::IV_A);
    /*const*/ Bool& B = vi<Bool>(VoterECCBasicFBType::IV_B);
    /*const*/ Bool& C = vi<Bool>(VoterECCBasicFBType::IV_C);

    Bool& state = vo<Bool>(VoterECCBasicFBType::OV_STATE);

    // TODO: return const reference !
    const BOOL& a = A.getDataRef();
    const BOOL& b = B.getDataRef();
    const BOOL& c = C.getDataRef();

    BOOL a_ = a;
    BOOL b_ = b;
    BOOL c_ = c;

    BOOL state_ = (a_ && b_) || (a_ && c_) || (b_ && c_);
    state.setData(state_);

    // Vi<Bool>* A =
    // (Vi<Bool>*)findInputVariableByName(VoterECCBasicFBType::IV_A); Vi<Bool>*
    // B = (Vi<Bool>*)findInputVariableByName(VoterECCBasicFBType::IV_B);
    // Vi<Bool>* C =
    // (Vi<Bool>*)findInputVariableByName(VoterECCBasicFBType::IV_C);

    // Vo<Bool>* State =
    //     (Vo<Bool>*)findOutputVariableByName(VoterECCBasicFBType::OV_STATE);

    // if (A && B && C && State) {
    //   BOOL* a = A->getDataBox().getData();
    //   BOOL* b = B->getDataBox().getData();
    //   BOOL* c = C->getDataBox().getData();

    //   if (a && b && c) {
    //     BOOL a_ = *a;
    //     BOOL b_ = *b;
    //     BOOL c_ = *c;

    //     BOOL state_ = (a_ && b_) || (a_ && c_) || (b_ && c_);
    //     State->getDataBox().setData(state_);
    //   }
    // }
  }
  void ResetAlg() {
    // WB_LOGD("ResetAlg()");

    Vo<Bool>* State =
        (Vo<Bool>*)findOutputVariableByName(VoterECCBasicFBType::OV_STATE);

    if (State) {
      State->getDataBox().setData(false);
    }
  }
};

const char* VoterECCBasicFBType::FB_TYPE_NAME = "VoterECCBasicFBType";

const char* VoterECCBasicFBType::IE_VOTE = "Vote";
const char* VoterECCBasicFBType::IE_RESET = "Reset";
const char* VoterECCBasicFBType::IV_A = "A";
const char* VoterECCBasicFBType::IV_B = "B";
const char* VoterECCBasicFBType::IV_C = "C";

const char* VoterECCBasicFBType::OE_VOTED = "Voted";
const char* VoterECCBasicFBType::OE_READY = "Ready";
const char* VoterECCBasicFBType::OV_STATE = "State";

const char* VoterECCBasicFBType::A_VoteAlg = "VoteAlg";
const char* VoterECCBasicFBType::A_ResetAlg = "ResetAlg";

const char* VoterECCBasicFBType::ES_Ready = "Ready";
const char* VoterECCBasicFBType::ES_Vote = "Vote";
const char* VoterECCBasicFBType::ES_VotedPos = "VotedPos";
const char* VoterECCBasicFBType::ES_Reset = "Reset";