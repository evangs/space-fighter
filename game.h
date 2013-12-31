/*
game.h
represents the game

Game
	Player player
	bool game_over

	initialize()
	play()
*/

typedef struct
{
    //the player
    Player player;

    //the stars
    Star* stars;

    //the explosions
    Explosion* explosions;

    //the clouds
    Cloud* clouds;

    //the enemies
    Enemy* enemies;

    //the space_fighters
    SpaceFighter* space_fighters;

    //boralusk
    Boralusk boralusk;

    //the powerups
    Powerup* powerups;

    //bool to keep track if game is over or not
    bool game_over;

}Game;

void GameInit( Game * g )
{
	g->game_over = false;
	player_init( &g->player, ( graphics.screen_width - PLAYER_WIDTH ) / 2, graphics.screen_height - 130, 0 );

	g->stars = (Star *) malloc( MAX_STARS * sizeof(Star) );

	//init stars
	int i;
	for( i = 0; i < MAX_STARS; i++ )
	{
		star_init( &g->stars[i] );
	}

	g->explosions = (Explosion *) malloc( MAX_EXPLOSIONS * sizeof(Explosion) );

	for( i = 0; i < MAX_EXPLOSIONS; i++ )
	{
		explosion_init( &g->explosions[i], -64, -64, 64, 64 );
		g->explosions[i].is_dead = true;
	}

	g->clouds = (Cloud *) malloc( MAX_CLOUDS * sizeof(Cloud) );

	for( i = 0; i < MAX_CLOUDS; i++ )
	{
		cloud_init( &g->clouds[i], -16, -16, 16, 16 );
		g->clouds[i].is_dead = true;
	}

	g->enemies = (Enemy *) malloc( 5 * sizeof(Enemy) );

	for( i = 0; i < 5; i++ )
	{
		enemy_init( &g->enemies[i], rand() % graphics.screen_width, (rand() % 200 + 64) * -1, 0 );
		g->enemies[i].y_vel = 40;
	}

	g->space_fighters = (SpaceFighter *) malloc( 2 * sizeof(SpaceFighter) );

	for( i = 0; i < 2; i++ )
	{
		space_fighter_init( &g->space_fighters[i], rand() % graphics.screen_width, (rand() % 200 + 64) * -1, 0 );
		g->space_fighters[i].y_vel = 30;
		g->space_fighters[i].x_vel = ( rand() % 41 ) - 20;
	}

	g->powerups = (Powerup *) malloc( MAX_POWERUPS * sizeof(Powerup) );

	for( i = 0; i < MAX_POWERUPS; i++ )
	{
		powerup_init( &g->powerups[i], rand() % graphics.screen_width, (rand() % 200 + 64) * -1, 16, 16, HEALTHPACK );
		g->powerups[i].is_dead = true;
	}
	printf("done init");
}

int GamePlay( Game * g )
{
	bool exit_now = false;
	bool quit = false;
	bool paused = false;
	bool frozen = false;
	int frozen_elapsed = 0;

	boralusk_init( &g->boralusk );
	g->boralusk.is_dead = true;

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
        	if( event.type == SDL_KEYDOWN )
        	{
        		if( event.key.keysym.sym == controls.pause )
        		{
        			paused = paused ? false : true;
        		}
        	}

        	//handle player input
        	player_handle_input( &g->player );

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
                exit_now = true;
            }
        }

        if ( !paused ) 
        {
        	// if( Mix_PausedMusic() )
        	// {
        	// 	Mix_ResumeMusic();
        	// }

        	if( frozen )
        	{
        		frozen_elapsed += TimerGetTicks( &delta );
        		if( frozen_elapsed > 5000 )
        		{
        			frozen = false;
        		}	
        	}

        	if( g->player.score % 26 == 25 )
        	{
        		int i;
        		for( i = 0; i < MAX_POWERUPS; i++ )
        		{
        			if( g->powerups[i].is_dead )
        			{
        				powerup_init( &g->powerups[i], rand() % graphics.screen_width, (rand() % 200 + 64) * -1, 32, 32, HEALTHPACK );
        				g->player.score++;
        				break;
        			}
        		}
        	}

        	if( g->player.score % 36 == 35 )
        	{
        		int i;
        		for( i = 0; i < MAX_POWERUPS; i++ )
        		{
        			if( g->powerups[i].is_dead )
        			{
        				powerup_init( &g->powerups[i], rand() % graphics.screen_width, (rand() % 200 + 64) * -1, 32, 32, FREEZER );
        				g->player.score++;
        				break;
        			}
        		}
        	}

        	//reinit dead enemies
        	if( g->player.score < BORALUSK_LEVEL )
        	{
	        	int d;
	        	for( d = 0; d < 5; d++ )
	        	{
	        		if( g->enemies[d].is_dead )
	        		{
	        			int f;
	        			bool b_alive = false;
	        			for( f = 0; f < MAX_BULLETS; f++ )
	        			{
	        				if( g->enemies[d].bullets[f].type > -1 )
	        				{
	        					b_alive = true;
	        					break;
	        				}
	        			}
	        			if( !b_alive )
	        			{
	               			enemy_init( &g->enemies[d], rand() % graphics.screen_width, -200, 0 );
							g->enemies[d].y_vel = 40;
						}
					}
				}
			}
			if( g->player.score < BORALUSK_LEVEL )
        	{
	        	int d;
	        	for( d = 0; d < 2; d++ )
	        	{
	        		if( g->space_fighters[d].is_dead )
	        		{
	        			int f;
	        			bool b_alive = false;
	        			for( f = 0; f < 2; f++ )
	        			{
	        				if( g->space_fighters[d].bullets[f].type > -1 )
	        				{
	        					b_alive = true;
	        					break;
	        				}
	        			}
	        			if( !b_alive )
	        			{
	               			space_fighter_init( &g->space_fighters[d], rand() % graphics.screen_width, -200, 0 );
							g->space_fighters[d].y_vel = 30;
							g->space_fighters[d].x_vel = ( rand() % 41 ) - 20;
						}
					}
				}
			}
			//if all enemies are dead spawn the boralusk
			if( g->boralusk.is_dead )
			{
				bool live_enemy = false;
				int i;
				for( i = 0; i < 5; i++ )
				{
					if( !g->enemies[i].is_dead )
					{
						live_enemy = true;
						break;
					}
				}
				if( !live_enemy )
				{
					//play sound effect
				    if( Mix_PlayChannel( -1, boss_music, 0 ) == -1 )
				    {
				        printf("error playing sfx boss music");
				    }
					boralusk_init( &g->boralusk );
					g->player.score += 10;
				}
			}

        	//remove dead stars and create new ones
        	int i;
        	for( i = 0; i < MAX_STARS; i++ )
        	{
        		if( g->stars[i].y > graphics.screen_height )
        		{
        			star_init( &g->stars[i] );
        		}
        	}

        	//upate the stars
        	for( i = 0; i < MAX_STARS; i++ )
        	{
        		star_update( &g->stars[i], TimerGetTicks( &delta ) );
        	}

        	for( i = 0; i < MAX_EXPLOSIONS; i++ )
        	{
        		if( !g->explosions[i].is_dead )
        		{
        			explosion_update( &g->explosions[i], TimerGetTicks( &delta ) );
        		}
        	}

        	for( i = 0; i < MAX_CLOUDS; i++ )
        	{
        		if( !g->clouds[i].is_dead )
        		{
        			cloud_update( &g->clouds[i], TimerGetTicks( &delta ) );
        		}
        	}

        	//update powerups
        	for( i = 0; i < MAX_POWERUPS; i++ )
        	{
        		if( !g->powerups[i].is_dead )
        		{
        			powerup_update( &g->powerups[i], TimerGetTicks( &delta ) );
        		}
        	}

        	//update the enemies
        	for( i = 0; i < 5; i++ )
        	{
        		enemy_move( &g->enemies[i], TimerGetTicks( &delta ), frozen );
        	}
        	for( i = 0; i < 2; i++ )
        	{
        		space_fighter_move( &g->space_fighters[i], TimerGetTicks( &delta ), frozen );
        	}

        	//update the boralusk
        	boralusk_move( &g->boralusk, TimerGetTicks( &delta ) );

	        //move the player
	        player_move( &g->player, TimerGetTicks( &delta ) );

	        //check for collisions between bullets and enemies
	        for( i = 0; i < MAX_BULLETS; i++ )
	        {
	        	if( g->player.bullets[i].type > -1 )
	        	{
		        	int j;
		        	for( j = 0; j < 5; j++ )
		        	{
		        		if( !g->enemies[j].is_dead )
		        		{
			        		if( check_collision( g->enemies[j].box, g->enemies[j].bounding_boxes, 1, g->player.bullets[i].box ) )
			        		{
			        			g->player.bullets[i].type = -1;
			        			g->enemies[j].hit_points -= 25;
			        			int l;
			        			for( l = 0; l < MAX_CLOUDS; l++ )
			        			{
			        				if( g->clouds[l].is_dead )
			        				{
			        					cloud_init( &g->clouds[l], g->player.bullets[i].box.x - 8, g->player.bullets[i].box.y - 8, 16, 16 );
			        					break;
			        				}
			        			}
			        			if( g->enemies[j].hit_points < 1 )
			        			{
				        			g->enemies[j].is_dead = true;
				        			g->player.score++;
				        			
				        			int k;
				        			for( k = 0; k < MAX_EXPLOSIONS; k++ )
				        			{
				        				if( g->explosions[k].is_dead )
				        				{
				        					//play sound effect
										    if( Mix_PlayChannel( -1, explode, 0 ) == -1 )
										    {
										        printf("error playing sfx explode");
										    }
				        					explosion_init( &g->explosions[k], g->enemies[j].box.x, g->enemies[j].box.y, 64, 64 );
				        					break;
				        				}
				        			}
				        		}
			        		}
			        	}
		        	}
		        }
	        }
	        for( i = 0; i < MAX_BULLETS; i++ )
	        {
	        	if( g->player.bullets[i].type > -1 )
	        	{
		        	int j;
		        	for( j = 0; j < 2; j++ )
		        	{
		        		if( !g->space_fighters[j].is_dead )
		        		{
			        		if( check_collision( g->space_fighters[j].box, g->space_fighters[j].bounding_boxes, 1, g->player.bullets[i].box ) )
			        		{
			        			g->player.bullets[i].type = -1;
			        			g->space_fighters[j].hit_points -= 25;
			        			int l;
			        			for( l = 0; l < MAX_CLOUDS; l++ )
			        			{
			        				if( g->clouds[l].is_dead )
			        				{
			        					cloud_init( &g->clouds[l], g->player.bullets[i].box.x - 8, g->player.bullets[i].box.y - 8, 16, 16 );
			        					break;
			        				}
			        			}
			        			if( g->space_fighters[j].hit_points < 1 )
			        			{
				        			g->space_fighters[j].is_dead = true;
				        			g->player.score += 5;
				        			
				        			int k;
				        			for( k = 0; k < MAX_EXPLOSIONS; k++ )
				        			{
				        				if( g->explosions[k].is_dead )
				        				{
				        					//play sound effect
										    if( Mix_PlayChannel( -1, explode, 0 ) == -1 )
										    {
										        printf("error playing sfx explode");
										    }
				        					explosion_init( &g->explosions[k], g->space_fighters[j].box.x, g->space_fighters[j].box.y, 64, 64 );
				        					break;
				        				}
				        			}
				        		}
			        		}
			        	}
		        	}
		        }
	        }

	        //check for collisions between player bullets and boralusk
	        for( i = 0; i < MAX_BULLETS; i++ )
	        {
	        	if( g->player.bullets[i].type > -1 )
	        	{
	        		if( !g->boralusk.is_dead )
	        		{
		        		if( check_collision( g->boralusk.box, g->boralusk.bounding_boxes, 1, g->player.bullets[i].box ) )
		        		{
		        			g->player.bullets[i].type = -1;
		        			g->boralusk.hit_points -= 25;
		        			int l;
		        			for( l = 0; l < MAX_CLOUDS; l++ )
		        			{
		        				if( g->clouds[l].is_dead )
		        				{
		        					cloud_init( &g->clouds[l], g->player.bullets[i].box.x - 8, g->player.bullets[i].box.y - 8, 16, 16 );
		        					break;
		        				}
		        			}
		        			if( g->boralusk.hit_points < 1 )
		        			{
			        			g->boralusk.is_dead = true;
			        			g->player.score += 125;
			        			
			        			int k;
			        			for( k = 0; k < MAX_EXPLOSIONS; k++ )
			        			{
			        				if( g->explosions[k].is_dead )
			        				{
			        					//play sound effect
									    if( Mix_PlayChannel( -1, explode, 0 ) == -1 )
									    {
									        printf("error playing sfx explode");
									    }
			        					explosion_init( &g->explosions[k], g->boralusk.box.x, g->boralusk.box.y, 64, 64 );
			        					break;
			        				}
			        			}
			        		}
		        		}
		        	}
		        }
	        }

	        //check for collisions between player and enemy bullets
	        if( g->player.type > -1 )
	        {
		        for( i = 0; i < 5; i++ )
		        {
		        	int j;
		        	for( j = 0; j < MAX_BULLETS; j++ )
		        	{
		        		if( g->enemies[i].bullets[j].type > -1 )
		        		{
			        		if( check_collision( g->player.box, g->player.bounding_boxes, 4, g->enemies[i].bullets[j].box ) )
			        		{
			        			g->player.hit_points -= 50;
			        			g->enemies[i].bullets[j].type = -1;
			        			int l;
			        			for( l = 0; l < MAX_CLOUDS; l++ )
			        			{
			        				if( g->clouds[l].is_dead )
			        				{
			        					cloud_init( &g->clouds[l], g->enemies[i].bullets[j].box.x - 8, g->enemies[i].bullets[j].box.y - 8, 16, 16 );
			        					break;
			        				}
			        			}
			        			if( g->player.hit_points < 1 )
			        			{
			        				g->player.type = -1;
			        				int k;
				        			for( k = 0; k < MAX_EXPLOSIONS; k++ )
				        			{
				        				if( g->explosions[k].is_dead )
				        				{
				        					//play sound effect
										    if( Mix_PlayChannel( -1, explode, 0 ) == -1 )
										    {
										        printf("error playing sfx explode");
										    }
				        					explosion_init( &g->explosions[k], g->player.box.x, g->player.box.y, 64, 64 );
				        					quit = true;
				        					break;
				        				}
				        			}
			        			}
			        		}
			        	}
		        	}
		        }
		    }

		    //check for collisions between player and enemy bullets
	        if( g->player.type > -1 )
	        {
		        for( i = 0; i < 2; i++ )
		        {
		        	int j;
		        	for( j = 0; j < 2; j++ )
		        	{
		        		if( g->space_fighters[i].bullets[j].type > -1 )
		        		{
			        		if( check_collision( g->player.box, g->player.bounding_boxes, 4, g->space_fighters[i].bullets[j].box ) )
			        		{
			        			g->player.hit_points -= 50;
			        			g->space_fighters[i].bullets[j].type = -1;
			        			int l;
			        			for( l = 0; l < MAX_CLOUDS; l++ )
			        			{
			        				if( g->clouds[l].is_dead )
			        				{
			        					cloud_init( &g->clouds[l], g->space_fighters[i].bullets[j].box.x - 8, g->space_fighters[i].bullets[j].box.y - 8, 16, 16 );
			        					break;
			        				}
			        			}
			        			if( g->player.hit_points < 1 )
			        			{
			        				g->player.type = -1;
			        				int k;
				        			for( k = 0; k < MAX_EXPLOSIONS; k++ )
				        			{
				        				if( g->explosions[k].is_dead )
				        				{
				        					//play sound effect
										    if( Mix_PlayChannel( -1, explode, 0 ) == -1 )
										    {
										        printf("error playing sfx explode");
										    }
				        					explosion_init( &g->explosions[k], g->player.box.x, g->player.box.y, 64, 64 );
				        					quit = true;
				        					break;
				        				}
				        			}
			        			}
			        		}
			        	}
		        	}
		        }
		    }

		    //check for collisions between player and powerups
	        if( g->player.type > -1 )
	        {
		        for( i = 0; i < MAX_POWERUPS; i++ )
		        {
		        	if( !g->powerups[i].is_dead )
	        		{
		        		if( check_collision( g->player.box, g->player.bounding_boxes, 4, g->powerups[i].box ) )
		        		{
		        			switch( g->powerups[i].type )
		        			{
		        				case HEALTHPACK: g->player.hit_points += 50; break;
		        				case FREEZER: frozen = true; frozen_elapsed = 0; break;
		        			}
		        			g->powerups[i].is_dead = true;
		        		}
		        	}
		        }
		    }

	        //restart delta timer
        	TimerStart( &delta );

	        //check for collisions

	        //Fill screen
	    	SDL_FillRect( graphics.screen, &graphics.screen->clip_rect, SDL_MapRGB( graphics.screen->format, 0, 0, 0 ) );

	    	//show the stars
	    	for( i = 0; i < MAX_STARS; i++ )
        	{
        		star_show( &g->stars[i] );
        	}

        	//show the enemies
        	for( i = 0; i < 5; i++ )
        	{
        		enemy_show( &g->enemies[i] );
        	}
        	for( i = 0; i < 2; i++ )
        	{
        		space_fighter_show( &g->space_fighters[i] );
        	}

        	//show the boralusk
        	boralusk_show( &g->boralusk );

	        //show the player
	        player_show( &g->player );

	        //show the explosions
	        for( i = 0; i < MAX_EXPLOSIONS; i++ )
	        {
	        	if( !g->explosions[i].is_dead )
	        	{
	        		explosion_show( &g->explosions[i] );
	        	}
	        }

	        //show the explosions
	        for( i = 0; i < MAX_CLOUDS; i++ )
	        {
	        	if( !g->clouds[i].is_dead )
	        	{
	        		cloud_show( &g->clouds[i] );
	        	}
	        }

	        //show the powerups
	        for( i = 0; i < MAX_POWERUPS; i++ )
	        {
	        	if( !g->powerups[i].is_dead )
	        	{
	        		powerup_show( &g->powerups[i] );
	        	}
	        }

	        //show the score
	        char score[100];
		    sprintf( score, "%d", g->player.score );

		    score_msg = TTF_RenderText_Solid( font, score, text_color );
		    apply_surface( ( graphics.screen_width - ( score_msg->w + 10) ), 10, score_msg, graphics.screen, NULL );
	    }
	    else
	    {
	    	//pause the music
	    	// Mix_PauseMusic();

	    	//Fill screen
	    	SDL_FillRect( graphics.screen, &graphics.screen->clip_rect, SDL_MapRGB( graphics.screen->format, 0x80, 0x80, 0x80 ) );
	    	
	    	message = TTF_RenderText_Solid( font, "Paused", text_color );
	    	apply_surface( ( graphics.screen_width - message->w ) / 2, ( graphics.screen_height - message->h ) / 2, message, graphics.screen, NULL );

	    	//restart delta timer
        	TimerStart( &delta );
	    }

	    //Update the screen
        if( SDL_Flip( graphics.screen ) == -1 )
        {
            return 1;
        }
    }

    quit = false;
    while( !quit )
    {
	    while( SDL_PollEvent( &event ) )
	    {
	    	if( event.type == SDL_MOUSEBUTTONDOWN )
	    	{
	    		quit = true;
	    	}

	    	//handle player input
	    	player_handle_input( &g->player );

	        //If the user has Xed out the window
	        if( event.type == SDL_QUIT )
	        {
	            //Quit the program
	            quit = true;
	            exit_now = true;
	        }
	    }
	}

    if( exit_now )
    {
    	return -1;
    }

    //if high score, get players name
 //    if( g->score > highscores.scores[0].score )
 //    {
 //    	quit = false;

 //    	//keep track of whether or not the user has entered their name
 //    	bool nameEntered = false;

	//     //the gets the user's name
	//     StringInput name;
	//     StringInputInit( &name );

	//     //set the message
	//     message = TTF_RenderText_Solid( font, "High Score! Enter Your Name:", text_color );

	//     //While the user hasn't quit
	//     while( !quit )
	//     {
	//         //while there's events to handle
	//         while( SDL_PollEvent( &event ) )
	//         {
	//             //If the user hasn't entered their name yet
	//             if( nameEntered == false )
	//             {
	//                 //Get user input
	//                 StringInputHandleInput( &name );

	//                 //If the enter key was pressed
	//                 if( ( event.type == SDL_KEYDOWN ) && ( event.key.keysym.sym == SDLK_RETURN ) )
	//                 {
	//                     //Change the flag
	//                     nameEntered = true;
	//                     quit = true;
	//                 }
	//             }

	//             //If the user has Xed out the window
	//             if( event.type == SDL_QUIT )
	//             {
	//                 //Quit the program
	//                 quit = true;
	//             }
	//         }

	//         //Fill screen
 //    		SDL_FillRect( graphics.screen, &graphics.screen->clip_rect, SDL_MapRGB( graphics.screen->format, 0x80, 0x80, 0x80 ) );

	//         //Show the message
	//         apply_surface( ( graphics.screen_width - message->w ) / 2, ( ( graphics.screen_height / 2 ) - message->h ) / 2, message, graphics.screen, NULL );

	//         //Show the name on the screen
	//         StringInputShowCentered( &name );

	//         //update the screen
	//         if( SDL_Flip( graphics.screen ) == -1 )
	//         {
	//             return true;
	//         }
	//     }

	//     Highscore hs;
	//     hs.score = g->score;
	//     strcpy( hs.name, name.str );
	//     highscores_add_score( &highscores, hs );

	//     StringInputDel( &name );
	// }

    return 0;
}

void game_clean_up( Game * g )
{
	player_clean_up( &g->player );

	int i;
	for(i = 0; i < 5; i++)
	{
		enemy_clean_up( &g->enemies[i] );
	}
	for(i = 0; i < 2; i++)
	{
		space_fighter_clean_up( &g->space_fighters[i] );
	}

	boralusk_clean_up( &g->boralusk );

	free( g->stars );
	free( g->explosions );
	free( g->clouds );
	free( g->enemies );
	free( g->space_fighters );
	free( g->powerups );
	g->stars = NULL;
	g->explosions = NULL;
	g->clouds = NULL;
	g->enemies = NULL;
	g->space_fighters = NULL;
	g->powerups = NULL;   
}