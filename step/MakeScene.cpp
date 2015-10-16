//
//  MakeScene.cpp
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/9.
//
//

#include "MakeScene.h"

string make_fruit[3]={"","",""};

MakeScene::MakeScene():m_iFriutNum(0),m_rStoveRect(Rect::ZERO),oldPos(Vec2::ZERO),m_iToolZorder(0),m_iStepNum(0),m_bSyrupMove(true),m_iLadleNum(0)
{
    m_bCloseDevice = false;
    m_bIsSounds = true;
    isTouch = true;
    m_pBowlMix = nullptr;
    m_pHand = nullptr;
    m_pSwitchOff = true;
}
MakeScene::~MakeScene()
{
    
}
bool MakeScene::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!GameBaseScene::init());
        initUI();
        bRet = true;
    } while (0);
    return bRet;
}
void MakeScene::onEnter()
{
    GameBaseScene::onEnter();
}
void MakeScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
    toolAdd();
    this->schedule(schedule_selector(MakeScene::checkStep), 2.5);
}
void MakeScene::onExit()
{
    GameBaseScene::onExit();
}
void MakeScene::initUI()
{
    m_pBG = Sprite::create("images/make/bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 320, 480);
    this->addToBGLayer(m_pBG);
 
    ui_button = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("cocos_studio/choose_flavors.json");
    this->addToUILayer(ui_button);
    
    ui_next = ui::Helper::seekWidgetByName(ui_button, "btn_next");
    ui_next->setTag(100);
    ui_next->setVisible(false);
    ui_next ->addTouchEventListener(CC_CALLBACK_2(MakeScene::touchEvent, this));

    auto stove = Sprite::create("images/make/stove.png");
    stove->setPosition(kAdapterScreen->getExactPostion(Vec2(316,329)));
    stove->setTag(222);
    this->addToContentLayer(stove,10);

    Vec2 pos = stove->getPosition();
    m_rStoveRect = Rect(pos.x - 155,pos.y+120,280,100);
    
    m_pFire = Sprite::create("images/make/fire0.png");
    m_pFire->setPosition(Vec2(212,203));
    m_pFire->setAnchorPoint(Vec2(0.5,0.3));
    stove->addChild(m_pFire);
    m_pFire->setVisible(false);
    
    m_pPotUnder = Sprite::create("images/make/pot_under.png");
    m_pPotUnder->setAnchorPoint(Vec2(0.5,0));
    m_pPotUnder->setPosition(Vec2(212,203 - 20));
    stove->addChild(m_pPotUnder);
    
    auto potCover = Sprite::create("images/make/pot_up.png");
    potCover->setAnchorPoint(Vec2::ZERO);
    m_pPotUnder->addChild(potCover,1000);
    
    m_pSwitch = ToolSprite::create("images/make/on.png");
    m_pSwitch->setPosition(Vec2(215,84));
    stove->addChild(m_pSwitch);
    m_pSwitch->setTag(kMakeSwitch);
    m_pSwitch->setDelegate(this);
    m_pSwitch->setIsMove(false);
    m_pSwitch->cancelEvent(true);
    
    //根据选择界面，获取需要的水果,赋值到数组
    m_iFriutNum = (int)g_vChooseTaste.size();
    
    for (int i = 0; i<m_iFriutNum; i++) {
        
        make_fruit[i] = g_vChooseTaste.at(i);
        
        
    }
    if(m_iFriutNum>1){
    
        m_sMixStr = "mix";
    
    }else{
        
        m_sMixStr = make_fruit[0];
    }
    //各种材料1
    auto gum = ToolSprite::create("images/make/gum2.png");
    kAdapterScreen->setExactPosition(gum, 320,150,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addToContentLayer(gum);
    gum->setDelegate(this);
    gum->setTag(kMakeGum);
    gum->addRect(m_rStoveRect);
    gum->cancelEvent(true);
    gum->setPositionX(gum->getPositionX() - 1000);
   
    auto sugar = ToolSprite::create("images/make/sugar.png");
    kAdapterScreen->setExactPosition(sugar,0,150,Vec2(1,1),kBorderRight,kBorderTop);
    this->addToContentLayer(sugar);
    sugar->setDelegate(this);
    sugar->setTag(kMakeSugar);
    sugar->cancelEvent(true);
    sugar->addRect(m_rStoveRect);
    sugar->setPositionX(sugar->getPositionX() - 1000);
   
    //各种材料2
    auto chicle = ToolSprite::create("images/make/chicle_base.png");
    kAdapterScreen->setExactPosition(chicle, 320,150,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addToContentLayer(chicle);
    chicle->setDelegate(this);
    chicle->setTag(kMakeChicle);
    chicle->cancelEvent(true);
    chicle->addRect(m_rStoveRect);
    chicle->setPositionX(chicle->getPositionX() + 1000);
    
    auto water = ToolSprite::create("images/make/water.png");
    kAdapterScreen->setExactPosition(water,0,150,Vec2(0,1),kBorderLeft,kBorderTop);
    this->addToContentLayer(water);
    water->setDelegate(this);
    water->setTag(kMakeWater);
    water->cancelEvent(true);
    water->addRect(m_rStoveRect);
    water->setPositionX(water->getPositionX() + 1000);
    
    auto syrup = ToolSprite::create("images/make/rice_syrup.png");
    kAdapterScreen->setExactPosition(syrup,0,150,Vec2(1,1),kBorderRight,kBorderTop);
    this->addToContentLayer(syrup);
    syrup->setDelegate(this);
    syrup->setTag(kMakeSyrup);
    syrup->cancelEvent(true);
    syrup->addRect(m_rStoveRect);
    syrup->setPositionX(syrup->getPositionX() + 1000);
    
    //开启重力感应
    //    Device::setAccelerometerEnabled(true);
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(MakeScene::didAccelerate, this));
    //在事件监听器中注册
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
}
void MakeScene::didAccelerate(Acceleration* acc, Event* event)
{
    
    log("%f",acc->x);
    if (m_bCloseDevice) {
        m_dAccX = acc->x * 200;
    }
    
    
    
}
void MakeScene::toolAdd()
{
    auto func = CallFunc::create([=](){
    
        dynamic_cast<ToolSprite*>(this->m_pContentLayer->getChildByTag(kMakeGum))->cancelEvent(false);
        dynamic_cast<ToolSprite*>(this->m_pContentLayer->getChildByTag(kMakeSugar))->cancelEvent(false);
    
    });
    
    this->m_pContentLayer->getChildByTag(kMakeGum)->runAction(Sequence::create(MoveBy::create(0.5,Vec2(1000,0)),func, NULL));
    this->m_pContentLayer->getChildByTag(kMakeSugar)->runAction(MoveBy::create(0.5,Vec2(1000,0)));
    addFuritToStoven(0);
}
void MakeScene::_toolAdd()
{
    auto func = CallFunc::create([=](){
        
        dynamic_cast<ToolSprite*>(this->m_pContentLayer->getChildByTag(kMakeChicle))->cancelEvent(false);
        dynamic_cast<ToolSprite*>(this->m_pContentLayer->getChildByTag(kMakeWater))->cancelEvent(false);
        dynamic_cast<ToolSprite*>(this->m_pContentLayer->getChildByTag(kMakeSyrup))->cancelEvent(false);
        
    });
    this->m_pContentLayer->getChildByTag(kMakeChicle)->runAction(Sequence::create(MoveBy::create(0.5,Vec2(-1000,0)),func, NULL));
    this->m_pContentLayer->getChildByTag(kMakeWater)->runAction(MoveBy::create(0.5,Vec2(-1000,0)));
    this->m_pContentLayer->getChildByTag(kMakeSyrup)->runAction(MoveBy::create(0.5,Vec2(-1000,0)));

}
void MakeScene::showLadie()
{
    auto ladle = ToolSprite::create("images/make/ladle1.png");
    ladle->setDelegate(this);
    ladle->addRect(m_rStoveRect);
    kAdapterScreen->setExactPosition(ladle, 0 - 1000, 100,Vec2(1,1),kBorderRight,kBorderTop);
    this->addToContentLayer(ladle,50);
    ladle->cancelEvent(true);
    ladle->setTag(kMakeLadle);
    ladle->setResponsePoint(Vec2(0.2,0.1));
    
    auto func = CallFunc::create([=](){
        
        ladle->cancelEvent(false);
    
    });
    
    ladle->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-1000,0)),func,nullptr));

}
void MakeScene::showHand()
{
    if (!m_pHand) {
        
        m_pHand = Sprite::create("images/oven/point.png");
        m_pHand->setAnchorPoint(Vec2(0.11,0.8));
        m_pHand->setPosition(Vec2(214 + 40,85));
        this->m_pContentLayer->getChildByTag(222)->addChild(m_pHand,100);
        
        
        m_pSwitch->cancelEvent(false);
        
        MoveBy* move = MoveBy::create(0.5, Vec2(-40,0));
        ScaleBy* scale = ScaleBy::create(0.25, 0.8, 1.0);
        
        m_pHand->runAction(RepeatForever::create(Sequence::create(move,scale,scale->reverse(),move->reverse(), NULL)));
    }
}
void MakeScene::addFuritToStoven(int num)
{
    auto plate = ToolSprite::create("images/make/plate0.png");
    kAdapterScreen->setExactPosition(plate, 0,150 + 90,Vec2(0,1),kBorderLeft,kBorderTop);
    this->addToContentLayer(plate);
    plate->addRect(m_rStoveRect);
    plate->setDelegate(this);
    plate->setTag(kMakeFruit);
    plate->setUserData((void*)num);
    plate->setPositionX(plate->getPositionX() - 1000);
    
    plate->runAction(MoveBy::create(0.5,Vec2(1000,0)));
    
    auto fruit = Sprite::create(__String::createWithFormat("images/make/plate_%s.png",make_fruit[num].c_str())->getCString());
    fruit->setAnchorPoint(Vec2::ZERO);
    plate->addChild(fruit);
    
    

}
void MakeScene::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    oldPos = toolSprite->getPosition();
    m_iToolZorder = toolSprite->getLocalZOrder();
    log("---->%d",m_iToolZorder);
    toolSprite->getParent()->reorderChild(toolSprite, 100);
    
    if(tag == kMakeSyrup){
    
        toolSprite->setTexture("images/make/rice_syrup_ladle1.png");
        
    }else if(tag == kMakeSwitch){
    
        if (m_pSwitchOff) {
            
            if(m_pHand){
                m_pHand->stopAllActions();
                m_pHand->removeFromParent();
                m_pHand=nullptr;
            
            }
            
            
            m_pSwitch->setTexture("images/make/off.png");
            m_pSwitch->cancelEvent(true);
            m_pFire->setVisible(true);
            auto pOvenAnimation = Animation::create();
            
            pOvenAnimation->addSpriteFrameWithFile("images/make/fire0.png");
            pOvenAnimation->addSpriteFrameWithFile("images/make/fire1.png");
            pOvenAnimation->addSpriteFrameWithFile("images/make/fire2.png");
            pOvenAnimation->setDelayPerUnit(0.2);
            pOvenAnimation->setLoops(-1);
            
            auto pOvenAnimat = Animate::create(pOvenAnimation);
            m_pFire->runAction(pOvenAnimat);
            
            
            //火的音效
            
            AudioHelp::getInstance()->playLoofEffect("oven_work.mp3");
            showLadie();
        }else {
            AudioHelp::getInstance()->stopLoofEffect();
            
            if(m_pHand){
                m_pHand->stopAllActions();
                m_pHand->removeFromParent();
                m_pHand=nullptr;
                
            }
            
            m_pSwitch->setTexture("images/make/off.png");
            m_pSwitch->cancelEvent(true);
            m_pFire->setVisible(false);
        
            auto chooseParticle = ParticleSystemQuad::create("particle/progressLost.plist");
            chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
            chooseParticle->setVisible(true);
            kAdapterScreen->setExactPosition(chooseParticle, 320, 480);
            chooseParticle->setDuration(1.0);
            this->addChild(chooseParticle,100);
            
            AudioHelp::getInstance()->playEffect("hurrah.mp3");
            
            
            ui_next->setVisible(true);
            //next
        
        
        }
     }
    
}
void MakeScene::onTouchMove(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    
    if (tag == kMakeSyrup) {
        
        if (m_bSyrupMove) {
            m_bSyrupMove = false;
            
            toolSprite->setTexture("images/make/rice_syrup_ladle2.png");
            
            auto syrup = Sprite::create("images/make/rice_syrup.png");
            syrup->setAnchorPoint(Vec2(1,1));
            syrup->setPosition(toolSprite->getPosition());
            syrup->setTag(333);
            this->addToContentLayer(syrup);
            
        }
        
    }

}
void MakeScene::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    toolSprite->cancelEvent(true);
    
    toolSprite->runAction(Sequence::create(MoveTo::create(0.5, oldPos),CallFunc::create([=](){
    
        toolSprite->cancelEvent(false);
        toolSprite->getParent()->reorderChild(toolSprite, m_iToolZorder);
        
        if (tag == kMakeSyrup) {
            m_bSyrupMove = true;
            toolSprite->setTexture("images/make/rice_syrup.png");
            
            if (this->m_pContentLayer->getChildByTag(333)) {
                this->m_pContentLayer->getChildByTag(333)->removeFromParent();
            }
            
        }else if (tag == kMakeLadle) {
            
            toolSprite->setTexture("images/make/ladle1.png");
            
            
        }
        
    }), NULL));
    
    
}
void MakeScene::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    int tag = toolSprite->getTag();
    switch (tag) {
        case kMakeFruit:
            {
                size_t fruitNum = (size_t)toolSprite->getUserData();
                toolSprite->setTag(777);
                toolSprite->cancelEvent(true);
                toolSprite->setVisible(false);

                __String* str =  __String::createWithFormat("images/make/putin/%s_1.png",make_fruit[fruitNum].c_str());
                
                
                MoveBy* move1 = MoveBy::create(0.5, Vec2(0,-170));
                EaseSineIn* esi1 = EaseSineIn::create(move1);
                
                MoveBy* move2 = MoveBy::create(0.5, Vec2(0,-170));
                EaseSineIn* esi2 = EaseSineIn::create(move2);
                
                MoveBy* move3 = MoveBy::create(0.5, Vec2(0,-170));
                EaseSineIn* esi3 = EaseSineIn::create(move3);
                
                auto fruit = Sprite::create(str->getCString());
                fruit->setAnchorPoint(Vec2(0.5,0.5));
                fruit->setPosition(Vec2(533/2,300));
                m_pPotUnder->addChild(fruit,2);
                fruit->runAction(Sequence::create(esi1, RemoveSelf::create(),NULL));
                
                
                auto fruit1 = Sprite::create(str->getCString());
                fruit1->setAnchorPoint(Vec2(0.5,0.5));
                fruit1->setPosition(Vec2(533/2,300));
                fruit1->setVisible(false);
                m_pPotUnder->addChild(fruit1,2);
                
                fruit1->runAction(Sequence::create(DelayTime::create(0.35),CallFunc::create([=](){
                    
                    fruit1->setVisible(true);
                    
                }),esi2, RemoveSelf::create(),NULL));
                
                auto fruit2 = Sprite::create(str->getCString());
                fruit2->setAnchorPoint(Vec2(0.5,0.5));
                fruit2->setPosition(Vec2(533/2,300));
                fruit2->setVisible(false);
                m_pPotUnder->addChild(fruit2,2);
                
                
                auto func = CallFunc::create([=](){
                    
                    
                    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
                   
                    auto _fruit = Sprite::create(__String::createWithFormat("images/make/putin/%s_3.png",make_fruit[fruitNum].c_str())->getCString());
                    _fruit->setPosition(m_pPotUnder->getContentSize()/2);
                    m_pPotUnder->addChild(_fruit);
                    
                    if (m_iFriutNum-1>fruitNum) {
                        
                        addFuritToStoven((int)fruitNum+1);
                        
                    }else{
                    
                        //水果完成
                        log("----水果完成");
                        m_iStepNum++;
                    }
                    
                });
                
                
                
                fruit2->runAction(Sequence::create(DelayTime::create(0.55),CallFunc::create([=](){
                    
                    fruit2->setVisible(true);
                    
                }),esi3, RemoveSelf::create(),func,NULL));
                
                
                AudioHelp::getInstance()->playEffect("pour ice.mp3");
                
                
                
            }
            break;
        case kMakeGum:
        {
            m_dAccX=0.0;
            toolSprite->cancelEvent(true);
            toolSprite->setDelegate(nullptr);

            toolSprite->setVisible(false);
            
            auto gum = Sprite::create("images/make/gum1.png");
            gum->setAnchorPoint(Vec2(0.5,0.3));
            gum->setPosition(Vec2(533/2,300));
            gum->setTag(kMakeGum);
            m_pPotUnder->addChild(gum,2);
            
            auto sp = Sprite::create("images/make/incline.png");
            kAdapterScreen->setExactPosition(sp, 320, 480);
            this->addToUILayer(sp);
            
            auto func = CallFunc::create([=](){
                
                
                this->schedule(schedule_selector(MakeScene::sugarAction), 0.1);
                Device::setAccelerometerEnabled(true);
                m_bCloseDevice = true;
                
            });
            
            
            
            RotateBy* rota = RotateBy::create(0.5, -50);
            RotateBy* rota1 = RotateBy::create(0.5, -50);
            sp->runAction(Sequence::create(rota,rota->reverse(),rota1,rota1->reverse(),RemoveSelf::create(),func,NULL));
            
            
        }
            break;
        case kMakeSugar:
        {
            toolSprite->cancelEvent(true);
            toolSprite->setDelegate(nullptr);
            toolSprite->setVisible(false);
            
            auto sugar = Sprite::create("images/make/sugar.png");
            sugar->setAnchorPoint(Vec2(0.5,0.5));
            sugar->setPosition(Vec2(533/2 + 100,270));
            sugar->setTag(kMakeSugar+200);
            m_pPotUnder->addChild(sugar,2);
            
            auto sugarStoven = Sprite::create("images/make/suger_1.png");
            sugarStoven->setPosition(m_pPotUnder->getContentSize()/2);
            sugarStoven->setVisible(false);
            m_pPotUnder->addChild(sugarStoven);
            sugarStoven->setTag(kMakeSugar+100);
            
            RotateBy* rota= RotateBy::create(0.5, -100);
            
            auto addSugar = CallFunc::create([=](){
            
                auto particle = ParticleSystemQuad::create("particle/sugarParticle.plist");
                particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
                particle->setVisible(true);
                particle->setScale(0.7);
                particle->setAnchorPoint(Vec2(1,1));
                particle->setDuration(0.25);
                particle->setPosition(Vec2(99,160));
                particle->setRotation(100);
                sugar->addChild(particle,2);
                
                AudioHelp::getInstance()->playEffect("add powder.mp3");
            });
            
            auto addsugar1 = CallFunc::create([=](){
                
                
                dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(kMakeSugar+100))->setVisible(true);
            });
            auto addsugar2 = CallFunc::create([=](){
                dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(kMakeSugar+100))->setTexture("images/make/suger_2.png");
            });
            auto addsugar3 = CallFunc::create([=](){
                dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(kMakeSugar+100))->setTexture("images/make/suger_3.png");
                
                m_pPotUnder->getChildByTag(kMakeSugar+200)->setVisible(false);
                //oven
                m_iStepNum++;
            });
            
            
            sugar->runAction(Sequence::create(rota,JumpBy::create(0.5, Vec2(), 20, 1),addSugar,DelayTime::create(0.75),addsugar1,JumpBy::create(0.5, Vec2(), 20, 1),addSugar,DelayTime::create(0.75),addsugar2,JumpBy::create(0.5, Vec2(), 20, 1),addSugar,DelayTime::create(0.75), addsugar3,NULL));
            
            
        }
            break;
        case kMakeWater:
        {
            m_dAccX=0.0;
            toolSprite->cancelEvent(true);
            toolSprite->setDelegate(nullptr);
            
            toolSprite->setVisible(false);
            
            auto water = Sprite::create("images/make/water.png");
            water->setAnchorPoint(Vec2(0.5,0.5));
            water->setPosition(Vec2(533/2-100,300));
            water->setTag(kMakeWater);
            m_pPotUnder->addChild(water,2);
            
            auto sp = Sprite::create("images/make/incline1.png");
            kAdapterScreen->setExactPosition(sp, 320, 480);
            this->addToUILayer(sp);
            
            auto func = CallFunc::create([=](){
                
                if(abs(m_dAccX)>80){
                    
                    m_dAccX = 0.0;
                }
                this->schedule(schedule_selector(MakeScene::waterAction), 0.1);
                Device::setAccelerometerEnabled(true);
                m_bCloseDevice = true;
                
            });
            
            
            
            RotateBy* rota = RotateBy::create(0.5, 50);
            RotateBy* rota1 = RotateBy::create(0.5, 50);
            sp->runAction(Sequence::create(rota,rota->reverse(),rota1,rota1->reverse(),RemoveSelf::create(),func,NULL));
        }
            break;
        case kMakeChicle:
        {
            toolSprite->cancelEvent(true);
            toolSprite->setDelegate(nullptr);
            
            toolSprite->setVisible(false);
            
            auto chicle = Sprite::create("images/make/chicle_base.png");
            chicle->setAnchorPoint(Vec2(0.5,0.5));
            chicle->setPosition(Vec2(533/2-100,300));
            chicle->setTag(kMakeChicle);
            m_pPotUnder->addChild(chicle,2);
            
        
            auto addChicle = CallFunc::create([=](){
                
                auto particle = ParticleSystemQuad::create("particle/sugarParticle1.plist");
                particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
                particle->setVisible(true);
                particle->setScale(0.7);
                particle->setAnchorPoint(Vec2(0,1));
                particle->setDuration(1.0);
                particle->setRotation(-60);
                particle->setPosition(Vec2(169,76));
                chicle->addChild(particle,2);
                
                AudioHelp::getInstance()->playEffect("add powder.mp3");
                
            });
            auto func = CallFunc::create([=](){
                
                chicle->setVisible(false);
                
                auto chicleStoven = Sprite::create("images/make/chicle_base_3.png");
                chicleStoven->setPosition(m_pPotUnder->getContentSize()/2);
                m_pPotUnder->addChild(chicleStoven);
                chicleStoven->setTag(kMakeSugar+100);
                
                //over
                m_iStepNum++;
            });
            RotateBy* rota = RotateBy::create(0.5, 60);
            
            
            chicle->runAction(Sequence::create(rota,addChicle,DelayTime::create(1.5),func,NULL));
            
        }
            break;
        case kMakeSyrup:
        {
            toolSprite->cancelEvent(true);
            toolSprite->setDelegate(nullptr);
            toolSprite->setVisible(false);
            
            auto syrup = Sprite::create("images/make/rice_syrup_ladle2.png");
            syrup->setAnchorPoint(Vec2(0.5,0.5));
            syrup->setPosition(Vec2(533/2,300));
            syrup->setTag(kMakeSyrup);
            m_pPotUnder->addChild(syrup,2);
            
            auto syrupStoven = Sprite::create("images/make/rice_syrup_1.png");
            syrupStoven->setPosition(m_pPotUnder->getContentSize()/2);
            syrupStoven->setTag(kMakeSyrup+100);
            syrupStoven->setVisible(false);
            m_pPotUnder->addChild(syrupStoven);
            
            auto addsyrup = CallFunc::create([=](){
                
                auto _gum = Sprite::create("images/make/rice_syrup1.png");
                _gum->setPosition(Vec2(109,34));
                m_pPotUnder->getChildByTag(kMakeSyrup)->addChild(_gum);
                _gum->setAnchorPoint(Vec2(0.5,1));
                _gum->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0,-100)),RemoveSelf::create(), NULL));
                
                AudioHelp::getInstance()->playEffect("drop_oil.mp3");
                
            });
            
            auto _addsyrup1 = CallFunc::create([=](){
                
                
                dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(kMakeSyrup+100))->setVisible(true);
                
            });
            auto _addsyrup2 = CallFunc::create([=](){
                
                
                dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(kMakeSyrup+100))->setTexture("images/make/rice_syrup_2.png");
                
            });
            auto _addsyrup3 = CallFunc::create([=](){
                
                
                dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(kMakeSyrup+100))->setTexture("images/make/rice_syrup_3.png");
                m_pPotUnder->getChildByTag(kMakeSyrup)->setVisible(false);
                //oven
                m_iStepNum++;
                
                if (this->m_pContentLayer->getChildByTag(333)) {
                    this->m_pContentLayer->getChildByTag(333)->removeFromParent();
                }
            });
            
            
            syrup->runAction(Sequence::create(addsyrup,DelayTime::create(0.5),_addsyrup1,addsyrup,DelayTime::create(0.5),_addsyrup2,addsyrup,DelayTime::create(0.5),_addsyrup3, NULL));
        }
            break;
        case kMakeLadle:
        {
            toolSprite->setTexture("images/make/ladle2.png");
            
            m_iLadleNum++;
            
            
            if (m_bIsSounds) {
                AudioHelp::getInstance()->playEffect("stir the batter.mp3");
                m_bIsSounds = false;
            }
            
            if (isTouch) {
                isTouch=false;
                auto funcSounds=CallFunc::create([&](){
                    
                    m_bIsSounds = true;
                    isTouch=true;
                });
                
                this->runAction(Sequence::create(DelayTime::create(3.0),funcSounds, NULL));
                
            }
            
            

            
            if (m_iLadleNum == 40) {
                
                if(!m_pBowlMix){
                    
                    m_pPotUnder->removeAllChildren();
                    
                    auto potCover = Sprite::create("images/make/pot_up.png");
                    potCover->setAnchorPoint(Vec2::ZERO);
                    m_pPotUnder->addChild(potCover,10);
                    
                    m_pBowlMix = Sprite::create(__String::createWithFormat("images/make/stir/%s_stir1.png",m_sMixStr.c_str())->getCString());
                    m_pBowlMix->setPosition(m_pPotUnder->getContentSize()/2);
                    m_pPotUnder->addChild(m_pBowlMix);
                    
                    
                }
                
                
            }else if(m_iLadleNum == 80){
                
                m_pBowlMix->setTexture(__String::createWithFormat("images/make/stir/%s_stir2.png",m_sMixStr.c_str())->getCString());
                
            }else if(m_iLadleNum == 120){
                
                m_pBowlMix->setTexture(__String::createWithFormat("images/make/stir/%s_stir3.png",m_sMixStr.c_str())->getCString());
                
            }else if(m_iLadleNum == 160){
                
                m_pBowlMix->setTexture(__String::createWithFormat("images/make/stir/%s_stir4.png",m_sMixStr.c_str())->getCString());
                
                toolSprite->cancelEvent(true);
                
                auto func = CallFunc::create([=](){
                   
                    
                    
                    
                    m_pSwitchOff = false;
                    m_pSwitch->cancelEvent(false);
                    showHand();
                });
                
                toolSprite->runAction(Sequence::create(MoveBy::create(0.5,Vec2(1000,0)),func,nullptr));
            }

            
            
        }
            break;
        default:
            break;
    }
}
void MakeScene::onTouchMoveOutRect(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    if (tag == kMakeLadle) {
        
        toolSprite->setTexture("images/make/ladle1.png");
        
        
    }

}
void MakeScene::waterAction(float dt)
{
    if (m_dAccX < - 60 ) {
        
    }else if(m_dAccX > 100){
        
        m_pPotUnder->getChildByTag(kMakeWater)->setRotation(100);
        dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(kMakeWater))->setTexture("images/make/water_5.png");
        
        auto water = Sprite::create("images/make/water_2.png");
        water->setPosition(m_pPotUnder->getContentSize()/2);
        m_pPotUnder->addChild(water);
        water->setVisible(false);
        water->setTag(kMakeWater+100);
        
        this->unschedule(schedule_selector(MakeScene::waterAction));
        Device::setAccelerometerEnabled(false);
        
        m_bCloseDevice = false;
        
       
     
        
        
        auto addWater = CallFunc::create([=](){
            
            auto water = Sprite::create("images/make/water_1.png");
            water->setPosition(Vec2(154,184));
            m_pPotUnder->getChildByTag(kMakeWater)->addChild(water);
            water->setAnchorPoint(Vec2(0.5,1));
            water->setRotation(-100);
            water->runAction(Sequence::create(ScaleTo::create(0.25, 1,1.5),ScaleTo::create(0.25, 1.0,1.0),FadeOut::create(0.5), NULL));
            
             AudioHelp::getInstance()->playEffect("add water.mp3");
            
        });
        
        auto addWater1 = CallFunc::create([=](){
            
            
            dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(kMakeWater+100))->setVisible(true);
        });
        auto addWater2 = CallFunc::create([=](){
            dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(kMakeWater+100))->setTexture("images/make/water_3.png");
        });
        auto addWater3 = CallFunc::create([=](){
            dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(kMakeWater+100))->setTexture("images/make/water_4.png");
            
           m_pPotUnder->getChildByTag(kMakeWater)->setVisible(false);
            m_iStepNum++;
        });
        
        
        m_pPotUnder->getChildByTag(kMakeWater)->runAction(Sequence::create(addWater,DelayTime::create(0.75),addWater1,addWater,DelayTime::create(0.75),addWater2,addWater,DelayTime::create(0.75),addWater3, NULL));
        
    }
    else{
        
        m_pPotUnder->getChildByTag(kMakeWater)->setRotation(m_dAccX);
        
    }
        
   
}
void MakeScene::sugarAction(float dt)
{
    if (m_dAccX >40 ) {
        
    }else if(m_dAccX <=-100){
        m_pPotUnder->getChildByTag(kMakeGum)->setRotation(-100);
       
        auto _gum = Sprite::create("images/make/gum_1.png");
        _gum->setPosition(m_pPotUnder->getContentSize()/2);
        m_pPotUnder->addChild(_gum);
        _gum->setVisible(false);
        _gum->setTag(101);
        
        this->unschedule(schedule_selector(MakeScene::sugarAction));
        Device::setAccelerometerEnabled(false);
        
        m_bCloseDevice = false;
        
        auto func = CallFunc::create([=](){
            
            m_pPotUnder->getChildByTag(kMakeGum)->setVisible(false);
         

            m_iStepNum++;
            m_dAccX = 0.0;
        });
        
        auto addGum = CallFunc::create([=](){
        
            auto _gum = Sprite::create("images/make/gum3.png");
            _gum->setPosition(Vec2(84,140));
            m_pPotUnder->getChildByTag(kMakeGum)->addChild(_gum);
            _gum->setAnchorPoint(Vec2(0.5,1));
            _gum->setRotation(90);
            _gum->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-150,10)),RemoveSelf::create(), NULL));
            
            AudioHelp::getInstance()->playEffect("drop_oil.mp3");
            
        });
       
        auto _addGum1 = CallFunc::create([=](){
            
            
            dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(101))->setVisible(true);
            
        });
        auto _addGum2 = CallFunc::create([=](){
            
            
            dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(101))->setTexture("images/make/gum_2.png");
            
        });
        auto _addGum3 = CallFunc::create([=](){
            
            
            dynamic_cast<Sprite*>(m_pPotUnder->getChildByTag(101))->setTexture("images/make/gum_3.png");
            
        });
        
        
        m_pPotUnder->getChildByTag(kMakeGum)->runAction(Sequence::create(addGum,DelayTime::create(0.5),_addGum1,addGum,DelayTime::create(0.5),_addGum2,addGum,DelayTime::create(0.5),addGum,DelayTime::create(0.5),_addGum3,func,NULL));
        
   
        
        
        
    }
    else{
        
        m_pPotUnder->getChildByTag(kMakeGum)->setRotation(m_dAccX);
        
    }

}
void MakeScene::checkStep(float dt)
{
    log("m_istep----->%d",m_iStepNum);
    
    if (m_iStepNum == 3) {
        m_iStepNum++;
        _toolAdd();
    }
    if (m_iStepNum == 7) {
        m_iStepNum++;
        this->unschedule(schedule_selector(MakeScene::checkStep));
        showHand();
    }

}
void MakeScene::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                
            }
        }
    }
}
void MakeScene::nextStep()
{
    log("next");
    hideNextButton();
    
}