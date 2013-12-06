//
//  GameManager.h
//  CollisionBall
//
//  Created by li bo on 13-11-17.
//
//

#ifndef __CollisionBall__GameManager__
#define __CollisionBall__GameManager__

#include <iostream>
#include "cocos2d.h"
#include "b2Sprite.h"
#include <map>
#include "itemManager.h"

#define NOOFITEMTYPES 1
using namespace std;
class HelloWorld;
class itemManager;

class GameManager:public cocos2d::CCObject {
    
public:
    ~GameManager();
    GameManager(HelloWorld* game);
    static HelloWorld* managedGame();
    void init();
    void produceItemsByType(uint nType);
    void destroyItemByType(uint nType);
    void update(float dt);
    CC_SYNTHESIZE(HelloWorld*, _game, Game);
    void resetItem(int type);
    
private:
    CCArray* _managers;
};

#endif /* defined(__CollisionBall__GameManager__) */
