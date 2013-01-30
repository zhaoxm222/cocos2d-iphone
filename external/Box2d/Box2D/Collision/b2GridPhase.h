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
#include <Box2D/Common/b2Draw.h>
#include <Box2D/Common/b2StackAllocator.h>
#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Dynamics/b2Fluid.h>

#define GRID_COLLISION_SIZE 16
#define DEFAULT_GRID_CAPACITY 1000
#define GRID_SIZE (0.25f)
#define b2_nullGrid (0xFFFFFFFF)

struct b2Grid
{
    /// collision index , reference to b2GridPhase collisionData
    //uint32 collisionIdx_;
    bool collision_;
    
    union
    {
        uint32 next_;
        b2Fluid* list_;
    };
};


class b2GridPhase
{
public:
    b2GridPhase( b2Vec2 pos, b2Vec2 size );
    
    void AddCollisionShape( b2Shape* shape, b2Transform xf );
    
    uint32 MoveFluidParticle( b2Fluid* fluid, float t );
    void DelFluidParticle( b2Fluid* fluid);
    
    b2Grid* GetGrid( uint32 gridID );
    
    uint32 GetNearGrid( uint32* grids, uint32 maxCap, b2Vec2 pos, float radius );
    
    bool IsInBlock( const b2Vec2& pos );
    bool IsInBlock( int32 gridX, int32 gridY );
    
    void DrawDebug( b2Draw* draw, b2StackAllocator* stack );
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


inline int32 b2GridPhase::GridX( float x )
{
    return floor( ( x - pos_.x ) / GRID_SIZE );
}

inline int32 b2GridPhase::GridY( float y )
{
    return floor( ( y - pos_.y ) / GRID_SIZE );
}

inline bool b2GridPhase::IsInBlock(const b2Vec2& pos)
{
    uint32 gridID = GridX( pos.x ) + GridY( pos.y ) * nWidth_;
    return grids_[gridID] != b2_nullGrid && gridData_[grids_[gridID]].collision_;
}

inline bool b2GridPhase::IsInBlock(int32 gridX, int32 gridY)
{
    if( gridX < 0 || gridX >= nWidth_ || gridY < 0 || gridY >= nHeight_ )
        return false;
    
    uint32 gridID = gridX + gridY * nWidth_;
    return grids_[gridID] != b2_nullGrid && gridData_[grids_[gridID]].collision_;
}

inline b2Grid* b2GridPhase::GetGrid(uint32 gridID)
{
    if( gridID >= nHeight_ * nWidth_ )
        return NULL;
    
    if( grids_[gridID] == b2_nullGrid )
        return NULL;
    
    return gridData_ + grids_[gridID];
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
