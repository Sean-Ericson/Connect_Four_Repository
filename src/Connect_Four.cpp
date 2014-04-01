#include "Connect_Four.hpp"
#include <iostream>

Board::Board()
{
    clearBoard();
    lastCol = -1;
    lastRow = -1;
}

bool Board::checkBoardFull()
{
    if (fullCol[0] && fullCol[1] && fullCol[2] && fullCol[3] && fullCol[4] &&
        fullCol[5] && fullCol[6])
    {
        boardFull = true;
        return true;
    }
    return false;
}

bool Board::clearBoard(void)
{
    for (register char a = 0; a < COLS; ++a)
    {
        for (register char b = 0; b < ROWS; ++b)
        {
            state[(int)a][(int)b] = NoPlayer;
        }
        fullCol[(int)a] = false;
    }

    empty = true;
    boardFull = false;
    return true;
}

bool Board::colIsFull(int col)
{
    if ( (col < 0) || (col >= COLS) )
    {
        return false;
    }

    return fullCol[col];
}

int Board::getPiece(int col, int row)
{
    if ( (col < 0) || (col >= COLS) || (row < 0) || (row >= ROWS) )
    {
        return -1;
    }

    return state[col][row];
}

bool Board::placePiece(int col, int row, int player, bool replace)
{
    if ( (col >= COLS) || (col < 0 ) ||
         (row >= ROWS) || (row < 0) ||
         (player < 1) || (player > 2) ||
         ( (!replace) && (state[col][row] != NoPlayer) ) )
    {
        return false;
    }

    state[col][row] = player;
    
    lastCol = col;
    lastRow = row;

    if (row == TOP_ROW)
    {
        fullCol[col] = true;
        checkBoardFull();
    }

    empty = false;
    return true;
}

ConnectFourGame::ConnectFourGame()
{
    p1Score = 0;
    p2Score = 0;
    round = 0;
    roundWinner = NoPlayer;
    p1Turn = true;
    p1Type = HUMAN;
    p2Type = HUMAN;
    roundWon = false;

    setNewRound();
}

bool ConnectFourGame::addPoints(int player, int points)
{
    if ( (player < Player1) || (player > Player2) )
    {
        return false;
    }

    if (player == Player1)
    {
        p1Score += points;
    }

    if (player == Player2)
    {
        p2Score += points;
    }

    return true;
}

int ConnectFourGame::checkWin(int player, int col, int row)
{
    int score = 0;

    if( (board.getPiece(col, row-1) == player) &&  // x
        (board.getPiece(col, row-2) == player) &&  // o
        (board.getPiece(col, row-3) == player) ){  // o

        ++score;
    }///////////////////////////////////////////////////////////////////////////
    if( (board.getPiece(col-1, row-1) == player) &&  //    x
        (board.getPiece(col-2, row-2) == player) &&  //   o
        (board.getPiece(col-3, row-3) == player) ){  //  o
                                                     // o
        ++score;
    }
    if( (board.getPiece(col+1, row+1) == player) &&  //    o
        (board.getPiece(col-1, row-1) == player) &&  //   x
        (board.getPiece(col-2, row-2) == player) ){  //  o
                                                     // o
        ++score;
    }
    if( (board.getPiece(col+1, row+1) == player) &&  //    o
        (board.getPiece(col+2, row+2) == player) &&  //   o
        (board.getPiece(col-1, row-1) == player) ){  //  x
                                                     // o
        ++score;
    }
    if( (board.getPiece(col+1, row+1) == player) &&  //    o
        (board.getPiece(col+2, row+2) == player) &&  //   o
        (board.getPiece(col+3, row+3) == player) ){  //  o
                                                     // x
        ++score;
    } //////////////////////////////////////////////////////////////////////////
    if( (board.getPiece(col+1, row-1) == player) &&  // x
        (board.getPiece(col+2, row-2) == player) &&  //  o
        (board.getPiece(col+3, row-3) == player) ){  //   o
                                                     //    o
        ++score;
    }
    if( (board.getPiece(col-1, row+1) == player) &&  // o
        (board.getPiece(col+1, row-1) == player) &&  //  x
        (board.getPiece(col+2, row-2) == player) ){  //   o
                                                     //    o
        ++score;
    }
    if( (board.getPiece(col-1, row+1) == player) &&  // o
        (board.getPiece(col-2, row+2) == player) &&  //  o
        (board.getPiece(col+1, row-1) == player) ){  //   x
                                                     //    o
        ++score;
    }
    if( (board.getPiece(col-1, row+1) == player) &&  // o
        (board.getPiece(col-2, row+2) == player) &&  //  o
        (board.getPiece(col-3, row+3) == player) ){  //   o
                                                     //    x
        ++score;
    } //////////////////////////////////////////////////////////////////////////
    if( (board.getPiece(col+1, row) == player) &&  // x o o o
        (board.getPiece(col+2, row) == player) &&  //
        (board.getPiece(col+3, row) == player) ){  //
                                                   //
        ++score;
    }
    if( (board.getPiece(col-1, row) == player) &&  // o x o o
        (board.getPiece(col+1, row) == player) &&  //
        (board.getPiece(col+2, row) == player) ){  //
                                                   //
        ++score;
    }
    if( (board.getPiece(col-1, row) == player) &&  // o o x o
        (board.getPiece(col-2, row) == player) &&  //
        (board.getPiece(col+1, row) == player) ){  //
                                                   //
        ++score;
    }
    if( (board.getPiece(col-1, row) == player) &&  // o o o x
        (board.getPiece(col-2, row) == player) &&  //
        (board.getPiece(col-3, row) == player) ){  //
                                                   //
        ++score;
    }
    
    if (score > 0)
    {
        return score;
    }

    if ( board.boardIsFull() )
    {
        return Tie;
    }

    return NoPlayer;
}

bool ConnectFourGame::dropPiece(int col, int player)
{   
    int result;
    
    if (board.boardIsFull() || board.colIsFull(col))
    {
        return false;
    }

    for (register char r = 0; r < ROWS; ++r)
    {
        if ( board.placePiece(col, r, player, false) )
        {
            break;
        }
    }
}

bool ConnectFourGame::dropPieceAuto(int col, int player)
{
    int result;
    
    if ( !dropPiece(col, player) )
    {
        return false;
    }
    
    result = checkWin(getActivePlayer(), board.getLastCol(),board.getLastRow());
    
    if (result == NoPlayer)
    {
        return true;
    }
    
    if (result == Tie)
    {
        setRoundWinner(Tie);
    }
    
    if (result > 0)
    {
        addPoints(getActivePlayer(), result);
        setRoundWinner( getActivePlayer() );
    }
    
    return true;
}

int ConnectFourGame::getScore(int player)
{
    if (player == Player1)
    {
        return p1Score;
    }
    else if (player == Player2)
    {
        return p2Score;
    }
    
    return -1;
}

bool ConnectFourGame::setNewRound(void)
{
    board.clearBoard();
    ++round;
    roundWinner = NoPlayer;
    roundWon = false;
    p1Turn = ( (round % 2) == 1);
    newRound = true;
    return true;
}

void ConnectFourGame::setPlayerType(int player, bool type)
{
    if (player == Player1)
    {
        p1Type = type;
    }
    
    if (player == Player2)
    {
        p2Type = type;
    }
}

bool ConnectFourGame::setRoundWinner(int winner)
{
    if ( (winner < Tie) || (winner > Player2) )
    {
        return false;
    }
    
    roundWinner = winner;
    roundWon = true;
    
    return true;
}

void ConnectFourGame::updateState()
{
    int tmpVar;
    if ( p1Turn && (p1Type == AI) )
    {
        if (!roundWon)
        {
            tmpVar = AIselectMove();
            dropPieceAuto( tmpVar, getActivePlayer() );
        }
    }
}