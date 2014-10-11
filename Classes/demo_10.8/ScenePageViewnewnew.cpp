//#include "ScenePageView.h"
//
//
//bool ScenePageView::init()
//{
//    if (!Scene::init())
//    {
//        return false;
//    }
//    //下面的4个点
//
//    Sprite* spri[4];
//    spri[0] = Sprite::create("pointselected.png");
//    spri[0]->setScale(0.5, 0.5);
//    addChild(spri[0],88888888);
//    spri[0]->setPosition(winSize.height/3,winSize.width/9.3);
//    spri[1] = Sprite::create("pointnormal.png");
//    spri[1]->setScale(0.5, 0.5);
//    addChild(spri[1],88888888);
//    spri[1]->setPosition(winSize.height/2.8,winSize.width/9.3);
//    spri[2] = Sprite::create("pointnormal.png");
//    spri[2]->setScale(0.5, 0.5);
//    addChild(spri[2],88888888);
//    spri[2]->setPosition(winSize.height/2.6,winSize.width/9.3);
//    spri[3]= Sprite::create("pointnormal.png");
//    spri[3]->setScale(0.5, 0.5);
//    addChild(spri[3],88888888);
//    spri[3]->setPosition(winSize.height/2.4,winSize.width/9.3);
//    
//    for (int i=1; i<=3;i++) {
//        spri[i]->setTexture("pointnormal.png");
//    }
//    
//
//    _pageView = PageView::create();
//    _pageView->setContentSize(Size(visible.width,visible.height));
//    _pageView->removeAllPages();
//
//    
//    //粒子效果
//    auto lizi = ParticleSystemQuad::create("lizi.plist");
//
//    lizi->setPosition(Vec2(0,0));
//    lizi->setLife(6);
//    lizi->setAnchorPoint(Vec2(0,0));
//    lizi->setColor(Color3B(255,0,0));
//    lizi->setSourcePosition(Vec2(winSize.width/2,visible.height/2));
//    lizi->setPositionType(ParticleSystem::PositionType::RELATIVE);
//
//    
//    //4个关卡
//    for (int i = 0; i<4; ++i)
//    {
//        Layout* layout = Layout::create();
//        //layout->setContentSize(Size(1024.0f, 768.0f));
//        layout->setContentSize(Size(visible.width,visible.height));
//        log("visible size %f,%f",visible.width,visible.height);
//        //layout->setBackGroundColor(Color3B(255,0,0));
//        auto imageView = Button::create(StringUtils::format("00%d.jpg",(i+1)));
//        imageView->setScale9Enabled(true); //取消点击后放大
// 
//        imageView->setScale(0.5,0.5);
//        imageView->setPosition(Vec2(layout->getContentSize().width/2 , layout->getContentSize().height/2 ));
//        layout->addChild(imageView);
//        //点击事件
//        if(i==0)
//        {
//            //标题
//            auto spring = Sprite::create("spring.png");
//            spring->setScale(0.5, 0.5);
//            spring->setPosition(winSize.width/2,winSize.height/1.2);
//            layout->addChild(spring,88888888);
//            
//            //灯光
//            auto sun = Sprite::create("sun.png");
//            
//            sun->setUserData((void*)true);
//            
//            auto func = [=](Ref* ref)
//            {
//                auto node =  static_cast<Node*>(ref);
//                bool flag = (bool)node->getUserData();
//                 auto o = node->getOpacity();
//                if(flag)
//                {
//                    if(o > 90)o -= 3;
//                    else flag =false;
//                }
//                else
//                {
//                    if(o < 255)o += 3;
//                    else flag =true;
//                }
//                sun->setUserData((void*)flag);
//                node->setOpacity(o);
//            };
//            sun->setAnchorPoint(Vec2(0,1));
//            
//            sun->setScale(0.5, 0.5);
//            sun->cocos2d::Node::setPosition(winSize.width/2.8,winSize.height/1.4);
//            layout->addChild(sun);
//            CallFuncN* call = CallFuncN::create(func);
//            Sequence* seq = Sequence::create(DelayTime::create(0.1),call,nullptr);
//            sun->setUserData((void*)true);
//            sun->runAction(RepeatForever::create(seq));
//            
//            //加入植物
//            auto plant1 = Sprite::create("plant1.png");
//            plant1->setScale(0.5, 0.5);
//            plant1->setPosition(winSize.width/2,winSize.height/3);
//            layout->addChild(plant1,88888888);
//            layout->addChild(lizi);
//            
//        }
//        else if(i==1)
//        {
//            auto summer = Sprite::create("summer.png");
//            summer->setScale(0.5, 0.5);
//            summer->setPosition(winSize.width/2,winSize.height/1.2);
//            layout->addChild(summer,88888888);
//            //灯光
//            auto sun = Sprite::create("sun.png");
//
//            sun->setAnchorPoint(Vec2(0,1));
//
//            sun->cocos2d::Node::setPosition(winSize.width/2.8,winSize.height/1.4);
//            layout->addChild(sun);
//
//        }
//        else if(i==2)
//        {
//            auto autumn = Sprite::create("autumn.png");
//            autumn->setScale(0.5, 0.5);
//            autumn->setPosition(winSize.width/2,winSize.height/1.2);
//            layout->addChild(autumn,88888888);
//            //灯光
//            auto sun = Sprite::create("sun.png");
//
//            sun->setAnchorPoint(Vec2(0,1));
//
//            sun->setScale(0.5, 0.5);
//            sun->cocos2d::Node::setPosition(winSize.width/2.8,winSize.height/1.4);
//            layout->addChild(sun);
//        }
//        else if(i==3)
//        {
//            auto winter = Sprite::create("winter.png");
//            winter->setScale(0.5, 0.5);
//            winter->setPosition(winSize.width/2,winSize.height/1.2);
//            layout->addChild(winter,88888888);
//            
//        }
//
//
//        
//        _pageView->insertPage(layout, i);
//        
////        LayerColor* colorbackground = LayerColor::create(Color4B(11, 11, 11, 255));
////        this->addChild(colorbackground);
//    
//    }
//
//
//
//    _pageView->addEventListener([=](Ref*,PageView::EventType type)
//                                {
////                                    Vec2 vec =lizi->getPosition();
////                                    lizi->setPosition(vec+Vec2(-100,-30));
//                                    static int oldIndex =0;
//                                    
//                                    for (int i=0; i<4; i++) {
//                                        spri[i]->setTexture("pointnormal.png");
//                                    }
//                              
//                                  
//                                    int i =_pageView->getCurPageIndex();
//                                    
//        
//                                    
//                                            if (i==0) {
//                                               
//                                            }
//                                           else  if (i==1) {
//                                               
//                                            }
//                                            else  if (i==2) {
//                                                
//                                            }
//                                            else  if (i==3) {
//                                               
//                                            }
//                                    spri[i]->setTexture("pointselected.png");
//                                });
//    _pageView->setPosition(Vec2(0, 0));
//    addChild(_pageView,0);
//    
//    return true;
//}
//
//void ScenePageView::onButtonClicked(Ref* pSender, Widget::TouchEventType type)
//{
//    switch (type) {
//        case Widget::TouchEventType::ENDED :{
//            ssize_t index = _pageView->getCurPageIndex();
//            
//            log("click index %zd",index);
//            auto scene = SceneProcess::create(index);//跳转到SceneProcess界面
//            Director::getInstance()->replaceScene(scene);
//            
//        }
//            break;
//        default:
//            break;
//    }
//    
//}
