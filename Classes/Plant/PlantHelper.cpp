#include "PlantHelper.h"
#include <algorithm>
#include "Plant.h"
#include "ItemModel.h"
#include "GameHeaders.h"
#include "GameLayerMap.h"
namespace PlantHelper
{
    float PlantRayCastContext::getLeftCrashLengthAve()
    {
        float ave= 0.0f;
        for (auto i: _leftCrashLen ) {
            ave += i;
        }
        ave /= _leftCrashLen.size();
        return ave;
    }
    float PlantRayCastContext::getRightCrashLengthAve()
    {
        float ave = 0.0f;
        for (auto i: _rightCrashLen ) {
            ave += i;
        }
        ave /= _rightCrashLen.size();
        return ave;
    }
    int PlantRayCastContext::getLeftCrashCount(float length)
    {
    
        return std::count_if(_leftCrashLen.begin(),_leftCrashLen.end(),[=](float value)->bool
            {
                return value <= length;
            });
    }
    int PlantRayCastContext::getRightCrashCount(float length)
    {
        return std::count_if(_rightCrashLen.begin(),_rightCrashLen.end(),[=](float value)->bool
                             {
                                 return value <= length;
                             });
    }
    float PlantRayCastContext::getMinCrashLengthAll()
    {
        return std::min(getMinCrashLengthLeft(), getMinCrashLenghtRight());
    }
    float PlantRayCastContext::getMinCrashLengthLeft()
    {
        float n =-1;
        for (auto i:_leftCrashLen) {
            if(n>=0)
            {
                n = std::min(n, i);
            }
            else n = i;
        }
        return n;
    }
    float PlantRayCastContext::getMinCrashLenghtRight()
    {
        float n =-1;
        for (auto i:_rightCrashLen) {
            if(n>=0)
            {
                n = std::min(n, i);
            }
            else n = i;
        }
        return n;
    }
    

        PlantRayCastContext getPlantRayCastContext(PlantNode* plant,float testLength,float startAnalg,float stepAngle,int count,std::function<bool(ItemModel*)> call)
    {
        PlantRayCastContext retContext;
        auto physicalWorld = GamePhysicalWorld::getInstance();

        Vec2 pto = plant->getHeadPositionInWorld();
       // Vec2 pto = plant->getHeadUnitPositionInWorld();
        Vec2 ptTop = pto + Vec2(0,testLength);
        
        for (int i = 1; i <= count; i++) {
            float angle =  i*stepAngle;
            Vec2 ptLeft = MathHelper::getRotatePosition(pto, ptTop,startAnalg+ -angle);
            Vec2 ptRight = MathHelper::getRotatePosition(pto, ptTop,startAnalg+ angle);
            float left = physicalWorld->rayCastTest(pto, ptLeft,call);
            if (left < 0) {
                left = testLength +3;
            }
            float right = physicalWorld->rayCastTest(pto, ptRight,call);
            if (right < 0) {
                right = testLength +3;
            }
            retContext._leftCrashLen.push_back(left);
            retContext._rightCrashLen.push_back(right);
        }
        
        return retContext;
    }
    PlantRayCastContext getPlantRayCastContext(PlantNode* plant,float testLength, const std::list<unsigned char>& _typeList,float startAnalg,float stepAngle,int count)
    {
         auto call = [&](ItemModel* item)->bool
        {
            // return item->getType()>100;
            for (auto i: _typeList) {
                if (i == item->getType()) {
                    return true;
                }
            }
            return false;
        };
        return getPlantRayCastContext(plant,testLength,startAnalg,stepAngle,count,call);
    }
    PlantGrowContext getGrowContextGrowNextUnitLength(PlantNode* plant)
    {
        
    }
    PlantGrowContext getGrowContextMap(PlantNode* plant)
    {
        
    }
    PlantGrowContext getGrowContextStone(PlantNode* plant)
    {
        
    }
    PlantGrowContext getGrowContextAngle(PlantNode* plant)
    {
        
    }
    PlantGrowContext getRayCastGrowContext(PlantNode* plant)
    {
        
    }
    void getGroeDirList(PlantNode* plant, std::vector<FaceDirection>& dirList)
    {
     //   int listtype = 0;
        float angle =plant->getHeadAnalge();
//        listtype = angle                                                                                                                                                                                                       <= 0 ? 1:2;
        if (angle <= 0) {
            dirList.push_back(FaceDirection::FaceRight);
            dirList.push_back(FaceDirection::FaceLeft);
        }
        else
        {
            dirList.push_back(FaceDirection::FaceLeft);
            dirList.push_back(FaceDirection::FaceRight);
        }
    }
    PlantGrowContext getGrowContextGrowNextUnitLengthTestMap(PlantNode* plant)
    {
        PlantGrowContext grow(true);
        Vec2 pto = plant->getHeadUnitPositionInWorld();
        
   
        Vec2 left = plant->getHeadNextPositionInWorld(4, FaceLeft);
        Vec2 right =  plant->getHeadNextPositionInWorld(4, FaceRight);
//        auto  mapLayer = GameLayerMap::getRunningLayer();
//      //  auto physicalWorld = GamePhysicalWorld::getInstance();
//        if (mapLayer->PointIsInDirtOrOutMapGrid(left))grow._left = false;
//        if (mapLayer->PointIsInDirtOrOutMapGrid(right))grow._right
//            = false;
//        return grow;
        

        auto physicalWorld = GamePhysicalWorld::getInstance();
        
        auto callGrid = [](ItemModel* item)->bool
        {
            return item->getType() == TypeBorderLine||
                   item->isStone();
        };
        float leftLen = physicalWorld->rayCastTest(pto, left,callGrid);
        if (leftLen >= 0)grow._left = false;
        float rightLen = physicalWorld->rayCastTest(pto, right,callGrid);
        if (rightLen >= 0)grow._right = false;
        return grow;
    }
    PlantGrowContext getGrowContextGrowNextUnitLengthTestStone(PlantNode* plant)
    {
        PlantGrowContext grow(true);
        Vec2 pto = plant->getHeadPositionInWorld();
        float len = GameRuntime::getInstance()->getPlantRayCrashMinLength();
        auto cpHead = plant->getTopContorlPoint();
        float angle = cpHead._angle ;
        cpHead._angle -= 40;
        Vec2 left =cpHead.getPositionTopByLength(30);//   plant->getHeadNextPosition(16, FaceLeft);
        cpHead._angle = angle +40;
        Vec2 right =cpHead.getPositionTopByLength(30);/// plant->getHeadNextPosition(16, FaceRight);
        //        Vec2 left = plant->getNextGrowUnitLengthPositionInWorld(FaceLeft);
        //        Vec2 right = plant->getNextGrowUnitLengthPositionInWorld(FaceRight);
        auto physicalWorld = GamePhysicalWorld::getInstance();
        if (pto == left || pto ==right) {
            int n=0;
        }
        auto callGrid = [](ItemModel* item)->bool
        {
            return item->isStone();
            //            unsigned char type = item->getType();
            //            return type >=101 && type<=105;
        };
        float leftLen = physicalWorld->rayCastTest(pto, left,callGrid);
        if (leftLen >= 0)grow._left = true;
        float rightLen = physicalWorld->rayCastTest(pto, right,callGrid);
        if (rightLen >= 0)grow._right = true;
        if(!grow._left&&!grow._right)
        {
            if (angle < 0) {
                grow._right = true;
            }
            else
            {
                grow._left = true;
            }
        }
        return grow;

        
//        PlantGrowContext grow(true);
////        float angle = plant->getHeadAnalge();
////        Vec2 cpo = plant->getHeadPositionInWorld();
////        auto physicalWorld = GamePhysicalWorld::getInstance();
////        auto rayCall = [](ItemModel* item)->bool
////        {
////            return item->isStone();
////        };
////        bool isInStone = PlantHelper::isPlantHeadInStone(plant);
////        Vec2 testLeft =cpo + Vec2(-40,0);
////        Vec2 testRight = cpo + Vec2(40,0);
////        if (!isInStone) {
////            float lenLeft = physicalWorld->rayCastTest(cpo,testLeft,rayCall);
////            float lenRight = physicalWorld->rayCastTest(cpo,testRight,rayCall);
////            if (lenLeft >= 0 && angle <= 0) {
////                grow._left = false;
////            }
////            if (lenRight >= 0 && angle > 0) {
////                grow._right = false;
////            }
////        }
////        else{
////            if (angle <= 0 && physicalWorld->isInStone(testLeft) ) {
////                grow._left = false;
////            }
////            if ( angle > 0  && physicalWorld->isInStone(testRight)) {
////                grow._right = false;
////            }
////        }
////    
//        return grow;
    }
    bool isPlantHeadInStone(PlantNode* plant)
    {
        Vec2 cpo = plant->getHeadPositionInWorld();
        auto physicalWorld = GamePhysicalWorld::getInstance();
        return physicalWorld->isInStone(cpo);
    }
    float PlantRadiusHelper::getRadiusByHeight(float height)
    {
        bool isFind = false;
        if (_contextList.empty()) {
            return _defaultRadius;
        }
        float retRadius = _defaultRadius;

        int i = 0;
        for(; i < _contextList.size();i++)
        {
            if(_contextList[i]._height >= height)
            { isFind = true;break;}
        }
        if(isFind)
        {
            if(i == 0) retRadius = _contextList[0]._radius;
            else
            {
                int preIndex = i-1;
                float preRadius,nowRadius,preHeight,nowHeight;
                preRadius = _contextList[preIndex]._radius;
                nowRadius = _contextList[i]._radius;
                preHeight = _contextList[preIndex]._height;
                nowHeight = _contextList[i]._height;
                float d = (height-preHeight)/(nowHeight - preHeight);
                retRadius = preRadius + (nowRadius-preRadius)*d;
            }
        }
        else
        {
            if (i<=0) retRadius =_contextList[0]._radius;
            else retRadius =_contextList[_contextList.size()-1]._radius;
        }
        return retRadius;
    }
        
    void PlantRadiusHelper::addOneContext(float height,float radius)
    {
        RadiusContext tt;
        tt._height =height;
        tt._radius = radius;
        _contextList.push_back(tt);
    }

    
};