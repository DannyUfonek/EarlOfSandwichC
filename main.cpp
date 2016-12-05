/*#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

SDL_Texture* loadTexture( std::string path, SDL_Renderer * render );

// nejake konstanty
static int SCREEN_WIDTH = 1000;
static int SCREEN_HEIGHT = 700;

int main(int argc,char**argv)
{

	SDL_Init(SDL_INIT_VIDEO);

    SDL_Texture * texture;
    SDL_Window * okno;
	okno=SDL_CreateWindow("Earl of Sandwich", //nazev
				SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,  //pozice (posouvatelne okno)
				SCREEN_WIDTH,SCREEN_HEIGHT, //rozmer
				0);	//flags

	SDL_Renderer* render;

	render=SDL_CreateRenderer(okno,-1,SDL_RENDERER_ACCELERATED);

	bool success = true;
    if( render == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Initialize renderer color
        SDL_SetRenderDrawColor( render, 0xFF, 0xFF, 0xFF, 0xFF );

        //Initialize PNG loading
        int imgFlags = IMG_INIT_JPG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
            printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
            success = false;
        }
    }

    texture = loadTexture("windowsontheworld.jpg", render);

    // vyrob plosinku
	SDL_Rect obdelnik;
	obdelnik.x=450;
	obdelnik.y=650;
	obdelnik.w=100;
	obdelnik.h=20;

    int rychlost = 50;

    // prvni vyrenderovani
    SDL_SetRenderDrawColor(render,255,255,255,255);
    SDL_RenderCopy(render, texture, NULL, NULL);
    SDL_RenderPresent(render);
    SDL_RenderClear( render);

    SDL_SetRenderDrawColor(render,0,0,0,255);
    SDL_RenderFillRect(render,&obdelnik);
    SDL_RenderPresent(render);

	int state=1;
	while(state)
	{
		SDL_Event udalost;
		while(SDL_PollEvent(&udalost))
		{
            // chytani klavesnice
			if(udalost.type==SDL_QUIT)
				// ukonci hru
				state=0;

			if(udalost.type==SDL_KEYDOWN)
			{
			    if(udalost.key.keysym.sym==SDLK_ESCAPE)
                {
                    // ukonci hru
                    state=0;
                }
                // ziskej stav klaves na klavesnici
                const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
			    // pohyb plosinky doleva a doprava
                if( currentKeyStates[ SDL_SCANCODE_LEFT ] && obdelnik.x > 0)
				{
                    SDL_SetRenderDrawColor(render,0,255,0,255);
                    obdelnik.x = obdelnik.x -rychlost;
				}
                if( currentKeyStates[ SDL_SCANCODE_RIGHT ] && obdelnik.x < SCREEN_WIDTH - obdelnik.w)
				{
                    SDL_SetRenderDrawColor(render,255,100,200,255);
				    obdelnik.x = obdelnik.x +rychlost;
				}
                // prekresli obdelnik
                SDL_RenderCopy(render, texture, NULL, NULL);
                SDL_RenderFillRect(render,&obdelnik);
                SDL_RenderPresent(render);

                SDL_Delay(20);
			}
		}
	}

    // Ukoncit, uklidit
	//Free loaded image
	SDL_DestroyTexture( texture );
	texture = NULL;

	//Destroy window
	SDL_DestroyRenderer( render );
	SDL_DestroyWindow( okno );
	okno = NULL;
	render = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

return 0;
}

SDL_Texture* loadTexture( std::string path , SDL_Renderer* render )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( render, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}*/

//Using SDL, SDL_image, standard math, and strings
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
//vlastni soubory
#include "sprite.h"
#include "constants.h"

//Screen dimension constants
const int SCREEN_WIDTH = 980;
const int SCREEN_HEIGHT = 720;

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene sprites
SDL_Rect gSpriteClips[ 4 ];
LTexture gSpriteSheetTexture;


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render( int x, int y, SDL_Rect* clip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

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
		gWindow = SDL_CreateWindow( "Earl of Sandwich",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,  // posouvatelne okno
                             SCREEN_WIDTH, SCREEN_HEIGHT, // viz zacatek
                             SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_JPG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError() );
					success = false;
				}

			}
		}
	}

	return success;
}

SDL_Texture* loadTexture( std::string path , SDL_Renderer* render )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( render, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "dots.png" ) )
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
		gSpriteClips[ 2 ].w = 100;
		gSpriteClips[ 2 ].h = 100;

		//Set bottom right sprite
		gSpriteClips[ 3 ].x = 100;
		gSpriteClips[ 3 ].y = 100;
		gSpriteClips[ 3 ].w = 100;
		gSpriteClips[ 3 ].h = 100;
	}

	return success;
}

void close()
{
	//Free loaded images
	gSpriteSheetTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int counter = 0;

int main( int argc, char* args[] )
{
    // vyrob plosinku
	SDL_Rect obdelnik;
	obdelnik.x=450;
	obdelnik.y=650;
	obdelnik.w=100;
	obdelnik.h=20;

    int rychlost = 50;
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
        SDL_Texture * texture;

        texture = loadTexture("windowsontheworld.jpg", gRenderer);
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
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
                        if( currentKeyStates[ SDL_SCANCODE_LEFT ] && obdelnik.x > 0)
                        {
                            SDL_SetRenderDrawColor(gRenderer,0,255,0,255);
                            obdelnik.x = obdelnik.x -rychlost;
                            counter--;
                        }
                        if( currentKeyStates[ SDL_SCANCODE_RIGHT ] && obdelnik.x < SCREEN_WIDTH - obdelnik.w)
                        {
                            SDL_SetRenderDrawColor(gRenderer,255,100,200,255);
                            obdelnik.x = obdelnik.x +rychlost;
                            counter++;
                        }
                    }
				}
				// chytani klavesnice

				//Render top left sprite

                // prekresli obdelnik
                SDL_RenderCopy(gRenderer, texture, NULL, NULL);
                SDL_RenderFillRect(gRenderer,&obdelnik);
                gSpriteSheetTexture.render( 0, 0, &gSpriteClips[ counter ] );
				if (counter > 3)
                {
                    counter = 0;
                }
                if (counter < 0)
                {
                    counter = 3;
                }
				//Update screen
				SDL_RenderPresent( gRenderer );
				SDL_Delay(20);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

