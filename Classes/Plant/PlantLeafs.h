#ifndef __Grow_beta_test02__PlantLeafs__
#define __Grow_beta_test02__PlantLeafs__
#include "common.h"
#include "GameLayerPlant.h"
#include "Plant.h"
#include "PlantHelper.h"
using namespace PlantHelper;
class PlantLeafCountext
{
public:
    PlantLeafCountext():_heightInPlant(0),_leaf(nullptr),_isLeft(true){}
    PlantLeafCountext(float height,Sprite* leaf,bool isLeft):_heightInPlant(height),_leaf(leaf),_isLeft(isLeft){}
    float _heightInPlant;
    Sprite* _leaf;
    bool  _isLeft;
};
struct LeafSaveInfo
{
    bool  isleft;
    int   tag;
    float rotateAngle;
    float heightInPlant;
    Vec2  position;
};
class PlantLeafListBase
{
public:
    PlantLeafListBase():_plantId(0),_leafsStep(0),_leafStepHeight(0),_leafStepRandHeight(0),_node(nullptr)
    {
    }
    void addLeafNodeToEnd(Sprite* node,float height,bool isLeft);
    void updateLeafsHeight(float height);
    void updateLeafsScale();
    PlantLeafCountext& getTopLeaf()
    {
       return  *(_leafList.rbegin());
    }
    bool  isEmpty(){return _leafList.empty(); }
    
    int queryLeafsByHeightRange(float start,float end,std::list<PlantLeafCountext>* outList = nullptr);
    
    float getTopLeafHeight();
    
    virtual void  checkAddLeaf() ;
    virtual void  checkRemoveLeafs() ;
    virtual void  moveDownLeafs(float yLen) ;
    
    void saveInfoToData(cocos2d::Data& out);
    void resetInfoBySaveData(const cocos2d::Data& in);
    
    std::list<PlantLeafCountext> _leafList;
    PlantRadiusHelper _leafScaleRadius;
    int     _plantId;
    Node*   _node;
    int _leafStepHeight;
    int _leafStepRandHeight;
    int _leafsStep;
    std::vector<std::string> _pathLeafs;
};
class PlantLeafsHelper_1 : public PlantLeafListBase
{
public:
    PlantLeafsHelper_1():PlantLeafListBase(){}
};
class PlantLeafList_level_1 : public PlantLeafListBase
{
public:
    void  update(float dt);
};
#endif /* defined(__Grow_beta_test02__PlantLeafs__) */
