//
//  b2GridPhase.cpp
//  cocos2d-ios
//
//  Created by zhaoxm on 13-1-10.
//
//

#include <Box2D/Collision/b2GridPhase.h>
#include <memory>

b2GridPhase::b2GridPhase( uint32 width, uint32 height )
{
    nWidth_ = width;
    nHeight_ = height;
    
    //collisionDataSize_ = 0;
    //collisionDataCapacity_ = DEFAULT_GRID_CAPACITY * GRID_COLLISION_SIZE * GRID_COLLISION_SIZE;
    //collisionData_ = (uint8*)b2Alloc( collisionDataCapacity_ * sizeof( uint8 ) );
    
    grids_ = (uint32*)b2Alloc( nWidth_ * nHeight_ * sizeof(uint32) );
    memset( grids_, 0xff, nWidth_ * nHeight_ * sizeof(uint32) );
    
    gridDataSize_ = 0;
    gridDataCapacity_ = DEFAULT_GRID_CAPACITY;
    gridData_ = (b2Grid*)b2Alloc( gridDataCapacity_ * sizeof(b2Grid) );
    memset( gridData_, 0, gridDataCapacity_*sizeof(b2Grid) );
    
    for( int i=0; i<gridDataCapacity_; ++i )
    {
        gridData_[i].next_ = i + 1;
    }
    gridData_[gridDataCapacity_-1].next_ = b2_nullGrid;
    
    freeGrids_ = 0;
    
    pos_.SetZero();
}


void b2GridPhase::AddCollisionShape(b2Shape *shape, b2Transform tf)
{
    // Create proxies in the broad-phase.
	int nChild = shape->GetChildCount();
    
    b2AABB aabb;
	for (int32 i = 0; i < nChild; ++i)
	{
		//shape->ComputeAABB( &aabb, tf, i);
	}
}


uint32 b2GridPhase::AllocateGrid()
{
    if( freeGrids_ == b2_nullGrid )
    {
        b2Grid* oldData = gridData_;
        
        gridDataCapacity_ *= 2;
        gridData_ = (b2Grid*)b2Alloc( gridDataCapacity_ * sizeof(b2Grid) );
        
        memcpy(gridData_, oldData, gridDataSize_*sizeof(b2Grid));
        for( int i = gridDataSize_; i < gridDataCapacity_; ++i )
        {
            gridData_[i].next_ = i + 1;
        }
        gridData_[gridDataCapacity_ - 1].next_ = b2_nullGrid;
        
        freeGrids_ = gridDataSize_;
    }
    
    int ret = freeGrids_;
    freeGrids_ = gridData_[freeGrids_].next_;
    
    ++ gridDataSize_;
    
    return ret;
}


void b2GridPhase::DrawLine(b2Vec2 s, b2Vec2 e)
{
    b2Vec2 delt = e - s;
    
    int startX = GridX(s.x);
    int startY = GridY(s.y);
    
    int endX = GridX(e.x);
    int endY = GridY(e.y);
    
    if( delt.LengthSquared() < b2_epsilon )
    {
        DrawPixel( startX, startY );
        return;
    }
    
    if( abs(delt.x) > abs(delt.y) )
    {
        float slop = delt.y / delt.x;
        int step = delt.x > 0 ?  1 : -1;
        for( int i = startX; i <= endX; i += step )
        {
            DrawPixel( i, GridY( s.y + slop ));
        }
    }
    else
    {
        float slop = delt.x / delt.y;
        int step = delt.y > 0 ? 1 : -1;
        for( int i = startY; i <= endY; i += step )
        {
            DrawPixel(GridX(s.x + slop), i);
        }
    }
    
}