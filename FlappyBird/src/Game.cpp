#include "Game.h"

const int WINDOW_WIDTH = 450;
const int WINDOW_HEIGHT = 700;
const unsigned int FRAMERATE_LIMIT = 60;
const size_t MAX_PIPES_VECT = 3;
const float TIMER_DELIMITER_MICROSEC = 1000000.;
const float PIPE_GENERATION_PERIOD_SEC = 3.5;
const float GROUND_MOVE_SPEED_COEF = 100.;
const float GROUND_OFFSET_SPRITE_MIN = 24.;
const float SCORE_TEXT_Y_POS = 5.;
const float START_GAME_BLINK_RATE_COEF = 2.;
const float GET_READY_Y_POS = 230.;

Game::Game() :
	window(std::make_unique<sf::RenderWindow>(
		sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
		"Flappy Bird!",
		sf::Style::Titlebar | sf::Style::Close)), // window resize prohibition
	endGameTexture(std::make_unique<sf::Texture>()),
	backgroundTexture(std::make_unique<sf::Texture>()),
	bird(std::make_unique<Bird>()),
	groundTexture(std::make_unique<sf::Texture>()),
	pipeGeneratingClock(std::make_unique<sf::Clock>()),
	font(std::make_unique<sf::Font>())
{
	window->setFramerateLimit(FRAMERATE_LIMIT);
	endGameTexture->loadFromFile("res/textures/gameover.png");
	for (const auto& path : {
	"res/textures/get_ready/1.png",
	"res/textures/get_ready/2.png"
		}) {
		std::unique_ptr<sf::Texture> frame = std::make_unique<sf::Texture>();
		frame->loadFromFile(path);
		startGameTextures.push_back(std::move(frame));
	}
	backgroundTexture->loadFromFile("res/textures/background/day.png");
	groundTexture->loadFromFile("res/textures/ground.png");
	srand((unsigned int)time(nullptr));
	font->loadFromFile("res/fonts/04B_19__.TTF");
}

void Game::run() {
	sf::Clock deltaClock;

	while (windowIsOpen()) {
		sf::Event event{};
		while (windowPollEvent(event))
			handleEvent(event);

		updateDelta((float)deltaClock.getElapsedTime().asMicroseconds() / TIMER_DELIMITER_MICROSEC);
		deltaClock.restart();

		update();
		draw();
		windowDisplay();
	}
}

void Game::update() {
	bird->update(backgroundTexture, delta, gameRunning, gameOvered);
	for (const auto& pipe : pipes)
		pipe->update(bird, delta, gameRunning, gameOvered, score);

	if (gameRunning && !gameOvered) {
		if (pipeGeneratingClock->getElapsedTime().asSeconds() > PIPE_GENERATION_PERIOD_SEC) {
			pipeGeneratingClock->restart();
			pipes.push_back(std::make_unique<Pipe>(window));

			if (pipes.size() > MAX_PIPES_VECT)
				pipes.erase(pipes.begin());
		}
	}
}

void Game::draw() {
	window->clear();

	window->draw(sf::Sprite(*backgroundTexture));

	for (const auto& pipe : pipes)
		pipe->draw();

	// draw and move ground sprite
	sf::Sprite groundSprite(*groundTexture);
	if (!(!gameRunning || gameOvered)) {
		groundOffset -= delta * GROUND_MOVE_SPEED_COEF;
		if (groundOffset <= -GROUND_OFFSET_SPRITE_MIN)
			groundOffset += GROUND_OFFSET_SPRITE_MIN;
	}
	groundSprite.setPosition(groundOffset, (float)backgroundTexture->getSize().y);
	window->draw(groundSprite);

	// draw layer under ground sprite
	sf::RectangleShape lowerRectangle({
		(float)window->getSize().x,
		(float)(window->getSize().y - backgroundTexture->getSize().y - groundTexture->getSize().y) });
	lowerRectangle.setPosition(0, (float)(backgroundTexture->getSize().y + groundTexture->getSize().y));
	lowerRectangle.setFillColor(sf::Color::Yellow);
	window->draw(lowerRectangle);

	bird->draw(window);

	sf::Text scoreText("Score: " + std::to_string(score), *font);
	scoreText.setPosition(
		(window->getSize().x - scoreText.getLocalBounds().width) / 2,
		SCORE_TEXT_Y_POS);
	window->draw(scoreText);

	// draw and change main menu tutorial
	currentFrame += delta * START_GAME_BLINK_RATE_COEF;
	while (currentFrame > startGameTextures.size())
		currentFrame -= startGameTextures.size();
	sf::Sprite getReadySprite(*startGameTextures[(int)currentFrame].get());
	getReadySprite.setPosition(0., GET_READY_Y_POS);
	if (!gameRunning && !gameOvered)
		window->draw(getReadySprite);

	// draw end game sprite
	if (gameRunning && gameOvered) {
		sf::Sprite endGameSprite(*endGameTexture);
		endGameSprite.setPosition(
			(float)(window->getSize().x - endGameTexture->getSize().x) / 2,
			(float)(window->getSize().y - endGameTexture->getSize().y) / 2);
		window->draw(endGameSprite);
	}
}

void Game::handleEvent(sf::Event& event) {
	if (event.type == sf::Event::Closed)
		window->close();

	if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) ||
		(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)) {
		if (!gameRunning) {
			gameRunning = true;
			pipeGeneratingClock->restart();
			pipes.push_back(std::make_unique<Pipe>(window));
		}
		bird->flap(gameRunning, gameOvered);
		if (gameRunning && gameOvered) {
			gameRunning = gameOvered = false;
			pipes.clear();
			score = 0;
			bird.reset();
			bird = std::make_unique<Bird>();
		}
	}
}

void Game::updateDelta(float delta_) { 
	delta = delta_;
}

bool Game::windowIsOpen() const { 
	return window->isOpen(); 
}

bool Game::windowPollEvent(sf::Event& event) {
	return window->pollEvent(event);
}

void Game::windowDisplay() {
	window->display();
}
