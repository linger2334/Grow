#ifndef __Grow_Demo__LayerPlant__
#define __Grow_Demo__LayerPlant__
#include "PlantBase.h"
#include "Macro.h"
#include "PlantLine.h"
#include "CPLineNode.h"
#include "Plant1.h"
#include "GameManager.h"

class LayerPlant_1 : public GameLayerRollY
{
public:
    CREATE_FUNC(LayerPlant_1);
    virtual  bool init();
    virtual void  moveDown(float y);
    virtual void update(float dt);
    void  updateHead(float dt);
    float grow(float len);
    
    float getPlantTopHeight();
    
    void  checkAddLeaf();
    
    void   checkLeafSubHeight(float _len);
    bool crashTest(Vec2);
    Plant_1*  _plant;
    bool    _isNeedGrow;
    
    FaceDirection      _preDir;
    float              _preGrowLen;
    
    std::list<Sprite*> _leafList;
    SpriteBatchNode*   _leafBatch;
    float              _leafTopHeight;
    bool               _left;
};



#endif /* defined(__Grow_Demo__LayerPlant__) */
