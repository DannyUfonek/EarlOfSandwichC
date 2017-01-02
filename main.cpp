//Using SDL, SDL_image, standard math, and strings
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
//vlastni soubory
#include "sprite.h"
#include "constants.h"


//ztruktury barev
SDL_Color WHITE = {255,255,255,255};
SDL_Color BLACK = {0,0,0,255};
SDL_Color RED = {255,0,0,255};

//The window we'll be rendering to
SDL_Window* okno = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

//Globally used fonts
TTF_Font* titleFont = NULL;
TTF_Font* arcadeFont = NULL;



//================================================================
//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Scene sprites
SDL_Rect gSpriteClips[ 4 ];
Sprite gSpriteSheetTexture;

Sprite plosinka;
Projectile kulicka(10,10,100,100);
// text
Sprite nadpis;
Sprite pressanykey;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		okno = SDL_CreateWindow( "Earl of Sandwich",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,  // posouvatelne okno
                             SCREEN_WIDTH, SCREEN_HEIGHT, // viz zacatek
                             SDL_WINDOW_SHOWN );
		if( okno == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			renderer = SDL_CreateRenderer( okno, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // VSYNC zajisti 60 fps, dale to nemusime resit
			if( renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_JPG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}

			}
		}
	}

	return success;
}

// FUNKCE PRO NACTENI OBRAZKU ZE SOUBORU DO SURFACE
SDL_Surface* loadSurfaceFromFile( std::string path , SDL_Renderer* render )
{

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}

	return loadedSurface;
}
//FUNKCE PRO VYTVORENI SURFACE Z TEXTU
SDL_Surface* renderSurfaceFromText(TTF_Font* font, std::string text, SDL_Color textColor )
{
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( font, text.c_str(), textColor );

    if( textSurface == NULL )
	{
		printf( "Unable to render text %s! SDL_TTF Error: %s\n", text.c_str(), TTF_GetError() );
	}
	//Return success
	return textSurface;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

    // nacti fonty
	titleFont = TTF_OpenFont( "Brushstrike trial.ttf", 90 );
	if( titleFont == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	arcadeFont = TTF_OpenFont( "ka1.ttf", 40 );
	if( arcadeFont == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadTexture(loadSurfaceFromFile("dots.png", renderer), renderer)
        || !kulicka.loadTexture(loadSurfaceFromFile("dots.png", renderer), renderer, 100, 100) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	else
	{
		//Set top left sprite
		gSpriteClips[ 0 ].x =   0;
		gSpriteClips[ 0 ].y =   0;
		gSpriteClips[ 0 ].w = 100;
		gSpriteClips[ 0 ].h = 100;

		//Set top right sprite
		gSpriteClips[ 1 ].x = 100;
		gSpriteClips[ 1 ].y =   0;
		gSpriteClips[ 1 ].w = 100;
		gSpriteClips[ 1 ].h = 100;

		//Set bottom left sprite
		gSpriteClips[ 2 ].x =   0;
		gSpriteClips[ 2 ].y = 100;
		gSpriteClips[ 2 ].w = 50;
		gSpriteClips[ 2 ].h = 50;

		//Set bottom right sprite
		gSpriteClips[ 3 ].x = 100;
		gSpriteClips[ 3 ].y = 100;
		gSpriteClips[ 3 ].w = 100;
		gSpriteClips[ 3 ].h = 100;

	}
	// nacti textove sprity
    if( !nadpis.loadTexture(renderSurfaceFromText(titleFont, "Earl of Sandwich", {255, 0, 0, 255}), renderer)
       ||!pressanykey.loadTexture(renderSurfaceFromText(arcadeFont, "Press any key to play", {255, 0, 0, 255}),renderer))
	{
		printf( "Failed to load text!\n" );
		success = false;
	}
	return success;
}

void close()
{
	//Free loaded images
	gSpriteSheetTexture.free();
	plosinka.free();
	nadpis.free();

	// vymaz fonty
	TTF_CloseFont( titleFont );
	titleFont = NULL;
	TTF_CloseFont( arcadeFont );
	arcadeFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( okno );
	okno = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int counter = 0;

int main( int argc, char* args[] )
{
    //vyrob Rect pro obrazovku
    SDL_Rect obrazovka;
    obrazovka.x = 0;
    obrazovka.y = 0;
    obrazovka.w = SCREEN_WIDTH;
    obrazovka.h = SCREEN_HEIGHT;
    // vyrob plosinku a posun na spravne misto
	Sprite plosinka;
	SDL_Surface * s;
	s = SDL_CreateRGBSurface(0, 100, 20, 32,0,0,0,0);
    if (s == NULL) {
        printf("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
    }

	SDL_FillRect(s, NULL, SDL_MapRGB(s->format,0,0,0));
    plosinka.loadTexture(s, renderer);
	plosinka.sRect.x=450;
	plosinka.sRect.y=650;
    int rychlost = 100;

    // inicializuj kulicku
    //Projectile kulicka(10,10,100,100);


	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
	    // vyrob pozadi
        SDL_Texture * background;
        // muzeme rovnou prevest na texture, neresime totiz, jak velke pozadi bude, takze ztrata informace sirky a vysky nevadi.
        background = SDL_CreateTextureFromSurface(renderer, loadSurfaceFromFile("windowsontheworld.jpg", renderer));
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
            bool start = false;
		    bool quit = false;
		    //Event handler
			SDL_Event e;

			//prvni vyrenderovani


		    //===============================TITLE SCREEN=======================================
            // posun veci na spravne misto
            nadpis.sRect.x = SCREEN_WIDTH/2 - nadpis.sRect.w/2;
            nadpis.sRect.y = 100;
            pressanykey.sRect.x = SCREEN_WIDTH/2 - pressanykey.sRect.w/2;
            pressanykey.sRect.y = 600;

		    while( !start)
            {
                //Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
					    //ukonci hru
					    quit = true;
					    start = true;
					}
                    if(e.type==SDL_KEYDOWN)
                    {
                        if(e.key.keysym.sym==SDLK_ESCAPE)
                        {
                            // ukonci hru
                            quit = true;
                            start = true;
                        }
                        else
                        {
                            start = true;
                        }
                    }
				}
                SDL_SetRenderDrawColor(renderer,0,0,0,255);
                SDL_RenderFillRect(renderer, NULL);

                nadpis.render(renderer);
                if(counter/30 == 1)
                {
                    pressanykey.render(renderer);
                    ++counter;
                    if(counter/30 == 2)
                    {
                    counter = 0;
                    }
                }
                else
                {
                    ++counter;
                }
                SDL_RenderPresent( renderer );

            }
			//===============================MAIN LOOP===================================

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//uzivatel vykrizkuje okno
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
                    if(e.type==SDL_KEYDOWN)
                    {
                        if(e.key.keysym.sym==SDLK_ESCAPE)
                        {
                            // ukonci hru
                            quit = true;
                        }
                        // ziskej stav klaves na klavesnici
                        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
                        // pohyb plosinky doleva a doprava
                        if( currentKeyStates[ SDL_SCANCODE_LEFT ] && plosinka.sRect.x > 0)
                        {
                            SDL_SetRenderDrawColor(renderer,0,255,0,255);
                            plosinka.sRect.x = plosinka.sRect.x -rychlost;
                        }
                        if( currentKeyStates[ SDL_SCANCODE_RIGHT ] && plosinka.sRect.x < SCREEN_WIDTH - plosinka.sRect.w)
                        {
                            SDL_SetRenderDrawColor(renderer,255,100,200,255);
                            plosinka.sRect.x = plosinka.sRect.x +rychlost;
                        }
                    }
				}
				// chytani klavesnice

				//Render top left sprite

                // prekresli kulicku
                if (counter > 3)
                {
                    counter = 0;
                }
                if (counter < 0)
                {
                    counter = 3;
                }

                kulicka.update(&plosinka);
                // vypln pozadi
                SDL_RenderCopy(renderer, background, NULL, NULL);
                plosinka.render(renderer);
                kulicka.render(renderer, &gSpriteClips[ 2 ] );
				//Update screen
				SDL_RenderPresent( renderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

