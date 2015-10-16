//
//  PushBallScene.cpp
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/15.
//
//

#include "PushBallScene.h"
#include "BallConfig.h"
#include "ParabolaTo.h"

#define GLASSTAG 1001
#define GLASSCOVERTAG 1002
#define COINTAG  1003
#define MACCOVER 1004

string ballStr[3]={"","",""};
Vec2 slitPushPos[10]={Vec2(138,155),Vec2(138,170),Vec2(141,158),Vec2(138,287),Vec2(138,166),Vec2(117,166),Vec2(128,165),Vec2(138,50),Vec2(128,375),Vec2(130,135)};
PushBallScene::PushBallScene()
{
    m_iMacNum =0;
}
PushBallScene::~PushBallScene()
{
    
}
Scene* PushBallScene::createScene()
{
    Scene* scene = Scene::createWithPhysics();
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);//调试
    scene->getPhysicsWorld()->setGravity(Vec2(0,-1000));
//    scene->getPhysicsWorld()->set÷
    PushBallScene* layer = PushBallScene::create();
    scene->addChild(layer);
    return scene;
}
bool PushBallScene::init()
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
void PushBallScene::onEnter()
{
    GameBaseScene::onEnter();
}
void PushBallScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
}
void PushBallScene::onExit()
{
    GameBaseScene::onExit();
}
void PushBallScene::initUI()
{
    m_pBG = Sprite::create("images/select/bg_choose.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 320, 480);
    this->addToBGLayer(m_pBG);
    
    auto borderUpper = PhysicsBody::createEdgeChain(bgLine,2,PHYSICSBODY_MATERIAL_DEFAULT,10);
    m_pBG->setPhysicsBody(borderUpper);
    
    ui_button = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("cocos_studio/choose_flavors.json");
    this->addToUILayer(ui_button,10);
    
    
    
    ui_next = ui::Helper::seekWidgetByName(ui_button, "btn_next");
    ui_next->setTag(100);
    ui_next->setVisible(false);
    ui_next ->addTouchEventListener(CC_CALLBACK_2(PushBallScene::touchEvent, this));

    ui_coin = ui::Helper::seekWidgetByName(ui_button, "btn_coin");
    ui_coin->setTag(200);
    ui_coin->setOpacity(0);
//    ui_coin->setVisible(false);
    ui_coin ->addTouchEventListener(CC_CALLBACK_2(PushBallScene::touchEvent, this));
    
    
    //根据选择界面，获取需要的水果,赋值到数组
//    m_iFriutNum = (int)g_vChooseTaste.size();
    
    for (int i = 0; i<g_vChooseTaste.size(); i++) {
        
        ballStr[i] = g_vChooseTaste.at(i);
    
    }
    initMac();
    initBall();
    
    pushBall();
}
#pragma mark - 倒球,物理世界
void PushBallScene::initBall()
{
    m_pGlass = ToolSprite::create("images/select/glass_bottle0.png");
    kAdapterScreen->setExactPosition(m_pGlass, 120, 0,Vec2::ZERO,kBorderLeft,kBorderNone);
    m_pGlass->setDelegate(this);
    m_pGlass->setAnchorPoint(Vec2::ZERO);
    m_pGlass->setIsMove(false);
    m_pGlass->setTag(GLASSTAG);
    m_pGlass->cancelEvent(true);
    
    Vec2 pos = m_pBG->convertToWorldSpace(Vec2(0,340+120));
    m_pGlass->setPositionY(pos.y);
    this->addToContentLayer(m_pGlass,5);
    
    auto borderUpper = PhysicsBody::createEdgeChain(glass,29,PHYSICSBODY_MATERIAL_DEFAULT,10);
    m_pGlass->setPhysicsBody(borderUpper);
    Sprite* glassCover = Sprite::create("images/select/glass_bottle1.png");
    glassCover->setAnchorPoint(Vec2::ZERO);
    glassCover->setTag(GLASSCOVERTAG);
    m_pGlass->addChild(glassCover,100);
    
    Vec2 _pos = m_pMac->convertToWorldSpace(Vec2(-48,323 + 250));
    m_pGlass->setPosition(_pos);
    
    for(int i = 0;i<8;i++){
        
        __String* sball = nullptr;
        sball = __String::createWithFormat("images/select/one_%s.png",ballStr[0].c_str());
        if(i>3 && i<6){
        
            if (ballStr[1]!="") {
                sball = __String::createWithFormat("images/select/one_%s.png",ballStr[1].c_str());
            
            }
        }else if(i>6){
            
            if (ballStr[2]!="") {
                sball = __String::createWithFormat("images/select/one_%s.png",ballStr[2].c_str());
                
            }
            
                
            
        }
        
        auto ball = Sprite::create(sball->getCString());
//        ball->setPosition(m_pGlass->convertToWorldSpace(Vec2(130,260)));
        ball->setPosition(m_pGlass->convertToWorldSpace(Vec2(130,170)));
        this->addToContentLayer(ball);
//        ball->setPosition(Vec2(130,260));
//        m_pGlass->addChild(ball);
        
        
        auto body = PhysicsBody::createCircle(ball->getContentSize().width / 2 - 5);
        ball->setPhysicsBody(body);
        body->getShape(0)->setMass(5010);
        body->getShape(0)->setRestitution(0);
    }

//    RotateBy* move =RotateBy::create(5.0, 360);
//    m_pGlass->runAction(Sequence::create(move, NULL));
//
}
void PushBallScene::initMac()
{
    int macNum = 6;
    m_iMacNum = 6;
    __String* str = __String::createWithFormat("images/select_mac/machine%d.png",macNum+1);
    m_pMac = Sprite::create(str->getCString());
    kAdapterScreen->setExactPosition(m_pMac, 0 + 300, 0,Vec2(1,0),kBorderRight,kBorderNone);
    Vec2 pos = m_pBG->convertToWorldSpace(Vec2(0,340+200));
    m_pMac->setPositionY(pos.y);
    this->addToContentLayer(m_pMac);

    auto macCover = Sprite::create(__String::createWithFormat("images/select_mac/machine%d_open.png",macNum+1)->getCString());
    macCover->setAnchorPoint(Vec2(1,0));
    macCover->setPosition(m_pMac->getPosition()+Vec2(144,-504 + 254));
    macCover->setTag(MACCOVER);
    this->addToContentLayer(macCover,10);
    
    Sprite* slit = Sprite::create(__String::createWithFormat("images/select_mac/machine%d_slit.png",macNum+1)->getCString());
    slit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    slit->setPosition(slitPushPos[macNum]);
    macCover->addChild(slit);
    
    
    
    if(macNum == 0){
    
        auto borderUpper = PhysicsBody::createEdgeChain(mac_0,54,PHYSICSBODY_MATERIAL_DEFAULT,3);
        m_pMac->setPhysicsBody(borderUpper);
        
    }else if(macNum == 1){
    
        auto borderUpper = PhysicsBody::createEdgeChain(mac_1,23,PHYSICSBODY_MATERIAL_DEFAULT,10);
        m_pMac->setPhysicsBody(borderUpper);
        
    }else if(macNum == 2){
        
        auto borderUpper = PhysicsBody::createEdgeChain(mac_2,25,PHYSICSBODY_MATERIAL_DEFAULT,10);
        m_pMac->setPhysicsBody(borderUpper);
        
    }else if(macNum == 3){
        
        auto borderUpper = PhysicsBody::createEdgeChain(mac_3,23,PHYSICSBODY_MATERIAL_DEFAULT,10);
        m_pMac->setPhysicsBody(borderUpper);
        
    }else if(macNum == 4){
        
        auto borderUpper = PhysicsBody::createEdgeChain(mac_4,22,PHYSICSBODY_MATERIAL_DEFAULT,10);
        m_pMac->setPhysicsBody(borderUpper);
        
    }else if(macNum == 5){
        
        auto borderUpper = PhysicsBody::createEdgeChain(mac_5,10,PHYSICSBODY_MATERIAL_DEFAULT,10);
        m_pMac->setPhysicsBody(borderUpper);
        
    }else if(macNum == 6){
        
        auto borderUpper = PhysicsBody::createEdgeChain(mac_6,10,PHYSICSBODY_MATERIAL_DEFAULT,10);
        m_pMac->setPhysicsBody(borderUpper);
        
    }else if(macNum == 7){
        
        auto borderUpper = PhysicsBody::createEdgeChain(mac_7,10,PHYSICSBODY_MATERIAL_DEFAULT,10);
        m_pMac->setPhysicsBody(borderUpper);
        
    }else if(macNum == 8){
        
        auto borderUpper = PhysicsBody::createEdgeChain(mac_8,10,PHYSICSBODY_MATERIAL_DEFAULT,10);
        m_pMac->setPhysicsBody(borderUpper);
        
    }else if(macNum == 9){
        
        auto borderUpper = PhysicsBody::createEdgeChain(mac_9,19,PHYSICSBODY_MATERIAL_DEFAULT,10);
        m_pMac->setPhysicsBody(borderUpper);
        
    }
}
void PushBallScene::pushBall()
{
    
    
    
    Vec2 pos = m_pMac->convertToWorldSpace(Vec2(-48,323 + 250));
    MoveTo* move = MoveTo::create(2.0, pos);
//    RotateBy* rota = RotateBy::create(5.0, 130);
    
    auto func = CallFunc::create([=](){
        
        m_pGlass->cancelEvent(false);
    
    });
    
    m_pGlass->runAction(Sequence::create(func, NULL));

}
void PushBallScene::showCoin()
{
    auto layerColor = LayerColor::create(Color4B(0,0,0,100));
    this->addToUILayer(layerColor);
    layerColor->setOpacity(0);
    
    auto func = CallFunc::create([=](){
        
        ui_coin->runAction(Sequence::create(FadeIn::create(0.5),CallFunc::create([=](){
        
            this->coinAction();
        
        }), NULL));
        
    });
    layerColor->runAction(Sequence::create(FadeTo::create(1.0, 100),func, NULL));
    
}
//一个硬币飞出来
void PushBallScene::coinAction()
{
    
}
void PushBallScene::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    
    if (tag == GLASSTAG) {
        toolSprite->cancelEvent(true);
        
        auto func = CallFunc::create([=](){
            
            //展示硬币
            this->showCoin();
//            m_pGlass->setVisible(false);
            m_pGlass->getChildByTag(GLASSCOVERTAG)->setOpacity(0);
        });
        
        
         RotateBy* rota = RotateBy::create(3.0, 130);
        m_pGlass->runAction(Sequence::create(rota,DelayTime::create(2.0), FadeOut::create(0.5),func,NULL));
        
        
    }
    
}
void PushBallScene::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case 200:
                {
                    auto coin = ToolSprite::create("cocos_studio/studio_ui/choose_flavor/coin.png");
                    coin->setPosition(ui_coin->getPosition());
                    this->addToUILayer(coin);
                    coin->setDelegate(this);
                    coin->setTag(COINTAG);
//                    coin->setIsMove(false);
                    
                   Vec2 pos =  this->m_pContentLayer->getChildByTag(MACCOVER)->convertToWorldSpace(slitPushPos[m_iMacNum]);
                    
                    auto parabola = ParabolaTo::create(5.0, coin->getPosition(), pos);
                    
                    coin->runAction(Sequence::create(parabola,nullptr));
                    
                }
                    break;
            }
        }
    }
}
void PushBallScene::nextStep()
{
    log("next");
    hideNextButton();
    
}