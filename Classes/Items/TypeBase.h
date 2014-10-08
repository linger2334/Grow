
#ifndef Grow_Goods_test_TypeBase_h
#define Grow_Goods_test_TypeBase_h

enum
{
    TypeStone   = 101,
    TypeDirtLine = 111,
};

class TypeBase
{
public:
    TypeBase():_type(0){}
    TypeBase(char type):_type(type){}
    void setType(char type){_type = type;}
    char getType(){return _type;}
    bool  isDirtLine(){return _type == TypeDirtLine;}
    bool  isStone(){return _type ==  TypeStone; }
    bool  isMustCrashType(){return _type > 100;}
    bool  isNeedCallBackType(){ return _type < 99;}
    char _type;
};


#endif
