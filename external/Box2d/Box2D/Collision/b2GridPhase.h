//
//  b2GridPhase.h
//  cocos2d-ios
//
//  Created by zhaoxm on 13-1-10.
//
//

#ifndef B2_GRID_PHASE_H
#define B2_GRID_PHASE_H

#include <Box2D/Common/b2Math.h>

class b2GridPhase
{
public:
    b2GridPhase( float width, float height, float gridSize, int gridCells );
    
    
private:
    
    b2Vec2  m_position;     // grid phase left top pos
    
    float m_width;          // grid area , in meters
    float m_height;
    int m_nWidth;           // grid area , in grids
    int m_nHeight;
    
    float m_gridSize;       // grid size , in meters
    
    int m_gridCells;        // split grid by cells
    float m_cellSize;       
}


#endif /* defined(B2_GRID_PHASE_H) */
