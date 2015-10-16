//
//  MakeBatterScene.cpp
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/14.
//
//

#include "MakeBatterScene.h"

string makeBatter_fruit[3]={"","",""};

Vec2 linePos[4]={Vec2(262,26),Vec2(196,26),Vec2(131,26),Vec2(64,26)};
Vec2 batterPos[5]={Vec2(23,25),Vec2(90,25),Vec2(148,25),Vec2(204,25),Vec2(259,25)};
Vec2 cutBatterMovePos[5]={Vec2(156,452),Vec2(229,514),Vec2(363,541),Vec2(488,523),Vec2(581,470)};

Vec2 verts[40]={
   Vec2(-89.4f, 145.2f),
   Vec2(-93.1f, 145.8f),
   Vec2(-75.0f, 114.0f),
   Vec2(-72.3f, 101.5f),
   Vec2(-79.8f, 86.3f),
   Vec2(-93.2f, 72.0f),
   Vec2(-114.6f, 43.8f),
   Vec2(-125.3f, 4.7f),
   Vec2(-127.4f, -16.8f),
   Vec2(-120.5f, -50.6f),
   Vec2(-107.7f, -67.3f),
   Vec2(-100.1f, -74.5f),
   Vec2(-86.0f, -53.0f),
   Vec2(-67.0f, -52.0f),
   Vec2(-47.5f, -48.8f),
   Vec2(-37.5f, -49.9f),
   Vec2(-29.6f, -30.1f),
   Vec2(-28.5f, -15.6f),
   Vec2(-14.6f, -10.3f),
   Vec2(2.0f, -25.5f),
   Vec2(7.9f, -48.1f),
   Vec2(20.7f, -26.9f),
   Vec2(31.3f, -21.1f),
   Vec2(42.7f, -19.0f),
   Vec2(55.1f, -24.9f),
   Vec2(65.5f, -39.4f),
   Vec2(84.2f, -45.3f),
   Vec2(99.8f, -55.3f),
   Vec2(112.9f, -48.2f),
   Vec2(119.0f, -40.9f),
   Vec2(123.1f, -14.7f),
   Vec2(122.5f, 14.4f),
   Vec2(113.8f, 43.8f),
   Vec2(95.8f, 66.2f),
   Vec2(78.5f, 86.9f),
   Vec2(73.6f, 110.8f),
   Vec2(81.5f, 130.4f),
   Vec2(86.3f, 139.0f),
   Vec2(88.7f, 142.7f),
   Vec2(-91.8f, 145.8f),

};

MakeBatterScene::MakeBatterScene():m_iFriutNum(0),m_iBatterMoveNum(0),oldPos(Vec2::ZERO),m_iLineNum(0),m_bKnife(true),m_pTouch(true),m_iBallTag(0),m_iBallNum(0),m_bIsAction(true)
{
    
}
MakeBatterScene::~MakeBatterScene()
{
    
}
Scene* MakeBatterScene::createScene()
{
    Scene* scene = Scene::createWithPhysics();
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);//调试
    scene->getPhysicsWorld()->setGravity(Vec2(0,-1000));
    
    MakeBatterScene* layer = MakeBatterScene::create();
    scene->addChild(layer);
    return scene;
}
bool MakeBatterScene::init()
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
void MakeBatterScene::onEnter()
{
    GameBaseScene::onEnter();
}
void MakeBatterScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
}
void MakeBatterScene::onExit()
{
    GameBaseScene::onExit();
}
void MakeBatterScene::initUI()
{

    m_pBG = Sprite::create("images/make/bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 320, 480);
    this->addToBGLayer(m_pBG);
    
    ui_button = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("cocos_studio/choose_flavors.json");
    this->addToUILayer(ui_button);
    
    ui_next = ui::Helper::seekWidgetByName(ui_button, "btn_next");
    ui_next->setTag(100);
    ui_next->setVisible(false);
    ui_next ->addTouchEventListener(CC_CALLBACK_2(MakeBatterScene::touchEvent, this));

    m_pBoard = Sprite::create("images/batter/board_on.png");
    m_pBoard->setPosition(Vec2(384 - 1000,524));
    m_pBG->addChild(m_pBoard);
    
    //根据选择界面，获取需要的水果,赋值到数组
    m_iFriutNum = (int)g_vChooseTaste.size();
    
    for (int i = 0; i<m_iFriutNum; i++) {
        
        makeBatter_fruit[i] = g_vChooseTaste.at(i);
        
        
    }
    if(m_iFriutNum>1){
        
        m_sMixStr = "mix";
        
    }else{
        
        m_sMixStr = makeBatter_fruit[0];
    }
    
    m_pBatter = Sprite::create(__String::createWithFormat("images/batter/%s/batter0.png",m_sMixStr.c_str())->getCString());
    m_pBatter->setPosition(Vec2(384 - 1000,524));
    m_pBG->addChild(m_pBatter);;
    
   
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MakeBatterScene::onTouchBegan,this);
    listener->onTouchMoved = CC_CALLBACK_2(MakeBatterScene::onTouchMoved,this);
    listener->onTouchEnded = CC_CALLBACK_2(MakeBatterScene::onTouchEnded,this);
    listener->setSwallowTouches(true);//不向下传递触摸
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    
    
    auto func = CallFunc::create([=](){
    
        showHand();
        
    });
    
    m_pBoard->runAction(Sequence::create(MoveBy::create(0.5, Vec2(1000,0)),func,NULL));
    m_pBatter->runAction(MoveBy::create(0.5, Vec2(1000,0)));
}
#pragma mark - 屏幕点击
bool MakeBatterScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    if (!m_pTouch) {
        return false;
    }
  
    Vec2 _pos = touch->getLocation();
    Vec2 pos = m_pBG->convertToNodeSpace(_pos);
    if (m_pBatter->getBoundingBox().containsPoint(pos) && m_bKnife) {
        
        log("touch");
        
        if (m_bIsAction) {
            m_bIsAction = false;
            ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
            ScaleTo* s2 = ScaleTo::create(0.2f,0.90,1.08f);
            ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
            m_pBatter->runAction(Sequence::create(s1,s2,s3,DelayTime::create(0.5),CallFunc::create([=](){
                
                m_bIsAction = true;
                
            }),nullptr));
            
        }
        
        

        
        return true;
    }
    
    Vec2 __pos = m_pBG->convertToWorldSpace(m_pBoard->getPosition());
    
    Rect rect = Rect(__pos.x - 25,__pos.y - 25,70,50);
    
    if (!m_bKnife && rect.containsPoint(_pos)) {
        
        log("cut touch");
        if (m_pBG->getChildByTag(m_iBallTag)) {
            
            if (m_bIsAction) {
                m_bIsAction=false;
                
                ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
                ScaleTo* s2 = ScaleTo::create(0.2f,0.90,1.08f);
                ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
                
                m_pBG->getChildByTag(m_iBallTag)->runAction(Sequence::create(s1,s2,s3,DelayTime::create(0.5),CallFunc::create([=](){
                    
                    m_bIsAction=true;
                    
                }),nullptr));
            }
        }
        return true;
    }
    return  false;
}
void MakeBatterScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    Vec2 posDelta = touch->getDelta();
    
    if (abs(posDelta.y) > 5 || abs(posDelta.x)>5) {
        
        log("move");
        m_iBatterMoveNum++;
        if (m_bKnife) {
            
            
            if(m_iBatterMoveNum%10 == 0 &&m_iBatterMoveNum>0){
                
                m_pBatter->setTexture(__String::createWithFormat("images/batter/%s/batter%d.png",m_sMixStr.c_str(),m_iBatterMoveNum/10)->getCString());
                
            }
            
            if (m_iBatterMoveNum>40) {
                m_iBatterMoveNum=-1000;
                //over
//                _eventDispatcher->removeAllEventListeners();
                m_pTouch=false;
                showKnife();
                
            }
            
            
            
        }else{
            
            if (m_iBatterMoveNum == 10 && m_iBatterMoveNum>0) {
                m_iBatterMoveNum=-1000;
                
                if (m_pBG->getChildByTag(m_iBallTag)) {
                    
                    dynamic_cast<Sprite*>(m_pBG->getChildByTag(m_iBallTag))->setTexture(__String::createWithFormat("images/select/one_%s.png",m_sMixStr.c_str())->getCString());
                    m_pTouch=false;
                    m_bIsAction = false;
//                    addToGlass();
                    this->scheduleOnce(schedule_selector(MakeBatterScene::addToGlass), 1.0);
                }

                
            }
        }
    }
}
void MakeBatterScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    if (m_bKnife && m_bIsAction) {
        
        m_pBatter->stopAllActions();
        
    }else if(!m_bKnife && m_bIsAction){
    
        if (m_pBG->getChildByTag(m_iBallTag)) {
            
            m_pBG->getChildByTag(m_iBallTag)->stopAllActions();
        }
    }
}
void MakeBatterScene::showHand()
{
    auto hand = Sprite::create("images/make/point.png");
    hand->setPosition(m_pBoard->getPosition());
    m_pBG->addChild(hand,500);
    hand->setAnchorPoint(Vec2(0.09,0.8));

    MoveBy* move = MoveBy::create(0.5, Vec2(20,20));
     MoveBy* move1 = MoveBy::create(0.5, Vec2(20,20));
    hand->runAction(Sequence::create(move,move->reverse(),move1,move1->reverse(),RemoveSelf::create(),NULL));
}
void MakeBatterScene::addToGlass(float dt)
{
    if(m_pBG->getChildByTag(m_iBallTag)){
        
        Sprite* ball = dynamic_cast<Sprite*>(m_pBG->getChildByTag(m_iBallTag));
        
        int pos = rand()%50 + 10;
        ball->runAction(Sequence::create(MoveTo::create(1.5, m_pBG->convertToNodeSpace(m_pGlass->convertToWorldSpace(Vec2(128+pos,302)))), CallFunc::create([=](){
        
            m_pBG->reorderChild(m_pBG->getChildByTag(m_iBallTag), 20);
            auto body = PhysicsBody::createCircle(ball->getContentSize().width / 2 - 5);
              ball->setPhysicsBody(body);
            body->getShape(0)->setMass(100);
            for(auto tool:m_vToolCut){
                tool->cancelEvent(false);
            }
            m_bIsAction = true;
            m_pTouch=true;
            m_iBallNum++;
            if (m_iBallNum == 5) {
                
                auto chooseParticle = ParticleSystemQuad::create("particle/progressLost.plist");
                chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
                chooseParticle->setVisible(true);
                kAdapterScreen->setExactPosition(chooseParticle, 320, 480);
                chooseParticle->setDuration(1.0);
                this->addChild(chooseParticle,100);
                
                AudioHelp::getInstance()->playEffect("hurrah.mp3");
                
                ui_next->setVisible(true);
                
            }
            
            
        }),NULL));
    
    }
}

#pragma mark - cut batter
void MakeBatterScene::showKnife()
{
    
    m_pKnife = ToolSprite::create("images/cut/knife.png");
    m_pKnife->setDelegate(this);
    kAdapterScreen->setExactPosition(m_pKnife, 0 - 500, 480,Vec2(1,0.5),kBorderRight,kBorderNone);
    this->addToContentLayer(m_pKnife);
    m_pKnife->cancelEvent(true);
    m_pKnife->setResponsePoint(Vec2(0.26,0.52));
    m_pKnife->setTag(100);
    auto func = CallFunc::create([=](){
    
        showLine(m_iLineNum);
        
    });
    
    m_pKnife->runAction(Sequence::create(MoveBy::create(0.5,Vec2(-500,0)),func, NULL));
}
void MakeBatterScene::showLine(int num)
{
    auto line = Sprite::create("images/cut/prompt_dotted_line.png");
    line->setAnchorPoint(Vec2(0.83,0.18));
    line->setPosition(linePos[num]);
    m_pBatter->addChild(line);
        
    m_pKnife->addRectByNode(line);
   
    m_pKnife->cancelEvent(false);

}
void MakeBatterScene::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    oldPos = toolSprite->getPosition();
}
void MakeBatterScene::onTouchMove(ToolSprite* toolSprite,Touch *pTouch)
{
    
    
}
void MakeBatterScene::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    static bool isFirst = true;
    Vec2 posDetal = pTouch->getDelta();
    int tag = toolSprite->getTag();
    
    if (tag == 100) {
        if (posDetal.y>50) {
            
            toolSprite->clearRectVector();
            m_iLineNum++;
            m_pBatter->removeAllChildren();
            m_pBatter->setTexture(__String::createWithFormat("images/batter/%s/batter5_%d.png",m_sMixStr.c_str(),m_iLineNum)->getCString());
            if (m_iLineNum == 1) {
                m_pBatter->setPositionY(m_pBatter->getPositionY()-58);
            }
            if(m_iLineNum>3){
                
                //刀消失
                m_pKnife->cancelEvent(true);
                m_pKnife->runAction(MoveBy::create(0.5, Vec2(1000,0)));
                
                //案板移动，出现瓶子
                showGlass();
                //            showCut();
                
            }else{
                
                showLine(m_iLineNum);
                
            }
        }
    }else{
        
        for (auto tool : m_vToolCut) {
            
            tool->cancelEvent(true);
        }
        if (isFirst) {
            isFirst=false;
            showHand();
        }
        
        
        m_vToolCut.eraseObject(toolSprite);
        toolSprite->setVisible(false);
        
        m_iBallTag = toolSprite->getTag() + 200;
        auto cut = Sprite::createWithTexture(toolSprite->getTexture());
//        auto body = PhysicsBody::createCircle(cut->getContentSize().width / 2);
//        cut->setPhysicsBody(body);
        cut->setPosition(Vec2(m_pBoard->getPosition().x - 85,m_pBoard->getPosition().y - 25) + cut->getContentSize()/2 + Vec2(cut->getContentSize().width,0));
        m_pBG->addChild(cut,200);
        cut->setTag(m_iBallTag);
        
        m_iBatterMoveNum = 0;
        m_pTouch = true;
    
        m_bKnife = false;
    
    }
    
}
void MakeBatterScene::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    toolSprite->cancelEvent(true);
    toolSprite->runAction(Sequence::create(MoveTo::create(1.0, oldPos),CallFunc::create([=](){
    
        toolSprite->cancelEvent(false);
    
     }), NULL));
}
#pragma mark - make cut
void MakeBatterScene::showGlass()
{
    m_pGlass= Sprite::create("images/select/glass_bottle0.png");
    m_pGlass->setPosition(Vec2(599 + 1000,552 + 150));
    m_pGlass->setAnchorPoint(Vec2(0.48,0.06));
    m_pBG->addChild(m_pGlass);
    
//    auto body = PhysicsBody::createEdgeBox(m_pGlass->getContentSize() - Size(50,50), PHYSICSBODY_MATERIAL_DEFAULT, 3);//设置要添加到节点中的物理body
    auto borderUpper = PhysicsBody::createEdgePolygon(verts,40);
    m_pGlass->setPhysicsBody(borderUpper);
    
    Sprite* shadow = Sprite::create("images/select/glass_shadow.png");
    shadow->setAnchorPoint(Vec2::ZERO);
    m_pGlass->addChild(shadow,-1);
    
    
    Sprite* glassCover = Sprite::create("images/select/glass_bottle1.png");
//    glassCover->setAnchorPoint(Vec2::ZERO);
//    m_pGlass->addChild(glassCover,10);
    
    glassCover->setPosition(Vec2(599 + 1000,552));
    glassCover->setAnchorPoint(Vec2(0.48,0.06));
    m_pBG->addChild(glassCover,100);
    
    __String* str = __String::createWithFormat("images/select/%s.png",m_sMixStr.c_str());
    
    Sprite* waste = Sprite::create(str->getCString());
    waste->setPosition(Vec2(134,115));
    glassCover->addChild(waste,-1);
    
    auto func = CallFunc::create([=](){
        
        showCut();
        
    });
   
    
    m_pBoard->runAction(MoveTo::create(0.5, Vec2(363,414)));
    glassCover->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-1000,0)),nullptr));
    m_pGlass->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-1000,0)),DelayTime::create(0.5),func, NULL));
    m_pBatter->runAction(MoveTo::create(0.5, Vec2(363,414)));
}
void MakeBatterScene::showCut()
{
    m_pBatter->setVisible(false);
    for (int i = 1; i< 6; i++) {
        
        auto tool = ToolSprite::create(__String::createWithFormat("images/batter/%s/cut%d.png",m_sMixStr.c_str(),i)->getCString());
        tool->setPosition(m_pBG->convertToNodeSpace(m_pBatter->convertToWorldSpace(batterPos[i-1])) + Vec2(75,0));
        m_pBG->addChild(tool);
        tool->setDelegate(this);
        tool->setTag(i-1);
        m_vToolCut.pushBack(tool);
        
    }
    
    for (auto tools:m_vToolCut) {
        
        tools->runAction(Sequence::create(MoveTo::create(0.5, cutBatterMovePos[tools->getTag()]),CallFunc::create([=](){
            
            if (tools->getTag() == 4) {
                
                //出现手
//                m_pTouch=true;
                
                showHand1();
            }
            
            
        }),nullptr));
        
    }
    
}
void MakeBatterScene::showHand1()
{
    int toolNum = rand()%4;
    auto hand = Sprite::create("images/make/point.png");
    hand->setPosition(m_vToolCut.at(toolNum)->getPosition());
    m_pBG->addChild(hand);
    hand->setAnchorPoint(Vec2(0.09,0.8));
    
    hand->runAction(Sequence::create(MoveTo::create(0.5, m_pBoard->getPosition()),RemoveSelf::create(),nullptr));
    
  
    m_vToolCut.at(toolNum)->runAction(JumpBy::create(1.0, Vec2(0,0), 30, 2));
    for(auto tool:m_vToolCut){
        
        Vec2 pos = m_pBG->convertToWorldSpace(m_pBoard->getPosition());
        
        Rect rect = Rect(pos.x - 25,pos.y - 25,70,50);
        tool->addRect(rect);
    }
}
void MakeBatterScene::touchEvent(Ref *obj , Widget::TouchEventType type)
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

void MakeBatterScene::nextStep()
{
    log("next");
    hideNextButton();
    
}