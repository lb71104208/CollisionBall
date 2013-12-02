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

static int param = 1;

HelloWorld::HelloWorld()
{
    setTouchEnabled( true );
    setAccelerometerEnabled( true );

    _screenSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* bg =  CCSprite::create("bg.png");
    bg->setPosition(ccp(_screenSize.width*0.5f, _screenSize.height*0.5f));
    this->addChild(bg);
    
    _startflag= CCSprite::create("logo.png");
    _startflag->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
    this->addChild(_startflag,1);
    _startflag->setVisible(true);
    
    _player1ScoreLabel = CCLabelTTF::create("0", "Arial", 60);
    _player1ScoreLabel->setPosition(ccp(_screenSize.width - 60, _screenSize.height * 0.5 + 80));
    _player1ScoreLabel->setRotation(90);
    this->addChild(_player1ScoreLabel);
    
    _player2ScoreLabel = CCLabelTTF::create("0", "Arial", 60);
    _player2ScoreLabel->setPosition(ccp(_screenSize.width - 60, _screenSize.height * 0.5 - 80));
    _player2ScoreLabel->setRotation(90);
    this->addChild(_player2ScoreLabel);
    
    _gameManager = new GameManager(this);
    _gameManager->init();
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
    
    delete _gameManager;
    _gameManager = NULL;
    
    delete m_debugDraw;
    delete _collisionListener;
    
    CC_SAFE_RELEASE(_players);
    CC_SAFE_RELEASE(_baselines);
    CC_SAFE_RELEASE(_items);
    //delete m_debugDraw;
}

void HelloWorld::initPhysics()
{

    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    _world = new b2World(gravity);

    // Do we want to let bodies sleep?
    //_world->SetAllowSleeping(true);
    //_world->SetContinuousPhysics(true);
    
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
        edge.Set(b2Vec2(0.0f, i==0?124.0f/PTM_RATIO:(_screenSize.height -124.0f)/PTM_RATIO),b2Vec2(_screenSize.width /PTM_RATIO,i==0?124.0f/PTM_RATIO:(_screenSize.height -124.0f)/PTM_RATIO));
        
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

    
    //add player
    _players = CCArray::createWithCapacity(2);
    _players->retain();
    Player * player;
    for (int i = 0 ; i<2; i++) {
        player = Player::create(this);
        
        player->setSpritePosition(ccp(_screenSize.width/2, i==1?player->getContentSize().height *5:_screenSize.height - player->getContentSize().height *5));
        player->getBody()->SetTransform(player->getBody()->GetPosition(), M_PI*0.5);
        this->addChild(player);
        _players->addObject(player);

    }
    
    _items = CCArray::createWithCapacity(3);
    _items->retain();
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
    
    
    
    if(_gameState == kMatchNew||_gameState == kGameOver)
    {
        resetGame();
        return;
    }
    
    else if (_gameState != kGamePlaying) {
        return;
    }
    
    _gameManager->produceItems(dt);
    
    int velocityIterations = 10;
    int positionIterations = 20;
    
    uint substeps = 3;
    float32 subdt = dt / substeps;
    
    for (uint i = 0; i < substeps; i++) {
        _world->Step(subdt, velocityIterations, positionIterations);
        
        // do your physics-related stuff inside here but leave any sprites manipulation outside this loop
    }
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    //_world->Step(dt, velocityIterations, positionIterations);
   
    _ball->update(dt);
    for (int p = 0; p < _players->count(); p++) {
        
       Player* player = (Player *) _players->objectAtIndex(p);
        //player->setPosition(player->getNextPosition());
        player->update(dt);
    }
    
    //track invisible objects
    int count = _items->count();
    Item * item;
    
    for (int i = 0; i < count; i++) {
        
        item = (Item *) _items->objectAtIndex(i);
        
        if (!item->isVisible()) {
            
            item->hide();
            _items->removeObject(item);
        } else {
            
            item->update(dt);
        }
    }

}

void HelloWorld::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
    if (_gameState == kMatchOver) {
        _gameState = kMatchNew;
    }
    if (_gameState == kGameReady) {
        _startflag->setVisible(false);
        _ball->getBody()->ApplyLinearImpulse(b2Vec2(0.0,-100.0),_ball->getBody()->GetWorldCenter());
        _gameState = kGamePlaying;
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
    Player* winner;
    //if player 1 scored...
    if (player == kPlayer1Tag) {
        winner = (Player*)_players->objectAtIndex(0);
        winner->setScore(winner->getScore()+1);
        sprintf(score_buffer,"%i", winner->getScore());
        //sprintf(score_buffer,"%s", "哈哈哈");
        _player1ScoreLabel->setString(score_buffer);
    }
    //if player 2 scored...
    else
    {
        winner = (Player*)_players->objectAtIndex(1);
        winner->setScore(winner->getScore()+1);
        sprintf(score_buffer,"%i", winner->getScore());
        _player2ScoreLabel->setString(score_buffer);

    }
    
    if (winner->getScore() == 3) {
        if (player == 1) {
            playerWin(kPlayer1Tag);
        }
        else
        {
            playerWin(kPlayer2Tag);
        }
        return;
    }
    _gameState = kGameOver;
}

void HelloWorld::playerWin(int player)
{
    if (player == 1) {
        _winnerflag= CCSprite::create("player1win.png");
        _winnerflag->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
        this->addChild(_winnerflag,1);
        _winnerflag->setVisible(true);
    }
    else{
        _winnerflag= CCSprite::create("player2win.png");
        _winnerflag->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
        this->addChild(_winnerflag,1);
        _winnerflag->setVisible(true);
    }
    
    _gameState = kMatchOver;

}

void HelloWorld::resetGame()
{
    if (_gameState == kMatchNew) {
        for (int i = 0; i<2; i++) {
            Player* player = (Player*)_players->objectAtIndex(i);
            player->setScore(0);
            player->Scale(1.0f);
        }
        _player1ScoreLabel->setString("0");
        _player2ScoreLabel->setString("0");
        
        _winnerflag->setVisible(false);
        this->removeChild(_winnerflag);
    }
    
    //move players to original position
    for (int p = 0; p < _players->count(); p++) {
        Player* ply = (Player*)_players->objectAtIndex(p);
        ply->setSpritePosition(ccp(_screenSize.width/2, p==1?ply->getContentSize().height *5:_screenSize.height - ply->getContentSize().height *5));
        //clear current touches
        ply->setTouch(NULL);
    }
    _ball->setVisible(true);
    _ball->setSpritePosition(ccp(_screenSize.width/2,_screenSize.height/2));
    _ball->getBody()->SetLinearVelocity(b2Vec2(0,0));
    _startflag->setVisible(true);
    
    for (int i=0; i<_items->count(); i++) {
        Item* item = (Item*)_items->objectAtIndex(i);
        _world->DestroyBody(item->getBody());
        item->setVisible(false);
        this->removeChild(item);
    }
    _items->removeAllObjects();
    _gameManager->init();
    param = 1;
    _gameState = kGameReady;
    
}

void HelloWorld::initItem(int type)
{
    srand(time(0));
    int x = rand() % (int) (_screenSize.width * 0.8f) + _screenSize.width * 0.1f;
    int y = rand() % (int) ((_screenSize.height - 2*124.0f) * 0.8f) + 124.0f;
    CCPoint appearPosition = CCPoint(x, y);
    
    switch (type) {
        case kItemShorten:
        {
            Item* itemShorten = Item::create(this,kItemShorten,appearPosition,5.0f);
            this->addChild(itemShorten,1,kItemShorten);
            _items->addObject(itemShorten);
            itemShorten->getBody()->ApplyLinearImpulse(b2Vec2(x/10,y/10), itemShorten->getBody()->GetWorldCenter());
        }
            break;
            
        default:
            break;
    }
}

void HelloWorld::applyEffects(int itemType,int playerTag, Item* item)
{
    switch (itemType) {
        case kItemShorten:
        {
            Player* affectplayer;
            if (playerTag == kPlayer1Tag) {
                affectplayer = (Player*)_players->objectAtIndex(kPlayer2Tag);
            }
            else
            {
                affectplayer = (Player*)_players->objectAtIndex(kPlayer1Tag);
            }
            
            if (param < 4) {
                    affectplayer->Scale(1.0f - 0.2f * param);
                    param ++;
                }
            
            //_world->DestroyBody(item->getBody());
            item->setVisible(false);
            //this->removeChild(item);
            //_items->removeObject(item);
            _gameManager->resetItem(kItemShorten);
        }
            break;
            
        default:
            break;
    }
}