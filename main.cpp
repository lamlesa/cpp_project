#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow starter_window(sf::VideoMode(1200, 800), "SONOR GAME", sf::Style::Titlebar | sf::Style::Close);

    while (starter_window.isOpen())
    {
        sf::Event event;
        while (starter_window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    starter_window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        starter_window.close();
                    break;
            }
        }

        starter_window.clear(); // старый кадр стирается
        starter_window.display(); // рисование окончено
    }

    return 0;
}