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
#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Dynamics/b2Body.h>

#define GRID_COLLISION_SIZE 16
#define DEFAULT_GRID_CAPACITY 1000
#define GRID_SIZE (0.25f)
#define b2_nullGrid (0xFFFF)

struct b2Grid
{
    
    void AddParticle( b2Body* body );
    void DelParticle( b2Body* body );
    
    
    /// collision index , reference to b2GridPhase collisionData
    //uint32 collisionIdx_;
    bool collision_;
    
    /// user data
    void* userData_;
    
    union
    {
        uint32 next_;
        b2Body* body_;
    };
};


class b2GridPhase
{
public:
    b2GridPhase( uint32 width, uint32 height );
    
    void SetPosition( b2Vec2& pos );
    
    void AddCollisionShape( b2Shape* shape, b2Transform xf );
    
    uint32 AddParticleBody( b2Body* body, b2Vec2 pos );
    b2Vec2 GetGridForce( b2Body* body, uint32& grid, b2Vec2 newPos );
    
private:
    
    uint32 AllocateGrid();
    
    int32 GridX( float x );
    int32 GridY( float y );
    
    void DrawPixel( int32 x, int32 y );
    void DrawLine( b2Vec2 s, b2Vec2 e );
    void DrawTrigle( b2Vec2 v1, b2Vec2 v2, b2Vec2 v3 );
    
    /// grid phase left bottom pos
    b2Vec2 pos_;
    
    /// grid phase area
    uint32 nWidth_, nHeight_;
    
    /// collision data
    //uint8* collisionData_;
    //uint32 collisionDataSize_;
    //uint32 collisionDataCapacity_;

    /// grids matrix
    uint32* grids_;
    b2Grid* gridData_;
    uint32 gridDataSize_;
    uint32 gridDataCapacity_;
    
    uint32 freeGrids_;
};


inline void b2Grid::AddParticle(b2Body *body)
{
    
}

inline void b2Grid::DelParticle(b2Body *body)
{
    
}


inline void b2GridPhase::SetPosition( b2Vec2& pos )
{
    pos_ = pos;
}

inline int32 b2GridPhase::GridX( float x )
{
    return floor( ( x - pos_.x ) / GRID_SIZE );
}

inline int32 b2GridPhase::GridY( float y )
{
    return floor( ( y - pos_.y ) / GRID_SIZE );
}

inline void b2GridPhase::DrawPixel(int32 x, int32 y)
{
    if( x < 0 || x >= nWidth_ || y < 0 || y >= nHeight_ )
        return;
    
    uint32 index = y * nWidth_ + x;
    if( grids_[index] == b2_nullGrid )
        grids_[index] = AllocateGrid();
    
    gridData_[grids_[index]].collision_ = true;
}

#endif /* defined(B2_GRID_PHASE_H) */
