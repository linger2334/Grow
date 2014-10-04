
#ifndef __PlantOri__MathHelper__
#define __PlantOri__MathHelper__
#include "Macro.h"
namespace MathHelper
{
Vec3 getRotatePosition(Vec3 pto,float len,float angle);
Vec3 getRotatePosition(Vec3 pto,Vec3 pt,float angle);
   
Vec2 getRotatePosition(Vec2 pto,float len,float angle);
Vec2 getRotatePosition(Vec2 pto,Vec2 pt,float angle);
float getRotateAngle(float acrLength,float ratius);
    float getAcrLength(float angle,float radius );
void getWidthHeight(float p[2],float len,float angle);

};
using namespace MathHelper;
#endif /* defined(__PlantOri__MathHelper__) */
