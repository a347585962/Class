//
//  MakeBatterScene.h
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/14.
//
//

#ifndef __KFOOD1105_GumballCandymaker__MakeBatterScene__
#define __KFOOD1105_GumballCandymaker__MakeBatterScene__

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"

using namespace ui;
USING_NS_CC;


class MakeBatterScene:public GameBaseScene,public ToolSpriteDelegate{
private:
    MakeBatterScene();
    ~MakeBatterScene();
public:
    virtual bool init();
    CREATE_FUNC(MakeBatterScene);
//    CREATE_SCENE(MakeBatterScene);
    
    static Scene* createScene();
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
public:
    void initUI();
public:
    //下一步
    void nextStep();
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    
    bool onTouchBegan(Touch *touch, Event *unused_event);
    void onTouchMoved(Touch *touch, Event *unused_event);
    void onTouchEnded(Touch *touch, Event *unused_event);
    
    //toolSprite
    void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchMove(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
    
    void showKnife();
    void showLine(int num);
    void showGlass();
    void showCut();
    void showHand();
    void showHand1();
    
    void addToGlass(float dt);
public:
    Sprite* m_pBG;
    ui::Widget *ui_button;
    ui::Widget *ui_next;
    
    Sprite* m_pBoard;
    Sprite* m_pBatter;
    Sprite* m_pGlass;
    
    ToolSprite* m_pKnife;
    
    int m_iFriutNum;
    string m_sMixStr;
    int m_iBatterMoveNum;
    int m_iLineNum;
    int m_iBallTag;
    bool m_pTouch;
    int m_iBallNum;
    
    bool m_bKnife;
    bool m_bIsAction;
    Vec2 oldPos;
    Vector<ToolSprite*> m_vToolCut;
};

#endif /* defined(__KFOOD1105_GumballCandymaker__MakeBatterScene__) */
