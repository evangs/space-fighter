/*
player.h
represents the player in the game

Player
	SDL_Rect box  //holds position(x,y) and size(w,h)
	int type      //value of -1 denotes that the player is dead
	int score
	int x_vel
	int y_vel
	int hit_points
	Bullet bullets[]
	Particle particles[]

	init()
	show()
	move()
	handle_input()
	shoot()
*/

typedef struct
{
    Box box;

    int type;

    int score;

    int x_vel, y_vel;

    int hit_points;

    Box* bounding_boxes;
    //24,1  15,41
    //2,45  59,11
    //9,15  5,31
    //49,15  5,31

    Bullet* bullets;

    //the particles
    Particle* particles;

}Player;

void player_init( Player * p, int x, int y, int type )
{
    p->bounding_boxes = (Box *) malloc( 4 * sizeof(Box) );

	p->bounding_boxes[0].x = 24;
	p->bounding_boxes[0].y = 1;
	p->bounding_boxes[0].w = 15;
	p->bounding_boxes[0].h = 41;

	p->bounding_boxes[1].x = 2;
	p->bounding_boxes[1].y = 45;
	p->bounding_boxes[1].w = 59;
	p->bounding_boxes[1].h = 11;

	p->bounding_boxes[2].x = 9;
	p->bounding_boxes[2].y = 15;
	p->bounding_boxes[2].w = 5;
	p->bounding_boxes[2].h = 31;

	p->bounding_boxes[3].x = 49;
	p->bounding_boxes[3].y = 15;
	p->bounding_boxes[3].w = 5;
	p->bounding_boxes[3].h = 31;

	p->box.x = x;
	p->box.y = y;
	p->box.w = PLAYER_WIDTH;
	p->box.h = PLAYER_HEIGHT;

	p->type = type;
	p->score = 0;
	p->x_vel = 0;
	p->y_vel = 0;
	p->hit_points = 100;

    p->bullets = (Bullet *) malloc( MAX_BULLETS * sizeof(Bullet) ); 

	int i;
	for( i = 0; i < MAX_BULLETS; i++ )
	{
		bullet_init( &p->bullets[i], 0, 0, -1, 0, 0 );
	}

    p->particles = (Particle *) malloc( MAX_PARTICLES * sizeof(Particle) ); 

	//initialize the particles
    for( i = 0; i < MAX_PARTICLES; i++ )
    {
        Particle tempParticle;
        particle_init( &tempParticle, x, y );
        p->particles[i] =  tempParticle;
    }
}

void player_show_particles( Player * p )
{
    //Go through particles
    int i;
    for( i = 0; i < MAX_PARTICLES; i++ )
    {
        //Delete and replace dead particles
        if( particle_is_dead( &p->particles[i] ) == true )
        {
            Particle tempParticle;
            particle_init( &tempParticle,  p->box.x, p->box.y );
            p->particles[i] = tempParticle;
        }
    }

    //Show particles
    for( i = 0; i < MAX_PARTICLES; i++ )
    {
        particle_show( &p->particles[i] );
    }
}

void player_show( Player * p )
{
	if( p->type > -1 )
	{
		apply_surface( p->box.x, p->box.y, player, graphics.screen, NULL );

		player_show_particles( p );

		//show bullets
		int i;
		for( i = 0; i < MAX_BULLETS; i++ )
		{
			bullet_show( &p->bullets[i] );
		}

        if( p->hit_points > 50 )
        {
            //show shield
            //Set surface alpha
            int modifier = ( p->hit_points / 50 - 2 ) * 50;
            if( modifier > 100 )
            {
                modifier = 100;
            }
            SDL_SetAlpha( shield, SDL_SRCALPHA, 50 + modifier );
            apply_surface( p->box.x, p->box.y , shield, graphics.screen, NULL );
        }
	}
}

void player_shoot( Player * p, int side )
{
    //play sound effect
    if( Mix_PlayChannel( -1, pew, 0 ) == -1 )
    {
        printf("error playing sfx pew");
    }

	if( p->type > -1 )
	{
		int i;
		for( i = 0; i < MAX_BULLETS; i++ )
		{
			if( p->bullets[i].type < 0 )
			{
				p->bullets[i].type = 0;
				if( side == 0 )
				{
					p->bullets[i].box.x = p->box.x + 10;
				}
				else
				{
					p->bullets[i].box.x = p->box.x + 50;
				}
				p->bullets[i].box.y = p->box.y + 15;
				p->bullets[i].y_vel = BULLET_VELOCITY;
				break;
			}
		}
	}
}

void player_move( Player * p, Uint32 deltaTicks )
{
	//Move the player left or right
    p->box.x += p->x_vel * ( deltaTicks / 1000.f );

    //If the player went too far to the left
    if( p->box.x < 0 )
    {
        //move it back
        p->box.x = 0;
    }
    //or the right
    else if( p->box.x + p->box.w > LEVEL_WIDTH )
    {
        //move back
        p->box.x = LEVEL_WIDTH - p->box.w;
    }

    //update bullets
    int i;
    for( i = 0; i < MAX_BULLETS; i++ )
    {
    	bullet_update( &p->bullets[i], deltaTicks );
    }
}

void player_handle_input( Player * p )
{
    //if a key is pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        if( event.key.keysym.sym == controls.left )
        {
            p->x_vel -= PLAYER_VELOCITY;
        }
        else if( event.key.keysym.sym == controls.right )
        {
            p->x_vel += PLAYER_VELOCITY;
        }
        else if( event.key.keysym.sym == controls.shoot_left )
        {
            player_shoot( p, 0 );
        }
        else if( event.key.keysym.sym == controls.shoot_right )
        {
        	player_shoot( p, 1 );
        }
    }   

    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        //Adjust the velocity
        if( event.key.keysym.sym == controls.left )
        {
            p->x_vel += PLAYER_VELOCITY;
        }
        else if( event.key.keysym.sym == controls.right )
        {
        	p->x_vel -= PLAYER_VELOCITY;
        }
    } 
}

void player_clean_up( Player * p )
{
    free(p->bullets);
    free(p->particles);
    free(p->bounding_boxes);
    p->bullets = NULL;
    p->particles = NULL;
    p->bounding_boxes = NULL;
}