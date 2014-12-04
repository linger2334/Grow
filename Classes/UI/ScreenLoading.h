//
//  ScreenLoading.h
//  Grow-UI
//
//  Created by 林纲 王 on 14-11-16.
//
//

#ifndef __Grow_UI__ScreenLoading__
#define __Grow_UI__ScreenLoading__

#include <stdio.h>
#include "Macro.h"

class ScreenLoading : public Scene
{
public:
    static ScreenLoading* createScene(int levelNum);
    bool init(int levelNum);
    
    virtual  void onEnterTransitionDidFinish();
    void loadingCallback(Texture2D* texture);
protected:
    ProgressTimer* _progressBar;
    int _levelNum;
};

#endif /* defined(__Grow_UI__ScreenLoading__) */
