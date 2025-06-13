#include "game.h"

Victim::Victim(float radius, const sf::Color &color, float x, float y) : is_selected(false) 
{
    shape.setRadius(radius);
    shape.setFillColor(color);
    shape.setPosition(x, y);

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

void Victim::move(float target_x, float target_y)
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

void Hunter::move(float target_x, float target_y)
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

    shape.move(dx, dy);
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
        victims.emplace_back(radius, sf::Color::Green, 1000.f, 25.f + i * (2 * radius + 110));
    }
}

Game::Game() : hunter(75.f, sf::Color::Red, 25.f, 300.f), selected_victim(nullptr), hunter_score(0), victims_score(0)
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
    if (is_left_click)
    {
        if (!selected_victim)
        {
            for (auto& victim : victims)
            {
                if (victim.contains(mouse_x, mouse_y) && !victim.has_moved)
                {
                    if (selected_victim)
                    {
                        selected_victim->is_selected = false;
                        selected_victim->hide_preview();
                    }
                    selected_victim = &victim;
                    victim.is_selected = true;
                    victim.show_preview(mouse_x, mouse_y);
                    break;
                }
            }

            if (hunter.contains(mouse_x, mouse_y) && !hunter.has_moved)
            {
                if (selected_victim)
                {
                    selected_victim->is_selected = false;
                    selected_victim->hide_preview();
                }
                selected_victim = &hunter;
                hunter.is_selected = true;
                hunter.show_preview(mouse_x, mouse_y);
            }
        }
    }
    else
    {
        if (selected_victim)
        {
            selected_victim->move(mouse_x, mouse_y);
            selected_victim->hide_preview();
            selected_victim->is_selected = false;

            if (selected_victim == &hunter)
            {
                is_victims_turn = true;
                for (auto& victim : victims)
                {
                    victim.has_moved = false;
                }
            }
            else
            {
                if (is_victims_turn)
                {
                    bool all_victims_moved = true;
                    for (const auto& victim : victims)
                    {
                        if (!victim.has_moved)
                        {
                            all_victims_moved = false;
                            break;
                        }
                    }
                    if (all_victims_moved)
                    {
                        is_victims_turn = false;
                        hunter.has_moved = false;
                    }
                }
            }

            selected_victim = nullptr;
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

bool Game::is_collision(const sf::CircleShape &shape_1, const sf::CircleShape &shape_2)
{
    sf::Vector2f pos_1 = shape_1.getPosition();
    sf::Vector2f pos_2 = shape_2.getPosition();
    float radius_1 = shape_1.getRadius();
    float radius_2 = shape_2.getRadius();

    float dx = pos_1.x - pos_2.x;
    float dy = pos_1.y - pos_2.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance < (radius_1 + radius_2);
}

void Game::update()
{
    this->update_events();

    for (auto it = victims.begin(); it != victims.end(); )
    {
        if (is_collision(hunter.shape, it->shape))
        {
            hunter_score++;
            it = victims.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto& victim : victims)
    {
        sf::Vector2f position = victim.shape.getPosition();
        if (position.x >= 500 && position.x < 510)
        {
            victims_score++;
            victim.shape.setPosition(1000.f, position.y);
        }
        else if (position.x >= 750 && position.x < 760)
        {
            victims_score++;
            victim.shape.setPosition(1000.f, position.y);
        }
        else if (position.x >= 900 && position.x < 910)
        {
            victims_score++;
            victim.shape.setPosition(1000.f, position.y);
        }
    }

    hunter_score_text.setString("Hunter Score: " + std::to_string(hunter_score));
    victims_score_text.setString("Victims Score: " + std::to_string(victims_score));
}


void Game::render()
{
    this->window->clear(sf::Color(0, 255, 0, 255));
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
    hunter_score_text.setPosition(this->window->getSize().x / 2.0f, this->window->getSize().y - 30.0f);

    sf::FloatRect victimsTextBounds = victims_score_text.getLocalBounds();
    victims_score_text.setOrigin(victimsTextBounds.left + victimsTextBounds.width / 2.0f, victimsTextBounds.top + victimsTextBounds.height / 2.0f);
    victims_score_text.setPosition(this->window->getSize().x / 2.0f, this->window->getSize().y - 10.0f);

    this->window->draw(hunter_score_text);
    this->window->draw(victims_score_text);

    this->window->display();
}

void Game::run()
{
    while (this->window->isOpen())
    {
        update();
        render();
    }
}
