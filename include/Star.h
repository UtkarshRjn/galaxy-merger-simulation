#ifndef _STAR_H
#define _STAR_H

//--- Implementation -----------------------------------------------------------
#include"Vector.h"

#define G 100
#define soft 0.1*0.1

struct State
{
    Vector pos;
    Vector vel;
    Vector acc;
};

/** \brief Implementation of a single star in the galaxy. */
class Star{
private:
    State state;
    double mass;
    friend class BHTreeNode;
public:
    // Returns force vector acting on this star due to the argument star
    Vector isPulledby(Star& star); 
    
    // Returns force vector acting on this star due to the argument node (with position at com and mass as total mass)
    Vector isPulledby(BHTreeNode *node); 
};

#endif // _STAR_H