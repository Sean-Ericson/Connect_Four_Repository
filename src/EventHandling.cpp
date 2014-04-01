#include "GameIO.hpp"

/* External Variables */
extern sf::RenderWindow window;
extern sf::View view;
extern sf::Event event;
extern ConnectFourGame game;

int selectColPos = 3;

bool handleKeyPress(void)
{   
    if ( (event.key.code == sf::Keyboard::Left) && !game.roundIsWon() && 
         (selectColPos > 0) )
    {
        --selectColPos;
        return true;
    }
    
    if ( (event.key.code == sf::Keyboard::Right) && !game.roundIsWon() && 
         (selectColPos < 6) )
    {
        ++selectColPos;
        return true;
    }
    
    if ( (event.key.code == sf::Keyboard::Down) && !game.roundIsWon() )
    {
        if ( game.dropPieceAuto(selectColPos, game.getActivePlayer() ) )
        {
            dropAnim( game.board.getLastCol(), game.board.getLastRow(), 
                      game.getActivePlayer() );
            selectColPos = 3;
            game.switchPlayerTurn();
        }
    }
    
    if ( (event.key.code == sf::Keyboard::Space) && game.roundIsWon() )
    {
        clearBoardAnim();
        game.setNewRound();
    }
}

bool handleResize(void){
    
    bool limitWindowSize = true;
    int minWindowSize = 71, smallSide;
    
    if ( (limitWindowSize) && (event.size.height < minWindowSize) )
    {
        event.size.height = minWindowSize;
        window.setSize( sf::Vector2u(event.size.width, event.size.height) );
    }
    
    view.setSize(event.size.width, event.size.height);
    
    if (event.size.width <= event.size.height)
    {
        view.zoom(1.0 * InitWinX / event.size.width);
    }
    else
    {
        view.zoom(1.0 * InitWinY / event.size.height);
    }

    window.setView(view);
}

void AIturn(void)
{
    int tmpVar;
    
    if ( (game.getPlayerType(Player1) == AI) && 
         (game.getActivePlayer() == Player1) &&( !game.roundIsWon() ) )
    {
        tmpVar = game.AIselectMove();
        
        if ( (tmpVar > 6) || (tmpVar < 0) )
        {
            std::cout << "AIselectMove dun messed up." << std::endl;
            return;
        }
        
        game.dropPieceAuto(tmpVar, Player1);
        game.switchPlayerTurn();
    }
    else if ( (game.getPlayerType(Player2) == AI) && 
              (game.getActivePlayer() == Player2) &&( !game.roundIsWon() ) )
    {
        tmpVar = game.AIselectMove();
        
        if ( (tmpVar > 6) || (tmpVar < 0) )
        {
            std::cout << "AIselectMove dun messed up." << std::endl;
            return;
        }
        
        game.dropPieceAuto(tmpVar, Player2);
        game.switchPlayerTurn();
    }
    return;
}