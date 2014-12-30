#include "PlantHelper.h"
#include <algorithm>
#include "Plant.h"
#include "ItemModel.h"
#include "GameHeaders.h"
#include "GameLayerMap.h"
namespace PlantHelper
{
    float checkDetFromPointAToPointBWithPointC(Vec2 pointA,Vec2 pointB,Vec2 pointC)
    {
        float result = pointA.x*pointB.y + pointB.x*pointC.y + pointC.x*pointA.y - pointA.y*pointB.x -pointB.y*pointC.x - pointC.y*pointA.x;
        return result;
    }
    bool PlantRayCastContext::isHasAnyCrashLenght(float lenght)
    {
        auto call = [=](const float& var)
        {
            return var > 0 && var <= lenght;
        };
        auto ip = std::find_if(_leftCrashLen.begin(), _leftCrashLen.end(), call);
        if (ip!=_leftCrashLen.end()) {
            return true;
        }
        auto ip2 = std::find_if(_rightCrashLen.begin(), _rightCrashLen.end(), call);
        if (ip2!=_rightCrashLen.end()) {
            return true;
        }
        return false;
    }
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
    //////
    
    int PlantRayCastContext1::getCrashLengthCountByAngleRange(float length,int startAngle,int endAngle)
    {
        return std::count_if(_crashLengths.begin(),_crashLengths.end(),[=](PlantRayCastContext1::Context& value)->bool
                             {
                                 return value._angle>=startAngle &&
                                        value._angle<=endAngle &&
                                        value._length <= length;
                             });
    }
    float PlantRayCastContext1::getMinCrashLengthByAngleRange(int startAngle,int endAngle)
    {
        float n =-1;
        for (auto i:_crashLengths) {
            if (i._angle< startAngle || i._angle > endAngle)continue;
            if(n>=0)
            {
                n = std::min(n, i._length);
            }
            else n = i._length;
        }
        return n;
    }
    float PlantRayCastContext1::getCrashLengthAveByAngleRange(int startAngle,int endAngle)
    {
        float ave = 0.0f;
        int count = 0;
        for (auto i: _crashLengths ) {
            if (i._angle< startAngle || i._angle > endAngle)continue;
            ave += i._length;
            count ++;
        }
        if (count == 0) {
            return 0.0f;
        }
        ave /= count;
        return ave;
    }
    bool PlantRayCastContext1::isHasAnyCrashLenght(float lenght)
    {
        auto call = [=](const PlantRayCastContext1::Context& var)
        {
            return var._length > 0 && var._length <= lenght;
        };
        auto ip = std::find_if(_crashLengths.begin(), _crashLengths.end(), call);
        if (ip!=_crashLengths.end()) {
            return true;
        }
        return false;
    }
    //////
    
    
    
    
    PlantRayCastContext getPlantRayCastContext(Vec2 pt,float testLength,float startAnalg,float stepAngle,int count,std::function<bool(ItemModel*)> call)
    {
        PlantRayCastContext retContext;
        auto physicalWorld = GamePhysicalWorld::getInstance();
        
        Vec2 pto = pt;
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

        PlantRayCastContext getPlantRayCastContext(PlantNode* plant,float testLength,float startAnalg,float stepAngle,int count,std::function<bool(ItemModel*)> call)
    {
        PlantRayCastContext retContext;
        auto physicalWorld = GamePhysicalWorld::getInstance();

       // Vec2 pto = plant->getHeadPositionInWorld();
        Vec2 pto = plant->getHeadUnitPositionInWorld();
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
    
    PlantRayCastContext1 getPlantRayCastContext1(Vec2 pt,float testLength,float startAnalg,float stepAngle,int count,std::function<bool(ItemModel*)> call)
    {
        PlantRayCastContext1 retContext;
        //retContext._crashLengths.resize(count);
        
        auto physicalWorld = GamePhysicalWorld::getInstance();
        
        Vec2 pto = pt;
        Vec2 ptTop = pto + Vec2(0,testLength);
        for (int i = 1; i <= count; i++) {
            int angle =  startAnalg+i*stepAngle;
            Vec2 ptTest = MathHelper::getRotatePosition(pto, ptTop,angle);
            float lenght = physicalWorld->rayCastTest(pto, ptTest,call);
            if (lenght < 0) {
                lenght = testLength;
            }
            PlantRayCastContext1::Context context;
            context._angle = angle;
            context._length =lenght;
            retContext._crashLengths.push_back(context);
        }
        return std::move(retContext);
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
 
    void getGroeDirList(PlantNode* plant, std::vector<FaceDirection>& dirList)
    {
     //   int listtype = 0;
        float angle =plant->getHeadAnalge();
//        listtype = angle                                                                                                                                                                                                       <= 0 ? 1:2;
        //中心优先
//        Vec2 pt0 = plant->getHeadUnitPositionInWorld();
//        Vec2 pt1 = plant->getNextContorlPointInWorld(40, FaceTop)._point;
//        Size vsize = GameRuntime::getInstance()->getVisibleSize();
//        Vec2 pt2 = Vec2(vsize.width*0.5,vsize.height);
//        float  ret = checkDetFromPointAToPointBWithPointC(pt0,pt1,pt2);

        
        if (angle<=0) {
            dirList.push_back(FaceDirection::FaceRight);
            dirList.push_back(FaceDirection::FaceLeft);
        }
        else
        {
            dirList.push_back(FaceDirection::FaceLeft);
            dirList.push_back(FaceDirection::FaceRight);
        }
        //dirList.push_back(FaceDirection::FaceTop);
    }
    PlantGrowContext getGrowContextGrowNextUnitLengthTestMap(PlantNode* plant)
    {
        PlantGrowContext grow(true);
        Vec2 pto = plant->getHeadPositionInWorld();

        Vec2 left = plant->getHeadNextPositionByTopCPInWorld(17, FaceLeft);
        Vec2 right =  plant->getHeadNextPositionByTopCPInWorld(17, FaceRight);
        Vec2 top = plant->getHeadNextPositionByTopCPInWorld(17, FaceTop);
    
        auto physicalWorld = GamePhysicalWorld::getInstance();

        auto callGrid = [=](ItemModel* item)->bool
        {
            return item->getType() == TypeBorderLine;
        };
        float leftLen = physicalWorld->rayCastTest(pto, left,callGrid);
        if (leftLen >= 0 || physicalWorld->isInMapDirtOrOutMap(left))grow._left = false;
        float rightLen = physicalWorld->rayCastTest(pto, right,callGrid);
        if (rightLen >= 0 || physicalWorld->isInMapDirtOrOutMap(right))grow._right = false;
        float topLen = physicalWorld->rayCastTest(pto, top,callGrid);
        if (topLen >= 0 || physicalWorld->isInMapDirtOrOutMap(top)) grow._top  = false;
        return grow;
    }
    PlantGrowContext getGrowContextGrowNextUnitLengthTestStone(PlantNode* plant)
    {
        PlantGrowContext grow(true);
        Vec2 pto = plant->getHeadPositionInWorld();
        
        Vec2 left = plant->getHeadNextPositionByTopCPInWorld(4, FaceLeft);
        Vec2 right =  plant->getHeadNextPositionByTopCPInWorld(4, FaceRight);
        
        auto callGrid = [=](ItemModel* item)->bool
        {
            return item->isStone();
        };

        auto conLeft = PlantHelper::getPlantRayCastContext(left,100,0, 10,18,callGrid);
        auto conRight = PlantHelper::getPlantRayCastContext(right,100,0, 10,18,callGrid);
        if (conLeft.isHasAnyCrashLenght(30))grow._left = false;
        if (conRight.isHasAnyCrashLenght(30))grow._right = false;
        return grow;
    }
    bool isPlantHeadInStone(PlantNode* plant)
    {
        Vec2 cpo = plant->getHeadPositionInWorld();
        auto physicalWorld = GamePhysicalWorld::getInstance();
        return physicalWorld->isInStone(cpo);
    }
    
    float  getMinMaxAngleByLength(float length,float minLen,float maxLen,float minAngle,float maxAngle)
    {
        float retAngle = minAngle;
        if(length < minLen)
        {
            
        }
        if (length > minLen && length < maxLen) {
            float t = (length - minLen) / (maxLen - minLen);
            retAngle = minAngle + (maxAngle - minAngle) * t;
        }
        else if(length >= maxLen)
        {
            retAngle = maxAngle;
        }
        return retAngle;
    }
    ///////////////
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