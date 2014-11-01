#ifndef __Grow_Demo__LayerPlant__
#define __Grow_Demo__LayerPlant__
#include "PlantBase.h"
#include "common.h"
#include "CPLineNode.h"
#include "Plant1.h"
#include "GameManager.h"
#include "Box2D/Box2D.h"
#include "LayerPlantBase.h"
class GrowContext
{
public:
    GrowContext():_left(false),_right(false),_top(false){}
    GrowContext(bool left,bool right,bool top):_left(left),_right(right),_top(top){}
    bool isCanGrowByDir(FaceDirection dir)
    {
        bool ret = false;
        switch(dir)
        {
            case FaceLeft:
                ret  = _left;
                break;
            case FaceRight:
                ret  = _right;
                break;
            case FaceTop:
                ret  = _top;
                break;
            default:
                assert(0);
                break;
        };
        return ret;
    }
    bool _left,_right,_top;
};
class LayerPlant_1 : public LayerPlantBase
{
public:
    CREATE_FUNC(LayerPlant_1);
    virtual  bool init();
    virtual  void onEnter();
    virtual  void onExit();
    virtual void  moveDown(float y);
    virtual void update(float dt);
    void  updateHead(float dt);
    float grow(float len);
    float grow1(float len);
    virtual float getPlantTopHeightInView();
    float getPlantTopHeight();
    void  checkAddLeaf();


    void removeBorderLight();
    void removeLightBorderByCell(int tag);
    void showCrashBorderLight();
    
    void getPlantRayCrashLength(float outLens[3]);
    
    void renderPlant();
    void checkLeafSubHeight(float _len);
    bool crashTest(Vec2);
    
    void createHeadB2Body();
    void updateHeadB2Body();
    
    void getGrowDirList( std::vector<FaceDirection>& dirList);
    void getFirstGrowDirList( std::vector<FaceDirection>& dirList);
    void getNomorGrowDirList( std::vector<FaceDirection>& dirList);
    
    GrowContext getCanGrowContext();
    GrowContext getMinRangeCanGrowContext();
    GrowContext getFirstCanGrowContext();
    bool   isCashPoint(Vec2 pt, int type =0);
    bool   isCrashPointGridAndStone(Vec2 pt);
    
    int    getPlantHeadViewWidthType();
    
    void doReGrow(float speed, float len);
    
    int   reGrow(float dt);
    float      _reGrowLength;
    Plant_1*  _plant;
    bool    _isNeedGrow;
    bool    _isReGrow;
    float   _reGrowSpeed;
    
    std::list<Sprite*> _leafList;
    SpriteBatchNode*   _leafBatch;
    float              _leafTopHeight;
    bool               _left;
    RenderTexture*     _render;
    float              _leafCheckAddHeight;
    Vec2               _leafPoint;
    bool               _isCheckAddleaf;
    

    float              _turnGrowLength;
    
    Sprite*             _lightSprite;
    bool                _islightRuningAction;
    SpriteBatchNode*    _crashBorderBatch;
    std::map<GridCell,Sprite*> _crashBorderLightMap;
    b2Body*        _headBody;
    b2World*     _world;
    
    int         _musicId;
};



#endif /* defined(__Grow_Demo__LayerPlant__) */
