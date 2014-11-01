
#include "GB2ShapeCache-x.h"
#include "Box2D/Box2D.h"
#include "CCNS.h"

using namespace cocos2d;

/**
 * 自定义夹具类，封装
 */
class FixtureDef {
public:
    FixtureDef()
    : next(NULL) {}
    
    ~FixtureDef() {
        delete next;
        delete fixture.shape;
    }
    
    FixtureDef *next;
    b2FixtureDef fixture;
    int callbackData;
};

class BodyDef {
public:
	BodyDef()
	: fixtures(NULL) {}
	
	~BodyDef() {
		if (fixtures)
			delete fixtures;
	}
	
	FixtureDef *fixtures;
    cocos2d::Point anchorPoint;
};

GB2ShapeCache* GB2ShapeCache::_sharedGB2ShapeCache = nullptr;
GB2ShapeCache::CGarbo GB2ShapeCache::Garbo;

GB2ShapeCache* GB2ShapeCache::getInstance(void) {
	if (!_sharedGB2ShapeCache) {
		_sharedGB2ShapeCache = new GB2ShapeCache();
        _sharedGB2ShapeCache->init();
	}
	
	return _sharedGB2ShapeCache;
}

bool GB2ShapeCache::init() {
	return true;
}

void GB2ShapeCache::reset() {
	std::map<std::string, BodyDef *>::iterator iter;
	for (iter = shapeObjects.begin() ; iter != shapeObjects.end() ; ++iter) {
		delete iter->second;
	}
	shapeObjects.clear();
}

void GB2ShapeCache::addFixturesToBody(b2Body *body, const std::string &shape) {
	std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find(shape);
	assert(pos != shapeObjects.end());
	
	BodyDef *so = (*pos).second;

	FixtureDef *fix = so->fixtures;
    while (fix) {
        body->CreateFixture(&fix->fixture);
        fix = fix->next;
    }
}

cocos2d::Point GB2ShapeCache::anchorPointForShape(const std::string &shape) {
	std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find(shape);
	assert(pos != shapeObjects.end());
	
	BodyDef *bd = (*pos).second;
	return bd->anchorPoint;
}

void GB2ShapeCache::addShapesWithFile(const std::string &plist) {
    std::string fullName = FileUtils::getInstance()->fullPathForFilename(plist);
	__Dictionary* dict = __Dictionary::createWithContentsOfFile(fullName.c_str());
	CCAssert(dict != NULL, "Shape-file not found"); // not triggered - cocos2dx delivers empty dict if non was found
    CCAssert(dict->count() != 0, "plist file empty or not existing");
	
	__Dictionary *metadataDict = (__Dictionary *)dict->objectForKey("metadata");
    int format = static_cast<__String *>(metadataDict->objectForKey("format"))->intValue();
    ptmRatio = static_cast<__String *>(metadataDict->objectForKey("ptm_ratio"))->floatValue();
	CCAssert(format == 1, "Format not supported");

	__Dictionary *bodyDict = (__Dictionary *)dict->objectForKey("bodies");

    b2Vec2 vertices[b2_maxPolygonVertices];
    
    DictElement* pElement;
	std::string bodyName;
	__Dictionary *bodyData;

        CCDICT_FOREACH(bodyDict,pElement){
        bodyName = pElement->getStrKey();
        bodyData =(__Dictionary*)pElement->getObject();
		BodyDef *bodyDef = new BodyDef();
		bodyDef->anchorPoint = PointFromString(static_cast<__String *>(bodyData->objectForKey("anchorpoint"))->getCString());
		
		__Array *fixtureList = (__Array *)(bodyData->objectForKey("fixtures"));
        FixtureDef **nextFixtureDef = &(bodyDef->fixtures);

        Ref* object;
		CCARRAY_FOREACH(fixtureList, object){
            b2FixtureDef basicData;
            __Dictionary *fixtureData = (__Dictionary*)object;
			
            basicData.filter.categoryBits = static_cast< __String*>(fixtureData->objectForKey("filter_categoryBits"))->intValue();
            basicData.filter.maskBits = static_cast<__String*>(fixtureData->objectForKey("filter_maskBits"))->intValue();
            basicData.filter.groupIndex = static_cast<__String *>(fixtureData->objectForKey("filter_groupIndex"))->intValue();
            basicData.friction = static_cast<__String *>(fixtureData->objectForKey("friction"))->floatValue();
            basicData.density = static_cast<__String *>(fixtureData->objectForKey("density"))->floatValue();
            basicData.restitution = static_cast<__String *>(fixtureData->objectForKey("restitution"))->floatValue();
            basicData.isSensor = (bool)static_cast<__String *>(fixtureData->objectForKey("isSensor"))->boolValue();

			__String *cb = static_cast<__String *>(fixtureData->objectForKey("userdataCbValue"));
			
            int callbackData = 0;
			
			if (cb)
				callbackData = cb->intValue();
            
			std::string fixtureType = static_cast<__String *>(fixtureData->objectForKey("fixture_type"))->getCString();
			
			if (fixtureType == "POLYGON") {
				__Array *polygonsArray = (__Array *)(fixtureData->objectForKey("polygons"));
				Ref* singlePolygon;
				CCARRAY_FOREACH(polygonsArray, singlePolygon){
                    FixtureDef *fix = new FixtureDef();
                    fix->fixture = basicData; // copy basic data
                    fix->callbackData = callbackData;
					
                    b2PolygonShape *polyshape = new b2PolygonShape();
                    int vindex = 0;
                    
					__Array *verticeArray = (__Array *)singlePolygon;

                    assert(verticeArray->count() <= b2_maxPolygonVertices);
					
                    Ref* singlePoint;
					CCARRAY_FOREACH(verticeArray, singlePoint){
                       __String* vert = (__String*)singlePoint;
                        cocos2d::Point offset = PointFromString(vert->getCString());
                        vertices[vindex].x = (offset.x / ptmRatio) ; 
                        vertices[vindex].y = (offset.y / ptmRatio) ; 
                        vindex++;
                    }
                    
                    polyshape->Set(vertices, vindex);
                    fix->fixture.shape = polyshape;
                    
                    // create a list
                    *nextFixtureDef = fix;
                    nextFixtureDef = &(fix->next);
				}
				
			} else if (fixtureType == "CIRCLE") {
				FixtureDef *fix = new FixtureDef();
                fix->fixture = basicData; // copy basic data
                fix->callbackData = callbackData;
                
                __Dictionary *circleData = (__Dictionary *)fixtureData->objectForKey("circle");
                
                b2CircleShape *circleShape = new b2CircleShape();
				
                circleShape->m_radius = static_cast<__String *>(circleData->objectForKey("radius"))->floatValue()/ ptmRatio;
                cocos2d::Point p = PointFromString(static_cast<__String *>(circleData->objectForKey("position"))->getCString());
                circleShape->m_p = b2Vec2(p.x / ptmRatio, p.y / ptmRatio);
                fix->fixture.shape = circleShape;
				
                // create a list
                *nextFixtureDef = fix;
                nextFixtureDef = &(fix->next);

			} else {
				CCAssert(0, "Unknown fixtureType");
			}
		}
        // add the body element to the hash
        shapeObjects[bodyName] = bodyDef;
	}
}