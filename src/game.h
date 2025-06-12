#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath>

// struct Hunter
// {
//     sf::Image hunter_image;
//     sf::Sprite hunter_sprite;
//     sf::Texture hunter_texture;
//     int hunter_id, x, y;
//     bool draw = 0;
//     Hunter()
//     {
//         hunter_image.loadFromFile("images/hunter.png");
//         float scaleX = 0.15f;
//         float scaleY = 0.15f;
//         hunter_texture.loadFromImage(hunter_image);
//         hunter_sprite.setTexture(hunter_texture);
//         hunter_sprite.setScale(scaleX, scaleY);
//         hunter_sprite.setPosition(20, 20);
//     }
// };

// struct Victim
// {
//     sf::Image victim_image;
//     sf::Sprite victim_sprite;
//     sf::Texture victim_texture;
//     int victim_id, x, y;
//     static int position_x, position_y;
//     bool draw = 0;
//     Victim()
//     {
//         victim_image.loadFromFile("images/victim.png");
//         float scaleX = 0.15f;
//         float scaleY = 0.15f;
//         victim_texture.loadFromImage(victim_image);
//         victim_sprite.setTexture(victim_texture);
//         victim_sprite.setScale(scaleX, scaleY);
//         victim_sprite.setPosition(position_x, position_y);
//         position_x += 100;
//     }
// };

class Victim
{
public:
    sf::CircleShape shape;
    sf::CircleShape preview_shape;
    bool is_selected;
    Victim(float radius, const sf::Color& color, float x, float y);
    void move(float target_x, float target_y);
    bool contains(float x, float y);
    void show_preview(float target_x, float target_y);
    void hide_preview();
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
    std::vector<Victim> victims;
    Victim* selected_victim;
    Victim* previewed_victim;
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
    void run();
    void handle_mouse_click(float mouse_x, float mouse_y);
    void handle_mouse_move(float mouse_x, float mouse_y);
};
#endif