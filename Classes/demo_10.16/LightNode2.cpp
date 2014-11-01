#include "LightNode2.h"

#include "SceneGame.h"
#include "LayerPlant.h"
#define HEIGHT_STEP 60
static inline float tuoyuanXat( float a, float bx, float c, float t )//返回X坐标
{
    //参数方程
    return -a*cos(2*3.1415926*t)+a;
}
static inline float tuoyuanYat( float a, float by, float c, float t )//返回Y坐标
{
    float b = sqrt(powf(a, 2) - powf(c, 2));//因为之前定义的参数是焦距c而不是短半轴b，所以需要计算出b
    //参数方程
    return b*sin(2*3.1415926*t);
}
bool LightNode2::init()
{
    Sprite::initWithFile(NODE_IMAGE);
    _time = 1.0f;
    _speed =1;
    this->setZOrder(40);
    _isClockwise = rand()%100>50;
    _rotateSpeed = (rand()%5+5.0f)/100.0f;
    this->_heightInPlant = 0;
    _isInit =false;
    _isMovingStartCP = true;
    
   // BlendFunc func = {GL_ONE,GL_ONE};
    //setBlendFunc(func);
    resetRotate(true);
    setStateNeedInit();
    this->setOpacity(0);
    return true;
}
 void   LightNode2::normalRuning(float dt)
{
    if (!_isClockwise) {
        _time-= _rotateSpeed *dt;
        if(_time <0)
        {
            _isClockwise = rand()%100>50;
            _time = _isClockwise?0:1;
        }
    }
    else if (_isClockwise)
    {
        _time+= _rotateSpeed *dt;
        if(_time > 1)
        {
            _isClockwise = rand()%100>50;
            _time = _isClockwise?1:0;
        }
    }
    updateNodePosition(_time);
    
    if (!isNeedMove()) {
        _speed=0;
        return ;
    }
    updatePosition(dt*_speed);
    _speed+= dt*(rand()%10+5);
    //_speed*= 1.3;
    if (_speed >= 300) {
        _speed= 300;
    }
}
void LightNode2::update(float dt)
{
    
    if(isStateNeedInit())
    {
         doInitState();
        return ;
    }
    else if(isStateNormal())
    {
        if(!_isAutoMoving){return ;}
        normalRuning(dt);
    }

}
bool  LightNode2::isNeedMove()
{
    auto  ip = _cpList->rbegin();
    auto  end = _cpList->rend();
    if (ip==end) {return false;}
    
    auto preip =ip;
    while (ip != end) {
        if ((int)ip->_height == (int)_heightInPlant) {
            break;
        }
        ip++;
    }
    if (ip == end) {   setStateNeedInit(); return false;}
    if (preip->_height - ip->_height <= _heightTop) { return false; }
    return true;
}
void LightNode2::updatePosition(float dt)
{
    float moveLen =dt;
    auto  ip = _cpList->rbegin();
    auto begin = ip;
    auto  end = _cpList->rend();
    if (ip==end) {return ;}
    while (ip != end) {
        if ((int)ip->_height == (int)_heightInPlant) {
            break;
        }
        ip++;
    }
    if (ip == end) { return;}
    ip--;
    float tlen=0;
    Vec2 vnode = this->_contorlPosion;
    float heig = _heightInPlant;
    while (ip != begin) {
        Vec2 vt = _plantNode->convertToWorldSpace(ip->_point);
        float len = vnode.getDistance(vt);
        
        if (tlen + len >= moveLen) {
            _heightInPlant = heig;
            float  nmovelen = moveLen - tlen;
            Vec2 vmove = vt - vnode;
            float tt = nmovelen / len;
            vmove *= tt;
            vnode += vmove;
            this->_contorlPosion=vnode;
            break;
        }
        tlen += len;
        ip--;
        vnode = vt;
        heig = ip->_height;
    }
}
Vec2  LightNode2::getPositionByTimeAndCP(float time,Vec2 contorlPoint)
{
    Vec2 s_startPosition = contorlPoint;
    float a = _alen;
    float bx = s_startPosition.x;
    float by = s_startPosition.y;
    float c = _clen;
    float x = tuoyuanXat(a, bx, c, time);
    float y = tuoyuanYat(a, by, c, time);
    s_startPosition.add(Vec2(x-a, y));
    return s_startPosition;
}
void  LightNode2::updateNodePosition(float time)
{
    this->setPosition(getPositionByTimeAndCP(time , _contorlPosion));
}
bool LightNode2::initByTopHeight(float height)
{
    _heightTop =height;
    auto  ip = _cpList->rbegin();
    auto  end = _cpList->rend();
    if (ip==end) {return false;}
    
    auto top = _cpList->rbegin();
    while (ip!=end) {
        if (top->_height - ip->_height >= height) {break;}
        ip++;
    }
    if (ip!=end) {
        Vec2 nv = _plantNode->convertToWorldSpace(ip->_point);
        this->_heightInPlant =ip->_height;
        _contorlPosion = nv;
        
        return true;
    }
    else return false;
}

void  LightNode2::runInitAction()
{
    this->stopAllActions();
    this->setOpacity(0);
    FadeTo* fadeTo = FadeTo::create(INIT_ACTION_TIME,MIN_OPACITY);
    Vec2 pt = _initEndPosition - this->getPosition();
    MoveBy* moveTo = MoveBy::create(1.5,pt);
    
    auto call = CallFuncN::create([](Node* node)
                                  {
                                      static_cast<LightNode2*>(node)->doNormalState();
                                  });
//    ccBezierConfig v;
//    v.controlPoint_1 = this->getPosition();
//    v.controlPoint_2 =
//    v.endPosition = _initEndPosition;
//
//    v.controlPoint_1 = convertToNodeSpace(ccp(140, 200));
//    v.controlPoint_2 = convertToNodeSpace(ccp(320, 50));
//    v.endPosition = convertToNodeSpace(ccp(500, 300));
//    Action* action = BezierTo::create(3, v);
//    sprite->runAction(action);
    Sequence* seq = Sequence::create(fadeTo,moveTo,call,nullptr);
    
    this->runAction(seq);
    
}


bool LightNode2::doInitState()
{
    bool ret = initByTopHeight(_heightTop);
    if ( ret) {
        float time = randRotateTime();
        _time = time;
        Vec2 pt = getPositionByTimeAndCP(time , _contorlPosion);
        //this->updateNodePosition(time);
        
        this->setState(LightState::StateRunInitAction);
        _initEndPosition = pt;
        runInitAction();
     }
    return ret;
}
void LightNode2::lightNormalActionCallback(Node* node1)
{
    node1->stopAllActions();
    DelayTime* delayTime =DelayTime::create(rand()%20+3);
    FadeTo* in = FadeTo::create(rand()%4+1,MAX_OPACITY);
    FadeTo* out = FadeTo::create(rand()%4+1,MIN_OPACITY);
  
    //FadeTo* in2 = FadeTo::create(rand()%4+3,MAX_OPACITY);
    CallFuncN* call = CallFuncN::create(CC_CALLBACK_1(LightNode2::lightNormalActionCallback, this));
    Sequence* seq = Sequence::create(in,delayTime,out,call,NULL);
    node1->runAction(seq);
}
bool LightNode2::doNormalState()
{
    CallFuncN* call = CallFuncN::create(CC_CALLBACK_1(LightNode2::lightNormalActionCallback, this));
    this->runAction(call);
    this->setState(LightState::StateNormal);
}

bool LightNode2::doStopState()
{
    
}