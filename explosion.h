/*
explosion.h
represents an explosion in the game

Explosion
	SDL_Rect box 	  //position and dimensions of explosion
	SDL_Rect clips[]  //the frames from the sprite sheet
	int frame
	bool is_dead      //true if the explosion is dead

	init()
	show()
	update()
*/

typedef struct
{
    //ofsets
    SDL_Rect box;

    SDL_Rect clips[3];

    int frame;

    bool is_dead;

    int elapsed;

}Explosion;

void explosion_init( Explosion * e, int x, int y, int w, int h )
{
	e->box.x = x;
	e->box.y = y;
	e->box.w = w;
	e->box.h = h;

	e->clips[0].x = 0;
	e->clips[0].y = 64;
	e->clips[0].w = w;
	e->clips[0].h = h;

	e->clips[1].x = w;
	e->clips[1].y = 64;
	e->clips[1].w = w;
	e->clips[1].h = h;

	e->clips[2].x = w * 2;
	e->clips[2].y = 64;
	e->clips[2].w = w;
	e->clips[2].h = h;

	e->frame = 0;
	e->elapsed = 0;

	e->is_dead = false;
}

void explosion_show( Explosion * e )
{
	//Show the explosion
	if( !e->is_dead )
	{
    	apply_surface( e->box.x, e->box.y, sprite_sheet, graphics.screen, &e->clips[ e->frame ] );
    }
}

void explosion_update( Explosion * e, Uint32 deltaTicks )
{
	e->elapsed += deltaTicks;
	if( e->elapsed > 160 )
	{
		e->frame++;
		e->elapsed = 0;
		if( e->frame > 2 )
		{
			e->is_dead = true;
		}
	}
}