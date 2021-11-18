#include <simplecpp>
#include <deque>
#include <random>
#include <chrono>
#include "star.h"

void Star::initStar(){           
    star_circle.reset(start_x, start_y, STAR_SIZE);
    star_circle.setColor(COLOR("white")); 
    star_circle.setFill(true);
}

void Star::nextStep(double t) {
    // cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
    // cerr << "vx=" << vx << ",vy=" << vy << endl;
    // cerr << "ax=" << ax << ",ay=" << ay << endl;
    
    star_circle.move(vx*t, vy*t);
    vx += ax*t;
    vy += ay*t;
} // End Galaxy::nextStep()

void Star::update(){
    double X = star_circle.getX() - galaxy.getXPos();
    double Y = star_circle.getY() - galaxy.getYPos();

    double r = sqrt(X*X + Y*Y);
    double rel_speed = sqrt(galaxy.getMass()*G/r);
    double angle_rad = (X>0) ? atan(-Y/X) : atan(-Y/X) - PI ; 

    ax = -(rel_speed*rel_speed/r)*cos(angle_rad);
    ay = (rel_speed*rel_speed/r)*sin(angle_rad);
}