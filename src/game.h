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

struct Victim
{
    sf::Image victim_image;
    sf::Sprite victim_sprite;
    sf::Texture victim_texture;
    int victim_id, x, y;
    static int position_x, position_y;
    bool draw = 0;
    Victim()
    {
        victim_image.loadFromFile("images/victim.png");
        float scaleX = 0.15f;
        float scaleY = 0.15f;
        victim_texture.loadFromImage(victim_image);
        victim_sprite.setTexture(victim_texture);
        victim_sprite.setScale(scaleX, scaleY);
        victim_sprite.setPosition(position_x, position_y);
        position_x += 100;
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
    Victim* victim_1;
    Victim* victim_2;
    Victim* victim_3;
    Victim* victim_4;
    Victim* victim_5;
public:
    Game();
    virtual ~Game();
    void initialize_variables();
    void initialize_window();
    void initialize_players();
    const bool running();
    void update();
    void render();
    void update_events();
};
#endif