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
#include "ui/CocosGUI.h"
#include "Macro.h"
#include "MyScrollView.h"

class Myscrollview:public GrowUp::MyScrollView
{
public:
    static Myscrollview* create(Size size, Node* container = NULL);
    static Myscrollview* create();

    void deaccelerateScrolling(float dt) override;
    void setCallBackFunc(Node* target, SEL_CallFunc callfun);
    
    bool isEndScrolling;
protected:
    Node* m_callbackListener;
    SEL_CallFunc m_callback;
};


class AchievementUi:public Layer,public GrowUp::ScrollViewDelegate
{
public:
    AchievementUi();
    ~AchievementUi();
    
    static Scene* createScene();
    CREATE_FUNC(AchievementUi);

    bool init();
    void addFlowers();
    void addScrollview();
    void addMenuUI();
    void setBottomItemDisappear();
    
    void flowerClicked(Ref* sender);
    void popupSeedlingPage(int index);
    
    void update(float dt);
    inline float getIphonefactor(){
        float result = 0.0;
        if(Director::getInstance()->getWinSize().height<1000)
            result = 0.01*VisibleSize.height;
        if(Director::getInstance()->getWinSize().height>1024.0&&Director::getInstance()->getWinSize().height<1200.0)
            result = -0.016*VisibleSize.height;
        return result;
            }
    void adjustFlowerPos(MenuItemImage* flower,int index);
protected:
    Layer* m_container;
    Myscrollview* _scrollview;
    ValueVector _flowersInfo;
    cocos2d::ui::Layout* _seedlingPage;
};

//灰度
class  gray: public Layer
{
public:
    virtual bool init();
    CREATE_FUNC(gray);
    
};


#endif /* defined(__GrowUp__AchievementUi__) */
