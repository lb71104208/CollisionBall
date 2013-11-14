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

class Item : public Ball  {
    
public:
    ~Item();
    Item(int type, float duration, float timer);
};

#endif /* defined(__CollisionBall__item__) */
