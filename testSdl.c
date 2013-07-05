#include "SDL/SDL.h"
#include <stdio.h>

static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;
static const int SCREEN_BPP = 16;

static SDL_Surface *screen = NULL;
static SDL_Event   event;

void draw_line( int x, int y, int width, int r, int g, int b, SDL_Surface *destination )
{
	int i;
	Uint16 *bufp;
	Uint16 color = (r >> 3) << 11 | (g >>2) << 6 | b >> 3;

	bufp = (Uint16 *)screen->pixels + y * screen->pitch/2 + x;

	for( i = 0; i < width; i++ )
		*bufp++ = color; 
}

void draw_rect( SDL_Rect *rect, int r, int g, int b, SDL_Surface *destination )
{
	int j;
	for( j = rect->y; j < rect->y + rect->h ; j++ ){
		draw_line( rect->x, j, rect->w, r, g, b, destination );
	}
}

int test_sdl( void ) {

    int quit = 0;

printf("TEST SDL !\n");
    //initialisation de tout les sous-systemes de sdl
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
        return 1;
    }

    //on met en place l'ecran
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //Si il y a une erreur lors de la mise en place de l'ecran
    if( screen == NULL ) {
        return 1;
    }

    //on met en place la barre caption de la fenetre
    SDL_WM_SetCaption( "Hello World", NULL );

//     //Chargement des images
//     message = load_image( "hello_world.bmp" );
//     background = load_image( "background.bmp" );

//     //Application des surfaces sur l'ecran
//     apply_surface( 0, 0, background, screen );
// 
//     //Application du message sur l'ecran
//     apply_surface( 180, 140, message, screen );

    SDL_Rect rect;
    rect.x = 50;
    rect.y = 50;
    rect.w = 50;
    rect.h = 50;
    draw_rect( &rect, 255, 0, 0, screen );

    //mise �jour de l'ecran
    if( SDL_Flip( screen ) == -1 ) {
        return 1;
    }

    //Tant que l'utilisateur n'a pas quitter
    while( quit == 0 ) {
        //tant qu'il y a un evenement dans le handler
        while( SDL_PollEvent( &event ) ) {
            //Si l'utilisateur ferme la fenetre avec le X
            if( event.type == SDL_QUIT ) {
                //On quitte le programme
                quit = 1;
            }
        }
    }

    //On quitte sdl
    SDL_Quit();
	
    return 0;
}

