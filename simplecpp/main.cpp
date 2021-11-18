#include <simplecpp>
#include <deque>
#include <random>
#include <chrono>
#include "MovingObject.h"
#include "star.h"

using namespace simplecpp;

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