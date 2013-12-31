//The headers
#include <stdio.h>
#include <time.h>
#include "bool.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "prototypes.h"

#include "main_menu/graphics.h"
#include "main_menu/sound.h"
#include "main_menu/controls.h"

#include "highscores.h"

#include "gameglobals.h"
#include "stringinput.h"
#include "button.h"
#include "timer.h"
#include "box.h"
#include "cloud.h"
#include "explosion.h"
#include "star.h"
#include "particle.h"
#include "powerup.h"
#include "bullet.h"
#include "player.h"
#include "boralusk.h"
#include "enemy.h"
#include "alien_fighter.h"
#include "collision.h"
#include "game.h"
#include "splash_screen.h"
#include "main_menu/highscores_menu.h"
#include "main_menu/mainmenu.h"


SDL_Surface *load_image( char* filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0, 0 ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    graphics_set_video_mode( &graphics, 800, 500, 32 );

    //If there was an error in setting up the screen
    if( graphics.screen == NULL )
    {
        return false;
    }

    //initialize sdl_ttf
    if ( TTF_Init() == -1 )
    {
        return 1;
    }

    //initialize sdl_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return 1;
    }

    //load controls
    controls_load_keys( &controls );

    //setup sound
    sound_update( &sound, 128, 128, true );

    //Set the window caption
    SDL_WM_SetCaption( "Space Fighter", NULL );

    //load high scores
    highscores_read_from_file( &highscores );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load images
    play_button = load_image( "Space Fighter.app/Contents/Resources/play.png" );
    quit_button = load_image( "Space Fighter.app/Contents/Resources/quit.png" );
    highscores_button = load_image( "Space Fighter.app/Contents/Resources/highscores.png" );
    sound_button = load_image( "Space Fighter.app/Contents/Resources/sound.png" );
    controls_button = load_image( "Space Fighter.app/Contents/Resources/controls.png" );
    arrow = load_image( "Space Fighter.app/Contents/Resources/arrow.png" );
    splash_screen = load_image( "Space Fighter.app/Contents/Resources/splash_screen.png" );

    player = load_image( "Space Fighter.app/Contents/Resources/Fighter Spacecraft.gif" );
    particle = load_image( "Space Fighter.app/Contents/Resources/red_particle.bmp" );
    star = load_image( "Space Fighter.app/Contents/Resources/star.png" );
    bullet = load_image( "Space Fighter.app/Contents/Resources/bullet.png" );
    enemy_bullet = load_image( "Space Fighter.app/Contents/Resources/enemy_bullet.png" );
    sprite_sheet = load_image( "Space Fighter.app/Contents/Resources/spritesheet.png" );
    alien_fighter = load_image( "Space Fighter.app/Contents/Resources/space_fighter_aeon.png");
    cloud = load_image( "Space Fighter.app/Contents/Resources/dust_cloud.png" );
    shield = load_image( "Space Fighter.app/Contents/Resources/shield.png" );
    healthpack = load_image( "Space Fighter.app/Contents/Resources/healthpack.png" );
    icicle = load_image( "Space Fighter.app/Contents/Resources/icicle.png" );

    boralusk = load_image( "Space Fighter.app/Contents/Resources/boralusk.png" );
    snot_bullet = load_image( "Space Fighter.app/Contents/Resources/snot_bullet.png" );

    font = TTF_OpenFont( "Space Fighter.app/Contents/Resources/AccidentalPresidency.ttf", 24 );

    if( boralusk == NULL || snot_bullet == NULL )
    {
        printf( "error loading boralusk\n" );
        return false;
    }    

    if( splash_screen == NULL )
    {
        printf( "error loading splash_screen\n" );
        return false;
    }

    if( healthpack == NULL || icicle == NULL )
    {
        printf( "error loading powerups\n" );
        return false;
    }    

    if( ( play_button == NULL ) || ( quit_button == NULL ) || ( highscores_button == NULL ) || ( sound_button == NULL ) || ( controls_button == NULL ) || ( arrow == NULL ) )
    {
        printf( "error loading menu items\n" );
        return false;
    }

    //If there was a problem in loading the images
    if( ( player == NULL ) || ( particle == NULL ) || ( star == NULL ) || ( bullet == NULL ) || ( enemy_bullet == NULL ) || ( sprite_sheet == NULL ) || ( alien_fighter == NULL ) || ( cloud == NULL ) || ( shield == NULL ))
    {
        printf( "error loading player\n" );
        return false;
    }

    //if there was an error in loading the font
    if( font == NULL )
    {
        printf( "error loading font\n" );
        return false;
    }

    //load the music
    // music = Mix_LoadMUS( "Space Fighter.app/Contents/Resources/snake_music.wav" );

    //if there was a problem loading the music
    // if( music == NULL )
    // {
    //     return 1;
    // }

    //Load the sound effects
    pew = Mix_LoadWAV( "Space Fighter.app/Contents/Resources/pew.wav" );
    explode = Mix_LoadWAV( "Space Fighter.app/Contents/Resources/explosion.wav" );
    boss_music = Mix_LoadWAV( "Space Fighter.app/Contents/Resources/boss_music.wav" );

    //If there was a problem loading the sound effects
    if( ( pew == NULL ) || ( explode == NULL ) || ( boss_music == NULL ) )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //save high scores
    highscores_save_to_file( &highscores );

    //Free the surfaces
    SDL_FreeSurface( play_button );
    SDL_FreeSurface( highscores_button );
    SDL_FreeSurface( quit_button );
    SDL_FreeSurface( sound_button );
    SDL_FreeSurface( controls_button );
    SDL_FreeSurface( arrow );
    SDL_FreeSurface( splash_screen );

    SDL_FreeSurface( message );

    SDL_FreeSurface( player );
    SDL_FreeSurface( score_msg );
    SDL_FreeSurface( star );
    SDL_FreeSurface( bullet );
    SDL_FreeSurface( particle );
    SDL_FreeSurface( sprite_sheet );
    SDL_FreeSurface( alien_fighter );
    SDL_FreeSurface( cloud );
    SDL_FreeSurface( shield );

    SDL_FreeSurface( healthpack );
    SDL_FreeSurface( icicle );

    SDL_FreeSurface( boralusk );
    SDL_FreeSurface( snot_bullet );

    //Free the sound effects
    Mix_FreeChunk( pew );
    Mix_FreeChunk( explode );
    Mix_FreeChunk( boss_music );

    //Free the music
    //Mix_FreeMusic( music );

    //close the font
    TTF_CloseFont( font );

    //Quit SDL_mixer
    Mix_CloseAudio();

    //quit sdl_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    //seed random with time
    srand(time(0));

    bool quit = false;
    int gameOver = -1;
    
    
    //Initialize
    if( !init() )
    {
        printf( "init failed\n" );
        return 1;
    }

    //Load the files
    if( !load_files() )
    {
        printf( "load failed\n" );
        return 1;
    }

    
    int status = 0;

    status = splash_screen_play();

    if( status == -1 )
    {
        quit = true;
    }
    

    //Fill screen
    SDL_FillRect( graphics.screen, &graphics.screen->clip_rect, SDL_MapRGB( graphics.screen->format, 0x80, 0x80, 0x80 ) );

    //While the user hasn't quit
    while( quit == false )
    {
        Mainmenu mm;
        mainmenu_init( &mm );
        status = mainmenu_play( &mm );
        if( status == -1 )
        {
            break;
        }

        //While there's event to handle
        while( SDL_PollEvent( &event ) )
        {

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }


        //Update the screen
        if( SDL_Flip( graphics.screen ) == -1 )
        {
            return 1;
        }
    }

    //Clean up
    clean_up();

    return 0;   //no errors
}
