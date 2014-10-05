//
//  LevelFileHandler.cpp
//  GrowUp
//
//  Created by wlg on 8/21/14.
//
//

#include "LevelFileHandler.h"
#include "FileHelper.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;

LevelFileHandler::LevelFileHandler()
{
    
}

LevelFileHandler::~LevelFileHandler()
{
    
}

LevelFileHandler* LevelFileHandler::createWithFileName(const char* filename)
{
    LevelFileHandler* handler = new LevelFileHandler();
    if (handler && handler->init(filename))
    {
        handler->autorelease();
        return handler;
    }
    else
    {
        CC_SAFE_DELETE(handler);
        return nullptr;
    }
}

bool LevelFileHandler::init(const char *filename)
{
    _filename = filename;
    this->loadFile();
    return true;
}

void LevelFileHandler::loadFile()
{
//    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(_filename+".xml");
    std::string documentPath = FileUtils::getInstance()->getWritablePath() + _filename + ".xml";
    bool fileExists = FileUtils::getInstance()->isFileExist(documentPath);
    if(fileExists){
        
        XMLDocument *pDoc = new XMLDocument();
        pDoc->LoadFile(documentPath.c_str());
        
        tinyxml2::XMLElement* rootElement = pDoc->RootElement();
        XMLElement* objElement = rootElement->FirstChildElement();
        
        while (objElement) {
            const char* type = objElement->Value();
            int id = 0;
            float x = 0,y = 0,angle = DEFAULT_ANGLE,scale = DEFAULT_SCALE;
            int localZorder = 0;
            
            id = objElement->IntAttribute("id");
            x = objElement->FloatAttribute("x");
            y = objElement->FloatAttribute("y");
            objElement->QueryFloatAttribute("angle", &angle);
            objElement->QueryFloatAttribute("scale", &scale);
            localZorder = objElement->IntAttribute("localZorder");
            
            Item item(Flame_Red,id,x,y,angle,scale,localZorder);
            if (!strcmp(type,"Flame_Red")) {
                item.type = Flame_Red;
            }
            else if(!strcmp(type, "Flame_Green")){
                item.type = Flame_Green;
            }
            else if(!strcmp(type, "Flame_Blue")){
                item.type = Flame_Blue;
            }
            else if(!strcmp(type, "Flame_White")){
                item.type = Flame_White;
            }
            else if(!strcmp(type, "Flame_Orange")){
                item.type = Flame_Orange;
            }
            else if(!strcmp(type, "Rock_Circle")){
                item.type = Rock_Circle;
            }
            else if(!strcmp(type, "Rock_Ellipse")){
                item.type = Rock_Ellipse;
            }
            else if(!strcmp(type, "Rock_Gray")){
                item.type = Rock_Gray;
            }
            else if(!strcmp(type, "Cicada")){
                item.type = Cicada;
                Features_Cicada* features = new Features_Cicada();
                
                if(XMLElement* wElement = objElement->FirstChildElement("w")){
                    wElement->QueryFloatText(&features->w);
                }
                if (XMLElement* includedAngeleElement = objElement->FirstChildElement("includedAngle")) {
                    includedAngeleElement->QueryFloatText(&features->includedAngle);
                }
                if (XMLElement* intervalElement = objElement->FirstChildElement("interval")) {
                    intervalElement->QueryFloatText(&features->interval);
                }
                if(XMLElement* bellTransparencyElement = objElement->FirstChildElement("bellTransparency")){
                    bellTransparencyElement->QueryFloatText(&features->bellTransparency);
                }
                
                item.features = features;
            }
            else if(!strcmp(type, "Dragon_Anti")){
                item.type = Dragon_Anti;
                Features_Dragon* features = new Features_Dragon();
                if(XMLElement* wElement = objElement->FirstChildElement("w")){
                    wElement->QueryFloatText(&features->w);
                }
                if (XMLElement* backTransparencyElement = objElement->FirstChildElement("backTransparency")) {
                    backTransparencyElement->QueryFloatText(&features->backTransparency);
                }
                item.features = features;
            }
            else if (!strcmp(type, "Dragon_Clockwise")){
                item.type = Dragon_Clockwise;
                Features_Dragon* features = new Features_Dragon();
                if(XMLElement* wElement = objElement->FirstChildElement("w")){
                    wElement->QueryFloatText(&features->w);
                }
                if (XMLElement* backTransparencyElement = objElement->FirstChildElement("backTransparency")) {
                    backTransparencyElement->QueryFloatText(&features->backTransparency);
                }
                item.features = features;
            }
            else if(!strcmp(type, "Eye")){
                item.type = Eye;
            }
            else if(!strcmp(type,"Polygon")){
                item.type = Polygon;
            }
            _items.push_back(item);
            objElement = objElement->NextSiblingElement();
        }
        //只有Document对象需要释放
        if (pDoc) {
            delete pDoc;
            pDoc = nullptr;
        }
    }else{
        _items.clear();
#ifdef COCOS2D_DEBUG
        log("file %s not exists!",documentPath.c_str());
#endif
    }
}

void LevelFileHandler::removeItemWithID(int id)
{
    for(Item item : this->_items)
    {
        if (item.id == id) {
            _items.erase(find(_items.begin(),_items.end(),item));
            break;
        }
    }
}

void LevelFileHandler::saveItemInformation(int id, float x, float y, float angle, float scale,int localZorder)
{
    for (Item& item : _items) {
        if (item.id == id) {
            item.x = x;
            item.y = y;
            item.angle = angle;
            item.scale = scale;
            item.localZorder = localZorder;
            break;
        }
    }

}

int LevelFileHandler::saveFile()
{
    XMLError result;
    XMLDocument* doc = new XMLDocument();
    if(!doc)
        return -100;
    
    XMLDeclaration* declare = doc->NewDeclaration();
    doc->LinkEndChild(declare);
    if(!declare)
        return -200;
    
    XMLElement* levelElement = doc->NewElement("level");
    doc->LinkEndChild(levelElement);
    if(!levelElement)
        return -200;
    
    for (Item& item : _items) {
        XMLElement* itemElement = doc->NewElement(" ");
        if(!itemElement)
            return -200;
        switch (item.type) {
            case Flame_Red:
                itemElement->SetName("Flame_Red");
                break;
            case Flame_Green:
                itemElement->SetName("Flame_Green");
                break;
            case Flame_Blue:
                itemElement->SetName("Flame_Blue");
                break;
            case Flame_White:
                itemElement->SetName("Flame_White");
                break;
            case Flame_Orange:
                itemElement->SetName("Flame_Orange");
                break;
            case Rock_Circle:
                itemElement->SetName("Rock_Circle");
                break;
            case Rock_Ellipse:
                itemElement->SetName("Rock_Ellipse");
                break;
            case Rock_Gray:
                itemElement->SetName("Rock_Gray");
                break;
            case Cicada:
            {
                itemElement->SetName("Cicada");
                Features_Cicada* features = (Features_Cicada*)item.features;
                char buf[50];
                
                if (features->w!=kDefaultCicadaW) {
                    XMLElement* w = doc->NewElement("w");
                    itemElement->LinkEndChild(w);
                    
                    XMLText* wContent = doc->NewText(gcvt(features->w, 8, buf));
                    w->LinkEndChild(wContent);
                }
                
                if (features->includedAngle!=kDefaultCicadaIncludedAngle) {
                    XMLElement* includedAngle = doc->NewElement("includedAngle");
                    itemElement->LinkEndChild(includedAngle);
                    XMLText* includedAngleContent = doc->NewText(gcvt(features->includedAngle, 7, buf));
                    includedAngle->LinkEndChild(includedAngleContent);
                }
                
                if (features->interval!=kDefaultCicadaInterval) {
                    XMLElement* interval = doc->NewElement("interval");
                    itemElement->LinkEndChild(interval);
                    XMLText* intervalContent = doc->NewText(gcvt(((Features_Cicada*)item.features)->interval, 7, buf));
                    interval->LinkEndChild(intervalContent);
                }
                
                if (features->bellTransparency!=kDefaultCicadaBellTransparency) {
                    XMLElement* bellTransparency = doc->NewElement("bellTransparency");
                    itemElement->LinkEndChild(bellTransparency);
                    XMLText* bellTransparencyContent = doc->NewText(gcvt(((Features_Cicada*)item.features)->bellTransparency, 6, buf));
                    bellTransparency->LinkEndChild(bellTransparencyContent);
                }
            }
                break;
            case Dragon_Anti:
            {
                itemElement->SetName("Dragon_Anti");
                Features_Dragon* features = (Features_Dragon*)item.features;
                char buf[50];
                
                if (features->w!=kDefaultDragonW) {
                    XMLElement* w = doc->NewElement("w");
                    itemElement->LinkEndChild(w);
                    XMLText* wContent = doc->NewText(gcvt(((Features_Dragon*)item.features)->w, 8, buf));
                    w->LinkEndChild(wContent);
                }
                
                if (features->backTransparency!=kDefaultDragonBackTransparency) {
                    XMLElement* backTransparency = doc->NewElement("backTransparency");
                    itemElement->LinkEndChild(backTransparency);
                    XMLText* backTransparencyContent = doc->NewText(gcvt(((Features_Dragon*)item.features)->backTransparency,6, buf));
                    backTransparency->LinkEndChild(backTransparencyContent);
                }
            }
                break;
            case Dragon_Clockwise:
            {
                itemElement->SetName("Dragon_Clockwise");
                Features_Dragon* features = (Features_Dragon*)item.features;
                char buf[50];
                
                if (features->w!=kDefaultDragonW) {
                    XMLElement* w = doc->NewElement("w");
                    itemElement->LinkEndChild(w);
                    XMLText* wContent = doc->NewText(gcvt(((Features_Dragon*)item.features)->w, 8, buf));
                    w->LinkEndChild(wContent);
                }
                
                if (features->backTransparency!=kDefaultDragonBackTransparency) {
                    XMLElement* backTransparency = doc->NewElement("backTransparency");
                    itemElement->LinkEndChild(backTransparency);
                    XMLText* backTransparencyContent = doc->NewText(gcvt(((Features_Dragon*)item.features)->backTransparency,6, buf));
                    backTransparency->LinkEndChild(backTransparencyContent);
                }
            }
                break;
            case Eye:
                itemElement->SetName("Eye");
                break;
            case Polygon:
                itemElement->SetName("Polygon");
                break;
            default:
                break;
        }
        itemElement->SetAttribute("id", item.id);
        itemElement->SetAttribute("x", item.x);
        itemElement->SetAttribute("y", item.y);
        
        if (item.angle != DEFAULT_ANGLE) {
            itemElement->SetAttribute("angle", item.angle);
        }
        if (item.scale != DEFAULT_SCALE) {
            itemElement->SetAttribute("scale",item.scale);
        }
        
        itemElement->SetAttribute("localZorder", item.localZorder);
        levelElement->LinkEndChild(itemElement);
    }
    
    [FileHelper createFolder:@"levels"];
    std::string savePath = FileUtils::getInstance()->getWritablePath() + _filename + ".xml";
    result = doc->SaveFile(savePath.c_str());
    if (result==XML_SUCCESS) {
        log("Save file Success!in:%s",savePath.c_str());
    }else{
        log("Can not save file:%s",savePath.c_str());
    }
    //只有Document对象需要释放
    delete doc;
    return result;
}

void LevelFileHandler::reload()
{
    this->_items.clear();
    this->loadFile();
}

