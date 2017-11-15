#ifndef GameScene_h
#define GameScene_h
#include "cocos2d.h"
#include "Enemy.h"
#include "Player.h"
#include "Shield.h"
#include <vector> 

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    void setShoot( float dt);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
private:
    cocos2d::Size m_visibleSize;
    cocos2d::Layer* m_layer;
    cocos2d::PhysicsWorld *sceneWorld;
    Player *player;
    cocos2d::Label *scoreLabel;
    cocos2d::Sprite** enemies; //Should always be 55 (5 rows of 11)
    cocos2d::Sprite** shields;
    
    void SetPhysicsWorld(cocos2d::PhysicsWorld *world) { sceneWorld = world; }
    cocos2d::Sprite** SpawnEnemies();
    cocos2d::Sprite** SpawnShields();

    bool onContactBegin( cocos2d::PhysicsContact &contact);
    void OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void update( float dt);
    bool isMultipleEleven(int one, int two);
    void enemyShoot(float dt);
    
    int score;
    bool enemyBitMap[55];
    bool frame1;
    int frameSwitch; //will switch sprite appearance when it reaches 10
    int movement;
    int previousCollision;
    std::vector<int> toDelete; //contains sprites to be deleted upon update()
    int missileNum;
    
};

#endif /* GameScene_h */
