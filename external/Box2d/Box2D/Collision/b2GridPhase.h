//
//  b2GridPhase.h
//  cocos2d-ios
//
//  Created by zhaoxm on 13-1-10.
//
//

#ifndef B2_GRID_PHASE_H
#define B2_GRID_PHASE_H

#include <Box2D/Common/b2Settings.h>
#include <Box2D/Common/b2Math.h>

#define GRID_COLLISION_SIZE 16
#define DEFAULT_GRID_COLLISION_COUNT 1000

struct b2Grid
{
    /// collision index , reference to b2GridPhase collisionData
    uint32 collisionIdx_;
    
    /// next grid, linked as a shape
    b2Grid* next_;
    
    /// user data
    void*  userData;
};


class b2GridPhase
{
public:
    b2GridPhase( uint32 width, uint32 height );
    
    void SetPosition( b2Vec2& pos );
    
private:
    /// grid phase left top pos
    b2Vec2 pos_;
    
    /// grid phase area
    uint32 nWidth_, nHeight_;
    
    /// collision data
    uint8* collisionData_;

    /// grids matrix
    uint32* grids_;
    b2Grid* gridData_;
    uint32 gridDataSize_;
};


inline void b2GridPhase::SetPosition( b2Vec2& pos )
{
    pos_ = pos;
}


#endif /* defined(B2_GRID_PHASE_H) */
