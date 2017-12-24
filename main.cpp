#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>
#define LARGEUR_ECRAN 1350
#define HAUTEUR_ECRAN 720
int tracerDroite(SDL_Surface *ecran, SDL_Rect point1, SDL_Rect point2, unsigned char red, unsigned char green, unsigned char blue);
void fairePh(SDL_Surface *ecran, int nombreTraits);

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
        exit(EXIT_FAILURE);
    }

    SDL_Surface *ecran = SDL_SetVideoMode(LARGEUR_ECRAN, HAUTEUR_ECRAN, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    SDL_ShowCursor(SDL_DISABLE);
    int nombreTraits = 3, nombreTraitsPrecedent = 3;
    fairePh(ecran, nombreTraits);


    SDL_Event event;
    int continuer = 1;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            case SDLK_a:
                continuer = 0;
                break;
            case SDLK_UP:
                nombreTraits += 3;
                break;
            case SDLK_DOWN:
                nombreTraits -= 3;
                if (nombreTraits < 3)
                {
                    nombreTraits = 3;
                }
            case SDLK_r:
                nombreTraits = 5;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        if (nombreTraits != nombreTraitsPrecedent)
        {
            SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
            SDL_Flip(ecran);
            fairePh(ecran, nombreTraits);
            nombreTraitsPrecedent = nombreTraits;
        }
    }
    SDL_ShowCursor(SDL_ENABLE);
    SDL_FreeSurface(ecran);
    SDL_Quit();
    return EXIT_SUCCESS;
}

void fairePh(SDL_Surface *ecran, int nombreTraits)
{
    unsigned char red = 0, green = 190, blue = 160;

    SDL_Rect point1, point2;

    int i = 0;
    for (i = 0 ; i< (nombreTraits*4) ; i++)
    {
            point1.x = (-abs(i - nombreTraits*2)+nombreTraits*2)*(LARGEUR_ECRAN/(nombreTraits*2));
            point1.y = HAUTEUR_ECRAN/2;

            point2.x = LARGEUR_ECRAN/2;
            point2.y = (HAUTEUR_ECRAN/(nombreTraits*2))*(abs(abs(i-3*nombreTraits)-2*nombreTraits));

            tracerDroite(ecran, point1, point2, red, green, blue);
    }

}

int tracerDroite(SDL_Surface *ecran, SDL_Rect point1, SDL_Rect point2, unsigned char red, unsigned char green, unsigned char blue)
{
    int i = 0;
    if (point1.x > point2.x)
    {
        i = point1.x;
        point1.x = point2.x;
        point2.x = i;
        i = point1.y;
        point1.y = point2.y;
        point2.y = i;
    }

    SDL_Surface *pixel = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 32, 0, 0, 0, 0);
    SDL_FillRect(pixel, NULL, SDL_MapRGB(ecran->format, red, green, blue));

    double x1 = point1.x, x2 = point2.x, y1 = point1.y, y2 = point2.y;
    double coefficient = 0;
    if (x1 == x2 && y1 >= y2)
    {
        coefficient = -2000;
    }
    else if (x1 == x2 && y1 < y2)
    {
        coefficient = 2000;
    }
    else
    {
        coefficient = (y2 - y1)/(x2 - x1);
    }
    double ordonneeALOrigine = (point1.y) - coefficient*(point1.x);
    SDL_Rect positionPixel;
    positionPixel.x = point1.x;
    positionPixel.y = point1.y;

    if (coefficient >= 1.0)
    {
        for (positionPixel.y = point1.y; positionPixel.y <= point2.y; positionPixel.y++)
        {
            positionPixel.x = (positionPixel.y - ordonneeALOrigine)/coefficient;
            SDL_BlitSurface(pixel, NULL, ecran, &positionPixel);
        }
    }
    if (coefficient <= 1.0)
    {
        for (positionPixel.y = point1.y; positionPixel.y >= point2.y; positionPixel.y--)
        {
            positionPixel.x = (positionPixel.y - ordonneeALOrigine)/coefficient;
            SDL_BlitSurface(pixel, NULL, ecran, &positionPixel);
        }
    }
    if (coefficient > -1.0 && coefficient < 1.0)
    {
        for (positionPixel.x = point1.x; positionPixel.x <= point2.x; positionPixel.x++)
        {
            positionPixel.y = coefficient*(positionPixel.x) + ordonneeALOrigine;
            SDL_BlitSurface(pixel, NULL, ecran, &positionPixel);
        }
    }

    SDL_FreeSurface(pixel);
    SDL_Flip(ecran);
    return EXIT_SUCCESS;
}

