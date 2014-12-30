//
//  GuideManager.cpp
//  Grow-UI
//
//  Created by wlg on 12/2/14.
//
//

#include "GuideManager.h"
#include "tinyxml2/tinyxml2.h"
#include "StatisticsData.h"
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
    std::string filePath = FileUtils::getInstance()->fullPathForFilename("guide.xml");
    bool isFileExist = FileUtils::getInstance()->isFileExist(filePath);
    if (isFileExist) {
        XMLDocument* pDoc = new XMLDocument();
        std::vector<GuideInfo> phaseInfos;
        int ret = pDoc->LoadFile(filePath.c_str());
        if (ret == XML_SUCCESS) {
            XMLElement* rootElement = pDoc->RootElement();
            XMLElement* levelElement = rootElement->FirstChildElement(StringUtils::format("level%d",levelid).c_str());
            
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
                    guidePhase->QueryBoolAttribute("noSynchro", &noSynchro);
                    GuideInfo singleInfo(sequcnece,instanceName,subSeq,noSynchro);
                    phaseInfos.push_back(singleInfo);
                    
                    noSynchro = false;
                    guidePhase = guidePhase->NextSiblingElement();
                }
                
                std::sort(phaseInfos.begin(), phaseInfos.end());
            }
        }else{
            log("XMLError is %d",ret);
        }
        
        if (pDoc) {
            delete pDoc;
            pDoc = nullptr;
        }
        
        setUp(phaseInfos);
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
            _instanceQueue.push_back(_memberMap[_sequenceName.at(i)]);
            _finishList.push_back(false);
        }
    }
}

void GuideManager::uninstall()
{
    if (_setUp) {
        _setUp = false;
        _paused = false;
        levelid = -1;
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

void GuideManager::unRegister(GuideComponentDelegate *instance)
{
    if (instance) {
        std::string name = instance->getInstanceName();
        if (_memberMap[name]) {
            for (int i = 0; i<_sequenceName.size(); i++) {
                if (!_sequenceName.at(i).compare(name)) {
                    _instanceQueue.at(i) = nullptr;
                }
            }
        }
    }
}

void GuideManager::start(int from/* = 0*/)
{
    goNextStep(from);
}

void GuideManager::goNextStep(int designedStep/* = -1*/)
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

int GuideManager::getStepIndexBySequence(int sequenceNum)
{
    int len = _phaseArray.size();
    for (int i = 0; i<len; i++) {
        if (_phaseArray.at(i).sequence == sequenceNum) {
            return i;
        }
    }
    
    return 0;
}

int GuideManager::getSequenceNumByIndex(int index)
{
    int len = _phaseArray.size();
    CCASSERT(index>=0 && index<len, "index is not valid!");
    return _phaseArray.at(index).sequence;
}

void GuideManager::onStepFinish(GuideInfo &guidePhase)
{
    if (!guidePhase.noSynchro) {
        StatisticsData::getRunningLayer()->guideSequence = getSequenceNumByIndex(_currentStep + 1);
    }
}

void GuideManager::onGuideFinish()
{
    uninstall();
}
