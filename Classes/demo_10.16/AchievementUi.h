//
//  AchievementUi.h
//  GrowUp
//
//  Created by wlg on 8/28/14.
//
//

#ifndef __GrowUp__AchievementUi__
#define __GrowUp__AchievementUi__

#include <iostream>
#include "cocos-ext.h"
#include "Macro.h"
#include "MyScrollView.h"

class Myscrollview:public GrowUp::MyScrollView
{
public:
    static Myscrollview* create( cocos2d::Size size, Node* container = NULL);
    static Myscrollview* create();

    void deaccelerateScrolling(float dt) override;
    void setCallBackFunc(Node* target, SEL_CallFunc callfun);
    
    Node* m_callbackListener;
    SEL_CallFunc m_callback;
};


class AchievementUi:public Layer,public GrowUp::ScrollViewDelegate
{
public:
    AchievementUi();
    ~AchievementUi();
    
    void onEnter();
    virtual void onEnterTransitionDidFinish();
    void onExit();
    
    static Scene* createScene();
    CREATE_FUNC(AchievementUi);

    bool init();
    void addFlowers();
    void addScrollview();
    void addMenuUI();
    void addListener();
    void setBottomItemDisappear();
    
    void back(Ref* sender);
    void flowerClicked(Ref* sender);
    virtual void scrollViewDidScroll(GrowUp::MyScrollView* view);
    
    Layer* m_container;
    Myscrollview* _scrollview;
    LayerColor* _graylayer;
};


#endif /* defined(__GrowUp__AchievementUi__) */
