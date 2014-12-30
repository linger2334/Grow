#ifndef __GrowDemo__PlantB2World__
#define __GrowDemo__PlantB2World__
#include "common.h"
#include "ItemModel.h"

class PlantNode;
class PlantHeadBody : public ItemModel
{
public:
    PlantHeadBody(int index):_plantIndex(index){setType(99);}
    
    PlantNode* getPlantNode();
    
    int _plantIndex;
};

class PlantLineBody : public ItemModel
{
public:
    PlantLineBody():ItemModel()
    {
        setType(98);
    }
    
   // PlantNode* getPlantNode(){}
    int _plantIndex;
};
class PlantLineBorderBody : public ItemModel
{
public:
    PlantLineBorderBody():ItemModel()
    {
        setType(97);
    }
    
   // PlantNode* getPlantNode(){}
    int _plantIndex;
};
#endif /* defined(__GrowDemo__PlantB2World__) */
