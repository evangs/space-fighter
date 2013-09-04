//collision.h

int check_collision( Box A, Box bounding_a[], int bounding_length, Box B )
{
	if( check_collision_rect( A, B ) )
	{
		// check each bounding rect
		int i;
		for( i = 0; i < bounding_length; i++ )
		{
			Box tmp;
			tmp.x = A.x + bounding_a[i].x;
			tmp.y = A.y + bounding_a[i].y;
			tmp.w = bounding_a[i].w;
			tmp.h = bounding_a[i].h;
			if( check_collision_rect( tmp, B ) )
			{
				return true;
			}
		}
		return false;
	}
	return false;
}

int check_collision_rect( Box A, Box B )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return 0;
    }

    if( topA >= bottomB )
    {
        return 0;
    }

    if( rightA <= leftB )
    {
        return 0;
    }

    if( leftA >= rightB )
    {
        return 0;
    }

    //If none of the sides from A are outside B
    return 1;
}