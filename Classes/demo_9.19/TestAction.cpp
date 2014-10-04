#include "TestAction.h"

//
//TuoyuanBy
//
LRTuoyuanBy* LRTuoyuanBy::actionWithDuration(ccTime t, const lrTuoyuanConfig& c)//利用之前定义的椭圆的三个参数初始化椭圆
{
    LRTuoyuanBy *pTuoyuanBy = new LRTuoyuanBy();
    pTuoyuanBy->initWithDuration(t, c);
    pTuoyuanBy->autorelease();
    
    return pTuoyuanBy;
}

bool LRTuoyuanBy::initWithDuration(ccTime t, const lrTuoyuanConfig& c)
{
    if (CCActionInterval::initWithDuration(t))
    {
        m_sConfig = c;
        return true;
    }
    
    return false;
}
void LRTuoyuanBy::update(ccTime time)
{
    if (_target)
    {
        //_target->setPosition(Vec2(rand()%30,rand()%30));
        cocos2d::Point s_startPosition = Vec2(0,0);// _target->getParent()->getPosition();//m_sConfig.centerPosition;//中心点坐标
        float a = m_sConfig.aLength;
        float bx = s_startPosition.x;
        float by = s_startPosition.y;
        float c = m_sConfig.cLength;
        
        float x = tuoyuanXat(a, bx, c, time);//调用之前的坐标计算函数来计算出坐标值
        float y = tuoyuanYat(a, by, c, time);
        Vec2 po = ccpAdd( s_startPosition, ccp(x-a, y));
        _target->setPosition(po);//由于我们画计算出的椭圆你做值是以原点为中心的，所以需要加上我们设定的中心点坐标
    }
}