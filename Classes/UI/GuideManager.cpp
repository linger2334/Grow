//
//  GuideManager.cpp
//  Grow-UI
//
//  Created by wlg on 12/2/14.
//
//

#include "GuideManager.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;

GuideManager* GuideManager::_sharedGuideManager = nullptr;
GuideManager::Recycle GuideManager:: _recycle;

GuideManager* GuideManager::getInstance()
{
    if (!_sharedGuideManager) {
        _sharedGuideManager = new GuideManager();
        _sharedGuideManager->init();
    }
    
    return _sharedGuideManager;
}

GuideInfo::GuideInfo(int _sequence,std::string& _instanceName,int _subSeq,bool _noSynchro)
{
    sequence = _sequence;
    instanceName = _instanceName;
    subSeq = _subSeq;
    noSynchro = _noSynchro;
}


void GuideManager::init()
{
    _setUp = false;
    _paused = false;
    _currentStep = -1;
    _nextStep = 0;
    levelid = -1;
}

void GuideManager::loadGuideXML()
{
    bool isFileExist = FileUtils::getInstance()->isFileExist("guide.xml");
    if (isFileExist) {
        XMLDocument* pDoc = new XMLDocument();
        pDoc->LoadFile("guide.xml");
        
        XMLElement* rootElement = pDoc->RootElement();
        XMLElement* levelElement = rootElement->FirstChildElement(StringUtils::format("level%d",levelid).c_str());
        std::vector<GuideInfo> phaseInfos;
        if (levelElement) {
            XMLElement* guidePhase = levelElement->FirstChildElement();
            int sequcnece;
            std::string instanceName;
            int subSeq;
            bool noSynchro = false;
            
            while (guidePhase) {
                sequcnece = guidePhase->IntAttribute("sequence");
                instanceName = guidePhase->Attribute("instanceName");
                subSeq = guidePhase->IntAttribute("subSeq");
                noSynchro = guidePhase->QueryBoolAttribute("noSynchro", &noSynchro);
                GuideInfo singleInfo(sequcnece,instanceName,subSeq,noSynchro);
                phaseInfos.push_back(singleInfo);
                
                guidePhase = guidePhase->NextSiblingElement();
            }
            
            std::sort(phaseInfos.begin(), phaseInfos.end());
        }
        
        if (pDoc) {
            delete pDoc;
            pDoc = nullptr;
        }
        
        setUp(phaseInfos);
        start();
    }
}

void GuideManager::setUp(std::vector<GuideInfo> &guidePhases)
{
    if (!_setUp) {
        _setUp = true;
        int len = guidePhases.size();
        for (int i =0; i<len; i++) {
            _phaseArray.push_back(guidePhases.at(i));
            _sequenceName.push_back(guidePhases.at(i).instanceName);
            _instanceQueue[i] = _memberMap[_sequenceName.at(i)];
            _finishList.push_back(false);
        }
    }
}

void GuideManager::uninstall()
{
    if (_setUp) {
        _setUp = false;
        if (_currentStep >= 0) {
            doClear(_instanceQueue.at(_currentStep));
        }
        _sequenceName.clear();
        _phaseArray.clear();
        _instanceQueue.clear();
        _finishList.clear();
        _memberMap.clear();
        _currentStep = -1;
        _nextStep = 0;
    }
}

void GuideManager::Register(GuideComponentDelegate *instance)
{
    if (instance) {
        std::string name = instance->getInstanceName();
        if (!_memberMap[name]) {
            _memberMap[name] = instance;
            
            if (_setUp) {
                for (int i = 0; i<_sequenceName.size(); i++) {
                    if (!_sequenceName.at(i).compare(name)) {
                        _instanceQueue.at(i) = instance;
                        if (i == _nextStep) {
                            goNextStep(i);
                        }
                    }
                }
            }
        }
    }
}

void GuideManager::start(int from/*0*/)
{
    goNextStep(from);
}

void GuideManager::goNextStep(int designedStep/*-1*/)
{
    if (_paused) {
        resumeGuide();
        return;
    }
    
    if (designedStep < 0) {
        _nextStep = _currentStep + 1;
    }else{
        _nextStep = designedStep;
    }
    
    if (_currentStep>=0 && _nextStep>0) {
        markFinish(_currentStep);
    }
    
    if (_nextStep<_instanceQueue.size() && _instanceQueue.at(_nextStep)) {
        
        _currentStep = _nextStep;
        _instanceQueue.at(_nextStep)->guideProcess(_phaseArray.at(_nextStep));
    }else if(_nextStep == _instanceQueue.size()){
        onGuideFinish();
        uninstall();
    }else{
        pauseGuide();
    }
}

void GuideManager::pauseGuide()
{
     if(!_paused)
         _paused = true;
}

void GuideManager::resumeGuide()
{
    if (_paused) {
        _paused = false;
        
        _currentStep = _nextStep;
        _instanceQueue.at(_nextStep)->guideProcess(_phaseArray.at(_nextStep));
    }
}

void GuideManager::markFinish(int index)
{
    if (!_finishList[index]) {
        doClear(_instanceQueue.at(index));
        onStepFinish(_phaseArray.at(index));
        _finishList[index] = true;
    }
}

void GuideManager::doClear(GuideComponentDelegate* instance)
{
    if (instance) {
        instance->guideClear();
        //        hideBorder();
        hideScreenMask();
    }
}

void GuideManager::onStepFinish(GuideInfo &guidePhase)
{
    
}

void GuideManager::onGuideFinish()
{
    
}
