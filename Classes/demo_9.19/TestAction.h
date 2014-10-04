#ifndef __Grow_Demo__TestAction__
#define __Grow_Demo__TestAction__
#include "Macro.h"

typedef float ccTime ;
// 定义一个结构来包含确定椭圆的参数
typedef struct _lrTuoyuanConfig {
    //中心点坐标
    cocos2d::Point centerPosition;
    //椭圆a长，三角斜边
    float aLength;
    //椭圆c长，三角底边
    float cLength;
} lrTuoyuanConfig;
class  LRTuoyuanBy : public ActionInterval
{
public:
    //用“动作持续时间”和“椭圆控制参数”初始化动作
    bool initWithDuration(float t, const lrTuoyuanConfig& c);
    virtual void update(float time);//利用update函数来不断的设定坐标
public:
    //用“动作持续时间”和“椭圆控制参数”创建动作
    static LRTuoyuanBy *actionWithDuration(float t, const lrTuoyuanConfig& c);
    
    virtual ActionInterval* reverse() const {}
	virtual ActionInterval *clone() const {}
protected:
    lrTuoyuanConfig m_sConfig;
    cocos2d::Point m_startPosition;
    cocos2d::Point s_startPosition;
};

 inline float tuoyuanXat( float a, float bx, float c, float t )//返回X坐标
{
    //参数方程
    return -a*cos(2*3.1415926*t)+a;
}
 inline float tuoyuanYat( float a, float by, float c, float t )//返回Y坐标
{
    float b = sqrt(powf(a, 2) - powf(c, 2));//因为之前定义的参数是焦距c而不是短半轴b，所以需要计算出b
    //参数方程
    return b*sin(2*3.1415926*t);
}
#endif /* defined(__Grow_Demo__TestAction__) */
