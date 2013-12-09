//
//  itemManager.h
//  CollisionBall
//
//  Created by li bo on 13-11-22.
//
//

#ifndef __CollisionBall__itemManager__
#define __CollisionBall__itemManager__

#include <iostream>
#include "HelloWorldScene.h"

enum ITEMSTATUS {
    kItemCanProduce,
    kItemCanNotProduce,
    kItemDisplaying,
    kItemDestroy
    };

class itemManager:public CCObject {
private:
    int itemType;
    float m_silentDelta;
    float m_displayingDelta;
    
public:
    ~itemManager();
    itemManager(int type);
    void update(float dt);
    CC_SYNTHESIZE(int, _state, State);
    CC_SYNTHESIZE(float, _timer, Timer);

};

#endif /* defined(__CollisionBall__itemManager__) */
