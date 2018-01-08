#include "Game.h"
#include "GameMini/GameMini.h"

Game *Game::_game = nullptr;

Game::Game()
{
}

Game::~Game()
{
	delete[] _game;
	_game = nullptr;
}

void Game::gameInit()
{
	GameMini *game = new GameMini();
	game->setCurrent();
	game->init();
}

void Game::gameTact()
{
	_game->tact();
}

void Game::gameDraw()
{
	_game->draw();
}

void Game::setCurrent()
{
	if (_game)
	{
		delete[] _game;
	}

	_game = this;
}

void Game::init()
{

}

void Game::tact()
{

}

void Game::draw()
{

}
