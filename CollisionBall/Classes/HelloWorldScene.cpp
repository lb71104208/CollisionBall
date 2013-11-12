//
//  HelloWorldScene.cpp
//  CollisionBall
//
//  Created by li bo on 13-11-6.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

HelloWorld::HelloWorld()
{
    setTouchEnabled( true );
    setAccelerometerEnabled( true );

    _screenSize = CCDirector::sharedDirector()->getWinSize();
    
//    CCSprite* bg =  CCSprite::create("court.png");
//    bg->setPosition(ccp(_screenSize.width*0.5f, _screenSize.height*0.5f));
//    this->addChild(bg);
    
    _startflag= CCSprite::create("logo.png");
    _startflag->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
    this->addChild(_startflag,1);
    _startflag->setVisible(true);
    
    _player1ScoreLabel = CCLabelTTF::create("0", "Arial", 60);
    _player1ScoreLabel->setPosition(ccp(_screenSize.width - 60, _screenSize.height * 0.5 - 80));
    _player1ScoreLabel->setRotation(90);
    this->addChild(_player1ScoreLabel);
    
    _player2ScoreLabel = CCLabelTTF::create("0", "Arial", 60);
    _player2ScoreLabel->setPosition(ccp(_screenSize.width - 60, _screenSize.height * 0.5 + 80));
    _player2ScoreLabel->setRotation(90);
    this->addChild(_player2ScoreLabel);

    // init physics
    this->initPhysics();
    setTouchEnabled( true );

    scheduleUpdate();
    
    _gameState = kGameReady;
    _player1Score = 0;
    _player2Score = 0;
    
    
    
}

HelloWorld::~HelloWorld()
{
    delete _world;
    _world = NULL;
    
    delete m_debugDraw;
    delete _collisionListener;
    
    CC_SAFE_RELEASE(_players);
    CC_SAFE_RELEASE(_baselines);
    //delete m_debugDraw;
}

void HelloWorld::initPhysics()
{

    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    _world = new b2World(gravity);

    // Do we want to let bodies sleep?
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    
     m_debugDraw = new GLESDebugDraw( PTM_RATIO );
     _world->SetDebugDraw(m_debugDraw);
    
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    m_debugDraw->SetFlags(flags);

    _collisionListener = new CollisionListener(this);
    _world->SetContactListener(_collisionListener);
    
    // Define the table body
    
    b2BodyDef bodyDef;
    // add baselines
    _baselines = CCArray::createWithCapacity(2);
    _baselines->retain();
    
    bodyDef.position.Set(0, 0);

    b2Body * baseline;
    b2EdgeShape edge;
    b2Sprite* baselineData;
    for (int i = 0; i < 2; i++) {
        bodyDef.type = b2_staticBody;
        edge.Set(b2Vec2(0.0f, i* _screenSize.height /PTM_RATIO),b2Vec2(_screenSize.width /PTM_RATIO,i*_screenSize.height/PTM_RATIO));
        
        baseline = _world->CreateBody(&bodyDef);
        baseline->CreateFixture(&edge,0);
        baselineData = new b2Sprite(this, kSpriteBaseline);
        baseline->SetUserData(baselineData);
        _baselines->addObject(baselineData);
    }
    
    // add table sides
    b2BodyDef tableBodyDef;
    tableBodyDef.type = b2_staticBody;
    tableBodyDef.position.Set(0, 0);
    b2Body* tableBody = _world->CreateBody(&tableBodyDef);
    
    // left
    b2EdgeShape tableSide;
    tableSide.Set(b2Vec2(0, 0),b2Vec2(0, _screenSize.height/PTM_RATIO));
    tableBody->CreateFixture(&tableSide,0);
    
    // right
    tableSide.Set(b2Vec2(_screenSize.width/PTM_RATIO, 0),b2Vec2(_screenSize.width/PTM_RATIO, _screenSize.height/PTM_RATIO));
    tableBody->CreateFixture(&tableSide,0);
    
    //add ball
    _ball = Ball::create(this, kSpriteBall, ccp(_screenSize.width/2, _screenSize.height/2));
    this->addChild(_ball);
    
    //create box2d body
//    b2BodyDef ballBodyDef;
//    ballBodyDef.type = b2_dynamicBody;
//    
//    b2Body *ballBody;
//    ballBody = _world->CreateBody(&bodyDef);
//    ballBody->SetSleepingAllowed(true);
//    ballBody->SetLinearDamping(1.2);
//    ballBody->SetAngularDamping(0.8);
//    ballBody->SetTransform(b2Vec2(_screenSize.width/2, _screenSize.height/2),0);
//    
//    //create circle shape
//    b2CircleShape  circle;
//    circle.m_radius = 8/PTM_RATIO;
//    
//    //define fixture
//    b2FixtureDef fixtureDef;
//    fixtureDef.shape = &circle;
//    fixtureDef.density = 5;
//    fixtureDef.restitution = 0.7;
//    ballBody->CreateFixture(&fixtureDef);

    
    //add player
    _players = CCArray::createWithCapacity(2);
    _players->retain();
    Player * player;
    for (int i = 0 ; i<2; i++) {
        player = Player::create(this);
        
        player->setSpritePosition(ccp(_screenSize.width/2, i==0?player->getContentSize().height *2:_screenSize.height - player->getContentSize().height *2));
        player->getBody()->SetTransform(player->getBody()->GetPosition(), M_PI*0.5);
        this->addChild(player);
        _players->addObject(player);

    }
}
//
void HelloWorld::draw()
{
   
    CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    _world->DrawDebugData();

    kmGLPopMatrix();
}

void HelloWorld::update(float dt)
{
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    if (_gameState != kGamePlaying) {
        return;
    }
    
    int velocityIterations = 10;
    int positionIterations = 10;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    _world->Step(dt, velocityIterations, positionIterations);
   
    _ball->update(dt);
    for (int p = 0; p < _players->count(); p++) {
        
       Player* player = (Player *) _players->objectAtIndex(p);
        //player->setPosition(player->getNextPosition());
        player->update(dt);
    }

}

void HelloWorld::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
    if (_startflag->isVisible()==true) {
        _startflag->setVisible(false);
        _gameState = kGamePlaying;
        _ball->getBody()->ApplyLinearImpulse(b2Vec2(0.0,-100.0),_ball->getBody()->GetWorldCenter());
        return;
    }
    
    
    CCSetIterator i;
    CCTouch* touch;
    CCPoint tap;
    Player* player;
    
    for (i=pTouches->begin(); i!=pTouches->end(); i++) {
        touch = (CCTouch*)(*i);
        if (touch) {
            tap=touch->getLocation();
            for (int p = 0; p < 2; p++) {
        		player = (Player *) _players->objectAtIndex(p);
                
                if (player->boundingBox().containsPoint(tap)) {
                	//store player's touch
                	player->setTouch(touch);
                    //_ball->getBody()->ApplyLinearImpulse(b2Vec2(0,100), _ball->getBody()->GetWorldCenter());
				}
        	}
        }
    }
}

void HelloWorld::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *event)
{
    CCSetIterator i;
    CCTouch* touch;
    CCPoint tap;
    Player* player;
    
    for( i = pTouches->begin(); i != pTouches->end(); i++) {
		touch = (CCTouch*) (*i);
		
		if(touch) {
			tap = touch->getLocation();
			
			for (int p = 0; p < _players->count(); p++) {
        		
        		player = (Player *) _players->objectAtIndex(p);
        		//if touch belongs to player
        		if (player->getTouch() != NULL && player->getTouch() == touch) {
                    CCPoint nextPosition = tap;
                    
                    //keep player inside screen
                    if (nextPosition.x < player->getContentSize().width/2)
                        nextPosition.x = player->getContentSize().width/2;
                    if (nextPosition.x > _screenSize.width - player->getContentSize().width/2)
                        nextPosition.x = _screenSize.width - player->getContentSize().width/2;
                    if (nextPosition.y < player->getContentSize().height/2)
                        nextPosition.y  = player->getContentSize().height/2;
                    if (nextPosition.y > _screenSize.height - player->getContentSize().height/2)
                        nextPosition.y = _screenSize.height - player->getContentSize().height/2;
                
                    
                    //keep player inside its court
                    if (player->getPositionY() < _screenSize.height * 0.5f) {
                        if (nextPosition.y > _screenSize.height * 0.5 - player->getContentSize().height/2 ) {
                            nextPosition.y = _screenSize.height * 0.5 - player->getContentSize().height/2;
                        }
                    } else {
                        if (nextPosition.y < _screenSize.height * 0.5 + player->getContentSize().height/2) {
                            nextPosition.y = _screenSize.height * 0.5 + player->getContentSize().height/2;
                        }
                    }
                    player->getBody()->SetTransform(b2Vec2(nextPosition.x/PTM_RATIO,nextPosition.y/PTM_RATIO),b2_pi/2 );
                    //player->setPosition(nextPosition);
                    //player->setVector(ccp(tap.x - player->getPositionX(), tap.y - player->getPositionY()));
				}
        	}
		}
	}
}


void HelloWorld::ccTouchesEnded(CCSet* pTouches, CCEvent* event) {
    
	CCSetIterator i;
	CCTouch* touch;
    Player * player;
    
    //loop through all ending touches
	for( i = pTouches->begin(); i != pTouches->end(); i++) {
		touch = (CCTouch*) (*i);
		
		if(touch) {
			for (int p = 0; p < _players->count(); p++) {
        		player = (Player *) _players->objectAtIndex(p);
        		if (player->getTouch() != NULL && player->getTouch() == touch) {
					//if touch ending belongs to this player, clear it
					player->setTouch(NULL);
                    player->setVector(CCPointZero);
				}
        	}
		}
	}
	
}


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new HelloWorld();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

void HelloWorld::playerScore (int player) {
    
    SimpleAudioEngine::sharedEngine()->playEffect("score.wav");
    
    char score_buffer[10];
    
    //if player 1 scored...
    if (player == 1) {
        
        _player1Score++;
        sprintf(score_buffer,"%i", _player1Score);
        _player1ScoreLabel->setString(score_buffer);
        //move ball to player 2 court
        _ball->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5 + 2 * _ball->getContentSize().width/2));
        
        //if player 2 scored...
    } else {
        
        _player2Score++;
        sprintf(score_buffer,"%i", _player2Score);
        _player2ScoreLabel->setString(score_buffer);
        //move ball to player 1 court
        _ball->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5 - 2 * _ball->getContentSize().width/2));
        
    }
    //move players to original position
    
    for (int p = 0; p < _players->count(); p++) {
        Player* ply = (Player*)_players->objectAtIndex(p);
        if (p == 0) {
            ply->setPosition(ccp(_screenSize.width * 0.5, ply->getContentSize().height/2));
        }
        else
        {
            ply->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height - ply->getContentSize().height/2));
        }
        //clear current touches
        ply->setTouch(NULL);
    }
}