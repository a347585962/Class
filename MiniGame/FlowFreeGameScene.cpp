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
//依次为绿，黄，蓝，橙，红

//每一个方框均有一个坐标值，例如(0,0)，现在需要在方框上加一个精灵以达到填充效果。
//将精灵的TAG值，用其坐标点表示出来。
//TAG = x*10 + y(即十位为横坐标，个位为纵坐标)

Vec2 ballInFreamPos[5][2]={{Vec2(1,1),Vec2(2,4)},{Vec2(3,1),Vec2(4,4)},{Vec2(2,3),Vec2(2,0)},{Vec2(3,0),Vec2(4,3)},{Vec2(0,4),Vec2(1,0)}};

int boxTag[25]={0,1,2,3,4,10,11,12,13,14,20,21,22,23,24,30,31,32,33,34,40,41,42,43,44};

FlowFreeGameScene::FlowFreeGameScene() {

    m_pBallShadow = nullptr;
    //阴影移动，过后的记录坐标值
    mOldLoadPos = Vec2::ZERO;
    m_iMoveCheckNum = 0;
    mMyBallType = -1;
    m_bGameIsOver =false;
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
    
    
    this->schedule(schedule_selector(FlowFreeGameScene::checkOver),2.0);
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

void FlowFreeGameScene::checkOver(float dt)
{
    Vector<Node*> vector = m_pFream->getChildren();
    int boxNum = 0;
    for (int i = 0; i<vector.size(); i++) {
        
        int tag = vector.at(i)->getTag();
        if (vector.at(i)->isVisible()) {
            
            if (checkTagVerts(tag)) {
                boxNum++;
            }
        }
    }
    log("---->%d",boxNum);
    if (boxNum == 25) {
        
        log("success");
        m_bGameIsOver = true;
        this->unschedule(schedule_selector(FlowFreeGameScene::checkOver));
    }
}
bool FlowFreeGameScene::checkTagVerts(int tag)
{

    for (int i = 0; i<25; i++) {
        
        if (tag == boxTag[i]) {
            return true;
        }
    }
    return false;
}
bool FlowFreeGameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    if (m_bGameIsOver) {
        
        return false;
    }
    
    Vec2 pos = m_pFream->convertToNodeSpace(touch->getLocation());
    Size sizeFream = m_pFream->getContentSize();
    Vec2 posNumFream = getFreamBoxPos(pos);
    log("%d",checkTouchBall(posNumFream));
    if (checkTouchBall(posNumFream)!=-1) {
        //如果点击到小球点的位置，则把所有此小球的填充路径去掉
        deleteAllChildLoad(checkTouchBall(posNumFream));
        
        int num = checkTouchBall(posNumFream);
        mMyBallType = posNumFream.x*10 + posNumFream.y;
        if(!m_pBallShadow){

            m_pBallShadow = Sprite::create(__String::createWithFormat("images/minigame/flowfree/bottom-frame2_%d.png",num+1)->getCString());
            m_pBallShadow->setPosition(pos);
            //根据球的颜色顺序，设置阴影的TAG值
            m_pBallShadow->setTag(num + 100);
            m_pFream->addChild(m_pBallShadow,1);
            
            addFillBox(posNumFream,m_pBallShadow->getTag() - 100);
        }
    }
    //如果点击到了一个已经有的位置，则把此位置以上的填充路径全部移除掉
    if(checkPosIsInVector(posNumFream)){
    
        deleteUnLoad(posNumFream);
        int num = checkTagInWhichVectro(posNumFream.x*10 + posNumFream.y);
        mMyBallType = posNumFream.x*10 + posNumFream.y;
        if(!m_pBallShadow){
            
            m_pBallShadow = Sprite::create(__String::createWithFormat("images/minigame/flowfree/bottom-frame2_%d.png",num+1)->getCString());
            m_pBallShadow->setPosition(pos);
            //根据球的颜色顺序，设置阴影的TAG值
            m_pBallShadow->setTag(num + 100);
            m_pFream->addChild(m_pBallShadow,1);
//            addFillBox(posNumFream,m_pBallShadow->getTag() - 100);
        }
    }
    return true;
    
}
void FlowFreeGameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    if(m_pBallShadow){
    
        m_pBallShadow->setPosition(m_pBallShadow->getPosition()+touch->getDelta());
        
        if (m_pBallShadow->getPosition().x > 0 && m_pBallShadow->getPosition().y<m_pFream->getContentSize().height) {
            
            Vec2 posNumFream = getFreamBoxPos(m_pBallShadow->getPosition());
            //测试此时的阴影位置，是否在某一个球上
            if (checkTouchBall(posNumFream)==-1) {
                //如果未移动到有填充物的方框，将此方框填充
                if (!checkPosIsInVector(posNumFream)) {
                    addFillBox(posNumFream,m_pBallShadow->getTag() - 100);
                }else{
                
                    moveBack(posNumFream,m_pBallShadow->getTag() - 100);
                    
                }
            }else{
                //移动到自己，或者另一个，需要特别判断
                if(checkTouchBall(posNumFream) == m_pBallShadow->getTag() - 100){
                    if (posNumFream.x*10 + posNumFream.y == mMyBallType) {
                        //checkLoad(m_pBallShadow->getTag() - 100);
                    }else{
                    
                        addFillBox(posNumFream,m_pBallShadow->getTag() - 100);
                        onTouchEnded(touch,unused_event);
                        
                    }
                }else{
                    
                    onTouchEnded(touch,unused_event);
                }
            }
            
        }else{
            onTouchEnded(touch,unused_event);
        }
    }
}
void FlowFreeGameScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    if(m_pBallShadow){
    
        m_pBallShadow->removeFromParent();
        m_pBallShadow = nullptr;
        m_iMoveCheckNum = 0;
    }
}
//将初始小球与终点小球填充颜色
void FlowFreeGameScene::addBoxFromBall(Vec2 pos,int type)
{
    
}
//如果点击到了已经填充的路径中间某个点,则根据栈的原理，删除比其后入栈的无效路径
void FlowFreeGameScene::deleteUnLoad(Vec2 pos)
{
    int tag = pos.x*10 + pos.y;
    if(checkTagInWhichVectro(tag)!=-1){
        deleteLoadFromVector(pos,checkTagInWhichVectro(tag));
    }
}
//删除对应的无效路径
//绿，黄，蓝，橙，红
void FlowFreeGameScene::deleteLoadFromVector(Vec2 pos,int type)
{
    int tag = pos.x*10 + pos.y;
    if (type == 0) {
        int k = 100;
        for (int i = 0;i< m_vGreenLoadSprite.size(); i++) {
            if (m_vGreenLoadSprite.at(i)->getTag() == tag)
                k = i;
            if (k<i) {
                auto sp = dynamic_cast<Sprite*>(m_vGreenLoadSprite.at(i));
                sp->setVisible(false);
            }
        }
    }else if (type == 1) {
        int k = 100;
        for (int i = 0;i<m_vYellowLoadSprite.size(); i++) {
            if (m_vYellowLoadSprite.at(i)->getTag() == tag)
                k = i;
            
            if (k<i) {
                auto sp = dynamic_cast<Sprite*>(m_vYellowLoadSprite.at(i));
                sp->setVisible(false);
                }
            }
    }else if (type == 2) {
        int k = 100;
        for (int i = 0;i<m_vBlueLoadSprite.size(); i++) {
            if (m_vBlueLoadSprite.at(i)->getTag() == tag)
                k = i;
            if (k<i) {
                auto sp = dynamic_cast<Sprite*>(m_vBlueLoadSprite.at(i));
                sp->setVisible(false);
            }
        }
    }else if (type == 3) {
        int k = 100;
        for (int i = 0;i<m_vOrangeLoadSprite.size(); i++) {
            if (m_vOrangeLoadSprite.at(i)->getTag() == tag)
                k = i;
            if (k<i) {
                auto sp = dynamic_cast<Sprite*>(m_vOrangeLoadSprite.at(i));
                sp->setVisible(false);
            }
        }
    }else if (type == 4) {
        int k = 100;
        for (int i = 0;i<m_vRedLoadSprite.size(); i++) {
            if (m_vRedLoadSprite.at(i)->getTag() == tag)
                k = i;
            if (k<i) {
                auto sp = dynamic_cast<Sprite*>(m_vRedLoadSprite.at(i));
                sp->setVisible(false);
            }
        }
    }
}
//根据坐标位置转换的TAG值，判断精灵是否在哪一个向量容器里面
int FlowFreeGameScene::checkTagInWhichVectro(int tag)
{
    for (auto sp:m_vGreenLoadSprite) {
        if (sp->getTag() == tag) {
            return 0;
        }
    }
    for (auto sp:m_vYellowLoadSprite) {
        if (sp->getTag() == tag) {
            return 1;
        }
    }for (auto sp:m_vBlueLoadSprite) {
        if (sp->getTag() == tag) {
            return 2;
        }
    }
    for (auto sp:m_vOrangeLoadSprite) {
        if (sp->getTag() == tag) {
            return 3;
        }
    }
    for (auto sp:m_vRedLoadSprite) {
        if (sp->getTag() == tag) {
            return 4;
        }
    }
    return -1;
}
//判断移动回滚,去掉不需要的填充物
void FlowFreeGameScene::moveBack(Vec2 pos,int type)
{
    if(type == 0){
        if (m_vGreenLoadSprite.size()>1) {
            auto sp = dynamic_cast<Sprite*>(m_vGreenLoadSprite.at(m_vGreenLoadSprite.size() - 2));
            int tag = sp->getTag();
            int num = pos.x*10 + pos.y;
            if (num == tag) {
                auto sp1 = dynamic_cast<Sprite*>(m_vGreenLoadSprite.at(m_vGreenLoadSprite.size()-1));
                Vec2 _pos = Vec2((int)sp1->getTag()/10,sp1->getTag()%10);
//                m_vGreenLoadSprite.popBack();
                sp1->removeFromParent();
                m_vGreenLoadSprite.eraseObject(sp1);
                for(vector<Vec2>::iterator it  = m_vAllLoadPos.begin(); it != m_vAllLoadPos.end();)
                {
                    if ((*it) == _pos) {
                        m_vAllLoadPos.erase(it);
                    }else{
                    
                        ++it;
                    }
                }
            }
        }
    }else if(type == 1){
        if (m_vYellowLoadSprite.size()>1) {
            auto sp = dynamic_cast<Sprite*>(m_vYellowLoadSprite.at(m_vYellowLoadSprite.size() - 2));
            int tag = sp->getTag();
            int num = pos.x*10 + pos.y;
            if (num == tag) {
                auto sp1 = dynamic_cast<Sprite*>(m_vYellowLoadSprite.at(m_vYellowLoadSprite.size()-1));
                Vec2 _pos = Vec2((int)sp1->getTag()/10,sp1->getTag()%10);
                //                m_vGreenLoadSprite.popBack();
                sp1->removeFromParent();
                m_vYellowLoadSprite.eraseObject(sp1);
                for(vector<Vec2>::iterator it  = m_vAllLoadPos.begin(); it != m_vAllLoadPos.end();)
                {
                    if ((*it) == _pos) {
                        m_vAllLoadPos.erase(it);
                    }else{
                        
                        ++it;
                    }
                }
            }
        }
    }else if(type == 2){
        if (m_vBlueLoadSprite.size()>1) {
            auto sp = dynamic_cast<Sprite*>(m_vBlueLoadSprite.at(m_vBlueLoadSprite.size() - 2));
            int tag = sp->getTag();
            int num = pos.x*10 + pos.y;
            if (num == tag) {
                auto sp1 = dynamic_cast<Sprite*>(m_vBlueLoadSprite.at(m_vBlueLoadSprite.size()-1));
                Vec2 _pos = Vec2((int)sp1->getTag()/10,sp1->getTag()%10);
                //                m_vGreenLoadSprite.popBack();
                sp1->removeFromParent();
                m_vBlueLoadSprite.eraseObject(sp1);
                for(vector<Vec2>::iterator it  = m_vAllLoadPos.begin(); it != m_vAllLoadPos.end();)
                {
                    if ((*it) == _pos) {
                        m_vAllLoadPos.erase(it);
                    }else{
                        
                        ++it;
                    }
                }
            }
        }
    }else if(type == 3){
        if (m_vOrangeLoadSprite.size()>1) {
            auto sp = dynamic_cast<Sprite*>(m_vOrangeLoadSprite.at(m_vOrangeLoadSprite.size() - 2));
            int tag = sp->getTag();
            int num = pos.x*10 + pos.y;
            if (num == tag) {
                auto sp1 = dynamic_cast<Sprite*>(m_vOrangeLoadSprite.at(m_vOrangeLoadSprite.size()-1));
                Vec2 _pos = Vec2((int)sp1->getTag()/10,sp1->getTag()%10);
                //                m_vGreenLoadSprite.popBack();
                sp1->removeFromParent();
                m_vOrangeLoadSprite.eraseObject(sp1);
                for(vector<Vec2>::iterator it  = m_vAllLoadPos.begin(); it != m_vAllLoadPos.end();)
                {
                    if ((*it) == _pos) {
                        m_vAllLoadPos.erase(it);
                    }else{
                        
                        ++it;
                    }
                }
            }
        }
    }else if(type == 4){
        if (m_vRedLoadSprite.size()>1) {
            auto sp = dynamic_cast<Sprite*>(m_vRedLoadSprite.at(m_vRedLoadSprite.size() - 2));
            int tag = sp->getTag();
            int num = pos.x*10 + pos.y;
            if (num == tag) {
                auto sp1 = dynamic_cast<Sprite*>(m_vRedLoadSprite.at(m_vRedLoadSprite.size()-1));
                Vec2 _pos = Vec2((int)sp1->getTag()/10,sp1->getTag()%10);
                //                m_vGreenLoadSprite.popBack();
                sp1->removeFromParent();
                m_vRedLoadSprite.eraseObject(sp1);
                for(vector<Vec2>::iterator it  = m_vAllLoadPos.begin(); it != m_vAllLoadPos.end();)
                {
                    if ((*it) == _pos) {
                        m_vAllLoadPos.erase(it);
                    }else{
                        
                        ++it;
                    }
                }
            }
        }
    }
}
void FlowFreeGameScene::deleteMoveBack(int type)
{
    if(type == 0){
        if (m_vGreenLoadSprite.size()>1) {
            for (auto sp:m_vGreenLoadSprite) {
                
                sp->removeFromParent();
            }
            m_vGreenLoadSprite.clear();
        }
        
    }else if(type == 1){
        if (m_vYellowLoadSprite.size()>1) {
            for (auto sp:m_vYellowLoadSprite) {
                sp->removeFromParent();
            }
            m_vYellowLoadSprite.clear();
        }
    }else if(type == 2){
        if (m_vBlueLoadSprite.size()>1) {
            for (auto sp:m_vBlueLoadSprite) {
                
                sp->removeFromParent();
            }
            m_vBlueLoadSprite.clear();
        }
        
    }else if(type == 3){
        if (m_vOrangeLoadSprite.size()>1) {
            
            for (auto sp:m_vOrangeLoadSprite) {
                
                sp->removeFromParent();
            }
            m_vOrangeLoadSprite.clear();
        }
        
    }else if(type == 4){
        if (m_vRedLoadSprite.size()>1) {
            
            for (auto sp:m_vRedLoadSprite) {
                
                sp->removeFromParent();
            }
            m_vRedLoadSprite.clear();
        }
        
    }

}
bool FlowFreeGameScene::deleteAllChildLoad(int type)
{
    if(type == 0){
        
        if (m_vGreenLoadSprite.size()!=0) {
            
            for (auto sp:m_vGreenLoadSprite) {
                
                sp->removeFromParent();
            }
            m_vGreenLoadSprite.clear();
        }
        
    }else if(type == 1){
        if (m_vYellowLoadSprite.size()!=0) {
            
            for (auto sp:m_vYellowLoadSprite) {
                

                sp->removeFromParent();
            }
            m_vYellowLoadSprite.clear();
            
        }
        
    }else if(type == 2){
        if (m_vBlueLoadSprite.size()!=0) {
            
            for (auto sp:m_vBlueLoadSprite) {
                
                sp->removeFromParent();
            }
            m_vBlueLoadSprite.clear();
        }
        
    }else if(type == 3){
        if (m_vOrangeLoadSprite.size()!=0) {
            
            for (auto sp:m_vOrangeLoadSprite) {
                
                sp->removeFromParent();
            }
            m_vOrangeLoadSprite.clear();
        }
        
    }else if(type == 4){
        if (m_vRedLoadSprite.size()!=0) {
            
            for (auto sp:m_vRedLoadSprite) {
                
                sp->removeFromParent();
            }
            m_vRedLoadSprite.clear();
        }
        
    }
    
    m_vAllLoadPos.clear();
    
    return false;
}
//判断此时的坐标，是否位于每一个
bool FlowFreeGameScene::checkPosIsInBoxCenter(Vec2 pos)
{

}

//检测某一点是否已经在容器里面
bool FlowFreeGameScene::checkPosIsInVector(Vec2 pos)
{
    log("compare");
    for(vector<Vec2>::iterator it  = m_vAllLoadPos.begin(); it != m_vAllLoadPos.end(); it++)
    {
        if ((*it) == pos) {
            return true;
        }
    }
    
    return false;
}
void FlowFreeGameScene::addFillBox(Vec2 pos,int colorType)
{
    //根据方框坐标，以及颜色类型，填充方框
    m_vAllLoadPos.push_back(pos);
    
    mOldLoadPos = pos;
    auto sprite = Sprite::create(__String::createWithFormat("images/minigame/flowfree/bottom-frame1_%d.png",colorType+1)->getCString());
    float lon = m_pFream->getContentSize().width/5;
    float _lon = m_pFream->getContentSize().height/5;
    sprite->setPosition(Vec2((pos.x + 0.5)* lon ,(pos.y + 0.5)*_lon));
    sprite->setTag(pos.x*10 + pos.y);
    m_pFream->addChild(sprite,2);
    
    //将添加的填充物，加入容器
    if(colorType == 0){
    
        m_vGreenLoadSprite.pushBack(sprite);
        
     }else if(colorType == 1){
    
        m_vYellowLoadSprite.pushBack(sprite);
        
    }else if(colorType == 2){
        
        m_vBlueLoadSprite.pushBack(sprite);
        
    }else if(colorType == 3){
        
        m_vOrangeLoadSprite.pushBack(sprite);
        
    }else if(colorType == 4){
        
        m_vRedLoadSprite.pushBack(sprite);
        
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