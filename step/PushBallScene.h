//
//  PushBallScene.h
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/15.
//
//

#ifndef __KFOOD1105_GumballCandymaker__PushBallScene__
#define __KFOOD1105_GumballCandymaker__PushBallScene__

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"

using namespace ui;
USING_NS_CC;

//步骤的基本模板
class PushBallScene:public GameBaseScene,public ToolSpriteDelegate{
private:
    PushBallScene();
    ~PushBallScene();
public:
    virtual bool init();
    CREATE_FUNC(PushBallScene);
//    CREATE_SCENE(PushBallScene);
    
    static Scene* createScene();
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
public:
    void initUI();
    
    void initBall();
    void initMac();
    void pushBall();
    void showCoin();
    void coinAction();
public:
    //下一步
    void nextStep();
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchMove(ToolSprite* toolSprite,Touch *pTouch){};
    void onTouchUp(ToolSprite* toolSprite,Touch *pTouch){};
    void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index){};
public:
    Sprite* m_pBG;
    ToolSprite* m_pGlass;
    Sprite* m_pMac;
    
    ui::Widget *ui_button;
    ui::Widget *ui_next;
    ui::Widget *ui_coin;
    
    int m_iMacNum;
};

#endif /* defined(__KFOOD1105_GumballCandymaker__PushBallScene__) */
