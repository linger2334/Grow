#include "TuoYuanRotateNode.h"


bool TuoYuanRotateNode::init()
{
    Node::init();
    _isRotate = false;
    return true;
}
void TuoYuanRotateNode::initOvalConfig(Vec2 centerPosition,float OneCycle,float a,float b,float isclock)
{
    _config.centerPosition =centerPosition;
    _config.a = a;
    _config.b = b;
    _config.moveInAnticlockwise = isclock;
    _timeNow = 0;
    _timeByOneCycle = OneCycle;
    _isRotate = false;
}
void TuoYuanRotateNode::update(float dt)
{
    if(_isRotate)
    {
        if(_config.moveInAnticlockwise)
        {
            _timeNow += dt;
            if (_timeNow >= _timeByOneCycle) {
                _timeNow = 0;
            }
        }
        else{
            _timeNow -= dt;
            if (_timeNow <= 0.005) {
                _timeNow = _timeByOneCycle;
            }
        }
    }
   updatePosition();
}
void   TuoYuanRotateNode::updatePosition()
{
    this->setPosition(getNowTimeTargetPosition());
}
void TuoYuanRotateNode::updateRotatePosition(float dt)
{
    this->setPosition(getNowTimeTargetPosition());
}