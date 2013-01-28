//
//  b2Fluid.h
//  cocos2d-ios
//
//  Created by zhaoxm on 13-1-20.
//
//

#ifndef B2_FLUID_H
#define B2_FLUID_H

#include <Box2D/Common/b2Math.h>

class b2Body;

struct b2FluidParam
{
    b2FluidParam()
    {
        viscosity       =   0.8f;
        pMass           =   0.00020543;
        pRadius         =   0.004;
        pRestDensity    =   1000;
        pDist           =   pow ( pMass / pRestDensity, 1/3.0 );
        smoothRadius    =   0.01f;
        smoothRadius2   =   smoothRadius * smoothRadius;
        intStiff        =   1.0f;
        extStiff        =   10000.f;
        limit           =   20.f;
        limit2          =   limit * limit;
        simScale        =   0.02;
        simScaleI       =   1.f / simScale;
        realRadius      =   smoothRadius * simScaleI;
        
        
        poly6Kern = 315.0f / (64.0f * 3.141592 * pow( smoothRadius, 9) );	// Wpoly6 kernel (denominator
        spikyKern = -45.0f / (3.141592 * pow( smoothRadius, 6) );			// Laplacian of viscocity (denominator): PI h^6
        lapKern = 45.0f / (3.141592 * pow( smoothRadius, 6) );
    }
    
    float viscosity;
    float pMass;
    float pRadius;
    float pRestDensity;
    float pDist;
    float smoothRadius;
    float smoothRadius2;
    float intStiff;
    float extStiff;
    float limit;
    float limit2;
    float simScale;
    float simScaleI;
    float realRadius;
    
    float poly6Kern;
    float spikyKern;
    float lapKern;
};

extern b2FluidParam gFluidParam;

struct b2Fluid
{
    b2Fluid( b2Body* body );
    
    b2Body* body_;
    
    // SPH pressure
    float pressure_;
    float density_;
    b2Vec2 force_;
    
    // linked as a list in gridPhase
    b2Fluid* prev_;
    b2Fluid* next_;
    
    uint32 gridID_;
};

#endif /* defined(B2_FLUID_H) */
