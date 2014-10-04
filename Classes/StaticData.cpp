//
//  StaticData.cpp
//  GrowUp
//
//  Created by wlg on 8/12/14.
//
//

#include "StaticData.h"
using namespace std;

#define STATIC_DATA_PATH "static_data.plist"

static StaticData* s_sharedStaticData = NULL;

StaticData::StaticData()
{
    m_staticDataPath = STATIC_DATA_PATH;
}

StaticData::~StaticData()
{
    
}

bool StaticData::init()
{
    m_map = FileUtils::getInstance()->getValueMapFromFile(m_staticDataPath);
    return true;
}

StaticData* StaticData::getInstance()
{
    if (!s_sharedStaticData) {
        s_sharedStaticData = new StaticData();
        s_sharedStaticData->init();
    }
    return s_sharedStaticData;
}

void StaticData::purge()
{
    CC_SAFE_RELEASE_NULL(s_sharedStaticData);
}

const char* StaticData::stringFromKey(std::string key)
{
    return m_map.at(key).asString().c_str();
}

int StaticData::intFromKey(std::string key)
{
    return m_map.at(key).asInt();
}

float StaticData::floatFromKey(std::string key)
{
    return m_map.at(key).asFloat();
}

bool StaticData::booleanFromKey(std::string key)
{
    return m_map.at(key).asBool();
}

Point StaticData::pointFromKey(std::string key)
{
    return Vec2(m_map.at(key).asValueMap().at("x").asFloat(),m_map.at(key).asValueMap().at("y").asFloat());
}

Rect StaticData::rectFromKey(std::string key)
{
    return RectFromString(m_map.at(key).asString());
}

Size StaticData::sizeFromKey(std::string key)
{
    return Size(m_map.at(key).asValueMap().at("width").asFloat(),m_map.at(key).asValueMap().at("height").asFloat());
}



