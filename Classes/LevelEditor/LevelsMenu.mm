//
//  LevelsMenu.cpp
//  GrowUp
//
//  Created by wlg on 9/16/14.
//
//

#include "LevelsMenu.h"
#include "LevelFileHandler.h"
#include "LevelEditor.h"
#include "GameManager.h"

#define LEVEL1_COUNT 4
#define LEVEL2_COUNT 6
#define LEVEL3_COUNT 8
#define LEVEL4_COUNT 10

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
        
        std::string relativePath;
        std::string fullPath;
        char labelString[30];
        bool fileExists;
        consecutiveNumbers = 0;
        memset(fileExist, 0, sizeof(fileExist));
        
        sumCount1 = LEVEL1_COUNT;
        sumCount2 = LEVEL1_COUNT + LEVEL2_COUNT;
        sumCount3 = LEVEL1_COUNT + LEVEL2_COUNT + LEVEL3_COUNT;
        sumCount4 = LEVEL1_COUNT + LEVEL2_COUNT + LEVEL3_COUNT + LEVEL4_COUNT;
        
        for (int levelNumber = 0;levelNumber<sizeof(fileExist); levelNumber++) {
            
            if (levelNumber < sumCount1) {
                relativePath = StringUtils::format("levels/level %d - %d",1,levelNumber +1);
            }else if (levelNumber<sumCount2 && levelNumber>=sumCount1){
                relativePath = StringUtils::format("levels/level %d - %d",2,levelNumber-sumCount1 +1);
            }else if (levelNumber<sumCount3 && levelNumber>=sumCount2){
                relativePath = StringUtils::format("levels/level %d - %d",3,levelNumber-sumCount2 +1);
            }else if (levelNumber<sumCount4 && levelNumber>=sumCount3){
                relativePath = StringUtils::format("levels/level %d - %d",4,levelNumber-sumCount3 +1);
            }else{
                break;
            }
            strncpy(labelString, relativePath.c_str() + strlen("levels/"), sizeof(labelString));
            fullPath = FileUtils::getInstance()->getWritablePath()+ relativePath + ".xml";
            fileExists = FileUtils::getInstance()->isFileExist(fullPath);

            if (fileExists) {
                Text* label = Text::create();
                label->setString(labelString);
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
                fileExist[levelNumber] = 1;
            }
        }
        
        for (; consecutiveNumbers<sizeof(fileExist); consecutiveNumbers++) {
            if (!fileExist[consecutiveNumbers]) {
                break;
            }
        }
        
        if (consecutiveNumbers < sumCount4) {
            Text* newLevelLabel = Text::create("NewLevel", "Marker Felt.ttf", 36);
            newLevelLabel->setContentSize(cocos2d::Size(100,40));
            newLevelLabel->setTouchEnabled(true);
            newLevelLabel->setTouchScaleChangeEnabled(true);
            Layout* newlabel_layout = Layout::create();
            newlabel_layout->setTouchEnabled(true);
            newlabel_layout->setContentSize(newLevelLabel->getContentSize());
            newLevelLabel->setPosition(Vec2(newlabel_layout->getContentSize().width/2,newlabel_layout->getContentSize().height/2));
            newlabel_layout->addChild(newLevelLabel);
            listView->addChild(newlabel_layout);
        }
        
        listView->setContentSize(cocos2d::Size(VisibleSize.width,VisibleSize.height));
        listView->setGravity(cocos2d::ui::ListView::Gravity::CENTER_HORIZONTAL);
        listView->setItemsMargin(50.0);
        listView->addEventListener(CC_CALLBACK_2(LevelsMenu::selectLevel, this));
        this->addChild(listView);
        
        return true;
    }
    
    return false;
}
#include "LevelManager.h"

void LevelsMenu::selectLevel(cocos2d::Ref *sender, cocos2d::ui::ListView::EventType eventtype)
{
    ListView* listView = static_cast<ListView*>(sender);
    int fileIndex = 0;
    long selectedIndex = listView->getCurSelectedIndex() + 1;
    int i;
    for ( i = 0; i<sizeof(fileExist); i++) {
        if (fileExist[i]) {
            if (!--selectedIndex) break;
            
        }
    }
    //选中newlevel,那就是连续号的下一位
    if (selectedIndex == 1 ) {
        fileIndex = consecutiveNumbers;
    }else{
    //否则就是查表位
        fileIndex = i;
    }

    int levelid = 0;
    std::string fileName;
    if (fileIndex<sumCount1) {
        levelid = 1;
        fileName = StringUtils::format("levels/level %d - %d",levelid,fileIndex + 1);
    }else if (fileIndex<sumCount2 && fileIndex>=sumCount1){
        levelid = 2;
        fileName = StringUtils::format("levels/level %d - %d",levelid,fileIndex-sumCount1 + 1);
    }else if (fileIndex<sumCount3 && fileIndex>=sumCount2){
        levelid = 3;
        fileName = StringUtils::format("levels/level %d - %d",levelid,fileIndex-sumCount2 + 1);
    }else if (fileIndex<sumCount4 && fileIndex>=sumCount3){
        levelid = 4;
        fileName = StringUtils::format("levels/level %d - %d",levelid,fileIndex-sumCount3 + 1);
    }else{
        return;
    }
    
    LevelFileHandler* fileHandler = LevelFileHandler::createWithFileName(fileName.c_str());
    GameManager* gameManager = GameManager::getInstance();
    gameManager->_fileHandler = fileHandler;
    
    switch (eventtype) {
        case ui::ListView::EventType::ON_SELECTED_ITEM_END:
            
            LevelManager::getInstance()->selectLevel(levelid);
            if(selectedIndex == 1){
                Director::getInstance()->replaceScene(LevelEditor::createScene());
            }else{
                auto manager = GameManager::getInstance();
                manager->_levelFileName = fileName;
                manager->navigationToGameScene();
            }
            
            break;
            
        default:
            break;
    }

}