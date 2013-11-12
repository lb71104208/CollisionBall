

#ifndef __CollisionListener__
#define __CollisionListener__

#include "Box2D.h"
#include "Ball.h"
#include "player.h"
#include "HelloWorldScene.h"

class CollisionListener : public b2ContactListener
{
    public :
    CollisionListener (HelloWorld * game);
    CC_SYNTHESIZE(HelloWorld *, _game, Game);

    virtual void BeginContact(b2Contact* contact);
    //virtual void EndContact(b2Contact* contact);
    //virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    //virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
};


#endif /* defined(__MiniPool__CollisionListener__) */
