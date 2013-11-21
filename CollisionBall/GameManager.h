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
using namespace std;
class HelloWorld;

class GameManager:public cocos2d::CCObject {
    
public:
    ~GameManager();
    GameManager(HelloWorld* game);
    static HelloWorld* managedGame();
    void init();
    void produceItems();
    CC_SYNTHESIZE(HelloWorld*, _game, Game);
    void resetItem(int type);
    
private:
    map<int, int> appearingItemsMap;     //0没出现 1正在出现
};

#endif /* defined(__CollisionBall__GameManager__) */
