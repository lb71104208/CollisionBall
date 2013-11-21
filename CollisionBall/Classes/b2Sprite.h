//
//  b2Sprite.h
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#ifndef __MiniPool__b2Sprite__
#define __MiniPool__b2Sprite__

#define PTM_RATIO 32.0
#define BALL_RADIUS 40.0

#include "cocos2d.h"
#include "Box2D.h"

class HelloWorld;

enum {
    kSpriteBall,
    kSpritePlayer,
    kSpriteBaseline
};

enum ITEMS {
    kItemShorten = 3
};

using namespace cocos2d;

class b2Sprite : public CCSprite {
    
public:
    
    b2Sprite (HelloWorld * game, int type);

    CC_SYNTHESIZE(b2Body *, _body, Body);
    CC_SYNTHESIZE(HelloWorld *, _game, Game);
    CC_SYNTHESIZE(int, _type, Type);
    
    virtual void setSpritePosition (CCPoint position);
    virtual void update(float dt);
    virtual void hide(void);
    virtual void reset(void);
    virtual float mag();


};


#endif /* defined(__MiniPool__b2Sprite__) */
