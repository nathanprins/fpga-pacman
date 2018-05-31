/*
 * PacMan.cpp
 *
 *  Created on: 2 May 2018
 *      Author: natha
 */

#include "Game.h"

Game::Game(ControllerInterface* control, VideoInterface* video) : ci(control), vi(video){
	vi->clear(0);

	setInSquare(&player, 10, 15);
};

void Game::update(int elapsedTime){
	if(state == LEVEL_RUN){
		switch(ci->getActiveButton()){
			case BUTTON_UP: player.setDir(DIR_UP); break;
			case BUTTON_DOWN: player.setDir(DIR_DOWN); break;
			case BUTTON_LEFT: player.setDir(DIR_LEFT); break;
			case BUTTON_RIGHT: player.setDir(DIR_RIGHT); break;
			default: break;
		}
		updateMovement(&player, elapsedTime);

		int xs = player.getX() / 8.0;
		int ys = player.getY() / 8.0;
		if(map[ys][xs] == pd){
			points += 100;
			printf("Points: %d \n", points);
			map[ys][xs] = em;
		}
	}
};

void Game::setInSquare(Entity* en, int xs, int ys){
	en->setPos(xs*8, ys*8);
};

void Game::distanceToWalls(Entity* en, int* up, int* down, int* left, int* right){
	int x = en->getX();
	int y = en->getY();
	int xSq = en->getX() / 8.0;
	int ySq = en->getY() / 8.0;

	int upSq = 0, downSq = 0, leftSq = 0, rightSq = 0;
	if(x % 8 == 0){
		while(walkable(map[ySq - upSq - 1][xSq])) 	 upSq++;
		while(walkable(map[ySq + downSq + 1][xSq]))  downSq++;
	}
	if(y % 8 == 0){
		while(walkable(map[ySq][xSq - leftSq - 1]))  leftSq++;
		while(walkable(map[ySq][xSq + rightSq + 1])) rightSq++;
	}

	int xPixel = (xSq * 8) - x;
	int yPixel = (ySq * 8) - y;

	*up 	= (upSq * 8) - yPixel;
	*down 	= (downSq * 8) + yPixel;
	*left 	= (leftSq * 8) - xPixel;
	*right 	= (rightSq * 8) + xPixel;
};

void Game::updateMovement(Entity* en, int elapsedTime){
	en->update(elapsedTime);

	int up, down, left, right;
	distanceToWalls(en, &up, &down, &left, &right);

	float timeFraction = (float)elapsedTime / 1000;
	int walk = roundf(timeFraction * en->getVelocity());

	switch(en->getDir()){
		case DIR_UP: 	en->setPos(en->getX(), en->getY() - SHORTEST(walk, up)); break;
		case DIR_DOWN: 	en->setPos(en->getX(), en->getY() + SHORTEST(walk, down)); break;
		case DIR_LEFT: 	en->setPos(en->getX() - SHORTEST(walk, left), en->getY()); break;
		case DIR_RIGHT: en->setPos(en->getX() + SHORTEST(walk, right), en->getY()); break;
		default: break;
	};
};

uint8_t Game::walkable(map_item_t item){
	return (item == em || item == pd);
};

void Game::draw(){
	switch(state){
		case LEVEL_START:
			drawMap();
			state = LEVEL_RUN;
			break;
		case LEVEL_RUN:
			player.draw(vi);
			break;
	}
};

void Game::drawMap(){
	for(int v = 0; v < 27; v++){
			for(int h = 0; h < 21; h++){
				if(simpleMap){
					if(!walkable(map[v][h]))
						vi->drawRect(H(0), V(0), 8, 8, RGB565(0, 74, 241));
				}else{
					switch(map[v][h]){
						case pd:
							vi->drawRect(H(3), V(3), 2, 2, RGB565(255, 165, 0));
							break;
						case c1:
							vi->drawLine(H(4), V(2), H(7), V(2), RGB565(0, 74, 241));
							vi->drawLine(H(2), V(4), H(2), V(7), RGB565(0, 74, 241));
							vi->drawPixel(H(3), V(3), RGB565(0, 74, 241));
							vi->drawLine(H(6), V(5), H(7), V(5), RGB565(0, 74, 241));
							vi->drawLine(H(5), V(6), H(5), V(7), RGB565(0, 74, 241));
							break;
						case c2:
							vi->drawLine(H(0), V(2), H(3), V(2), RGB565(0, 74, 241));
							vi->drawLine(H(5), V(4), H(5), V(7), RGB565(0, 74, 241));
							vi->drawPixel(H(4), V(3), RGB565(0, 74, 241));
							vi->drawLine(H(0), V(5), H(1), V(5), RGB565(0, 74, 241));
							vi->drawLine(H(2), V(5), H(2), V(7), RGB565(0, 74, 241));
							break;
						case c3:
							vi->drawLine(H(2), V(0), H(2), V(3), RGB565(0, 74, 241));
							vi->drawLine(H(4), V(5), H(7), V(5), RGB565(0, 74, 241));
							vi->drawPixel(H(3), V(4), RGB565(0, 74, 241));
							vi->drawLine(H(5), V(0), H(5), V(1), RGB565(0, 74, 241));
							vi->drawLine(H(6), V(2), H(7), V(2), RGB565(0, 74, 241));
							break;
						case c4:
							vi->drawLine(H(5), V(0), H(5), V(3), RGB565(0, 74, 241));
							vi->drawLine(H(0), V(5), H(3), V(5), RGB565(0, 74, 241));
							vi->drawPixel(H(4), V(4), RGB565(0, 74, 241));
							vi->drawLine(H(2), V(0), H(2), V(1), RGB565(0, 74, 241));
							vi->drawLine(H(0), V(2), H(1), V(2), RGB565(0, 74, 241));
							break;
						case ph:
							vi->drawLine(H(0), V(2), H(7), V(2), RGB565(0, 74, 241));
							vi->drawLine(H(0), V(5), H(7), V(5), RGB565(0, 74, 241));
							break;
						case pv:
							vi->drawLine(H(2), V(0), H(2), V(7), RGB565(0, 74, 241));
							vi->drawLine(H(5), V(0), H(5), V(7), RGB565(0, 74, 241));
							break;
						case tu:
							vi->drawLine(H(0), V(5), H(7), V(5), RGB565(0, 74, 241));
							vi->drawLine(H(0), V(2), H(1), V(2), RGB565(0, 74, 241));
							vi->drawLine(H(6), V(2), H(7), V(2), RGB565(0, 74, 241));
							vi->drawLine(H(2), V(0), H(2), V(1), RGB565(0, 74, 241));
							vi->drawLine(H(5), V(0), H(5), V(1), RGB565(0, 74, 241));
							break;
						case td:
							vi->drawLine(H(0), V(2), H(7), V(2), RGB565(0, 74, 241));
							vi->drawLine(H(0), V(5), H(1), V(5), RGB565(0, 74, 241));
							vi->drawLine(H(6), V(5), H(7), V(5), RGB565(0, 74, 241));
							vi->drawLine(H(2), V(6), H(2), V(7), RGB565(0, 74, 241));
							vi->drawLine(H(5), V(6), H(5), V(7), RGB565(0, 74, 241));
							break;
						case tl:
							vi->drawLine(H(5), V(0), H(5), V(7), RGB565(0, 74, 241));
							vi->drawLine(H(2), V(0), H(2), V(1), RGB565(0, 74, 241));
							vi->drawLine(H(0), V(2), H(1), V(2), RGB565(0, 74, 241));
							vi->drawLine(H(0), V(5), H(1), V(5), RGB565(0, 74, 241));
							vi->drawLine(H(2), V(6), H(2), V(7), RGB565(0, 74, 241));
							break;
						case tr:
							vi->drawLine(H(2), V(0), H(2), V(7), RGB565(0, 74, 241));
							vi->drawLine(H(5), V(0), H(5), V(1), RGB565(0, 74, 241));
							vi->drawLine(H(6), V(2), H(7), V(2), RGB565(0, 74, 241));
							vi->drawLine(H(6), V(5), H(7), V(5), RGB565(0, 74, 241));
							vi->drawLine(H(5), V(6), H(5), V(7), RGB565(0, 74, 241));
							break;
						case cu:
							vi->drawLine(H(2), V(0), H(2), V(3), RGB565(0, 74, 241));
							vi->drawLine(H(5), V(0), H(5), V(3), RGB565(0, 74, 241));
							vi->drawLine(H(3), V(4), H(4), V(4), RGB565(0, 74, 241));
							break;
						case cd:
							vi->drawLine(H(2), V(4), H(2), V(7), RGB565(0, 74, 241));
							vi->drawLine(H(5), V(4), H(5), V(7), RGB565(0, 74, 241));
							vi->drawLine(H(3), V(3), H(4), V(3), RGB565(0, 74, 241));
							break;
						case cl:
							vi->drawLine(H(0), V(2), H(3), V(2), RGB565(0, 74, 241));
							vi->drawLine(H(0), V(5), H(3), V(5), RGB565(0, 74, 241));
							vi->drawLine(H(4), V(3), H(4), V(4), RGB565(0, 74, 241));
							break;
						case cr:
							vi->drawLine(H(4), V(2), H(7), V(2), RGB565(0, 74, 241));
							vi->drawLine(H(4), V(5), H(7), V(5), RGB565(0, 74, 241));
							vi->drawLine(H(3), V(3), H(3), V(4), RGB565(0, 74, 241));
							break;
						default: break;
					}
				}

			}
		}
};

