/*
round.h
represents a round in the game

load enemy positions and velocities from the level file
when an enemy comes on screen it moves at the velocity from the file

enemy x y x_vel y_vel type
powerup x y x_vel y_vel type

Round
	Enemy enemies[]
	Background
	File level

	init()
*/

typedef struct
{
    Enemy enemies[MAX_ENEMIES];


}Round;

void round_init( Round * r )
{
	int i;
	Enemy tmp;
	enemy_init( &tmp, 0, 0, 0 );
	for( i = 0; i < MAX_ENEMIES; i++ )
	{
		r->enemies[i] = tmp;
	}
}