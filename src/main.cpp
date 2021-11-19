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

    Galaxy milkyway(2,80,WINDOW_X/2+1,WINDOW_Y/2+1,1,0);
    Galaxy andromeda(1,40,3*WINDOW_X/4+1,WINDOW_Y/4+1,1,0);

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

    for(auto star:milkyway.star_arr){
        my_bhtree.Insert(star,0);
    }        
    for(auto star:andromeda.star_arr){
        my_bhtree.Insert(star,0);
    }

    // sf::CircleShape shape(2.f);
    sf::Vector2f starPosition;
    // shape.setFillColor(sf::Color::White);
    // shape.setPosition(starPosition);
    // float xVelocity = -3;
    // float yVelocity = 3;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

        }
        // starPosition.x += rand()%2;
        // starPosition.y += rand()%2; 
        // shape.setPosition(starPosition);
        for(int i=0;i<milkyway.star_arr.size();i++){

            // Vector new_acc = Vector(5,5);
            // Vector new_acc = my_bhtree.CalculateForce(star);

            // Vector new_pos = star.update_state(new_acc);
            // starPosition.x = new_pos.getX()+5;
            // starPosition.y = new_pos.getY()+5;
 
            starPosition.x = milkyway.star_arr[i].state.pos.getX() + milkyway.star_arr[i].state.vel.getX();
            starPosition.y = milkyway.star_arr[i].state.pos.getY() + milkyway.star_arr[i].state.vel.getY();
            cout << "x_new = " << starPosition.x << endl;
            cout << "y_new = " << starPosition.y << endl; 
            cout << "x_old = " << milkyway.star_arr[i].state.pos.getX() << endl;
            cout << "y_old = " << milkyway.star_arr[i].state.pos.getY() << endl; 
            milkyway.star_arr[i].shape.setPosition(starPosition);
        }
        for(int i=0;i<andromeda.star_arr.size();i++){

            // Vector new_acc = Vector(5,5);
            // Vector new_acc = my_bhtree.CalculateForce(star);

            // Vector new_pos = star.update_state(new_acc);
            // starPosition.x = new_pos.getX();
            // starPosition.y = new_pos.getY();

            starPosition.x = andromeda.star_arr[i].state.pos.getX() + andromeda.star_arr[i].state.vel.getX();
            starPosition.y = andromeda.star_arr[i].state.pos.getY() + andromeda.star_arr[i].state.vel.getY();
            andromeda.star_arr[i].shape.setPosition(starPosition);
        }
        
        my_bhtree.Reset(ul,lr);
        for(auto star:milkyway.star_arr){
            my_bhtree.Insert(star,0);
        }        
        for(auto star:andromeda.star_arr){
            my_bhtree.Insert(star,0);
        }

        window.clear();

        for(auto star:milkyway.star_arr){
            window.draw(star.shape);
        }        
        for(auto star:andromeda.star_arr){
            window.draw(star.shape);
        }

        // window.draw(shape);
        window.draw(vline,2,sf::Lines);
        window.draw(hline,2,sf::Lines);
        window.display();
    }

    return 0;
}