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
    
    collisionData_ = (uint8*)b2Alloc( DEFAULT_GRID_COLLISION_COUNT * GRID_COLLISION_SIZE * GRID_COLLISION_SIZE * sizeof( uint8 ) );
    grids_ = (uint32*)b2Alloc( nWidth_ * nHeight_ * sizeof(uint32) );
    gridData_ = NULL;
    gridDataSize_ = 0;
}
