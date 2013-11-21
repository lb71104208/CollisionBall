//
//  Ball.h
//  MiniPool
//
//  Created by Roger Engelbert on 1/13/13.
//
//

#ifndef __MiniPool__Ball__
#define __MiniPool__Ball__


#include "b2Sprite.h"

class Ball : public b2Sprite {

public:
    ~Ball();
    Ball(HelloWorld * game, int type, CCPoint position);
    static Ball* create(HelloWorld * game, int type, CCPoint position);
    virtual void reset(void);
    virtual void update (float dt);
private:
        CCPoint _startPosition;
protected:
    void initBallWithFile(const char* pszFilename);
    
};

#endif /* defined(__MiniPool__Ball__) */
