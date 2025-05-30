#include "game.h"

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

void Game::initialize_enemies()
{
    hunter = new Hunter();
}

Game::Game()
{
    this->initialize_variables();
    this->initialize_enemies();
    this->initialize_window();
    field_image.loadFromFile("images/default_field.jpg");
    field_texture.loadFromImage(field_image);
    field_sprite.setTexture(field_texture);
    field_sprite.setPosition(10, 10);
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
        }
    }
}

void Game::update()
{
    this->update_events();
}

void Game::render()
{
    this->window->clear(sf::Color(0, 255, 0, 255));
    this->window->draw(field_sprite);
    this->window->draw(hunter->hunter_sprite);
    this->window->display();
}