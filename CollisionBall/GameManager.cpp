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
    itemManager* itemmgr;
  //  printf("%d ",i);
    switch (nType) {
        case kItemShorten:
        {
            int i = 0 + rand()%100;
            if (i<5) {
                _game->initItem(kItemShorten);
                itemmgr = (itemManager*) _managers->objectAtIndex(nType -3);
                itemmgr->setState(kItemDisplaying);
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

void GameManager::destroyItemByType(uint nType)
{
    _game->destroyItem(kItemShorten);
    //itemmgr = (itemManager*) _managers->objectAtIndex(nType -3);
}

void GameManager::update(float dt)
{
    itemManager* itemmanager;
    
    for (int i = 0; i<NOOFITEMTYPES; i++) {
        itemmanager = (itemManager*)_managers->objectAtIndex(i);
        itemmanager->update(dt);
        if (itemmanager->getState() == kItemCanProduce) {
            this->produceItemsByType(i+3);
        }
        else if ( itemmanager ->getState() == kItemDestroy)
        {
            this->destroyItemByType(i+3);
        }
    }

}

void GameManager::resetItem(int type)
{
    itemManager*  itemmanager = (itemManager*)_managers->objectAtIndex(type -3);
    itemmanager->setState(kItemCanNotProduce);
}


