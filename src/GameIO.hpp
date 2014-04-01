/* 
 * File:   GameIO.hpp
 * Author: anonymous
 *
 * Created on March 13, 2014, 5:49 PM
 */

#ifndef GAMEIO_HPP
#define	GAMEIO_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "Connect_Four.hpp"
#include <iostream>
#include <cstdio>
#include <string>

const int InitWinX = 712;
const int InitWinY = 440;

const int BoardPosX = 10;
const int BoardPosY = 70;
const int BoardWidth = 420;
const int BoardHeight = 360;

/* Event Handling */
bool handleResize(void);
bool handleKeyPress(void);
void AIturn(void);

/* Drawing */
bool updateWindow(void);
void AIselectAnim(int col);
void drawBoard(void);
bool drawColSelect();
bool drawScoreBox(void);
bool drawWinText(int player);
bool dropAnim(int col, int row, int player);
bool clearBoardAnim(void);
std::string intToStr(int num);

#endif	/* GAMEIO_HPP */