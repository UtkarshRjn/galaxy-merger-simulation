#include"Star.h"

//--- Implementation -----------------------------------------------------------
#include"Vector.h"

//--- Standard includes --------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

Star::Star(State &argstate,double argmass){
    state = argstate;
    mass = argmass;    
    shape.setRadius(2.0f);
    shape.setFillColor(sf::Color::White);
    sf::Vector2f starPosition(state.pos.getX(),state.pos.getY());
    shape.setPosition(starPosition);
}

Vector Star::isPulledby(Star& star){
    Vector acc = Vector(0,0);

    if(&star == this){return acc;}

    double x1 = state.pos.getX();
    double y1 = state.pos.getY();
    double x2 = star.state.pos.getX();
    double y2 = star.state.pos.getY();
    
    double r = sqrt((x1-y2)*(x1-x2)+ (y1-y2)*(y1-y2));

    if(r>0){
        double f = G * star.mass / (r*r*r) ; // Not multiplying by mass of this star because we are concerned about acc

        acc.setX(f*(x2-x1));
        acc.setY(f*(y2-y1)); 
    }else{
        acc.setX(0);
        acc.setY(0);
    }

    return acc;

};

Vector Star::update_state(Vector& new_acc){
    state.pos = state.pos + state.vel;
    state.vel = state.vel + state.acc;
    state.acc = new_acc;

    cout << "x=" << state.pos.getX() << ",y=" << state.pos.getY() << endl;
    cout << "vx=" << state.vel.getX() << ",vy=" << state.vel.getY() << endl;
    cout << "ax=" << state.acc.getX() << ",ay=" << state.acc.getY() << endl;

    return state.pos;
}

void Star::setPos(Vector& new_pos){
    state.pos = new_pos;
}
void Star::setVel(Vector& new_vel){
    state.vel = new_vel;
}   
void Star::setAcc(Vector& new_acc){
    state.acc = new_acc;
}
void Star::setMass(double argmass){
    mass = argmass;
}