//
//  LayerItem.cpp
//  GrowUp
//
//  Created by wlg on 9/30/14.
//
//

#include "LayerItem.h"
#include "GameManager.h"
#include "SceneGame.h"



LayerItem::LayerItem()
{
    
}

LayerItem::~LayerItem()
{
    
}

bool LayerItem::init()
{
    if (GameLayerRollY::init()) {
        setContentSize(cocos2d::Size(WinSize.width,WinSize.height*PAGE_COUNTS));
        return true;
    }

    return false;
}

void LayerItem::moveDown(float y)
{
    Vector<Node*>& items = this->getChildren();
    for (Node*& item : items) {
        item->setPositionY(item->getPositionY() - y);
    }
}

void LayerItem::loadItemsAndBodys()
{
    GameManager* gameManager = GameManager::getInstance();
    for(Item& item : gameManager->_fileHandler->_items)
    {
        ItemModel* itemModel = ItemModel::create(item);
        addChild(itemModel);
        itemModel->createBody(gameManager->_sceneGame->getPhysicsHandler()->getItemBodies());
    }
}
