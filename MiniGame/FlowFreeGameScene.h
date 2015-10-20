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
    
    bool onTouchBegan(Touch *touch, Event *unused_event);
    void onTouchMoved(Touch *touch, Event *unused_event);
    void onTouchEnded(Touch *touch, Event *unused_event);
    
    void addFillBox(Vec2 pos,int colorType);
    bool checkPosIsInVector(Vec2 pos);
    bool checkPosIsInBoxCenter(Vec2 pos);
    
    //根据类型判断是否删除已有路径
    bool checkLoad(int type);
    void moveBack(Vec2 pos,int type);
private:
    void initBall();
    void initView();
    int checkTouchBall(Vec2 pos);
    Vec2 getFreamBoxPos(Vec2 pos);
    
    Sprite* m_pFream;
    Sprite* m_pBallShadow;
    Vec2 freamHasBoxButton[5][5];
    
    Vector<Sprite*> m_vGreenLoadSprite;
    Vector<Sprite*> m_vYellowLoadSprite;
    Vector<Sprite*> m_vBlueLoadSprite;
    Vector<Sprite*> m_vOrangeLoadSprite;
    Vector<Sprite*> m_vRedLoadSprite;
    vector<Vec2> m_vAllLoadPos;
   
    Vec2 mOldLoadPos;
    int m_iMoveCheckNum;
};



#endif /* defined(__KFOOD1105_GumballCandymaker__FlowFreeGameScene__) */
