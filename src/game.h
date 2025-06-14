#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath>

class Victim
{
public:
    sf::CircleShape shape;
    sf::CircleShape preview_shape;
    bool is_selected;
    Victim(float radius, const sf::Color& color, float x, float y);
    virtual void move(float target_x, float target_y, float window_width, float window_height);
    bool contains(float x, float y);
    void show_preview(float target_x, float target_y);
    void hide_preview();
    bool has_moved;
    sf::Vector2f previous_position;
    bool crossed_scoring_line;
    bool reached_left_edge;
};

class Hunter : public Victim
{
public:
    Hunter(float radius, const sf::Color& color, float x, float y);
    void move(float target_x, float target_y, float window_width, float window_height);
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
    Hunter hunter;
    bool is_victims_turn;
    int hunter_score;
    int victims_score;
    sf::Font font;
    sf::Text hunter_score_text;
    sf::Text victims_score_text;
    sf::SoundBuffer eat_buffer;
    sf::Sound eat_sound;
    sf::SoundBuffer win_buffer;
    sf::Sound win_sound;
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
    void handle_mouse_click(float mouse_x, float mouse_y, bool which_button);
    void handle_mouse_move(float mouse_x, float mouse_y);
    void check_collisions_and_boundaries();
    void check_game_end_condition();
    void display_game_over(const std::string& message);
    void reset_moves();
};
#endif