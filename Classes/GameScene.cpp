#include "Definitions.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include <limits>

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    layer->SetPhysicsWorld( scene->getPhysicsWorld());
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    //for collision
    previousCollision = -1;
    
    //movement direction (initially negative x direction)
    movement = -1;
    
    //frame change
    frame1 = true;
    frameSwitch = 0;
    
    //for enemyMissiles
    missileNum = std::numeric_limits<int>::max() - 100;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    m_visibleSize = visibleSize;
    m_layer = this;
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
    //make the Player Ship
    player = new Player(this);
    
    //for Collision detection
    auto edgeBody = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edgeBody->setCollisionBitmask(EDGE_COLLISION_BITMASK);
    edgeBody->setContactTestBitmask(true);
    
    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    edgeNode->setPhysicsBody(edgeBody);
    this->addChild(edgeNode);
    
    //Spawn the enemies
    enemies = SpawnEnemies();
    for(int i =0 ; i < 55; i++)
    {
        this->addChild(enemies[i], 999);
        enemyBitMap[i] = true; //true indicates there is an enemy
    }
    
    //Spawn the Shields
    shields = SpawnShields();
    for(int i = 0; i < 4; i++)
    {
        this->addChild(shields[i], 999);
    }
    
    //Contact Listener for Collisions
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    //Touch Listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    //Acceleration Listener
    Device::setAccelerometerEnabled(true);
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(GameScene::OnAcceleration, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
    //score
    score = 0;
    __String *tempScore = __String::createWithFormat("SCORE: %i", score);
    scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/space_invaders.ttf", visibleSize.height * SCORE_FONT_SIZE);
    scoreLabel->setPosition(Vec2(scoreLabel->getContentSize().width/2 + 10, visibleSize.height));
    this->addChild(scoreLabel, 1000);
    
    this->schedule(schedule_selector(GameScene::update), 0.03);
    this->schedule(schedule_selector(GameScene::enemyShoot), 3);

    return true;
}

Sprite** GameScene::SpawnEnemies()
{
    Enemy* enemy = new Enemy();
    return enemy->SpawnEnemies();
}

Sprite** GameScene::SpawnShields()
{
    Shield* shield = new Shield();
    return shield->SpawnShields();
}
bool GameScene::onContactBegin( cocos2d::PhysicsContact &contact )
{
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    
    if ( ( PLAYER_COLLISION_BITMASK == a->getCollisionBitmask( ) && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask() ) || ( PLAYER_COLLISION_BITMASK == b->getCollisionBitmask( ) && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask() )
        ||
        ( PLAYER_COLLISION_BITMASK == a->getCollisionBitmask( ) && ENEMYWEAPON_COLLISION_BITMASK == b->getCollisionBitmask() ) || ( PLAYER_COLLISION_BITMASK == b->getCollisionBitmask( ) && ENEMYWEAPON_COLLISION_BITMASK == a->getCollisionBitmask() )
        )
    {
        //GAME OVER
        CCLOG("GAME OVER");
        GameOverScene *placeholder = new GameOverScene();
        auto scene = placeholder->createScene(score);
        Director::getInstance()->replaceScene(scene);
    }
    else if ( (OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask() && PLAYERMISSILE_COLLISION_BITMASK == b->getCollisionBitmask()) || ( PLAYERMISSILE_COLLISION_BITMASK == a->getCollisionBitmask( ) && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask() ) )
    {
        CCLOG("ENEMY DESTROYED BY PLAYER MISSILE");
        int tagNum = a->getTag();
        int tagNum2 = b->getTag();
        removeChildByTag(tagNum);
        removeChildByTag(tagNum2);

        if(tagNum < 155)
        {
            removeChildByTag(tagNum2-100);
           ((Sprite*) getChildByTag(tagNum-100))->setTexture("Enemies/invader_explosion.png");
            enemyBitMap[tagNum-100] = false;
            toDelete.push_back(tagNum-100);
        }
        else
        {
            removeChildByTag(tagNum-100);
            ((Sprite*) getChildByTag(tagNum2-100))->setTexture("Enemies/invader_explosion.png");
            enemyBitMap[tagNum2-100] = false;
            toDelete.push_back(tagNum2-100);
        }

        
        score += 10;
        if(score == 550)
        {
            CCLOG("GAME COMPLETE");
            GameOverScene *placeholder = new GameOverScene();
            auto scene = placeholder->createScene(score);
            Director::getInstance()->replaceScene(scene);
        }
         __String *tempScore = __String::createWithFormat("SCORE: %i", score);
        scoreLabel->setString(tempScore->getCString());
    } //end PLAYERMISSILE with ENEMY
    else if ( (OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask() && EDGE_COLLISION_BITMASK == b->getCollisionBitmask()) || ( EDGE_COLLISION_BITMASK == a->getCollisionBitmask( ) && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask() ) )
    {
        
        if (OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask())
        {
            if(!isMultipleEleven(previousCollision, a->getTag()-100))
            {
                previousCollision = a->getTag()-100;
                movement*=-1;
                for(int i = 0; i < 55; i++)
                    if(enemyBitMap[i])
                        enemies[i]->setPosition(Vec2(enemies[i]->getPositionX(), enemies[i]->getPositionY() - 5));
            }
        }
        else
        {
            if(!isMultipleEleven(previousCollision, b->getTag()-100))
            {
                previousCollision = b->getTag()-100;
                movement*=-1;
                for(int i = 0; i < 55; i++)
                    if(enemyBitMap[i])
                        enemies[i]->setPosition(Vec2(enemies[i]->getPositionX(), enemies[i]->getPositionY() - 5));
            }
        }
    } //end EDGE_COLLISION_BITMASK with OBSTACLE_COLLISION_BITMASK
    else if (SHIELD_COLLISION_BITMASK == a->getCollisionBitmask() || SHIELD_COLLISION_BITMASK == b->getCollisionBitmask())
    {
     //destroy the shield and whatever collided with it
        CCLOG("SHIELD COLLISION");
        int tagNum = a->getTag();
        int tagNum2 = b->getTag();
        
        //deal with enemy collision
        if (tagNum < 155)
            enemyBitMap[tagNum-100] = false;
        if (tagNum2 < 155)
            enemyBitMap[tagNum2-100] = false;
        
        removeChildByTag(tagNum);
        removeChildByTag(tagNum-100);
        removeChildByTag(tagNum2);
        removeChildByTag(tagNum2-100);
        
    } //end SHIELD_COLLISION_BITMASK
    else if ( (ENEMYWEAPON_COLLISION_BITMASK == a->getCollisionBitmask() && PLAYERMISSILE_COLLISION_BITMASK == b->getCollisionBitmask()) || ( PLAYERMISSILE_COLLISION_BITMASK == a->getCollisionBitmask( ) && ENEMYWEAPON_COLLISION_BITMASK == b->getCollisionBitmask() ) )
    {
        //just remove both missiles
        int tagNum = a->getTag();
        int tagNum2 = b->getTag();
        removeChildByTag(tagNum);
        removeChildByTag(tagNum-100);
        removeChildByTag(tagNum2);
        removeChildByTag(tagNum2-100);
    }
    else if ( (PLAYER_COLLISION_BITMASK == a->getCollisionBitmask() && EDGE_COLLISION_BITMASK == b->getCollisionBitmask()) || ( PLAYER_COLLISION_BITMASK == b->getCollisionBitmask( ) && EDGE_COLLISION_BITMASK == a->getCollisionBitmask() ) )
    {
        CCLOG("EDGE COLLISION");
        player->setCanMove();
        
    }
    return true;
}

void GameScene::OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event)
{
    if (acc->x > 0)
    {
        player->move(1);
    }
    else if (acc->x < 0)
    {
        player->move(-1);
    }
}

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
    player->Shoot(this);
    return true;
}

void GameScene::update( float dt )
{
    while (!toDelete.empty())
    {
        removeChildByTag(toDelete[0]);
        toDelete.erase(toDelete.begin());
    }
    frameSwitch++;
    for(int i = 0; i<55; i++)
    {
        if(enemyBitMap[i])
        {
            enemies[i]->setPosition(enemies[i]->getPosition().x + movement, enemies[i]->getPosition().y);
            
            if(frame1 && frameSwitch == 5)
            {
                if(i < 22)
                    enemies[i]->setTexture("Enemies/invaderAframe2.png");
                else if(i < 44)
                    enemies[i]->setTexture("Enemies/invaderBframe2.png");
                else if(i < 55)
                    enemies[i]->setTexture("Enemies/invaderCframe2.png");
            }
            else if (!frame1 && frameSwitch == 5)
            {
                if(i < 22)
                    enemies[i]->setTexture("Enemies/invaderAframe1.png");
                else if(i < 44)
                    enemies[i]->setTexture("Enemies/invaderBframe1.png");
                else if(i < 55)
                    enemies[i]->setTexture("Enemies/invaderCframe1.png");
            }
        }
    }
    if(frame1)
        frame1 = false;
    else
        frame1 = true;
    if(frameSwitch == 5)
        frameSwitch = 0;
 }

//check if one can reach two by adding/subtracting 11
bool GameScene::isMultipleEleven(int one, int two)
{
    if(one == two)
        return true;
    if (one > two)
    {
        while(one>two)
        {
            one-=11;
            if(one ==two)
                return true;
        }
        return false;
    }
    else
    {
        while(one < two)
        {
            two-=11;
            if(one==two)
                return true;
        }
        return false;
    }
}

void GameScene::enemyShoot(float dt)
{
    int shooter = cocos2d::RandomHelper::random_int(0,10);
    int original = shooter;
    int i = 1;
    while(shooter != 100)
    {
        if (shooter!=original)
        {
            i++;
            original == 10 ? shooter = 0 : shooter = original + i;
        }
        while(shooter < 55)
        {
            if(enemyBitMap[shooter] == true)
            {
                CCLOG("ENEMY SHOOT");
                //adapt code
                //make a sprite  that moves up and has a collision detector
                auto missile = Sprite::create("Enemies/enemy_missle.png");
                auto missileBody = PhysicsBody::createBox(missile->getContentSize());
                missileBody->setDynamic(true);
                missileBody->setCollisionBitmask(ENEMYWEAPON_COLLISION_BITMASK);
                missileBody->setContactTestBitmask(true);
                missile->setPhysicsBody(missileBody);
                missileBody->setGravityEnable(false);

                missileBody->setTag(100+missileNum);
                missile->setTag(missileNum);
                missileNum--;
                
                missile->setPosition(Vec2(enemies[shooter]->getPositionX(), enemies[shooter]->getPositionY() - enemies[shooter]->getContentSize().height));
                m_layer->addChild(missile, 999);
                
                auto missileAction = MoveBy::create(m_visibleSize.width/GRID_SIZE_Y, Vec2(0, m_visibleSize.width*-1.5));
                missile->runAction(missileAction);
                shooter = 100;
            }
            else
                shooter += 11;
        }
    }
}


void GameScene::setShoot(float dt)
{
    player->setShoot();
}