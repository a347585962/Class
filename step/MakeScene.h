//
//  MakeScene.h
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/9.
//
//

#ifndef __KFOOD1105_GumballCandymaker__MakeScene__
#define __KFOOD1105_GumballCandymaker__MakeScene__

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"

using namespace ui;
USING_NS_CC;

enum makeTool{
    kMakeSwitch,
    kMakeFruit,
    kMakeGum,
    kMakeWater,
    kMakeSyrup,
    kMakeChicle,
    kMakeSugar,
    kMakeLadle,
};


//make
class MakeScene:public GameBaseScene,public ToolSpriteDelegate{
private:
    MakeScene();
    ~MakeScene();
public:
    virtual bool init();
    CREATE_FUNC(MakeScene);
    CREATE_SCENE(MakeScene);
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
public:
    void initUI();
public:
    //下一步
    void nextStep();
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    
    void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
    void onTouchMove(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchMoveOutRect(ToolSprite* toolSprite,Touch *pTouch);
    
    void addFuritToStoven(int num);
    void toolAdd();
    void _toolAdd();
    void checkStep(float dt);
    void sugarAction(float dt);
     void waterAction(float dt);
    void showLadie();
    void showHand();
    //重力感应
    virtual void didAccelerate(Acceleration* acc, Event* event);
public:
    Sprite* m_pBG;
    
    ui::Widget *ui_button;
    ui::Widget *ui_next;
    
    Sprite* m_pPotUnder;
    Sprite* m_pFire;
    ToolSprite* m_pSwitch;
    
    double m_dAccX;
    int m_iFriutNum;
    int m_iToolZorder;
    int m_iStepNum;
    Rect m_rStoveRect;
    bool m_bCloseDevice;
    Vec2 oldPos;
    bool m_bSyrupMove;
    Sprite* m_pBowlMix;
    string m_sMixStr;
    Sprite* m_pHand;
    
    int m_iLadleNum;
    bool m_bIsSounds;
    bool isTouch;
    bool m_pSwitchOff;
};

#endif /* defined(__KFOOD1105_GumballCandymaker__MakeScene__) */
