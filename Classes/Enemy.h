#ifndef Enemy_h
#define Enemy_h

class Enemy
{
public:
    Enemy();
    cocos2d::Sprite** SpawnEnemies();//cocos2d::Layer *layer);
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
};

#endif /* Enemy_hpp */
