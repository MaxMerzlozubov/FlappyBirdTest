#include "Game.h"

const double TIMER_DELIMITER_MICROSEC = 1000. * 1000.;

int main() {
	Game game;

	game.run();
	
	return EXIT_SUCCESS;
}
