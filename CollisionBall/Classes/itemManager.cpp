//
//  itemManager.cpp
//  CollisionBall
//
//  Created by li bo on 13-11-22.
//
//

#include "itemManager.h"

itemManager::~itemManager()
{
    
}

itemManager::itemManager(int type)
{
    switch (type) {
        case kItemShorten:
        {
            m_silentDelta = 1.5f;
            m_displayingDelta = 5.0f;
        }
            break;
            
        default:
            break;
    }
    itemType = type;
    _state = kItemCanNotProduce;
    _timer = 0;
}

void itemManager::update(float dt)
{
    if (_state == kItemCanNotProduce) {
        _timer += dt;
        if (_timer>m_silentDelta) {
            _state = kItemCanProduce;
            _timer = 0;
        }
    }
    
    else if (_state == kItemDisplaying)
    {
        _timer += dt;
        if (_timer > m_displayingDelta) {
            _state = kItemDestroy;
            _timer = 0;
        }
    }
}
