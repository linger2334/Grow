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
#ifdef GROW_LEVELEDITOR
    CC_SAFE_RELEASE(_boundFlowerIDDict);
#endif
    
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
    scrollViewContentOffSet = (PAGE_COUNTS-1)*DefiniteSize.height;
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
            float elapsedTime = kDefaultElapsedTime;
            int bindedTriggerID = kDefaultBindedTriggerID;
            bool isAutoSmoothing = kDefaultAutoSmoothingState;
            std::map<std::string,bool> animationControlInstructions;
            std::vector<std::vector<AnimationInfo>> animationInfos;
            void* features = nullptr;
            
            if (!strcmp(typestr,"Flame_Blue")) {
                type = Flame_Blue;
            }
            else if(!strcmp(typestr, "Flame_Orange")){
                type = Flame_Orange;
            }
            else if(!strcmp(typestr, "Flame_Violet")){
                type = Flame_Violet;
            }
            else if(!strcmp(typestr, "Flame_White")){
                type = Flame_White;
            }
            else if(!strcmp(typestr, "Rock_Circle")){
                type = Rock_Circle;
            }
            else if (!strcmp(typestr, "Rock_Cover")){
                type = Rock_Cover;
            }
            else if(!strcmp(typestr, "Rock_Crinkle")){
                type = Rock_Crinkle;
            }
            else if(!strcmp(typestr, "Rock_Cross")){
                type = Rock_Cross;
            }
            else if(!strcmp(typestr, "Rock_Dagger")){
                type = Rock_Dagger;
            }
            else if(!strcmp(typestr, "Rock_Ellipse")){
                type = Rock_Ellipse;
            }
            else if(!strcmp(typestr, "Rock_Mount")){
                type = Rock_Mount;
            }
            else if(!strcmp(typestr, "Rock_MountInv")){
                type = Rock_MountInv;
            }
            else if (!strcmp(typestr, "Rock_Ordinary")){
                type = Rock_Ordinary;
            }
            else if(!strcmp(typestr, "Rock_Ovoid")){
                type = Rock_Ovoid;
            }
            else if (!strcmp(typestr, "Rock_Pebble")){
                type = Rock_Pebble;
            }
            else if (!strcmp(typestr, "Rock_Pillar")){
                type = Rock_Pillar;
            }
            else if (!strcmp(typestr, "Rock_Pocket")){
                type = Rock_Pocket;
            }
            else if(!strcmp(typestr, "Rock_Rect")){
                type = Rock_Rect;
            }
            else if(!strcmp(typestr, "Rock_Trape")){
                type = Rock_Trape;
            }
            else if(!strcmp(typestr, "Cicada")){
                type = Cicada;
                Features_Cicada feat;
                
                if (XMLElement* fanningSpeedElement = objElement->FirstChildElement("fanningSpeed")) {
                    fanningSpeedElement->QueryFloatText(&feat.fanningSpeed);
                }
                if (XMLElement* autoTurnHeadElement = objElement->FirstChildElement("autoTurnHead")) {
                    autoTurnHeadElement->QueryBoolText(&feat.autoTurnHead);
                }
                if (XMLElement* autoFanningElement = objElement->FirstChildElement("autoFanning")) {
                    autoFanningElement->QueryBoolText(&feat.autoFanning);
                }
                if (XMLElement* isReversalStatus = objElement->FirstChildElement("isReversalStatus")) {
                    isReversalStatus->QueryBoolText(&feat.isReversalStatus);
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
            else if(!strcmp(typestr, "Gear_Button")){
                type = Gear_Button;
                Features_GearButton feat;
                if (XMLElement* bindIDElement = objElement->FirstChildElement("bindID")) {
                    bindIDElement->QueryIntText(&feat.bindID);
                }
                if(XMLElement* sinkSpeedElement = objElement->FirstChildElement("sinkSpeed")){
                    sinkSpeedElement->QueryFloatText(&feat.sinkSpeed);
                }
                features = &feat;
            }
            else if(!strcmp(typestr, "Gear_Gate")){
                type = Gear_Gate;
                Features_GearGate feat;
                if(XMLElement* gapElement = objElement->FirstChildElement("gap")){
                gapElement->QueryIntText(&feat.gap);
                }
                if(XMLElement* startRateElement = objElement->FirstChildElement("startRate"))
                {
                    startRateElement->QueryFloatText(&feat.startRate);
                }
                features = &feat;
            }else if (!strcmp(typestr, "Gear_Reversal")){
                type = Gear_Reversal;
            }
            else if(!strcmp(typestr, "Barrier")){
                type = Barrier_;
            }
            else if(!strcmp(typestr, "Decoration_Bridge")){
                type = Decoration_Bridge;
            }
            else if (!strcmp(typestr, "Decoration_Flower")){
                type = Decoration_Flower;
                Features_DecorationFlower feat;
                if (XMLElement* flowerIDElement = objElement->FirstChildElement("flowerID")) {
                    flowerIDElement->QueryIntText(&feat.flowerID);
                }
                features = &feat;
            }
            else if (!strcmp(typestr, "Decoration_FlowerInv")){
                type = Decoration_FlowerInv;
                Features_DecorationFlower feat;
                if (XMLElement* flowerIDElement = objElement->FirstChildElement("flowerID")) {
                    flowerIDElement->QueryIntText(&feat.flowerID);
                }
                features = &feat;
            }
            else if(!strcmp(typestr, "Decoration_Pendant")){
                type = Decoration_Pendant;
            }
            else if (!strcmp(typestr, "Sprouts_Dextro")){
                type = Sprouts_Dextro;
                Features_Sprouts feat;
                if (XMLElement* growSpeedElement = objElement->FirstChildElement("growSpeed")) {
                    growSpeedElement->QueryFloatText(&feat.growSpeed);
                }
                features = &feat;
            }
            else if (!strcmp(typestr, "Sprouts_Levo")){
                type = Sprouts_Levo;
                Features_Sprouts feat;
                if (XMLElement* growSpeedElement = objElement->FirstChildElement("growSpeed")) {
                    growSpeedElement->QueryFloatText(&feat.growSpeed);
                }
                features = &feat;
            }
            else if (!strcmp(typestr, "Sprouts_Slope")){
                type = Sprouts_Slope;
                Features_Sprouts feat;
                if (XMLElement* growSpeedElement = objElement->FirstChildElement("growSpeed")) {
                    growSpeedElement->QueryFloatText(&feat.growSpeed);
                }
                features = &feat;
            }
            
            id = objElement->IntAttribute("id");
            x = objElement->FloatAttribute("x");
            y = objElement->FloatAttribute("y");
            objElement->QueryFloatAttribute("angle", &angle);
            objElement->QueryFloatAttribute("scale", &scale);
            localZorder = objElement->IntAttribute("localZorder");
            objElement->QueryBoolAttribute("isAnimated", &isAnimated);
            objElement->QueryFloatAttribute("triggerTime", &triggerTime);
            objElement->QueryFloatAttribute("elapsedTime", &elapsedTime);
            objElement->QueryIntAttribute("bindedTriggerID", &bindedTriggerID);
            objElement->QueryBoolAttribute("isAutoSmoothing", &isAutoSmoothing);
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
            
            Item item(type,id,x,y,angle,scale,localZorder,isAnimated,triggerTime,elapsedTime,bindedTriggerID,isAutoSmoothing, animationControlInstructions,animationInfos,features);
            _items.push_back(item);
            
            objElement = objElement->NextSiblingElement();
        }
        
        _items.sort(OrderByHeight);
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
    //多边形文件
    std::string plistPath = FileUtils::getInstance()->getWritablePath() + _filename + "polygons.plist";
    if (FileUtils::getInstance()->isFileExist(plistPath)) {
        _polygonsDict = __Dictionary::createWithContentsOfFile(plistPath.c_str());
    }else{
        _polygonsDict = __Dictionary::create();
    }
    CC_SAFE_RETAIN(_polygonsDict);

#ifdef GROW_LEVELEDITOR
    //花的ID是否已经被绑定的文件
    std::string flowerIDPath = FileUtils::getInstance()->getWritablePath() + "boundFlowerID.plist";
    if(FileUtils::getInstance()->isFileExist(flowerIDPath)){
        _boundFlowerIDDict = __Dictionary::createWithContentsOfFile(flowerIDPath.c_str());
    }else{
        _boundFlowerIDDict = __Dictionary::create();
        __Array* flowerIDArray = __Array::createWithCapacity(FLOWER_COUNT);
        for (int i = 0; i<FLOWER_COUNT; i++) {
            flowerIDArray->addObject(__String::createWithFormat("%d",0));
        }
        _boundFlowerIDDict->setObject(flowerIDArray, "flowerID");
    }
    CC_SAFE_RETAIN(_boundFlowerIDDict);
#endif

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
            case Flame_Blue:
                itemElement->SetName("Flame_Blue");
                break;
            case Flame_Orange:
                itemElement->SetName("Flame_Orange");
                break;
            case Flame_Violet:
                itemElement->SetName("Flame_Violet");
                break;
            case Flame_White:
                itemElement->SetName("Flame_White");
                break;
            case Rock_Circle:
                itemElement->SetName("Rock_Circle");
                break;
            case Rock_Cover:
                itemElement->SetName("Rock_Cover");
                break;
            case Rock_Crinkle:
                itemElement->SetName("Rock_Crinkle");
                break;
            case Rock_Cross:
                itemElement->SetName("Rock_Cross");
                break;
            case Rock_Dagger:
                itemElement->SetName("Rock_Dagger");
                break;
            case Rock_Ellipse:
                itemElement->SetName("Rock_Ellipse");
                break;
            case Rock_Mount:
                itemElement->SetName("Rock_Mount");
                break;
            case Rock_MountInv:
                itemElement->SetName("Rock_MountInv");
                break;
            case Rock_Ordinary:
                itemElement->SetName("Rock_Ordinary");
                break;
            case Rock_Ovoid:
                itemElement->SetName("Rock_Ovoid");
                break;
            case Rock_Pebble:
                itemElement->SetName("Rock_Pebble");
                break;
            case Rock_Pillar:
                itemElement->SetName("Rock_Pillar");
                break;
            case Rock_Pocket:
                itemElement->SetName("Rock_Pocket");
                break;
            case Rock_Rect:
                itemElement->SetName("Rock_Rect");
                break;
            case Rock_Trape:
                itemElement->SetName("Rock_Trape");
                break;
            case Cicada:
            {
                itemElement->SetName("Cicada");
                if(item.features){
                    Features_Cicada* features = (Features_Cicada*)item.features;
                    char buf[50];
                    
                    if (features->fanningSpeed!=kDefaultCicadaFanningSpeed) {
                        XMLElement* fanningSpeed = doc->NewElement("fanningSpeed");
                        itemElement->LinkEndChild(fanningSpeed);
                        
                        sprintf(buf, "%f",features->fanningSpeed);
                        XMLText* fanningSpeedContent = doc->NewText(buf);
                        fanningSpeed->LinkEndChild(fanningSpeedContent);
                    }
                    
                    if (features->autoTurnHead!=kDefaultCicadaAutoTurnHeadState) {
                        XMLElement* autoTurnHead = doc->NewElement("autoTurnHead");
                        itemElement->LinkEndChild(autoTurnHead);
                        
                        sprintf(buf, features->autoTurnHead? "1" : "0");
                        XMLText* autoTurnHeadContent = doc->NewText(buf);
                        autoTurnHead->LinkEndChild(autoTurnHeadContent);
                    }
                    
                    if (features->autoFanning!=kDefaultCicadaAutoFanningState) {
                        XMLElement* autoFanning = doc->NewElement("autoFanning");
                        itemElement->LinkEndChild(autoFanning);
                        
                        sprintf(buf, features->autoFanning? "1" : "0");
                        XMLText* autoFanningContent = doc->NewText(buf);
                        autoFanning->LinkEndChild(autoFanningContent);
                    }
                    
                    if (features->isReversalStatus!=kDefaultCicadaReversalStatus) {
                        XMLElement* isReversalStatus = doc->NewElement("isReversalStatus");
                        itemElement->LinkEndChild(isReversalStatus);
                        
                        sprintf(buf, features->isReversalStatus? "1" : "0");
                        XMLText* isReversalStatusContent = doc->NewText(buf);
                        isReversalStatus->LinkEndChild(isReversalStatusContent);
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
            case Gear_Button:
            {
                itemElement->SetName("Gear_Button");
                if(item.features){
                    Features_GearButton* feat = (Features_GearButton*)item.features;
                    char buf[30];
                    
                    if (feat->bindID != kDefaultGearButtonBindID) {
                        XMLElement* bindIDElement = doc->NewElement("bindID");
                        itemElement->LinkEndChild(bindIDElement);
                        
                        sprintf(buf, "%d",feat->bindID);
                        XMLText* bindIDContent = doc->NewText(buf);
                        bindIDElement->LinkEndChild(bindIDContent);
                    }
                    if(feat->sinkSpeed != kDefaultGearButtonSinkSpeed){
                        XMLElement* sinkSpeedElement = doc->NewElement("sinkSpeed");
                        itemElement->LinkEndChild(sinkSpeedElement);
                        
                        sprintf(buf, "%f",feat->sinkSpeed);
                        XMLText* sinkSpeedContent = doc->NewText(buf);
                        sinkSpeedElement->LinkEndChild(sinkSpeedContent);
                    }
                }
            }
                break;
            case Gear_Gate:
            {
                itemElement->SetName("Gear_Gate");
                Features_GearGate* feat = (Features_GearGate*)item.features;
                char buf[30];
                
                if (feat->gap != kDefaultGearGateGap) {
                    XMLElement* gapElement = doc->NewElement("gap");
                    itemElement->LinkEndChild(gapElement);
                    
                    sprintf(buf, "%d",feat->gap);
                    XMLText* gapContent = doc->NewText(buf);
                    gapElement->LinkEndChild(gapContent);
                }
                if (feat->startRate != kDefaultGearGateStartRate) {
                    XMLElement* startRateElement = doc->NewElement("startRate");
                    itemElement->LinkEndChild(startRateElement);
                    
                    sprintf(buf, "%f",feat->startRate);
                    XMLText* startRateContent = doc->NewText(buf);
                    startRateElement->LinkEndChild(startRateContent);
                }
            }
                break;
            case Gear_Reversal:
                itemElement->SetName("Gear_Reversal");
                break;
            case Barrier_:
                itemElement->SetName("Barrier");
                break;
            case Decoration_Bridge:
                itemElement->SetName("Decoration_Bridge");
                break;
            case Decoration_Flower:
            {
                itemElement->SetName("Decoration_Flower");
                Features_DecorationFlower* feat = (Features_DecorationFlower*)item.features;
                char buf[30];
                
                if (feat->flowerID != kDefaultDecorationFlowerID) {
                    XMLElement* flowerIDElement = doc->NewElement("flowerID");
                    itemElement->LinkEndChild(flowerIDElement);
                    
                    sprintf(buf, "%d",feat->flowerID);
                    XMLText* flowerIDContent = doc->NewText(buf);
                    flowerIDElement->LinkEndChild(flowerIDContent);
                }
                
            }
                break;
            case Decoration_FlowerInv:
            {
                itemElement->SetName("Decoration_FlowerInv");
                Features_DecorationFlower* feat = (Features_DecorationFlower*)item.features;
                char buf[30];
                
                if (feat->flowerID != kDefaultDecorationFlowerID) {
                    XMLElement* flowerIDElement = doc->NewElement("flowerID");
                    itemElement->LinkEndChild(flowerIDElement);
                    
                    sprintf(buf, "%d",feat->flowerID);
                    XMLText* flowerIDContent = doc->NewText(buf);
                    flowerIDElement->LinkEndChild(flowerIDContent);
                }
            }
                break;
            case Decoration_Pendant:
                itemElement->SetName("Decoration_Pendant");
                break;
            case Sprouts_Dextro:
            {
                itemElement->SetName("Sprouts_Dextro");
                Features_Sprouts* feat = (Features_Sprouts*)item.features;
                char buf[30];
                
                if (feat->growSpeed != kDefaultSproutsGrowSpeed) {
                    XMLElement* growSpeedElement = doc->NewElement("growSpeed");
                    itemElement->LinkEndChild(growSpeedElement);
                    
                    sprintf(buf, "%f",feat->growSpeed);
                    XMLText* growSpeedContent = doc->NewText(buf);
                    growSpeedElement->LinkEndChild(growSpeedContent);
                }
            }
                
                break;
            case Sprouts_Levo:
            {
                itemElement->SetName("Sprouts_Levo");
                Features_Sprouts* feat = (Features_Sprouts*)item.features;
                char buf[30];
                
                if (feat->growSpeed != kDefaultSproutsGrowSpeed) {
                    XMLElement* growSpeedElement = doc->NewElement("growSpeed");
                    itemElement->LinkEndChild(growSpeedElement);
                    
                    sprintf(buf, "%f",feat->growSpeed);
                    XMLText* growSpeedContent = doc->NewText(buf);
                    growSpeedElement->LinkEndChild(growSpeedContent);
                }
            }
                
                break;
            case Sprouts_Slope:
            {
                itemElement->SetName("Sprouts_Slope");
                Features_Sprouts* feat = (Features_Sprouts*)item.features;
                char buf[30];
                
                if (feat->growSpeed != kDefaultSproutsGrowSpeed) {
                    XMLElement* growSpeedElement = doc->NewElement("growSpeed");
                    itemElement->LinkEndChild(growSpeedElement);
                    
                    sprintf(buf, "%f",feat->growSpeed);
                    XMLText* growSpeedContent = doc->NewText(buf);
                    growSpeedElement->LinkEndChild(growSpeedContent);
                }
            }
                
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
        if (item.elapsedTime != kDefaultElapsedTime) {
            itemElement->SetAttribute("elapsedTime", item.elapsedTime);
        }
        if (item.bindedTriggerID != kDefaultBindedTriggerID) {
            itemElement->SetAttribute("bindedTriggerID", item.bindedTriggerID);
        }
        if (item.isAutoSmoothing != kDefaultAutoSmoothingState) {
            itemElement->SetAttribute("isAutoSmoothing", item.isAutoSmoothing);
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
    
#ifdef GROW_LEVELEDITOR
    std::string flowerIDPath = FileUtils::getInstance()->getWritablePath() + "boundFlowerID.plist";
    bool flowerIDSuccess = _boundFlowerIDDict->writeToFile(flowerIDPath.c_str());
    
    if (result==XML_SUCCESS && plistSuccess && flowerIDSuccess) {
        log("Save file Success!in:%s",savePath.c_str());
    }else{
        log("Can not save file:%s",savePath.c_str());
    }
#endif
    //只有Document对象需要释放
    delete doc;
    return result;
}

void LevelFileHandler::reload()
{
    this->_items.clear();
    this->_polygonsDict->removeAllObjects();
    CC_SAFE_RELEASE_NULL(_polygonsDict);
#ifdef GROW_LEVELEDITOR
    this->_boundFlowerIDDict->removeAllObjects();
    CC_SAFE_RELEASE_NULL(_boundFlowerIDDict);
#endif
    this->loadFile();
}

