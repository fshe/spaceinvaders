#ifndef GameOverScene_h
#define GameOverScene_h
#include "cocos2d.h"

class GameOverScene : public cocos2d::Layer
{
public:
    cocos2d::Scene* createScene(int score);
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameOverScene);
private:
    void goToMenuScene(cocos2d::Ref *sender);
    
};

#endif /* GameOverScene_h */
