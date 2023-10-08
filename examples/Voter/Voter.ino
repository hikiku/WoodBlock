// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#include <WoodBlock.h>

#include <ExampleCommon/VoterECCBasicFBType.h>
#include "VoterDiscarder.h"
#include "VoterDriver.h"

// For user extend data type
extern bool extend_check4ConnectDataType(unsigned int outDataType,
                                         unsigned int inDataType) {
  return false;
}

FBNetwork fbNetwork;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Serial.println("setup():enter...");

  fbNetwork.attachFBInstance(FBInstance::create<VoterDriverFBType>("Driver"));
  fbNetwork.attachFBInstance(FBInstance::create<VoterECCBasicFBType>("Voter"));
  fbNetwork.attachFBInstance(
      FBInstance::create<VoterDiscarderFBType>("Discarder"));
  {
    const char* outVariableNames[] = {VoterDriverFBType::OV_A,
                                      VoterDriverFBType::OV_B,
                                      VoterDriverFBType::OV_C};
    const char* inVariableNames[] = {VoterECCBasicFBType::IV_A,
                                     VoterECCBasicFBType::IV_B,
                                     VoterECCBasicFBType::IV_C};
    fbNetwork.connect("Driver", VoterDriverFBType::OE_VOTE, outVariableNames,
                      ARRAY_SIZE(outVariableNames), "Voter",
                      VoterECCBasicFBType::IE_VOTE, inVariableNames,
                      ARRAY_SIZE(inVariableNames));

    fbNetwork.connect("Driver", VoterDriverFBType::OE_RESET, {}, 0, "Voter",
                      VoterECCBasicFBType::IE_RESET, {}, 0);
  }

  {
    const char* outVariableNames[] = {VoterECCBasicFBType::OV_STATE};
    const char* inVariableNames[] = {VoterDiscarderFBType::IV_STATE};
    /*bool result =*/fbNetwork.connect(
        "Voter", VoterECCBasicFBType::OE_VOTED, outVariableNames,
        ARRAY_SIZE(outVariableNames), "Discarder",
        VoterDiscarderFBType::IE_VOTED, inVariableNames,
        ARRAY_SIZE(inVariableNames));
    // WB_LOGD("{Voter[Voted(State)]} => {Discarder[Voted(State)]}, result=%s",
    // result ? "true" : "false");
    /*result =*/fbNetwork.connect("Voter", VoterECCBasicFBType::OE_READY,
                                  outVariableNames,
                                  ARRAY_SIZE(outVariableNames), "Discarder",
                                  VoterDiscarderFBType::IE_READY,
                                  inVariableNames, ARRAY_SIZE(inVariableNames));
    // WB_LOGD("{Voter[Ready(State)]} => {Discarder[Ready(State)]}, result=%s",
    // result ? "true" : "false");
  }
  // Serial.println("setup():exit...");
}

void loop() {
  // put your main code here, to run repeatedly:

  // Serial.println("loop():");
  fbNetwork.fetchExternalEvents();
  delay(1000);
}

// xtensa-esp32-elf-addr2line -pfiaC -e build/PROJECT.elf ADDRESS
// C:\Users\liang\AppData\Local\Arduino15\packages\esp32\tools\xtensa-esp32-elf-gcc\esp-2021r2-patch5-8.4.0\bin\xtensa-esp32-elf-addr2line
// -pfiaC -e
// C:\Users\liang\AppData\Local\Temp\arduino\sketches\80819CFA0703B283A9ABCB02D3F3B7DA\Voter.ino.elf
// 0x400d1ebc:0x3ffb2110 0x400d1eef:0x3ffb2150 0x400d1ce4:0x3ffb2170
// 0x400d1eaa:0x3ffb21b0 0x400d1eef:0x3ffb21f0 0x400d1ce4:0x3ffb2210
// 0x400d1d21:0x3ffb2250 0x400d23ed:0x3ffb2270 0x400d5fa9:0x3ffb2290