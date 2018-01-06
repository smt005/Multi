#include "Game.h"

Game *Game::_game = nullptr;

Game::Game()
{
}

Game::~Game()
{
}

void Game::init()
{
	if (_game)
	{
		delete _game;
	}

	_game = new Game();
}

void Game::tact()
{
}
