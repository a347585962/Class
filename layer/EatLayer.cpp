//
//  EatLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-11-10.
//
//

#include "EatLayer.h"
#include "AdapterScreen.h"
#include "ToolSprite.h"
#include "Dialog.h"
#include "CFSystemFunction.h"
#include "FavoriteManager.h"
#include "AudioHelp.h"
#include "ActionUtils.h"
#include "IAPManager.h"

enum buttonType
{
    KBACKBUTTON,
    KFAVORITEBUTTON,
    KHOMEBUTTON,
    KNEXTBUTTON,
    KEMAILBUTTON,
    KSAVEBUTTON,
};
EatLayer::EatLayer():m_pSpriteTip(nullptr)
{
    
}
EatLayer::~EatLayer()
{
    
}
bool EatLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initUI();
        initMenu();
        
        isInit=true;
        
    }while(0);
    
    return true;
    
}

void EatLayer::initUI()
{
    __String* bg_str = nullptr;
    bg_str = __String::create("images/dec/decoration_bg.jpg");
  
    auto bg = Sprite::create(bg_str->getCString());
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addToBackGroundLayer(bg,5);
    
    __String* cone_str = nullptr;
    if(g_dec_plates == -1)
        cone_str = __String::create("images/cream/stands.png");
    else
        cone_str = __String::createWithFormat("images/dec/plates/plates%d.png",g_dec_plates);

    auto pCone = Sprite::create(cone_str->getCString());
    pCone->setPosition(Vec2(bg->getContentSize().width/2,460));
    bg->addChild(pCone);
    
    
    auto cake = Sprite::create(__String::create("images/cream/cream.png")->getCString());
    cake->setPosition(this->convertToWorldSpace(pCone->convertToWorldSpace(Vec2(300,430))));
    this->addToContentLayer(cake,5);
   
    
   
    
    
    m_pEatAgain = ToolSprite::create("images/dec/eat again.png");
    kAdapterScreen->setExactPosition(m_pEatAgain, 320, 100 - 500,Vec2(0.5, 1),kBorderNone,kBorderTop);
    m_pEatAgain->setDelegate(this);
    m_pEatAgain->setIsMove(false);
    m_pEatAgain->setTag(100);
    m_pEatAgain->cancelEvent(true);
    this->addToUILayer(m_pEatAgain,20);
    
    
    RenderTexture *rt = RenderTexture::create(visibleSize.width, visibleSize.height);
    rt->begin();
    
//    pCone->visit();
    if(!g_eat_cake){
        
        cake->visit();
        
    }
    decorationLayer->visit();
    
    
    rt->end();
    Director::getInstance()->getRenderer()->render();
    rt->saveToFile("eat.png");
    this->scheduleOnce(schedule_selector(EatLayer::getScribbleCallback), 0.2f);
  
    RenderTexture *rt1 = RenderTexture::create(visibleSize.width, visibleSize.height);
    rt1->begin();
    
    if(!g_eat_cake){
        
        
        
    }
    
    
    rt1->end();
    Director::getInstance()->getRenderer()->render();
    rt1->saveToFile("eat1.png");
    this->scheduleOnce(schedule_selector(EatLayer::_getScribbleCallback), 0.2f);
    
//    pCone->setVisible(false);
    cake ->setVisible(false);
}
void EatLayer::initMenu()
{
    auto favBtn = ui::Button::create("images/button/btn_favorite@2x.png");
    favBtn -> addTouchEventListener(CC_CALLBACK_2(EatLayer::touchEvent, this));
    favBtn->setTag(KFAVORITEBUTTON);
    kAdapterScreen->setExactPosition(favBtn, 320 + 88, 100,Vec2(0.5, 0),kBorderNone,kBorderBottom);
    this->addToUILayer(favBtn, 20);
    
    auto homeBtn = ui::Button::create("images/button/btn_home@2x.png");
    homeBtn -> addTouchEventListener(CC_CALLBACK_2(EatLayer::touchEvent, this));
    homeBtn->setTag(KHOMEBUTTON);
    kAdapterScreen->setExactPosition(homeBtn, 320 + 88 + 88, 100,Vec2(0.5, 0),kBorderNone,kBorderBottom);
    this->addToUILayer(homeBtn, 20);
    
    auto backBtn = ui::Button::create("images/button/btn_back@2x.png");
    backBtn -> addTouchEventListener(CC_CALLBACK_2(EatLayer::touchEvent, this));
    backBtn->setTag(KBACKBUTTON);
    kAdapterScreen->setExactPosition(backBtn, 320 - 88 - 88, 100,Vec2(0.5, 0),kBorderNone,kBorderBottom);
    this->addToUILayer(backBtn, 20);
    
//    auto nextBtn = ui::Button::create("images/button/btn_next@2x.png");
//    nextBtn -> addTouchEventListener(CC_CALLBACK_2(EatLayer::touchEvent, this));
//    nextBtn->setTag(KNEXTBUTTON);
//    kAdapterScreen->setExactPosition(nextBtn, 10, 100,Vec2(0.5, 0),kBorderRight,kBorderBottom);
//    this->addToUILayer(nextBtn, 20);
    
    auto emailBtn = ui::Button::create("images/button/btn_mail@2x.png");
    emailBtn -> addTouchEventListener(CC_CALLBACK_2(EatLayer::touchEvent, this));
    emailBtn->setTag(KEMAILBUTTON);
    kAdapterScreen->setExactPosition(emailBtn, 320 - 88, 100,Vec2(0.5, 0),kBorderNone,kBorderBottom);
    this->addToUILayer(emailBtn, 20);
    
    auto saveBtn = ui::Button::create("images/button/btn_save@2x.png");
    saveBtn -> addTouchEventListener(CC_CALLBACK_2(EatLayer::touchEvent, this));
    saveBtn->setTag(KSAVEBUTTON);
    kAdapterScreen->setExactPosition(saveBtn, 320, 100,Vec2(0.5, 0),kBorderNone,kBorderBottom);
    this->addToUILayer(saveBtn, 20);
}

void EatLayer::onEnter()
{
    BaseLayer::onEnter();
    
    
}
void EatLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
    _showTip();
    if(kIAPManager->isShowAds()){
        
        //        AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeBannerAds);
        AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, true);
    }
}
void EatLayer::onExit()
{
    BaseLayer::onExit();
   
}
void EatLayer::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    m_pEatAgain->cancelEvent(true);
    ActionUtils::moveBy(m_pEatAgain, Vec2(0,500),0.2);

    this->scheduleOnce(schedule_selector(EatLayer::getScribbleCallback), 0.2f);
    this->scheduleOnce(schedule_selector(EatLayer::_getScribbleCallback), 0.2f);
}

void EatLayer::onBtnCallBack(Ref* sender)
{
    
}
void EatLayer::eatAgain()
{
    if(node&&nodeBom){
      
        AudioHelp::getInstance()->stopLoofEffect();
        node->disable();
        node->removeFromParent();
        node = nullptr;
        
        nodeBom->disable();
        nodeBom->removeFromParent();
        nodeBom = nullptr;
       
        
//        this->unschedule(schedule_selector(EatLayer::getScribbleCallback));
//        this->unschedule(schedule_selector(EatLayer::_getScribbleCallback));
        m_pEatAgain->cancelEvent(false);
        
        ScaleBy* move = ScaleBy::create(0.5, 1.1);
        m_pEatAgain->runAction(RepeatForever::create(Sequence::create(move,move->reverse(),NULL)));
        
    }


}
void EatLayer::onScribbleTouchOver()
{
    if(node && nodeBom){
        
        node->disable();
        nodeBom->disable();
    }
    auto func = CallFunc::create([&](){
        
        eatAgain();
    });
    ActionUtils::moveBy(m_pEatAgain, Vec2(0,-500),1.0,func);

}
void EatLayer::onScribbleTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
    AudioHelp::getInstance()->playEffect("eat.mp3");
    
    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/biting.plist");
    emitter1->setDuration(0.2);
    //    emitter1->setPosition(sprite->getParent()->convertToWorldSpace(sprite->getPosition()));
    emitter1->setAutoRemoveOnFinish(true);
    emitter1->setPosition(pTouch->getLocation());
    this->getParent()->addChild(emitter1, 30);
}
void EatLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case KFAVORITEBUTTON:
                {
                    m_pFavoriteTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
                    m_pFavoriteTexture->retain();
                    m_pFavoriteTexture->begin();
                    m_pBackGroundLayer->visit();
                    m_pContentLayer->visit();
                    m_pFavoriteTexture->end();
                    Director::getInstance()->getRenderer()->render();
                    m_pFavoriteTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
                    
                    Sprite* brush = Sprite::create("images/shop_fav/mask.png");
                    Sprite* mixFavImage = Sprite::create("images/shop_fav/frame.png");
                    Sprite* bg = Sprite::createWithTexture(m_pFavoriteTexture->getSprite()->getTexture());
                    bg->setFlippedY(true);

                    bg->setScale(600/this->getContentSize().height);
                   
                    
                    m_pFavoriteTexture = RenderTexture::create(brush->getContentSize().width, brush->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
                    m_pFavoriteTexture->retain();
                    m_pFavoriteTexture->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
                    
                    BlendFunc ccbFunc = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
                    brush->setBlendFunc(ccbFunc);
                    brush->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
                    
                    m_pFavoriteTexture->begin();
                    mixFavImage->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
                    mixFavImage->visit();
                    bg->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
                    
                    bg->visit();
                    brush->visit();
                    m_pFavoriteTexture->end();

                    
                    scheduleOnce(schedule_selector(EatLayer::saveToPhoto), 0.01f);
                    btn->setTouchEnabled(false);
                }
                    break;
                case KHOMEBUTTON:
                {
                    AudioHelp::getInstance()->playSelectEffect();
                    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
                    //                    dialog -> setAnchorPoint(Vec2(0,0));
                    //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
                    dialog -> setAnchorPoint(Vec2(0,0));
                    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
                    dialog->setContentText(__String::createWithFormat("Do you want to start over with a new food?")->getCString());
                    dialog->setPositiveBtnText("");
                    dialog->setNegativeBtnText("");
                    dialog->setCallback(this);
                    this->addToUILayer(dialog, 9999);
                    
                }
                    break;
                case KBACKBUTTON:
                {
                    AudioHelp::getInstance()->playEffect("previous page.mp3");
                    SceneManager::getInstance()->popScene();
                    remove(__String::createWithFormat("%seat.png",FileUtils::getInstance()->getWritablePath().c_str())->getCString());
                    
                    remove(__String::createWithFormat("%seat1.png",FileUtils::getInstance()->getWritablePath().c_str())->getCString());
                    
                    if(node&&nodeBom){
                    
                        node->removeFromParent();
                        nodeBom->removeFromParent();
                    
                    }
                    remove(__String::createWithFormat("%scake.png",FileUtils::getInstance()->getWritablePath().c_str())->getCString());
                    
                    
                    
                }
                    break;
                case KNEXTBUTTON:
                {
                    remove(__String::createWithFormat("%seat.png",FileUtils::getInstance()->getWritablePath().c_str())->getCString());
                    
                    remove(__String::createWithFormat("%seat1.png",FileUtils::getInstance()->getWritablePath().c_str())->getCString());
                    
                    remove(__String::createWithFormat("%scake.png",FileUtils::getInstance()->getWritablePath().c_str())->getCString());
                    
                    Director::getInstance()->popToRootScene();
                    SceneChangeManager->enterSelectScene();
                    
                }
                    break;
                case KEMAILBUTTON:
                {
                    
                    _m_pFavoriteTexture = RenderTexture::create(visibleSize.width, visibleSize.height,Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
                    
                    _m_pFavoriteTexture->retain();
                    _m_pFavoriteTexture->begin();
                    m_pBackGroundLayer->visit();
                   
                    
                    m_pContentLayer->visit();
                    
                    _m_pFavoriteTexture->end();
                    Director::getInstance()->getRenderer()->render();
                    _m_pFavoriteTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
                    
                    
                    btn->setTouchEnabled(false);
                  
                    bool success = _m_pFavoriteTexture->saveToFile("cake.png");
                    //bool success = m_pFavoriteTexture->saveToFile(m_sPath.c_str());
//                    Image* image1 = _m_pFavoriteTexture->newImage();
//                    bool success = image1->saveToFile("cake.png");
                    if (success) {
                        log("----------->success!");
                    }
                    this->scheduleOnce(schedule_selector(EatLayer::savePic), 0.5f);
                }
                    break;
                case KSAVEBUTTON:
                {
                    _m_pFavoriteTexture = RenderTexture::create(visibleSize.width, visibleSize.height,Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
                    
                    _m_pFavoriteTexture->retain();
                    _m_pFavoriteTexture->begin();
                    m_pBackGroundLayer->visit();
                    
                    
                    m_pContentLayer->visit();
                    
                    _m_pFavoriteTexture->end();
                    Director::getInstance()->getRenderer()->render();
                    _m_pFavoriteTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
                    

                    
                    
                    AudioHelp::getInstance()->playEffect("button_camera.mp3");
                    scheduleOnce(schedule_selector(EatLayer::_saveToPhoto), 0.01f);
                    btn->setTouchEnabled(false);
                }
                    break;
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
    }
}
void EatLayer::savePic(float ft)
{
    
    //    string str = FileUtils::getInstance()->getWritablePath()+"email.png";
    
        log("%s",m_sPath.c_str());
    string str = FileUtils::getInstance()->getWritablePath()+"cake.png";
    log("%s",str.c_str());
    CFSystemFunction sys;
    sys.sendEmailAndFilePic("", "I just made this yummy Cake in the application! Let's play together. ", str.c_str());
    Button *btn = dynamic_cast<Button *>(this->m_pUILayer->getChildByTag(KEMAILBUTTON));
    btn->setTouchEnabled(true);
}
void EatLayer::_saveToPhoto(float ft)
{
    Image* image1 = _m_pFavoriteTexture->newImage();
    CFSystemFunction sys;
    sys.saveToAlbum(image1, CC_CALLBACK_1(EatLayer::_addFavoriteCallback, this),"Cake");
}
void EatLayer::_showTip()
{
    
    if (!m_pSpriteTip) {
        m_pSpriteTip = Sprite::create("images/dec/tte@2x.png");
        this->addToUILayer(m_pSpriteTip, 255);
        
        m_pSpriteTip->setZOrder(101);
    }
    showTip();
}
void EatLayer::showTip()
{
    if (m_pSpriteTip) {
        m_pSpriteTip->setPosition(Vec2(visibleSize.width+534.30f,877.51f));
        m_pSpriteTip->runAction(Sequence::create(MoveTo::create(1.5f, kAdapterScreen->getExactPostion(Vec2(320.30f,877.51f))),
                                                 DelayTime::create(1.0f),
                                                 MoveTo::create(1.5f, kAdapterScreen->getExactPostion(Vec2(-320.30f,877.51f))),
                                                 nullptr));
        
    }
}

void EatLayer::onNegativeClick(void*)
{
    m_bIsShowReturn = false;
}

void EatLayer::getScribbleCallback(float ft)
{
    
    node  = ScribbleTouchNode::create(Size(visibleSize.width, visibleSize.height));
    string str = FileUtils::getInstance()->getWritablePath()+"eat.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str);
    node->getScribble()->antiAliasing(node->getCanvas());
    node->useTarget(str);
    node->setDelegate(this);
    node->useBrush(str,Scribble::eBrush);
    node->paint(Vec2::ZERO);
    kAdapterScreen->setExactPosition(node, 320, 480);
    this->addToContentLayer(node, 10);
    node->enable();
    node->enableTouchMoved(false);
    node->useBrush("images/button/eat_mask1.png",Scribble::eEaser);
    
}
void EatLayer::_getScribbleCallback(float ft)
{
    
    nodeBom  = ScribbleTouchNode::create(Size(visibleSize.width, visibleSize.height));
    string str = FileUtils::getInstance()->getWritablePath()+"eat1.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str);
    nodeBom->getScribble()->antiAliasing(nodeBom->getCanvas());
    nodeBom->useTarget(str);
    nodeBom->setDelegate(nullptr);
    nodeBom->useBrush(str,Scribble::eBrush);
    nodeBom->paint(Vec2::ZERO);
    kAdapterScreen->setExactPosition(nodeBom, 320, 480);
    this->addToContentLayer(nodeBom, 5);
    nodeBom->enable();
    nodeBom->enableTouchMoved(false);
    nodeBom->useBrush("images/button/eat_mask.png",Scribble::eEaser);
    
}


void EatLayer::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
    size_t lType = (size_t)type;
    if (lType == kDialogReturnHome)
    {
        
        
        if(kIAPManager->isShowAds())
        {
            AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, false);
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
    if (lType == kDialogTypeNone)
    {
        
    }
    
}

void EatLayer::saveToPhoto(float ft)
{
    FavoriteManager::getInstance()->saveToFavorite(m_pFavoriteTexture,CC_CALLBACK_1(EatLayer::addFavoriteCallback, this),FAVORITE_DIR);
}
void EatLayer::_addFavoriteCallback(bool isSuc)
{
    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogTypeNone, Dialog::DIALOG_TYPE_SINGLE);
    
    dialog -> setAnchorPoint(Vec2(0,0));
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    
    if(isSuc)
    {
        
        dialog->setContentText(__String::create("Your photo has been saved to your album!")->getCString());
    }
    else
    {
        dialog->setContentText("Fail to save photo!");
    }
    Button *btn = dynamic_cast<Button *>(this->m_pUILayer->getChildByTag(KSAVEBUTTON));
    btn->setTouchEnabled(true);
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    this->addToUILayer(dialog, 9999);
}

void EatLayer::addFavoriteCallback(bool isSuc)
{
    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogTypeNone, Dialog::DIALOG_TYPE_SINGLE);
    
    dialog -> setAnchorPoint(Vec2(0,0));
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    
    if(isSuc)
    {
        
        dialog->setContentText(__String::create("Your food has been added to your favorites.")->getCString());
    }
    else
    {
        dialog->setContentText("Save Favorite Failed!");
    }
    Button *btn = dynamic_cast<Button *>(this->m_pUILayer->getChildByTag(KFAVORITEBUTTON));
    btn->setTouchEnabled(true);
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    this->addToUILayer(dialog, 9999);
}



// 监听Android返回键事件
void EatLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!m_bIsShowReturn)
        {
            m_bIsShowReturn = true;
            Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText(__String::createWithFormat("Are you sure you want to start over?")->getCString());
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }
    }
}


