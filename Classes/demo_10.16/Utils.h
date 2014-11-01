//
//  Utils.h
//  GrowUp
//
//  Created by wlg on 14-8-11.
//
//

#ifndef GrowUp_Utils_h
#define GrowUp_Utils_h

#include "Macro.h"

class Utils
{
public:
    static void replaceLayer(Layer* layer)
    {
        Scene* scene = Scene::create();
        scene->addChild(layer);
        Director::getInstance()->replaceScene(scene);
    }
    
    static void runWithLayer(Layer* layer)
    {
        Scene* scene = Scene::create();
        scene->addChild(layer);
        Director::getInstance()->runWithScene(scene);
    }
    
    static Sprite* addSprite(Node* parent,const char* filename,int localzorder=0,int tag=0)
    {
        Sprite* sprite = Sprite::create(filename);
        parent->addChild(sprite);
        sprite->setPosition(Vec2(WinSize.width/2,WinSize
                                 .height/2));
        sprite->setLocalZOrder(localzorder);
        sprite->setTag(tag);
        return sprite;
    }
    
    static void moveNode(Node* node,const Point& off)
    {
        node->setPosition(node->getPosition()+off);
    }
    
    static char* format(int number,const char* prefix = "",const char* suffix = "")
    {
        static char buf[512];
        sprintf(buf,"%s%d%s",prefix,number,suffix);
        return buf;
    }
    
    static MenuItem* createMenuItemRB(Node* parent,const char* png1,const char* png2,ccMenuCallback menucallback)
    {
        MenuItem* item = MenuItemImage::create(png1,png2,menucallback);
        Menu* menu = Menu::createWithItem(item);
        parent->addChild(menu);
        item->setPosition(Vec2(WinSize.width/2 - item->getBoundingBox().size.width/2,item->getBoundingBox().size.height/2 - WinSize.height/2));
        return item;
    }
    
    static void loadAnimation_Plist(const char* aniName,const char* plist,int frameCount,float delay,const char* prefix,const char* suffix)
    {
        SpriteFrameCache* cache = SpriteFrameCache::getInstance();
        cache->addSpriteFramesWithFile(plist);
        Vector<SpriteFrame*>vec;
        for (int i= 0; i<frameCount; i++) {
            SpriteFrame* frame = cache->getSpriteFrameByName(format(i+1,prefix,suffix));
            vec.pushBack(frame);
        }
        
        Animation* animation = Animation::createWithSpriteFrames(vec,delay);
        AnimationCache::getInstance()->addAnimation(animation,aniName);
    }
    
    static void loadAnimation_Png(const char* aniName,const char* pngFile,int frameCount,float delay,int width,int height,int startIdx = 0)
    {
        Texture2D* texture = TextureCache::getInstance()->addImage(pngFile);
        Vector<SpriteFrame*> vec;
        for (int i = startIdx; i<frameCount + startIdx; i++) {
            SpriteFrame* spriteframe = SpriteFrame::createWithTexture(texture,Rect(i*width,0,width,height));
            vec.pushBack(spriteframe);
        }
        
        Animation* animation = Animation::createWithSpriteFrames(vec,delay);
        AnimationCache::getInstance()->addAnimation(animation,aniName);
    }
    
    static void loadSpriteFrame_Png(const char* pngFile,int idx,const char* frameName,int width,int height)
    {
        Texture2D* texture = TextureCache::getInstance()->addImage(pngFile);
        SpriteFrame* spriteframe = SpriteFrame::createWithTexture(texture,Rect(idx*width,0,width,height));
        SpriteFrameCache::getInstance()->addSpriteFrame(spriteframe,frameName);
    }
    
};

#endif
