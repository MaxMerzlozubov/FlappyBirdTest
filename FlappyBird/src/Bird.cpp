#include "Bird.h"

const float BIRD_START_Y_POS = 400.;
const float BIRD_X_POS = 50.;
const float SPEED_AFTER_FLAP = 420.;
const float BIRD_ROTATION_COEF = 0.04;
const float BIRD_FLAP_FREQUENCY = 10.;
const float GRAVITATION_FORCE = 1200.;

Bird::Bird() : y(BIRD_START_Y_POS), vel(0.), currentFrame(0.)
{
	for (const auto& path : {
		"res/textures/bird/1-2.png",
		"res/textures/bird/1-3.png",
		"res/textures/bird/1-2.png",
		"res/textures/bird/1-1.png",
		}) {
		std::unique_ptr<sf::Texture> frame = std::make_unique<sf::Texture>();
		frame->loadFromFile(path);
		frames.push_back(std::move(frame));
	}
	texture = frames[0].get();
}

sf::FloatRect Bird::getRect() const {
	auto size = texture->getSize();
	return {
		BIRD_X_POS, y,
		(float)size.x, (float)size.y
	};
}

void Bird::flap(bool gameRunning, bool gameOvered) const {
	if (!gameRunning || gameOvered)
		return;
	vel = -SPEED_AFTER_FLAP;
}

void Bird::draw(const std::unique_ptr<sf::RenderWindow>& window) {
	sf::Sprite birdSprite(*texture);
	birdSprite.setRotation(BIRD_ROTATION_COEF * vel);
	birdSprite.setPosition(BIRD_X_POS, y);

	window->draw(birdSprite);
}

void Bird::update(const std::unique_ptr<sf::Texture>& backgroundTexture,
	float delta, bool gameRunning,
	bool& gameOvered) {
	currentFrame += delta * BIRD_FLAP_FREQUENCY;
	while (currentFrame > frames.size())
		currentFrame -= frames.size();
	texture = frames[(int)currentFrame].get();
	if (gameRunning) {
		vel += delta * GRAVITATION_FORCE;
		y += vel * delta;

		if (y < 0. || y + texture->getSize().y > backgroundTexture->getSize().y)
			gameOvered = true;

		if (y + texture->getSize().y > backgroundTexture->getSize().y) {
			y = (float)(backgroundTexture->getSize().y - texture->getSize().y);
			vel = 0.;
		}
	}
}
