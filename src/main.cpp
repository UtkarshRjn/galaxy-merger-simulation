#include <SFML/Graphics.hpp>
#include <chrono>
#include <random>

class Node{
    // A class for a node within the quadtree.
    
    // We use the terminology "child" for nodes in the next depth level - consistent with tree nomenclature
    // If a node is "childless" then it represents a body
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Galaxy Simulation!!");

    sf::CircleShape shape(10.f);
    sf::CircleShape shape2(10.f);
    shape.setFillColor(sf::Color::Green);
    shape2.setFillColor(sf::Color::Red);
    shape.setPosition(100.f,300.f);
    shape2.setPosition(100.f,30.f);
    unsigned seed = chrono::steady_clock::now().time_since_epoch().count(); 
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(0.0,1.0);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.draw(shape2);
        window.display();
    }

    return 0;
}