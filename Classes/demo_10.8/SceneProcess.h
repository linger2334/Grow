

#ifndef __GrowUp__SceneProcess__
#define __GrowUp__SceneProcess__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;


class SceneProcess :public Scene{
    
    
public:
    
    SceneProcess(int);//重新创建SceneProcess以便传入index

    static SceneProcess* create(int index)
    {
        SceneProcess *pRet = new SceneProcess(index);
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }
    
    bool init();

    void loadingCallback(Ref* pSender);//加载一张图片完成后跳转的毁掉函数
    
    void onEnter();
    
    private :
    LoadingBar * _loadingBar;
    cocos2d::ProgressTimer* loadProgress;
    
    cocos2d::LabelTTF* _percentLabel;//加载进度label
    cocos2d::LabelTTF* _loadLabel;//显示 loading: 的label
    
    int m_numSp;//要加载的精灵数目,初始化为 20 张
    int m_loadedSp;//已加载的精灵数目
    int _index;
};

#endif