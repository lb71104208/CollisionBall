//
//  item.cpp
//  CollisionBall
//
//  Created by li bo on 13-11-14.
//
//

#include "item.h"

Item::~Item()
{
    
}

Item::Item(HelloWorld * game, int type, CCPoint position,float duration):Ball(game,type,position)
{
    _duration = duration;
}

Item* Item::create(HelloWorld * game, int type, CCPoint position, float duration) {
    Item * itemSprite = new Item(game, type, position,duration);
	
    if (itemSprite) {
        switch (type) {
            case kSpriteBall:
                itemSprite->initBallWithFile("ball.png");
                break;
                
            case kItemShorten:
                itemSprite->initBallWithFile("itemshtn.png");
                break;
                
            default:
                break;
        }

		itemSprite->autorelease();
		return itemSprite;
	}
	CC_SAFE_DELETE(itemSprite);
	return NULL;
}
