/*
splash_screen.h
represents the splash screen

SplashScreen
	
	show()
	play()
*/

void splash_screen_show()
{
	//Fill screen
	SDL_FillRect( graphics.screen, &graphics.screen->clip_rect, SDL_MapRGB( graphics.screen->format, 0, 0, 0 ) );

	apply_surface( (graphics.screen_width - 400) / 2, (graphics.screen_height - 400) / 2, splash_screen, graphics.screen, NULL );
}

int splash_screen_play()
{
	bool exit_now = false;
	bool quit = false;
	bool paused = false;
	int elapsed = 0;

	//keeps track of time since last rendering
    Timer delta;
    TimerInit( &delta );

    //start delta timer
    TimerStart( &delta );

	//While the user hasn't quit
    while( quit == false )
    {
        //While there's event to handle
        while( SDL_PollEvent( &event ) )
        {
        	if( event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN )
        	{
        		quit = true;
        	}

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
                exit_now = true;
            }
        }

        elapsed += TimerGetTicks( &delta );

        if( elapsed > 3000 )
        {
        	quit = true;
        }

        //restart delta timer
    	TimerStart( &delta );

        //show the splash screen
        splash_screen_show();

	    //Update the screen
        if( SDL_Flip( graphics.screen ) == -1 )
        {
            return 1;
        }
    }
    if( exit_now )
    {
    	return -1;
    }

    return 0;
}