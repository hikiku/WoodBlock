// FunctionBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino
#include <iterator>   //
#include <list>

#include <WoodBlock/Namespace.hpp>
#include <WoodBlock/Macro.h>

#include <WoodBlock/Core/FunctionBlock.h>


WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class FunctionBlockContainer {
 public:
  FunctionBlockContainer() : functionBlocks() {}
  ~FunctionBlockContainer() {
    std::list<FunctionBlock*> functionBlocks;
    for (std::list<FunctionBlock*>::iterator it = functionBlocks.begin();
         it != functionBlocks.end(); ++it) {
      delete *it;
    }
    functionBlocks.clear();
  }

  FunctionBlock* findFunctionBlockByName(const String& functionBlockName) {
    // std::list<FunctionBlock*> functionBlocks;
    for (std::list<FunctionBlock*>::iterator it = functionBlocks.begin();
         it != functionBlocks.end(); ++it) {
      if ((*it)->getName().equals(functionBlockName)) {
        return *it;
      }
    }

    return nullptr;
  }

  // Don't delete a managed FunctionBlock directly. Please call
  // unhostAndDeleteFunctionBlock()!
  bool hostFunctionBlock(FunctionBlock& functionBlock) {
    functionBlocks.push_back(&functionBlock);
    return true;
  }
  bool unhostAndDeleteFunctionBlock(const String& functionBlockName) {
    // std::list<FunctionBlock *> functionBlocks;
    for (std::list<FunctionBlock*>::iterator it = functionBlocks.begin();
         it != functionBlocks.end(); ++it) {
      if ((*it)->getName().equals(functionBlockName)) {
        delete *it;
        functionBlocks.erase(it);
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

    FunctionBlock* srcBlock = findFunctionBlockByName(srcBlockName);
    FunctionBlock* dstBlock = findFunctionBlockByName(destBlockName);
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
  std::list<FunctionBlock*> functionBlocks;
};

WOODBLOCK_END_PUBLIC_NAMESPACE