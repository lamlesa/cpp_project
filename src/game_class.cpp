#include "game.h"

Victim::Victim(float radius, const sf::Color &color, float x, float y) : is_selected(false) 
{
    shape.setRadius(radius);
    shape.setFillColor(color);
    shape.setPosition(x, y);

    preview_shape.setRadius(radius);
    preview_shape.setFillColor(sf::Color(color.r, color.g, color.b, 100));
    preview_shape.setPosition(-100, -100);
    preview_shape.setOutlineThickness(2);
    preview_shape.setOutlineColor(sf::Color::White);
}

void Victim::move(float target_x, float target_y)
{
    sf::Vector2f current_position = shape.getPosition();
    float dx = target_x - (current_position.x + shape.getRadius());
    float dy = target_y - (current_position.y + shape.getRadius());
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > shape.getRadius() * 2)
    {
        dx /= distance;
        dy /= distance;
        dx *= shape.getRadius() * 2;
        dy *= shape.getRadius() * 2;
    }

    shape.move(dx, dy);
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

    if (distance > shape.getRadius() * 2)
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
    preview_shape.setPosition(-100, -100);
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

Game::Game()
{
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    this->initialize_variables();
    this->initialize_players();
    this->initialize_window();
    field_image.loadFromFile("images/default_field.jpg");
    field_texture.loadFromImage(field_image);
    field_sprite.setTexture(field_texture);
    field_sprite.setScale(scaleX, scaleY);
    field_sprite.setPosition(0, 0);
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
                    handle_mouse_click(event.mouseButton.x, event.mouseButton.y);
                break;
            case sf::Event::MouseMoved:
                if (selected_victim)
                    handle_mouse_move(event.mouseMove.x, event.mouseMove.y);
                break;
        }
    }
}

void Game::handle_mouse_click(float mouse_x, float mouse_y)
{
    if (!selected_victim)
    {
        for (auto& victim : victims)
        {
            if (victim.contains(mouse_x, mouse_y))
            {
                selected_victim = &victim;
                victim.is_selected = true;
            }
            else
            {
                victim.is_selected = false;
                victim.hide_preview();
            }
        }
    }
    else
    {
        selected_victim->move(mouse_x, mouse_y);
        selected_victim->hide_preview();
        selected_victim = nullptr;
    }
}

void Game::handle_mouse_move(float mouse_x, float mouse_y)
{
    if (selected_victim)
    {
        selected_victim->show_preview(mouse_x, mouse_y);
    }
}

void Game::update()
{
    this->update_events();
    if (selected_victim)
    {
        float diameter = selected_victim->shape.getRadius() * 2;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            selected_victim->move(-diameter, 0.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            selected_victim->move(diameter, 0.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            selected_victim->move(0.f, -diameter);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            selected_victim->move(0.f, diameter);
        }
    }
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
