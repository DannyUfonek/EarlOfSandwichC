//Using SDL, SDL_image, standard math, and strings
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib> /* srand, rand */
#include <ctime>
//vlastni soubory
#include "sprite.h"
#include "constants.h"


//struktury barev
SDL_Color WHITE = {255,255,255,255};
SDL_Color BLACK = {0,0,0,255};
SDL_Color RED = {255,0,0,255};

//The window we'll be rendering to
SDL_Window* okno = NULL;

//The window renderer
SDL_Renderer* gameRenderer = NULL;

//nejake fonty
TTF_Font* titleFont = NULL;
TTF_Font* arcadeFont = NULL;
TTF_Font* monoFont = NULL;
TTF_Font* insulaFont = NULL;
TTF_Font* insulaFontSmall = NULL;


//================================================================
//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

Sprite plosinka;

//staticky text (title)
Sprite nadpis;
Sprite navod1;
Sprite navod2;
Sprite pressanykey;
Group TitleSprites;

//staticky text (hra)
Sprite scoreText;
Sprite livesText;

//staticky text (prohra/vyhra)
Sprite youLose;
Sprite youWin;
Sprite pressEnterToRestart;

//================LEVELY A HERNI LOGIKA (V POZDEJSI VERZI BY LEVELY MELY BYT NACITANY Z EXTERNIHO SOUBORU)=================
std::string level_one[9] = {"a", "o", "co", "tu", "jen", "jde", "chyt", "tohle", "vsechno"};
int levelLength = 9;
// omezeni (pozdeji by to melo byt dynamicke)
const int maxWordLength = 10;
// samotne pole na pismenka jednoho slova
glyphProjectile*letters[maxWordLength];

int currentWordLength = 0;
int indexOfCurrentWord = 0;
//std::string currentWord;
//bool pro skok na dalsi slovo
bool wordComplete = false;
// slovo na horejsku obrazovky
Sprite wholeWord;

// HERNI DATA
int lives = 3;
int score = 0;
//dynamicky text
Sprite scoreNumber;
Sprite livesNumber;
// textura, ktera se bude predelavat
SDL_Texture*livesTexture;
SDL_Texture*scoreTexture;


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
	insulaFontSmall = TTF_OpenFont("INSULA_.ttf", 40);
	if( insulaFontSmall == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	// vyrob vsechny textove sprity
	SDL_Surface*s1 = renderSurfaceFromText(insulaFont, "GLYPH PONG v0.01", {255, 255, 255, 255});
	SDL_Surface*s2 = renderSurfaceFromText(monoFont, "Press any key to play", {255, 255, 255, 255});
    SDL_Surface*s3 = renderSurfaceFromText(insulaFontSmall, "Compose words from letters on the screen\n move using arrow keys", {255, 255, 255, 255});
    SDL_Surface*s4 = renderSurfaceFromText(insulaFontSmall, "use space to complete a word once all letters\n have been deflected at least once", {255, 255, 255, 255});

    SDL_Surface*s5 = renderSurfaceFromText(insulaFont, "You won", {255, 255, 255, 255});
    SDL_Surface*s6 = renderSurfaceFromText(insulaFont, "You lose", {255, 255, 255, 255});
    SDL_Surface*s7 = renderSurfaceFromText(insulaFont, "Lives: ", {255, 255, 255, 255});
    SDL_Surface*s8 = renderSurfaceFromText(insulaFont, "Score: ", {255, 255, 255, 255});
    SDL_Surface*s9 = renderSurfaceFromText(monoFont, "PRESS ENTER TO PLAY AGAIN", {255, 255, 255, 255});

    if( !nadpis.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s1), s1->w, s1->h)
       ||!pressanykey.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s2), s2->w, s2->h)
       ||!navod1.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s3), s3->w, s3->h)
       ||!navod2.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s4), s4->w, s4->h)
       ||!youWin.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s5), s5->w, s5->h)
       ||!youLose.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s6), s6->w, s6->h)
       ||!livesText.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s7), s7->w, s7->h)
       ||!scoreText.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s8), s8->w, s8->h)
       ||!pressEnterToRestart.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, s9), s9->w, s9->h))
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
	// vymaz fonty
	TTF_CloseFont( titleFont );
	titleFont = NULL;
	TTF_CloseFont( arcadeFont );
	arcadeFont = NULL;
    TTF_CloseFont( monoFont );
	monoFont = NULL;
	TTF_CloseFont( insulaFont );
	insulaFont = NULL;
	TTF_CloseFont( insulaFontSmall );
	insulaFontSmall = NULL;

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
    /* initialize random seed: */
    srand(time(NULL));
    // inicializuj pole pro pismena
    letters[0] = NULL;
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
		    bool win = false;
		    //Event handler
			SDL_Event e;

			//prvni vyrenderovani
            //============vyrob plosinku a posun na spravne misto
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
            nadpis.sRect.y = 80;

            navod1.sRect.x = SCREEN_WIDTH/2 - navod1.sRect.w/2;
            navod1.sRect.y = nadpis.sRect.y + nadpis.sRect.h + 80;
            navod2.sRect.x = SCREEN_WIDTH/2 - navod2.sRect.w/2;
            navod2.sRect.y = navod1.sRect.y + navod1.sRect.h + 80;

            pressanykey.sRect.x = SCREEN_WIDTH/2 - pressanykey.sRect.w/2;
            pressanykey.sRect.y = 800;

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

			//While application is running
			while( !quit )
			{
			    // ====================GENEROVANI LEVELU==================================
			    // vygeneruj dalsi slovo z levelu, pokud muzu
			    if (indexOfCurrentWord < levelLength)
			    {
			        printf("currentWordLength = %i\n", currentWordLength);
                    //nacti slovo (preved na char*, aby se to dalo hezky pismeno po pismenu vyrabet)
                    char*currentWord = &level_one[indexOfCurrentWord][0];
                    printf("loaded word %s\n", currentWord);
                    //vycisti pismena dohraneho slova

                        for (int j = 0; j < currentWordLength; j++)
                        {
                            printf("freing\n");
                            //printf("freeing previous letter %s\n", letters[j]->glyph);
                            //letters[j]->free();
                            // predchozi dva radky nefunguji, proto se s tim neotravuj
                            delete(letters[j]);
                        }
                        wholeWord.free();


                    currentWordLength = 0;
                    int I = 0;
                    bool wordEnd = false;
                    printf("space is free, going to generate a word \n");
                    //vygeneruj nova pismena slova
                    while (!wordEnd)
                    {
                        // projdi slovo pismeno po pismenu
                        printf("currently on letter %s \n", currentWord);

                        if (*currentWord!= '\0')
                        {
                            printf("generating surface for %s", currentWord);
                            SDL_Surface*letterSurface;
                            // vyrob pismeno nekde nahore (-100 tam je aby se pismeno nezaseklo na pravem okraji v perpetualni kolizi)
                            glyphProjectile*newLetter = new glyphProjectile(*currentWord, 7,7, rand()%(SCREEN_WIDTH-100), 100);
                            // vyrenderuj pismeno
                            char*text;
                            strncpy ( text, currentWord, 1 );
                            letterSurface = renderSurfaceFromText(insulaFont, text, {0, 0, 0, 255});
                            //nacti ho do glyphprojectile
                            newLetter->loadTexture(SDL_CreateTextureFromSurface(gameRenderer, letterSurface), letterSurface->w, letterSurface->h);
                            //uloz do letters
                            letters[I] = newLetter;
                            printf("generated and saved letter at x = %i and at index %i within letters array \n", newLetter->sRect.x, I);
                            I = I+1;
                            //posun pointer na dalsi pismeno
                            currentWord = currentWord + sizeof(char);
                        }
                        else
                        {
                            printf("reached end of word \n");
                            wordEnd = true;
                        }

                    }

                    //nakonec nacti cele slovo
                    // vrat se na zacatek slova
                    currentWord = &level_one[indexOfCurrentWord][0];
                    SDL_Surface*wordSurface;
                    wordSurface = renderSurfaceFromText(insulaFont, currentWord, {0, 0, 0, 255});
                    wholeWord.loadTexture(SDL_CreateTextureFromSurface(gameRenderer, wordSurface), wordSurface->w, wordSurface->h);
                    // posun ho doprostred nahoru
                    wholeWord.sRect.x = SCREEN_WIDTH/2 - wholeWord.sRect.w/2;
                    wholeWord.sRect.y = 80;

                    // slovo vyrobeno, posun pocitadlo a zas rozjed hru
                    indexOfCurrentWord++;
                    currentWordLength = I;
                    printf("currentWordLength = %i\n", currentWordLength);
                    wordComplete = false;
			    }
			    else
                {
                    //pokud nemuzu sehnat dalsi slovo, znamena to vyhru
                    win = true;
                }

			    while (!quit && !wordComplete && lives != 0 && !win)
                {
                    // hraj soucasne slovo

                    //====================UDALOSTI=================
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
                            if(e.key.keysym.sym==SDLK_SPACE)
                            {
                                // projed vsechny pismena, jestli se uz srazily
                                for(int i = 0; i<currentWordLength; i++)
                                {
                                    if (letters[i]->hasCollided == false)
                                    {
                                        printf("not all letters have collided yet\n");
                                        // pokud se vsechny jeste aspon jednou neodrazily,
                                        // nedelej nic
                                    }
                                    else
                                    {
                                        printf("word complete\n");
                                        score = score+currentWordLength;
                                        wordComplete = true;
                                    }
                                }
                            }
                        }

                    }

                    //====================HRA=================
                    // pohni vsemi pismenky
                    for (int i = 0; i < currentWordLength; i++)
                    {
                        glyphProjectile*letter = letters[i];
                        projectileState state = letter->update(&plosinka);
                        if (state == LOST)
                        {
                            lives--;
                            printf("GLYPH LOST! REMAINING LIFE: %i", lives);
                            letter->sRect.y = 70;
                        }
                        if (state == COLLIDING)
                        {
                            letter->hasCollided = true;
                        }
                    }

                    // ===============VYKRESLENI==========

                    // vypln pozadi
                    SDL_RenderCopy(gameRenderer, background, NULL, NULL);
                    // vykresli vse
                    plosinka.render(gameRenderer);
                    wholeWord.render(gameRenderer);
                    for (int i = 0; i < currentWordLength; i++)
                    {
                        letters[i]->render(gameRenderer);
                    }
                    //kulicka.render(gameRenderer, &gSpriteClips[ 2 ] );
                    //SDL_RenderFillRect(gameRenderer,&kulicka.sRect);
                    //Update screen
                    SDL_RenderPresent( gameRenderer );
                }

                //posun skore na spravne misto
                if (win == true || lives == 0)
                {
                    youLose.sRect.x = SCREEN_WIDTH/2 - youLose.sRect.w/2;
                    youLose.sRect.y = 100;
                    youWin.sRect.x = SCREEN_WIDTH/2 - youWin.sRect.w/2;
                    youWin.sRect.y = 100;
                    scoreText.sRect.x = SCREEN_WIDTH/2 - scoreText.sRect.w/2 - scoreNumber.sRect.w;
                    scoreText.sRect.y = SCREEN_HEIGHT/2;
                    scoreNumber.sRect.x = SCREEN_WIDTH/2;
                    scoreNumber.sRect.y = SCREEN_HEIGHT/2;
                    pressEnterToRestart.sRect.x = SCREEN_WIDTH/2 - pressEnterToRestart.sRect.w/2;
                    pressEnterToRestart.sRect.y = 800;
                }


                while(win && !quit)
                {
                    // LOOP PRO VYHRU
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
                            if(e.key.keysym.sym ==SDLK_RETURN)
                            {
                                // restartuje hru
                                lives = 3;
                                score = 0;
                                start = true;
                                win = false;
                            }
                        }

                    }
                    // vypln pozadi
                    SDL_SetRenderDrawColor(gameRenderer,0,0,0,255);
                    SDL_RenderFillRect(gameRenderer, NULL);
                    // vykresli vse
                    youWin.render(gameRenderer);
                    scoreText.render(gameRenderer);
                    scoreNumber.render(gameRenderer);
                    // blikani
                    if(counter/30 == 1)
                    {
                        pressEnterToRestart.render(gameRenderer);
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
                while(lives == 0 && !quit)
                {
                    // LOOP PRO PROHRU
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
                            if(e.key.keysym.sym ==SDLK_RETURN)
                            {
                                // restartuje hru
                                lives = 3;
                                score = 0;
                                start = true;
                                win = false;
                            }
                        }

                    }
                    // vypln pozadi
                    SDL_SetRenderDrawColor(gameRenderer,0,0,0,255);
                    SDL_RenderFillRect(gameRenderer, NULL);
                    // vykresli vse
                    youLose.render(gameRenderer);
                    scoreText.render(gameRenderer);
                    scoreNumber.render(gameRenderer);
                    // blikani
                    if(counter/30 == 1)
                    {
                        pressEnterToRestart.render(gameRenderer);
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
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

