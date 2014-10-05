//
//  Macro.h
//  GrowUp
//
//  Created by wlg on 14-8-11.
//
//

#ifndef GrowUp_Macro_h
#define GrowUp_Macro_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameConstants.h"
//#include "editor-support/cocostudio/CCSGUIReader.h"
//#include "cocostudio/CocoStudio.h"
//#include "ui/CocosGUI.h"
//using namespace cocos2d::ui;
//using namespace cocostudio;

USING_NS_CC;
USING_NS_CC_EXT;

#define WinSize cocos2d::Size(768,1024) //设计分辨率，绘制区，可能包含空白
#define VisibleSize Director::getInstance()->getVisibleSize()  //设计分辨率可视区域大小，绘制区的可见部分
#define VisibleOrigin Director::getInstance()->getVisibleOrigin()  //可见区 原点

#define PTM_RATIO 32
#define PAGE_COUNTS 10.000
#define DEFAULT_ANGLE 0.0
#define DEFAULT_SCALE 1.000

#define cc_to_b2Vec(x,y) (b2Vec2((x)/PTM_RATIO,(y)/PTM_RATIO))

enum Item_Type{
    Flame_Red,
    Flame_Green,
    Flame_Blue,
    Flame_White,
    Flame_Orange,
    Rock_Circle,
    Rock_Ellipse,
    Rock_Gray,
    Cicada,
    Dragon_Anti,
    Dragon_Clockwise,
    Eye,
    Polygon
};


#define IMAGE_FLAME_RED "Flame_Red.png"
#define IMAGE_FLAME_GREEN "Flame_Green.png"
#define IMAGE_FLAME_BLUE "Flame_Blue.png"
#define IMAGE_FLAME_WHITE "Flame_White.png"
#define IMAGE_FLAME_ORANGE "Flame_Orange.png"
#define IMAGE_ROCK_CIRCLE "Rock_Circle.png"
#define IMAGE_ROCK_ELLIPSE "Rock_Ellipse.png"
#define IMAGE_ROCK_GRAY "Rock_Gray.png"
#define IMAGE_CICADA "Cicada.png"
#define IMAGE_DRAGON_ANTI "Dragon_Anti.png"
#define IMAGE_DRAGON_CLOCKWISE "Dragon_Clockwise.png"
#define IMAGE_EYE "Eye.png"

#define FLAME_RED_WIDTH 120
#define FLAME_RED_HEIGHT 120
#define FLAME_GREEN_WIDTH 120
#define FLAME_GREEN_HEIGHT 120
#define FLAME_BLUE_WIDTH 120
#define FLAME_BLUE_HEIGHT 120
#define FLAME_WHITE_WIDTH 60
#define FLAME_WHITE_HEIGHT 73
#define FLAME_ORANGE_WIDTH 120
#define FLAME_ORANGE_HEIGHT 120
#define ROCK_CIRCLE_WIDTH 303
#define ROCK_CIRCLE_HEIGHT 302
#define ROCK_ELLIPSE_WIDTH 117
#define ROCK_ELLIPSE_HEIGHT 266
#define ROCK_GRAY_WIDTH 64
#define ROCK_GRAY_HEIGHT 64
#define CICADA_WIDTH 83
#define CICADA_HEIGHT 132
#define DRAGON_ANTI_WIDTH 191
#define DRAGON_ANTI_HEIGHT 207
#define DRAGON_CLOCKWISE_WIDTH 191
#define DRAGON_CLOCKWISE_HEIGHT 207
#define EYE_WIDTH 29
#define EYE_HEIGHT 17


#define IMAGE_EDITOR_ADDITEM_NORMAL "addItem_normal.png"
#define IMAGE_EDITOR_ADDITEM_SELECTED "addItem_addItem_selected.png"
#define IMAGE_EDITOR_SAVE_NORMAL "save_normal.png"
#define IMAGE_EDITOR_SAVE_SELECTED "save_selected.png"
#define IMAGE_EDITOR_RESET_NORMAL "reset_normal.png"
#define IMAGE_EDITOR_RESET_SELECTED "reset_selected.png"
#define IMAGE_EDITOR_COPY_NORMAL "copy_normal.png"
#define IMAGE_EDITOR_COPY_SELECTED "copy_selected.png"
#define IMAGE_EDITOR_DELETE_NORMAL "delete_normal.png"
#define IMAGE_EDITOR_DELETE_SELECTED "delete_selected.png"
#define IMAGE_EDITOR_PLAY_NORMAL "play_normal.png"
#define IMAGE_EDITOR_PLAY_SELECTED "play_selected.png"
#define IMAGE_EDITOR_HIDE_NORMAL "hide_normal.png"
#define IMAGE_EDITOR_HIDE_SELECTED "hide_selected.png"


#define IMAGE_MATERIAL_DIRT "dirt512_2.png"


#endif
