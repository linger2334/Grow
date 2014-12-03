#ifndef __Grow_Demo_test__GameRuntime__
#define __Grow_Demo_test__GameRuntime__
#include "common.h"
#include "Singleton.hpp"

class ItemModel;
// 程序加载完 做好适配后初始化的 运行时必须的信息 、程序的一些静态内容
class GameRuntime :public Singleton<GameRuntime>
{
public:
    GameRuntime()
    {
        initGameRuntimeInfo();
    }
    void  initGameRuntimeInfo();
    
    //划土面积半径
    CC_SYNTHESIZE(int,_touchClearGridRadius,TouchClearGridRadius);
    
    //设计分辨率
    CC_SYNTHESIZE(cocos2d::Size,_visibleSize,VisibleSize);
    
    //单元像素大小
    CC_SYNTHESIZE(cocos2d::Size,_pixelSize,PixelSize);
    
    //地图单元格的像素大小 适配高清用 通常为8*8
    CC_SYNTHESIZE(cocos2d::Size,_mapGridUnitPixelSize,MapGridUnitPixelSize);
    
    //地图单元格大小（宽，高）
    CC_SYNTHESIZE(cocos2d::Size,_mapGridSize,MapGridSize);
    
    
    CC_SYNTHESIZE(float,_mapGridMaxHeight,MapGridMaxHeight);
    

    CC_SYNTHESIZE(cocos2d::Size,_mapDirtMaskGridSize,MapDirtMaskGridSize);
    
    //植物控制点间的高度 单位纹理像素
    CC_SYNTHESIZE(cocos2d::Size,_plantGrowUnitLength,PlantGrowUnitLength);
    
    //植物渲染时的精细程度 单位为纹理像素高度  默认为1:
    CC_SYNTHESIZE(cocos2d::Size,_plantVerticesUnitLength,PlantVerticesUnitLength);
    
    //各个层的基本大小
    CC_SYNTHESIZE(cocos2d::Size,_gameLayerSize,GameLayerSize);
    
    CC_SYNTHESIZE(float ,_plantSubCPCheckHeight,PlantSubCCheckPHeight);
    CC_SYNTHESIZE(float ,_plantSubCPHeight,PlantSubCPHeight);
    
    CC_SYNTHESIZE(float ,_plantCPListUnitHeight,PlantCPListUnitHeight);
    CC_SYNTHESIZE(float ,_plantVerticesUnitHeight,PlantVerticesUnitHeight);
    
    CC_SYNTHESIZE(float ,_plantRayCrashMinLength,PlantRayCrashMinLength);
    
    CC_SYNTHESIZE(cocos2d::Vec2,_gameLayerUIBorderAPLeft,GameLayerUIBorderAPLeft);
    CC_SYNTHESIZE(cocos2d::Vec2,_gameLayerUIBorderAPRight,GameLayerUIBorderAPRight);
};

#endif /* defined(__Grow_Demo_test__GameRuntime__) */
