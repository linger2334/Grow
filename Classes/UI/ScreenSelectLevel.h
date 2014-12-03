//
//  ScreenSelectLevel.h
//  Grow-UI
//
//  Created by 林纲 王 on 14-11-16.
//
//

#ifndef __Grow_UI__ScreenSelectLevel__
#define __Grow_UI__ScreenSelectLevel__

#include <stdio.h>
#include "Macro.h"
#include "ui/CocosGUI.h"

class  MyPageView: public ui::PageView
{
public:
    CREATE_FUNC(MyPageView);
    virtual void handleReleaseLogic(Touch *touch);
};

class ScreenSelectLevel : public Scene
{
public:
    CREATE_FUNC(ScreenSelectLevel)
    bool init();
    
    void addPageView();
    void addPointIndicator();
    void addMenuUI();
protected:
    MyPageView* _pageView;
    ui::ImageView* _point;
    std::vector<Vec2> _pointPos;
};

#endif /* defined(__Grow_UI__ScreenSelectLevel__) */
