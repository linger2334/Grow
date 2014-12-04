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
//    _fixturesCache->addShapesWithFile("Item_fixtures.plist");
    for (Item& item : _itemsNeedLoaded) {
        ItemModel* newItem = ItemModel::create(item);
        
        if(newItem->_type == Gear_Button || newItem->_type == Decoration_Pendant){
            _layerItemExt->addChild(newItem,item.localZorder);
        }else{
            addChild(newItem,item.localZorder);
            if(newItem->isStone()){
                _rocks.push_back(newItem);
            }
        }
        
        newItem->createBody();
    }
//    _fixturesCache->reset();
    
    loadAllPolygons();
}

void LayerItem::loadItemsAndBodys(float height)
{
//    bool screenNumIncreased = false;
    std::list<Item>:: iterator iter;
    //第一次加载
    if (firstload) {
        integerHeight = static_cast<int>(height/DefiniteSize.height);
        
//        _fixturesCache->addShapesWithFile("Item_fixtures.plist");
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
//        _fixturesCache->reset();
        //运行时
    }else{
//        int newIntegerHeight = static_cast<int>(height/DefiniteSize.height);
//        if (newIntegerHeight - integerHeight >=1) {
//            integerHeight = newIntegerHeight;
//            screenNumIncreased = true;
        
//            _fixturesCache->addShapesWithFile("Item_fixtures.plist");
        
            for (iter = _itemsNeedLoaded.begin(); iter!=_itemsNeedLoaded.end(); ) {
//                if (iter->y <= integerHeight + 1+ kDefaultPreLoadScreenCount) {
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
        
//            _fixturesCache->reset();
        }
    
    loadPolygons(true,height);
}

void LayerItem::loadAllPolygons()
{
    __Dictionary* polygonsDict = GameManager::getInstance()->_fileHandler->_polygonsDict;
    DictElement* singlePolygon;
    __Array* vertexArray;
    Ref* vertex;
    
    int tag;
    Vec2 position;
    bool isConvex;
    std::vector<Vec2> vertexes;
    
    CCDICT_FOREACH(polygonsDict, singlePolygon){
        const char* mainKey = singlePolygon->getStrKey();
        char buf[10];
        strlcpy(buf, mainKey+strlen("polygon"), sizeof(buf));
        tag = atoi(buf);
        
        __Dictionary* property = static_cast<__Dictionary*>(singlePolygon->getObject());
        position = PointFromString((static_cast<__String*>(property->objectForKey("position")))->getCString());
        isConvex = (static_cast<__Bool*>(property->objectForKey("isConvex")))->getValue();
        
        vertexArray = (__Array*)property->objectForKey("vertexes");
        
        CCARRAY_FOREACH(vertexArray, vertex){
            Vec2 singleVertex = PointFromString(((__String*)vertex)->getCString());
            vertexes.push_back(singleVertex);
        }
        
        PolygonInfo info(tag,position,isConvex,vertexes);
        class::Polygon* newPolygon = Polygon::create(info);
        addChild(newPolygon);
        _polygons.push_back(newPolygon);
        
        vertexes.clear();
    }
    
}

void LayerItem::loadPolygons(bool needload,float height)
{
    //
    __Dictionary* polygonsDict = GameManager::getInstance()->_fileHandler->_polygonsDict;
    DictElement* singlePolygon;
    __Array* vertexArray;
    Ref* vertex;
    
    int tag;
    Vec2 position;
    bool isConvex;
    std::vector<Vec2> vertexes;
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
            
            if (position.y<=height/DefiniteSize.height+kDefaultInitialLoadScreenCount && position.y>=height/DefiniteSize.height) {
                vertexArray = static_cast<__Array*>(property->objectForKey("vertexes"));
                CCARRAY_FOREACH(vertexArray, vertex){
                    Vec2 singleVertex = PointFromString(((__String*)vertex)->getCString());
                    vertexes.push_back(singleVertex);
                }
                
                PolygonInfo newPolygonInfo(tag,position,isConvex,vertexes);
                class::Polygon* newPolygon = Polygon::create(newPolygonInfo);
                addChild(newPolygon);
                _polygons.push_back(newPolygon);
                
                vertexes.clear();
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
//                if (it->second <= integerHeight+1+kDefaultPreLoadScreenCount) {
                if(it->second <= (height+DefiniteSize.height)/DefiniteSize.height){
                    sprintf(suffix,"polygon%d",it->first);
                    __Dictionary*property = static_cast<__Dictionary*>(polygonsDict->objectForKey(suffix));
                    
                    tag = it->first;
                    position = PointFromString(((__String*)(property->objectForKey("position")))->getCString());
                    isConvex = (static_cast<__Bool*>(property->objectForKey("isConvex")))->getValue();
                    vertexArray = static_cast<__Array*>(property->objectForKey("vertexes"));
                    CCARRAY_FOREACH(vertexArray, vertex){
                        Vec2 singleVertex = PointFromString(((__String*)vertex)->getCString());
                        vertexes.push_back(singleVertex);
                    }
                    
                    PolygonInfo newPolygonInfo(tag,position,isConvex,vertexes);
                    class::Polygon* newPolygon = Polygon::create(newPolygonInfo);
                    addChild(newPolygon);
                    _polygons.push_back(newPolygon);
                    //
                    vertexes.clear();
                    //
                    it = _polygonsNeedLoaded.erase(it);
                }else{
                    break;
                }
            }
        }
    }
    
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

