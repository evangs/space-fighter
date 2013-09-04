/*
particle.h
represents a particle used in particle effects

Particle
	int x, y 	  //position of particle
	int frame
	int type

	init()
	show()
	is_dead()
*/

typedef struct
{
    //ofsets
    int x, y;

    //current frame of animation
    int frame;

    //type of particle
    int type;

}Particle;

void particle_init( Particle * p, int x, int y )
{
	//set offsets
	int ypo = 64 + ( rand() % 15 );
	p->y = y + ypo;
	if( ypo < 70 )
	{
		p->x = x + 18 + ( rand() % 27 );
	}
	else
	{
		p->x = x + 24 + ( rand() % 15 );
	}

	//initialize animation
	p->frame = rand() % 5;

	//set type
	switch( rand() % 3 )
	{
		case 0: p->type = 0; break;
		case 1: p->type = 1; break;
		case 2: p->type = 2; break;
	}
}

void particle_show( Particle * p )
{
	//Show image
    apply_surface( p->x, p->y, particle, graphics.screen, NULL );

    //Animate
    p->frame++;
}

bool particle_is_dead( Particle * p )
{
	if( p->frame > 2 )
    {
        return true;
    }

    return false;
}