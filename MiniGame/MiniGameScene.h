//
//  MiniGameScene.h
//  CocosGame
//
//  Created by zhangdingquan on 15/10/15.
//
//

#ifndef __CocosGame__MiniGameScene__
#define __CocosGame__MiniGameScene__

#include <stdio.h>
#include "cocos2d.h"
#include "AdapterScreen.h"

USING_NS_CC;
using namespace std;

class MiniGameScene : public Layer {
public:
    MiniGameScene();
    ~MiniGameScene();
    virtual bool init();
    CREATE_FUNC(MiniGameScene);
    static Scene *scene();
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
protected:
    Size m_siVisibleSize;
    
protected:
    Sprite* m_pSightSp;
    void initSightSp();
    EventListenerTouchOneByOne* m_pSightListner;
    
protected:
    Sprite* m_pWarheadSp;
    bool m_bWarheadRunning;
    void initWarheadSp();
    void warheadRun();
    
protected:
    int m_iDizzyCount;
    float m_fDizzyRunTime;
    Vector<Sprite*> m_vDizzySp;
    vector<Vec2>    m_vDizzyPos;
    bool m_bWarheadReady;
    
    void initDizzyPos();
    void makeDizzySp(int count);
    void startAction(float dt);
    
    virtual void update(float dt);
    void checkCollision();
    int m_iWarHead;
};

#endif /* defined(__CocosGame__MiniGameScene__) */
