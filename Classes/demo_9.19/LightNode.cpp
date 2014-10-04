#include "LightNode.h"
#include "SceneGame.h"
#include "LayerPlant.h"

#define HEIGHT_STEP 60

bool LightNode::init()
{
    Node::init();

    return true;
}

void LightNode::update(float dt)
{
    
    if (!_plantNode) {
        return;
    }
    auto top = _cpList->rbegin();
    if (top == _cpList->rend()) return ;
    float topHeight = top->_height;
    if ( _lightHeight >= topHeight - _plantHeight)
    {
        _speedHeight = 10;
        return;
    }
    _lightHeight += _speedHeight *dt;//_speedHeight*dt;
    if ( _lightHeight >= topHeight - _plantHeight)
    {
        _lightHeight  = topHeight - _plantHeight;
    }

    _speedHeight += 1.2*dt;
    updatePosition();
}
void LightNode::updatePosition()
{
    auto ip = _cpList->rbegin();
    auto end = _cpList->rend();

    int count = 0;
    while (ip!=end) {
        if (ip->_height<=_lightHeight) {
            break;
        }
        count++;
        ip++;
    }
    if (--count <= 1) {
        return;
    }
    if(ip == end) return;
    Vec2 newPosition;
    if (ip!=end) {
        auto tip = ip-1;
        
        Vec2 bv , tv;
        switch(_positionType)
        {
            case CPCenter:
                bv = ip->_point;
                tv = tip->_point;
                break;
            case CPLeft:
                bv = ip->getPositionRightByLength(_rotateLen);
                tv = tip->getPositionRightByLength(_rotateLen);
                break;
            case CPRight:
                bv = ip->getPositionLeftByLength(_rotateLen);
                tv = tip->getPositionLeftByLength(_rotateLen);
                break;
        }
        bv =  _plantNode->convertToWorldSpace(bv);
        tv =   _plantNode->convertToWorldSpace(tv);
        Vec2 moveV = tv-bv;
        newPosition = bv +  moveV;
        log("height %f",_lightHeight);
    }
    else return;
    this->setPosition(newPosition);
}

void LightNodeListHelper::addLight(Node* node)
{
    
    _lightList.push_back(LigthListNode(node,0));
    node->setPosition(Vec2(300,-100));
    
    _sceneGame->addChild(node,888);
    node->setVisible(false);

}
void LightNodeListHelper::removeLight(Node* node)
{
    auto ip = std::find(_lightList.begin(),_lightList.end(),node);
    
    if(ip != _lightList.end())
    {
        ip->_node->removeFromParent();
        _lightList.erase(ip);
    }
}

void LightNodeListHelper::subItemHeight(float len)
{
    for (auto& i:_lightList ) {
        
        i._height-= len;
    }
}
void LightNodeListHelper::update(float dt)
{
    _lightAddSpeedStep+=2*dt;
    _lightAddSpeedStep*=2;
    updateLight();
    
}
void LightNodeListHelper::updateLight()
{
    auto& list = GameManager::getInstance()->getLayerPlant()->_plant->_cpLineNode._cpList;
 
    float topHeight = list.rbegin()->_height;

    int index =0;
    int moveLightCount =0;
    for (auto& i:_lightList)
    {
        auto ip =list.begin();
        auto end = list.end();
        if( i._isRuningAction)
        {
            index++;
            moveLightCount++;
            continue;
        }
        if (i._height<(topHeight-index*HEIGHT_STEP)) {
            
            bool flag =false;
            while(ip!=end)
            {
                if (ip->_height>topHeight-index*HEIGHT_STEP) {
                    flag =true;
                    break;
                }
                if(ip->_height>i._height)
                    break;
                ip++;
            }
            if(ip == end || flag)break;
           
            Vec2 pt = index%2==0? ip->getPositionLeftByLength(40) : ip->getPositionRightByLength(40);
            i._node->setVisible(true);
            Vec2 ptNew =GameManager::getInstance()->getLayerPlant()->_plant->convertToWorldSpace( pt);
            Vec2 ptOld = i._node->getPosition();
            Vec2 ptmove = ptNew - ptOld;
            float len =ptOld.distance(ptNew);
            float dt ;
            dt = len /_lightAddSpeedStep;
            CallFuncN* call = CallFuncN::create([&](Node* node1)
                {
                     auto ip1 = std::find(_lightList.begin(),_lightList.end(),node1);
                    if (ip1!=_lightList.end()) {
                        ip1->_isRuningAction=false;
                        
                    }
                });
            MoveBy* move = MoveBy::create(dt, ptmove);
            i._node->runAction(Sequence::create(move,call,nullptr));
            i._isRuningAction = true;
            i._height = ip->_height;
            
             moveLightCount++;
 
        }
        index++;
    }
    if (moveLightCount ==0) {
        _lightAddSpeedStep = 2;
    }
}