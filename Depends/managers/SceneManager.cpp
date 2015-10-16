//
//  SceneManager.cpp
//  X_BrainTest_zlwd
//
//  Created by studio on 14-1-18.
//
//

#include "SceneManager.h"
#include "HomeScene.h"

#include "FavScene.h"
#include "ShareScene.h"
#include "ShopScene.h"
#include "EatScene.h"
#include "LoadingScene.h"
#include "SelectScene.h"
#include "DecorationScene.h"

#include "CutFruitScene.h"
#include "MakeScene.h"
#include "MakeBatterScene.h"
#include "SelectMacScene.h"
#include "PushBallScene.h"
USING_NS_CC;

SceneManager::SceneManager(){
}

void SceneManager::enterLoadingScene()
{
    Scene* pScene = LoadingScene::create();
    this->replaceScene(pScene);
}

Scene* SceneManager::enterHomeScene()
{
    Scene* pScene = HomeScene::create();
    this->replaceScene(pScene);
    return pScene;
}
Scene* SceneManager::enterSelectScene()
{
    Scene* pScene = SelectScene::create();
    //this->showScene(pScene);
    this->replaceScene(pScene);
    
    return pScene;
}
void SceneManager::enterShareScene()
{
    Scene* pScene = ShareScene::create();
    this->showScene(pScene);
}

void SceneManager::enterFavScene()
{
    
    Scene* pScene = FavScene::create();
    this->showScene(pScene);
}

void SceneManager::enterShopScene()
{
    Scene* pScene = ShopScene::create();
    this->showScene(pScene);
}

void SceneManager::popScene()
{
    Director::getInstance()->popScene();
}
void SceneManager::popToRootScene()
{
    Director::getInstance()->popToRootScene();
    SceneManager::enterHomeScene();
}

void SceneManager::enterDecScene()
{
    Scene* pScene = DecorationScene::create();
    this->replaceScene(pScene);

}

Scene* SceneManager::enterEatScene()
{
    Scene* pScene = EatScene::create();
    this->showScene(pScene);
    
    return pScene;

}
void SceneManager::enterCutFruitScene()
{
    Scene* pScene = CutFruitScene::createScene();
    this->replaceScene(pScene);

}
void SceneManager::enterMakeScene()
{
    Scene* pScene = MakeScene::createScene();
    this->replaceScene(pScene);

}
void SceneManager::enterMakeBatterScene()
{
    Scene* pScene = MakeBatterScene::createScene();
    this->replaceScene(pScene);

}
void SceneManager::enterSelectMacScene()
{
    Scene* pScene = SelectMacScene::createScene();
    this->replaceScene(pScene);

}
void SceneManager::enterPushBallScene()
{
    Scene* pScene = PushBallScene::createScene();
    this->replaceScene(pScene);
    
}
void SceneManager::showScene(cocos2d::Scene *scene)
{
    if( Director::getInstance()->getRunningScene() )
        Director::getInstance()->pushScene(TransitionFade::create(0.5, scene, Color3B::WHITE));
    else
        Director::getInstance()->runWithScene(scene);
}

void SceneManager::replaceScene(cocos2d::Scene *scene)
{
    if( Director::getInstance()->getRunningScene() )
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene, Color3B::WHITE));
    else
        Director::getInstance()->runWithScene(scene);
    
}