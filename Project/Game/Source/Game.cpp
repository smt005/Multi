#include "Game.h"
#include "GameMini/GameMini.h"
#include "GameEvolutionTest/GameEvolutionTest.h"
#include "GameTerrain/GameTerrain.h"
#include "Platform/Source/Application.h"
#include <string>

Game *Game::_game = nullptr;

void Game::gameInit()
{
	const std::string& nameGame = App::getAppConfig().getNameGame();
	if (nameGame == "GameMini")
	{
		GameMini *game = new GameMini();
		game->setCurrent();
	}
	else if (nameGame == "GameEvolutionTest")
	{
		GameEvolutionTest *game = new GameEvolutionTest();
		game->setCurrent();
	}
	else if (nameGame == "GameTerrain")
	{
		GameTerrain *game = new GameTerrain();
		game->setCurrent();
	}
	else
	{
		Game *game = new Game();
		game->setCurrent();
	}
}

void Game::gameSave()
{
	if (!_game) return;

	Game* gameTemp = _game;
	_game = nullptr;
	gameTemp->save();
	delete _game;
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
		_game->save();
		delete[] _game;
	}

	_game = this;
	_game->init();
}

void Game::init()
{

}

void Game::save()
{
}

void Game::tact()
{

}

void Game::draw()
{

}
