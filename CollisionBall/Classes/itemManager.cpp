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
            m_delta = 2.5f;
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
        if (m_timer>m_delta) {
            _state = kItemCanProduce;
            m_timer = 0;
        }
    }
}
