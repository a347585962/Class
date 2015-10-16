#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "AdapterScreen.h"
#include "IAPManager.h"
#include "SceneManager.h"
#include "CFSystemFunction.h"
#include "Global.h"
#include "AdLoadingLayerBase.h"
#include "AdsLoadingLayer.h"
#include "HomeLayer.h"
#include "AdsManager.h"
//#include "GameController.h"
#include "MiniGameScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
//    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    kIAPManager->initIAP("IAPInfo.csv");
#endif
//
    
    //获取屏幕大小
    kAdapterScreen->screenSize = glview->getFrameSize();
    log("screenSize------%f,%f",kAdapterScreen->screenSize.width,kAdapterScreen->screenSize.height);
    
    //适配
    kAdapterScreen->setUpScreenAdapter(640, 960);
    
    srand((int)time(0));

    log("screenSize------%f,%f",kAdapterScreen->screenSize.width,kAdapterScreen->screenSize.height);

    // turn on display FPS
    //  director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    
//  SceneChangeManager- >enterGameScene();
//  SceneChangeManager->enterHomeScene();
//    SceneChangeManager->enterSelectScene();
//  SceneChangeManager->enterLoadingScene();
 
//  SceneChangeManager->enterDecScene();

//  SceneChangeManager->enterShopScene();


//  SceneChangeManager->enterCutFruitScene();
    
//  SceneChangeManager->enterFavScene();
    
 
    g_vChooseTaste.push_back("apple");
//  g_vChooseTaste.push_back("chocolate");
//  g_vChooseTaste.push_back("peach");
//  SceneChangeManager->enterMakeScene();
//  SceneChangeManager->enterMakeBatterScene();
    SceneChangeManager->enterSelectMacScene();
//    SceneChangeManager->enterPushBallScene();
    
//    Director::getInstance()->runWithScene(MiniGameScene::scene());
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
     Director::getInstance()->stopAnimation();
//    Director::getInstance()->pause();
     CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
  
    if (IAPManager::getInstance()->isShowAds())
    {
        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);
        
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
