#include <simplecpp>
#include <deque>
#include <random>
#include <chrono>
#include "MovingObject.h"
#include "galaxy.h"

using namespace simplecpp;

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

        }
        
        void initStar(){           
            star_circle.reset(start_x, start_y, STAR_SIZE);
            star_circle.setColor(COLOR("white")); 
            star_circle.setFill(true);
        }

        void nextStep(double t) {
            // cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
            // cerr << "vx=" << vx << ",vy=" << vy << endl;
            // cerr << "ax=" << ax << ",ay=" << ay << endl;
            
            star_circle.move(vx*t, vy*t);
            vx += ax*t;
            vy += ay*t;
        } // End Galaxy::nextStep()

        void update(){
            double X = star_circle.getX() - galaxy.getXPos();
            double Y = star_circle.getY() - galaxy.getYPos();

            double r = sqrt(X*X + Y*Y);
            double rel_speed = sqrt(galaxy.getMass()*G/r);
            double angle_rad = (X>0) ? atan(-Y/X) : atan(-Y/X) - PI ; 

            ax = -(rel_speed*rel_speed/r)*cos(angle_rad);
            ay = (rel_speed*rel_speed/r)*sin(angle_rad);
        }

        void increase_ax(double argax){ax += argax;}
        void increase_ay(double argay){ay += argay;}

        double getMass(){return mass;}
};

void update_acc(deque<Star> &star_array){
    for(size_t i=0;i<star_array.size();i++){
        star_array[i].update();
    }
}

void update_acc_galaxy(Galaxy &galaxy1 , Galaxy &galaxy2){
    double distance = sqrt((galaxy1.getXPos()-galaxy2.getXPos())*(galaxy1.getXPos()-galaxy2.getXPos()) + (galaxy1.getYPos()-galaxy2.getYPos())*(galaxy1.getYPos()-galaxy2.getYPos()));
    double ax = 20*G*galaxy2.getMass()/(distance*distance)*(galaxy2.getXPos()-galaxy1.getXPos())/distance;
    double ay = 20*G*galaxy1.getMass()/(distance*distance)*(galaxy2.getYPos()-galaxy1.getYPos())/distance;

    galaxy1.set_ax(ax);
    galaxy1.set_ay(ay);

    galaxy2.set_ax(-ax);
    galaxy2.set_ay(-ay);
}

void mutual_acc(Star &star1, Star &star2){
    double distance = sqrt((star1.getX()-star2.getX())*(star1.getX()-star2.getX()) + (star1.getY()-star2.getY())*(star1.getY()-star2.getY()));

    double ax,ay;
    if(distance > 0){
        ax = G*star2.getMass()/(distance*distance)*(star2.getX()-star1.getX())/distance;
        ay = G*star1.getMass()/(distance*distance)*(star2.getY()-star1.getY())/distance;
    }else{
        ax = 0;
        ay = 0;
    }   

    star1.increase_ax(ax);
    star1.increase_ay(ay);

    star2.increase_ax(-ax);
    star2.increase_ay(-ay);
}

void grow(deque<Star> &star_array , Galaxy &galaxy1, Galaxy &galaxy2){
    for(int i=0;i<10;i++) {
        star_array.pop_front();
        star_array.pop_front();
        star_array.push_back(Star(galaxy1,500));
        star_array.push_back(Star(galaxy2,500));
    }
}

main_program{
    srand(time(0));
    initCanvas("Space",WINDOW_X,WINDOW_Y);
    Rectangle canvas(WINDOW_X/2,WINDOW_Y/2,WINDOW_X,WINDOW_Y);
    canvas.setColor(COLOR("BLACK"));
    canvas.setFill(true);

    Galaxy milkyway(GALAXY_SPEED,GALAXY_ANGLE_DEG,0,0,2000,WINDOW_X/2-400,WINDOW_Y/2-100);
    Galaxy andromeda(-GALAXY_SPEED,GALAXY_ANGLE_DEG,0,0,2000,WINDOW_X/2+400,WINDOW_Y/2+100);
    deque<Star> star_array;
    
    for(size_t i=0;i<200;i++) {
        if(rand()%2==0) star_array.push_back(Star(milkyway,500));
        else  star_array.push_back(Star(andromeda,500));
    }

    for(int t=0;t<150;t++){ 

        for(size_t i=0;i<star_array.size();i++) star_array[i].nextStep(STEP_TIME);
        milkyway.nextStep(STEP_TIME);
        andromeda.nextStep(STEP_TIME);
        
        update_acc(star_array);
        update_acc_galaxy(milkyway,andromeda);
        if(t%5 == 0 && (t<50 || t>60)) grow(star_array, milkyway, andromeda);
        for(size_t i=0;i<star_array.size();i++){
            for(size_t j=0;j<star_array.size();j++){
                if(i==j) continue;
                else mutual_acc(star_array[i],star_array[j]);
            }
        }
    }

    wait(2);
}