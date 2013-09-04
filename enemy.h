/*
enemy.h
represents an enemy in the game

Enemy
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

    SDL_Rect* clips;

    int frame;

    int elapsed;
    int bullet_elapsed;

    int type;

    int x_vel, y_vel;

    int hit_points;

    Box* bounding_boxes;
    //20,1  23,59

    Bullet bullets[10];

    bool is_dead;

}Enemy;

void enemy_init( Enemy * e, int x, int y, int type )
{
	e->box.x = x;
	e->box.y = y;
	e->type = type;
	e->x_vel = 0;
	e->y_vel = 0;

	e->bounding_boxes = (Box *) malloc( 1 * sizeof(Box) );

	e->bounding_boxes[0].x = 20;
	e->bounding_boxes[0].y = 1;
	e->bounding_boxes[0].w = 23;
	e->bounding_boxes[0].h = 59;

	e->box.w = ENEMY_WIDTH;
	e->box.h = ENEMY_HEIGHT;

	e->hit_points = 100;

	int i;
	Bullet tmp;
	bullet_init( &tmp, 0, 0, -1, 0, 0 );
	for( i = 0; i < MAX_BULLETS; i++ )
	{
		e->bullets[i] = tmp;
	}

	e->frame = 0;
	e->is_dead = false;
	e->elapsed = 0;
	e->bullet_elapsed = 0;

	e->clips = (SDL_Rect *) malloc( 4 * sizeof(SDL_Rect) );

	e->clips[0].x = 0;
	e->clips[0].y = 128;
	e->clips[0].w = ENEMY_WIDTH;
	e->clips[0].h = ENEMY_HEIGHT;

	e->clips[1].x = ENEMY_WIDTH;
	e->clips[1].y = 128;
	e->clips[1].w = ENEMY_WIDTH;
	e->clips[1].h = ENEMY_HEIGHT;

	e->clips[2].x = ENEMY_WIDTH * 2;
	e->clips[2].y = 128;
	e->clips[2].w = ENEMY_WIDTH;
	e->clips[2].h = ENEMY_HEIGHT;

	e->clips[3].x = ENEMY_WIDTH * 3;
	e->clips[3].y = 128;
	e->clips[3].w = ENEMY_WIDTH;
	e->clips[3].h = ENEMY_HEIGHT;
}

void enemy_show( Enemy * e )
{
	if( !e->is_dead )
	{
		apply_surface( e->box.x, e->box.y, sprite_sheet, graphics.screen, &e->clips[ e->frame ] );
	}

	//show bullets
	int i;
	for( i = 0; i < MAX_BULLETS; i++ )
	{
		bullet_show( &e->bullets[i] );
	}
}

void enemy_shoot( Enemy * e, int player_x, int player_y )
{
	float x_vel, y_vel;
	float x, y, abs_x, abs_y;
	abs_x = 0;
	abs_y = 0;
	x = player_x - e->box.x;
	y = player_y - e->box.y;

	float sum = abs(x) + abs(y);

	y_vel = (y / sum) * (BULLET_VELOCITY * .7) * -1;
	x_vel = (x / sum) * (BULLET_VELOCITY * .7);

	//x_vel = rand() % 2;

	int i;
	for( i = 0; i < MAX_BULLETS; i++ )
	{
		if( e->bullets[i].type < 0 )
		{
			e->bullets[i].type = 1;
			e->bullets[i].box.x = e->box.x;
			e->bullets[i].box.y = e->box.y;
			e->bullets[i].y_vel = (int)y_vel;
			e->bullets[i].x_vel = (int)x_vel;
			break;
		}
	}
}

void enemy_move( Enemy * e, Uint32 deltaTicks, bool frozen )
{
	if( !frozen )
	{
		if( !e->is_dead )
		{
			e->bullet_elapsed += deltaTicks;
			if( e->bullet_elapsed > 2000 )
			{
				enemy_shoot( e, rand() % graphics.screen_width, graphics.screen_height - 65 );
				e->bullet_elapsed = 0;
			}

			e->elapsed += deltaTicks;
			if( e->elapsed > 175 )
			{
				e->frame++;
				e->elapsed = 0;
				if( e->frame > 3 )
				{
					e->frame = 0;
				}
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
		    if( e->box.y > LEVEL_HEIGHT )
		    {
		        //kill
		        e->is_dead = true;
		    }
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

void enemy_clean_up( Enemy * e )
{
    free( e->bounding_boxes );
    free( e->clips );
    e->bounding_boxes = NULL;
    e->clips = NULL;
}