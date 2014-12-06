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
        bool isHasAnyCrashLenght(float lenght);
        std::list<float> _leftCrashLen;
        std::list<float> _rightCrashLen;
    };
    
    class PlantRayCastContext1
    {
    public:
        struct Context
        {
            int   _angle;
            float _length;
        };
        int getCrashLengthCountByAngleRange(float length,int startAngle,int endAngle);
        float getMinCrashLengthByAngleRange(int startAngle,int endAngle);
        float getCrashLengthAveByAngleRange(int startAngle,int endAngle);
        bool isHasAnyCrashLenght(float lenght);
        
        std::list<Context> _crashLengths;
    };
    
    class PlantGrowContext
    {
    public:
        PlantGrowContext():_left(false),_right(false),_top(false){}
        PlantGrowContext(bool type):_left(type),_right(type),_top(type){}
        bool isCanGrowByDir(FaceDirection dir)
        {
            switch (dir)
            {
                case FaceLeft:
                    return _left;
                case FaceRight:
                    return _right;
                case FaceTop:
                    return _top;
            }
            return false;
        }
        void setGrowdir(FaceDirection dir,bool isGrow )
        {
            switch (dir)
            {
                case FaceLeft:
                     _left = isGrow;
                    break;
                case FaceRight:
                     _right = isGrow;
                    break;
                case FaceTop:
                    _top = isGrow;
                    break;
            }
            return ;
        }
        bool allIsValue(bool test){return _left== test && _right ==test;}
        
        PlantGrowContext& operator &=(const PlantGrowContext& con)
        {
            _left &= con._left;
            _right &= con._right;
            _top &= con._top;
            return *this;
        }
        PlantGrowContext operator &(const PlantGrowContext& con)
        {
            PlantGrowContext temp = *this;;
            temp._left &= con._left;
            temp._right &= con._right;
            temp._top &= con._top;
            return temp;
        }
        
        PlantGrowContext& operator |=(const PlantGrowContext& con)
        {
            _left |= con._left;
            _right |= con._right;
            _top |= con._top;
            return *this;
        }
        PlantGrowContext operator |(const PlantGrowContext& con)
        {
            PlantGrowContext temp = *this;;
            temp._left |= con._left;
            temp._right |= con._right;
            temp._top |= con._top;
            return temp;
        }

        bool _left;
        bool _right;
        bool _top;
    };
    void getGroeDirList(PlantNode* plant, std::vector<FaceDirection>& dirList);
    PlantRayCastContext getPlantRayCastContext(PlantNode* plant,float testLength, const std::list<unsigned char>& _typeList,float startAnalg,float stepAngle,int count);
    PlantRayCastContext getPlantRayCastContext(PlantNode* plant,float testLength,float startAnalg,float stepAngle,int count,std::function<bool(ItemModel*)> call);
    PlantRayCastContext1 getPlantRayCastContext1(Vec2 pt,float testLength,float startAnalg,float stepAngle,int count,std::function<bool(ItemModel*)> call);
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
