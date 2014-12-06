//
//  ScreenGuide.h
//  Grow-UI
//
//  Created by wlg on 12/1/14.
//
//

#ifndef __Grow_UI__ScreenGuide__
#define __Grow_UI__ScreenGuide__

#include <stdio.h>
#include "Macro.h"
#include "GameLayerBase.h"
#include "GuideManager.h"

class ScreenGuide : public GameLayerBase,public GameLayerHelper<ScreenGuide>
{
public:
    CREATE_FUNC(ScreenGuide)
    bool init();
    
    void moveDown(float y);
    void update(float dt);
protected:
    GuideManager* _guideManager;
};


#endif /* defined(__Grow_UI__ScreenGuide__) */
