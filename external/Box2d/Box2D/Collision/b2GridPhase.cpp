//
//  b2GridPhase.cpp
//  cocos2d-ios
//
//  Created by zhaoxm on 13-1-10.
//
//

#include <memory>

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Collision/b2GridPhase.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2EdgeShape.h>
#include <Box2D/Collision/Shapes/b2ChainShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>


b2GridPhase::b2GridPhase( b2Vec2 pos, b2Vec2 size )
{
    nWidth_ = ceil( size.x / GRID_SIZE );
    nHeight_ = ceil( size.y / GRID_SIZE );
    pos_ = pos;
    
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
}


void b2GridPhase::AddCollisionShape(b2Shape *shape, b2Transform xf)
{
    switch (shape->GetType())
	{
        case b2Shape::e_circle:
		{
			//b2CircleShape* circle = (b2CircleShape*)shape;
            
			//b2Vec2 center = b2Mul(xf, circle->m_p);
			//float32 radius = circle->m_radius;
			//b2Vec2 axis = b2Mul(xf.q, b2Vec2(1.0f, 0.0f));
            
			//DrawSolidCircle(center, radius, axis, color);
		}
            break;
            
        case b2Shape::e_edge:
		{
			b2EdgeShape* edge = (b2EdgeShape*)shape;
			b2Vec2 v1 = b2Mul(xf, edge->m_vertex1);
			b2Vec2 v2 = b2Mul(xf, edge->m_vertex2);
			DrawLine(v1, v2);
		}
            break;
            
        case b2Shape::e_chain:
		{
			b2ChainShape* chain = (b2ChainShape*)shape;
			int32 count = chain->m_count;
			const b2Vec2* vertices = chain->m_vertices;
            
			b2Vec2 v1 = b2Mul(xf, vertices[0]);
			for (int32 i = 1; i < count; ++i)
			{
				b2Vec2 v2 = b2Mul(xf, vertices[i]);
				DrawLine(v1, v2);
				//DrawCircle(v1, 0.05f, color);
				v1 = v2;
			}
		}
            break;
            
        case b2Shape::e_polygon:
		{
			b2PolygonShape* poly = (b2PolygonShape*)shape;
			int32 vertexCount = poly->m_vertexCount;
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			b2Vec2 vertices[b2_maxPolygonVertices];
            
			for (int32 i = 0; i < vertexCount; ++i)
			{
				vertices[i] = b2Mul(xf, poly->m_vertices[i]);
			}
            
			//DrawSolidPolygon(vertices, vertexCount, color);
		}
            break;
            
        default:
            break;
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
            gridData_[i].collision_ = false;
            gridData_[i].next_ = i + 1;
        }
        gridData_[gridDataCapacity_ - 1].next_ = b2_nullGrid;
        
        freeGrids_ = gridDataSize_;
    }
    
    uint32 ret = freeGrids_;
    freeGrids_ = gridData_[freeGrids_].next_;
    ++ gridDataSize_;
    
    gridData_[ret].list_ = NULL;
    
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
        float slop = delt.y * GRID_SIZE / delt.x;
        int step = delt.x > 0 ?  1 : -1;
        int i = startX;
        while( i != endX )
        {
            s.y += slop;
            DrawPixel( i, GridY( s.y ));
            i += step;
        }
    }
    else
    {
        float slop = delt.x * GRID_SIZE / delt.y;
        int step = delt.y > 0 ? 1 : -1;
        int i = startY;
        while( i != endY )
        {
            s.x += slop;
            DrawPixel( GridX(s.x), i);
            i += step;
        }
    }
    
}


uint32 b2GridPhase::MoveFluidParticle(b2Fluid* fluid, float t)
{
    b2Vec2 oldPos = fluid->body_->m_sweep.c;
    int32 oldGridX = GridX( oldPos.x );
    int32 oldGridY = GridY( oldPos.y );
    uint32 oldGridID = oldGridX + oldGridY * nWidth_;
    
    // if in block
    if( grids_[oldGridID] != b2_nullGrid && gridData_[grids_[oldGridID]].collision_ )
    {
       // find nearest unblock grid
        
    }
    
    
    
    // start at safe pos
    b2Vec2 newPos = oldPos;
    b2Vec2 speed = fluid->body_->m_linearVelocity;
    
    int32 newGridX = GridX( newPos.x );
    int32 newGridY = GridY( newPos.y );
    
    while( t > 0 )
    {
        float tx = b2_maxFloat;
        if( speed.x > b2_epsilon )
        {
            tx = ( ( newGridX + 1 ) * GRID_SIZE + pos_.x - newPos.x ) / speed.x;
        }
        else if( speed.x < -b2_epsilon )
        {
            tx = ( newGridX * GRID_SIZE + pos_.x - newPos.x ) / speed.x;
        }
        
        float ty = b2_maxFloat;
        if( speed.y > b2_epsilon )
        {
            ty = ( ( newGridY + 1 ) * GRID_SIZE + pos_.y - newPos.y ) / speed.y;
        }
        else if( speed.y < -b2_epsilon )
        {
            ty = ( newGridY * GRID_SIZE + pos_.y - newPos.y ) / speed.y;
        }
        
        float minT = b2Min( tx, ty);
        if( t <= minT )
        {
            newPos += t * speed;
            break;
        }
        
        if( tx < ty )
        {
            newPos += tx * speed;
            t -= tx;
            
            if( speed.x > 0 )
            {
                if( IsInBlock( newGridX + 1, newGridY) )
                    speed.x *= -0.2f;
                else
                    ++newGridX;
            }
            else
            {
                if( IsInBlock( newGridX - 1, newGridY))
                    speed.x *= -0.2f;
                else
                    --newGridX;
            }
        }
        else
        {
            newPos += ty * speed;
            t -= ty;
            
            if( speed.y > 0 )
            {
                if( IsInBlock( newGridX, newGridY + 1) )
                    speed.y *= -0.2f;
                else
                    ++newGridY;
            }
            else
            {
                if( IsInBlock( newGridX, newGridY -1 ) )
                    speed.y *= -0.2f;
                else
                    --newGridY;
            }
        }
    }
    
    
    fluid->body_->m_linearVelocity = speed;
    fluid->body_->m_sweep.c = newPos;
    
    
    // new grid
    newGridX = GridX(newPos.x);
    newGridY = GridY(newPos.y);
        
    uint32 newGridID = newGridX + newGridY * nWidth_;
    if( newGridX < 0 || newGridX >= nWidth_ || newGridY < 0 || newGridY >= nHeight_ )
        newGridID = b2_nullGrid;
    
    // remove old grid
    if( fluid->gridID_ != b2_nullGrid && fluid->gridID_ != newGridID )
    {
        //remove from old grid
        b2Grid& oldGrid = gridData_[ grids_[fluid->gridID_] ];
        if( oldGrid.list_ == fluid )
        {
            oldGrid.list_ = fluid->next_;
            if( fluid->next_ )
                fluid->next_->prev_ = NULL;
        }
        else
        {
            fluid->prev_->next_ = fluid->next_;
            if( fluid->next_)
                fluid->next_->prev_ = fluid->prev_;
        }
        
        fluid->gridID_ = b2_nullGrid;
    }
    
    
    // add to new grid
    if( newGridID != b2_nullGrid && fluid->gridID_ != newGridID )
    {
        if( grids_[newGridID] == b2_nullGrid )
            grids_[newGridID] = AllocateGrid();

        //add to new grid
        b2Grid& newGrid = gridData_[ grids_[newGridID] ];
        
        if( newGrid.list_ == NULL )
        {
            newGrid.list_ = fluid;
            fluid->prev_ = NULL;
            fluid->next_ = NULL;
        }
        else
        {
            fluid->prev_ = NULL;
            fluid->next_ = newGrid.list_;
            newGrid.list_->prev_ = fluid;
            newGrid.list_ = fluid;
        }
    }
    
    fluid->gridID_ = newGridID;
    return newGridID;
}


uint32 b2GridPhase::GetNearGrid(uint32 *grids, uint32 maxCap, b2Vec2 pos, float radius)
{
    int32 minX = GridX( pos.x - radius );
    int32 minY = GridY( pos.y - radius );
    int32 maxX = GridX( pos.x + radius );
    int32 maxY = GridY( pos.y + radius );
    
    if( minX < 0 ) minX = 0;
    if( minX >= nWidth_ ) return 0;
    
    if( maxX < 0 ) return 0;
    if( maxX >= nWidth_ ) maxX = nWidth_ - 1;
    
    if( minY < 0 ) minY = 0;
    if( minY >= nHeight_ ) return 0;
    
    if( maxY < 0 ) return 0;
    if( maxY >= nHeight_ ) maxY = nHeight_ - 1;
    
    uint32 count = 0;
    for( int32 x = minX; x <= maxX; ++x )
    {
        for( int32 y = minY; y <= maxY; ++y )
        {
            grids[ count++ ] = x + y * nWidth_;
            if( count == maxCap )
                return count;
        }
    }
    
    return count;
}

void b2GridPhase::DrawDebug(b2Draw *draw, b2StackAllocator* stack)
{
    uint32 maxVertice = 4 * 1024;
    b2Vec2 verts[4];
    
    for( int i = 0; i < nWidth_; ++i )
    {
        for( int j = 0; j < nHeight_; ++j )
        {
            uint32 gridID = i + j * nWidth_;
            if( grids_[gridID] == b2_nullGrid || gridData_[ grids_[gridID] ].collision_ == false )
                continue;
            
            int currVertice = 0;
            verts[ currVertice ] = pos_;
            verts[ currVertice ].x += i * GRID_SIZE;
            verts[ currVertice ].y += j * GRID_SIZE;
            currVertice++;
            
            verts[ currVertice ] = pos_;
            verts[ currVertice ].x += i * GRID_SIZE + GRID_SIZE;
            verts[ currVertice ].y += j * GRID_SIZE;
            currVertice++;
            
            verts[ currVertice ] = pos_;
            verts[ currVertice ].x += i * GRID_SIZE + GRID_SIZE;
            verts[ currVertice ].y += j * GRID_SIZE + GRID_SIZE;
            currVertice++;

            verts[ currVertice ] = pos_;
            verts[ currVertice ].x += i * GRID_SIZE;
            verts[ currVertice ].y += j * GRID_SIZE + GRID_SIZE;
            currVertice++;
            
            draw->DrawPolygon(verts, 4, b2Color(1, 1, 1));
            
            if( currVertice >= maxVertice - 1 )
                break;
        }
    }
}



