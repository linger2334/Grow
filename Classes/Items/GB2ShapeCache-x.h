
#ifndef GB2ShapeCache_x_h
#define GB2ShapeCache_x_h

#include "cocos2d.h"

class BodyDef;
class b2Body;

namespace cocos2d {
	class GB2ShapeCache {
	public:
		// 全局单例
		static GB2ShapeCache* getInstance(void);
		
	public:
		bool init();			
		void addShapesWithFile(const std::string &plist);
		void addFixturesToBody(b2Body *body, const std::string &shape);
		cocos2d::Point anchorPointForShape(const std::string &shape);
		void reset();
		float getPtmRatio() { return ptmRatio; }
		~GB2ShapeCache() { }
		
	private:
        //这里只能声明,类里面的静态变量要在类外部定义
        static GB2ShapeCache *_sharedGB2ShapeCache;
		std::map<std::string, BodyDef *> shapeObjects;
		GB2ShapeCache(void) {}
		float ptmRatio;
        
        class CGarbo{
        public:
            ~CGarbo()
            {
                if(GB2ShapeCache::_sharedGB2ShapeCache){
                    delete GB2ShapeCache::_sharedGB2ShapeCache;
                }
            }
        };
        static CGarbo Garbo;
	};
}

#endif
