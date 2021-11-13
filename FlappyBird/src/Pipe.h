#ifndef PIPE_H
#define PIPE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "Bird.h"

class Pipe {
private:
	const std::unique_ptr<sf::RenderWindow>& window;
	std::unique_ptr<sf::Texture> upperPipe;
	std::unique_ptr<sf::Texture> lowerPipe;
	float x;
	float y;
	bool scored;

public:
	Pipe(const std::unique_ptr<sf::RenderWindow>& window_);
	sf::FloatRect getUpperRect() const;
	sf::FloatRect getLowerRect() const;
	void draw() const;
	void update(const std::unique_ptr<Bird>& bird, float delta, bool gameRunning,
		bool& gameOvered, int& score);
};

#endif
