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
    m_timer = 0;
}

void itemManager::update(float dt)
{
    if (_state == kItemCanNotProduce) {
        m_timer += dt;
        if (m_timer>m_silentDelta) {
            _state = kItemCanProduce;
            m_timer = 0;
        }
    }
    
    else if (_state == kItemDisplaying)
    {
        m_timer += dt;
        if (m_timer > m_displayingDelta) {
            _state = kItemDestroy;
            m_timer = 0;
        }
    }
}
