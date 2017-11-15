#include "MenuScene.h"
#include "Definitions.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MenuScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto playButton = MenuItemFont::create( "Play", CC_CALLBACK_1( MenuScene::goToGameScene, this ) );
    playButton->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height/2));
    playButton->setFontName("space_invaders.ttf");
    
    auto *menu = Menu::create(playButton, NULL);
    menu->setPosition(Point(0,0));
    this->addChild(menu);
    
    return true;
}

void MenuScene::goToGameScene(Ref *sender)
{
    CCLOG("Play");
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}