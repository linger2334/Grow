//
//  FlowerDescription.h
//  Grow-UI
//
//  Created by wlg on 11/14/14.
//
//

#ifndef __Grow_UI__FlowerDescription__
#define __Grow_UI__FlowerDescription__

#include <iostream>
#include "Macro.h"

class MyPageView;

class FlowerDescription : public Layer
{
public:
    static Scene* createScene(int index);
    static FlowerDescription* create(int index);
    
    bool init(int index);
    void createDescriptionPage(int index);
    
protected:
    MyPageView* _pageView;
};


#endif /* defined(__Grow_UI__FlowerDescription__) */
