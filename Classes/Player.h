#ifndef Player_h
#define Player_h
class Player
{
public:
    Player(cocos2d::Layer *layer);
    void Shoot(cocos2d::Layer *layer);
    void setShoot();
    void move(int move);
    int getDirection();
    cocos2d::Sprite* getShip();
    void setCanMove(){ canMove = false; }

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Sprite *ship;
    int m_missileNum;
    bool isShooting;
    int direction;
    bool canMove;
};
#endif /* Player_h */
