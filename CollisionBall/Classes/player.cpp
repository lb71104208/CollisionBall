
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
    _body->SetLinearDamping(0.0);
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

void Player::Scale(float rate)
{
    this->setScaleX(rate);
    ((b2PolygonShape*)this->getBody()->GetFixtureList()->GetShape())->SetAsBox(this->boundingBox().size.height/2 /PTM_RATIO, this->boundingBox().size.width/2/ PTM_RATIO);
}

void Player::update(cocos2d::CCPoint direction, float velocity)
{
    CCPoint nextPosition ;
    nextPosition.x = this->getPosition().x+direction.x*velocity/2;
    nextPosition.y = this->getPosition().y+direction.y*velocity/2;
    
    
    //keep this inside screen
    if (nextPosition.x < this->getContentSize().width/2)
        nextPosition.x = this->getContentSize().width/2;
    if (nextPosition.x > _game->getScreenSize().width - this->getContentSize().width/2)
        nextPosition.x = _game->getScreenSize().width - this->getContentSize().width/2;
    if (nextPosition.y < this->getContentSize().height/2)
        nextPosition.y  = this->getContentSize().height/2;
    if (nextPosition.y > _game->getScreenSize().height - this->getContentSize().height/2)
        nextPosition.y = _game->getScreenSize().height - this->getContentSize().height/2;
    
    
    //keep this inside its court
    if (this->getPositionY() < _game->getScreenSize().height * 0.5f) {
        if (nextPosition.y > _game->getScreenSize().height * 0.5 - this->getContentSize().height/2 ) {
            nextPosition.y = _game->getScreenSize().height * 0.5 - this->getContentSize().height/2;
        }
        
        if (nextPosition.y<124.0f + this->getContentSize().height/2) {
            nextPosition.y = 124.0f + this->getContentSize().height/2;
        }
    } else {
        if (nextPosition.y < _game->getScreenSize().height * 0.5 + this->getContentSize().height/2) {
            nextPosition.y = _game->getScreenSize().height * 0.5 + this->getContentSize().height/2;
        }
        
        if (nextPosition.y> _game->getScreenSize().height - 124.0f - this->getContentSize().height/2) {
            nextPosition.y = _game->getScreenSize().height - 124.0f - this->getContentSize().height/2;
        }
    
    }

    this->getBody()->SetTransform(b2Vec2(nextPosition.x/PTM_RATIO, nextPosition.y/PTM_RATIO), b2_pi/2 );
    if (_body && isVisible()) {
        setPositionX(_body->GetPosition().x * PTM_RATIO);
        setPositionY(_body->GetPosition().y * PTM_RATIO);
    }
}
