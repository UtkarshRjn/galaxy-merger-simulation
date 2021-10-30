#include <simplecpp>
#include <vector>
#include <random>
#include <chrono>
#include "MovingObject.h"
#include "Galaxy.h"

using namespace simplecpp;

#define WINDOW_X 1400
#define WINDOW_Y 1000
#define STAR_SIZE 5
#define STEP_TIME 0.05
#define GALAXY_SPEED 100
#define GALAXY_ANGLE_DEG 0
#define G 100

#define LASSO_G 30
#define STAR_G 30

#define PLAY_Y_HEIGHT (WINDOW_Y-100)

class Star : public Sprite {
    double mass;
    double start_x;
    double start_y;
    double ax;
    double ay;
    double vx;
    double vy;
    
    Galaxy galaxy;
    Circle star_circle;

    public:
        Star(Galaxy arggalaxy, double argmass) : Sprite() { 
             unsigned seed = chrono::steady_clock::now().time_since_epoch().count(); 
             mass = argmass;
             galaxy = arggalaxy;
             std::default_random_engine generator(seed);
             std::normal_distribution<double> distribution(0.0,1.0);

             double X = distribution(generator)*100;
             double Y = distribution(generator)*100;
             start_x = galaxy.getX() + X;
             start_y = galaxy.getY() + Y;
             
            //  cout << galaxy.getX() << endl;
            //  cout << X << endl;

             double r = sqrt(X*X + Y*Y);
             double rel_speed = sqrt(mass*G/r);
             double angle_rad = atan(Y/X);
             
             vx = galaxy.getVx() - rel_speed*sin(angle_rad);
             vy = galaxy.getVy() + rel_speed*cos(angle_rad);

            //  cout << vx << endl;

             ax = -(mass*rel_speed*rel_speed/r)*cos(angle_rad);
             ay = -(mass*rel_speed*rel_speed/r)*sin(angle_rad);

             initStar();

        }
        
        void initStar(){
            star_circle.reset(start_x, start_y, STAR_SIZE);
            star_circle.setColor(COLOR("blue")); 
            star_circle.setFill(true);
        }

        void nextStep(double t) {
        // cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
        // cerr << "vx=" << vx << ",vy=" << vy << endl;
        // cerr << "ax=" << ax << ",ay=" << ay << endl;
        //cout << sqrt(vx*vx + vy*vy) << endl;
            star_circle.move(vx*t, vy*t);
            vx += ax*t;
            vy += ay*t;
        } // End Galaxy::nextStep()
};

 
main_program{
    srand(time(0));
    initCanvas("Space",WINDOW_X,WINDOW_Y);
    // repaint(COLOR("black"));
    // wait(5);
    Galaxy milkyway(GALAXY_SPEED,GALAXY_ANGLE_DEG,0,0,1000000,0,WINDOW_Y/2-100);
    // Galaxy andromeda(-GALAXY_SPEED,GALAXY_ANGLE_DEG,0,0,1000000,WINDOW_X,WINDOW_Y/2+100);
    vector<Star> star_array;
    
    for(int i=0;i<50;i++) star_array.push_back(Star(milkyway,rand()%10));
    // for(int i=0;i<50;i++) star_array.push_back(Star(andromeda,rand()%100));

    for(;;){ 
        for(int i=0;i<star_array.size();i++) star_array[i].nextStep(STEP_TIME);
        // cout << "yeah" << endl;

        wait(STEP_TIME);
    }

    wait(2);
}