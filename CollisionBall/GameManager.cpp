//
//  GameManager.cpp
//  CollisionBall
//
//  Created by li bo on 13-11-17.
//
//

#include "GameManager.h"
#include "HelloWorldScene.h"

GameManager::~GameManager()
{
    CC_SAFE_RELEASE(_managers);
}

GameManager::GameManager(HelloWorld* game)
{
    _game = game;
    _managers = CCArray::createWithCapacity(NOOFITEMTYPES);
    _managers->retain();
}


void GameManager::init()
{
    srand(time(0));
   
    
    for (int i=0; i<=NOOFITEMTYPES; i++) {
        int type = i + 3;
        itemManager* item = new itemManager(type);
        _managers->addObject(item);
    }
    
}

void GameManager::produceItemsByType(uint nType)
{
  //  printf("%d ",i);
    switch (nType) {
        case kItemShorten:
        {
            int i = 0 + rand()%100;
            if (i<5) {
                _game->initItem(kItemShorten);
            }
            else
            {
                return;
            }

        }
            break;
            
        default:
            break;
    }
   }

void GameManager::update(float dt)
{
    itemManager* itemmanager;
    
    for (int i = 0; i<NOOFITEMTYPES; i++) {
        itemmanager = (itemManager*)_managers->objectAtIndex(i);
        itemmanager->update(dt);
        if (itemmanager->getState() == kItemCanProduce) {
            this->produceItemsByType(i+2);
        }
    }

}

void GameManager::resetItem(int type)
{
    itemManager*  itemmanager = (itemManager*)_managers->objectAtIndex(type -3);
    itemmanager->setState(kItemCanNotProduce);
    srand(time(0));
}


