//
//  Ball.cpp
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#include "Ball.h"
#include "HelloWorldScene.h"

Ball::~Ball() {   
}

Ball::Ball (HelloWorld * game, int type, CCPoint position) : b2Sprite (game, type) {
    _type = type;
    _startPosition = position;
}

Ball* Ball::create(HelloWorld * game, int type, CCPoint position) {
    Ball * sprite = new Ball(game, type, position);
	if (sprite) {
        sprite->initBall();
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}

void Ball::initBall() {
    this->initWithFile("ball.png");

    //create box2d body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;

    _body = _game->getWorld()->CreateBody(&bodyDef);
    _body->SetSleepingAllowed(true);
    _body->SetLinearDamping(0.0);
    _body->SetAngularDamping(0.8);
    _body->SetBullet(true);
    
    //create circle shape
    b2CircleShape  circle;
    //circle.m_radius = BALL_RADIUS/PTM_RATIO;
    
    circle.m_radius = this->getContentSize().width/2/PTM_RATIO;
    
    //define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 1;
    fixtureDef.restitution = 0.7;
    
    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
    
    setSpritePosition(_startPosition);
}

void Ball::reset() {
    if (_body) {
        _body->SetLinearVelocity(b2Vec2_zero);
        _body->SetAngularVelocity(0);
       _body->SetAwake(true);
    }
    setSpritePosition(_startPosition);
    setVisible(true);
}

void Ball::update (float dt) {
    
    if (_body && isVisible()) {
        setPositionX(_body->GetPosition().x * PTM_RATIO);
        setPositionY(_body->GetPosition().y * PTM_RATIO);
    }
}