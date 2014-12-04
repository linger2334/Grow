#ifndef __Grow_Beta_01__TuoYuanRotateNode__
#define __Grow_Beta_01__TuoYuanRotateNode__
#include <cocos2d.h>
using namespace cocos2d;

typedef struct OvalConfig {
    //中心点坐标
    Vec2 centerPosition;
    //椭圆a长半轴
    float a;
    //椭圆b短半轴
    float b;
    //是否逆时针运动
    bool moveInAnticlockwise;

} lOvalConfig;


class TuoYuanRotateNode : public Node
{
public:
    CREATE_FUNC(TuoYuanRotateNode);
    virtual bool init();
    virtual void update(float dt);
    
    bool   isRotate(){return _isRotate;}
    
    void   startRotate(){_isRotate = true;}
    void   stopRotate(){_isRotate = false;}
    
    void   initOvalConfig(Vec2 centerPosition,float OneCycle,float a,float b,float isclock=true);
    void   updateRotatePosition(float dt);
    void   resetOvalConfig(const OvalConfig&  config);
    
    float  getTime(){return _timeNow;}
    void   setTime(float value){_timeNow = value;}
   
    float  getALength(){return _config.a;}
    void   setALength(float value){_config.a = value;}
    
    float  getBLength(){return _config.b;}
    void   setBLength(float value){_config.b = value;}
    
    float  getOneCycleTime(){return _config.a;}
    void   setOneCycleTime(float value){_config.a = value;}
    
    Vec2   getContorlPosition(){return  _config.centerPosition;}
    void   setContorlPosition(Vec2 pt){_config.centerPosition = pt;}
    void   changeClockwise(){_config.moveInAnticlockwise = _config.moveInAnticlockwise == true ? false : true;}
    
    void   setClockwise(bool isclockwise)
    {_config.moveInAnticlockwise = isclockwise;}
    
    bool   isClockwise(){return _config.moveInAnticlockwise;}
    
    Vec2   getNowTimeTargetPosition()
    {
        return getTargetPositionByTime(_timeNow/_timeByOneCycle);
    }
    Vec2   getNowTimeTargetPositionInWorld()
    {
        Vec2 pt = getTargetPositionByTime(_timeNow/_timeByOneCycle);
        auto p = this->getParent();
        if(p)pt = p->convertToWorldSpace(pt);
        return pt;
    }
    inline Vec2 getTargetPositionByTime(float t)
    {
        float x = getPositionXAtOval(t);//调用之前的坐标计算函数来计算出坐标值
        float y = getPositionYAtOval(t);
        return _config.centerPosition+ Vec2(x,y);
    }
    
    //x = a * cos(t)  t = [0, 2Pi]
    inline float getPositionXAtOval(float t ){//返回X坐标
        //参数方程
        if(_config.moveInAnticlockwise == false){
            return _config.a * cos(6.2831852 * (1 - t));
        }else{
            return _config.a * cos(6.2831852 * t);
        }
        
    }
    
    //y = b * sin(t)  t = [0, 2Pi]
    inline float getPositionYAtOval(float t ){//返回Y坐标
        //参数方程
        if(_config.moveInAnticlockwise == false){
            return _config.b * sin(6.2831852 * (1 - t));
        }else{
            return _config.b * sin(6.2831852 * t);
        }
    }
    
    bool       _isRotate;
    
    //单位秒
    float      _timeNow;
    //单位秒
    float      _timeByOneCycle;
    OvalConfig _config;
};

#endif /* defined(__Grow_Beta_01__TuoYuanRotateNode__) */
