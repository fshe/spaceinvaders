#ifndef Shield_h
#define Shield_h
class Shield
{
public:
    Shield();
    cocos2d::Sprite** SpawnShields();
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

};

#endif /* Shield_h */
