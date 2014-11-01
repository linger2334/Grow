#ifndef __Grow_Demo__LightNode2__
#define __Grow_Demo__LightNode2__
#include "common.h"
#include "GameManager.h"
#include "ContorlPoint.h"
#define NODE_IMAGE "light_2.png"

#define MIN_OPACITY  125
#define MAX_OPACITY  255

#define  INIT_ACTION_TIME 1.0f

class LightNode2 : public Sprite
{
public:
    enum LightState
    {
        StateNeedInit,
        StateRunInitAction,
        StateNormal,
        StateStop
    };

    CREATE_FUNC(LightNode2);
    virtual  bool init();
    virtual void update(float dt);
    
    bool   initByTopHeight(float height);
    void   updatePosition(float dt);
    void   testUpdatePosition()
    {
        this->setPosition(_contorlPosion);
    }
    void  updateNodePosition(float time);
    
    void  runInitAction();
    bool  isNeedMove();
    void  updateZorder();
    void  resetRotate(bool all =true)
    {
        if(all)
        {
            _alen = rand()%30+40;
            _clen = rand()%20+20;
        }
        else
        {
            bool flag = rand()%100>50;
            if(flag) _alen = rand()%30+40;
            else  _clen = rand()%20+20;
        }
    }
    float  randRotateTime()
    {
        return  1.0/(rand()%10);
    }
    void  randRotateSpeed()
    {
          _rotateSpeed = (rand()%5+5)/100;
    }
    void  resetRotate(float alen,float clen)
    {
        _alen = alen;
        _clen = clen;
    }
    void lightNormalActionCallback(Node* node1);
    
    bool doInitState();
    
    bool doNormalState();
    
    bool doStopState();
    
    void  normalRuning(float dt);
    
    bool isStateNeedInit(){return _state == LightState::StateNeedInit;}
    bool isStateRunInitAction(){return _state == LightState::StateRunInitAction;}
    bool isStateNormal(){return _state == LightState::StateNormal;}
    
    void setStateNeedInit(){this->setOpacity(0);_state = LightState::StateNeedInit;}
    void setState( LightState state){_state = state;}
    Vec2  getPositionByTimeAndCP(float time,Vec2 contorlPoint);
    
    int         _id;
    float       _alen;
    float       _clen;
    float       _time;
    Vec2        _contorlPosion;
    bool        _isAutoMoving;
    float       _heightInPlant;
    float       _targetPosition;
    float       _speed;
    Node*       _plantNode;
    float       _hightMoveLen;
    float       _heightTop;
    float       _rotateSpeed;
    bool        _isClockwise;
    
    bool        _isMovingStartCP;
    std::vector<ContorlPointV2>* _cpList;
    bool        _isInit;
    Vec2        _initStartPosition;
    Vec2        _initEndPosition;
    LightState         _state;
};


#endif /* defined(__Grow_Demo__LightNode2__) */
