//
//  item.h
//  CollisionBall
//
//  Created by li bo on 13-11-14.
//
//

#ifndef __CollisionBall__item__
#define __CollisionBall__item__

#include <iostream>
#include "Ball.h"
#include "HelloWorldScene.h"

class Item : public Ball  {
    
public:
    ~Item();
    Item(HelloWorld * game, int type, CCPoint position,float duration);
    static Item* create(HelloWorld * game, int type, CCPoint position, float duration);
    CC_SYNTHESIZE(float, _duration, Duration);
};

#endif /* defined(__CollisionBall__item__) */
