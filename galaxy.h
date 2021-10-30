// #ifndef _MOVINGOBJECT_INCLUDED_
// #define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

#define WINDOW_X 1400
#define WINDOW_Y 1000
#define HOLE_SIZE 5

using namespace simplecpp;

class Galaxy : public Sprite {
  double vx, vy; 
  double mass;
  Circle blackhole;
  void initGalaxy(double argvx, double argvy, double argax, double argay, double argmass) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; mass = argmass;
  } 
 public:
 double ax, ay; 
 Galaxy():Sprite(){};
 Galaxy(double argvx, double argvy, double argax, double argay, double argmass, double x, double y)
    : Sprite() {
    blackhole.reset(x, y, HOLE_SIZE);
    blackhole.setColor(COLOR("grey")); 
    blackhole.setFill(true);
    initGalaxy(argvx, argvy, argax, argay, argmass);
    this->move(x,y);
  }
 Galaxy(double speed, double angle_deg, double argax, double argay, double argmass, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initGalaxy(argvx, argvy, argax, argay, argmass);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  double getMass();
  double getVx();
  double getVy();

  void nextStep(double t);
}; // End Class Galaxy
// #endif
