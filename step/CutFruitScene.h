//
//  CutFruitScene.h
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/9.
//
//

#ifndef __KFOOD1105_GumballCandymaker__CutFruitScene__
#define __KFOOD1105_GumballCandymaker__CutFruitScene__

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"

using namespace ui;
USING_NS_CC;

enum{
    eTagKnife,
    eTagMachine,
    eTagIcon,
    eTagNext
};



class CutFruitScene:public GameBaseScene,public ToolSpriteDelegate{
private:
    CutFruitScene();
    ~CutFruitScene();
public:
    virtual bool init();
    CREATE_FUNC(CutFruitScene);
    CREATE_SCENE(CutFruitScene);
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
public:
    void initUI();
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchMove(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
public:
    //下一步
    void nextStep();
    
    void chooseKnife(int fruit);
    void chooseMac(int fruit);
    
    void checkFriute(string furite);
    int checkFriute4(string furite);
    int checkFriute3(string furite);
    int checkFriute2(string furite);
    
public:
    Sprite* m_pBG;
    
    ui::Widget *ui_button;
    ui::Widget *ui_next;
    ui::Widget *ui_knife;
    ui::Widget *ui_machine;
    ui::Widget *ui_icon;
    ui::Widget *ui_choose_button;
    
    LayerColor* m_pChooseToolsLayer;
    
    
    Layer* m_pOperateLayer;
    
    int m_iFriutNum;
    
    ToolSprite* m_pKnife;
    
    Vec2 oldPos;
    
};



#endif /* defined(__KFOOD1105_GumballCandymaker__CutFruitScene__) */
