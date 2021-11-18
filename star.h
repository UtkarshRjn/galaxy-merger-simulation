#include <simplecpp>
#include "MovingObject.h"
#include "galaxy.h"

#define WINDOW_X 1400
#define WINDOW_Y 1000
#define STAR_SIZE 2
#define STEP_TIME 0.05
#define GALAXY_SPEED 100
#define GALAXY_ANGLE_DEG 0
#define G 80

#define LASSO_G 30
#define STAR_G 30

class Star : public Sprite {
    double mass;
    double start_x;
    double start_y;
    double ay;
    double vx;
    double vy;
    
    Galaxy galaxy;
    Circle star_circle;

    public:
        double ax;
        Star(Galaxy arggalaxy, double argmass) : Sprite() {
            unsigned seed = chrono::steady_clock::now().time_since_epoch().count(); 
            mass = argmass;
            galaxy = arggalaxy;
            std::default_random_engine generator(seed);
            std::normal_distribution<double> distribution(0.0,1.0);

            double X = distribution(generator)*80;
            double Y = distribution(generator)*80;

            start_x = galaxy.getXPos() + X;
            start_y = galaxy.getYPos() - Y;
                
            double r = sqrt(X*X + Y*Y);
            double rel_speed = sqrt(galaxy.getMass()*G/r);
            double angle_rad = (X>0) ? atan(-Y/X) : tan(-Y/X) - PI ;

            vx = galaxy.getVx() - rel_speed*sin(angle_rad);
            vy = galaxy.getVy() + rel_speed*cos(angle_rad);

            ax = -(rel_speed*rel_speed/r)*cos(angle_rad);
            ay = (rel_speed*rel_speed/r)*sin(angle_rad);

            initStar();
        };
        
        void initStar(void);

        void nextStep(double t);

        void update(void);

        void increase_ax(double argax){ax += argax;}
        void increase_ay(double argay){ay += argay;}

        double getMass(){return mass;}
};