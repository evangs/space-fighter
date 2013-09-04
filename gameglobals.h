/*
gameglobals.h
global variables used in the game
*/

//Screen attributes
Graphics graphics;

//segment width and height
#define SEGMENT_WIDTH 16
#define SEGMENT_HEIGHT 16

//player directions
#define LEFT 0
#define RIGHT 1

//powerups
#define HEALTHPACK 0
#define FREEZER 1

#define BORALUSK 1

#define BORALUSK_LEVEL 10

//player dimension
int PLAYER_WIDTH = 64;
int PLAYER_HEIGHT = 64;

//enemy dimensions
int ENEMY_WIDTH = 64;
int ENEMY_HEIGHT = 64;

//boralusk dimensions
int BORALUSK_WIDTH = 432;
int BORALUSK_HEIGHT = 192;

int PLAYER_VELOCITY = 325;

//bullet dimensions
int BULLET_WIDTH = 4;
int BULLET_HEIGHT = 4;

//level dimensions
int LEVEL_WIDTH = 800;
int LEVEL_HEIGHT = 500;

int MAX_BULLETS = 10;
int MAX_PARTICLES = 20;
int MAX_STARS = 20;
int MAX_EXPLOSIONS = 5;
int MAX_CLOUDS = 25;
int MAX_POWERUPS = 5;

int MAX_ALIEN_FRAMES = 4;

int STAR_VELOCITY = 500;
int BULLET_VELOCITY = 400;

//The surfaces
SDL_Surface *play_button = NULL;
SDL_Surface *highscores_button = NULL;
SDL_Surface *quit_button = NULL;
SDL_Surface *sound_button = NULL;
SDL_Surface *controls_button = NULL;
SDL_Surface *arrow = NULL;
SDL_Surface *splash_screen = NULL;

SDL_Surface *message = NULL;
SDL_Surface *star = NULL;

SDL_Surface *player = NULL;
SDL_Surface *bullet = NULL;
SDL_Surface *enemy_bullet = NULL;
SDL_Surface *particle = NULL;
SDL_Surface *sprite_sheet = NULL;
SDL_Surface *cloud = NULL;
SDL_Surface *score_msg = NULL;
SDL_Surface *shield = NULL;
SDL_Surface *healthpack = NULL;
SDL_Surface *icicle = NULL;

SDL_Surface *boralusk = NULL;
SDL_Surface *snot_bullet = NULL;

//the font
TTF_Font *font = NULL;

//the color of the font
SDL_Color text_color = { 0, 255, 34 };

//The music that will be played
//Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *pew = NULL;
Mix_Chunk *explode = NULL;
Mix_Chunk *boss_music = NULL;

//game controls
Controls controls;

//game sound
Sound sound;

//The event structure
SDL_Event event;

//the high scores 
Highscores highscores;