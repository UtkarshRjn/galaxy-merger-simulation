//--- Standard includes --------------------------------------------------------
#include <SFML/Graphics.hpp>
#include <chrono>
#include <random>
#include <iostream>
#include <vector>

//--- Implementation -----------------------------------------------------------
#include "Galaxy.h"
#include "BHTree.h"
#include "Vector.h"

#define WINDOW_X 1000
#define WINDOW_Y 1000
#define FRAME_RATE 60 

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Galaxy Simulation!!");
    window.setFramerateLimit(FRAME_RATE);

    Galaxy milkyway(5000,200,WINDOW_X/2-500+1,WINDOW_Y/2+500+1,1,-1);
    Galaxy andromeda(5000,200,3*WINDOW_X/4+1,WINDOW_Y/4+1,-1,2);

    sf::Vertex vline[] = 
    {
        sf::Vertex(sf::Vector2f(WINDOW_X/2,0)),
        sf::Vertex(sf::Vector2f(WINDOW_X/2,WINDOW_Y))
    };    
    sf::Vertex hline[] = 
    {
        sf::Vertex(sf::Vector2f(0,WINDOW_Y/2)),
        sf::Vertex(sf::Vector2f(WINDOW_X,WINDOW_Y/2))
    };

    Vector ul = Vector(WINDOW_X,WINDOW_Y);
    Vector lr = Vector(0,0);
    BHTreeNode my_bhtree(ul,lr);

    // sf::CircleShape shape1(2.f);
    // sf::Vector2f starPosition1(100.f,100.f);
    // sf::CircleShape shape2(2.f);
    // sf::Vector2f starPosition2(500.f,500.f);
    // shape1.setFillColor(sf::Color::White);
    // shape1.setPosition(starPosition1);
    // shape2.setFillColor(sf::Color::White);
    // shape2.setPosition(starPosition2);

    // State state1 = {Vector(0,300),Vector(3,0),Vector(0,0)};
    // Star star1 = Star(state1,10);
    // State state2 = {Vector(1000,700),Vector(-3,0),Vector(0,0)};
    // Star star2 = Star(state2,10);
    
    // my_bhtree.Insert(star2,0);
    // my_bhtree.Insert(star1,0);
    for(unsigned int i=0;i<milkyway.star_arr.size();++i){
        my_bhtree.Insert(milkyway.star_arr[i],0);
    }        
    for(unsigned int i=0;i<andromeda.star_arr.size();++i){
        my_bhtree.Insert(andromeda.star_arr[i],0);
    }
    my_bhtree.ComputeMassDistribution();
    // cout << endl;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

        }

        // vector<Vector> accelaration_vector;
        // Vector new_acc = my_bhtree.CalculateForce(star1);
        // cout << "acc1 = " << new_acc.getX() << endl;

        // new_acc = my_bhtree.CalculateForce(star2);
        
        // star1.update_state(new_acc);
        // star2.update_state(new_acc);
        for(unsigned int i=0;i<milkyway.star_arr.size();++i){

            Vector new_acc = my_bhtree.CalculateForce(milkyway.star_arr[i]);
            milkyway.star_arr[i].update_state(new_acc);
            // if(i%100==0) cout << "acc2 = " << new_acc.getX() << endl;
        }
        for(unsigned int i=0;i<andromeda.star_arr.size();++i){

            Vector new_acc = my_bhtree.CalculateForce(andromeda.star_arr[i]);
            andromeda.star_arr[i].update_state(new_acc);
        }

        for(unsigned int i=0;i<milkyway.star_arr.size();++i){

            milkyway.star_arr[i].update_pos();
        }
        for(unsigned int i=0;i<andromeda.star_arr.size();++i){

            andromeda.star_arr[i].update_pos();
        }

        my_bhtree.Reset(ul,lr);
        // my_bhtree.Insert(star1,0);
        // my_bhtree.Insert(star2,0);

        for(unsigned int i=0;i<milkyway.star_arr.size();++i){
            my_bhtree.Insert(milkyway.star_arr[i],0);
        }        
        for(unsigned int i=0;i<andromeda.star_arr.size();++i){
            my_bhtree.Insert(andromeda.star_arr[i],0);
        }
        my_bhtree.ComputeMassDistribution();
        window.clear();

        for(auto star:milkyway.star_arr){
            window.draw(star.shape);
        }        
        for(auto star:andromeda.star_arr){
            window.draw(star.shape);
        }

        // window.draw(shape);
        // window.draw(star1.shape);
        // window.draw(star2.shape);
        window.draw(vline,2,sf::Lines);
        window.draw(hline,2,sf::Lines);
        window.display();
    }

    return 0;
}