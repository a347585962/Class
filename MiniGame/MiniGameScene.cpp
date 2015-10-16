//
//  MiniGameScene.cpp
//  CocosGame
//
//  Created by zhangdingquan on 15/10/15.
//
//

#include "MiniGameScene.h"

#define DIZZY_RUN_TAG     1101
#define DIZZY_LOCKED_DATA 1201
#define UNLOCK_DIZZY_TAG  1301

MiniGameScene::MiniGameScene() :
m_bWarheadRunning(false),
m_bWarheadReady(false),
m_iDizzyCount(5),
m_fDizzyRunTime(6.0f),
m_iWarHead(0){
    
}

MiniGameScene::~MiniGameScene() {
    _eventDispatcher->removeEventListener(m_pSightListner);
    unscheduleUpdate();
    if (isScheduled(schedule_selector(MiniGameScene::startAction))) {
        unschedule(schedule_selector(MiniGameScene::startAction));
    }
}

Scene *MiniGameScene::scene() {
    Scene* scene = Scene::create();
    MiniGameScene* layer = MiniGameScene::create();
    scene->addChild(layer);
    return scene;
}

bool MiniGameScene::init() {
    if (!Layer::init()) {
        return false;
    }
    m_siVisibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite* bgSp = Sprite::create("miniGame/bg.jpg");
    kAdapterScreen->setExactPosition(bgSp, 320, 480);
    this->addChild(bgSp, 0);
    
    Sprite* color2 = Sprite::create("miniGame/color2.png");
    kAdapterScreen->setExactPosition(color2, 250, 480);
    this->addChild(color2, 0);
    color2->runAction(JumpTo::create(5, color2->getPosition(), 200, 5));
    
    Sprite* color4 = Sprite::create("miniGame/color4.png");
    kAdapterScreen->setExactPosition(color4, 500, 480);
    this->addChild(color4, 0);
    
    color4->runAction(Sequence::create(JumpTo::create(5, color4->getPosition(), 300, 5), CallFunc::create([=]() {
        initDizzyPos();
        makeDizzySp(m_iDizzyCount);
    }), DelayTime::create(0.5f), CallFunc::create([=]() {
        color2->setTexture("miniGame/color1.png");
        color4->setTexture("miniGame/color3.png");
    }), DelayTime::create(0.5f), CallFunc::create([=]() {
        color2->runAction(Sequence::create(MoveTo::create(2, Vec2(m_siVisibleSize.width/2, 100)), FadeOut::create(0.5f), nullptr));
        color4->runAction(Sequence::create(MoveTo::create(2, Vec2(m_siVisibleSize.width/2, 100)), FadeOut::create(0.5f), CallFunc::create([=]() {
            initSightSp();
            initWarheadSp();
            this->scheduleUpdate();
        }), nullptr));
    }), nullptr));
    
    return true;
}

void MiniGameScene::initSightSp() {
    m_pSightSp = Sprite::create("miniGame/sight.png");
    kAdapterScreen->setExactPosition(m_pSightSp, 320, 480);
    this->addChild(m_pSightSp, 20);
    
    m_pSightListner = EventListenerTouchOneByOne::create();
    m_pSightListner->setSwallowTouches(true);
    m_pSightListner->onTouchBegan = [=](Touch* pTouch, Event* pEvent) {
        Vec2 touchPos = pTouch->getLocation();
        Vec2 realPos = m_pSightSp->getParent()->convertToNodeSpace(touchPos);
        if (m_pSightSp->getBoundingBox().containsPoint(realPos)) {
            if (m_bWarheadReady && !m_bWarheadRunning) {
                this->stopActionByTag(UNLOCK_DIZZY_TAG);
                m_pSightSp->setTexture("miniGame/sight.png");
                m_pSightSp->stopAllActions();
                
                warheadRun();
                return false;
            }
            return true;
        }
        return false;
    };
    m_pSightListner->onTouchMoved = [=](Touch* pTouch, Event* pEvent) {
        if (m_bWarheadReady) {
            return;
        }
        Vec2 prePos = pTouch->getPreviousLocation();
        m_pSightSp->setPosition(prePos);
        
        if (!m_bWarheadRunning) {
            float theRadians = (m_pWarheadSp->getPosition() - m_pSightSp->getPosition()).getAngle();
            float rotation = CC_RADIANS_TO_DEGREES(theRadians);
            m_pWarheadSp->setRotation(270 - rotation);
        }
        
        if (m_vDizzySp.size() != 0) {
            for (int i = 0; i < m_vDizzySp.size(); i++) {
                Sprite* dizzy = m_vDizzySp.at(i);
                if (dizzy->getBoundingBox().containsPoint(m_pSightSp->getPosition()) && !m_bWarheadRunning && !m_bWarheadReady) {
                    m_bWarheadReady = true;
                    dizzy->stopActionByTag(DIZZY_RUN_TAG);
                    dizzy->setUserData((void*)DIZZY_LOCKED_DATA);
                    m_pSightSp->setPosition(dizzy->getPosition());
                    m_pSightSp->setTexture("miniGame/sight_lock.png");
                    m_pSightSp->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.2f), ScaleTo::create(0.5f, 1), nullptr)));
                    Sequence* seq = Sequence::create(DelayTime::create(5.5f), CallFunc::create([=]() {
                        if (!dizzy->isVisible()) {
                            dizzy->removeFromParent();
                        } else {
                            dizzy->setUserData(nullptr);
                        }
                        m_pSightSp->setTexture("miniGame/sight.png");
                        m_pSightSp->stopAllActions();
                        m_bWarheadReady = false;
                    }), nullptr);
                    seq->setTag(UNLOCK_DIZZY_TAG);
                    this->runAction(seq);
                }
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pSightListner, m_pSightSp);
}

void MiniGameScene::initWarheadSp() {
    m_pWarheadSp = Sprite::create("miniGame/warhead1.png");
    m_pWarheadSp->setPosition(Vec2(m_siVisibleSize.width/2, 100));
    this->addChild(m_pWarheadSp, 10);
}

void MiniGameScene::initDizzyPos() {
    for (int i = 0; i < 30; i++) {
        m_vDizzyPos.push_back(Vec2(rand()%(int)(m_siVisibleSize.width - 120) + 60, rand()%(int)(m_siVisibleSize.height - 100) + 50));
    }
}

void MiniGameScene::makeDizzySp(int count) {
    m_vDizzySp.clear();
    for (int i = 0; i < count; i++) {
        int dizzyType = rand()%2 + 1;
        Sprite* dizzy = Sprite::create(StringUtils::format("miniGame/dizzy%d.png", dizzyType));
        if (i < 3) {
            dizzy->setPosition(Vec2(-100, m_siVisibleSize.height/(count + 1)*i + 150));
        } else {
            dizzy->setPosition(Vec2(m_siVisibleSize.width + 100, m_siVisibleSize.height/(count + 1)*i + 150));
        }
        dizzy->runAction(MoveTo::create(3.2f, m_vDizzyPos.at(rand()%30)));
        this->addChild(dizzy);
        dizzy->setTag(dizzyType);
        m_vDizzySp.pushBack(dizzy);
        
        Animation* animation = Animation::create();
        for (int i = 1; i <= 4; i++) {
            animation->addSpriteFrameWithFile(StringUtils::format("miniGame/%d/sugar_%d.png", dizzyType, i));
        }
        animation->setDelayPerUnit(0.1f);
        animation->setLoops(-1);
        Animate* animate = Animate::create(animation);
        dizzy->runAction(animate);
    }
    m_fDizzyRunTime -= 0.5;
    schedule(schedule_selector(MiniGameScene::startAction), m_fDizzyRunTime);
}

void MiniGameScene::startAction(float dt) {
    if (m_vDizzySp.size() == 0) {
        return;
    }
    for (int i = 0; i < m_vDizzySp.size(); i++) {
        Sprite* dizzy = m_vDizzySp.at(i);
        Sequence* seq = Sequence::create(MoveTo::create(dt, m_vDizzyPos.at(rand()%30)), DelayTime::create(rand()%10*0.1f), nullptr);
        seq->setTag(DIZZY_RUN_TAG);
        if ((size_t)dizzy->getUserData() != DIZZY_LOCKED_DATA) {
            dizzy->runAction(seq);
        }
    }
}

void MiniGameScene::warheadRun() {
    m_bWarheadRunning = true;
    
    float distance = sqrt((m_pSightSp->getPositionX() - m_pWarheadSp->getPositionX())*(m_pSightSp->getPositionX() - m_pWarheadSp->getPositionX()) +
                          (m_pSightSp->getPositionY() - m_pWarheadSp->getPositionY())*(m_pSightSp->getPositionY() - m_pWarheadSp->getPositionY()));
    float time = distance / 900;
    
    m_pWarheadSp->runAction(Sequence::create(MoveTo::create(time, m_pSightSp->getPosition()), FadeOut::create(0.01f), CallFunc::create([=]() {
        m_pWarheadSp->setPosition(Vec2(m_siVisibleSize.width/2, 100));
        m_pWarheadSp->runAction(FadeIn::create(0.5f));
        m_iWarHead++;
        
        m_pWarheadSp->setTexture(__String::createWithFormat("miniGame/warhead%d.png",m_iWarHead%2+1)->getCString());
        m_bWarheadRunning = false;
        m_bWarheadReady = false;
    }), nullptr));
}

void MiniGameScene::update(float dt) {
    if (m_vDizzySp.size() != 0) {
        checkCollision();
    } else {
        if (m_iDizzyCount <= 9) {
            m_iDizzyCount++;
        }
        makeDizzySp(m_iDizzyCount);
    }
}

void MiniGameScene::checkCollision() {
    for (int i = 0; i < m_vDizzySp.size(); i++) {
        Sprite* dizzy = m_vDizzySp.at(i);
        if (dizzy->getBoundingBox().containsPoint(m_pWarheadSp->getPosition()) && (size_t)dizzy->getUserData() == DIZZY_LOCKED_DATA && m_bWarheadReady) {
            m_pWarheadSp->stopAllActions();
            m_pWarheadSp->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([=]() {
                m_pWarheadSp->setPosition(Vec2(m_siVisibleSize.width/2, 100));
                m_iWarHead++;
                
                m_pWarheadSp->setTexture(__String::createWithFormat("miniGame/warhead%d.png",m_iWarHead%2+1)->getCString());
                m_pWarheadSp->runAction(FadeIn::create(0.5f));
                m_bWarheadRunning = false;
                m_bWarheadReady = false;
            }), nullptr));
            
            dizzy->stopAllActions();
            dizzy->setTexture(StringUtils::format("miniGame/dizzy%d.png", dizzy->getTag()));
            m_vDizzySp.eraseObject(dizzy);
            dizzy->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([=]() {
                dizzy->setVisible(false);
            }), nullptr));
            
            break;
        }
    }
}

void MiniGameScene::onEnter() {
    Layer::onEnter();
}

void MiniGameScene::onEnterTransitionDidFinish() {
    
}

void MiniGameScene::onExit() {
    Layer::onExit();
}