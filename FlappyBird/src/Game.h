#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
#include <vector>
#include "Bird.h"
#include "Pipe.h"

class Game {
public:
	Game();
	void run();

private:
	void update();
	void draw();
	void handleEvent(sf::Event& event);
	void updateDelta(float delta_);
	bool windowIsOpen() const;
	bool windowPollEvent(sf::Event& event);
	void windowDisplay();

	std::unique_ptr<sf::RenderWindow> window;
	std::vector<std::unique_ptr<sf::Texture>> startGameTextures;
	std::unique_ptr<sf::Texture> endGameTexture;
	std::unique_ptr<sf::Texture> backgroundTexture;
	std::unique_ptr<sf::Texture> groundTexture;
	std::unique_ptr<sf::Clock> pipeGeneratingClock;
	std::unique_ptr<sf::Font> font;
	std::unique_ptr<Bird> bird;
	std::list<std::unique_ptr<Pipe>> pipes;
	bool gameRunning{};
	bool gameOvered{};
	float groundOffset{};
	float delta{};
	int score{};
	float currentFrame{};
};

#endif
