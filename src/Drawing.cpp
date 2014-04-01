#include "GameIO.hpp"

extern sf::RenderWindow window;
extern sf::View view;
extern sf::Event event;
extern ConnectFourGame game;

extern int selectColPos;

sf::Color p1Color = sf::Color::Red;
sf::Color p2Color = sf::Color::Black;
const sf::Color BackGroundColor = sf::Color(185,122,87);

bool updateWindow(void)
{   
    window.clear(sf::Color::Cyan);
    
    sf::RectangleShape backGround( sf::Vector2f(InitWinX, InitWinY) );
    backGround.setFillColor(BackGroundColor);
    window.draw(backGround);
    
    drawBoard();
    drawScoreBox();
    
    if (!game.roundIsWon())
    {
        drawColSelect();
    }
    else
    {
        drawWinText( game.getRoundWinner() );
    }
    
    window.display();
}

void drawBoard(void)
{
    sf::RectangleShape gameBoard( sf::Vector2f(BoardWidth, BoardHeight) );
    gameBoard.setPosition(BoardPosX, BoardPosY);
    gameBoard.setFillColor(sf::Color::Blue);
    window.draw(gameBoard);
    
    sf::CircleShape circle(20.0, 100);
    
    for (int a = 0; a < 7; ++a)
    {
        for (int b = 0; b < 6; ++b)
        {
            if (game.board.getPiece(a, b) == NoPlayer)
            {
                circle.setFillColor(BackGroundColor);
            }
            else if (game.board.getPiece(a, b) == Player1)
            {
                circle.setFillColor(p1Color);
            }
            else if (game.board.getPiece(a, b) == Player2)
            {
                circle.setFillColor(p2Color);
            }
            
            circle.setPosition( BoardPosX + 10 + (60 * a), 
                                BoardPosY + 310 - (60 * b) );
            window.draw(circle);
        }
    }
}

bool drawColSelect()
{
    sf::CircleShape piece(20.0,100);
    
    if (game.getActivePlayer() == Player1)
    {
        piece.setFillColor(p1Color);
    }
    else
    {
        piece.setFillColor(p2Color);
    }
    
    piece.setPosition(BoardPosX + 10 + (60 * selectColPos), 20);
    window.draw(piece);
}

bool drawScoreBox(void)
{
    sf::RectangleShape box( sf::Vector2f(252, 420) );
    sf::Font font;
    sf::Text text;
    
    box.setPosition(450, 10);
    box.setFillColor(BackGroundColor);
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(-2.0);
    window.draw(box);
    
    box.setSize( sf::Vector2f(252, 60) );
    window.draw(box);
    
    if (!font.loadFromFile("Font/COMMUNIS.ttf"))
    {
        return false;
    }
    
    text.setString( "Round " + intToStr( game.getRounds() ) );
    text.setFont(font);
    text.setStyle(sf::Text::Regular);
    text.setCharacterSize(32);
    text.setColor(sf::Color::Black);
    text.setPosition(460, 20); 
    window.draw(text);
    
    text.setString("Player 1's Score:");
    text.setColor(p1Color);
    text.setCharacterSize(24);
    text.setPosition(460, 77);
    window.draw(text);
    
    text.setString( intToStr( game.getScore(Player1) ) );
    text.setCharacterSize(20);
    text.setPosition(460, 112);
    window.draw(text);
    
    text.setString("Player 2's Score:");
    text.setColor(p2Color);
    text.setCharacterSize(24);
    text.setPosition(460, 155);
    window.draw(text);
    
    text.setString( intToStr( game.getScore(Player2) ) );
    text.setCharacterSize(20);
    text.setPosition(460, 190);
    window.draw(text);
}

bool drawWinText(int player)
{
    sf::Font font;
    sf::Text text;
    
    if ( (player < Tie) || (player > Player2) || 
          !font.loadFromFile("Font/COMMUNIS.ttf") )
    {
        return false;
    }
    
    text.setFont(font);
    text.setStyle(sf::Text::Regular);
    text.setCharacterSize(32);
    text.setPosition(110, 10);
    
    switch (player)
    {
    case Player1:
        text.setColor(p1Color);
        text.setString("Player 1 Wins!");
        break;
    
    case Player2:
        text.setColor(p2Color);
        text.setString("Player 2 Wins!");
        break;
        
    case Tie:
        text.setColor(sf::Color::Black);
        text.setPosition(145, 10);
        text.setString("Tie Game!");
        break;
    }
    
    window.draw(text);
    
    text.setString("Press Space to Continue");
    text.setColor(sf::Color::Black);
    text.setCharacterSize(15);
    text.setPosition(136, 45);
    
    window.draw(text);
    
    return true;
}

bool dropAnim(int col, int row, int player)
{
    sf::Clock timingClock;
    sf::Time deltaTime = sf::milliseconds(100);
    sf::CircleShape piece(20.0, 100), blank(20.0, 100);
    
    if ( (player != Player1) && (player != Player2) )
    {
        return false;
    }
    
    piece.setFillColor( (player == Player1) ? p1Color : p2Color );
    blank.setFillColor(BackGroundColor);
    
    for (register char a = TOP_ROW; a >= row; --a)
    {
        timingClock.restart();
        
        piece.setPosition( BoardPosX + 10 + (60 * col), 
                           BoardPosY + 310 - (60 * a) );
        window.draw(piece);
        window.display();
        
        blank.setPosition( BoardPosX + 10 + (60 * col), 
                           BoardPosY + 310 - (60 * (a + 1) ) );
        window.draw(blank);
        window.display();
        
        while (timingClock.getElapsedTime() < deltaTime){}
    }
    
}

bool clearBoardAnim(void)
{
    
}

std::string intToStr(int num)
{
    char buff[4];
    sprintf( buff, "%d", num);
    return buff;
}