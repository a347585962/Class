//
//  HomeLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-22.
//
//

#include "HomeLayer.h"
#include "SettingLayer.h"
#include "../Depends/modules/CFSystemFunction.h"

#include "../Depends/managers/IAPManager.h"
#include "../Depends/utils/AudioHelp.h"

#include "ActionUtils.h"
#include "Global.h"
#include "MoregameBtn.h"
#include "SelectScene.h"
#include "ParabolaTo.h"

Vec2 friutePos[3]={Vec2(227,254),Vec2(559,190),Vec2(586,220)};

Vec2 ballPos[12]={Vec2(84,472),Vec2(157,460),Vec2(194,416),Vec2(253,420),Vec2(309,430),Vec2(350,446),Vec2(373,502),Vec2(324,502),Vec2(242,496),Vec2(163,485),Vec2(66,518),Vec2(148,538)};

HomeLayer::HomeLayer()
{
    
}
HomeLayer::~HomeLayer()
{

}
bool HomeLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initUI();
        isInit=true;
        
    }while(0);
    
    return true;
    
}

void HomeLayer::initUI()
{
    m_pBgSprite=Sprite::create("images/home/start_bg.jpg");
    m_pBgSprite->setAnchorPoint(Vec2(0.5,0.5));
    kAdapterScreen->setExactPosition(m_pBgSprite, 320, 480);
    this->addToBackGroundLayer(m_pBgSprite, 1);
    
 
    m_pLogo = ToolSprite::create("images/home/logo.png");
    
    m_pLogo->setDelegate(this);
    m_pLogo->setIsMove(false);
    kAdapterScreen->setExactPosition(m_pLogo, 320,m_pLogo->getContentSize().height/2+ 50 - 1000,Vec2(0.5,0.5),kBorderNone,kBorderTop);
    this->addToContentLayer(m_pLogo,5);

    ui_button = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("cocos_studio/home.json");
    this->addChild(ui_button,10);

    m_pStartItemSprite  = ui::Helper::seekWidgetByName(ui_button, "btn_play");
    m_pShopItemSprite   = ui::Helper::seekWidgetByName(ui_button, "btn_shop");
    m_pResItemSprite    = ui::Helper::seekWidgetByName(ui_button, "btn_rate_us");
    m_pMoreItemSprite   = ui::Helper::seekWidgetByName(ui_button, "btn_more");
    
    m_pStartItemSprite  ->setTag(eTagStart);
    m_pShopItemSprite   ->setTag(eTagShop);
    m_pResItemSprite    ->setTag(eRestore);
    m_pMoreItemSprite   ->setTag(eTagMoreGame);
    
    m_pStartItemSprite ->setVisible(false);
    m_pShopItemSprite  ->setVisible(false);
    m_pResItemSprite   ->setVisible(false);
    m_pMoreItemSprite  ->setVisible(false);

    m_pStartItemSprite->addTouchEventListener(CC_CALLBACK_2(HomeLayer::touchEvent, this));
    m_pShopItemSprite ->addTouchEventListener(CC_CALLBACK_2(HomeLayer::touchEvent, this));
    m_pResItemSprite  ->addTouchEventListener(CC_CALLBACK_2(HomeLayer::touchEvent, this));
    m_pMoreItemSprite ->addTouchEventListener(CC_CALLBACK_2(HomeLayer::touchEvent, this));
    
    
//    ui_button->setVisible(false);
    AudioHelp::getInstance()->playBackgroundMusic("startbg.mp3");
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_pShopItemSprite->setVisible(false);
#endif
    
    
    m_pPedestal = Sprite::create("images/home/pedestal.png");
    m_pPedestal->setPosition(kAdapterScreen->getExactPostion(Vec2(384.5,328)));
    m_pBgSprite->addChild(m_pPedestal);
    m_pPedestal->setScale(0);
    
    
    
    auto ball = Sprite::create("images/home/sugar.png");
    ball->setPosition(Vec2(224,352));
    m_pPedestal->addChild(ball);
    
    for (int i = 0 ; i < 12; i++) {
        
        auto _ball = Sprite::create(__String::createWithFormat("images/home/sugar%d.png",i)->getCString());
        _ball->setPosition(ballPos[i]);
        m_pPedestal->addChild(_ball);
        _ball->setTag(i);
        m_vBall.pushBack(_ball);
        
    }
    
    auto lid = Sprite::create("images/home/lid.png");
    lid->setAnchorPoint(Vec2(0.31,0.29));
    lid->setPosition(Vec2(214,400));
    m_pPedestal->addChild(lid);
    
    m_pLidCover = Sprite::create("images/home/lid_cover.png");
    m_pLidCover->setAnchorPoint(Vec2(0.6,0.5));
    m_pLidCover->setPosition(Vec2(409,312));
    lid->addChild(m_pLidCover);

    
    
    
    
//    showHotDog();
    this->scheduleOnce(schedule_selector(HomeLayer::MaterialComing1), 1.0);
    
    
    kIAPManager->m_fnAfterRestore = CC_CALLBACK_2(HomeLayer::AfterRestoreCallBack, this);
    
    
//    MoregameBtn* pBtn = MoregameBtn::create();
//    pBtn->addtoParentLayer(this);
//    pBtn->startLoading();
}
void HomeLayer::MaterialComing1(float dt)
{
    ScaleTo* scale = ScaleTo::create(0.5, 1.0);
    RotateBy* rota = RotateBy::create(0.5, 360);
    Spawn* sp = Spawn::create(scale,rota, NULL);
    
    
    auto func = CallFunc::create([&](){
        
       MaterialComing2();
    });
    
    
    m_pPedestal->runAction(Sequence::create(sp,func, NULL));
    
    
}
void HomeLayer::MaterialComing2()
{
    
    auto func = CallFunc::create([&](){
        
//        ShowLogo();
        MaterialComing3();
    });

    RotateBy* rota = RotateBy::create(1.0, 120);
    m_pLidCover->runAction(Sequence::create(rota,func, NULL));
    
}
void HomeLayer::MaterialComing3()
{
    
    for (int i = 5; i< m_vBall.size(); i++) {
        
        auto ball = dynamic_cast<Sprite*>(m_vBall.at(i));
        int tag = ball->getTag();
        
        Vec2 worldPos = m_pPedestal->convertToWorldSpace(ball->getPosition());
        
        if (tag== 6 || tag == 7) {
            
            auto parabola = ParabolaTo::create(2.0, worldPos, Vec2(worldPos.x + 1000,worldPos.y));
            
            ball->runAction(Sequence::create(DelayTime::create((tag - 6) * 0.35),parabola,nullptr));
            
        }else if(tag == 10 || tag == 11){
        
            auto parabola = ParabolaTo::create(1.0, worldPos, Vec2(worldPos.x - 1000,worldPos.y + 500));
            
            ball->runAction(parabola);
        
        }else if(tag == 1){
        
        
           
        
        }else{
        
            JumpBy* jump = JumpBy::create(1.0, Vec2(0,0), 500 - tag*20, 1);
            EaseSineOut* esa = EaseSineOut::create(jump);
            ball->runAction(Sequence::create(DelayTime::create(i*0.1),esa,CallFunc::create([=](){
                
                if (tag == 9) {
                    MoveBy* move = MoveBy::create(1.0, Vec2(0,400));
                    EaseSineOut* esa = EaseSineOut::create(move);
                    dynamic_cast<Sprite*>(m_vBall.at(1))->runAction(Sequence::create(esa,CallFunc::create([=](){
                        
                        ballBomb(m_vBall.at(1));
                        
                        
                        
                    }), NULL));
                }
                
                
                
                
            }),NULL));
            
            }
        
        }
}
    
void HomeLayer::ballBomb(Sprite* ball)
{
    ball->setTexture("images/home/sugar_1.png");
    auto pOvenAnimation = Animation::create();
    pOvenAnimation->addSpriteFrameWithFile("images/home/sugar_1.png");
    pOvenAnimation->addSpriteFrameWithFile("images/home/sugar_2.png");
    pOvenAnimation->addSpriteFrameWithFile("images/home/sugar_3.png");
    pOvenAnimation->addSpriteFrameWithFile("images/home/sugar_4.png");
    pOvenAnimation->setDelayPerUnit(0.4);
    pOvenAnimation->setLoops(1);
    
    
    
    auto func = CallFunc::create([&](){
        
        ShowLogo();
    });
    
    auto pOvenAnimat = Animate::create(pOvenAnimation);
    
    ball->runAction(Sequence::create(pOvenAnimat,RemoveSelf::create(),func,NULL));

}

void HomeLayer::ShowLogo(){
    
    ScaleTo* scale = ScaleTo::create(1.0, 1.0);
    EaseSineOut* esa = EaseSineOut::create(scale);
    ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
    ScaleTo* s2 = ScaleTo::create(0.2f,0.90,1.08f);
    ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
    m_pStartItemSprite->runAction(Sequence::create(ScaleTo::create(0, 0),CallFunc::create([=](){
        
        m_pStartItemSprite->setVisible(true);
        
    }),esa, s1,s2,s3,NULL));
    
    auto func = CallFunc::create([&](){
        
        ShowUIPlay();
    });
    
    m_pShopItemSprite->runAction(Sequence::create(MoveBy::create(0, Vec2(0,-200)),CallFunc::create([=](){
        
        m_pShopItemSprite->setVisible(true);
        
    }),MoveBy::create(1.0, Vec2(0,200)), NULL));
    m_pResItemSprite ->runAction(Sequence::create(MoveBy::create(0, Vec2(-200,-200)),CallFunc::create([=](){
        
        m_pResItemSprite->setVisible(true);
        
    }),MoveBy::create(1.0, Vec2(200,200)), NULL));
    m_pMoreItemSprite->runAction(Sequence::create(MoveBy::create(0, Vec2(200,-200)),CallFunc::create([=](){
        
        m_pMoreItemSprite->setVisible(true);
        
    }),MoveBy::create(1.0, Vec2(-200,200)), func,NULL));
    
    AudioHelp::getInstance()->playEffect("swish.mp3");
    ActionUtils::dropOut(m_pLogo, Vec2(0,-1000),1.0,func);
    
}
//显示开始按钮
void HomeLayer::ShowUIPlay()
{
    
    ScaleBy* scale = ScaleBy::create(0.8, 1.1);
    m_pStartItemSprite->runAction(RepeatForever::create(Sequence::create(scale,scale->reverse(), NULL)));
    
    
    auto chooseParticle = ParticleSystemQuad::create("particle/bakeToolParticle.plist");
    chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    chooseParticle->setVisible(true);
    chooseParticle->setPosition(m_pLogo->getContentSize()*.5);
    chooseParticle->setScale(1.5);
    m_pLogo->addChild(chooseParticle,4);
    
    MoveBy* move = MoveBy::create(1.0, Vec2(0,20));
    m_pLogo->runAction(RepeatForever::create(Sequence::create(move,move->reverse(), NULL)));
    
}
void HomeLayer::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    toolSprite->cancelEvent(true);

    ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
    ScaleTo* s2 = ScaleTo::create(0.2f,0.90,1.08f);
    ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
    toolSprite->runAction(Sequence::create(s1,s2,s3,CallFunc::create([=](){
    
        toolSprite->cancelEvent(false);
    
    }),nullptr));

}
void HomeLayer::showBear()
{
    
}


void HomeLayer::onEnter()
{
    BaseLayer::onEnter();
    
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
    

    AudioHelp::getInstance()->StopBackgroundMusic();
    AudioHelp::getInstance()->playBackgroundMusic("startbg.mp3");

  
 
}

void HomeLayer::onEnterTransitionDidFinish()
{
    
    BaseLayer::onEnterTransitionDidFinish();
    
}

void HomeLayer::onExit()
{
    BaseLayer::onExit();

}

void HomeLayer::showAdsLaod(EventCustom*)
{
    Director::getInstance()->stopAnimation();
}
//关闭全屏广告的回调
void HomeLayer::closeAdsLaod(EventCustom*)
{
    Director::getInstance()->startAnimation();
}
void HomeLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case eTagStart:
                {
                    
                    
                    ui_button->setEnabled(false);
                    
                    if(kIAPManager->isShowAds())
                    {
                        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
                        AdLoadingLayerBase::loadingDoneCallback = []()
                        {
                            SceneChangeManager->enterSelectScene();
                        };
                    }
                    else
                    {
                        SceneChangeManager->enterSelectScene();
                    }
                    CCLOG("startGame");
                    
                    AudioHelp::getInstance()->playStartEffect();
                    
                }
                    break;
                case eTagFavorite:
                {
                    CCLOG("Favorite");
                    //            ShowIcon::hide();
                    ui_button->setEnabled(false);
                    SceneChangeManager->enterFavScene();
                    AudioHelp::getInstance()->playSelectEffect();
                    auto func = CallFunc::create([&](){
                        
                        ui_button->setEnabled(false);
                        
                    });
                    ActionUtils::delayTimeAction(this,0.5,func);
                }
                    break;
                case eTagMoreGame:
                {
                    CCLOG("MoreGame");
                    AudioHelp::getInstance()->playSelectEffect();
                    CFSystemFunction st;
                    st.showMoreGame();
                    
                }
                    break;
                case eTagSetting:
                {
                    CCLOG("Setting");
                    ui_button->setEnabled(false);
                    AudioHelp::getInstance()->playSelectEffect();
                    SettingLayer* layer = SettingLayer::create();
                    layer->showInParent(this);
                    
                }
                    break;
                case eTagShop:
                {
                    CCLOG("shop");
                    //            ShowIcon::hide();
                    ui_button->setEnabled(false);
                    AudioHelp::getInstance()->playSelectEffect();
                    SceneChangeManager->enterShopScene();
                    
                    auto func = CallFunc::create([&](){
                        
                        ui_button->setEnabled(false);
                        
                    });
                    ActionUtils::delayTimeAction(this,0.5,func);
                }
                    break;
                case eTagPrivacy:
                {
                    
                    CCLOG("eTagPrivacy");
                    CFSystemFunction sys;
                    sys.rateUs();
                    
                }
                    break;
                case eRestore:
                {
                    ui_button->setEnabled(false);
                    kIAPManager->restore();
                    auto func = CallFunc::create([&](){
                        
                        ui_button->setEnabled(false);
                        
                    });
                    ActionUtils::delayTimeAction(this,1.0,func);
                    
                }
                    break;
                default:
                    break;
            }
        }
    }


}
void HomeLayer::onBtnCallBack(Ref* sender)
{
    
}
void HomeLayer::AfterRestoreCallBack(bool isRestore,bool isPurchase)
{
    
    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE);
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    
    if(isRestore)
    {
        if(isPurchase)
        {
            dialog->setContentText("Your content has been restored!");
        }
        else
        {
            dialog->setContentText("Sorry! It looks like you haven't purchased anything yet.");
            
        }
    }
    else
    {
        dialog->setContentText("Sorry, restore transaction failed !");
    }
    this->addToUILayer(dialog, 9999);
    
    
}


// 监听Android返回键事件
void HomeLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!m_bIsShowReturn)
        {
            m_bIsShowReturn = true;
            Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogExit, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText(ExitGame);
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addChild(dialog, 9999);
        }
    }
}

void HomeLayer::onNegativeClick(void* type)
{
    m_bIsShowReturn = false;
}
void HomeLayer::onPositiveClick(void* type)
{
    int lType = (uintptr_t)type;
    if(lType == kDialogReset){
        
        
    
    }
    if (lType == kDialogExit) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CFSystemFunction sys;
        sys.endSession();
        Director::getInstance()->end();
#endif
    }
    


}
