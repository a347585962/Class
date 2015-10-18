//
//  FlowFreeGameScene.cpp
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/15.
//
//

#include "FlowFreeGameScene.h"
#include "AdapterScreen.h"

//球在框里面的位置(整数位置，如(0,0)表示左下角第一个)
//依次为红，绿，蓝，黄，橙
Vec2 ballInFreamPos[5][2]={{Vec2(1,1),Vec2(2,4)},{Vec2(3,1),Vec2(4,4)},{Vec2(2,3),Vec2(2,0)},{Vec2(3,0),Vec2(4,3)},{Vec2(0,4),Vec2(1,0)}};
FlowFreeGameScene::FlowFreeGameScene() {

    m_pBallShadow = nullptr;
    
}

FlowFreeGameScene::~FlowFreeGameScene() {
   
}

Scene *FlowFreeGameScene::scene() {
    Scene* scene = Scene::create();
    FlowFreeGameScene* layer = FlowFreeGameScene::create();
    scene->addChild(layer);
    return scene;
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
bool FlowFreeGameScene::init() {
    if (!Layer::init()) {
        return false;
    }
    
    this->initView();
    
    this->initBall();
    
    return true;
}
void FlowFreeGameScene::initView()
{
    auto bg = Sprite::create("images/minigame/flowfree/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320,480);
    this->addChild(bg);
    
    m_pFream = Sprite::create("images/minigame/flowfree/frame.png");
    m_pFream->setPosition(kAdapterScreen->getExactPostion(Vec2(321,470)));
    this->addChild(m_pFream);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(FlowFreeGameScene::onTouchBegan,this);
    listener->onTouchMoved = CC_CALLBACK_2(FlowFreeGameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(FlowFreeGameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    
    
}
void FlowFreeGameScene::initBall()
{
    
    int lon = m_pFream->getContentSize().width/5;
    
    for (int i = 0; i<5; i++) {
        
        auto ball1 = Sprite::create(__String::createWithFormat("images/minigame/flowfree/sugar_%d.png",i+1)->getCString());
        ball1->setPosition(Vec2(ballInFreamPos[i][0].x * lon + lon/2,ballInFreamPos[i][0].y * lon + lon/2));
        m_pFream->addChild(ball1,5);
        
        auto ball2 = Sprite::create(__String::createWithFormat("images/minigame/flowfree/sugar_%d.png",i+1)->getCString());
        ball2->setPosition(Vec2(ballInFreamPos[i][1].x * lon + lon/2,ballInFreamPos[i][1].y * lon + lon/2));
        m_pFream->addChild(ball2,5);
        
        
    }
}
//根据Fream的坐标值，获取具体的方格位置
Vec2 FlowFreeGameScene::getFreamBoxPos(Vec2 pos)
{
    Size sizeFream = m_pFream->getContentSize();
    return Vec2((int)(pos.x/(sizeFream.width/5)),(int)(pos.y/(sizeFream.height/5)));
}
bool FlowFreeGameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    
    Vec2 pos = m_pFream->convertToNodeSpace(touch->getLocation());
    
    Size sizeFream = m_pFream->getContentSize();
    
    Vec2 posNumFream = getFreamBoxPos(pos);
   
    log("%d",checkTouchBall(posNumFream));
    if (checkTouchBall(posNumFream)!=-1) {
        int num = checkTouchBall(posNumFream);
        if(!m_pBallShadow){

            m_pBallShadow = Sprite::create(__String::createWithFormat("images/minigame/flowfree/bottom-frame2_%d.png",num+1)->getCString());
            m_pBallShadow->setPosition(pos);
            m_pFream->addChild(m_pBallShadow,1);
        }
    }
    return true;
    
}
void FlowFreeGameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    if(m_pBallShadow){
    
        m_pBallShadow->setPosition(m_pBallShadow->getPosition()+touch->getDelta());
        
        
        if (checkTouchBall(m_pBallShadow->getPosition())==-1) {
            
            
            
            
        }
    
    }
}
void FlowFreeGameScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    if(m_pBallShadow){
    
        m_pBallShadow->removeFromParent();
        m_pBallShadow = nullptr;
    
    }
}
//检测点击到了哪个球上
//
int FlowFreeGameScene::checkTouchBall(Vec2 pos)
{
    for (int i = 0; i<5; i++) {
        
        if (pos == ballInFreamPos[i][0]) {
            return i;
        }
        if (pos == ballInFreamPos[i][1]) {
            return i;
        }
        
    }
    return -1;
}