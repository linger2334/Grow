//
//  GuideManager.h
//  Grow-UI
//
//  Created by wlg on 12/2/14.
//
//

#ifndef __Grow_UI__GuideManager__
#define __Grow_UI__GuideManager__

#include <stdio.h>
#include "Macro.h"

struct GuideInfo{
    int sequence;
    std::string instanceName;
    int subSeq;
    bool noSynchro;
    
    GuideInfo(int _sequence,std::string& _instanceName,int _subSeq,bool _noSynchro);
    bool operator <(const GuideInfo& other)const 
    {
        return sequence < other.sequence;
    }
};

class GuideComponentDelegate
{
public:
    virtual ~GuideComponentDelegate() { }
    virtual void guideProcess(GuideInfo& guidePhase) = 0;
    virtual void guideClear() = 0;
    virtual void onNextStep() = 0;
    CC_SYNTHESIZE(std::string, _instanceName, InstanceName)
};

class GuideManager
{
public:
    static GuideManager* getInstance();
    void init();
    void loadGuideXML();
    void setUp(std::vector<GuideInfo>& guidePhases);
    void uninstall();
    void Register(GuideComponentDelegate* instance);
    void start(int from=0);
    void goNextStep(int designedStep=-1);
    void pauseGuide();
    void resumeGuide();
    
    void showScreenMask(cocos2d::Rect showRect,float maskAlpha=0.5,int maskColor=0,Node* parent=nullptr) { }
    void hideScreenMask(std::string maskName = " ") { }
//    void showRectBorder(cocos2d::Rect bounds,Node* parent) { }
//    void hideBorder() { }
    inline bool isSetUp() { return _setUp;}
    inline bool isPaused() { return _paused;}
    inline int getCurrentStep() { return _currentStep;}
    void onStepFinish(GuideInfo& guidePhase);
    void onGuideFinish();
    int levelid;
protected:
    void doClear(GuideComponentDelegate* instance);
    void markFinish(int index);
    
    std::map<std::string,GuideComponentDelegate*> _memberMap;
    std::vector<GuideComponentDelegate*> _instanceQueue;
    bool _setUp;
    bool _paused;
    int _currentStep;
    int _nextStep;
    std::vector<std::string> _sequenceName;
    std::vector<GuideInfo> _phaseArray;
    std::vector<bool>_finishList;
    std::map<std::string,DrawNode*> _maskHome;
    DrawNode* _border;

private:
    static GuideManager* _sharedGuideManager;
    
    class Recycle{
    public:
       ~Recycle()
        {
            if (GuideManager::_sharedGuideManager) {
                delete _sharedGuideManager;
            }
        }
    };
    static Recycle _recycle;
};

#endif /* defined(__Grow_UI__GuideManager__) */
