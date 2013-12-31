/*
alien_fighter.h
represents an alien fighter in the game

Enemy
	SDL_Rect box  //holds position(x,y) and size(w,h)
	int type      //value of -1 denotes that the alien fighter is dead
	int x_vel
	int y_vel
	int hit_points
	Bullet bullets[]
	AeonFlux fluxleft
	AeonFlux fluxright

	init()
	show()
	move()
	shoot()
*/

typedef struct
{
    Box box;

    SDL_Rect* clips;

    int frame;

    int elapsed;

    int bullet_elapsed;

    int type;

    int x_vel, y_vel;

    int hit_points;

    Box* bounding_boxes;
    //7,12  49,58

    Bullet bullets[2];

    bool is_dead;

}SpaceFighter;

void space_fighter_init( SpaceFighter * s, int x, int y, int type )
{
	s->box.x = x;
	s->box.y = y;
	s->type = type;
	s->x_vel = 0;
	s->y_vel = 0;

	s->bounding_boxes = (Box *) malloc( 1 * sizeof(Box) );

	s->bounding_boxes[0].x = 7;
	s->bounding_boxes[0].y = 12;
	s->bounding_boxes[0].w = 49;
	s->bounding_boxes[0].h = 46;

	s->box.w = 64;
	s->box.h = 64;

	s->hit_points = 150;

	int i;
	Bullet tmp;
	bullet_init( &tmp, 0, 0, -1, 0, 0 );
	for( i = 0; i < 2; i++ )
	{
		s->bullets[i] = tmp;
	}

	s->frame = 0;
	s->is_dead = false;
	s->elapsed = 0;
	s->bullet_elapsed = 0;

	s->clips = (SDL_Rect *) malloc( 2 * sizeof(SDL_Rect) );

	s->clips[0].x = 0;
	s->clips[0].y = 0;
	s->clips[0].w = 64;
	s->clips[0].h = 64;

	s->clips[1].x = 64;
	s->clips[1].y = 0;
	s->clips[1].w = 64;
	s->clips[1].h = 64;
}

void space_fighter_show( SpaceFighter * s )
{
	if( !s->is_dead )
	{
		apply_surface( s->box.x, s->box.y, alien_fighter, graphics.screen, &s->clips[ s->frame ] );
	}

	//show bullets
	int i;
	for( i = 0; i < 2; i++ )
	{
		bullet_show( &s->bullets[i] );
	}
}

void space_fighter_shoot( SpaceFighter * s, int player_x, int player_y )
{
	float x_vel, y_vel;
	float x, y, abs_x, abs_y;
	abs_x = 0;
	abs_y = 0;
	x = player_x - s->box.x;
	y = player_y - s->box.y;

	float sum = abs(x) + abs(y);

	y_vel = (y / sum) * (BULLET_VELOCITY * .9) * -1;
	x_vel = (x / sum) * (BULLET_VELOCITY * .9);

	//x_vel = rand() % 2;

	int i;
	for( i = 0; i < 2; i++ )
	{
		if( s->bullets[i].type < 0 )
		{
			s->bullets[i].type = 1;
			s->bullets[i].box.x = s->box.x + 32;
			s->bullets[i].box.y = s->box.y + 64;
			s->bullets[i].y_vel = (int)y_vel;
			s->bullets[i].x_vel = (int)x_vel;
			break;
		}
	}
}

void space_fighter_move( SpaceFighter * s, Uint32 deltaTicks, bool frozen )
{
	if( !frozen )
	{
		if( !s->is_dead )
		{
			s->bullet_elapsed += deltaTicks;
			if( s->bullet_elapsed > 500 )
			{
				space_fighter_shoot( s, rand() % graphics.screen_width, graphics.screen_height - 65 );
				s->bullet_elapsed = 0;
			}

			s->elapsed += deltaTicks;
			if( s->elapsed > 100 )
			{
				s->frame++;
				s->elapsed = 0;
				if( s->frame > 1 )
				{
					s->frame = 0;
				}
			}

			//Move the alien fighter left or right
			//printf( "dts = %d\n", deltaTicks );
		    s->box.x += s->x_vel * ( deltaTicks / 1000.f );

		    //If the bullet went too far to the left
		    if( s->box.x < 0 )
		    {
		        //move it back
		        s->box.x = 0;
		    }
		    //or the right
		    else if( s->box.x + s->box.w > LEVEL_WIDTH )
		    {
		        //move back
		        s->box.x = LEVEL_WIDTH - s->box.w;
		    }

		    //Move the enemy up or down
		    s->box.y += s->y_vel * ( deltaTicks / 1000.f );

		    //If the enemy went too far down
		    if( s->box.y > LEVEL_HEIGHT )
		    {
		        //kill
		        s->is_dead = true;
		    }
		}
	}

    //update bullets
    int i;
    for( i = 0; i < 2; i++ )
    {
    	if( s->bullets[i].type > -1 )
    	{
    		bullet_update( &s->bullets[i], deltaTicks );
    	}
    }
}

void space_fighter_clean_up( SpaceFighter * s )
{
    free( s->bounding_boxes );
    free( s->clips );
    s->bounding_boxes = NULL;
    s->clips = NULL;
}