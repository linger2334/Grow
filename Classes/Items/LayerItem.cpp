//
//  LayerItem.cpp
//  GrowUp
//
//  Created by wlg on 9/30/14.
//
//

#include <stdlib.h>
#include "LayerItem.h"
#include "GameManager.h"
#include "SceneGame.h"
#include "Polygon.h"


LayerItem::LayerItem():_fixturesCache(nullptr),firstload(true),integerHeight(0)
{
    
}

LayerItem::~LayerItem()
{
    
}

bool LayerItem::init()
{
    if (GameLayerRollY::init()) {
        setContentSize(cocos2d::Size(WinSize.width,WinSize.height*PAGE_COUNTS));
        _itemsNeedLoaded = GameManager::getInstance()->_fileHandler->_items;
        _fixturesCache = GB2ShapeCache::getInstance();
        
        return true;
    }

    return false;
}

void LayerItem::moveDown(float y)
{
    this->setPositionY(getPositionY() - y);
    
}

void LayerItem::loadAllItemsAndBodys()
{
    _fixturesCache->addShapesWithFile("Item_fixtures.plist");
    for (Item& item : _itemsNeedLoaded) {
        ItemModel* newItem = ItemModel::create(item);
        addChild(newItem,item.localZorder);
        newItem->createBody();
        _items.push_back(newItem);
    }
    _fixturesCache->reset();
    
    loadAllPolygons();
}

void LayerItem::loadItemsAndBodys(float height)
{
    bool screenNumIncreased = false;
    std::list<Item>:: iterator iter;
    //第一次加载
    if (firstload) {
        integerHeight = static_cast<int>(height/WinSize.height);
        
        _fixturesCache->addShapesWithFile("Item_fixtures.plist");
        for (iter = _itemsNeedLoaded.begin(); iter!= _itemsNeedLoaded.end();) {
            
            if (iter->y <= integerHeight+kDefaultInitialLoadScreenCount && iter->y>= height/WinSize.height) {
                ItemModel* newItem = ItemModel::create(*iter);
                addChild(newItem,(*iter).localZorder);
                newItem->createBody();
                _items.push_back(newItem);
                //
                iter = _itemsNeedLoaded.erase(iter);
            }else if(iter->y < height/WinSize.height){
                iter = _itemsNeedLoaded.erase(iter);
            }else{
                iter++;
            }
        }
        _fixturesCache->reset();
    //运行时
    }else{
        int newIntegerHeight = static_cast<int>(height/WinSize.height);
        if (newIntegerHeight - integerHeight >=1) {
            integerHeight = newIntegerHeight;
            screenNumIncreased = true;
            
            _fixturesCache->addShapesWithFile("Item_fixtures.plist");
            for (iter = _itemsNeedLoaded.begin(); iter!=_itemsNeedLoaded.end(); ) {
                if (iter->y <= integerHeight + 1+ kDefaultPreLoadScreenCount) {
                    ItemModel* newItem = ItemModel::create(*iter);
                    addChild(newItem,(*iter).localZorder);
                    newItem->createBody();
                    _items.push_back(newItem);
                    //
                    iter = _itemsNeedLoaded.erase(iter);
                }else{
                    iter++;
                }
            }
            _fixturesCache->reset();
        }
    }
    
    loadPolygons(screenNumIncreased,height);
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
            
            if (position.y<=integerHeight+kDefaultInitialLoadScreenCount && position.y>=height/WinSize.height) {
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
            }else if (position.y>integerHeight+2){
                _polygonsNeedLoaded.insert(std::make_pair(tag, position.y));
            }
        
        }
        //运行时
    }else{
        if (needload) {
            std::map<int,float>::iterator it;
            char suffix[20];
            
            for (it = _polygonsNeedLoaded.begin(); it != _polygonsNeedLoaded.end(); ) {
                
                if (it->second <= integerHeight+1+kDefaultPreLoadScreenCount) {
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
                    it++;
                }
            }
        }
    }
    
}

void LayerItem::update(float dt)
{
    //
    std::list<ItemModel*>::iterator itemIter;
    for(itemIter=_items.begin();itemIter!=_items.end();){
        ItemModel* item = *itemIter;
        Vec2 pointInGl = convertToWorldSpace(item->getPosition());
        if (pointInGl.y<-0.3*WinSize.height) {
            itemIter = _items.erase(itemIter);
            item->removeFromParent();
        }else{
            b2Vec2 bodyPosition = cc_to_b2Vec(pointInGl.x, pointInGl.y);
            float bodyAngle = -CC_DEGREES_TO_RADIANS(item->getRotation());
            item->getBody()->SetTransform(bodyPosition, bodyAngle);
            
            itemIter++;
        }
    }
    //
    std::list<class::Polygon*>::iterator polyIter;
    for (polyIter = _polygons.begin(); polyIter!=_polygons.end(); ) {
        class::Polygon* poly = *polyIter;
        Vec2 positionInGl = convertToWorldSpace(poly->getPosition());
        if (positionInGl.y<-0.3*WinSize.height) {
            polyIter = _polygons.erase(polyIter);
            poly->removeFromParent();
        }else{
            b2Vec2 bodyPosition = cc_to_b2Vec(positionInGl.x, positionInGl.y);
            poly->getBody()->SetTransform(bodyPosition, -CC_DEGREES_TO_RADIANS(poly->getRotation()));
            
            polyIter++;
        }
    }

}


