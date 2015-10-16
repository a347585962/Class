//
//  FlowFreeGameScene.cpp
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/15.
//
//

#include "FlowFreeGameScene.h"
FlowFreeGameScene::FlowFreeGameScene() {

}

FlowFreeGameScene::~FlowFreeGameScene() {
   
}

Scene *FlowFreeGameScene::scene() {
    Scene* scene = Scene::create();
    FlowFreeGameScene* layer = FlowFreeGameScene::create();
    scene->addChild(layer);
    return scene;
}

bool FlowFreeGameScene::init() {
    if (!Layer::init()) {
        return false;
    }
    
    return true;
}
void FlowFreeGameScene::onEnter()
{
    Layer::onEnter();
}
void FlowFreeGameScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void FlowFreeGameScene::onExit()
{
    Layer::onExit();
}