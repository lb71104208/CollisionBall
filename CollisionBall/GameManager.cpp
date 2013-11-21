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
    
}

GameManager::GameManager(HelloWorld* game)
{
    _game = game;
}



void GameManager::init()
{
    srand(time(0));
    appearingItemsMap.clear();
    appearingItemsMap.insert(map<int, int> :: value_type(kItemShorten,0));
    
}

void GameManager::produceItems()
{
    int i = 0 + rand()%100;
  //  printf("%d ",i);
    if (i<5) {
        map<int,int >::iterator it= appearingItemsMap.find(kItemShorten);
        int flag = it->second;
        if (flag == 0) {
            _game->initItem(kItemShorten);
            it->second = 1;
        }
        
    }
    else
    {
        return;
    }
}

void GameManager::resetItem(int type)
{
    srand(time(0));
    switch (type) {
        case kItemShorten:
        {
            map<int,int >::iterator it= appearingItemsMap.find(kItemShorten);
            it->second = 0;
        }
            break;
            
        default:
            break;
    }
}


