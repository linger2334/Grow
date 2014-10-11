 #include "LayerPlant.h"
#include "GameManager.h"
#include "ContorlPoint.h"
#include "LightNode.h"
#include "LayerLight.h"

#define MIN_WIDTH 20
#define MAX_WIDTH 70
#define HEAD_HEIGHT 180
#define MOVE_LENGTH 1
#define GROW_HEIGHT 1
#define HEAD_ADD  0.5
#define GROW_STEP 60
#define MINGROW_LEN 5
#define ROTATE_LENGTH 100

#define LEAF_RAND_LENGTH 100
#define LEAF_LENGHT      300

bool LayerPlant_1::init()
{
    GameLayerRollY::init();
    this->_crashBorderBatch = SpriteBatchNode::create("border_outlight.png");
    addChild(_crashBorderBatch);
    BlendFunc func = {GL_ONE,GL_ONE_MINUS_SRC_ALPHA};
     this->_crashBorderBatch->setBlendFunc(func);
    
    _leafBatch = SpriteBatchNode::create("plant1_leaf_1.png");
    addChild(_leafBatch);
    
    _lightSprite= Sprite::create("light_big.png");
    addChild(_lightSprite);
    _lightSprite->setVisible(false);
    
    _plant = Plant_1::create();
    _plant->_crashTestCallBack = GameManager::getInstance()->_plantGrowCrashTestCallback;
    //_plant->setVisible(false);
       _plant->initPlantInfo(HeadCursorV2(ContorlPointV2(Vec2(0,0),150,0,0),HEAD_HEIGHT,ROTATE_LENGTH),GROW_HEIGHT);
    addChild(_plant);
    //_plant->retain();
    _plant->setPosition(Vec2(Director::getInstance()->getWinSize().width*0.5f,0));
    
    _preDir = FaceDirection::FaceTop;
    _preGrowLen =0;
    _isNeedGrow =true;

    _leafTopHeight = _plant->_headCur._cp._height;
    _left =true;
    Size size = GameManager::getInstance()->getVisibleSize();
   // _render = RenderTexture::create(size.width, size.height);
    
   // addChild(_render);
  //  _render->setPosition(Vec2(size.width*0.5,size.height*0.5));
   // _render->addChild(_plant);
   // BlendFunc func = {GL_ONE,GL_ZERO};
   // _render->getSprite()->setBlendFunc(func);
   // _render->getSprite()->getTexture()->setAliasTexParameters();
   // _growSpeed = GROW_STEP;
    setGrowSpeed(GROW_STEP);
    _islightRuningAction=false;

    return true;
}
void LayerPlant_1::renderPlant()
{
    return;
   _plant->setVisible(true);
   _render->beginWithClear(0, 0, 0, 0);
   _plant->visit();
   _render->end();
   _plant->setVisible(false);
}
void LayerPlant_1::onEnter()
{
    GameLayerRollY::onEnter();
   // if(_plant)_plant->onEnter();
}
void LayerPlant_1::onExit()
{   GameLayerRollY::onExit();
   //if(_plant)_plant->onExit();
}
void  LayerPlant_1::update(float dt)
{
    //if (!_isNeedGrow) return ;
    float angle = fabs(_plant->_headCur.getAngle());

    _plant->_headCur._rotateLenght = 100- angle/2.0f;
    float growLen = _growSpeed*dt;
    bool isGrow =false;
    while (growLen>0) {
        growLen = grow(growLen);
        if(growLen >= 0)
        {
            isGrow =true;
         updateHead(dt);
        }
    }
    if (!isGrow && !_islightRuningAction)  {
        // _lightSprite->setPosition(_plant->convertToWorldSpace(_plant->_headCur.getPosition()));
        // float dt = _plant->_headCur.getAngle();
        // _lightSprite->setRotation(dt);
        //_lightSprite->setVisible(true);
        //_lightSprite->setOpacity(0);
        
        FadeTo* fadeto = FadeTo::create(5,100);
        FadeTo* fadeto1 = FadeTo::create(5,0);
        Sequence* seq = Sequence::create(fadeto, fadeto1,nullptr);
        _lightSprite->stopAllActions();
        _lightSprite->runAction(RepeatForever::create(seq));
        _islightRuningAction = true;
        showCrashBorderLight();
    }
    else if(isGrow)
    {_lightSprite->stopAllActions();_lightSprite->setVisible(false);
        _islightRuningAction=false;
       // _crashBorderBatch->removeAllChildren();
    }
    renderPlant();
}
void  LayerPlant_1::checkAddLeaf()
{
   
    float randlen = rand()%LEAF_RAND_LENGTH+LEAF_LENGHT;
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
            angle =  _plant->_headCur._cp._angle+30 + 22 - rand()%20;
            _left =false;
        }
        else
        {
             sp->setAnchorPoint(Vec2(0,0.5));
            angle =   _plant->_headCur._cp._angle-30 + 22 -rand()%20;
            _left = true;
        }
        //ScaleTo* scale = ScaleTo::create(0.5, 0.1);
        //sp->runAction(scale);
        float time = (_growSpeed - 30.0f)/ 210.0f;
        FadeIn* fadein = FadeIn::create(2- time*1.2);
        sp->setOpacity(0);
        sp->runAction(fadein);
        sp->setRotation(angle);
       
        _leafTopHeight = _plant->_headCur._cp._height;
       // int n =_leafTopHeight;
        sp->setUserData((void*)(int)_leafTopHeight);
    
    }
}

float LayerPlant_1::grow(float len)
{
    float retLen = -1;
    auto grow = _plant->getCanGrowContext();
    FaceDirection dir = _plant->_headCur.getFaceDirection();
    FaceDirection trnDir;
    bool isGrow = true;
    bool angle = fabs(_plant->_headCur.getAngle())<120;
    bool flagleft = true;
    if (_plant->_headCur.getAngle()<-40) {
     flagleft =_plant->convertToWorldSpace(_plant->_headCur.getPosition()).x>150;
    }
    
    bool flagGrowSelfOrTopDir = true;
    if (fabs(_plant->_headCur.getAngle())>90&& fabs(_plant->_headCur._cp._point.y - _plant->_headTopHeight)>50) {
        flagGrowSelfOrTopDir = false;
    }
    
  //  && _plant->_headCur.getAngle()>-70 ;
    
    bool flagright = true;
//    if (_plant->_headCur.getAngle()>40) {
//        flagright =_plant->convertToWorldSpace(_plant->_headCur.getPosition()).x<Director::getInstance()->getWinSize().width-150;
//    }
    
   // &&_plant->_headCur.getAngle()<70 ;
//    if (flagGrowSelfOrTopDir && _preGrowLen < MINGROW_LEN ) {
//    if (_preDir == FaceDirection::FaceTop) {
//         if (flagleft&&flagright&&grow._top){trnDir=FaceDirection::FaceTop;}
//         else isGrow =false;
//    }
//    else if (_preDir == FaceDirection::FaceLeft) {
//        if (flagleft&&grow._left) {trnDir=FaceDirection::FaceLeft;}
//        else isGrow =false;
//    }
//    else if (_preDir == FaceDirection::FaceRight) {
//        if (flagright&&grow._right) {trnDir=FaceDirection::FaceRight;}
//        else isGrow =false;
//    }
// 
//        
//    }
//    else isGrow =false;
     isGrow =false;
    if (!isGrow) {
        _preGrowLen = 0;
        isGrow = true;
    if (dir == FaceDirection::FaceTop)
    {
        
        if (grow._top) {trnDir=FaceDirection::FaceTop;}
        else
        {
            bool testCrash = true;
            int testCount = 5;
            int minCount = 4;
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
            if (!isGrow) {
//                if(_plant->convertToWorldSpace(_plant->_headCur.getPositionTLeft()).x<200&&
                 if (_plant->_headCur.getAngle()<=0 )
                {
                    if (grow._right) {
                        trnDir = FaceDirection::FaceRight;
                        isGrow = true;
                    }
                    else if(grow._left)
                    {
                        trnDir = FaceDirection::FaceLeft;
                        isGrow = true;
                    }
              
                }
                else if (_plant->_headCur.getAngle()>0 )
                {
               
                     if(grow._left)
                    {
                        trnDir = FaceDirection::FaceLeft;
                        isGrow = true;
                    }
                    else   if (grow._right) {
                        trnDir = FaceDirection::FaceRight;
                        isGrow = true;
                    }
                    
                }
//                else  if(_plant->convertToWorldSpace( _plant->_headCur.getPositionTRight()).x>(Director::getInstance()->getWinSize().width-200)&&
//                   grow._left)
//                {
//                    trnDir = FaceDirection::FaceLeft;
//                    isGrow = true;
//                }
            }
        }
    }
    else if (dir == FaceDirection::FaceLeft)
    {

        if (grow._right) {trnDir=FaceDirection::FaceRight;}
        else if (flagGrowSelfOrTopDir&&flagleft&&grow._top){trnDir=FaceDirection::FaceTop;}
        else if(flagGrowSelfOrTopDir&&flagleft&&grow._left){trnDir=FaceDirection::FaceLeft;}
        else isGrow =false;
    }
    else if (dir == FaceDirection::FaceRight)
    {
       
        if (grow._left) {trnDir=FaceDirection::FaceLeft;}
        else  if (flagGrowSelfOrTopDir&&flagright&&grow._top){trnDir=FaceDirection::FaceTop;}
        else if(flagGrowSelfOrTopDir&&flagright&&grow._right){trnDir=FaceDirection::FaceRight;}
        else isGrow =false;
    }
        
    }
    
    if (isGrow) {
       
        retLen =_plant->grow(trnDir, len);
        if(retLen>0)
        _preGrowLen += retLen;
        
        if (_plant->_headCur._cp._point.y>_plant->_headTopHeight) {
            _plant->_headTopHeight= _plant->_headCur._cp._point.y;
        }
        checkAddLeaf();
        float len =_plant->_headCur._cp._height;
        float testlen =512;
        if (len > testlen) {
            _plant->checkHeightSub(testlen);
        
            _leafTopHeight-=testlen;
            checkLeafSubHeight(testlen);
            GameManager::getInstance()->getLayerLight()->subLightHeight(testlen);
        }
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
    renderPlant();
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
      //rip->_radius = 100;
        if (topHeight - rip->_height <= 60) {
            rip->_radius = (topHeight - rip->_height)/60 *10;
        }
        else if (topHeight - rip->_height <=660) {
            rip->_radius = (topHeight-60 - rip->_height)/600 * 40.0f+10;
        }
        else if (topHeight - rip->_height > 660) {
            rip->_radius = (topHeight - rip->_height - 660)/400 * 40.0f+50.0f;
            if (rip->_radius >=MAX_WIDTH) {
                rip->_radius  =MAX_WIDTH;
            }
        }
        else break;
        rip->_radius*=2;
        rip++;
    }
 
    auto list = _leafBatch->getChildren();
    for (auto i = list.begin();i!=list.end();i++) {
        
        float height = (int)(*i)->getUserData();
        if (topHeight - height <= 1000) {
            float scale = (topHeight - height)/1000*0.3 + 0.1;
            (*i)->setScale(scale);
        
       }
       else (*i)->setScale(0.4);
        
        
    }

}

void  LayerPlant_1::showCrashBorderLight()
{
    auto gamemanager = GameManager::getInstance();
    LayerBorder* border = gamemanager->getLayerBorder();
    auto pt = _plant->convertToWorldSpace(_plant->_headCur._cp._point);
    auto mapGrid = gamemanager->getMapGrid();
    auto cell = mapGrid->getMapGridCellByPosition(pt);
    int x, y,width,height;
    int rid = 23;
    x = cell._x-rid;
    y = cell._y-rid;
    width = x + 2*rid;
    height = y + 2*rid;
    if (x<0)x=0;
    if (y<0)y=0;
    if (width > mapGrid->_gridWidth)width = mapGrid->_gridWidth;
    if (height > mapGrid->_gridHeight)height = mapGrid->_gridHeight;
    auto end = border->_borderMap.end();
    std::list<GridCell> list;
    for (; y < height; y++) {
        for (int tx = x;tx < width ;tx++) {
            auto gridCell =GridCell(tx,y);
           // log("map %d,%d",gridCell._x,gridCell._y);
            auto ip = border->_borderMap.find(gridCell);
            if (ip!=end) {
                list.push_back(gridCell);
         
            }
        }
    }

  //  _crashBorderBatch->removeAllChildren();
    for (auto& i : list) {
        Sprite* sp = Sprite::createWithTexture(_crashBorderBatch->getTexture());
       // log("crash Cell %d,%d",i._x,i._y);
        auto pt = mapGrid->getPositionByMapGridCell(i);
        sp->setPosition(pt);
        sp->setScale(0.15);
        sp->setOpacity(0);
        sp->setUserData((void*)0);
        sp->setTag(i._y*mapGrid->_gridWidth+i._x);
        FadeTo* fadeto = FadeTo::create(2,60);
        sp->runAction(fadeto);
       // _crashBorderLightMap.insert(std::make_pair(i,sp));
       _crashBorderBatch->addChild(sp);
    }
    
}
void LayerPlant_1::removeBorderLight()
{
    auto& list = _crashBorderBatch->getChildren();
    for (auto& i: list) {
        if ((int)i->getUserData()==100) continue;
        FadeTo* fadeto = FadeTo::create(0.3,0);
        
        CallFuncN* call = CallFuncN::create([](Node* node)
                                            {
                                                node->removeFromParent();
                                            });
        i->setUserData((void*)100);
        Sequence* seq = Sequence::create(fadeto, call,nullptr);
        i->runAction(seq);
    }
}
void LayerPlant_1::removeLightBorderByCell(int tag)
{
    auto node = _crashBorderBatch->getChildByTag(tag);
    if (node) {
        node->removeFromParent();
    }
}
