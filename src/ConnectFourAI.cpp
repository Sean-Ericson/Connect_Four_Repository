#include "Connect_Four.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

int totalNodes = 0, totalActions = 0;

Action::Action()
{
    std::cout << "Constructing action with no parameters" << std::endl;
    ++totalActions;
    std::cout << "total actions: " << totalActions << std::endl;
}
    
Action::Action(int m)
    :move(m)
{
    std::cout << "Constructing action with a move parameter: " << m;
    std::cout << std::endl;
    ++totalActions;
    std::cout << "total actions: " << totalActions << std::endl;
}
    
Action::Action(int m, int v)
    :move(m), value(v)
{
    std::cout << "Constructing action with a move parameter: " << m;
    std::cout << " and a value parameter: " << v << std::endl;
    ++totalActions;
    std::cout << "total actions: " << totalActions << std::endl;
}
    
Action::~Action()
{
    std::cout << "Destructing an action"  << std::endl;
    --totalActions;
    std::cout << "total actions: " << totalActions << std::endl;
}
    
Node::Node()
{
    std::cout << "Constructing a Node with no parameters" << std::endl;
    ++totalNodes;
    std::cout << "total nodes: " << totalNodes << std::endl;
}

Node::Node(const Node& sourceNode)
{
    std::cout << "Constructing a Node with the copy constructor" << std::endl;
    ++totalNodes;
    std::cout << "total nodes: " << totalNodes << std::endl;
    
    memcpy( state, sourceNode.state, (sizeof(int) * 7 * 6) );
    
    AIplayer = sourceNode.AIplayer;
    curPlayer = sourceNode.curPlayer;
    lcol = sourceNode.lcol;
    lrow = sourceNode.lrow;
    actions = sourceNode.actions;
}

Node::~Node()
{
    std::cout << "Destructing a Node" << std::endl;
    --totalNodes;
    std::cout << "total nodes: " << totalNodes << std::endl;
    
    for (size_t a = 0; a < actions.size(); ++a)
    {
        std::cout << "deleting and action as part of the node deconstructor" << std::endl;
        delete actions[a];
    }
}

Node& Node::operator=(const Node& sourceNode)
{
    std::cout << "Using Node's = operator" << std::endl;
    
    memcpy( state, sourceNode.state, (sizeof(int) * 7 * 6) );
    
    AIplayer = sourceNode.AIplayer;
    curPlayer = sourceNode.curPlayer;
    lcol = sourceNode.lcol;
    lrow = sourceNode.lrow;
    actions = sourceNode.actions;
}

int ConnectFourGame::AIselectMove(void)
{
    std::cout << "*******************************************************" << std::endl;
    std::cout << "Begining execution of AIselectMove()" << std::endl;
    std::cout << "The AI is acting as player " << getActivePlayer() << std::endl << std::endl;
    
    Node* pInitialNode = new Node;
    Node* pNewNode;
    short int highestVal = -100;
    std::vector<short int> bestMoves;
    
    if (roundWon) // this will make sure the AI is not trying to make a move from an already winning position
    {
        return -1;
    }
    
    srand( time(NULL) );
    rand();
    
    pInitialNode->curPlayer = getActivePlayer();
    pInitialNode->AIplayer = pInitialNode->curPlayer;
    
    for (register short int col = 0; col < 7; ++col)
    {
        for (register short int row = 0; row < 6; ++row)
        {
            pInitialNode->state[col][row] = board.getPiece(col, row);
        }
        
        if (pInitialNode->state[col][TOP_ROW] == NoPlayer)
        {
            std::cout << "Adding column " << col << " to possible moves" << std::endl;
            
            pInitialNode->actions.push_back( new Action(col) );
            pNewNode = makeMove(pInitialNode, col);
            std::cout << "GUNNA TRY TA ADD DAT VALUE"<< std::endl;
            pInitialNode->actions[pInitialNode->actions.size() - 1]->value = moveVal(pNewNode, 1, 1);
            
            std::cout << "Column " << col << " has a value of " << pInitialNode->actions[col]->value << std::endl;;
            
            delete pNewNode;
            
            if (pInitialNode->actions[col]->value > highestVal)
            {
                highestVal = pInitialNode->actions[col]->value;
            }
        }
    }
    
    std::cout << "The hightest move value was " << highestVal << std::endl;
    
    for (register short int a = 0; a < pInitialNode->actions.size(); ++a)
    {
        if (pInitialNode->actions[a]->value == highestVal)
        {
            bestMoves.push_back(pInitialNode->actions[a]->move);
        }
    }
    
    delete pInitialNode;
    
    std::cout << "The amount of moves with this value is " << bestMoves.size() << std::endl;
    
    if (bestMoves.size() == 0)
        return -1;
    
    std::cout << "Ending function AIselectMove()" << std::endl;
    
    return bestMoves[RandFun( 0, (bestMoves.size() - 1) )];
    
}

int ConnectFourGame::moveVal(Node* pNode, int curDepth, int maxDepth)
{
    std::cout << std::endl << "Begining execution of function moveVal" << std::endl;
    std::cout << "The current depth is " << curDepth << std::endl << std::endl;
    
    short int curVal, highestVal = -100, lastPlayer;
    Node* pNewNode;
    
    curVal = AIcheckWin(pNode->state, pNode->curPlayer, pNode->lcol, pNode->lrow);
    
    std::cout << "The checkWin result of this node is " << curVal << std::endl;
    
    if (curVal != 0)
    {
        if (curVal == Tie)
        {
            return 0;
        }
        
        return (pNode->curPlayer == pNode->AIplayer) ? curVal : (-1 * curVal); // man I need to add comments
    }
    
    if (curDepth == maxDepth)
    {
        return curVal;
    }
    
    pNode->curPlayer = (pNode->curPlayer == Player1) ? Player2 : Player1;
    
    if (pNode->curPlayer != pNode->AIplayer)
    {
        highestVal = 100;
    }
    
    for (register short int a = 0; a < COLS; ++a)
    {
        if (pNode->state[a][TOP_ROW] == NoPlayer)
        {
            pNode->actions.push_back( new Action(a) );
            pNewNode = makeMove(pNode, a);
            pNode->actions[pNode->actions.size() - 1]->value = moveVal(pNewNode, (curDepth + 1), maxDepth);
            
            delete pNewNode;
            
            if ( (pNode->curPlayer == pNode->AIplayer) && 
                 (pNode->actions[a]->value > highestVal) )
            {
                highestVal = pNode->actions[a]->value;
            }
            
            if ( (pNode->curPlayer != pNode->AIplayer) && 
                 (pNode->actions[a]->value < highestVal) )
            {
                highestVal = pNode->actions[a]->value;
            }
        }
    }
    
    std::cout << "returning from moveVal()" << std::endl << std::endl;
    
    return highestVal;
}

Node* ConnectFourGame::makeMove(Node* pPassedNode, int col)
{
    Node* pReturnNode = new Node(*pPassedNode);
    
    for (register int a = 0; a < ROWS; ++a)
    {
        if (pReturnNode->state[col][a] == NoPlayer)
        {
            pReturnNode->state[col][a] = pReturnNode->curPlayer;
            pReturnNode->lcol = col;
            pReturnNode->lrow = a;
            break;
        }
    }
    
    pReturnNode->actions.clear();
    
    return pReturnNode;
}

int ConnectFourGame::AIcheckWin(int board[][6], int player, int col, int row)
{
    int score = 0;

    if( (board[col][row-1] == player) &&  // x
        (board[col][row-2] == player) &&  // o
        (board[col][row-3] == player) ){  // o

        ++score;
    }///////////////////////////////////////////////////////////////////////////
    if( (board[col-1][row-1] == player) &&  //    x
        (board[col-2][row-2] == player) &&  //   o
        (board[col-3][row-3] == player) ){  //  o
                                            // o
        ++score;
    }
    if( (board[col+1][row+1] == player) &&  //    o
        (board[col-1][row-1] == player) &&  //   x
        (board[col-2][row-2] == player) ){  //  o
                                            // o
        ++score;
    }
    if( (board[col+1][row+1] == player) &&  //    o
        (board[col+2][row+2] == player) &&  //   o
        (board[col-1][row-1] == player) ){  //  x
                                            // o
        ++score;
    }
    if( (board[col+1][row+1] == player) &&  //    o
        (board[col+2][row+2] == player) &&  //   o
        (board[col+3][row+3] == player) ){  //  o
                                            // x
        ++score;
    } //////////////////////////////////////////////////////////////////////////
    if( (board[col+1][row-1] == player) &&  // x
        (board[col+2][row-2] == player) &&  //  o
        (board[col+3][row-3] == player) ){  //   o
                                            //    o
        ++score;
    }
    if( (board[col-1][row+1] == player) &&  // o
        (board[col+1][row-1] == player) &&  //  x
        (board[col+2][row-2] == player) ){  //   o
                                            //    o
        ++score;
    }
    if( (board[col-1][row+1] == player) &&  // o
        (board[col-2][row+2] == player) &&  //  o
        (board[col+1][row-1] == player) ){  //   x
                                            //    o
        ++score;
    }
    if( (board[col-1][row+1] == player) &&  // o
        (board[col-2][row+2] == player) &&  //  o
        (board[col-3][row+3] == player) ){  //   o
                                            //    x
        ++score;
    } //////////////////////////////////////////////////////////////////////////
    if( (board[col+1][row] == player) &&  // x o o o
        (board[col+2][row] == player) &&  //
        (board[col+3][row] == player) ){  //
                                          //
        ++score;
    }
    if( (board[col-1][row] == player) &&  // o x o o
        (board[col+1][row] == player) &&  //
        (board[col+2][row] == player) ){  //
                                          //
        ++score;
    }
    if( (board[col-1][row] == player) &&  // o o x o
        (board[col-2][row] == player) &&  //
        (board[col+1][row] == player) ){  //
                                          //
        ++score;
    }
    if( (board[col-1][row] == player) &&  // o o o x
        (board[col-2][row] == player) &&  //
        (board[col-3][row] == player) ){  //
                                          //
        ++score;
    }
    
    if (score > 0)
    {
        return score;
    }

    if ( (board[0][TOP_ROW] != NoPlayer) &&
         (board[1][TOP_ROW] != NoPlayer) &&
         (board[2][TOP_ROW] != NoPlayer) &&
         (board[3][TOP_ROW] != NoPlayer) &&
         (board[4][TOP_ROW] != NoPlayer) &&
         (board[5][TOP_ROW] != NoPlayer) &&
         (board[6][TOP_ROW] != NoPlayer) )
    {
        return Tie;
    }

    return NoPlayer;
}

int ConnectFourGame::RandFun( int min, int max )
{
    if( min > max ){
        
        return (min + 1);
    }
    
    return (int)( ( ( rand() / (RAND_MAX * 1.0) ) * (1 + max - min) ) + min );
}

