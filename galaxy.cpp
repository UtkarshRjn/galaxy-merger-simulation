//Utkarsh Ranjan
//200050147

#include <simplecpp>
#include "Galaxy.h"

using namespace simplecpp;

void Galaxy::nextStep(double t) {
  // cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  // cerr << "vx=" << vx << ",vy=" << vy << endl;
  // cerr << "ax=" << ax << ",ay=" << ay << endl;
  //cout << sqrt(vx*vx + vy*vy) << endl;
  move(vx*t, vy*t);
  vx += ax*t;
  vy += ay*t;
} // End Galaxy::nextStep()

double Galaxy::getXPos() {
  return getX();
}

double Galaxy::getYPos() {
  return getY();
}

double Galaxy::getVx() {
  return vx;
}

double Galaxy::getVy(){
  return vy;
}

double Galaxy::getMass() {
  return mass;
}