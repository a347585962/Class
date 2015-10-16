//
//  FlowFreeGameScene.h
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/15.
//
//

#ifndef __KFOOD1105_GumballCandymaker__FlowFreeGameScene__
#define __KFOOD1105_GumballCandymaker__FlowFreeGameScene__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class FlowFreeGameScene : public Layer {
public:
    FlowFreeGameScene();
    ~FlowFreeGameScene();
    virtual bool init();
    CREATE_FUNC(FlowFreeGameScene);
    static Scene *scene();
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
};



#endif /* defined(__KFOOD1105_GumballCandymaker__FlowFreeGameScene__) */
