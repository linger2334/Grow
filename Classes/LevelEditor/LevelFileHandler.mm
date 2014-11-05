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
    CC_SAFE_RELEASE(_polygonsDict);
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
    _polygonsDict = nullptr;
    scrollViewZoomFactor = 1.0;
    scrollViewContentOffSet = (PAGE_COUNTS-1)*WinSize.height;
    this->loadFile();
    return true;
}

void LevelFileHandler::loadFile()
{
    //    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(_filename+".xml");
    //
    std::string documentPath = FileUtils::getInstance()->getWritablePath() + _filename + ".xml";
    bool fileExists = FileUtils::getInstance()->isFileExist(documentPath);
    if(fileExists){
        
        XMLDocument *pDoc = new XMLDocument();
        pDoc->LoadFile(documentPath.c_str());
        
        tinyxml2::XMLElement* rootElement = pDoc->RootElement();
        XMLElement* objElement = rootElement->FirstChildElement();
        
        while (objElement) {
            const char* typestr = objElement->Value();
            Item_Type type;
            int id = 0;
            float x = 0,y = 0,angle = kDefaultAngle,scale = kDefaultScale;
            int localZorder = 0;
            bool isAnimated = kDefaultAnimatedOnState;
            float triggerTime = kDefaultTriggerTime;
            std::map<std::string,bool> animationControlInstructions;
            std::vector<std::vector<AnimationInfo>> animationInfos;
            void* features = nullptr;
            
            if (!strcmp(typestr,"Flame_Red")) {
                type = Flame_Red;
            }
            else if(!strcmp(typestr, "Flame_Green")){
                type = Flame_Green;
            }
            else if(!strcmp(typestr, "Flame_Blue")){
                type = Flame_Blue;
            }
            else if(!strcmp(typestr, "Flame_White")){
                type = Flame_White;
            }
            else if(!strcmp(typestr, "Flame_Orange")){
                type = Flame_Orange;
            }
            else if(!strcmp(typestr, "Rock_Circle")){
                type = Rock_Circle;
            }
            else if(!strcmp(typestr, "Rock_Ellipse")){
                type = Rock_Ellipse;
            }
            else if(!strcmp(typestr, "Rock_Gray")){
                type = Rock_Gray;
            }
            else if(!strcmp(typestr, "Cicada")){
                type = Cicada;
                Features_Cicada feat;
                
                if(XMLElement* wElement = objElement->FirstChildElement("w")){
                    wElement->QueryFloatText(&feat.w);
                }
                if (XMLElement* includedAngeleElement = objElement->FirstChildElement("includedAngle")) {
                    includedAngeleElement->QueryFloatText(&feat.includedAngle);
                }
                if (XMLElement* fanningDurationElement = objElement->FirstChildElement("fanningDuration")) {
                    fanningDurationElement->QueryFloatText(&feat.fanningDuration);
                }
                if (XMLElement* intervalElement = objElement->FirstChildElement("interval")) {
                    intervalElement->QueryFloatText(&feat.interval);
                }
                if(XMLElement* bellyTransparencyElement = objElement->FirstChildElement("bellyTransparency")){
                    bellyTransparencyElement->QueryFloatText(&feat.bellyTransparency);
                }
                features = &feat;
                
            }
            else if(!strcmp(typestr, "Dragon_Anti")){
                type = Dragon_Anti;
                Features_Dragon feat;
                if(XMLElement* wElement = objElement->FirstChildElement("w")){
                    wElement->QueryFloatText(&feat.w);
                }
                if (XMLElement* backTransparencyElement = objElement->FirstChildElement("backTransparency")) {
                    backTransparencyElement->QueryFloatText(&feat.backTransparency);
                }
                features = &feat;
            }
            else if (!strcmp(typestr, "Dragon_Clockwise")){
                type = Dragon_Clockwise;
                Features_Dragon feat;
                if(XMLElement* wElement = objElement->FirstChildElement("w")){
                    wElement->QueryFloatText(&feat.w);
                }
                if (XMLElement* backTransparencyElement = objElement->FirstChildElement("backTransparency")) {
                    backTransparencyElement->QueryFloatText(&feat.backTransparency);
                }
                features = &feat;
            }
            else if (!strcmp(typestr, "DoubDragon_Anti")){
                type = DoubDragon_Anti;
                Features_DoubleDragon feat;
                if (XMLElement* wElement = objElement->FirstChildElement("w")) {
                    wElement->QueryFloatText(&feat.w);
                }
                if (XMLElement* bellyTransparencyElement = objElement->FirstChildElement("bellyTransparency")) {
                    bellyTransparencyElement->QueryFloatText(&feat.bellyTransparency);
                }
                features = &feat;
            }
            else if(!strcmp(typestr, "DoubDragon_Clockwise")){
                type = DoubDragon_Clockwise;
                Features_DoubleDragon feat;
                if (XMLElement* wElement = objElement->FirstChildElement("w")) {
                    wElement->QueryFloatText(&feat.w);
                }
                if (XMLElement* bellyTransparencyElement = objElement->FirstChildElement("bellyTransparency")) {
                    bellyTransparencyElement->QueryFloatText(&feat.bellyTransparency);
                }
                features = &feat;
            }
            else if (!strcmp(typestr, "Serpent")){
                type = Serpent_;
                Features_Serpent feat;
                if (XMLElement* absorptionRateElement = objElement->FirstChildElement("absorptionRate")) {
                    absorptionRateElement->QueryFloatText(&feat.absorptionRate);
                }
                if (XMLElement* holeTransparencyElement = objElement->FirstChildElement("holeTransparency")) {
                    holeTransparencyElement->QueryFloatText(&feat.holeTransparency);
                }
                features = &feat;
            }
            else if(!strcmp(typestr, "Eye")){
                type = Eye_;
            }
            
            
            id = objElement->IntAttribute("id");
            x = objElement->FloatAttribute("x");
            y = objElement->FloatAttribute("y");
            objElement->QueryFloatAttribute("angle", &angle);
            objElement->QueryFloatAttribute("scale", &scale);
            localZorder = objElement->IntAttribute("localZorder");
            objElement->QueryBoolAttribute("isAnimated", &isAnimated);
            objElement->QueryFloatAttribute("triggerTime", &triggerTime);
            //
            bool animatedLoopState = kDefaultAnimatedLoopState;
            std::vector<AnimationInfo> animationGroupInfo;
            XMLElement* animations = objElement->FirstChildElement("animations");
            if (animations) {
                XMLElement* animationGroup = animations->FirstChildElement();
                //
                while (animationGroup) {
                    XMLElement* singleAnimation = animationGroup->FirstChildElement();
                    while (singleAnimation) {
                        float waitTime = kDefaultAnimationWaitTime;
                        float rotation = kDefaultAnimationRotation;
                        float rotationSpeed = kDefaultAnimationRotationSpeed;
                        float moveSpeed = kDefaultAnimationMoveSpeed;
                        Vec2 position = kDefaultAnimationPosition;
                        
                        singleAnimation->QueryFloatAttribute("waitTime", &waitTime);
                        singleAnimation->QueryFloatAttribute("rotation", &rotation);
                        singleAnimation->QueryFloatAttribute("rotationSpeed", &rotationSpeed);
                        singleAnimation->QueryFloatAttribute("moveSpeed", &moveSpeed);
                        position = PointFromString(singleAnimation->Attribute("position")) ;
                        
                        AnimationInfo animation(waitTime,rotation,rotationSpeed,moveSpeed,position);
                        animationGroupInfo.push_back(animation);
                        
                        singleAnimation = singleAnimation->NextSiblingElement();
                    }
                    //
                    animationGroup->QueryBoolAttribute("loop", &animatedLoopState);
                    animationControlInstructions.insert(std::make_pair(animationGroup->Name(), animatedLoopState));
                    animationInfos.push_back(animationGroupInfo);
                    animationGroupInfo.clear();
                    animationGroup = animationGroup->NextSiblingElement();
                }

            }
            
            Item item(type,id,x,y,angle,scale,localZorder,isAnimated,triggerTime,animationControlInstructions,animationInfos,features);
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
    //
    std::string plistPath = FileUtils::getInstance()->getWritablePath() + _filename + "polygons.plist";
    if (FileUtils::getInstance()->isFileExist(plistPath)) {
        _polygonsDict = __Dictionary::createWithContentsOfFile(plistPath.c_str());
    }else{
        _polygonsDict = __Dictionary::create();
    }
    CC_SAFE_RETAIN(_polygonsDict);
}

void LevelFileHandler::removeItemWithID(int id)
{
    for(Item& item : this->_items)
    {
        if (item.id == id) {
            _items.erase(find(_items.begin(),_items.end(),item));
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
                if(item.features){
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
                    
                    if (features->fanningDuration!=kDefaultCicadaFanningDuration) {
                        XMLElement* fanningDuration = doc->NewElement("fanningDuration");
                        itemElement->LinkEndChild(fanningDuration);
                        XMLText* fanningDurationContent = doc->NewText(gcvt(((Features_Cicada*)item.features)->fanningDuration, 7, buf));
                        fanningDuration->LinkEndChild(fanningDurationContent);
                    }
                    
                    if (features->interval!=kDefaultCicadaInterval) {
                        XMLElement* interval = doc->NewElement("interval");
                        itemElement->LinkEndChild(interval);
                        XMLText* intervalContent = doc->NewText(gcvt(((Features_Cicada*)item.features)->interval, 7, buf));
                        interval->LinkEndChild(intervalContent);
                    }
                    
                    if (features->bellyTransparency!=kDefaultCicadaBellyTransparency) {
                        XMLElement* bellyTransparency = doc->NewElement("bellyTransparency");
                        itemElement->LinkEndChild(bellyTransparency);
                        XMLText* bellyTransparencyContent = doc->NewText(gcvt(((Features_Cicada*)item.features)->bellyTransparency, 6, buf));
                        bellyTransparency->LinkEndChild(bellyTransparencyContent);
                    }
                }
            }
                break;
            case Dragon_Anti:
            {
                itemElement->SetName("Dragon_Anti");
                if(item.features){
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
            }
                break;
            case Dragon_Clockwise:
            {
                itemElement->SetName("Dragon_Clockwise");
                if(item.features){
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
            }
                break;
            case DoubDragon_Anti:
            {
                itemElement->SetName("DoubDragon_Anti");
                if (item.features) {
                    Features_DoubleDragon* feat = (Features_DoubleDragon*)item.features;
                    char buf[30];
                    
                    if (feat->w != kDefaultDragonW) {
                        XMLElement* w = doc->NewElement("w");
                        itemElement->LinkEndChild(w);
                        
                        sprintf(buf,"%f",feat->w);
                        XMLText* wContent = doc->NewText(buf);
                        w->LinkEndChild(wContent);
                    }
                    if (feat->bellyTransparency!=kDefaultDragonBellyTransparency) {
                        XMLElement* bellyTransparency = doc->NewElement("bellTransparency");
                        itemElement->LinkEndChild(bellyTransparency);
                        
                        sprintf(buf,"%f",feat->bellyTransparency);
                        XMLText* bellyTransparencyContent = doc->NewText("bellyTransparency");
                        bellyTransparency->LinkEndChild(bellyTransparencyContent);
                    }
                }
            }
                break;
            case DoubDragon_Clockwise:
            {
                itemElement->SetName("DoubDragon_Clockwise");
                if (item.features) {
                    Features_DoubleDragon* feat = (Features_DoubleDragon*)item.features;
                    char buf[30];
                    
                    if (feat->w != kDefaultDragonW) {
                        XMLElement* w = doc->NewElement("w");
                        itemElement->LinkEndChild(w);
                        
                        sprintf(buf,"%f",feat->w);
                        XMLText* wContent = doc->NewText(buf);
                        w->LinkEndChild(wContent);
                    }
                    if (feat->bellyTransparency!=kDefaultDragonBellyTransparency) {
                        XMLElement* bellyTransparency = doc->NewElement("bellTransparency");
                        itemElement->LinkEndChild(bellyTransparency);
                        
                        sprintf(buf,"%f",feat->bellyTransparency);
                        XMLText* bellyTransparencyContent = doc->NewText("bellyTransparency");
                        bellyTransparency->LinkEndChild(bellyTransparencyContent);
                    }
                }
            }
                break;
            case Serpent_:
            {
                itemElement->SetName("Serpent");
                if (item.features) {
                    Features_Serpent* feat = (Features_Serpent*)item.features;
                    char buf[30];
                    
                    if (feat->absorptionRate != kDefaultSerpentAbsorptionRate) {
                        XMLElement* absorptionRate = doc->NewElement("absorptionRate");
                        itemElement->LinkEndChild(absorptionRate);
                        
                        sprintf(buf, "%f",feat->absorptionRate);
                        XMLText* absorptionRateContent = doc->NewText(buf);
                        absorptionRate->LinkEndChild(absorptionRateContent);
                    }
                    if (feat->holeTransparency!=kDefaultSerpentHoleTransparency) {
                        XMLElement* holeTransparency = doc->NewElement("holeTransparency");
                        itemElement->LinkEndChild(holeTransparency);
                        
                        sprintf(buf,"%f",feat->holeTransparency);
                        XMLText* holeTransparencyContent =  doc->NewText(buf);
                        holeTransparency->LinkEndChild(holeTransparencyContent);
                        
                    }
                }
            }
                break;
            case Eye_:
                itemElement->SetName("Eye");
                break;
                
            default:
                break;
        }
        itemElement->SetAttribute("id", item.id);
        itemElement->SetAttribute("x", item.x);
        itemElement->SetAttribute("y", item.y);
        
        if (item.angle != kDefaultAngle) {
            itemElement->SetAttribute("angle", item.angle);
        }
        if (item.scale != kDefaultScale) {
            itemElement->SetAttribute("scale",item.scale);
        }
        itemElement->SetAttribute("localZorder", item.localZorder);
        if (item.isAnimated != kDefaultAnimatedOnState) {
            itemElement->SetAttribute("isAnimated", item.isAnimated);
        }
        if (item.triggerTime != kDefaultTriggerTime) {
            itemElement->SetAttribute("triggerTime", item.triggerTime);
        }

        XMLElement* animations = doc->NewElement("animations");
        int groupIndex = 0;
        for (std::vector<AnimationInfo>& animationGroupInfo : item.animationInfos) {
            
            XMLElement* eachAnimationGroup = doc->NewElement((StringUtils::format("group%d",groupIndex)).c_str());
            eachAnimationGroup->SetAttribute("loop", item.animationControlInstructions.at(eachAnimationGroup->Name()));
            int subIndex = 1;
            for (AnimationInfo& singleAnimationInfo : animationGroupInfo) {
                std::string name = StringUtils::format("animation%d",subIndex);
                std::string position = StringUtils::format("{%f,%f}",singleAnimationInfo.position.x,singleAnimationInfo.position.y);
                XMLElement* singleAnimation = doc->NewElement(name.c_str());
                singleAnimation->SetAttribute("waitTime", singleAnimationInfo.waitTime);
                singleAnimation->SetAttribute("rotation", singleAnimationInfo.rotation);
                singleAnimation->SetAttribute("rotationSpeed", singleAnimationInfo.rotationSpeed);
                singleAnimation->SetAttribute("moveSpeed", singleAnimationInfo.moveSpeed);
                singleAnimation->SetAttribute("position", position.c_str());
                
                eachAnimationGroup->LinkEndChild(singleAnimation);
                subIndex++;
            }
            //
            animations->LinkEndChild(eachAnimationGroup);
            groupIndex++;
        }
        //
        itemElement->LinkEndChild(animations);
        //
        levelElement->LinkEndChild(itemElement);
    }
    
    [FileHelper createFolder:@"levels"];
    std::string savePath = FileUtils::getInstance()->getWritablePath() + _filename + ".xml";
    result = doc->SaveFile(savePath.c_str());
    
    std::string plistPath = FileUtils::getInstance()->getWritablePath() + _filename + "polygons.plist";
    bool plistSuccess = _polygonsDict->writeToFile(plistPath.c_str());
    
    if (result==XML_SUCCESS && plistSuccess) {
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
    this->_polygonsDict->removeAllObjects();
    this->loadFile();
}

