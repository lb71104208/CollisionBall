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
#include "GameManager.h"
#include "item.h"

using namespace cocos2d;
class Item;

enum  {
    kPlayer1Tag,
    kPlayer2Tag
};

enum  {
    kGamePlaying,
    kGameReady,
    kGameOver,
    kMatchOver,
    kMatchNew
};

class HelloWorld : public cocos2d::CCLayer {
public:
    ~HelloWorld();
    HelloWorld();
    
    CC_SYNTHESIZE(b2World*, _world, World);
    CC_SYNTHESIZE(CCSize, _screenSize, ScreenSize) ;
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    void initPhysics();
    void resetGame();
    virtual void draw();
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void update(float dt);
    void playerScore (int player);
    void playerWin (int player);
    void initItem(int type);
    void applyEffects(int itemType, int playerTag, Item* item);

private:
    
    GLESDebugDraw * m_debugDraw;
    b2ContactListener * _collisionListener;
    
    CCTouch *_touch;
    CCArray* _baselines;
    CCArray* _players;
    CCArray* _items;
    Ball* _ball;
    CCSprite* _startflag;
    CCSprite* _winnerflag;
    CCLabelTTF* _player1ScoreLabel;
    CCLabelTTF* _player2ScoreLabel;
    
    GameManager* _gameManager;
    
    int _player1Score;
    int _player2Score;
    
    int _gameState;
    
    void resettGame(int winner);
    
};

#endif // __HELLO_WORLD_H__
