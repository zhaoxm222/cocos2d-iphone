//
//  b2Fluid.cpp
//  cocos2d-ios
//
//  Created by zhaoxm on 13-1-20.
//
//



#include <Box2D/Dynamics/b2Fluid.h>
#include <Box2D/Collision/b2GridPhase.h>

b2FluidParam gFluidParam;


b2Fluid::b2Fluid( b2Body* body )
{
    body_ = body;
    pressure_ = 0.f;
    density_ = 0.f;
    force_.SetZero();
    
    prev_ = NULL;
    next_ = NULL;
    
    gridID_ = b2_nullGrid;
}

