//Utkarsh Ranjan
//200050147

#include <simplecpp>
#include "MovingObject.h"

using namespace simplecpp;

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  // cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  // cerr << "vx=" << vx << ",vy=" << vy << endl;
  // cerr << "ax=" << ax << ",ay=" << ay << endl;
  //cout << sqrt(vx*vx + vy*vy) << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}