/*
 * File:   Connect_Four.hpp
 * Author: Sean Ericson
 *
 * Created on March 10, 2014, 6:53 AM
 */

#ifndef CONNECT_FOUR_HPP
#define	CONNECT_FOUR_HPP

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int COLS = 7;
const int ROWS = 6;
const int TOP_ROW = 5;

const bool HUMAN = true;
const bool AI = false;

enum Players
{
    Tie = -1,
    NoPlayer,
    Player1,
    Player2,
};

class Action
{
public:
    int move;
    int value;
    
    Action();
    
    Action(int m);
    
    Action(int m, int v);
    
    ~Action();
};

class Node
{
public:
    
    int state[7][6];
    int AIplayer, curPlayer, lcol, lrow;
    std::vector<Action*> actions;
    
    Node();
    Node(const Node& sourceNode);
    ~Node();
    Node& operator=(const Node& sourceNode);
};

class Board
{
private:
    int state[COLS][ROWS];
    bool fullCol[COLS];
    bool empty, boardFull;
    int lastCol, lastRow;

public:
    Board();
    bool checkBoardFull(void);
    bool clearBoard(void);
    bool colIsFull(int col);
    int getPiece(int col, int row);
    bool placePiece(int col, int row, int player, bool replace);

    bool boardIsFull(void){ return boardFull; }
    //int (*getBoardState(void))[ROWS]{ return state; }
    int getLastCol(void){ return lastCol; }
    int getLastRow(void){ return lastRow; }
    bool isEmpty(void){ return empty; }
};

class ConnectFourGame
{
private:
    int p1Score, p2Score, round, roundWinner;
    bool AImoved, p1Turn, p1Type, p2Type, roundWon, newRound;

public:
    Board board;

    ConnectFourGame();
    bool addPoints(int player, int points);
    int checkWin(int player, int col, int row);
    bool dropPiece(int col, int player);
    bool dropPieceAuto(int col, int player);
    int getScore(int player);
    bool setNewRound(void);
    void setPlayerType(int player, bool type);
    bool setRoundWinner(int winner);
    void updateState(void);
    
    /* AI functions */
    int AIselectMove(void);
    int moveVal(Node* pNode, int curDepth, int maxDepth);
    Node* makeMove(Node* pPassedNode, int col);
    int AIcheckWin(int board[][6], int player, int col, int row);
    int RandFun(int min, int max);

    int getActivePlayer(void){ return (p1Turn) ? Player1 : Player2; }
    bool getPlayerType(int player){return (player == Player1)? p1Type : p2Type;}
    int getRounds(void){ return round; }
    int getRoundWinner(void){ return roundWinner; }
    bool isNewRound(void){ return newRound; }
    bool isP1Turn(void){ return p1Turn; }
    bool roundIsWon(void){ return roundWon; }
    void switchPlayerTurn(void){ p1Turn = !p1Turn; }
    //void setRoundWon(bool won){ roundWon = won; }
};

int AImain(ConnectFourGame game, int AIplayer);

#endif	/* CONNECT_FOUR_HPP */
