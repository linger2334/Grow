//
//  LayerItem.cpp
//  GrowUp
//
//  Created by wlg on 9/30/14.
//
//
#include "CocosGUI.h"
#include "LayerItem.h"
#include "GameManager.h"
#include "Polygon.h"
#include "GearGate.h"
#include "TriggerNode.h"

using namespace ui;

LayerItem::LayerItem():_fixturesCache(nullptr),firstload(true),integerHeight(0),_layerItemExt(nullptr)
{
    
}

LayerItem::~LayerItem()
{
    _fixturesCache->reset();
}

bool LayerItem::init()
{
    if (GameLayerBase::init()) {
        setContentSize(cocos2d::Size(DefiniteSize.width,DefiniteSize.height*PAGE_COUNTS));
        _itemsNeedLoaded = GameManager::getInstance()->_fileHandler->_items;
        _fixturesCache = GB2ShapeCache::getInstance();
        _fixturesCache->addShapesWithFile("Item_fixtures.plist");
        
        _layerItemExt = Node::create();
        _layerItemExt->setContentSize(DefiniteSize);
        _layerItemExt->setAnchorPoint(Vec2(0.5,0));
        _layerItemExt->setPosition(Vec2(VisibleSize.width/2,0));
        _layerItemExt->setUserObject(this);
        
        return true;
    }
    
    return false;
}

void LayerItem::moveDown(float y)
{
    this->setPositionY(getPositionY() - y);
    _layerItemExt->setPositionY(getPositionY());
}

void LayerItem::loadAllItemsAndBodys()
{
    for (Item& item : _itemsNeedLoaded) {
        ItemModel* newItem = ItemModel::create(item);
        
        if(newItem->isSpecialItem()){
            _layerItemExt->addChild(newItem,item.localZorder);
        }else{
            addChild(newItem,item.localZorder);
            if(newItem->isStone()){
                _rocks.push_back(newItem);
            }
        }
        
        newItem->createBody();
    }
    
    loadAllPolygons();
}

void LayerItem::loadItemsAndBodys(float height)
{
    std::list<Item>:: iterator iter;
    //第一次加载
    if (firstload) {
        integerHeight = static_cast<int>(height/DefiniteSize.height);
        
        for (iter = _itemsNeedLoaded.begin(); iter!= _itemsNeedLoaded.end();) {
            
            if (iter->y <= integerHeight+kDefaultInitialLoadScreenCount && iter->y>= height/DefiniteSize.height) {
                ItemModel* newItem = ItemModel::create(*iter);
                
                if(newItem->isSpecialItem()){
                    _layerItemExt->addChild(newItem,(*iter).localZorder);
                }else{
                    addChild(newItem,(*iter).localZorder);
                    if(newItem->isStone()){
                        _rocks.push_back(newItem);
                    }
                }
                newItem->createBody();
                //
                iter = _itemsNeedLoaded.erase(iter);
            }else if(iter->y < height/DefiniteSize.height){
                iter = _itemsNeedLoaded.erase(iter);
            }else{
                break;
            }
        }
    ////
//        Sprite* rock = Sprite::create("Rock_Circle1.png");
//        rock->setPosition(Vec2(VisibleSize.width/2, VisibleSize.height/2));
//        Sprite* flame = Sprite::create("Flame_White.png");
//        flame->setPosition(Vec2(VisibleSize.width/2, VisibleSize.height/2));
//        Sprite* stoneGate = Sprite::create("StoneGate1.png");
//        stoneGate->setPosition(Vec2(VisibleSize.width/2, VisibleSize.height/2));
//        Animation* openGateAnimation = Animation::create();
//        for (int i = 1; i<10; i++) {
//            openGateAnimation->addSpriteFrameWithFile(StringUtils::format("StoneGate%d.png",i));
//        }
//        openGateAnimation->setDelayPerUnit(1.0/9);
//        openGateAnimation->setRestoreOriginalFrame(false);
//        Animate* openGate = Animate::create(openGateAnimation);
//        stoneGate->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1),openGate,NULL)));
//        
//        addChild(rock);
//        addChild(flame);
//        addChild(stoneGate);
    ///////
        
    }else{
        //运行时
        for (iter = _itemsNeedLoaded.begin(); iter!=_itemsNeedLoaded.end(); ) {
            if(iter->y <= (height + DefiniteSize.height)/DefiniteSize.height){
                ItemModel* newItem = ItemModel::create(*iter);
                if(newItem->isSpecialItem()){
                    _layerItemExt->addChild(newItem,(*iter).localZorder);
                }else{
                    addChild(newItem,(*iter).localZorder);
                    if (newItem->isStone()) {
                        _rocks.push_back(newItem);
                    }
                }
                newItem->createBody();
                //
                iter = _itemsNeedLoaded.erase(iter);
            }else{
                break;
            }
        }
    }

    loadPolygons(true,height);
}

void LayerItem::loadAllPolygons()
{
    __Dictionary* polygonsDict = GameManager::getInstance()->_fileHandler->_polygonsDict;
    DictElement* singlePolygon;
    __Array* vertexArray;
    __Array* ids;
    Ref* vertex;
    Ref* Id;
    
    int tag;
    Vec2 position;
    bool isConvex;
    std::string type;
    std::vector<Vec2> vertexes;
    std::vector<int> bindIDs;
    
    CCDICT_FOREACH(polygonsDict, singlePolygon){
        const char* mainKey = singlePolygon->getStrKey();
        char buf[10];
        strlcpy(buf, mainKey+strlen("polygon"), sizeof(buf));
        tag = atoi(buf);
        
        __Dictionary* property = static_cast<__Dictionary*>(singlePolygon->getObject());
        position = PointFromString((static_cast<__String*>(property->objectForKey("position")))->getCString());
        isConvex = (static_cast<__Bool*>(property->objectForKey("isConvex")))->getValue();
        type = "Polygon_";
        if (property->objectForKey("type")) {
            type = static_cast<__String*>(property->objectForKey("type"))->getCString();
        }
        
        vertexArray = (__Array*)property->objectForKey("vertexes");
        CCARRAY_FOREACH(vertexArray, vertex){
            Vec2 singleVertex = PointFromString(((__String*)vertex)->getCString());
            vertexes.push_back(singleVertex);
        }
        
        if (!type.compare("Polygon_")) {
            PolygonInfo newPolygonInfo(tag,position,isConvex,vertexes);
            class::Polygon* newPolygon = Polygon::create(newPolygonInfo);
            addChild(newPolygon);
            _polygons.push_back(newPolygon);
        }else if (!type.compare("Trigger")){
            ids = static_cast<__Array*>(property->objectForKey("bindIDs"));
            CCARRAY_FOREACH(ids, Id){
                int itemid = static_cast<__String*>(Id)->intValue();
                bindIDs.push_back(itemid);
            }
            TriggerInfo newTriggerInfo(tag,position,isConvex,vertexes,bindIDs);
            TriggerNode* newTrigger = TriggerNode::create(newTriggerInfo);
            addChild(newTrigger);
        }
        
        vertexes.clear();
        bindIDs.clear();
    }
    
}

void LayerItem::loadPolygons(bool needload,float height)
{
    //
    __Dictionary* polygonsDict = GameManager::getInstance()->_fileHandler->_polygonsDict;
    DictElement* singlePolygon;
    __Array* vertexArray;
    __Array* ids;
    Ref* vertex;
    Ref* Id;
    
    int tag;
    Vec2 position;
    bool isConvex;
    std::string type;
    std::vector<Vec2> vertexes;
    std::vector<int> bindIDs;
    //第一次加载
    if(firstload){
        firstload = false;
        
        CCDICT_FOREACH(polygonsDict, singlePolygon){
            const char* mainKey = singlePolygon->getStrKey();
            char buf[10];
            strlcpy(buf,mainKey+strlen("polygon"),sizeof(buf));
            tag = atoi(buf);
            
            __Dictionary* property = static_cast<__Dictionary*>(singlePolygon->getObject());
            position = PointFromString((static_cast<__String*>(property->objectForKey("position")))->getCString());
            isConvex = (static_cast<__Bool*>(property->objectForKey("isConvex")))->getValue();
            type = "Polygon_";
            if (property->objectForKey("type")) {
                type = static_cast<__String*>(property->objectForKey("type"))->getCString();
            }
            
            //
            if (position.y<=height/DefiniteSize.height+kDefaultInitialLoadScreenCount && position.y>=height/DefiniteSize.height) {
                vertexArray = static_cast<__Array*>(property->objectForKey("vertexes"));
                CCARRAY_FOREACH(vertexArray, vertex){
                    Vec2 singleVertex = PointFromString(((__String*)vertex)->getCString());
                    vertexes.push_back(singleVertex);
                }
                
                if (!type.compare("Polygon_")) {
                    PolygonInfo newPolygonInfo(tag,position,isConvex,vertexes);
                    class::Polygon* newPolygon = Polygon::create(newPolygonInfo);
                    addChild(newPolygon);
                    _polygons.push_back(newPolygon);
                }else if (!type.compare("Trigger")){
                    ids = static_cast<__Array*>(property->objectForKey("bindIDs"));
                    CCARRAY_FOREACH(ids, Id){
                        int itemid = static_cast<__String*>(Id)->intValue();
                        bindIDs.push_back(itemid);
                    }
                    TriggerInfo newTriggerInfo(tag,position,isConvex,vertexes,bindIDs);
                    TriggerNode* newTrigger = TriggerNode::create(newTriggerInfo);
                    addChild(newTrigger);
                }
                
                vertexes.clear();
                bindIDs.clear();
            }else if (position.y>height/DefiniteSize.height+kDefaultInitialLoadScreenCount){
                _polygonsNeedLoaded.push_back(std::make_pair(tag, position.y));
            }
        }
        std::sort(_polygonsNeedLoaded.begin(), _polygonsNeedLoaded.end(), OrderPolygonByHeight);
        
    }else{
        //运行时
        if (needload) {
            std::vector<std::pair<int, float>>::iterator it;
            char suffix[20];
            
            for (it = _polygonsNeedLoaded.begin(); it != _polygonsNeedLoaded.end(); ) {
                if(it->second <= (height+DefiniteSize.height)/DefiniteSize.height){
                    sprintf(suffix,"polygon%d",it->first);
                    __Dictionary*property = static_cast<__Dictionary*>(polygonsDict->objectForKey(suffix));
                    
                    tag = it->first;
                    position = PointFromString(((__String*)(property->objectForKey("position")))->getCString());
                    isConvex = (static_cast<__Bool*>(property->objectForKey("isConvex")))->getValue();
                    type = "Polygon_";
                    if (property->objectForKey("type")) {
                        type = static_cast<__String*>(property->objectForKey("type"))->getCString();
                    }
                    
                    vertexArray = static_cast<__Array*>(property->objectForKey("vertexes"));
                    CCARRAY_FOREACH(vertexArray, vertex){
                        Vec2 singleVertex = PointFromString(((__String*)vertex)->getCString());
                        vertexes.push_back(singleVertex);
                    }
                    
                    if (!type.compare("Polygon_")) {
                        PolygonInfo newPolygonInfo(tag,position,isConvex,vertexes);
                        class::Polygon* newPolygon = Polygon::create(newPolygonInfo);
                        addChild(newPolygon);
                        _polygons.push_back(newPolygon);
                    }else if (!type.compare("Trigger")){
                        ids = static_cast<__Array*>(property->objectForKey("bindIDs"));
                        CCARRAY_FOREACH(ids, Id){
                            int itemid = static_cast<__String*>(Id)->intValue();
                            bindIDs.push_back(itemid);
                        }
                        TriggerInfo newTriggerInfo(tag,position,isConvex,vertexes,bindIDs);
                        TriggerNode* newTrigger = TriggerNode::create(newTriggerInfo);
                        addChild(newTrigger);
                    }
                    //
                    vertexes.clear();
                    bindIDs.clear();
                    //
                    it = _polygonsNeedLoaded.erase(it);
                }else{
                    break;
                }
            }
        }
    }
    
}

void LayerItem::saveData()
{
//    int itemCounts = static_cast<int>(getChildrenCount());
//    for (int i = 0; i<itemCounts; i++) {
//        <#statements#>
//    }
}

void LayerItem::invertStatusInCurrentScreen()
{
    b2AABB currentScreen;
    currentScreen.lowerBound = cc_to_b2Vec(0,0);
    currentScreen.upperBound = cc_to_b2Vec(VisibleSize.width, VisibleSize.height);
    GameManager::getInstance()->getBox2dWorld()->QueryAABB(GameManager::getInstance()->getPhysicsHandler(), currentScreen);
}

void LayerItem::update(float dt)
{
    //所有的道具（除了特殊道具）
    int itemCounts = static_cast<int>(getChildrenCount());
    std::set<ItemModel*> toDelete;
    for (int i = 0; i<itemCounts; i++) {
        if(getChildren().at(i)->getTag() != -1){
            ItemModel* item = static_cast<ItemModel*>(getChildren().at(i));
            Vec2 pointInGl = convertToWorldSpace(item->getPosition());
            if(pointInGl.y < -0.3*VisibleSize.height){
                toDelete.insert(item);
            }else{
                if (item->getBody()) {
                    b2Vec2 bodyPosition = cc_to_b2Vec(pointInGl.x,pointInGl.y);
                    float bodyAngle = -CC_DEGREES_TO_RADIANS(item->getRotation());
                    item->getBody()->SetTransform(bodyPosition, bodyAngle);
                    
                }
            }
        }
    }
    
    for (ItemModel* item : toDelete) {
        if (item->_type == Gear_Gate) {
            ((GearGate*)item)->getLeftHalf()->removeFromParent();
            ((GearGate*)item)->getRightHalf()->removeFromParent();
        }else if(item->_type == Polygon_){
            _polygons.remove(item);
        }else if(item->isStone()){
            _rocks.remove(item);
        }
        
        this->removeChild(item);
    }
    toDelete.clear();
    
    //特殊道具
    itemCounts = static_cast<int>(_layerItemExt->getChildrenCount());
    for(int i = 0;i<itemCounts;i++){
        ItemModel* item = static_cast<ItemModel*>(_layerItemExt->getChildren().at(i));
        Vec2 pointInGl = convertToWorldSpace(item->getPosition());
        if(pointInGl.y < -0.3*VisibleSize.height){
            toDelete.insert(item);
        }else{
            if (item->getBody()) {
                b2Vec2 bodyPosition = cc_to_b2Vec(pointInGl.x,pointInGl.y);
                float bodyAngle = -CC_DEGREES_TO_RADIANS(item->getRotation());
                item->getBody()->SetTransform(bodyPosition, bodyAngle);
            }
        }
    }
    
    for (ItemModel* itemSpecial : toDelete) {
        _layerItemExt->removeChild(itemSpecial);
    }
    
    //
    
    
}

bool OrderPolygonByHeight(std::pair<int, float> &polygonIndex1, std::pair<int, float> &polygonIndex2)
{
    return polygonIndex1.second < polygonIndex2.second;
}

