//
//  CollisionListener.cpp
//  MiniPool
//
//  Created by Roger Engelbert on 1/14/13.
//
//

#include "CollisionListener.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CollisionListener::CollisionListener (HelloWorld * game) {
    _game = game;
}

void CollisionListener::BeginContact(b2Contact* contact) {
    //bool islocked = _game->getWorld()->IsLocked();
    b2Body * bodyA = contact->GetFixtureA()->GetBody();
    b2Body * bodyB = contact->GetFixtureB()->GetBody();
    
    b2Sprite * spriteA = (b2Sprite *) bodyA->GetUserData();
    b2Sprite * spriteB = (b2Sprite *) bodyB->GetUserData();
    
    if (spriteA && spriteB) {
        
        //track collision between balls and pockets
        if (spriteA->getType() == kSpriteBaseline) {
            spriteB->setVisible(false);
            if (spriteA->getPosition().y < _game->getContentSize().height/2) {
                _game->playerScore(kPlayer2Tag);
            }
            else
            {
                _game->playerScore(kPlayer1Tag);
            }
        
        } else if (spriteB->getType() == kSpriteBaseline) {
            spriteA->setVisible(false);
            if (spriteB->getPosition().y < _game->getContentSize().height/2) {
                _game->playerScore(kPlayer2Tag);
            }
            else
            {
                _game->playerScore(kPlayer1Tag);
            }
        
        }
        else if ((spriteA->getType() == kSpriteBall &&
                    spriteB->getType() == kSpritePlayer) ||
                   (spriteB->getType() == kSpriteBall &&
                    spriteA->getType() == kSpritePlayer)) {
            SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
        }
    }
}


