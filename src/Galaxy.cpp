#include "Galaxy.h"

//--- Implementation -----------------------------------------------------------
#include "Vector.h"

//--- Standard includes --------------------------------------------------------
#include <random>
#include <chrono>
#include <cmath>
#include <iostream>
using namespace std;

Galaxy::Galaxy(int num_stars,double radius, double start_x, double start_y, double start_vx, double start_vy){
    this->num_stars = num_stars;
    this->start_vx = start_vx;
    this->start_vy = start_vy;
    this->start_x = start_x;
    this->start_y = start_y;
    this->radius = radius;
    State state{Vector(start_x,start_y),
            Vector(start_vx,start_vy),
            Vector(0,0)};
    Star blackhole = Star(state,BLACKHOLE_MASS);
    blackhole.shape.setFillColor(sf::Color::Blue);
    blackhole.shape.setRadius(BLACKHOLE_RADIUS);
    star_arr.push_back(blackhole);
    initGalaxy();
}

void Galaxy::initGalaxy(){
    
    double X,Y;
    double strt_x, strt_y;
    double r, rel_speed, angle_rad;
    double vx,vy;
    double ax,ay;
    double rad, theta;
    for(int i=0;i<num_stars;i++){
        unsigned seed = chrono::steady_clock::now().time_since_epoch().count();

        std::default_random_engine generator(seed);
        std::uniform_real_distribution<double> distribution_rad(radius/10,radius);
        std::uniform_real_distribution<double> distribution_theta(0,2*M_PI);
        
        rad = distribution_rad(generator);
        theta = distribution_theta(generator);

        X = rad*cos(theta);
        Y = rad*sin(theta);

        strt_x = this->start_x + X;
        strt_y = this->start_y - Y;
            
        r = sqrt(X*X + Y*Y);
        rel_speed = sqrt(BLACKHOLE_MASS*G/r);
        angle_rad = (X>0) ? atan(-Y/X) : tan(-Y/X) - M_PI ;

        vx = this->start_vx - rel_speed*sin(angle_rad);
        vy = this->start_vy + rel_speed*cos(angle_rad);
        // cout << "vx: " << vx << endl;

        ax = -(rel_speed*rel_speed/r)*cos(angle_rad);
        ay = (rel_speed*rel_speed/r)*sin(angle_rad);
        // cout << "ax: " << ax << endl;

        State state{Vector(strt_x,strt_y),
                    Vector(vx,vy),
                    Vector(ax,ay)};
        Star star = Star(state,STAR_MASS);
        star_arr.push_back(star);      
    }
}