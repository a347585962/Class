//
//  ShopLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-24.
//
//

#include "ShopLayer.h"
#include "SceneManager.h"
#include "IAPManager.h"
#include "AudioHelp.h"
#include "Global.h"
#include "ActionUtils.h"

static vector<string> IAPTypes={"shop_btn_christmas pack@2x.png","shop_btn_decoration@2x.png","shop_btn_cone@2x.png","shop_btn_fruit@2x.png",
    "shop_btn_restore@2x.png"};
string shopItemBg[3]={"unlockall.png","morecake.png","moredecorations.png"};

ShopLayer::ShopLayer()
{
    
}
ShopLayer::~ShopLayer()
{
    
}

void ShopLayer::onEnter()
{
    BaseLayer::onEnter();
    _eventDispatcher->addCustomEventListener("closeAds", CC_CALLBACK_0(ShopLayer::shopCloaeAds, this));
    if (kIAPManager->isShowAds()) {
        AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, false);
    }
}
void ShopLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}

void ShopLayer::onExit()
{
    BaseLayer::onExit();
    if (kIAPManager->isShowAds()) {
        AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, true);
    }
    
}
void ShopLayer::shopCloaeAds()
{
    if(kIAPManager->isShowAds()){
        
        AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, false);
    }
}
bool ShopLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        isFirstIn=true;
        
        initUI();
        isInit=true;
        
    }while(0);
    
    return true;
    
}
void ShopLayer::initData()
{
  
    for(auto i = IAPTypes.begin();i<IAPTypes.end();i++)
    {
        string buypath = (*i).c_str();
        Sprite* data = Sprite::create("images/shop_fav/"+buypath);
        m_vTypes.pushBack(data);
    }
}
void ShopLayer::initUI()
{
    kIAPManager->m_fnAfterPurchase = CC_CALLBACK_1(ShopLayer::purchaseFinishCallback, this);
    kIAPManager->m_fnAfterRestore = CC_CALLBACK_2(ShopLayer::AfterRestoreCallBack, this);

    Sprite* m_pBgSprite=Sprite::create("images/home/start_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBgSprite, 320, 480);
    this->addToBackGroundLayer(m_pBgSprite, 3);
    
    auto backBtn = ui::Button::create("images/button/btn_back@2x.png");
    backBtn -> addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchEvent, this));
    backBtn->setTag(30);
    kAdapterScreen->setExactPosition(backBtn, 10, 10,Vec2(0, 1),kBorderLeft,kBorderTop);
    this->addToUILayer(backBtn, 20);
   
    auto title = Sprite::create("images/shop_fav/shop.png");
    kAdapterScreen->setExactPosition(title, 320, 20,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addToUILayer(title, 10);
    
    
    for (int i=0; i<3; i++) {
        
        auto itemBuy = ui::Button::create(__String::createWithFormat("images/shop_fav/%s",shopItemBg[i].c_str())->getCString());
        kAdapterScreen->setExactPosition(itemBuy, 320,150 + 222* i,Vec2(0.5,1),kBorderNone,kBorderTop);
        itemBuy -> addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchEvent, this));
        itemBuy->setTag(100+i);
        itemBuy->setScale(0.8);
        this->addToContentLayer(itemBuy, 5);
    }
    

}

void ShopLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    if(type == ui::Widget::TouchEventType::ENDED&&btn->getTag() == 30)
    {
        SceneManager::getInstance()->popScene();
        AudioHelp::getInstance()->playDeleteEffect();
        
    }else if (type == ui::Widget::TouchEventType::ENDED&&btn->getTag()>90)
    {
        int tag = btn->getTag()-100;
        
        if(!kIAPManager->isPackagePurchased(tag))
        {
            
            kIAPManager->purchase(tag);
            
        }
        else
        {
            
            Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE);
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText("You have already purchased this package!");
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }

        
        
    }else if(type == ui::Widget::TouchEventType::ENDED&&btn->getTag() == 40){
    
        
        
        
    }
}

void ShopLayer::onScrolling(float offsetX,float offsetY)
{

    log("%f %f",offsetX,offsetY);
}
void ShopLayer::onScrollEnd(float offsetX,float offsetY)
{

}
void ShopLayer::onScrollDidZoom(cocos2d::extension::ScrollView* view)
{

}
void ShopLayer::onBuyCallback(int tag,MenuItem* menuItem,int index)
{
    log("-----------IAP %d",index);

    if(index == 4){
        
        kIAPManager->restore();
        return;
    }
    
    if(!kIAPManager->isPackagePurchased(index))
    {

        kIAPManager->purchase(index);
        
    }
    else
    {
        
        Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE);
        kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
        
        dialog->setContentText("You have already purchased this package!");
        
        dialog->setPositiveBtnText("");
        dialog->setNegativeBtnText("");
        dialog->setCallback(this);
        this->addToUILayer(dialog, 9999);
    }
    
    
}
void ShopLayer::AfterRestoreCallBack(bool isRestore,bool isPurchase)
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
void ShopLayer::onNegativeClick(void* type)
{
}

void ShopLayer::onPositiveClick(void* type)
{
}

void ShopLayer::purchaseFinishCallback(bool isPurchase)
{
    log("Purchase is successful");
}

