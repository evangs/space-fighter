/*
star.h
represents a star in the game background

Star
	int x, y 	  //position of star

	init()
	show()
	update()
*/

typedef struct
{
    //ofsets
    int x, y;

}Star;

void star_init( Star * s )
{
	//set offsets
	s->x = rand() % LEVEL_WIDTH;
	s->y = ( rand() % LEVEL_HEIGHT ) * -1;
}

void star_show( Star * s )
{
	//Show image
    apply_surface( s->x, s->y, star, graphics.screen, NULL );
}

bool star_update( Star * s, Uint32 deltaTicks )
{
	//Move the star down
    s->y += STAR_VELOCITY * ( deltaTicks / 1000.f );
}