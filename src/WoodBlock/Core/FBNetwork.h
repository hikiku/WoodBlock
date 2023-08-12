// FBInstance - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino
#include <iterator>   //
#include <list>

#include <WoodBlock/Macro.h>
#include <WoodBlock/Namespace.hpp>

#include <WoodBlock/Core/FBInstance.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class FBNetwork {
 public:
  FBNetwork() : fbInstances() {}
  ~FBNetwork() {
    std::list<FBInstance*> fbInstances;
    for (std::list<FBInstance*>::iterator it = fbInstances.begin();
         it != fbInstances.end(); ++it) {
      delete *it;
    }
    fbInstances.clear();
  }

  FBInstance* findFbInstanceByName(const String& fbInstanceName) {
    // std::list<FBInstance*> fbInstances;
    for (std::list<FBInstance*>::iterator it = fbInstances.begin();
         it != fbInstances.end(); ++it) {
      if ((*it)->getName().equals(fbInstanceName)) {
        return *it;
      }
    }

    return nullptr;
  }

  // Don't delete a managed FBInstance directly. Please call
  // detachAndDeleteFbInstance()!
  bool attachFbInstance(FBInstance& functionBlock) {
    fbInstances.push_back(&functionBlock);
    return true;
  }
  bool detachAndDeleteFbInstance(const String& fbInstanceName) {
    // std::list<FBInstance *> fbInstances;
    for (std::list<FBInstance*>::iterator it = fbInstances.begin();
         it != fbInstances.end(); ++it) {
      if ((*it)->getName().equals(fbInstanceName)) {
        delete *it;
        fbInstances.erase(it);
        return true;
      }
    }

    return false;
  }

  bool connect(const char* srcBlockName, const char* outEventName,
               const char* outVariableNames[], int sizeofOutVariables,
               const char* destBlockName, const char* inEventName,
               const char* inVariableNames[], int sizeofInVariables) {
    // check
    WB_CHECK_PTR_RETURN_VALUE(srcBlockName, false);
    WB_CHECK_PTR_RETURN_VALUE(outEventName, false);
    WB_CHECK_PTR_RETURN_VALUE(outVariableNames, false);
    WB_CHECK_EXP_RETURN_VALUE((sizeofOutVariables <= 0), false);
    WB_CHECK_PTR_RETURN_VALUE(destBlockName, false);
    WB_CHECK_PTR_RETURN_VALUE(inEventName, false);
    WB_CHECK_PTR_RETURN_VALUE(inVariableNames, false);
    WB_CHECK_EXP_RETURN_VALUE((sizeofInVariables <= 0), false);
    WB_CHECK_EXP_RETURN_VALUE((sizeofOutVariables != sizeofInVariables), false);

    FBInstance* srcBlock = findFbInstanceByName(srcBlockName);
    FBInstance* dstBlock = findFbInstanceByName(destBlockName);
    if (srcBlock == nullptr) {
      // TODO: printf(WARNING, "srcBlock is nullptr!");
      return false;
    }
    if (dstBlock == nullptr) {
      // TODO: printf(WARNING, "dstBlock is nullptr!");
      return false;
    }
    bool result =
        srcBlock->connectTo(outEventName, outVariableNames, *dstBlock,
                            inEventName, inVariableNames, sizeofInVariables);
    if (!result) {
      // TODO: printf(HINT, "connectTo() is false!");
      return false;
    }
    return true;
  }

 private:
  std::list<FBInstance*> fbInstances;

  // TODO: std::list<DataConnection> dataConnections;
  // TODO: std::List<EventConnection> eventConnections;

  // TODO: std::List<AdapterConnection> adapterConnections
};

WOODBLOCK_END_PUBLIC_NAMESPACE