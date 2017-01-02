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
SDL_Renderer* gameRenderer = NULL;

//Globally used fonts
TTF_Font* titleFont = NULL;
TTF_Font* arcadeFont = NULL;
TTF_Font* monoFont = NULL;
TTF_Font* insulaFont = NULL;
TTF_Font* gothicFont = NULL;


//================================================================
//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

Sprite plosinka;
Projectile kulicka(10,10,100,100);
glyphProjectile a('a', 10,10,100,100);
// text
Sprite nadpis;
Sprite pressanykey;

//LEVELY (POSTUPNE BY MELY BYT NACITANY Z EXTERNIHO SOUBORU)
char*level_one[9] = {"a", "o", "co", "tu", "jen", "jde", "chyt", "tohle", "vsechno"};
int levelLength = 9;

// HERNI DATA
int lives = 3;
int score = 0;


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
		okno = SDL_CreateWindow( "GLYPH PONG v0.01",
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
			//Create gameRenderer for window
			gameRenderer = SDL_CreateRenderer( okno, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // VSYNC zajisti 60 fps, dale to nemusime resit
			if( gameRenderer == NULL )
			{
				printf( "gameRenderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
			    printf("created renderer \n");
				//Initialize renderer color
				SDL_SetRenderDrawColor( gameRenderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a );

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
SDL_Surface* loadSurfaceFromFile( std::string path )
{

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s \n", path.c_str(), IMG_GetError() );
	}
	else
    {
        printf("successfully loaded image from %s into surface\n", path.c_str());
    }

	return loadedSurface;
}
//FUNKCE PRO VYTVORENI SURFACE Z TEXTU
SDL_Surface* renderSurfaceFromText(TTF_Font* font, std::string text, SDL_Color textColor )
{
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended( font, text.c_str(), textColor );

    if( textSurface == NULL )
	{
		printf( "Unable to render text %s! SDL_TTF Error: %s\n", text.c_str(), TTF_GetError() );
	}
	else
    {
        printf("successfully rendered text '%s' into surface\n", text.c_str());
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
	monoFont = TTF_OpenFont("courbd.ttf", 80);
    if( monoFont == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	insulaFont = TTF_OpenFont("INSULA_.ttf", 80);
    if( insulaFont == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	gothicFont = TTF_OpenFont("ArgBrujS.ttf", 80);
	if( gothicFont == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
    /*
	//Load sprite sheet texture
	SDL_Surface*sI = loadSurfaceFromFile("dotsSmall.png");
	// najdi pruhledne pixely
	SDL_SetColorKey(sI,SDL_TRUE, SDL_MapRGB(sI->format, 0,255,255));
	SDL_SetSurfaceBlendMode(sI,SDL_BLENDMODE_NONE);

    //cervena
    gSpriteClips[ 0 ].x =   0;
    gSpriteClips[ 0 ].y =   0;
    gSpriteClips[ 0 ].w = 50;
    gSpriteClips[ 0 ].h = 50;

    //zluta
    gSpriteClips[ 1 ].x = 50;
    gSpriteClips[ 1 ].y =   0;
    gSpriteClips[ 1 ].w = 50;
    gSpriteClips[ 1 ].h = 50;

    //zelena
    gSpriteClips[ 2 ].x =  0;
    gSpriteClips[ 2 ].y = 50;
    gSpriteClips[ 2 ].w = 50;
    gSpriteClips[ 2 ].h = 50;

    //modra
    gSpriteClips[ 3 ].x = 50;
    gSpriteClips[ 3 ].y = 50;
    gSpriteClips[ 3 ].w = 50;
    gSpriteClips[ 3 ].h = 50;

	if( !gSpriteSheetTexture.loadTexture(SDL_CreateTextureFromSurface(gameRenderer,sI), sI->w, sI->h)
        || !kulicka.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, sI), 50, 50))
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	else
	{


	}
	*/
	// vyrob textove sprity
	SDL_Surface*s1 = renderSurfaceFromText(insulaFont, "GLYPH PONG", {255, 255, 255, 255});
	SDL_Surface*s2 = renderSurfaceFromText(insulaFont, "Press any key to play", {255, 255, 255, 255});
    SDL_Surface*s3 = renderSurfaceFromText(insulaFont, &a.glyph, {0, 0, 0, 255});

    if( !nadpis.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s1), s1->w, s1->h)
       ||!pressanykey.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s2), s2->w, s2->h)
       ||!a.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s3), s3->w, s3->h))
	{
		printf( "Failed to load text!\n" );
		success = false;
	}


	return success;
}

void close()
{
	//Free loaded images
	//gSpriteSheetTexture.free();
	plosinka.free();
	nadpis.free();
	//kulicka.free();
	a.free();

	// vymaz fonty
	TTF_CloseFont( titleFont );
	titleFont = NULL;
	TTF_CloseFont( arcadeFont );
	arcadeFont = NULL;
    TTF_CloseFont( monoFont );
	monoFont = NULL;
	TTF_CloseFont( insulaFont );
	insulaFont = NULL;
	TTF_CloseFont( gothicFont );
	gothicFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( gameRenderer );
	SDL_DestroyWindow( okno );
	okno = NULL;
	gameRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int counter = 0;

int main( int argc, char* args[] )
{
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
        background = SDL_CreateTextureFromSurface(gameRenderer, loadSurfaceFromFile("old-paper-parchment-plain-med.jpg"));
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
            //vyrob Rect pro obrazovku
            SDL_Rect obrazovka;
            obrazovka.x = 0;
            obrazovka.y = 0;
            obrazovka.w = SCREEN_WIDTH;
            obrazovka.h = SCREEN_HEIGHT;
            // vyrob plosinku a posun na spravne misto
            //Sprite plosinka;
            SDL_Surface * s;
            s = SDL_CreateRGBSurface(0, 100, 20, 32,0,0,0,0);
            if (s == NULL) {
                printf("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
            }

            SDL_FillRect(s, NULL, SDL_MapRGB(s->format,0,0,0));
            plosinka.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s), s->w, s->h);
            plosinka.sRect.x=450;
            plosinka.sRect.y=850;
            int rychlost = 100;

            // inicializuj kulicku
            //Projectile kulicka(10,10,100,100);


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
                SDL_SetRenderDrawColor(gameRenderer,0,0,0,255);
                SDL_RenderFillRect(gameRenderer, NULL);

                nadpis.render(gameRenderer);
                if(counter/30 == 1)
                {
                    pressanykey.render(gameRenderer);
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
                SDL_RenderPresent( gameRenderer );

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
                            SDL_SetRenderDrawColor(gameRenderer,0,255,0,255);
                            plosinka.sRect.x = plosinka.sRect.x -rychlost;
                            printf("x pozice = %i\n", plosinka.sRect.x);
                        }
                        if( currentKeyStates[ SDL_SCANCODE_RIGHT ] && plosinka.sRect.x < SCREEN_WIDTH - plosinka.sRect.w)
                        {
                            SDL_SetRenderDrawColor(gameRenderer,255,100,200,255);
                            plosinka.sRect.x = plosinka.sRect.x +rychlost;
                            printf("x pozice = %i\n", plosinka.sRect.x);
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

                //kulicka.update(&plosinka);
                a.update(&plosinka);
                // vypln pozadi
                SDL_RenderCopy(gameRenderer, background, NULL, NULL);
                SDL_RenderFillRect(gameRenderer,&plosinka.sRect);
                plosinka.render(gameRenderer);
                //kulicka.render(gameRenderer, &gSpriteClips[ 2 ] );
                a.render(gameRenderer);
                //SDL_RenderFillRect(gameRenderer,&kulicka.sRect);
				//Update screen
				SDL_RenderPresent( gameRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

