// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#ifndef WoodBlockContainer_class_h
#define WoodBlockContainer_class_h
#ifdef __cplusplus

#include <list>
#include <iterator> //

#include <WString.h>

#include "WoodMacro.h"
#include "WoodBlock.h"

// class WoodBlock;

class WoodBlockContainer
{
public:
    WoodBlockContainer() : woodBlocks() {}
    ~WoodBlockContainer()
    {
        std::list<WoodBlock*> woodBlocks;
        for (std::list<WoodBlock*>::iterator it = woodBlocks.begin(); it != woodBlocks.end(); ++it)
        {
            delete *it;
        }
        woodBlocks.clear();
    }

    WoodBlock *findWoodBlockByName(const String &woodBlockName)
    {
        // std::list<WoodBlock*> woodBlocks;
        for (std::list<WoodBlock*>::iterator it = woodBlocks.begin(); it != woodBlocks.end(); ++it)
        {

            if ((*it)->getName().equals(woodBlockName))
            {
                return *it;
            }
        }

        return nullptr;
    }

    // Don't delete a managed WoodBlock directly. Please call unhostAndDeleteWoodBlock()!
    bool hostWoodBlock(WoodBlock &woodBlock)
    {
        woodBlocks.push_back(&woodBlock);
        return true;
    }
    bool unhostAndDeleteWoodBlock(const String &woodBlockName)
    {
        // std::list<WoodBlock *> woodBlocks;
        for (std::list<WoodBlock *>::iterator it = woodBlocks.begin(); it != woodBlocks.end(); ++it)
        {
            if ((*it)->getName().equals(woodBlockName))
            {
                delete *it;
                woodBlocks.erase(it);
                return true;
            }
        }

        return false;
    }

    bool connect(const char *srcBlockName, const char *outEventName, const char *outVariableNames[], int sizeofOutVariables,
                 const char *destBlockName, const char *inEventName, const char *inVariableNames[], int sizeofInVariables)
    {
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

        WoodBlock *srcBlock = findWoodBlockByName(srcBlockName);
        WoodBlock *dstBlock = findWoodBlockByName(destBlockName);
        if (srcBlock == nullptr)
        {
            // TODO: printf(WARNING, "srcBlock is nullptr!");
            return false;
        }
        if (dstBlock == nullptr)
        {
            // TODO: printf(WARNING, "dstBlock is nullptr!");
            return false;
        }
        bool result = srcBlock->connectTo(outEventName, outVariableNames,
                                          *dstBlock, inEventName, inVariableNames, sizeofInVariables);
        if (!result)
        {
            // TODO: printf(HINT, "connectTo() is false!");
            return false;
        }
        return true;
    }

private:
    std::list<WoodBlock*> woodBlocks;
};

#endif // __cplusplus
#endif // WoodBlockContainer