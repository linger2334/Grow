//
//  GuideNode.h
//  Grow-LevelEdit
//
//  Created by 林纲 王 on 14-12-6.
//
//

#ifndef __Grow_LevelEdit__GuideNode__
#define __Grow_LevelEdit__GuideNode__

#include <stdio.h>
#include "Macro.h"
#include "GuideManager.h"

class GuideNode : public ClippingNode,public GuideComponentDelegate
{
public:
    virtual ~GuideNode() { }
    virtual void guideProcess(GuideInfo& guidePhase) { }
    virtual void guideClear() { }
    virtual void onNextStep();
    void onExit();
};

class StrokeDirt : public GuideNode
{
public:
    CREATE_FUNC(StrokeDirt)
    bool init();
    
    void guideProcess(GuideInfo& guidePhase);
};

#endif /* defined(__Grow_LevelEdit__GuideNode__) */
