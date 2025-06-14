#include "game.h"

Victim::Victim(float radius, const sf::Color &color, float x, float y) : is_selected(false), has_moved(false), crossed_scoring_line(false), reached_left_edge(false)
{
    shape.setRadius(radius);
    shape.setFillColor(color);
    shape.setPosition(x, y);
    previous_position = sf::Vector2f(x, y);

    preview_shape.setRadius(radius);
    preview_shape.setFillColor(sf::Color(color.r, color.g, color.b, 100));
    preview_shape.setPosition(-1000, -1000);
    preview_shape.setOutlineThickness(2);
    preview_shape.setOutlineColor(sf::Color::White);
}

Hunter::Hunter(float radius, const sf::Color& color, float x, float y) : Victim(radius, color, x, y)
{
    shape.setRadius(radius);
    preview_shape.setRadius(radius);
    is_selected = false;
    has_moved = false;
}

void Victim::move(float target_x, float target_y, float window_width, float window_height)
{
    if (has_moved) return;

    sf::Vector2f current_pos = shape.getPosition();
    previous_position = current_pos;

    float dx = target_x - (current_pos.x + shape.getRadius());
    float dy = target_y - (current_pos.y + shape.getRadius());
    float length = std::sqrt(dx*dx + dy*dy);

    if (length > 0) {
        dx = dx / length * shape.getRadius() * 2;
        dy = dy / length * shape.getRadius() * 2;
    }

    shape.move(dx, dy);
    has_moved = true;
}



bool Victim::contains(float x, float y)
{
    sf::Vector2f position = shape.getPosition();
    float distance = std::sqrt(std::pow(x - position.x - shape.getRadius(), 2) +
                               std::pow(y - position.y - shape.getRadius(), 2));
    return distance <= shape.getRadius();
}

void Victim::show_preview(float target_x, float target_y)
{
    sf::Vector2f current_position = shape.getPosition();
    float dx = target_x - (current_position.x + shape.getRadius());
    float dy = target_y - (current_position.y + shape.getRadius());
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0)
    {
        dx /= distance;
        dy /= distance;
        dx *= shape.getRadius() * 2;
        dy *= shape.getRadius() * 2;
    }

    preview_shape.setPosition(shape.getPosition() + sf::Vector2f(dx, dy));
}

void Victim::hide_preview()
{
    preview_shape.setPosition(-1000, -1000);
}

void Hunter::move(float target_x, float target_y, float window_width, float window_height)
{
    if (has_moved)
    {
        return;
    }

    sf::Vector2f current_position = shape.getPosition();
    float dx = target_x - (current_position.x + shape.getRadius());
    float dy = target_y - (current_position.y + shape.getRadius());
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0)
    {
        dx /= distance;
        dy /= distance;
        dx *= shape.getRadius() * 2;
        dy *= shape.getRadius() * 2;
    }

    sf::Vector2f new_position = current_position + sf::Vector2f(dx, dy);

    if (new_position.x >= 0 && new_position.x + shape.getRadius() * 2 <= window_width)
    {
        shape.move(dx, 0);
    }
    if (new_position.y >= 0 && new_position.y + shape.getRadius() * 2 <= window_height)
    {
        shape.move(0, dy);
    }

    has_moved = true;
}



void Game::initialize_variables()
{
    this->window = nullptr;
}

void Game::initialize_window()
{
    this->video_mode.width = 1100;
    this->video_mode.height = 850;
    this->window = new sf::RenderWindow(this->video_mode, "SONOR GAME", sf::Style::Titlebar | sf::Style::Close);
}

void Game::initialize_players()
{
    float radius = 25.f;
    for (int i = 0; i < 5; ++i)
    {
        victims.emplace_back(radius, sf::Color::Green, 1000.f, 25.f + i * (2 * radius + 124));
    }
}

Game::Game() : hunter(50.f, sf::Color::Red, 0.f, 348.f), selected_victim(nullptr), 
               hunter_score(0), victims_score(0), game_ended(false)
{
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    this->initialize_variables();
    this->initialize_players();
    this->initialize_window();

    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    hunter_score_text.setFont(font);
    hunter_score_text.setCharacterSize(24);
    hunter_score_text.setFillColor(sf::Color::Red);
    hunter_score_text.setString("Hunter Score: 0");

    victims_score_text.setFont(font);
    victims_score_text.setCharacterSize(24);
    victims_score_text.setFillColor(sf::Color::Red);
    victims_score_text.setString("Victims Score: 0");

    field_image.loadFromFile("images/default_field.jpg");
    field_texture.loadFromImage(field_image);
    field_sprite.setTexture(field_texture);
    field_sprite.setScale(scaleX, scaleY);
    field_sprite.setPosition(0, 0);
    is_victims_turn = true;

    if (!eat_buffer.loadFromFile("sound_eat_victim.mp3") || !win_buffer.loadFromFile("sound_win.mp3"))
    {
        std::cerr << "Error loading sound files" << std::endl;
    }

    eat_sound.setBuffer(eat_buffer);
    win_sound.setBuffer(win_buffer);
    reset_moves();
    is_victims_turn = true;
}

Game::~Game()
{
    delete this->window;
}

const bool Game::running()
{
    return this->window->isOpen();
}

void Game::update_events()
{
    while (this->window->pollEvent(this->event))
    {
        switch(this->event.type)
        {
            case sf::Event::Closed:
                this->window->close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    this->window->close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    handle_mouse_click(event.mouseButton.x, event.mouseButton.y, true);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    handle_mouse_click(event.mouseButton.x, event.mouseButton.y, false);
                }
                break;
            case sf::Event::MouseMoved:
                if (selected_victim)
                    handle_mouse_move(event.mouseMove.x, event.mouseMove.y);
                break;
        }
    }
}

void Game::handle_mouse_click(float mouse_x, float mouse_y, bool is_left_click)
{
    if (is_left_click) {

        if (!selected_victim) {
            if (is_victims_turn) {
                
                for (auto& victim : victims) {
                    if (victim.contains(mouse_x, mouse_y) && !victim.has_moved) {
                        selected_victim = &victim;
                        victim.is_selected = true;
                        victim.show_preview(mouse_x, mouse_y);
                        break;
                    }
                }
            } else {
                
                if (hunter.contains(mouse_x, mouse_y) && !hunter.has_moved) {
                    selected_victim = &hunter;
                    hunter.is_selected = true;
                    hunter.show_preview(mouse_x, mouse_y);
                }
            }
        }
    } 
    else {
        
        if (selected_victim)
        {
            selected_victim->move(mouse_x, mouse_y, window->getSize().x, window->getSize().y);
            selected_victim->is_selected = false;
            selected_victim->hide_preview();
            selected_victim->has_moved = true;
            selected_victim = nullptr;

            if (is_victims_turn) {
                bool all_moved = true;
                for (const auto& v : victims) {
                    if (!v.has_moved) {
                        all_moved = false;
                        break;
                    }
                }
                if (all_moved) {
                    is_victims_turn = false;
                    hunter.has_moved = false;
                }
            } else {
                is_victims_turn = true;
                reset_moves();
            }
        }
    }
}

void Game::handle_mouse_move(float mouse_x, float mouse_y)
{
    if (selected_victim)
    {
        selected_victim->show_preview(mouse_x, mouse_y);
    }
}

void Game::check_collisions_and_boundaries()
{
    for (auto it = victims.begin(); it != victims.end(); )
    {
        sf::Vector2f victim_pos = it->shape.getPosition();
        float victim_right_edge = victim_pos.x + it->shape.getRadius() * 2;
        
        sf::Vector2f hunter_pos = hunter.shape.getPosition();
        float distance = std::sqrt(std::pow(hunter_pos.x - victim_pos.x, 2) + 
                                 std::pow(hunter_pos.y - victim_pos.y, 2));

        if (distance < hunter.shape.getRadius() + it->shape.getRadius())
        {
            it = victims.erase(it);
            hunter_score++;
            eat_sound.play();
            continue;
        }

        if (victim_right_edge <= 0 && !it->reached_left_edge)
        {
            it->reached_left_edge = true;
        }

        ++it;
    }
}

void Game::check_game_end_condition()
{
    if (hunter_score == 5)
    {
        win_sound.play();
        display_game_over("Hunter Wins!");
        return;
    }

    if (!victims.empty())
    {
        bool all_reached_edge = true;
        for (const auto& victim : victims)
        {
            if (!victim.reached_left_edge)
            {
                all_reached_edge = false;
                break;
            }
        }
        
        if (all_reached_edge)
        {
            win_sound.play();
            display_game_over("Victims Win!");
        }
    }
}

void Game::display_game_over(const std::string& message)
{
    sf::Text game_over_text;
    game_over_text.setFont(font);
    game_over_text.setString(message);
    game_over_text.setCharacterSize(50);
    game_over_text.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = game_over_text.getLocalBounds();
    game_over_text.setOrigin(textBounds.left + textBounds.width/2.0f, 
                           textBounds.top + textBounds.height/2.0f);
    game_over_text.setPosition(window->getSize().x/2.0f, 
                             window->getSize().y/2.0f);

    window->clear();
    window->draw(field_sprite);
    
    for (const auto& victim : victims)
        window->draw(victim.shape);
    window->draw(hunter.shape);
    
    window->draw(game_over_text);
    window->display();
}

void Game::reset_moves()
{
    for (auto& victim : victims)
    {
        victim.has_moved = false;
    }
    hunter.has_moved = false;
}

void Game::update()
{
    this->update_events();
    this->check_collisions_and_boundaries();
    this->check_game_end_condition();
}


void Game::render()
{
    this->window->clear(sf::Color(0, 0, 255, 255));
    this->window->draw(field_sprite);

    for (const auto& victim : victims)
    {
        this->window->draw(victim.shape);
        this->window->draw(victim.preview_shape);
    }

    this->window->draw(hunter.shape);
    this->window->draw(hunter.preview_shape);

    hunter_score_text.setString("Hunter Score: " + std::to_string(hunter_score));
    victims_score_text.setString("Victims Score: " + std::to_string(victims_score));

    sf::FloatRect hunterTextBounds = hunter_score_text.getLocalBounds();
    hunter_score_text.setOrigin(hunterTextBounds.left + hunterTextBounds.width / 2.0f, hunterTextBounds.top + hunterTextBounds.height / 2.0f);
    hunter_score_text.setPosition(this->window->getSize().x / 2.0f, this->window->getSize().y - 50.0f);

    sf::FloatRect victimsTextBounds = victims_score_text.getLocalBounds();
    victims_score_text.setOrigin(victimsTextBounds.left + victimsTextBounds.width / 2.0f, victimsTextBounds.top + victimsTextBounds.height / 2.0f);
    victims_score_text.setPosition(this->window->getSize().x / 2.0f, this->window->getSize().y - 20.0f);

    this->window->draw(hunter_score_text);
    this->window->draw(victims_score_text);

    sf::Text turnIndicator;
    turnIndicator.setFont(font);
    turnIndicator.setString(is_victims_turn ? "Victims turn" : "Hunter turn");
    turnIndicator.setCharacterSize(24);
    turnIndicator.setFillColor(is_victims_turn ? sf::Color::Red : sf::Color::Red);
    turnIndicator.setPosition(900, 800);
    this->window->draw(turnIndicator);

    sf::RectangleShape scoring_line(sf::Vector2f(2.f, static_cast<float>(window->getSize().y)));
    scoring_line.setPosition(500.f, 0.f);
    scoring_line.setFillColor(sf::Color(255, 255, 255, 150));
    window->draw(scoring_line);
    this->window->display();
}


void Game::run()
{
    while (this->window->isOpen())
    {
        this->update();
        this->render();
    }
}
