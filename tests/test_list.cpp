#include <gtest/gtest.h>
#include "../headers/game.h"

TEST(VictimTest, ConstructorInitializesCorrectly) {
    Victim victim(25.f, sf::Color::Green, 100.f, 200.f);
    EXPECT_FLOAT_EQ(victim.shape.getRadius(), 25.f);
    EXPECT_EQ(victim.shape.getFillColor(), sf::Color::Green);
    EXPECT_FALSE(victim.is_selected);
}

TEST(VictimTest, MoveWithinBounds) {
    Victim victim(25.f, sf::Color::Green, 100.f, 200.f);
    victim.move(150.f, 250.f, 800.f, 600.f);
    EXPECT_NE(victim.shape.getPosition(), sf::Vector2f(100.f, 200.f));
}

TEST(VictimTest, ContainsPoint) {
    Victim victim(25.f, sf::Color::Green, 100.f, 200.f);
    EXPECT_TRUE(victim.contains(110.f, 210.f));
    EXPECT_FALSE(victim.contains(0.f, 0.f));
}

TEST(HunterTest, InheritsFromVictim) {
    Hunter hunter(75.f, sf::Color::Red, 50.f, 300.f);
    EXPECT_FLOAT_EQ(hunter.shape.getRadius(), 75.f);
    EXPECT_EQ(hunter.shape.getFillColor(), sf::Color::Red);
}

TEST(GameTest, Initialization) {
    Game game;
    EXPECT_EQ(game.victims.size(), 5);
    EXPECT_FLOAT_EQ(game.hunter.shape.getRadius(), 75.f);
}

TEST(GameTest, HandleMouseClickSelectsVictim) {
    Game game;
    game.handle_mouse_click(1000.f, 25.f, true);
    EXPECT_NE(game.selected_victim, nullptr);
}

TEST(GameTest, TurnSwitching) {
    Game game;
    game.handle_mouse_click(1000.f, 25.f, true);
    game.handle_mouse_click(900.f, 25.f, false);
    EXPECT_TRUE(game.is_victims_turn);
}

TEST(GameTest, CollisionDetection) {
    Game game;
    game.hunter.shape.setPosition(1000.f, 25.f);
    game.check_collisions_and_boundaries();
    EXPECT_EQ(game.hunter_score, 1);
}

TEST(GameTest, ScoreCounting) {
    Game game;
    game.victims[0].shape.setPosition(10.f, 25.f);
    game.check_collisions_and_boundaries();
    EXPECT_GT(game.victims_score, 0);
}

TEST(GameTest, GameEndCondition) {
    Game game;
    game.hunter_score = 5;
    game.check_game_end_condition();
}

TEST(VictimTest, PreviewMovement) {
    Victim victim(25.f, sf::Color::Green, 100.f, 200.f);
    victim.show_preview(150.f, 250.f);
    EXPECT_NE(victim.preview_shape.getPosition(), sf::Vector2f(-1000.f, -1000.f));
}

TEST(GameTest, SoundEffects) {
    Game game;
    game.eat_sound.play();
    EXPECT_EQ(game.eat_sound.getStatus(), sf::Sound::Playing);
}

TEST(GameTest, WindowBounds) {
    Game game;
    EXPECT_EQ(game.window->getSize().x, 1100);
    EXPECT_EQ(game.window->getSize().y, 850);
}

TEST(GameTest, ScoreText) {
    Game game;
    game.hunter_score = 3;
    game.victims_score = 2;
    EXPECT_NE(game.hunter_score_text.getString().find("3"), std::string::npos);
    EXPECT_NE(game.victims_score_text.getString().find("2"), std::string::npos);
}

TEST(VictimTest, MoveState) {
    Victim victim(25.f, sf::Color::Green, 100.f, 200.f);
    victim.move(150.f, 250.f, 800.f, 600.f);
    EXPECT_TRUE(victim.has_moved);
}

TEST(GameTest, TextPositioning) {
    Game game;
    EXPECT_NEAR(game.hunter_score_text.getPosition().x, 550.f, 1.f);
    EXPECT_NEAR(game.hunter_score_text.getPosition().y, 800.f, 1.f);
}

TEST(GameTest, EventHandling) {
    Game game;
    sf::Event closeEvent;
    closeEvent.type = sf::Event::Closed;
    game.window->pollEvent(closeEvent);
}

TEST(GameTest, TextureInitialization) {
    Game game;
    EXPECT_TRUE(game.field_texture.getSize().x > 0);
}

TEST(GameTest, SelectionState) {
    Game game;
    game.handle_mouse_click(25.f, 300.f, true);
    EXPECT_EQ(game.selected_victim, &game.hunter);
}