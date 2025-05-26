#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

struct Hunter
{
    sf::Sprite Sprite;
    int hunter_id, x, y;
    bool draw = 0;
};

class Game
{
private:
    sf::RenderWindow* window;
    sf::VideoMode video_mode;
    sf::Event event;
    sf::Image field_image;
    sf::Texture field_texture;
    sf::Sprite field_sprite;
public:
    Game();
    virtual ~Game();
    void initialize_variables();
    void initialize_window();
    const bool running();
    void update();
    void render();
    void update_events();
};
#endif