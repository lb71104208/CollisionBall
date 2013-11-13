
#include "player.h"
#include "HelloWorldScene.h"

Player::~Player() {
}

Player::Player(HelloWorld * game):b2Sprite(game, kSpritePlayer) {
}

Player* Player::create(HelloWorld * game) {
    Player * sprite = new Player(game);
	if (sprite) {
        sprite->initPlayer();
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}


void Player::initPlayer() {
    
    //create body
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    
    _body = _game->getWorld()->CreateBody(&bodyDef);
    _body->SetSleepingAllowed(true);
    _body->SetLinearDamping(1.2);
    _body->SetAngularDamping(0.8);
    _body->SetBullet(true);
    
    //set sprite texture
    this->initWithFile("plate.png");
    //Define shape
    b2PolygonShape box;
    box.SetAsBox(this->getContentSize().height/2 /PTM_RATIO, this->getContentSize().width/2/ PTM_RATIO);
    
    //Define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 10;
    fixtureDef.restitution = 1;
    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
    
    _score = 0;
    
}

