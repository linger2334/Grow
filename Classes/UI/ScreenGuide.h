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

class ScreenGuide : public Layer
{
public:
    CREATE_FUNC(ScreenGuide)
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
};

#endif /* defined(__Grow_UI__ScreenGuide__) */
