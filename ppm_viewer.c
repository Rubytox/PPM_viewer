#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <SDL/SDL.h>

typedef struct {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} pixel;

typedef enum {
    PGM = 1,
    PPM = 3
} extension;

void set_pixel(SDL_Surface *surface, int x, int y, pixel p);
void pause();

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s image", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* file = fopen(argv[1], "rb");

    char *type = NULL;
    size_t len = 0;
    getline(&type, &len, file);

    extension ext = strcmp(type, "P5") ? PGM : PPM;

    char *dimensions = NULL;
    getline(&dimensions, &len, file);
    int length = atoi(strtok(dimensions, " "));
    int height = atoi(strtok(NULL, " "));

    char *nuances_str = NULL;
    getline(&nuances_str, &len, file);
    (void) nuances_str;

    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        fprintf(stderr, "Erreur de l'initialisation de la SDL: %s", SDL_GetError());

        exit(EXIT_FAILURE);
    }

    SDL_Surface *ecran = SDL_SetVideoMode(length, height, 32, SDL_HWSURFACE);
    if (!ecran) {
        fprintf(stderr, "Erreur dans la création de la fenêtre : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption(argv[1], NULL);

    uint8_t bytes[3] = { 0 };
    bool sortir = false;
    for (int x = 0; x < length; x++) {
        if (sortir)
            break;

        for (int y = 0; y < height; y++) {
            if (sortir)
                break;

            if (ext == PGM) {
                if (fread(&bytes[0], sizeof(uint8_t), 1, file) != 1) {
                    sortir = true;
                    break;
                }
                pixel p = { bytes[0], bytes[0], bytes[0] };
                set_pixel(ecran, x, y, p);
            }
            else if (ext == PPM) {
                if (fread(&bytes[0], sizeof(uint8_t), 1, file) != 1) {
                    sortir = true;
                    break;
                }
                if (fread(&bytes[1], sizeof(uint8_t), 1, file) != 1) {
                    sortir = true;
                    break;
                }
                if (fread(&bytes[2], sizeof(uint8_t), 1, file) != 1) {
                    sortir = true;
                    break;
                }
                pixel p = { bytes[0], bytes[1], bytes[2] };
                set_pixel(ecran, x, y, p);
            }

        }
    }

    SDL_Flip(ecran);

    pause();

    SDL_Quit();

    return EXIT_SUCCESS;
}


void set_pixel(SDL_Surface *surface, int x, int y, pixel p)
{
    Uint32 *p_screen = (Uint32 *) surface->pixels;

    int offset = (surface->pitch / sizeof(Uint32)) * y + x;
    *(p_screen + offset) = SDL_MapRGB(surface->format, p.r, p.g, p.b);
}

void pause()
{
    bool continuer = true;
    SDL_Event event;

    while (continuer) {
        SDL_WaitEvent(&event);

        switch (event.type) {
        case SDL_QUIT:
            continuer = false;
        }
    }
}
