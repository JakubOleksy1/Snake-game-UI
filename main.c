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
    STATE_MENU,
    STATE_GAME
} AppState;

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
    bool hovered;
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
    SDL_Texture *sprite_image;
    SDL_Rect sprite_rect;
} SdlSpriteS;
typedef struct {
    SdlContextS context;
    SdlAssetsS asset;
    SdlMusicS music;
    SdlButtonS buttons[3];
    const uint8_t *keystate;
    AppState state;
    SdlSpriteS sprite;
} SdlHandler;


bool sdl_initialize(SdlHandler *sdl);
void sdl_cleanUp(SdlHandler *sdl, int exit_status);
void menu_cleanUp(SdlHandler *sdl);
bool load_media(SdlHandler *sdl);
bool load_menu(SdlHandler *sdl);
//void sprite_update(SdlHandler *sdl);
void text_update(SdlHandler *sdl);

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));  
    (void)argc;
    (void)argv;

    SdlHandler sdl = {
        .context.window = NULL,
        .context.renderer = NULL,

        .asset.background = NULL,
        .asset.text_font = NULL,

        .buttons[0].text_color = {255, 255, 255, 255},
        .buttons[0].text_rect = {(SCREEN_WIDTH/4), (SCREEN_HEIGHT/4), 0, 0},
        .buttons[0].text_image = NULL,
        .buttons[0].id = START,
        .buttons[0].hovered = FALSE,

        .buttons[1].text_color = {255, 255, 255, 255},
        .buttons[1].text_rect = {(SCREEN_WIDTH/4), (SCREEN_HEIGHT/4)+100, 0, 0},
        .buttons[1].text_image = NULL,
        .buttons[1].id = OPTIONS,
        .buttons[1].hovered = FALSE,

        .buttons[2].text_color = {255, 255, 255, 255},
        .buttons[2].text_rect = {(SCREEN_WIDTH/4), (SCREEN_HEIGHT/4)+200, 0, 0},
        .buttons[2].text_image = NULL,
        .buttons[2].id = STOP,
        .buttons[2].hovered = FALSE,

        .sprite.sprite_image = NULL,
        .sprite.sprite_rect = {0, 0, 0, 0},
        //.text_xval = 3,
        //.text_yval = 3,
        //.sprite_vel = 5,
        .keystate = SDL_GetKeyboardState(NULL),

        .music.soundFoodEaten = NULL,
        .music.soundGameOver = NULL,
        .music.music = NULL,

        .state = STATE_MENU,
    };

    if(sdl_initialize(&sdl)) {
        sdl_cleanUp(&sdl, EXIT_FAILURE);
    }

    if(load_media(&sdl)) {
        sdl_cleanUp(&sdl, EXIT_FAILURE);
    }

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

                        /*case SDL_SCANCODE_SPACE:
                            SDL_SetRenderDrawColor(sdl.context.renderer, rand() % 256,
                                            rand() % 256, rand() % 256, 255);
                            Mix_PlayChannel(-1, sdl.music.soundGameOver, 0);
                            break;*/

                        /*case SDL_SCANCODE_M:
                            if(Mix_PausedMusic()) {
                                Mix_ResumeMusic();
                            } else {
                                Mix_PauseMusic();
                            }
                            break;*/

                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if(sdl.state == STATE_MENU) {
                        int mMX = event.motion.x; // m-mouse M-motion X-x
                        int mMY = event.motion.y; // m-mouse M-motion Y-y
                        
                        for(int i = 0; i < 3; i++) {
                            SDL_Rect *bRectangle = &sdl.buttons[i].text_rect;
                            
                            if( mMX >= bRectangle->x &&
                                mMY >= bRectangle->y&&
                                mMX <= bRectangle->x + bRectangle->w &&
                                mMY <= bRectangle->y + bRectangle->h) 
                            {
                                sdl.buttons[i].hovered = true;
                            } else {
                                sdl.buttons[i].hovered = false;
                            }
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN: 
                    if(sdl.state == STATE_MENU) {
                        int mBX = event.button.x; // m-mouse B-button X-x
                        int mBY = event.button.y; // m-mouse B-button Y-y
                        
                        for(int i = 0; i < 3; i++) {
                            SDL_Rect *bRectangle = &sdl.buttons[i].text_rect;
                            
                            if( mBX >= bRectangle->x &&
                                mBY >= bRectangle->y&&
                                mBX <= bRectangle->x + bRectangle->w &&
                                mBY <= bRectangle->y + bRectangle->h) 
                            {
                                switch(sdl.buttons[i].id) {

                                    case START:
                                        menu_cleanUp(&sdl);
                                        sdl.state = STATE_GAME;
                                        break;

                                    case OPTIONS:
                                        printf("Options");
                                        break;

                                    case STOP:
                                        sdl_cleanUp(&sdl, EXIT_SUCCESS);
                                        break;
                                        
                                    default:
                                        break;
                                }
                            }
                        }
                    }
                    break;
                default: 
                    break;
            }
        }

        //text_update(&sdl);

        //sprite_update(&sdl);
        if (sdl.state == STATE_MENU) {
            SDL_RenderClear(sdl.context.renderer);

            SDL_RenderCopy(sdl.context.renderer, sdl.asset.background, NULL, NULL);

            for(int i = 0; i < 3; i++) {
                if (sdl.buttons[i].hovered) {
                    SDL_SetRenderDrawColor(sdl.context.renderer, 100, 100, 100, 255);
                    SDL_RenderFillRect(sdl.context.renderer, &sdl.buttons[i].text_rect);
                }
                SDL_RenderCopy(sdl.context.renderer, sdl.buttons[i].text_image, NULL, &sdl.buttons[i].text_rect);
            }
            
        } else if (sdl.state == STATE_GAME) {
            // TEMP: simulate game end
            SDL_RenderClear(sdl.context.renderer);
            SDL_RenderCopy(sdl.context.renderer, sdl.sprite.sprite_image, NULL, &sdl.sprite.sprite_rect);
            sdl.state = STATE_MENU;
            load_menu(&sdl);
        }
        //Game game;
            // game_init(&game);
            //while(game.state == PLAYING) {
            //     game_clearScreen(); 
            //     game_input(&game);
            //     game_update(&game);
            //     game_render(&game);
            //     Sleep(100);
            //}
            // game_gameOver_msg(&game);
            // game_end(&game);
            //sdl.state= STATE_MENU
        SDL_RenderPresent(sdl.context.renderer);

        SDL_Delay(16);
    }

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
        sdl->sprite.sprite_rect.x -= sdl->sprite_vel;
    }
    if(sdl->keystate[SDL_SCANCODE_RIGHT] || sdl->keystate[SDL_SCANCODE_D]) {
        sdl->sprite.sprite_rect.x += sdl->sprite_vel; 
    }
    if(sdl->keystate[SDL_SCANCODE_UP] || sdl->keystate[SDL_SCANCODE_W]) {
        sdl->sprite.sprite_rect.y -= sdl->sprite_vel;
    }
    if(sdl->keystate[SDL_SCANCODE_DOWN] || sdl->keystate[SDL_SCANCODE_S]) {
        sdl->sprite.sprite_rect.y += sdl->sprite_vel;
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

    if (load_menu(sdl)) {
        return true;
    }   
    
    sdl->sprite.sprite_image = IMG_LoadTexture(sdl->context.renderer, "images/snake/Snake_head_u.png");
    if(!sdl->sprite.sprite_image) {
        fprintf(stderr, "Error loading texture.\n Error message: %s\n", IMG_GetError());
        return true;
    }

    if(SDL_QueryTexture(sdl->sprite.sprite_image, NULL, NULL, &sdl->sprite.sprite_rect.w, &sdl->sprite.sprite_rect.h)) {
        fprintf(stderr, "Error querying texture.\n Error message: %s\n", SDL_GetError());
        return true;
    }

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

bool load_menu(SdlHandler *sdl) {
    const char *labels[3] = { "START", "OPTIONS", "EXIT" };

    for (int i = 0; i < 3; i++) {
        SDL_Surface *surface = TTF_RenderText_Blended(sdl->asset.text_font, labels[i], sdl->buttons[i].text_color);
        if(!surface) {
            fprintf(stderr, "Error creating surface.\n Error message: %s\n", SDL_GetError());
            return true;
        }

        sdl->buttons[i].text_rect.w = surface->w;
        sdl->buttons[i].text_rect.h = surface->h;
        
        
        sdl->buttons[i].text_image = SDL_CreateTextureFromSurface(sdl->context.renderer, surface);
        SDL_FreeSurface(surface);
        if(!sdl->buttons[i].text_image) {
            fprintf(stderr, "Error creating texture.\n Error message: %s\n", SDL_GetError());
            return true;
        }   
    }
    return false;
}

void menu_cleanUp(SdlHandler *sdl) {
    for (int i = 0; i < 3; i++) {
        if (sdl->buttons[i].text_image) {
            SDL_DestroyTexture(sdl->buttons[i].text_image);
            sdl->buttons[i].text_image = NULL;
        }
    }
}

void sdl_cleanUp(SdlHandler *sdl, int exit_status) {
    Mix_HaltMusic();
    Mix_HaltChannel(-1);

    Mix_FreeMusic(sdl->music.music);
    Mix_FreeChunk(sdl->music.soundFoodEaten);
    Mix_FreeChunk(sdl->music.soundGameOver);
    //SDL_DestroyTexture(sdl->sprite.sprite_image);
    for(int i = 0; i < 3; i++) { 
        if (sdl->buttons[i].text_image) {
            SDL_DestroyTexture(sdl->buttons[i].text_image);
        }
    }

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