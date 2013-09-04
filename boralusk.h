/*
boralusk.h
represents a boralusk in the game

Boralusk
	SDL_Rect box  //holds position(x,y) and size(w,h)
	int type      //value of -1 denotes that the player is dead
	int x_vel
	int y_vel
	int hit_points
	Bullet bullets[]

	init()
	show()
	move()
	shoot()
*/

typedef struct
{
    Box box;

    int bullet_elapsed;

    int x_vel, y_vel;

    int hit_points;

    Box* bounding_boxes;

    Bullet bullets[10];

    bool is_dead;

}Boralusk;

void boralusk_init( Boralusk * e )
{
	e->box.x = ( graphics.screen_width - BORALUSK_WIDTH ) / 2;
	e->box.y = -1 * BORALUSK_HEIGHT;
	e->x_vel = 0;
	e->y_vel = 50;

	e->bounding_boxes = (Box *) malloc( 2 * sizeof(Box) );

	e->bounding_boxes[0].x = 0;
	e->bounding_boxes[0].y = 0;
	e->bounding_boxes[0].w = 431;
	e->bounding_boxes[0].h = 137;

	e->bounding_boxes[1].x = 160;
	e->bounding_boxes[1].y = 142;
	e->bounding_boxes[1].w = 100;
	e->bounding_boxes[1].h = 49;

	e->box.w = BORALUSK_WIDTH;
	e->box.h = BORALUSK_HEIGHT;

	e->hit_points = 1000;

	int i;
	Bullet tmp;
	bullet_init( &tmp, 0, 0, -1, 0, 0 );
	for( i = 0; i < MAX_BULLETS; i++ )
	{
		e->bullets[i] = tmp;
	}

	e->is_dead = false;
	e->bullet_elapsed = 0;
}

void boralusk_show( Boralusk * e )
{
	if( !e->is_dead )
	{
		apply_surface( e->box.x, e->box.y, boralusk, graphics.screen, NULL );
	}

	//show bullets
	int i;
	for( i = 0; i < MAX_BULLETS; i++ )
	{
		bullet_show( &e->bullets[i] );
	}
}

void boralusk_shoot( Boralusk * e )
{
	float x_vel, y_vel;
	float x, y, abs_x, abs_y;
	abs_x = 0;
	abs_y = 0;
	int j;
	for( j = 0; j < 5; j++ )
	{
		x = ( graphics.screen_width / 5.f * j ) - ( e->box.w / 2.f );
		y = ( graphics.screen_height - 64 ) - ( e->box.h / 2.f );

		float sum = abs(x) + abs(y);

		y_vel = (y / sum) * (BULLET_VELOCITY * .7) * -1;
		x_vel = (x / sum) * (BULLET_VELOCITY * .7);

		//x_vel = rand() % 2;

		int i;
		for( i = 0; i < MAX_BULLETS; i++ )
		{
			if( e->bullets[i].type < 0 )
			{
				e->bullets[i].type = 2;
				e->bullets[i].box.x = e->box.x + ( e->box.w / 2 );
				e->bullets[i].box.y = e->box.y = ( e->box.h - 20 );
				e->bullets[i].box.w = 20;
				e->bullets[i].box.h = 20;
				e->bullets[i].y_vel = (int)y_vel;
				e->bullets[i].x_vel = (int)x_vel;
				break;
			}
		}
	}
}

void boralusk_move( Boralusk * e, Uint32 deltaTicks )
{
	if( !e->is_dead )
	{
		e->bullet_elapsed += deltaTicks;
		if( e->bullet_elapsed > 5000 )
		{
			boralusk_shoot( e );
			e->bullet_elapsed = 0;
		}

		//Move the player left or right
		//printf( "dts = %d\n", deltaTicks );
	    e->box.x += e->x_vel * ( deltaTicks / 1000.f );

	    //If the bullet went too far to the left
	    if( e->box.x < 0 )
	    {
	        //move it back
	        e->box.x = 0;
	    }
	    //or the right
	    else if( e->box.x + e->box.w > LEVEL_WIDTH )
	    {
	        //move back
	        e->box.x = LEVEL_WIDTH - e->box.w;
	    }

	    //Move the enemy up or down
	    e->box.y += e->y_vel * ( deltaTicks / 1000.f );

	    //If the enemy went too far down
	    if( e->box.y > 0 )
	    {
			e->box.y = 0;
			e->y_vel = 0;
	    }
	}

    //update bullets
    int i;
    for( i = 0; i < MAX_BULLETS; i++ )
    {
    	if( e->bullets[i].type > -1 )
    	{
    		bullet_update( &e->bullets[i], deltaTicks );
    	}
    }
}

void boralusk_clean_up( Boralusk * e )
{
    free( e->bounding_boxes );
    e->bounding_boxes = NULL;
}