//
//  CutFruitScene.cpp
//  KFOOD1105_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/9.
//
//

#include "CutFruitScene.h"

string fruit_4_name[5]={"apple","kiwi_fruit","melon","pineapple","sour_apple"};

Vec2 fruit_4_pos[5][3]={{Vec2(115,144),Vec2(153,154),Vec2(184,157)},{Vec2(64,144),Vec2(126,143),Vec2(179,146)},{Vec2(90,96),Vec2(140,120),Vec2(197,129)},{Vec2(135,173),Vec2(237,148),Vec2(170,196)},{Vec2(115,144),Vec2(153,154),Vec2(184,157)}};

string fruit_3_name[10]={"lemon","lime","orange","pear","banana","blackcurrant","cinnamon","grape","mint_leaf","watermelon"};
Vec2 fruit_3_pos[10][2]={{Vec2(204,131),Vec2(111,117)},{Vec2(84,145),Vec2(179,140)},{Vec2(115,150),Vec2(199,158)},{Vec2(70,122),Vec2(139,121)},{Vec2(107,42),Vec2(273,42)},{Vec2(76,37),Vec2(267,86)},{Vec2(128,24),Vec2(267,24)},{Vec2(46,47),Vec2(153,44)},{Vec2(124,79),Vec2(302,99)},{Vec2(94,66),Vec2(191,90)}};

string fruit_2_name[5]={"vanilla","strawberry","raspberry","peach","fig"};
Vec2 fruit_2_pos[5]={Vec2(161,57),Vec2(83,96),Vec2(158,63),Vec2(117,80),Vec2(146,103)};

string fruit_1_name="chocolate";
Vec2 fruit_1_pos = Vec2(145,114);

Vec2 fruit_pos[3]={Vec2::ZERO,Vec2::ZERO,Vec2::ZERO};


CutFruitScene::CutFruitScene():m_iFriutNum(0)
{
    
}
CutFruitScene::~CutFruitScene()
{
    
}
bool CutFruitScene::init()
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
#pragma mark - Enter or Exit
void CutFruitScene::onEnter()
{
    GameBaseScene::onEnter();
}
void CutFruitScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
}
void CutFruitScene::onExit()
{
    GameBaseScene::onExit();
}
#pragma mark - initData
void CutFruitScene::initUI()
{
    m_pBG = Sprite::create("images/cut/bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 320, 480);
    this->addToBGLayer(m_pBG);
    
   
    //界面按钮
    ui_button = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("cocos_studio/cut_fruit.json");
    this->addToUILayer(ui_button);
    
    ui_next = ui::Helper::seekWidgetByName(ui_button, "btn_next");
    ui_next -> setTag(eTagNext);
    ui_next ->addTouchEventListener(CC_CALLBACK_2(CutFruitScene::touchEvent, this));
    
    ui_icon = ui::Helper::seekWidgetByName(ui_button, "btn_choose_tool");
    ui_icon -> setTag(eTagIcon);
    ui_icon ->addTouchEventListener(CC_CALLBACK_2(CutFruitScene::touchEvent, this));
    
    
    //弹出框选择按钮
    m_pChooseToolsLayer = LayerColor::create(Color4B(0,0,0,180));
    m_pChooseToolsLayer->setAnchorPoint(Vec2::ZERO);
    this->addToUILayer(m_pChooseToolsLayer,10);
    ui_choose_button = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("cocos_studio/popup_choose_tools.json");
    m_pChooseToolsLayer->addChild(ui_choose_button);
    
    ui_knife   = ui::Helper::seekWidgetByName(ui_choose_button, "btn_free");
    ui_knife   -> setTag(eTagKnife);
    ui_knife   ->addTouchEventListener(CC_CALLBACK_2(CutFruitScene::touchEvent, this));
    
    ui_machine = ui::Helper::seekWidgetByName(ui_choose_button, "btn_buy");
    ui_machine -> setTag(eTagMachine);
    ui_machine ->addTouchEventListener(CC_CALLBACK_2(CutFruitScene::touchEvent, this));
    
    //定义一个操作的Layer，所有操作类加载到这个layer
    m_pOperateLayer = Layer::create();
    m_pOperateLayer->setAnchorPoint(Vec2::ZERO);
    this->addToContentLayer(m_pOperateLayer);
    
    
    m_iFriutNum = (int)g_vChooseTaste.size();
    
    
}
//选择刀具
void CutFruitScene::chooseKnife(int fruitNum)
{
    
    checkFriute(g_vChooseTaste.at(fruitNum).c_str());
    
    auto board = Sprite::create("images/cut/board_on.png");
    board->setPosition(kAdapterScreen->getExactPostion(Vec2(354.5,346)));
    m_pOperateLayer->addChild(board);
    
    auto fruit = Sprite::create(__String::createWithFormat("images/cut/%s.png",g_vChooseTaste.at(fruitNum).c_str())->getCString());
    fruit->setPosition(kAdapterScreen->getExactPostion(Vec2(115,598)));
    m_pOperateLayer->addChild(fruit);
    fruit->setPositionX(fruit->getPositionX() - 500);
    
    auto _friute = ToolSprite::create(__String::createWithFormat("images/cut/%s.png",g_vChooseTaste.at(fruitNum).c_str())->getCString());
    _friute->setDelegate(this);
    _friute->setIsMove(false);
    _friute->setPosition(kAdapterScreen->getExactPostion(Vec2(115 + 50 + 70,598)));
    m_pOperateLayer->addChild(_friute);
    
    _friute->addRectByNode(board);
    _friute->setPositionX(_friute->getPositionX() - 500);
    
    board->setPositionX(board->getPositionX() + 500);
    
    fruit->runAction(MoveBy::create(0.5, Vec2(500,0)));
    _friute->runAction(MoveBy::create(0.5, Vec2(500,0)));
    
    board->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-500,0)),CallFunc::create([=](){
    
        if (fruitNum == 0) {
            
            _friute->setIsMove(true);
            
        }
    
    }), NULL));
    
    
    
}
//选择机器
void CutFruitScene::chooseMac(int fruit)
{
    
}
void CutFruitScene::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    oldPos = toolSprite->getPosition();
    int tag = toolSprite->getTag();
    switch (tag) {
        case eTagKnife:
        {
            
        }
           break;
            
        default:
            break;
    }
    
    
    
    
}
void CutFruitScene::onTouchMove(ToolSprite* toolSprite,Touch *pTouch)
{
    
}
void CutFruitScene::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    
}
void CutFruitScene::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    
    toolSprite->cancelEvent(true);
    toolSprite->runAction(Sequence::create(MoveTo::create(0.5, oldPos),CallFunc::create([=](){
    
        toolSprite->cancelEvent(false);
    
    }), NULL));

}
void CutFruitScene::checkFriute(string furite)
{
    if (checkFriute4(furite)!=-1) {
        
        int num = checkFriute4(furite);
        
        
        fruit_pos[0]= fruit_4_pos[num][0];
        fruit_pos[1]= fruit_4_pos[num][1];
        fruit_pos[2]= fruit_4_pos[num][2];
        
    }else if(checkFriute3(furite)!=-1){
    
        int num = checkFriute3(furite);
        
        fruit_pos[0]= fruit_3_pos[num][0];
        fruit_pos[1]= fruit_3_pos[num][1];
      
    
    }else if(checkFriute2(furite)!=-1){
        
        int num = checkFriute2(furite);
        
        fruit_pos[0]=fruit_2_pos[num];
      
        
    }else if(furite == "chocolate"){
    
        fruit_pos[0]=fruit_1_pos;
        
    }
   
}
int CutFruitScene::checkFriute4(string furite)
{
    for (int i=0;i<5;i++){
        
        if (furite == fruit_4_name[5]) {
            return i;
        }
        
        
    }
    
    return -1;
}
int CutFruitScene::checkFriute3(string furite)
{
    for (int i=0;i<10;i++){
        
        if (furite == fruit_3_name[10]) {
            return i;
        }
        
        
    }

    return -1;
}
int CutFruitScene::checkFriute2(string furite)
{
    for (int i=0;i<5;i++){
        
        if (furite == fruit_2_name[5]) {
            return i;
        }
        
        
    }
    return -1;
}
void CutFruitScene::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                    
                case eTagIcon:
                {
                    m_pChooseToolsLayer->setVisible(true);
                }
                    break;
                case eTagNext:
                {
                    
                }
                    break;
                case eTagKnife:
                {
                    m_pChooseToolsLayer->setVisible(false);
                    m_pOperateLayer->removeAllChildren();
                    chooseKnife(0);
                }
                    break;
                case eTagMachine:
                {
                    m_pChooseToolsLayer->setVisible(false);
                }
                    break;
            }
        }
    }

}
void CutFruitScene::nextStep()
{
    log("next");
    hideNextButton();
    
}