#include <SFML/Graphics.hpp>
#include <chrono>
#include <random>
#include <iostream>
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

    Galaxy milkyway(5000,80,WINDOW_X/2+1,WINDOW_Y/2+1,1,0);
    Galaxy andromeda(500,40,3*WINDOW_X/4+1,WINDOW_Y/4+1,80,80);

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

    for(unsigned int i=0;i<milkyway.star_arr.size();++i){
        my_bhtree.Insert(milkyway.star_arr[i],0);
    }        
    for(unsigned int i=0;i<andromeda.star_arr.size();++i){
        my_bhtree.Insert(andromeda.star_arr[i],0);
    }
    my_bhtree.ComputeMassDistribution();

    sf::CircleShape shape(2.f);
    sf::Vector2f starPosition2;
    starPosition2.x = 300.f;
    starPosition2.y = 300.f;
    shape.setFillColor(sf::Color::White);
    shape.setPosition(starPosition2);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

        }

        starPosition2.x = 500;
        starPosition2.y = 500; 
        shape.setPosition(starPosition2);
        for(unsigned int i=0;i<milkyway.star_arr.size();++i){

            Vector new_acc = my_bhtree.CalculateForce(milkyway.star_arr[i]);
            new_acc = new_acc/1e6;
            // if(i%100==0) cout << new_acc.getX() << endl;
            milkyway.star_arr[i].update_state(new_acc);
        }
        for(unsigned int i=0;i<andromeda.star_arr.size();++i){

            Vector new_acc = my_bhtree.CalculateForce(andromeda.star_arr[i]);
            if(i==0) new_acc = new_acc/1e4; 
            else new_acc = new_acc/1e6;
            // if(i%100==0) cout << new_acc.getX() << endl;
            andromeda.star_arr[i].update_state(new_acc);
        }
        
        my_bhtree.Reset(ul,lr);
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

        window.draw(shape);
        window.draw(vline,2,sf::Lines);
        window.draw(hline,2,sf::Lines);
        window.display();
    }

    return 0;
}