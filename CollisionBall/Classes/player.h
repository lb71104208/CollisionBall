

#ifndef __PLATE__
#define __PLATE__

#include "b2Sprite.h"

class Player : public b2Sprite {
public:
    ~Player();
    Player(HelloWorld * game);
    CC_SYNTHESIZE(CCPoint, _nextPosition, NextPosition);
	
	CC_SYNTHESIZE(CCPoint, _vector, Vector);
    CC_SYNTHESIZE(int, _score, Score);
    CC_SYNTHESIZE(CCTouch *, _touch, Touch);
    static Player* create(HelloWorld * game);
    //virtual void setPosition(const CCPoint& pos);
private:
    void initPlayer();
    
};
 

#endif