//
//  StaticData.h
//  GrowUp
//
//  Created by wlg on 8/12/14.
//
//

#ifndef __GrowUp__StaticData__
#define __GrowUp__StaticData__

#include <iostream>

#include "Macro.h"

#define STATIC_DATA_STRING(key) StaticData::getInstance()->stringFromKey(key)
#define STATIC_DATA_INT(key) StaticData::getInstance()->intFromKey(key)
#define STATIC_DATA_FLOAT(key) StaticData::getInstance()->floatFromKey(key)
#define STATIC_DATA_BOOLEAN(key) StaticData::getInstance()->booleanFromKey(key)
#define STATIC_DATA_POINT(key) StaticData::getInstance()->pointFromKey(key)
#define STATIC_DATA_RECT(key) StaticData::getInstance()->rectFromKey(key)
#define STATIC_DATA_SIZE(key) StaticData::getInstance()->sizeFromKey(key)

class StaticData:public Ref
{
public:
    static StaticData* getInstance();//单例模式，获取静态数据的实例
    void purge();//内存不够时使用
    
    const char* stringFromKey(std::string key);
    int intFromKey(std::string key);
    float floatFromKey(std::string key);
    bool booleanFromKey(std::string key);
    cocos2d::Point pointFromKey(std::string key);//格式key=x,value=0.0
    cocos2d::Rect rectFromKey(std::string key);//格式{0，0，x,y}
    cocos2d::Size sizeFromKey(std::string key);
    
    CC_SYNTHESIZE_READONLY(std::string, m_staticDataPath, StaticDataPath);
    
protected:
    cocos2d::ValueMap m_map;
    
private:
    StaticData();
    ~StaticData();
    bool init();
    
};

#endif /* defined(__GrowUp__StaticData__) */
