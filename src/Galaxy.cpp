#include "Galaxy.h"
#include "Vector.h"

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
    star_arr.push_back(blackhole);
    initGalaxy();
}

void Galaxy::initGalaxy(){
    
    double X,Y;
    double strt_x, strt_y;
    double r, rel_speed, angle_rad;
    double vx,vy;
    double ax,ay;
    for(int i=0;i<num_stars;i++){
        unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::normal_distribution<double> distribution(0.0,1.0);

        X = distribution(generator)*radius;
        Y = distribution(generator)*radius;

        strt_x = this->start_x + X;
        strt_y = this->start_y - Y;
            
        r = sqrt(X*X + Y*Y);
        rel_speed = sqrt(BLACKHOLE_MASS*G/r);
        angle_rad = (X>0) ? atan(-Y/X) : tan(-Y/X) - M_PI ;

        vx = this->start_vx - rel_speed*sin(angle_rad);
        vy = this->start_vy + rel_speed*cos(angle_rad);
        // cout << vx << endl;

        ax = -(rel_speed*rel_speed/r)*cos(angle_rad);
        ay = (rel_speed*rel_speed/r)*sin(angle_rad);

        State state{Vector(strt_x,strt_y),
                    Vector(vx/18,vy/18),
                    Vector(0,0)};
        Star star = Star(state,STAR_MASS);
        star_arr.push_back(star);      
    }
}