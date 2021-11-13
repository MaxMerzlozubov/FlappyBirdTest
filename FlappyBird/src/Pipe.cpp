#include "Pipe.h"
#include "Bird.h"

const float PIPE_MIDDLE_Y = 100.;
const float PIPE_GROWTH_STEP = 50.;
const float PIPE_MOVE_SPEED_COEF = 100.;
const float PIPE_Y_GAP = 340.;

Pipe::Pipe(const std::unique_ptr<sf::RenderWindow>& window_) :
	window(window_),
	upperPipe(std::make_unique<sf::Texture>()),
	lowerPipe(std::make_unique<sf::Texture>()),
	x((float)(window->getSize().x + upperPipe->getSize().x)),
	y(PIPE_MIDDLE_Y + (float)(rand() % 5 - 3) * PIPE_GROWTH_STEP),	// y [-50; +150]
	scored(false)
{
	sf::Image pipeImage;
	pipeImage.loadFromFile("res/textures/pipe.png");
	upperPipe->loadFromImage(pipeImage);
	pipeImage.flipVertically();
	lowerPipe->loadFromImage(pipeImage);
}

sf::FloatRect Pipe::getUpperRect() const {
	auto size = upperPipe->getSize();
	return {
			x, y + PIPE_Y_GAP,
			(float)size.x, (float)size.y
	};
}

sf::FloatRect Pipe::getLowerRect() const {
	auto size = upperPipe->getSize();
	return {
			x, y - PIPE_Y_GAP,
			(float)size.x, (float)size.y
	};
}

void Pipe::draw() const {
	sf::Sprite upperSprite(*upperPipe);
	upperSprite.setPosition(x, y + PIPE_Y_GAP);
	sf::Sprite lowerSprite(*lowerPipe);
	lowerSprite.setPosition(x, y - PIPE_Y_GAP);

	window->draw(upperSprite);
	window->draw(lowerSprite);
}

void Pipe::update(const std::unique_ptr<Bird>& bird,
	float delta, bool gameRunning,
	bool& gameOvered, int& score) {
	if (!gameRunning || gameOvered)
		return;

	x -= PIPE_MOVE_SPEED_COEF * delta;
	auto birdRect = bird->getRect();

	if (birdRect.intersects(getUpperRect()) || birdRect.intersects(getLowerRect()))
		gameOvered = true;

	if (x + upperPipe->getSize().x < birdRect.left && !scored) {
		scored = true;
		++score;
	}
}
