#ifndef _STAR_H
#define _STAR_H

//--- Implementation -----------------------------------------------------------
#include"Vector.h"
#include <SFML/Graphics.hpp>

#define G 1000
#define soft 0.1*0.1
#define BLACKHOLE_MASS 300
#define STAR_MASS 1

struct State
{
    Vector pos;
    Vector vel;
    Vector acc;
};

/** \brief Implementation of a single star in the galaxy. */
class Star{
private:
    double mass;
    friend class BHTreeNode;
    friend class Galaxy;
public:
    sf::Vector2f starPosition;
    State state;
    sf::CircleShape shape;
public:
    Star(){};
    Star(State &argstate,double argmass);
    
    // Returns force vector acting on this star due to the argument star
    Vector isPulledby(Star& star);
    void setPos(Vector& new_pos);
    void setVel(Vector& new_Vel);
    void setAcc(Vector& new_acc);
    void setMass(double argmass); 
    void update_state(Vector& new_acc);
};

#endif // _STAR_H