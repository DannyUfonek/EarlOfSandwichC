#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
//vlastni soubory
#include "sprite.h"
#include "group.h"


Sprite::Sprite()
{
	//Initialize
	sTexture = NULL;
	sRect.x = 0;
	sRect.y = 0;
    sRect.w = 0;
	sRect.h = 0;
}

Sprite::~Sprite()
{
	//Deallocate
	free();
}

bool Sprite::loadTexture( SDL_Surface*surface , SDL_Renderer* renderer, int customWidth, int customHeight)
{
    // NALOADUJE TEXTURE Z EXISTUJICIHO SURFACE
    // volitelne argumenty, pokud ma vysledny Rect byt mensi nez nacteny Surface (dobre pro nacitani spritesheetu)
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	if( surface == NULL )
	{
		printf( "Unable to load image! SDL_image Error: %s\n", IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, surface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from image! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
		    if (customWidth > 0 && customHeight > 0)
            {
                //zapis do Rectu vlastni sirku a vysku
                sRect.w = customWidth;
                sRect.h = customHeight;
            }
            else
            {
                //zapis do Rectu velikost obrazku
                sRect.w = surface->w;
                sRect.h = surface->h;
            }

		}

	}

	//Return success
	sTexture = newTexture;
	return sTexture != NULL;
}

void Sprite::free()
{
	//Free texture if it exists
	if( sTexture != NULL )
	{
		SDL_DestroyTexture( sTexture );
		sTexture = NULL;
        sRect.x = 0;
        sRect.y = 0;
        sRect.w = 0;
        sRect.h = 0;
	}
}

void Sprite::render(SDL_Renderer* renderer , SDL_Rect* clip )
{

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		sRect.w = clip->w;
		sRect.h = clip->h;
	}

	//Render sprite to screen
	SDL_RenderCopy( renderer, sTexture, clip, &sRect );
}
/*
void Sprite::add(Group*group)
{
    //pripoj Sprite na konec pole
    group->members[group->numberOfMembers] = this;
    groupIndex = group->numberOfMembers;
    group->numberOfMembers++;
}

void Sprite::leave(Group*group)
{
    //opust Group
    group->members[groupIndex] = NULL;
    // soupni vsechny sprity v poli za timto spritem o jedno misto doleva (aby tam nevzniklo prazdne misto)
    for (int i = groupIndex; i < group->numberOfMembers-1; i++)
    {
        members[i] = members[i+1];
    }
    groupIndex = -1;
    group->numberOfMembers--;
}
*/

// ===============GROUP=============

Group::Group()
{
    members[0] = NULL;
    numberOfMembers = 0;
}

Group::~Group()
{
    freeAll();
}

void Group::freeAll()
{
    //vymaz vsechny textury vsech spritu ve skupine a vymaz i pole (odzadu)
    for (int i = numberOfMembers; i > 0; i--)
    {
        members[i]->free();
        members[i] = NULL;
        numberOfMembers = numberOfMembers -1;
    }
}

void Group::renderAll(SDL_Renderer* renderer)
{
    //vyrenderuj vsechny sprity ve skupine
    for (int i = numberOfMembers; i > 0; i--)
    {
        if (members[i] == NULL)
        {
            continue;
        }
        else
        {
            members[i]->render(renderer);
        }
    }
}

void Group::add(Sprite*sprite)
{
    //pridej sprite na konec
    members[numberOfMembers] = sprite;
    numberOfMembers++;
}

void Group::removeFromGroup(Sprite*sprite)
{
    for (int i = 0; i < numberOfMembers; i++)
    {
        // najdi Sprite v poli (spritu by tam nemelo byt prilis moc, takze to je OK
        if(members[i] == sprite)
        {
            // soupni vsechny sprity v poli za timto spritem o jedno misto doleva, aby tam nevzniklo prazdne misto -
            //tim se zaroven smaze adresa na sprite ktery ma byt odebran
            for (int j = i; j < numberOfMembers-1; j++)
            {
                members[j] = members[j+1];
            }
            members[numberOfMembers] = NULL;
            break;
        }
    }
    numberOfMembers--;
}

