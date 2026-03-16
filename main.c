#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "game.h"
//#include "sdl_screen.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define WINDOW_TITLE "Snake Game"
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define IMAGE_FLAGS IMG_INIT_PNG
#define TEXT_SIZE 80
#define MIXER_FLAGS MIX_INIT_OGG
#define MIX_DEFAULT_FREQUENCY 44100 
#define MIX_DEFAULT_FORMAT AUDIO_S16SYS
#define MIX_DEFAULT_CHANNELS 2
#define MIX_MAX_VOLUME SDL_MIX_MAXVOLUME

typedef enum {
    START,
    OPTIONS,
    STOP
} ButtonId;

typedef struct {
    ButtonId id;
    SDL_Color text_color;
    SDL_Rect text_rect;
    SDL_Texture *text_image;
} SdlButtonS;

typedef struct {
    Mix_Chunk *soundFoodEaten;
    Mix_Chunk *soundGameOver;
    Mix_Music *music;
} SdlMusicS;

typedef struct {
    SDL_Texture *background;
    TTF_Font *text_font;
} SdlAssetsS;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} SdlContextS;

typedef struct {
    SdlContextS context;
    SdlAssetsS asset;
    SdlMusicS music;
    SdlButtonS buttons[3];
    const uint8_t *keystate;
} SdlHandler;

bool sdl_initialize(SdlHandler *sdl);
void sdl_cleanUp(SdlHandler *sdl, int exit_status);
bool load_media(SdlHandler *sdl);
//void sprite_update(SdlHandler *sdl);
void text_update(SdlHandler *sdl);

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));  
    (void)argc;
    (void)argv;
    //Game game;

    SdlHandler sdl = {
        .context.window = NULL,
        .context.renderer = NULL,
        .asset.background = NULL,
        .asset.text_font = NULL,
        .buttons[0].text_color = {255, 255, 255, 255},
        .buttons[0].text_rect = {(SCREEN_WIDTH/4), (SCREEN_HEIGHT/4), 0, 0},
        .buttons[0].text_image = NULL,
        .buttons[1].text_color = {255, 255, 255, 255},
        .buttons[1].text_rect = {(SCREEN_WIDTH/4), (SCREEN_HEIGHT/4)+100, 0, 0},
        .buttons[1].text_image = NULL,
        .buttons[2].text_color = {255, 255, 255, 255},
        .buttons[2].text_rect = {(SCREEN_WIDTH/4), (SCREEN_HEIGHT/4)+200, 0, 0},
        .buttons[2].text_image = NULL,
        //.text_xval = 3,
        //.text_yval = 3,
        //.sprite_image = NULL,
        //.sprite_rect = {0, 0, 0, 0},
        //.sprite_vel = 5,
        .keystate = SDL_GetKeyboardState(NULL),
        .music.soundFoodEaten = NULL,
        .music.soundGameOver = NULL,
        .music.music = NULL,
    };

    if(sdl_initialize(&sdl)) {
        sdl_cleanUp(&sdl, EXIT_FAILURE);
    }

    if(load_media(&sdl)) {
        sdl_cleanUp(&sdl, EXIT_FAILURE);
    }
    // game_init(&game);

    if(Mix_PlayMusic(sdl.music.music, -1)) {
        fprintf(stderr, "Error playing music.\n Error message: %s\n", Mix_GetError());
        return true;
    }
    
    while(true) {

        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            switch(event.type) {    
                case SDL_QUIT:
                    sdl_cleanUp(&sdl, EXIT_SUCCESS);
                    break;
                
                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            sdl_cleanUp(&sdl, EXIT_SUCCESS);
                            break;
                        case SDL_SCANCODE_SPACE:
                            /*SDL_SetRenderDrawColor(sdl.context.renderer, rand() % 256,
                                            rand() % 256, rand() % 256, 255);*/
                            Mix_PlayChannel(-1, sdl.music.soundGameOver, 0);
                            break;
                        case SDL_SCANCODE_M:
                            if(Mix_PausedMusic()) {
                                Mix_ResumeMusic();
                            } else {
                                Mix_PauseMusic();
                            }
                            break;
                        /*case SDL_SCANCODE_N:
                            if(Mix_PausedAudio(sdl.soundFoodEaten) && Mix_PausedAudio(sdl.soundGameOver)) {
                                Mix_ResumeAudio(sdl.soundFoodEaten);
                                Mix_ResumeAudio(sdl.soundGameOver);
                            } else {
                                Mix_PauseAudio(sdl.soundFoodEaten);
                                Mix_PauseAudio(sdl.soundGameOver);
                            }*/
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        //text_update(&sdl);

        //sprite_update(&sdl);

        SDL_RenderClear(sdl.context.renderer);

        SDL_RenderCopy(sdl.context.renderer, sdl.asset.background, NULL, NULL);

        SDL_RenderCopy(sdl.context.renderer, sdl.buttons[0].text_image, NULL, &sdl.buttons[0].text_rect);
        SDL_RenderCopy(sdl.context.renderer, sdl.buttons[1].text_image, NULL, &sdl.buttons[1].text_rect);
        SDL_RenderCopy(sdl.context.renderer, sdl.buttons[2].text_image, NULL, &sdl.buttons[2].text_rect);
        //SDL_RenderCopy(sdl.context.renderer, sdl.sprite_image, NULL, &sdl.sprite_rect);

        SDL_RenderPresent(sdl.context.renderer);

        SDL_Delay(16);
    }

    //while(game.state == PLAYING) {
    //     game_clearScreen(); 
    //     game_input(&game);
    //     game_update(&game);
    //     game_render(&game);
    //     Sleep(100);
    //}
    // game_gameOver_msg(&game);
    // game_end(&game); 
    sdl_cleanUp(&sdl, EXIT_SUCCESS);
    return 0;
}


/*void text_update(SdlHandler *sdl) {
    sdl->text_rect.x += sdl->text_xval;
    sdl->text_rect.y += sdl->text_yval; 
    if(sdl->text_rect.x + sdl->text_rect.w > SCREEN_WIDTH) {
        sdl->text_xval *= -1;
        Mix_PlayChannel(-1, sdl->soundFoodEaten, 0);
    }
    if(sdl->text_rect.x < 0) {
        sdl->text_xval *= -1;
        Mix_PlayChannel(-1, sdl->soundFoodEaten, 0);
    }
    if(sdl->text_rect.y + sdl->text_rect.h > SCREEN_HEIGHT) {
        sdl->text_yval *= -1;
        Mix_PlayChannel(-1, sdl->soundFoodEaten, 0);
    }
    if(sdl->text_rect.y < 0) {
        sdl->text_yval *= -1;
        Mix_PlayChannel(-1, sdl->soundFoodEaten, 0);
    }
}*/

/*void sprite_update(SdlHandler *sdl) {
    if(sdl->keystate[SDL_SCANCODE_LEFT] || sdl->keystate[SDL_SCANCODE_A]) {
        sdl->sprite_rect.x -= sdl->sprite_vel;
    }
    if(sdl->keystate[SDL_SCANCODE_RIGHT] || sdl->keystate[SDL_SCANCODE_D]) {
        sdl->sprite_rect.x += sdl->sprite_vel; 
    }
    if(sdl->keystate[SDL_SCANCODE_UP] || sdl->keystate[SDL_SCANCODE_W]) {
        sdl->sprite_rect.y -= sdl->sprite_vel;
    }
    if(sdl->keystate[SDL_SCANCODE_DOWN] || sdl->keystate[SDL_SCANCODE_S]) {
        sdl->sprite_rect.y += sdl->sprite_vel;
    }
}*/

bool load_media(SdlHandler *sdl) {
    sdl->asset.background = IMG_LoadTexture(sdl->context.renderer, "images/backgrounds/background.png");
    if(!sdl->asset.background) {
        fprintf(stderr, "Error creating texture.\n Error message: %s\n", IMG_GetError());
        return true;
    }

    sdl->asset.text_font = TTF_OpenFont("fonts/freesansbold.ttf", TEXT_SIZE); 
    if(!sdl->asset.text_font) {
        fprintf(stderr, "Error creating font.\n Error message: %s\n", TTF_GetError());
        return true;
    }

    SDL_Surface *surface_start = TTF_RenderText_Blended(sdl->asset.text_font, "START", sdl->buttons[0].text_color);
    if(!surface_start) {
        fprintf(stderr, "Error creating surface.\n Error message: %s\n", SDL_GetError());
        return true;
    }

    SDL_Surface *surface_options = TTF_RenderText_Blended(sdl->asset.text_font, "OPTIONS", sdl->buttons[1].text_color);
    if(!surface_options) {
        fprintf(stderr, "Error creating surface.\n Error message: %s\n", SDL_GetError());
        return true;
    }

    SDL_Surface *surface_exit = TTF_RenderText_Blended(sdl->asset.text_font, "EXIT", sdl->buttons[2].text_color);
    if(!surface_exit) {
        fprintf(stderr, "Error creating surface.\n Error message: %s\n", SDL_GetError());
        return true;
    }

    sdl->buttons[0].text_rect.w = surface_start->w;
    sdl->buttons[0].text_rect.h = surface_start->h;
    sdl->buttons[1].text_rect.w = surface_options->w;
    sdl->buttons[1].text_rect.h = surface_options->h;
    sdl->buttons[2].text_rect.w = surface_exit->w;
    sdl->buttons[2].text_rect.h = surface_exit->h;
    
    sdl->buttons[0].text_image = SDL_CreateTextureFromSurface(sdl->context.renderer, surface_start);
    sdl->buttons[1].text_image = SDL_CreateTextureFromSurface(sdl->context.renderer, surface_options);
    sdl->buttons[2].text_image = SDL_CreateTextureFromSurface(sdl->context.renderer, surface_exit);
    SDL_FreeSurface(surface_start);
    if(!sdl->buttons[0].text_image) {
        fprintf(stderr, "Error creating texture.\n Error message: %s\n", SDL_GetError());
        return true;
    }
    SDL_FreeSurface(surface_options);
    if(!sdl->buttons[1].text_image) {
        fprintf(stderr, "Error creating texture.\n Error message: %s\n", SDL_GetError());
        return true;
    }
    SDL_FreeSurface(surface_exit);
    if(!sdl->buttons[2].text_image) {
        fprintf(stderr, "Error creating texture.\n Error message: %s\n", SDL_GetError());
        return true;
    }
    
    /*sdl->sprite_image = IMG_LoadTexture(sdl->context.renderer, "images/snake/Snake_head_u.png");
    if(!sdl->sprite_image) {
        fprintf(stderr, "Error loading texture.\n Error message: %s\n", IMG_GetError());
        return true;
    }

    if(SDL_QueryTexture(sdl->sprite_image, NULL, NULL, &sdl->sprite_rect.w, &sdl->sprite_rect.h)) {
        fprintf(stderr, "Error querying texture.\n Error message: %s\n", SDL_GetError());
        return true;
    }*/

    sdl->music.soundFoodEaten = Mix_LoadWAV("sounds/food_eaten.ogg");
    if(!sdl->music.soundFoodEaten) {
        fprintf(stderr, "Error loading sound chunk.\n Error message: %s\n", Mix_GetError());
        return true;
    }

    sdl->music.soundGameOver = Mix_LoadWAV("sounds/game_over.ogg");
    if(!sdl->music.soundGameOver) {
        fprintf(stderr, "Error loading sound chunk.\n Error message: %s\n", Mix_GetError());
        return true;
    }

    sdl->music.music = Mix_LoadMUS("music/background-music.ogg");
    if(!sdl->music.music) {
        fprintf(stderr, "Error loading music.\n Error message: %s\n", Mix_GetError());
        return true;
    }

    return false;
}

void sdl_cleanUp(SdlHandler *sdl, int exit_status) {
    Mix_HaltMusic();
    Mix_HaltChannel(-1);

    Mix_FreeMusic(sdl->music.music);
    Mix_FreeChunk(sdl->music.soundFoodEaten);
    Mix_FreeChunk(sdl->music.soundGameOver);
    //SDL_DestroyTexture(sdl->sprite_image);
    SDL_DestroyTexture(sdl->buttons->text_image);
    TTF_CloseFont(sdl->asset.text_font);
    SDL_DestroyTexture(sdl->asset.background);
    SDL_DestroyRenderer(sdl->context.renderer);
    SDL_DestroyWindow(sdl->context.window);

    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    exit(exit_status);
}

bool sdl_initialize(SdlHandler *sdl) {
    if(SDL_Init(SDL_INIT_EVERYTHING != 0)) {
        fprintf(stderr, "Error initializing SDL.\n Error message: %s\n", SDL_GetError());
        return true;
    }

    int img_init = IMG_Init(IMAGE_FLAGS);
    if((img_init & IMAGE_FLAGS) != IMAGE_FLAGS) {
        fprintf(stderr, "Error initializing SDL_image.\n Error message: %s\n", IMG_GetError());
        return true;
    }

    if(TTF_Init()) {
        fprintf(stderr, "Error initializing SDL_ttf.\n Error message: %s\n", TTF_GetError());
        return true;
    }

    int mix_init = Mix_Init(MIXER_FLAGS);
    if((mix_init & MIXER_FLAGS) != MIXER_FLAGS) {
        fprintf(stderr, "Error initializing SDL_mixer.\n Error message: %s\n", Mix_GetError());
        return true;
    }

    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024)) {
        fprintf(stderr, "Error opening audio.\n Error message: %s\n", Mix_GetError());
        return true;
    }

    sdl->context.window = SDL_CreateWindow(WINDOW_TITLE, 
                    SDL_WINDOWPOS_CENTERED, 
                    SDL_WINDOWPOS_CENTERED, 
                    SCREEN_WIDTH, 
                    SCREEN_HEIGHT, 
                    SDL_WINDOW_RESIZABLE);

    if(!sdl->context.window) {
        fprintf(stderr, "Error creating window.\n Error message: %s\n", SDL_GetError());
        return true;
    }

    sdl->context.renderer = SDL_CreateRenderer(sdl->context.window, -1, 0);
    
    if(!sdl->context.renderer) {
        fprintf(stderr, "Error creating renderer.\n Error message: %s\n", SDL_GetError());
        return true;
    }

    SDL_Surface *icon_surf = IMG_Load("images/logo/snake-logo.png");
    if(!icon_surf) {
        fprintf(stderr, "Error loading surface.\n Error message: %s\n", IMG_GetError());
        return true;
    }

    SDL_SetWindowIcon(sdl->context.window, icon_surf);
    SDL_FreeSurface(icon_surf);

    return false; 
}