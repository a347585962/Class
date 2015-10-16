//
//  SelectMacScene.h
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/15.
//
//

#ifndef __KFOOD1105_GumballCandymaker__SelectMacScene__
#define __KFOOD1105_GumballCandymaker__SelectMacScene__

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"

using namespace ui;
USING_NS_CC;

//步骤的基本模板
class SelectMacScene:public GameBaseScene,public ToolSpriteDelegate{
private:
    SelectMacScene();
    ~SelectMacScene();
public:
    virtual bool init();
    CREATE_FUNC(SelectMacScene);
//    CREATE_SCENE(SelectMacScene);
    
     static Scene* createScene();
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
public:
    void initUI();
    void initData();
    
     void touchEvent(Ref *obj , Widget::TouchEventType type);
    void typeItemClick(int tag,MenuItem* menuItem,int index);
    
    
    
    
public:
    //下一步
    void nextStep();
public:
    Sprite* m_pBG;
    
    
    ui::Widget *ui_button;
    ui::Widget *ui_next;
    
    Vector<Sprite*> m_vTypes;
    KDGridView* m_pGirdView;
    KSSimpleAdapter* m_pAdapter;
    MenuItemSprite* m_pCurrentSprite;
    
    int m_iTouchIndex;
    ParticleSystemQuad* chooseParticle;
    
    
};

#endif /* defined(__KFOOD1105_GumballCandymaker__SelectMacScene__) */
