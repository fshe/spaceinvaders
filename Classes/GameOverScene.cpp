#include "MenuScene.h"
#include "Definitions.h"
#include "GameOverScene.h"
#include "GameScene.h"

USING_NS_CC;

int gamescore;
Scene* GameOverScene::createScene(int score)
{
    gamescore = score;
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOverScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto restartButton = MenuItemFont::create( "Restart", CC_CALLBACK_1( GameOverScene::goToMenuScene, this ) );
    restartButton->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height/2));
    restartButton->setFontName("space_invaders.ttf");
    auto *menu = Menu::create(restartButton, NULL);
    menu->setPosition(Point(0,0));
    this->addChild(menu);
    
    __String *tempScore = __String::createWithFormat("SCORE: %i", gamescore);
    auto scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/space_invaders.ttf", visibleSize.height * SCORE_FONT_SIZE);
    scoreLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height/2 - 50));
    this->addChild(scoreLabel, 1000);
    
    return true;
}

void GameOverScene::goToMenuScene(Ref *sender)
{
    CCLOG("Restart");
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}