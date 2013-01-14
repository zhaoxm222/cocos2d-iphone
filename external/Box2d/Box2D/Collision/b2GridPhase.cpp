//
//  b2GridPhase.cpp
//  cocos2d-ios
//
//  Created by zhaoxm on 13-1-10.
//
//

#include <Box2D/Collision/b2GridPhase.h>


b2GridPhase::b2GridPhase( uint32 width, uint32 height )
{
    nWidth_ = width;
    nHeight_ = height;
    
    collisionDataSize_ = 0;
    collisionDataCapacity_ = DEFAULT_GRID_CAPACITY * GRID_COLLISION_SIZE * GRID_COLLISION_SIZE;
    collisionData_ = (uint8*)b2Alloc( collisionDataCapacity_ * sizeof( uint8 ) );
    
    grids_ = (uint32*)b2Alloc( nWidth_ * nHeight_ * sizeof(uint32) );
    gridDataSize_ = 0;
    gridDataCapacity_ = DEFAULT_GRID_CAPACITY;
    gridData_ = (b2Grid*)b2Alloc( gridDataCapacity_ * sizeof(b2Grid) );
}


