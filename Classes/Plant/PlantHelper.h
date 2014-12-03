#ifndef __Grow_Demo_test__PlantGrowHelper__
#define __Grow_Demo_test__PlantGrowHelper__
#include "common.h"
#include "ContorlPoint.h"
class PlantNode;
class ItemModel;
namespace PlantHelper
{
    class PlantRayCastContext
    {
    public:
        int getLeftCrashCount(float length);
        int getRightCrashCount(float length);
        float getLeftCrashLengthAve();
        float getRightCrashLengthAve();
        float getMinCrashLengthAll();
        float getMinCrashLengthLeft();
        float getMinCrashLenghtRight();
        std::list<float> _leftCrashLen;
        std::list<float> _rightCrashLen;
    };
    
    class PlantGrowContext
    {
    public:
        PlantGrowContext():_left(false),_right(false){}
        PlantGrowContext(bool type):_left(type),_right(type){}
        bool isCanGrowByDir(FaceDirection dir)
        {
            switch (dir)
            {
                case FaceLeft:
                    return _left;
                case FaceRight:
                    return _right;
            }
            return false;
        }
        void setGrowdir(FaceDirection dir,bool isGrow )
        {
            switch (dir)
            {
                case FaceLeft:
                     _left = isGrow;
                case FaceRight:
                     _right = isGrow;
            }
            return ;
        }
        bool allIsValue(bool test){return _left== test && _right ==test;}
        bool _left;
        bool _right;
    };
    void getGroeDirList(PlantNode* plant, std::vector<FaceDirection>& dirList);
    PlantRayCastContext getPlantRayCastContext(PlantNode* plant,float testLength, const std::list<unsigned char>& _typeList,float startAnalg,float stepAngle,int count);
    PlantRayCastContext getPlantRayCastContext(PlantNode* plant,float testLength,float startAnalg,float stepAngle,int count,std::function<bool(ItemModel*)> call);
    PlantGrowContext getGrowContextMap(PlantNode* plant);
    PlantGrowContext getGrowContextStone(PlantNode* plant);
    PlantGrowContext getGrowContextAngle(PlantNode* plant);
    PlantGrowContext getRayCastGrowContext(PlantNode* plant);
    PlantGrowContext getGrowContextGrowNextUnitLengthTestMap(PlantNode* plant);
    PlantGrowContext getGrowContextGrowNextUnitLengthTestStone(PlantNode* plant);
    
    bool  isPlantHeadInStone(PlantNode* plant);
    class RadiusContext
    {
    public:
        float _height;
        float _radius;
    };
    
    class PlantRadiusHelper
    {
    public:

        typedef std::vector<RadiusContext> PlantRadiusContextList;
        PlantRadiusHelper():_defaultRadius(0){ _preIterator = _contextList.begin();}
        
        PlantRadiusHelper(float defaultRadius):_defaultRadius(defaultRadius){}
        
        float getRadiusByHeight(float);
        
        void addOneContext(float height,float radius);
        
        float _defaultRadius;
        PlantRadiusContextList _contextList;
        PlantRadiusContextList::iterator _preIterator;
        
    };
};

#endif /* defined(__Grow_Demo_test__PlantGrowHelper__) */
