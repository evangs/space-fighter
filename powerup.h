/*
powerup.h
represents a powerup in the game

Powerup
	SDL_Rect box 	  //position and dimensions of cloud
	int type
	bool is_dead      //true if the cloud is dead

	init()
	show()
	update()
*/

typedef struct
{
    //ofsets
    Box box;

    int type;

    int x_vel, y_vel;

    bool is_dead;

}Powerup;

void powerup_init( Powerup * p, int x, int y, int w, int h, int type )
{
	p->box.x = x;
	p->box.y = y;
	p->box.w = w;
	p->box.h = h;

	p->x_vel = 0;
	p->y_vel = 60;

	p->type = type;
	p->is_dead = false;
}

void powerup_show( Powerup * p )
{
	//Show the powerup
	if( !p->is_dead )
	{
		switch( p->type )
		{
    		case HEALTHPACK: apply_surface( p->box.x, p->box.y, healthpack, graphics.screen, NULL ); break;
    		case FREEZER: apply_surface( p->box.x, p->box.y, icicle, graphics.screen, NULL ); break;
    	}
    }
}

void powerup_update( Powerup * p, Uint32 deltaTicks )
{
	if( !p->is_dead )
	{
		//Move the powerup left or right
		//printf( "dts = %d\n", deltaTicks );
	    p->box.x += p->x_vel * ( deltaTicks / 1000.f );

	    //If the bullet went too far to the left
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

	    //Move the enemy up or down
	    p->box.y += p->y_vel * ( deltaTicks / 1000.f );

	    //If the enemy went too far down
	    if( p->box.y > LEVEL_HEIGHT )
	    {
	        //kill
	        p->is_dead = true;
	    }
	}
}