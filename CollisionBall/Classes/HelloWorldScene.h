//
//  HelloWorldScene.h
//  CollisionBall
//
//  Created by li bo on 13-11-6.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//  change
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__


// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"
#include "GLES-Render.h"
#include "Ball.h"
#include "player.h"
#include "CollisionListener.h"
using namespace cocos2d;

enum  {
    kPlayer1Tag,
    kPlayer2Tag
};

enum  {
     kGamePlaying,
     kGameReady
    };

class HelloWorld : public cocos2d::CCLayer {
public:
    ~HelloWorld();
    HelloWorld();
    
    CC_SYNTHESIZE(b2World*, _world, World);
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    void initPhysics();
    
    virtual void draw();
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void update(float dt);
    void playerScore (int player);

private:
    
    GLESDebugDraw * m_debugDraw;
    b2ContactListener * _collisionListener;
    
    CCTouch *_touch;
    CCArray* _baselines;
    CCArray* _players;
    
    Ball* _ball;
    CCSprite* _startflag;
    CCLabelTTF* _player1ScoreLabel;
    CCLabelTTF* _player2ScoreLabel;
    
    CCSize _screenSize;
    
    int _player1Score;
    int _player2Score;
    
    int _gameState;
    
    void resettGame(int winner);
    
};

#endif // __HELLO_WORLD_H__
