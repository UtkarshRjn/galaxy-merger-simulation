#include <simplecpp>
#include <string>
#include <fstream>
#include "MovingObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include<vector>

using namespace simplecpp;

#define WINDOW_X 1400
#define WINDOW_Y 1000
#define STAR_SIZE 5
#define STEP_TIME 0.05
#define STAR_SPEED 100
#define STAR_ANGLE_DEG 0
#define G 1e7

#define LASSO_G 30
#define STAR_G 30

#define PLAY_Y_HEIGHT (WINDOW_Y-100)

class Star : public MovingObject {
    double mass;
    double star_start_x;
    double star_start_y;
    double release_speed;
    double release_angle_deg;
    double star_ax;
    double star_ay;

    Circle star_circle;

    public:
        Star(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, double x, double y, double mass) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
            this -> mass = mass;
            release_speed = speed;
            release_angle_deg = angle_deg;
            star_ax = argax;
            star_ay = argay;
            initStar(x,y);
        }

        void initStar(double x, double y){
            star_start_x = x;
            star_start_y = y; 
            star_circle.reset(star_start_x, star_start_y, STAR_SIZE);
            star_circle.setColor(COLOR("blue")); 
            star_circle.setFill(true);
            addPart(&star_circle);
        }
        
        double getMass(){return mass;}

};


// class Galaxy : public MovingObject {
//     double mass;
//     double galaxy_start_x;
//     double galaxy_start_y;
//     double release_speed;
//     double release_angle_deg;
//     double galaxy_ax;
//     double galaxy_ay;
//     double radius;
//     std::vector<Star> star_array;

//     public:
//         Galaxy(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, double x, double y, double mass, double radius) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
//             this -> mass = mass;
//             this -> radius = radius;
//             release_speed = speed;
//             release_angle_deg = angle_deg;
//             galaxy_ax = argax;
//             galaxy_ay = argay;
//             initGalaxy(x,y);
//         }

//         void initGalaxy(double x, double y){
//             galaxy_start_x = x;
//             galaxy_start_y = y; 
//             int num = rand()%100;
//             for(int i=0;i<num;i++){
//                 std::default_random_engine generator;
//                 std::normal_distribution<double> distribution(0.0,1.0);
//                 double r = radius*abs(distribution(generator));
//                 double angle_rad = (rand()%360)*PI/180.0;
//                 bool paused = true, rtheta = true;
//                 double star_speed = STAR_SPEED;
//                 double star_angle_deg = STAR_ANGLE_DEG;
//                 double star_ax = 0;
//                 double star_ay = 0; 
//                 star_array.pushback(Star(double speed, double angle_deg, star_ax, star_ay, paused, rtheta, galaxy_start_x + r*cos(angle_rad), galaxy_start_y + r*sin(angle_rad) , rand()%100);
//             }
//         }
// };

void update_acc(Star &star1, Star &star2){
    double distance = sqrt((star1.getXPos()-star2.getXPos())*(star1.getXPos()-star2.getXPos()) + (star1.getYPos()-star2.getYPos())*(star1.getYPos()-star2.getYPos()));
    // cout << distance << endl;
    double ax = G*star2.getMass()/(distance*distance)*(star2.getXPos()-star1.getXPos())/distance;
    double ay = G*star1.getMass()/(distance*distance)*(star2.getYPos()-star1.getYPos())/distance;

    star1.set_ax(ax);
    star1.set_ay(ay);

    star2.set_ax(-ax);
    star2.set_ay(-ay);
}

 
main_program{
    initCanvas("Space",WINDOW_X,WINDOW_Y);
    // repaint(COLOR("black"));
    // wait(5);
    bool paused = true, rtheta = true;
    double star_speed = STAR_SPEED;
    double star_angle_deg = STAR_ANGLE_DEG;
    double star_ax = 0;
    double star_ay = 0;
    Star my_star1(star_speed, star_angle_deg, star_ax, star_ay, paused, rtheta, 0, WINDOW_Y/2 -100, 5);
    Star my_star2(-star_speed, star_angle_deg, -star_ax, star_ay, paused, rtheta, WINDOW_X, WINDOW_Y/2 + 100, 10);

    for(;;){ 
        my_star1.nextStep(STEP_TIME);
        my_star2.nextStep(STEP_TIME);
        update_acc(my_star1,my_star2);
        // cout << my_star1.ax << endl;

        if(my_star1.isPaused()) { my_star1.unpause();}
        if(my_star2.isPaused()) { my_star2.unpause();}

        wait(STEP_TIME);
    }

    wait(2);
}