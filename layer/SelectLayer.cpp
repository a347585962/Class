        //
//  SelectLayer.cpp
//  MLABS1043_BrownieMaker
//
//  Created by wusonglin1 on 15-1-5.
//
//

#include "SelectLayer.h"
#include "Global.h"
#include "IAPManager.h"

#include "ActionUtils.h"
#include "AudioHelp.h"


string tastPath[30]={"apple","banana","blackcurrant","blackcurrant_lemon_grape","chocolate","grape_watermelon","cinnamon","grape","chocolate_apple_peach","fig","kiwi_fruit","strawberry_lemon","melon","orange","peach","raspberry","pear","lime","lemon","watermelon","strawberry","mint_leaf_lime","vanilla","pineapple","sour_apple","mint_leaf","lemon_orange_lime","pineapple_orange_cinnamon","vanilla_sour_apple","pineapple_melon"};

//几种口味
//3，8，26，27，28，29

int severalTast_i[6]={3,8,26,27,28,29};
string severalTast_s[6][3]={{"blackcurrant","lemon","grape"},{"chocolate","apple","peach"},{"lemon","orange","lime"},{"pineapple","orange","cinnamon"},{"vanilla","sour_apple","none"},{"pineapple","melon","none"}};

//混合口味
//5,11,21
int fixTast_i[3]={5,11,21};
string fixTast_s[3][2]={{"grape","watermelon"},{"strawberry","lemon"},{"mint_leaf","lime"}};


SelectLayer::SelectLayer()
{
    
}
SelectLayer::~SelectLayer()
{
    
}
bool SelectLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initData();
        initUI();
        
        m_pCurrentSprite = nullptr;
      
        m_pIndex = -1;
        m_iFontNum = -1;
        m_iTouchIndex = -1;
        isInit=true;
        
    }while(0);
    
    return true;
    
}

void SelectLayer::initData()
{
    
    m_vTypes.clear();
    for (int i = 0; i<30; i++) {
        
        __String* str = __String::createWithFormat("images/select/%s.png",tastPath[i].c_str());
        
        
        Sprite* glass = Sprite::create("images/select/glass_bottle0.png");
        
        Sprite* shadow = Sprite::create("images/select/glass_shadow.png");
        shadow->setAnchorPoint(Vec2::ZERO);
        glass->addChild(shadow,-1);
        
        
        Sprite* waste = Sprite::create(str->getCString());
        waste->setPosition(Vec2(134,115));
        glass->addChild(waste,5);
        
        Sprite* glassCover = Sprite::create("images/select/glass_bottle1.png");
        glassCover->setAnchorPoint(Vec2::ZERO);
        glass->addChild(glassCover,10);
        
        if (checkFix(i)) {
            int num = -1;
            for(int k=0;k<3;k++){
                if (i == fixTast_i[k]) {
                    num = k;
                }
            }
            
            auto fruit = Sprite::create(__String::createWithFormat("images/select/%s_%s_1.png",fixTast_s[num][0].c_str(),fixTast_s[num][1].c_str())->getCString());
            fruit->setPosition(Vec2(125,-11));
            glass->addChild(fruit,20);
            
            auto ball = Sprite::create(__String::createWithFormat("images/select/one_%s_%s.png",fixTast_s[num][0].c_str(),fixTast_s[num][1].c_str())->getCString());
            ball->setPosition(glass->getContentSize()/2);
            glass->addChild(ball,5);
            ball->setTag(333);
            
        }else if(checkSeveral(i)){
        
            int num = -1;
            for(int k=0;k<6;k++){
                
                if (i == severalTast_i[k]) {
                     num = k;
                }
                
                
            }

            auto fruit1 = Sprite::create(__String::createWithFormat("images/select/%s_%s_%s_1.png",severalTast_s[num][0].c_str(),severalTast_s[num][1].c_str(),severalTast_s[num][2].c_str())->getCString());
            fruit1->setPosition(Vec2(135,-11));
            glass->addChild(fruit1,20);

            
            int ballNum = rand()%2;
            auto ball = Sprite::create(__String::createWithFormat("images/select/one_%s.png",severalTast_s[num][ballNum].c_str())->getCString());
            ball->setPosition(glass->getContentSize()/2);
            glass->addChild(ball,5);
            ball->setTag(333);
            
        }else{
        
            auto fruit = Sprite::create(__String::createWithFormat("images/select/%s_1.png",tastPath[i].c_str())->getCString());
            fruit->setPosition(Vec2(130,-11));
            glass->addChild(fruit,20);
        
            auto ball = Sprite::create(__String::createWithFormat("images/select/one_%s.png",tastPath[i].c_str())->getCString());
            ball->setPosition(glass->getContentSize()/2);
            glass->addChild(ball,50);
            ball->setTag(333);
        }
        
        
        
        
        m_vTypes.pushBack(glass);
        
        
    }
    

}
bool SelectLayer::checkFix(int num)
{
    for(int i=0;i<3;i++){
    
        if (num == fixTast_i[i]) {
            return true;
        }
        
    
    }
    return false;
}
bool SelectLayer::checkSeveral(int num)
{
    for(int i=0;i<6;i++){
        
        if (num == severalTast_i[i]) {
            return true;
        }
        
        
    }
    
    
    return false;
}
void SelectLayer::initUI()
{
    auto ui_button = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("cocos_studio/choose_flavors.json");
    this->addChild(ui_button,10);

    
    Sprite* bg = Sprite::create("images/select/bg_choose.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);

    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getVisibleSize().width, 621));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
    m_pGirdView->m_fHSpace = 10;
//    m_pGirdView->m_fVSpace = 20;
    m_pGirdView->m_fMarginTop = 200;
//    m_pGirdView->m_fMarginBottom = 100;
    m_pGirdView->m_fMarginRight = 100;
    m_pGirdView->m_fMarginLeft = 100;

    m_pGirdView->setEnable(false);
    kAdapterScreen->setExactPosition(m_pGirdView, 0, 250,Vec2(0,0),kBorderLeft,kBorderBottom);
//    m_pGirdView->setPosition(kAdapterScreen->getExactPostion(Vec2(0,465 + 100)));
//    m_pGirdView->setAnchorPoint(Vec2(0,0.5));
    this->addChild(m_pGirdView,7);
    
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setIapType(0);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(SelectLayer::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = true;
    
    nextBtn = ui::Helper::seekWidgetByName(ui_button, "btn_next");
    nextBtn->setTag(100);
    nextBtn->setVisible(false);
    nextBtn->addTouchEventListener(CC_CALLBACK_2(SelectLayer::touchEvent, this));
    
    chooseParticle = ParticleSystemQuad::create("particle/knifeParticl.plist");
    chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    chooseParticle->retain();
    
    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-520,0));
    
    
    for (int i = 0; i<m_vTypes.size(); i++) {
        

        auto card = Sprite::create(__String::createWithFormat("images/select/flavor_card/card%d.png",i)->getCString());
        card->setAnchorPoint(Vec2(0.48,0.03));
        card->setPosition(Vec2(127,337));
        m_vTypes.at(i)->addChild(card);
        
        
    }
    

}
void SelectLayer::onEnter()
{
    BaseLayer::onEnter();
    
    
    m_pGirdView->refresh();
    m_pCurrentSprite=nullptr;
}
void SelectLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
    chooseAction();
    if (kIAPManager->isShowAds()) {
        
        AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeBannerAds);
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    }
    
}
void SelectLayer::onExit()
{
    BaseLayer::onExit();
}
void SelectLayer::titleAction()
{

}
void SelectLayer::fontAction()
{
    m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(0,0), 0.5);
    auto func = CallFunc::create([=](){
        m_pGirdView->setEnable(true);
    });
    this->runAction(Sequence::create(DelayTime::create(1.0),func, NULL));
    
}
void SelectLayer::chooseAction()
{
   m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(-1020,0), 1.0);
    
    
   auto func = CallFunc::create([=](){
//       m_pGirdView->setEnable(true);
       fontAction();
       
   });
    this->runAction(Sequence::create(DelayTime::create(1.5),func, NULL));
}
void SelectLayer::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    
    if(kIAPManager->isFree("",index)){
    

        m_pGirdView->setEnable(false);
        MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);

        //设置口味数据
        setChooseData(index);
        
        //判断是否任务
        if (m_iTouchIndex == index) {
            return;
        }
        
        m_pCurrentSprite = item;
        m_pIndex = index;
        
        m_iTouchIndex = index;
       
        
        auto ball = dynamic_cast<Sprite*>(item->getNormalImage()->getChildByTag(333));
        
        
        for(int i = 0;i<3;i++){
            
            auto _ball = Sprite::createWithTexture(ball->getTexture());
            _ball->setPosition(item->convertToWorldSpace(item->getContentSize()/2));
            this->addChild(_ball,10);
            
            JumpBy* jump = JumpBy::create(1.0, Vec2(0,0), 300, 1);
            _ball->runAction(Sequence::create(DelayTime::create(i * 0.25),jump,RemoveSelf::create(),CallFunc::create([=](){
                
                if(i == 2){
                    m_iTouchIndex = -1;
                    m_pGirdView->setEnable(true);
                }
                
                }),NULL));
        
        }
        
        chooseParticle->removeFromParent();
        chooseParticle->setVisible(true);
        chooseParticle->setPosition(item->getContentSize()*.5);
        chooseParticle->setScale(1.5);
        item->addChild(chooseParticle,4);
        
        AudioHelp::getInstance()->playEffect("choose.mp3");
        
        nextBtn->setVisible(true);
        
        
    }else{
        m_pGirdView->setEnable(false);
        SceneChangeManager->enterShopScene();
    
    }
}
//设置口味的数据，存入一个全局容器
void SelectLayer::setChooseData(int index)
{
    g_vChooseTaste.clear();
    if (checkFix(index)) {
        
        int num = -1;
        for(int k=0;k<3;k++){
            if (index == fixTast_i[k]) {
                num = k;
            }
        }
        g_vChooseTaste.push_back(fixTast_s[num][0]);
        g_vChooseTaste.push_back(fixTast_s[num][1]);
        
    }else if(checkSeveral(index)){
        
        int num = -1;
        for(int k=0;k<6;k++){
            
            if (index == severalTast_i[k]) {
                num = k;
            }
        }
        g_vChooseTaste.push_back(severalTast_s[num][0]);
        g_vChooseTaste.push_back(severalTast_s[num][1]);
        
        if(severalTast_s[num][2]!= "none"){
            g_vChooseTaste.push_back(severalTast_s[num][2]);
        }
        
        
    }else {
        
        g_vChooseTaste.push_back(tastPath[index]);
        
    }
    

}
void SelectLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
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
            dialog->setContentText("Are you sure you want to start over?");
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }
    }
}

void SelectLayer::onNegativeClick(void* type)
{
    m_bIsShowReturn = false;
}
void SelectLayer::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
    
    if(kIAPManager->isShowAds())
    {
        
        AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
        AdLoadingLayerBase::loadingDoneCallback = []()
        {
            Director::getInstance()->popToRootScene();
            SceneChangeManager->enterHomeScene();
        };
    }
    else
    {
        Director::getInstance()->popToRootScene();
        SceneChangeManager->enterHomeScene();
    }
    
}
void  SelectLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case 100:
                {
//                    SceneChangeManager->enterCutFruitScene();
                    SceneChangeManager->enterMakeScene();
                }
                    break;
            }
        }
    }

}
