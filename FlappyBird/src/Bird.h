#ifndef BIRD_H
#define BIRD_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Bird {
public:
	Bird();
	sf::FloatRect getRect() const;
	void flap(bool gameRunning, bool gameOvered) const;
	void draw(const std::unique_ptr<sf::RenderWindow>& window);
	void update(
		const std::unique_ptr<sf::Texture>& backgroundTexture,
		float delta, bool gameRunning,
		bool& gameOvered);

private:
	std::vector<std::unique_ptr<sf::Texture>> frames;
	sf::Texture* texture;
	float y;
	mutable float vel;
	float currentFrame;
};

#endif
