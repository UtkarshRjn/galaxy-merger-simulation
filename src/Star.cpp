#include"Star.h"

//--- Implementation -----------------------------------------------------------
#include"Vector.h"
#include"BHTree.h"

//--- Standard includes --------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <sstream>

Vector Star::isPulledby(Star& star){
    Vector acc;

    if(&star == this){return acc;}

    double x1 = state.pos.getX();
    double y1 = state.pos.getY();
    double x2 = star.state.pos.getX();
    double y2 = star.state.pos.getY();
    
    double r = sqrt((x1-y2)*(x1-x2)+ (y1-y2)*(y1-y2));

    if(r>0){
        double f = G * star.mass / (r*r*r) ; // Not multiplying by mass of this star because we are concerned about acc

    }

};

Vector Star::isPulledby(BHTreeNode *node){
    
};