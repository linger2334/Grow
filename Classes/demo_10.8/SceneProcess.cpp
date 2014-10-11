#include "SceneProcess.h"



SceneProcess::SceneProcess(int index):m_numSp(100),m_loadedSp(0),_index(index)
{
    
}

bool SceneProcess::init()
{
    if(!Scene::init())
        return false;
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    _loadLabel = LabelTTF::create("Loading:","Arial",20);//创建显示Loading: 的label
    _loadLabel->setPosition(Point(visibleSize.width/2-30,visibleSize.height/2+30));
    this->addChild(_loadLabel,1);
    
    _percentLabel = LabelTTF::create("0%","Arial",20);//创建显示百分比的label
    _percentLabel->setPosition(Point(visibleSize.width/2+35,visibleSize.height/2+30));
    this->addChild(_percentLabel,2);
    
    auto loadBg = Sprite::create("sliderTrack2.png");//进程条的底图
    loadBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    //loadBg->setScale(5, 1);
    //loadBg->setContentSize(Size(500,13));
    this->addChild(loadBg,1);
    
//    _loadingBar = LoadingBar::create("CloseNormal.png");
//    _loadingBar->setScale9Enabled(true);
//    _loadingBar->setCapInsets(Rect(0, 0, 0, 0));
//    _loadingBar->setContentSize(Size(500, 13));
//    _loadingBar->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    
    loadProgress = ProgressTimer::create(Sprite::create("sliderProgress2.png"));//创建一个进程条
    //loadProgress->setScale(5,1);
    loadProgress->setBarChangeRate(Point(1,0));//设置进程条的变化速率
    loadProgress->setType(ProgressTimer::Type::BAR);//设置进程条的类型
    loadProgress->setMidpoint(Point(0,1));//设置进度的运动方向
    loadProgress->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    loadProgress->setPercentage(0.0f);//设置初始值为0
    this->addChild(loadProgress,2);
    
    
    return true;
}

void SceneProcess::onEnter()
{
    //加载n张图片，每加载完一张就调用回调函数:loadingCallback
    auto textureCache = Director::getInstance()->getTextureCache();
    
    for (int i = 0; i< m_numSp; ++i)
    {
        textureCache->addImageAsync("chooselevel.png",CC_CALLBACK_1(SceneProcess::loadingCallback,this));
    }
    //textureCache->addImageAsync("HelloWorld.png",CC_CALLBACK_1(SceneProcess::loadingCallback,this));
    //textureCache->addImageAsync("HelloWorld.png",CC_CALLBACK_1(SceneProcess::loadingCallback,this));
    //textureCache->addImageAsync("HelloWorld.png",CC_CALLBACK_1(SceneProcess::loadingCallback,this));
    //textureCache->addImageAsync("HelloWorld.png",CC_CALLBACK_1(SceneProcess::loadingCallback,this));
    //textureCache->addImageAsync("HelloWorld.png",CC_CALLBACK_1(SceneProcess::loadingCallback,this));
    //textureCache->addImageAsync("HelloWorld.png",CC_CALLBACK_1(SceneProcess::loadingCallback,this));

}

void SceneProcess::loadingCallback(Ref* pSender)
{
    log("m_loadedSp%d",m_loadedSp);
    ++m_loadedSp;//每进到这个函数一次，让m_loadedSp + 1
    
    char buf_str[16];
    sprintf(buf_str,"%d%%",(int)(((float)m_loadedSp / m_numSp) * 100),m_numSp);
    _percentLabel->setString(buf_str);//更新percentLabel的值
    
    float newPercent = 100 - ((float)m_numSp - (float)m_loadedSp)/((float)m_numSp/100);//计算进度条当前的百分比
    loadProgress->setPercentage(newPercent);//更新进度条
    //_loadingBar->setPercent(m_loadedSp);
    
    //图片加载完成后
    if(m_loadedSp == m_numSp)
    {
 //       auto scene = GameScene::createScene();
   //     Director::getInstance()->replaceScene(TransitionPageTurn::create(1,scene,true));
    }
}