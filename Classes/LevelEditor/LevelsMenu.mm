//
//  LevelsMenu.cpp
//  GrowUp
//
//  Created by wlg on 9/16/14.
//
//

#include "LevelsMenu.h"
#include "LevelFileHandler.h"
#include "SceneGame.h"
#include "LevelEditor.h"
#include "GameManager.h"

using namespace cocos2d::ui;

LevelsMenu::LevelsMenu()
{
    
}

LevelsMenu::~LevelsMenu()
{
 
}


Scene* LevelsMenu::createScene()
{
    Scene* scene = Scene::create();
    scene->addChild(LevelsMenu::create());
    return scene;
}



bool LevelsMenu::init()
{
    if(Layer::init())
    {
        ListView* listView = ListView::create();
        listView->setDirection(ui::ScrollView::Direction::VERTICAL);
        listView->setTouchEnabled(true);
        listView->setBounceEnabled(true);
        
        int levelNumber = 0;
        std::string fullPath;
        bool fileExists;
        do{
            std::string relativePath = StringUtils::format("levels/level %d - %d",levelNumber/4 +1,levelNumber%4 +1);
            fullPath = FileUtils::getInstance()->getWritablePath()+ relativePath + ".xml";
            fileExists = FileUtils::getInstance()->isFileExist(fullPath);
            
            Text* label = Text::create();
            if (fileExists) {
                label->setString(StringUtils::format("level %d - %d",levelNumber/4 +1,levelNumber%4 +1));
            }else{
                label->setString("New Level");
            }
            label->setFontName("Marker Felt.ttf");
            label->setFontSize(36);
            label->setContentSize(cocos2d::Size(100,40));
            label->setTouchEnabled(true);
            label->setTouchScaleChangeEnabled(true);
            
            Layout* label_item = Layout::create();
            label_item->setTouchEnabled(true);
            label_item->setContentSize(label->getContentSize());
            label->setPosition(Vec2(label_item->getContentSize().width/2,label_item->getContentSize().height/2));
            label_item->addChild(label);
            label_item->setTag(levelNumber);
            
            listView->addChild(label_item);
            levelNumber++;
        }while (fileExists);

        listView->setContentSize(cocos2d::Size(WinSize.width,WinSize.height));
        listView->setGravity(cocos2d::ui::ListView::Gravity::CENTER_HORIZONTAL);
        listView->setItemsMargin(50.0);
        listView->addEventListener(CC_CALLBACK_2(LevelsMenu::selectLevel, this));
        this->addChild(listView);
        
        return true;
    }
    
    return false;
}

void LevelsMenu::selectLevel(cocos2d::Ref *sender, cocos2d::ui::ListView::EventType eventtype)
{
    ListView* listView = static_cast<ListView*>(sender);
    long levelNumber = listView->getCurSelectedIndex();
    std::string fileName = StringUtils::format("levels/level %ld - %ld",levelNumber/4 + 1,levelNumber%4 + 1);
    LevelFileHandler* fileHandler = LevelFileHandler::createWithFileName(fileName.c_str());
    GameManager* gameManager = GameManager::getInstance();
    gameManager->_fileHandler = fileHandler;
    gameManager->_fileHandler->retain();
    gameManager->scrollViewOffset = 0.0;
    
    switch (eventtype) {
        case ui::ListView::EventType::ON_SELECTED_ITEM_START:
            
            break;
        case ui::ListView::EventType::ON_SELECTED_ITEM_END:
            
            if(levelNumber+1 == listView->getItems().size()){
                Director::getInstance()->replaceScene(LevelEditor::createScene());
            }else{
                Director::getInstance()->replaceScene(SceneGame::createScene());
            }
            break;
    }

}