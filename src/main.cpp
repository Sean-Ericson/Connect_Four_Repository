/*
 * File:   main.cpp
 * Author: anonymous
 *
 * Created on March 10, 2014, 6:52 AM
 */

#include "GameIO.hpp"
#define yolo main

/* Project Global Variables */
sf::RenderWindow window(sf::VideoMode(InitWinX, InitWinY, 32), "Connect Four");
sf::View view( sf::Vector2f(InitWinX / 2, InitWinY / 2), 
               sf::Vector2f(InitWinX, InitWinY) );
sf::Event event;
ConnectFourGame game;
//////////////////////////////

 yolo( void )
{
    sf::Image icon;
    if ( icon.loadFromFile( "weed.bmp" ) )
    {
        window.setIcon(48, 48, icon.getPixelsPtr() );
    }
    
    window.setKeyRepeatEnabled(false);
    window.setView(view);
    
    game.setPlayerType(Player2, AI);
    
    updateWindow();
    
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            
            if (event.type == sf::Event::Resized)
            {
                handleResize();
            }
            
            if (event.type == sf::Event::KeyPressed)
            {
                handleKeyPress();
            }
            
            AIturn();
            
            updateWindow();
            
            while (window.pollEvent(event)){}
        }
        
        
    }
    
    return 0;
}
