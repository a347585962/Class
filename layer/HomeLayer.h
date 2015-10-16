//
//  HomeLayer.h
//  OreoMaker
//
//  Created by wangbosheng1 on 14-10-22.
//
//

#ifndef __OreoMaker__HomeLayer__
#define __OreoMaker__HomeLayer__

#include <iostream>
#include "cocos2d.h"
#include "AdapterScreen.h"
#include "BaseLayer.h"
#include "Utils.h"
#include "SceneManager.h"
#include "ToolSprite.h"

USING_NS_CC;

enum{
    eTagStart,
    eTagFavorite,
    eTagShop,
    eTagSetting,
    eTagPrivacy,
    eTagMoreGame,
    eMiniGame,
    eRestore,
};
class HomeLayer:public BaseLayer,public ToolSpriteDelegate{
    
private:
    HomeLayer();
    ~HomeLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(HomeLayer);
    
public:
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
public:
    void initUI();
    void onBtnCallBack(Ref* sender);
       
    //展示全屏广告的回调
    virtual void showAdsLaod(EventCustom*);
    //关闭全屏广告的回调
    virtual void closeAdsLaod(EventCustom*);
    
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    
    void ShowUIMenu();
    void ShowLogo();
    void ShowUIPlay();
    void showHotDog();
    void showBear();
    void ballBomb(Sprite* ball);
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    
    void AfterRestoreCallBack(bool isRestore,bool isPurchase);
    
    void MaterialComing1(float dt);
    void MaterialComing2();
    void MaterialComing3();

private:
    ui::Widget *ui_button;
    ui::Widget *m_pStartItemSprite;
    ui::Widget *m_pShopItemSprite;
    ui::Widget *m_pMoreItemSprite;
//    ui::Widget *m_pFavItemSprite;
    ui::Widget *m_pResItemSprite;
    
    
    ToolSprite* m_pLogo;
    Sprite* m_pBgSprite;
//    Sprite* m_pCrab;
//    Sprite* m_pBear;
    Sprite* m_pPedestal;
    Sprite* m_pLidCover;
    Vector<Sprite*> m_vBall;
//    Sprite* m_pHotDog;
};

#endif /* defined(__OreoMaker__HomeLayer__) */
