#include "FlowerScene.h"


bool FlowerScene::init()
{
    
    if (!Scene::init()) {
        return false;
    }
    
    Layout* layout = Layout::create();
    layout->setSize(Size(winSize.width, winSize.height));
    //layout->setColor(Color3B(0, 0, 255));
    layout->setPosition(Point(0,0));
    //layout->setAnchorPoint(1,1);
    addChild(layout);
    
    
    ui::ScrollView* scrollView = ui::ScrollView::create();
    scrollView->setSize(Size(winSize.width/1.1 , winSize.height ));
    
    scrollView->setPosition(Point(winSize.width/2, 0));
    layout->addChild(scrollView);
  
    auto button = MenuItemImage::create("CloseNormal.png",
                                        "CloseNormal.png",
                                        [](Ref* pSender)
                                        {
 //                                           auto scene =  GameScene::create();
//                                            Director::getInstance()->replaceScene(scene);
                                            //Director::getInstance()->replaceScene(TransitionPageTurn::create(1,scene,true));
                                        });
    button->setPosition(Point(300,400));
    scrollView->addChild(button);

    ImageView* imageView = ImageView::create("stone01.png");
    imageView->setScale(0.5,0.5);
    float innerWidth = scrollView->getContentSize().width;
    float innerHeight = scrollView->getContentSize().height + imageView->getContentSize().height;
    
    scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));
    
    
//    Button* button = Button::create("CloseSelected.png", "CloseSelected.png");
//    button->setPosition(Point(innerWidth / 2.0f, scrollView->getInnerContainerSize().height - button->getContentSize().height / 2.0f));
//    scrollView->addChild(button);
    
//    Button* titleButton = Button::create("CloseSelected.png", "CloseSelected.png");
//    titleButton->setPosition(Point(innerWidth / 2.0f, 300));
//    scrollView->addChild(titleButton);
    
//    Button* button_scale9 = Button::create("CloseSelected.png", "CloseSelected.png");
//    button_scale9->setScale9Enabled(true);
//    button_scale9->setSize(Size(100.0f, button_scale9->getVirtualRendererSize().height));
//    button_scale9->setPosition(Point(innerWidth / 2.0f, titleButton->getBottomInParent() - titleButton->getSize().height));
//    scrollView->addChild(button_scale9);
    
    
    imageView->setPosition(Point(innerWidth / 2.0f, imageView->getSize().height / 2.0f));
    scrollView->addChild(imageView);

    return true;
}