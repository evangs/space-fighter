/*
bullet.h
represents a bullet in the game

Bullet
	SDL_Rect box  //holds position(x,y) and size(w,h)
	int type      //value of -1 denotes that the bullet is dead
	int x_vel
	int y_vel

	init()
	show()
	update()
*/

typedef struct
{
    Box box;

    int type;

    int x_vel, y_vel;

}Bullet;

void bullet_init( Bullet * b, int x, int y, int type, int x_vel, int y_vel )
{
	b->box.x = x;
	b->box.y = y;
	b->box.w = BULLET_WIDTH;
	b->box.h = BULLET_HEIGHT;

	b->type = type;
	b->x_vel = x_vel;
	b->y_vel = y_vel;
}

void bullet_show( Bullet * b )
{
	if( b->type > -1 )
	{
		switch( b->type )
		{
			case 0: apply_surface( b->box.x, b->box.y, bullet, graphics.screen, NULL ); break;
			case 1: apply_surface( b->box.x, b->box.y, enemy_bullet, graphics.screen, NULL ); break;
            case 2: apply_surface( b->box.x, b->box.y, snot_bullet, graphics.screen, NULL ); break;
		}
	}
}

void bullet_update( Bullet * b, Uint32 deltaTicks )
{
	//Move the bullet left or right
    b->box.x += b->x_vel * ( deltaTicks / 1000.f );

    //If the bullet went too far to the left
    if( b->box.x < 0 )
    {
        //remove it
        b->type = -1;
    }
    //or the right
    else if( b->box.x + b->box.w > LEVEL_WIDTH )
    {
        //remove it
        b->type = -1;
    }

    //Move the bullet up or down
    b->box.y -= b->y_vel * ( deltaTicks / 1000.f );

    //If the bullet went too far up
    if( b->box.y < 0 )
    {
        //remove it
        b->type = -1;
    }
    //or down
    else if( b->box.y + b->box.h > LEVEL_HEIGHT )
    {
        //remove it
        b->type = -1;
    }
}