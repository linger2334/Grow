#include "PlantLeafs.h"
#include "GameLayerPlant.h"
#include "GameLayerPlant.h"
#include "Plant.h"
void PlantLeafListBase::addLeafNodeToEnd(Sprite* node,float height,bool isLeft)
{
//    for (auto& i:_leafList) {
//        i._heightInPlant += height;
//    }
    _leafList.push_back(PlantLeafCountext(height,node,isLeft));
}
void PlantLeafListBase::updateLeafsHeight(float height)
{
    auto ip = _leafList.begin();
  //  auto end = _leafList.end();
    while (ip!=_leafList.end()) {
        ip->_heightInPlant += height;
        ip++;
    }
}
void PlantLeafListBase::updateLeafsScale()
{
   
    float topHeight = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(_plantId)->getTopCpHeight();
    for (auto& i:_leafList) {
     float scale =   _leafScaleRadius.getRadiusByHeight(topHeight-i._heightInPlant);
        i._leaf->setScale(scale);
    }
}


float PlantLeafListBase::getTopLeafHeight()
{
    float height =0;
    if(!isEmpty())
    {
        height = getTopLeaf()._heightInPlant;
    }
    return height;
}

int PlantLeafListBase::queryLeafsByHeightRange(float start,float end,std::list<PlantLeafCountext>* outList)
{
    int ret =0 ;
    for (auto& i : _leafList) {
        if ( i._heightInPlant>=start&& i._heightInPlant<end) {
            ret++;
            if (outList) {
                outList->push_back(i);
            }
        }
    }
    return ret;
}
#define OUTMAP(T,D) *((T*)(&buf[index])) = D;\
index += sizeof(D)
void PlantLeafListBase::saveInfoToData(cocos2d::Data& outData)
{
    std::string out;
    auto& leafs = _leafList;;
    int leafsCount = leafs.size();
    int count =  sizeof(_leafStepHeight)+sizeof(_leafStepRandHeight)+sizeof(_leafsStep)+sizeof(int)+(sizeof(LeafSaveInfo)*leafsCount);
    
    char * buf = new char[count];
    int index = 0;
    OUTMAP(int,_leafStepHeight);
    OUTMAP(int,_leafStepRandHeight);
    OUTMAP(int,_leafsStep);
    int size = leafs.size();
    OUTMAP(int,size);
    LeafSaveInfo* bufData = (LeafSaveInfo*)(&buf[index]);
    int i =0;
    for (auto& ip :leafs) {
        bufData[i].position = ip._leaf->getPosition();
        bufData[i].rotateAngle = ip._leaf->getRotation();
        bufData[i].isleft = ip._isLeft;
        bufData[i].heightInPlant = ip._heightInPlant;
        bufData[i].tag = ip._leaf->getTag();
        i++;
    }
    
    outData.copy((unsigned char*)buf,count);
}
void PlantLeafListBase::resetInfoBySaveData(const cocos2d::Data& in)
{
    const char* data = (const char*)in.getBytes();
    int index = 0;
    _leafStepHeight = *((int*)(&data[index]));
    index += sizeof(_leafStepHeight);
    
    _leafStepRandHeight = *((int*)(&data[index]));
    index += sizeof(_leafStepRandHeight);
    
    _leafsStep = *((int*)(&data[index]));
    index += sizeof(_leafsStep);
    
    int size =   *((int*)(&data[index]));
    index += sizeof(size);
    LeafSaveInfo* buf = (LeafSaveInfo*)(&data[index]);
    _leafList.clear();
    for (int i = 0 ; i < size; i++) {
        Sprite* sp = Sprite::create(_pathLeafs[buf[i].tag]);
        sp->setTag(buf[i].tag);
        _node->addChild(sp,-1);
    
        PlantLeafCountext context;
        context._heightInPlant = buf[i].heightInPlant;
        context._isLeft =   context._isLeft;
        context._leaf = sp;
        _leafList.push_back(context);
        if(context._isLeft)
        { sp->setAnchorPoint(Vec2(1,0.5));
            sp->setFlippedX(true);
        }
        else
        {
            sp->setAnchorPoint(Vec2(0,0.5));
        }
        sp->setRotation(buf[i].rotateAngle);
        sp->setPosition(buf[i].position);
    }
    updateLeafsScale();
}

void   PlantLeafListBase::moveDownLeafs(float yLen)
{
    auto ip = _leafList.begin();
    auto end = _leafList.end();
    while (ip!=end) {
        float y = ip->_leaf->getPositionY() - yLen;
        if (y > -100) {
            break;
        }
        ip->_leaf->removeFromParent();
        ip =_leafList.erase(ip);
    }
    while (ip!=end) {
        float y = ip->_leaf->getPositionY() - yLen;
        ip->_leaf->setPositionY(y);
        ip++;
    }
}

void   PlantLeafListBase::checkRemoveLeafs()
{
    auto ip = _leafList.begin();
    auto end = _leafList.end();
    while (ip!=end) {
        if (ip->_leaf->getPositionY()>-100) {
            break;
        }
        ip->_leaf->removeFromParent();
        ip = _leafList.erase(ip);
    }
}
void PlantLeafListBase::checkAddLeaf()
{
    auto layerPlant = GameLayerPlant::getRunningLayer();
    PlantNode* plant = layerPlant->getPlantNodeByIndex(_plantId);
    float plantTopHeight = layerPlant->getPlantNodeByIndex(_plantId)->getTopCpHeight();
    float lefaTopHeight =getTopLeafHeight();
   // srand(time(nullptr));
    if (_leafsStep == 0 ) {
        _leafsStep = rand()%_leafStepRandHeight+_leafStepHeight;
    }

    if (plantTopHeight - lefaTopHeight >_leafsStep) {

      //  srand(time(nullptr));
        int index =  rand()%_pathLeafs.size();
        assert(index<_pathLeafs.size());

        Sprite* sp = Sprite::create(_pathLeafs[index]);
        sp->setTag(index);
        _node->addChild(sp,-1);

        auto cp = plant->getContorlPointByHeight(plantTopHeight-40);
        float angle;
        bool _left = true;
        if (!isEmpty()) {
            _left = getTopLeaf()._isLeft == true ? false :true;
        }
        if(_left)
        { sp->setAnchorPoint(Vec2(1,0.5));
            sp->setFlippedX(true);
            angle =  cp._angle+30 + 22 - rand()%20;
        }
        else
        {
            sp->setAnchorPoint(Vec2(0,0.5));
            angle =  cp._angle-30 + 22 -rand()%20;
        }
        float growSpeed = layerPlant->getPlantInfoByIndex(_plantId)._growSpeed;
        float time = (growSpeed - 30.0f)/ 210.0f;
        FadeIn* fadein = FadeIn::create(2- time*1.2);
        sp->setOpacity(0);
        sp->runAction(fadein);
        sp->setRotation(angle);
        Vec2  v= plant->convertToWorldSpace(cp._point);
        sp->setPosition(v);
        addLeafNodeToEnd(sp,cp._height, _left);
         _leafsStep = (rand()%_leafStepRandHeight)+_leafStepHeight;
    }
}

