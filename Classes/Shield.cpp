#include "Shield.h"
#include "Definitions.h"
USING_NS_CC;

Shield::Shield()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
}
Sprite** Shield::SpawnShields()
{
    CCLOG("SPAWN SHIELDS");
    Sprite** shields = new Sprite*[4];
    
    for(int i = 0; i < 4; i++)
    {
        Sprite* shield;
        shield = Sprite::create("Player/player_shield.png");
        shield->setTag(57+i); //shields have tags 57-60
        auto shieldBody = PhysicsBody::createBox(shield->getContentSize());
        shieldBody->setTag(57+i+100);
        shieldBody->setDynamic(false);
        shieldBody->setGravityEnable(false);
        
        shieldBody->setCollisionBitmask(SHIELD_COLLISION_BITMASK);
        shieldBody->setContactTestBitmask(true);
        shield->setPhysicsBody(shieldBody);
        shield->setPosition(Vec2(
                                  (visibleSize.width/5) * (i+1),
                                  visibleSize.height / 10 + origin.y + 30)
                             );
        
        shields[i] = shield;
    }
    return shields;
}