// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <ExampleCommon/MyMacro.h>
#include <WoodBlock.h>

class VoterDriverFBType : public SIFBType {
 public:
  static const char* FB_TYPE_NAME;
  static const char* TV_STATUS;
  static const char* OV_A;
  static const char* OV_B;
  static const char* OV_C;
  static const char* OE_VOTE;
  static const char* OE_RESET;

  VoterDriverFBType() : SIFBType(VoterDriverFBType::FB_TYPE_NAME) {
    Vt<USInt>* tvStatus =
        addInternalVariable<USInt>(VoterDriverFBType::TV_STATUS);
    tvStatus->getDataBox().setData(0);

    addOutputVariable<Bool>(VoterDriverFBType::OV_A);
    addOutputVariable<Bool>(VoterDriverFBType::OV_B);
    addOutputVariable<Bool>(VoterDriverFBType::OV_C);

    {
      const char* outVariableNames[] = {VoterDriverFBType::OV_A,
                                        VoterDriverFBType::OV_B,
                                        VoterDriverFBType::OV_C};
      /*EventOutput* oeVote =*/
      addEventOutput(VoterDriverFBType::OE_VOTE, outVariableNames,
                     ARRAY_SIZE(outVariableNames));
      addEventOutput(VoterDriverFBType::OE_RESET, nullptr /*[]*/, 0);
    }
  }
  ~VoterDriverFBType() {}

  void executeEventInput(const EventInput& inEvent) {
    WB_OUT("TODO: Don't deal event(%s), line:%d \r\n",
           inEvent.getName().c_str(), __LINE__);
  }
  bool captureAndExecuteServiceInterfaceInEvent() {
    // WB_OUT("VoterDriverFBType::captureAndExecuteServiceInterfaceInEvent()\r\n");
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
      EventOutput* oeVote =
          (EventOutput*)findEventOutputByName(VoterDriverFBType::OE_VOTE);
      EventOutput* oeReset =
          (EventOutput*)findEventOutputByName(VoterDriverFBType::OE_RESET);
      Vo<Bool>* ovA =
          (Vo<Bool>*)findOutputVariableByName(VoterDriverFBType::OV_A);
      Vo<Bool>* ovB =
          (Vo<Bool>*)findOutputVariableByName(VoterDriverFBType::OV_B);
      Vo<Bool>* ovC =
          (Vo<Bool>*)findOutputVariableByName(VoterDriverFBType::OV_C);
      Vt<USInt>* tvStatus =
          (Vt<USInt>*)findInternalVariableByName(VoterDriverFBType::TV_STATUS);

      // WB_OUT("%s=%p \r\n", VoterDriverFBType::OE_VOTE, oeVote);
      // WB_OUT("%s=%p \r\n", VoterDriverFBType::OE_RESET, oeReset);
      // WB_OUT("%s=%p \r\n", VoterDriverFBType::OV_A, ovA);
      // WB_OUT("%s=%p \r\n", VoterDriverFBType::OV_B, ovB);
      // WB_OUT("%s=%p \r\n", VoterDriverFBType::OV_C, ovC);
      // WB_OUT("%s=%p \r\n", VoterDriverFBType::TV_STATUS, tvStatus);

      if (oeVote && oeReset && ovA && ovB && ovC && tvStatus) {
        USINT status = *(tvStatus->getDataBox().getData());
        tvStatus->getDataBox().setData(status + 1);

        WB_OUT("\r\n");
        switch (status % 8) {
          // [Vote(true, true, true)] event + [Reset] event
          case 0:
            ovA->getDataBox().setData(true);
            ovB->getDataBox().setData(true);
            ovC->getDataBox().setData(true);
            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \tA,B,C \t(* "
                "%s,%s,%s, "
                "\tline:%d *) \r\n",
                getName().c_str(), oeVote->getName().c_str(),
                true ? "true" : "false", true ? "true" : "false",
                true ? "true" : "false", __LINE__);
            generateEventOutput(*oeVote);

            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \t...... \t(* "
                "\t\t \tline:%d *) \r\n",
                getName().c_str(), oeReset->getName().c_str(), __LINE__);
            generateEventOutput(*oeReset);
            break;

          // [Vote(false, false, false)] event + [Reset] event
          case 1:
            ovA->getDataBox().setData(false);
            ovB->getDataBox().setData(false);
            ovC->getDataBox().setData(false);
            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \tA,B,C \t(* "
                "%s,%s,%s, "
                "\tline:%d *) \r\n",
                getName().c_str(), oeVote->getName().c_str(),
                false ? "true" : "false", false ? "true" : "false",
                false ? "true" : "false", __LINE__);
            generateEventOutput(*oeVote);

            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \t...... \t(* "
                "\t\t \tline:%d *) \r\n",
                getName().c_str(), oeReset->getName().c_str(), __LINE__);
            generateEventOutput(*oeReset);
            break;

          // [Vote(true, false, true)] event + [Reset] event
          case 2:
            ovA->getDataBox().setData(true);
            ovB->getDataBox().setData(false);
            ovC->getDataBox().setData(true);
            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \tA,B,C \t(* "
                "%s,%s,%s, "
                "\tline:%d *) \r\n",
                getName().c_str(), oeVote->getName().c_str(),
                true ? "true" : "false", false ? "true" : "false",
                true ? "true" : "false", __LINE__);
            generateEventOutput(*oeVote);

            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \t...... \t(* "
                "\t\t \tline:%d *) \r\n",
                getName().c_str(), oeReset->getName().c_str(), __LINE__);
            generateEventOutput(*oeReset);
            break;

          // [Vote(false, false, true)] event + [Reset] event
          case 3:
            ovA->getDataBox().setData(false);
            ovB->getDataBox().setData(false);
            ovC->getDataBox().setData(true);
            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \tA,B,C \t(* "
                "%s,%s,%s, "
                "\tline:%d *) \r\n",
                getName().c_str(), oeVote->getName().c_str(),
                false ? "true" : "false", false ? "true" : "false",
                true ? "true" : "false", __LINE__);
            generateEventOutput(*oeVote);

            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \t...... \t(* "
                "\t\t \tline:%d *) \r\n",
                getName().c_str(), oeReset->getName().c_str(), __LINE__);
            generateEventOutput(*oeReset);
            break;

          // [Vote(false, true, true)] event + [Reset] event
          case 4:
            ovA->getDataBox().setData(false);
            ovB->getDataBox().setData(true);
            ovC->getDataBox().setData(true);
            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \tA,B,C \t(* "
                "%s,%s,%s, "
                "\tline:%d *) \r\n",
                getName().c_str(), oeVote->getName().c_str(),
                false ? "true" : "false", true ? "true" : "false",
                true ? "true" : "false", __LINE__);
            generateEventOutput(*oeVote);

            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \t...... \t(* "
                "\t\t \tline:%d *) \r\n",
                getName().c_str(), oeReset->getName().c_str(), __LINE__);
            generateEventOutput(*oeReset);
            break;

          // [Vote(false, true, true)] event + [Reset] event
          case 5:
            ovA->getDataBox().setData(false);
            ovB->getDataBox().setData(true);
            ovC->getDataBox().setData(true);
            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \tA,B,C \t(* "
                "%s,%s,%s, "
                "\tline:%d *) \r\n",
                getName().c_str(), oeVote->getName().c_str(),
                false ? "true" : "false", true ? "true" : "false",
                true ? "true" : "false", __LINE__);
            generateEventOutput(*oeVote);

            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \t...... \t(* "
                "\t\t \tline:%d *) \r\n",
                getName().c_str(), oeReset->getName().c_str(), __LINE__);
            generateEventOutput(*oeReset);
            break;

          // [Vote] event + [Vote] event + [Reset] event
          case 6:
            ovA->getDataBox().setData(false);
            ovB->getDataBox().setData(true);
            ovC->getDataBox().setData(true);
            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \tA,B,C \t(* "
                "%s,%s,%s, "
                "\tline:%d *) \r\n",
                getName().c_str(), oeVote->getName().c_str(),
                false ? "true" : "false", true ? "true" : "false",
                true ? "true" : "false", __LINE__);
            generateEventOutput(*oeVote);

            ovA->getDataBox().setData(false);
            ovB->getDataBox().setData(true);
            ovC->getDataBox().setData(true);
            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \tA,B,C \t(* "
                "%s,%s,%s, "
                "\tline:%d *) \r\n",
                getName().c_str(), oeVote->getName().c_str(),
                false ? "true" : "false", true ? "true" : "false",
                true ? "true" : "false", __LINE__);
            generateEventOutput(*oeVote);

            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \t...... \t(* "
                "\t\t \tline:%d *) \r\n",
                getName().c_str(), oeReset->getName().c_str(), __LINE__);
            generateEventOutput(*oeReset);
            break;

          // double [Reset] event
          // case 7:
          default:
            WB_OUT(
                "%s \t\tGenerate: \tEVENT_OUTPUT \t%s \t\tWITH \t...... \t(* "
                "\t\t \tline:%d *) \r\n",
                getName().c_str(), oeReset->getName().c_str(), __LINE__);
            generateEventOutput(*oeReset);
            break;
        }
      }
      lasttime = time;
      return true;
    }
    return false;
  }
};
const char* VoterDriverFBType::FB_TYPE_NAME = "VoterDriver";
const char* VoterDriverFBType::TV_STATUS = "Status";
const char* VoterDriverFBType::OV_A = "A";
const char* VoterDriverFBType::OV_B = "B";
const char* VoterDriverFBType::OV_C = "C";
const char* VoterDriverFBType::OE_VOTE = "Vote";
const char* VoterDriverFBType::OE_RESET = "Reset";