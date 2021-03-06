#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
//vlastni soubory
#include "sprite.h"
#include "constants.h"


Sprite::Sprite()
{
	//Initialize
	sTexture = NULL;
	sRect.x = 0;
	sRect.y = 0;
    sRect.w = 0;
	sRect.h = 0;
	hidden = false;
}

Sprite::~Sprite()
{
	//Deallocate
	free();
}

bool Sprite::loadTexture( SDL_Texture*newTexture, int width, int height)
{
    // NALOADUJE TEXTURE Z EXISTUJICIHO SURFACE
    // volitelne argumenty, pokud ma vysledny Rect byt mensi nez nacteny Surface (dobre pro nacitani spritesheetu)
	//Get rid of preexisting texture
	free();

	sTexture = newTexture;
	if( newTexture == NULL )
	{
		printf( "Unable to load texture! SDL_image Error: %s\n", IMG_GetError() );
	}
	else
	{
        printf("texture loaded\n");
        if (width > 0 && height > 0)
        {
            //zapis do Rectu vlastni sirku a vysku
            sRect.w = width;
            sRect.h = height;
        }
        else
        {
            printf("ERROR: negative width \& height \n");
        }

	}

    //Return success
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
	if(!hidden)
    {
        SDL_RenderCopy( renderer, sTexture, clip, &sRect );
    }
}

/*
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
            //tim se zaroven smaze ukazatel na sprite ktery ma byt odebran
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

void Group::updateAll()
{
    for(int i = numberOfMembers; i > 0; i--)
    {
        members[i].update();
    }
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
    for (int i = numberOfMembers-1; i < 0; i--)
    {
        if (members[i] == NULL)
        {
            //muze se stat
            continue;
        }
        else
        {
            members[i]->render(renderer);
        }
    }
}

*/
Projectile::Projectile(int velx, int vely, int posx, int posy):Sprite()
{
	//Initialize
    velX = velx;
    velY = vely;
    sRect.x = posx;
    sRect.y = posy;

}

Projectile::~Projectile()
{
	//Deallocate
	free();
}

projectileState Projectile::update(Sprite * plosinka)
{
    // ======= POHYB =========
    if (plosinka !=NULL)
    {
        // pokud vztahujeme vuci plosince

        // POKUD HIDDEN, NEDELEJ NIC
        if (!hidden)
        {
            //POHYB V X
            if(0 < sRect.x && sRect.x < SCREEN_WIDTH -sRect.w)
            {
                sRect.x = sRect.x + velX;
            }
            else
            {
                //odraz se od strany
                velX = -velX;
                sRect.x = sRect.x + velX;
            }

            // POHYB V Y
            if(0 < sRect.y)
            {
                sRect.y = sRect.y + velY;
            }
            else
            {
                //odraz se od horni steny
                velY = -velY;
                sRect.y = sRect.y + velY;
            }

            if (sRect.y + sRect.h >= plosinka->sRect.y && SDL_HasIntersection(&sRect, &plosinka->sRect) == SDL_TRUE)
            {
                // proved kolizi s plosinkou
                velY = -velY;
                sRect.y = plosinka->sRect.y-sRect.h + velY;
                hasCollided = true;
                return COLLIDING;
            }

            if (sRect.y > SCREEN_HEIGHT)
            {
                // uber zivot pokud vyjede z planu (vyhodnocene v mainu)
                return LOST;
            }
            // pokud nic, vrat "FREE"
            return FREE;
        }
        else
        {
            return HIDDEN;
        }

    }


}

glyphProjectile::glyphProjectile(char character, int velx, int vely, int posx, int posy):Projectile(velx, vely, posx, posy)
{
    //initialize
    glyph = character;
    hasCollided = false;
}

glyphProjectile::~glyphProjectile()
{
    glyph = NULL;
    hasCollided = false;
    free();
}

