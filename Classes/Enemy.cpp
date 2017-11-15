#include "Enemy.h"
#include "Definitions.h"
USING_NS_CC;

Enemy::Enemy()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

}
Sprite** Enemy::SpawnEnemies()//Layer *layer)
{
    CCLOG("SPAWN ENEMIES");
    Sprite** enemies = new Sprite*[55];
    
    for(int i = 0; i < 55; i++)
    {
        Sprite* invader;
        if (i < 22)
            invader = Sprite::create("Enemies/invaderAframe1.png");
        else if (i < 44)
            invader = Sprite::create("Enemies/invaderBframe1.png");
        else if (i < 55)
            invader = Sprite::create("Enemies/invaderCframe1.png");
        invader->setTag(i);
        auto invaderBody = PhysicsBody::createBox(invader->getContentSize());
        invaderBody->setTag(i+100);
        invaderBody->setDynamic(true);
        invaderBody->setGravityEnable(false);

        invaderBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
        invaderBody->setContactTestBitmask(true);
        invader->setPhysicsBody(invaderBody);
        int j = i;
        while(j >= 11)
        {
            j-=11;
        }
        
        invader->setPosition(Vec2(
                            (visibleSize.width/110) * j * 8 + 55,
                            (visibleSize.height/40) * ((i/11)+1)*3 + 160)
                            );
        
        enemies[i] = invader;
    }
    return enemies;
}