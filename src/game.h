#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

struct Hunter
{
    sf::Image hunter_image;
    sf::Sprite hunter_sprite;
    sf::Texture hunter_texture;
    int hunter_id, x, y;
    bool draw = 0;
    Hunter()
    {
        hunter_image.loadFromFile("images/hunter.png");
        float scaleX = 0.15f;
        float scaleY = 0.15f;
        hunter_texture.loadFromImage(hunter_image);
        hunter_sprite.setTexture(hunter_texture);
        hunter_sprite.setScale(scaleX, scaleY);
        hunter_sprite.setPosition(20, 20);
    }
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
    Hunter* hunter;
public:
    Game();
    virtual ~Game();
    void initialize_variables();
    void initialize_window();
    void initialize_enemies();
    const bool running();
    void update();
    void render();
    void update_events();
};
#endif