#include "LayerPlant.h"
#include "GameManager.h"
#include "ContorlPoint.h"
#include "LightNode.h"
#define MIN_WIDTH 40
#define MAX_WIDTH 100
#define HEAD_HEIGHT 80
#define MOVE_LENGTH 1
#define GROW_HEIGHT 8
#define HEAD_ADD  0.5
#define GROW_STEP 120

bool LayerPlant_1::init()
{
    GameLayerRollY::init();
    _leafBatch = SpriteBatchNode::create("plant1_leaf_1.png");
    addChild(_leafBatch);
    _plant = Plant_1::create();
    _plant->_crashTestCallBack = GameManager::getInstance()->_plantGrowCrashTestCallback;

    _plant->initPlantInfo(HeadCursorV2(ContorlPointV2(Vec2(0,0),80,0,0),80,80), 8);
    addChild(_plant);
//    _plant->_headCur.moveNext(100);
//    _plant->addHeadToPlantLine();
    _plant->setPosition(Vec2(Director::getInstance()->getWinSize().width*0.5f,0));
    
    _preDir = FaceDirection::FaceTop;
    _preGrowLen =0;
    _isNeedGrow =true;
//    for (int i = 0; i<5; i++) {
//        Vec2 v = Vec2(100,100+i*8);
//        _plant->_cpLineNode._cpList.push_back(ContorlPointV2(v,24,0,i*8));
//    }
//    auto p = *_plant->_cpLineNode._cpList.rbegin();
//  //  p.getNextContorPosition(<#float angle#>)
//    _plant->_cpLineNode._cpList.push_back(ContorlPointV2(Vec2(100,100+5*8),24,-30,5*8));
//    for (int i = 777; i<10; i++) {
//        Vec2 v = Vec2(100-i*8,100+5*8);
//        _plant->_cpLineNode._cpList.push_back(ContorlPointV2(v,24,-90,i*8));
//    }
    _leafTopHeight = _plant->_headCur._cp._height;
    _left =true;
    return true;
}

void  LayerPlant_1::update(float dt)
{
    if (!_isNeedGrow) return ;

    float growLen = GROW_STEP*dt;

    while (growLen>0) {
        growLen = grow(growLen);
        if(growLen>0)
        updateHead(dt);
    }
    
}
void  LayerPlant_1::checkAddLeaf()
{
   
    float randlen = rand()%200+100;
    if (_plant->_headCur._cp._height - _leafTopHeight > randlen) {
        Sprite* sp = Sprite::createWithTexture(_leafBatch->getTexture());
        _leafBatch->addChild(sp);
       
        
        Vec2 pt = _plant->convertToWorldSpace(_plant->_headCur._cp._point);
        sp->setPosition(pt);
       // int count = _leafBatch->getChildrenCount();
        sp->setScale(0.0);
        float angle;
        if(_left)
        { sp->setAnchorPoint(Vec2(1,0.5));
            sp->setFlippedX(true);
            angle =  _plant->_headCur._cp._angle + 22 - rand()%44;
            _left =false;
        }
        else
        {
             sp->setAnchorPoint(Vec2(0,0.5));
            angle =   _plant->_headCur._cp._angle + 22 -rand()%44;
            _left = true;
        }
        //ScaleTo* scale = ScaleTo::create(0.5, 0.1);
        //sp->runAction(scale);
        sp->setRotation(angle);
       
        _leafTopHeight = _plant->_headCur._cp._height;
       // int n =_leafTopHeight;
        sp->setUserData((void*)(int)_leafTopHeight);
    
    }
}
#define MINGROW_LEN 16
float LayerPlant_1::grow(float len)
{
    float retLen = -1;
    auto grow = _plant->getCanGrowContext();
    FaceDirection dir = _plant->_headCur.getFaceDirection();
    FaceDirection trnDir;
    bool isGrow = true;
    if (_preGrowLen < MINGROW_LEN ) {
    if (_preDir == FaceDirection::FaceTop) {
         if (grow._top){trnDir=FaceDirection::FaceTop;}
         else isGrow =false;
    }
    else if (_preDir == FaceDirection::FaceLeft) {
        if (grow._left) {trnDir=FaceDirection::FaceLeft;}
        else isGrow =false;
    }
    else if (_preDir == FaceDirection::FaceRight) {
        if (grow._right) {trnDir=FaceDirection::FaceRight;}
        else isGrow =false;
    }
 
        
    }
    else isGrow =false;
    if (!isGrow) {
        isGrow = true;
    if (dir == FaceDirection::FaceTop)
    {
        if (grow._top) {trnDir=FaceDirection::FaceTop;}
        else
        {
            bool testCrash = true;
            int testCount = 5;
            int minCount = MIN_WIDTH/8;
            for (int i = 0; i < 2 ; i++) {
                int lcount = _plant->getHeadTopCrashGridCount(-testCount,testCrash);
                int rcount = _plant->getHeadTopCrashGridCount(testCount,testCrash);
                if (testCrash&& lcount < rcount)
                {
                    trnDir = FaceDirection::FaceLeft;
                    break;
                }
                else if (testCrash && lcount  > rcount)
                {
                    trnDir = FaceDirection::FaceRight;
                    break;
                }
                else  if (!testCrash&& lcount < rcount && rcount >= minCount)
                {
                    trnDir = FaceDirection::FaceRight;
                    break;
                }
                else if (!testCrash && lcount  > rcount && lcount >= minCount)
                {
                    trnDir = FaceDirection::FaceLeft;
                    break;
                }
                else if (testCrash && lcount == rcount && lcount == 0)
                {
                    testCount=20;
                    testCrash = false;
                }
                else isGrow =false;
            }
        }
    }
    else if (dir == FaceDirection::FaceLeft)
    {
        if (grow._right) {trnDir=FaceDirection::FaceRight;}
        else if (grow._top){trnDir=FaceDirection::FaceTop;}
        else if(grow._left){trnDir=FaceDirection::FaceLeft;}
        else isGrow =false;
    }
    else if (dir == FaceDirection::FaceRight)
    {
        if (grow._left) {trnDir=FaceDirection::FaceLeft;}
        else  if (grow._top){trnDir=FaceDirection::FaceTop;}
        else if(grow._right){trnDir=FaceDirection::FaceRight;}
        else isGrow =false;
    }
        
    }
    
    if (isGrow) {

        if (trnDir!=_preDir) {
            _preDir = trnDir;
            _preGrowLen =0;
            _plant->turnHead(trnDir);
        }
        retLen =_plant->grow(trnDir, len);
        if(retLen>0)
        _preGrowLen += retLen;
        
        checkAddLeaf();
//        float len =_plant->_headCur._cp._height;
//        float testlen =_plant->_cpLineNode._texture->getContentSize().height*2;
//        if (len > testlen) {
//            _plant->checkHeightSub(testlen);
//            GameManager::getInstance()->getLightListHelper()->subItemHeight(testlen);
//            _leafTopHeight-=testlen;
//            checkLeafSubHeight(testlen);
//        }
    }
    else{

            _isNeedGrow =false;
        auto crash = _plant->testHeadCrash();
    }
    return retLen;
}

float LayerPlant_1::getPlantTopHeight()
{
    return  _plant->convertToWorldSpace(_plant->_headCur.getContorlPoint()._point).y;
}

void  LayerPlant_1::moveDown(float y)
{
    float oldY = _plant->getPositionY();
    float newY = oldY -y;
    if (newY < -300) {
        _plant->subPlantCP(200);
        newY = newY +200;
    }
    _plant->setPositionY(newY);
    std::list<Node*> removeList ;
    auto list = _leafBatch->getChildren();
    for (auto i = list.begin();i!=list.end();i++) {
        (*i)->setPositionY((*i)->getPositionY()-y);
        Size wsize =Director::getInstance()->getWinSize();
        if (!(*i)->boundingBox().intersectsRect(Rect(0,0,wsize.width,wsize.height))) {
            removeList.push_back(*i);
        }
        
    }
    for (auto i:removeList ) {
        i->removeFromParent();
    }
    
}
 void   LayerPlant_1::checkLeafSubHeight(float _len)
{
    auto list = _leafBatch->getChildren();
    for (auto i = list.begin();i!=list.end();i++) {
        
        float height = (int)(*i)->getUserData();
        (*i)->setUserData((void*)(int)(height-=_len));
    }
}
void  LayerPlant_1::updateHead(float dt)
{
    auto rip = _plant->_cpLineNode._cpList.rbegin();
    auto rend =_plant->_cpLineNode._cpList.rend();
    
    if (rip ==rend) {
        return ;
    }
    float  topHeight  = rip->_height;
    while (rip!=rend) {
        if (topHeight - rip->_height <500) {
            rip->_radius = (topHeight - rip->_height)/1000 * 50.0f+30;
        }
        else break;

        rip++;
    }
 
    auto list = _leafBatch->getChildren();
    for (auto i = list.begin();i!=list.end();i++) {
        
        float height = (int)(*i)->getUserData();
        
        float scale = (topHeight - height)/600*0.3 + 0.1;
        (*i)->setScale(scale);
    }

    
}
