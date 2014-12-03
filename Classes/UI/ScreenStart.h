//
//  ScreenStart.h
//  Grow-UI
//
//  Created by wlg on 11/13/14.
//
//

#ifndef __Grow_UI__ScreenStart__
#define __Grow_UI__ScreenStart__

#include <iostream>
#include "Macro.h"

class ScreenStart : public Layer
{
public:
    static Scene* createScene();
    
    CREATE_FUNC(ScreenStart)
    bool init();
    void initBeginnerInfo();
};

#endif /* defined(__Grow_UI__ScreenStart__) */
