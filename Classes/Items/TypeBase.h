
#ifndef Grow_Goods_test_TypeBase_h
#define Grow_Goods_test_TypeBase_h

#include "Macro.h"
#include "TypeDef.h"
/////////////////// 移到Macro.h
//enum
//{
//    TypeDirtLine = 111,
//};
///////////////////////
class TypeBase
{
public:
    TypeBase():_type(0){}
    TypeBase(char type):_type(type){}
    virtual void setType(char type){_type = type;}
    virtual char getType(){return _type;}
    virtual bool  isDirtLine(){return _type == TypeBorderLine;}
    virtual bool  isStone(){return _type >=Rock_Circle&&_type<= Rock_Trape; }
    virtual bool  isMustCrashType(){return _type > 100;}
   virtual bool  isNeedCallBackType(){ return _type < 90;}
    char _type;
};


#endif
