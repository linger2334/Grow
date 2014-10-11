
#ifndef __UTIL_H__
#define __UTIL_H__



#include "Macro.h"

#define winSize Director::getInstance()->getVisibleSize()

#define enableTouch() do{ \
	setTouchEnabled(true); \
	setTouchMode(kCCTouchesOneByOne); \
}while (0)

struct Util
{
	
	static void replaceLayer(Layer* layer)
	{
		Scene* scene = Scene::create();
		scene->addChild(layer);
		Director::getInstacne()->replaceScene(scene);
	}

	static void runWithLayer(Layer* layer)
	{
		Scene* scene = Scene::create();
		scene->addChild(layer);
		Director::getInstacne()->runWithScene(scene);
	}

	static Sprite* addSprite(Node* parent, const char* filename, int zorder = 0, int tag = 0)
	{
		Sprite* sprite = Sprite::create(filename);
		parent->addChild(sprite);
		sprite->setPosition(Point(winSize.width / 2, winSize.height / 2));
		sprite->setZOrder(zorder);
		sprite->setTag(tag);
		return sprite;
	}

	static void scaleToFitWindow(Node* node)
	{
		node->setScaleX(winSize.width / node->getContentSize().width);
		node->setScaleY(winSize.height / node->getContentSize().height);
	}

	static void scaleToFitWindowY(Node* node, float margin = 0.0f)
	{
		node->setScale((winSize.height - margin * 2) / node->getContentSize().height);
	}

	static void scaleToFitWindowX(Node* node, float margin = 0.0f)
	{
		node->setScale((winSize.width - margin * 2) / node->getContentSize().width);
	}

	static void moveNode(Node* node, const Point& off)
	{
		node->setPosition(node->getPosition() + off);
	}

	static char* format(int number, const char* prefix = "", const char* suffix = "")
	{
		static char buf[1024];
		sprintf(buf, "%s%d%s", prefix, number, suffix);
		return buf;
	}

	static void setTexture(Sprite* sprite, const char* filename)
	{
		Texture2D* texture = TextureCache::sharedTextureCache()->addImage(filename);
		sprite->setTexture(texture);
		sprite->setTextureRect(CCRect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
	}

	static void scaleToSize(Node* node, const Size& size)
	{
		Size nodeSize = node->getContentSize();
		node->setScaleX(size.width / nodeSize.width);
		node->setScaleY(size.height / nodeSize.height);
	}

	static Point map2GL(const Point& ptMap, TMXTiledMap* map)
	{
		Point ptUI;
		ptUI.x = ptMap.x * map->getTileSize().width;
		ptUI.y = (ptMap.y + 1)* map->getTileSize().height;

		Point ptGL = ptUI;
		ptGL.y = map->getContentSize().height - ptUI.y;
		return ptGL;
	}
	static Point GL2map(const Point& ptGL, TMXTiledMap* map)
	{
		Point ptUI = ptGL;
		ptUI.y = map->getContentSize().height - ptGL.y;

		int x = ptUI.x / map->getTileSize().width;
		int y = ptUI.y / map->getTileSize().height;
		return ccp(x, y);
	}

	static MenuItem* createMenuItemRB(Node* parent, const char* png1, const char* png2,
		Node* target, SEL_MenuHandler selector)
	{
		MenuItem* item = MenuItemImage::create(png1,
			png2, target, selector);
		Menu* menu = Menu::createWithItem(item);
		parent->addChild(menu);
		item->setPosition(ccp(winSize.width / 2 - item->boundingBox().size.width / 2,
			item->boundingBox().size.height / 2 - winSize.height / 2));
		return item;
	}

	
	static void changeValue(LabelTTF* ttf, int cv)
	{
		const char* str = ttf->getString();
		int v = 0;
		if (str && strlen(str)>0)
			v = atoi(str);
		v += cv;
		ttf->setString(Util::format(v));
	}
	
	static bool isOutOfWindow(CCNode* node)
	{
		Point ptNode = node->getPosition();
		Point ptWorld = node->getParent()->convertToWorldSpace(ptNode);
#if 0
		if (ptWorld.x < 0) return true;
		else if (ptWorld.y < 0) return true;
		else if (ptWorld.x > winSize.width) return true;
		else if (ptWorld.y > winSize.height) return true;
#endif
#if 0
		if (ptWorld.x < 0 || ptWorld.y < 0 || ptWorld.x > winSize.width || ptWorld.y > winSize.height)
			return true;
		return false;
#endif

		return isOutOfWindow(ptWorld);
	}

	
	static bool isOutOfWindow(const CCPoint& ptWorld)
	{
		return (ptWorld.x < 0 || ptWorld.y < 0 || ptWorld.x > winSize.width || ptWorld.y > winSize.height);
	}

	
	/*static void loadAnimation(const char* aniName, const char* plist, int frameCount, float delay, const char* prefix, const char* suffix)
	{
		SpriteFrameCache* cache = SpriteFrameCache::sharedSpriteFrameCache();
		cache->addSpriteFramesWithFile(plist);
		CCArray* arr = CCArray::create();
		for (int i = 0; i < frameCount; i++)
		{
			CCSpriteFrame* frame = cache->spriteFrameByName(Util::format(i + 1, prefix, suffix));
			arr->addObject(frame);
		}

		CCAnimation* animation = CCAnimation::createWithSpriteFrames(arr, delay);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation, aniName);
	}
     */
};

class Panel : public Menu
{
public:
	CREATE_FUNC(Panel);

	bool TouchBegan(Touch* touch, Event* ev)
	{
		bool bTouch = Menu::TouchBegan(touch, ev);
		if (bTouch)
		{
			scheduleUpdate();
			return true;
		}
		return false;
	}

	void TouchEnded(CCTouch* touch, CCEvent* ev)
	{
		Menu::TouchEnded(touch, ev);
		unscheduleAllSelectors();
	}

	void update(float)
	{
		if (this->m_pSelectedItem && m_eState == kCCMenuStateTrackingTouch)
			this->m_pSelectedItem->activate();
	}
};

#endif