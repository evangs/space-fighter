/*
cloud.h
represents a dust cloud in the game

Cloud
	SDL_Rect box 	  //position and dimensions of cloud
	SDL_Rect clips[]  //the frames from the sprite sheet
	int frame
	bool is_dead      //true if the cloud is dead

	init()
	show()
	update()
*/

typedef struct
{
    //ofsets
    SDL_Rect box;

    SDL_Rect clips[2];

    int frame;

    bool is_dead;

    int elapsed;

}Cloud;

void cloud_init( Cloud * c, int x, int y, int w, int h )
{
	c->box.x = x;
	c->box.y = y;
	c->box.w = w;
	c->box.h = h;

	c->clips[0].x = 0;
	c->clips[0].y = 0;
	c->clips[0].w = w;
	c->clips[0].h = h;

	c->clips[1].x = w;
	c->clips[1].y = 0;
	c->clips[1].w = w;
	c->clips[1].h = h;

	c->frame = 0;
	c->elapsed = 0;

	c->is_dead = false;
}

void cloud_show( Cloud * c )
{
	//Show the cloud
	if( !c->is_dead )
	{
    	apply_surface( c->box.x, c->box.y, cloud, graphics.screen, &c->clips[ c->frame ] );
    }
}

void cloud_update( Cloud * c, Uint32 deltaTicks )
{
	c->elapsed += deltaTicks;
	if( c->elapsed > 100 )
	{
		c->frame++;
		c->elapsed = 0;
		if( c->frame > 2 )
		{
			c->is_dead = true;
		}
	}
}