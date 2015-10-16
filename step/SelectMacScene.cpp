//
//  SelectMacScene.cpp
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/15.
//
//

#include "SelectMacScene.h"
#include "IAPManager.h"


Vec2 slitPos[10]={Vec2(138,155),Vec2(138,170),Vec2(141,158),Vec2(138,287),Vec2(138,166),Vec2(117,166),Vec2(128,165),Vec2(138,50),Vec2(128,375),Vec2(130,135)};

SelectMacScene::SelectMacScene()
{
    m_pCurrentSprite = nullptr;
    
    
    m_iTouchIndex = -1;
}
SelectMacScene::~SelectMacScene()
{
    
}
Scene* SelectMacScene::createScene()
{
    Scene* scene = Scene::createWithPhysics();
        scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);//调试
    scene->getPhysicsWorld()->setGravity(Vec2(0,-1000));
    
    SelectMacScene* layer = SelectMacScene::create();
    scene->addChild(layer);
    return scene;
}
bool SelectMacScene::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!GameBaseScene::init());
        initData();
        initUI();
        bRet = true;
    } while (0);
    return bRet;
    
}
void SelectMacScene::initData()
{
    m_vTypes.clear();
    for (int i = 0; i<10; i++) {
        
        __String* str = __String::createWithFormat("images/select_mac/machine%d.png",i+1);
        

        Sprite* mac = Sprite::create(str->getCString());
        
        Sprite* sugar = Sprite::create(__String::createWithFormat("images/select_mac/machine%d_suger.png",i+1)->getCString());
        sugar->setAnchorPoint(Vec2::ZERO);
        mac->addChild(sugar);
        
        Sprite* door = Sprite::create(__String::createWithFormat("images/select_mac/machine%d_close.png",i+1)->getCString());
        door->setAnchorPoint(Vec2::ZERO);
        mac->addChild(door);
        
        Sprite* slit = Sprite::create(__String::createWithFormat("images/select_mac/machine%d_slit.png",i+1)->getCString());
        slit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        slit->setPosition(slitPos[i]);
        mac->addChild(slit);
        
        
        m_vTypes.pushBack(mac);
        
        
    }


}
void SelectMacScene::onEnter()
{
    GameBaseScene::onEnter();
}
void SelectMacScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
}
void SelectMacScene::onExit()
{
    GameBaseScene::onExit();
}
void SelectMacScene::initUI()
{
    
    
    m_pBG = Sprite::create("images/select/bg_choose.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 320, 480);
    this->addToBGLayer(m_pBG);
    
    ui_button = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("cocos_studio/choose_flavors.json");
    this->addToUILayer(ui_button);
    
    ui_next = ui::Helper::seekWidgetByName(ui_button, "btn_next");
    ui_next->setTag(100);
    ui_next->setVisible(false);
    ui_next ->addTouchEventListener(CC_CALLBACK_2(SelectMacScene::touchEvent, this));

    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getVisibleSize().width, 520));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
    m_pGirdView->m_fHSpace = 10;
    //    m_pGirdView->m_fVSpace = 20;
    m_pGirdView->m_fMarginTop = 0;
    //    m_pGirdView->m_fMarginBottom = 100;
    m_pGirdView->m_fMarginRight = 100;
    m_pGirdView->m_fMarginLeft = 100;
    kAdapterScreen->setExactPosition(m_pGirdView, 0, 0,Vec2::ZERO,kBorderLeft,kBorderNone);
    
    Vec2 pos = m_pBG->convertToWorldSpace(Vec2(0,378));
    m_pGirdView->setPositionY(pos.y);
//    m_pGirdView->setAnchorPoint(Vec2(0,0));
//    m_pGirdView->setPosition(Vec2(0 + kAdapterScreen->g_oOffset.x,333));
//    m_pBG->addChild(m_pGirdView);
    this->addToContentLayer(m_pGirdView);
    
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setIapType(0);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(SelectMacScene::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = true;
    
    
    chooseParticle = ParticleSystemQuad::create("particle/knifeParticl.plist");
    chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    chooseParticle->retain();
    
//    initBall();
    
}
void SelectMacScene::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    
    if(kIAPManager->isFree("",index)){
        
        
//        m_pGirdView->setEnable(false);
        MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
        
       //判断是否任务
        if (m_iTouchIndex == index) {
            return;
        }
        
        m_pCurrentSprite = item;
   
        
        m_iTouchIndex = index;
        
        
        chooseParticle->removeFromParent();
        chooseParticle->setVisible(true);
        chooseParticle->setPosition(item->getContentSize()*.5);
        chooseParticle->setScale(1.5);
        item->addChild(chooseParticle,4);
        
        AudioHelp::getInstance()->playEffect("choose.mp3");
        
        ui_next->setVisible(true);
        
        
    }else{
        m_pGirdView->setEnable(false);
        SceneChangeManager->enterShopScene();
        
    }
}


void SelectMacScene::touchEvent(Ref *obj , Widget::TouchEventType type)
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
void SelectMacScene::nextStep()
{
    log("next");
    hideNextButton();
    
}