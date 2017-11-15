#include "Player.h"
#include "Definitions.h"
#include "GameScene.h"
USING_NS_CC;

Player::Player(Layer *layer)
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    ship = Sprite::create("Player/spaceship.png");
    ship->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 10 + origin.y));
    direction = 0;
    canMove = true;
    isShooting = false;
    
    auto shipBody = PhysicsBody::createBox(ship->getContentSize());
    shipBody->setCollisionBitmask(PLAYER_COLLISION_BITMASK);
    shipBody->setContactTestBitmask(true);
    
    shipBody->setDynamic(true);
    shipBody->setGravityEnable(false);
    ship->setPhysicsBody(shipBody);
    m_missileNum = 56;
    layer->addChild(ship,100);
}

void Player::Shoot(Layer *layer)
{
    if (!isShooting)
    {
        CCLOG("MISSILE SHOT!");
        isShooting = true;
        layer->scheduleOnce(schedule_selector(GameScene::setShoot), 1);
        //make a sprite  that moves up and has a collision detector
        auto missile = Sprite::create("Player/player_missle.png");
        auto missileBody = PhysicsBody::createBox(missile->getContentSize());
        missileBody->setDynamic(true);
        missileBody->setGravityEnable(false);
        missileBody->setCollisionBitmask(PLAYERMISSILE_COLLISION_BITMASK);
        missileBody->setContactTestBitmask(true);
        missile->setPhysicsBody(missileBody);
    
        missileBody->setTag(100+m_missileNum);
        missile->setTag(m_missileNum);
        m_missileNum++;
    
        missile->setPosition(this->ship->getPositionX(), this->ship->getPositionY()+this->ship->getContentSize().height);
        layer->addChild(missile);
    
        auto missileAction = MoveBy::create(visibleSize.width/GRID_SIZE_Y, Vec2(0, visibleSize.width*1.5));
        missile->runAction(missileAction);
    }
}

void Player::setShoot()
{
    isShooting = false;
}

void Player::move(int move)
{
    if(canMove || direction != move)
    {
        canMove = true;
        direction = move;
        ship->setPosition(Vec2(ship->getPositionX()+move, ship->getPositionY()));
    }
}

int Player::getDirection()
{
    return direction;
}

cocos2d::Sprite* Player::getShip()
{
    return ship;
}
