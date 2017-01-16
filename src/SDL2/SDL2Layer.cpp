#include "SDL2Layer.h"
#include <stdexcept>
#include <iostream>

using namespace std;

/*
static const uint32_t FPS = 240;
static const uint32_t TICKS_PER_FRAME = 1000 / FPS;



SDL_Color createColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

#define BLACK createColor(0, 0, 0, 255)
#define GREY createColor(0x1E, 0x1E, 0x1E, 255)
#define WHITE createColor(255, 255, 255, 255)


    SDL_Surface *windowSurface = SDL_GetWindowSurface(window);

    SDL_Rect viewPort = {};
    viewPort.x = viewPort.y = 0;
    viewPort.w = windowSurface->w;
    viewPort.h = windowSurface->h;
    BufferRenderer *bufferRenderer = new BufferRenderer(windowSurface, viewPort, font);

    bool quit = false;
    SDL_Event e;
    Timer fpsTimer;
    Timer capTimer;

    fpsTimer.start();

    while (!quit) {
        capTimer.start();

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    bufferRenderer->remove();
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    bufferRenderer->insert('\n');
                } else if (e.key.keysym.sym == SDLK_LEFT) {
                    bufferRenderer->moveCursorBackward();
                } else if (e.key.keysym.sym == SDLK_RIGHT) {
                    bufferRenderer->moveCursorForward();
                } else if (e.key.keysym.sym == SDLK_UP) {
                    bufferRenderer->moveCursorUp();
                } else if (e.key.keysym.sym == SDLK_DOWN) {
                    bufferRenderer->moveCursorDown();
                }
            } else if (e.type == SDL_TEXTINPUT) {
                bufferRenderer->insert(e.text.text);
            } else if (e.type == SDL_WINDOWEVENT) {
                switch (e.window.event) {
                    case SDL_WINDOWEVENT_RESIZED: {
                        windowSurface = SDL_GetWindowSurface(window);
                        viewPort.w = windowSurface->w;
                        viewPort.h = windowSurface->h;
                        bufferRenderer->updateScreen(windowSurface, viewPort);
                    } break;
                    default:break;
                }

            }
        }

        SDL_Surface *bufferSurface = bufferRenderer->createTexture();

        SDL_BlitSurface(bufferSurface, NULL, windowSurface, &viewPort);
        SDL_UpdateWindowSurface(window);

        uint32_t frameTicks = capTimer.getTicks();
        if (frameTicks < TICKS_PER_FRAME) {
            SDL_Delay(TICKS_PER_FRAME - frameTicks);
        }
    }

    SDL_DestroyWindow(window);
    delete bufferRenderer;
    SDL_Quit();
    TTF_CloseFont(font);
    TTF_Quit();
    */

SDL2Layer::SDL2Layer() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw runtime_error(SDL_GetError());
    }

    if (TTF_Init() == -1) {
        throw runtime_error(SDL_GetError());
    }

    window = SDL_CreateWindow("Text Editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        throw runtime_error(SDL_GetError());
    }

    windowSurface = SDL_GetWindowSurface(window);

    font = TTF_OpenFont("assets/fonts/DejaVuSansMono.ttf", 38);
    if (font == NULL) {
        throw runtime_error(TTF_GetError());
    }
    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
    TTF_SetFontOutline(font, 0);
    TTF_SetFontKerning(font, 0);
    TTF_SetFontHinting(font, TTF_HINTING_NORMAL);
}

void SDL2Layer::getFontDimensions(int *width, int *height) {
    TTF_SizeUTF8(font, " ", width, height);
}

Viewport SDL2Layer::getViewport() {
    return Viewport(0, 0, windowSurface->w, windowSurface->h);
}

static SDL_Color createColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

#define BLACK createColor(0, 0, 0, 255)
#define GREY createColor(0x1E, 0x1E, 0x1E, 255)
#define WHITE createColor(255, 255, 255, 255)

void SDL2Layer::render(std::vector<Buffer *> buffers) {
    for (Buffer *& buffer : buffers) {
        SDL_Rect fillRect;
        fillRect.x = buffer->viewport.x;
        fillRect.y = buffer->viewport.y;
        fillRect.w = buffer->viewport.w;
        fillRect.h = buffer->viewport.h;
        SDL_FillRect(windowSurface, &fillRect, SDL_MapRGB(windowSurface->format, 0, 0, 0));

        int x = fillRect.x;
        int y = fillRect.y;

        uint64_t startLine = buffer->lineIndex() - buffer->relativeLineToDisplayCursor;

        for (uint64_t lineToDraw = startLine; lineToDraw < startLine + buffer->linesPerScreen; lineToDraw++) {
            if (lineToDraw < buffer->lines->size()) {

                std::string line = buffer->getLine(lineToDraw);

                if (y < (fillRect.y + fillRect.h)) {
                    int currY = y;
                    if (!line.empty()) {
                        // TODO: Font colors
                        SDL_Surface *textSurface = TTF_RenderUTF8_Shaded(font, line.c_str(), WHITE, BLACK);

                        if (textSurface) {
                            SDL_Rect dst;
                            dst.x = x;
                            dst.y = y;
                            dst.w = textSurface->w;
                            dst.h = textSurface->h;

                            SDL_BlitSurface(textSurface, &fillRect, windowSurface, &dst);

                            y += dst.h;

                            SDL_FreeSurface(textSurface);
                        }
                    } else {
                        int w, h;
                        TTF_SizeUTF8(font, " ", &w, &h);
                        y += h;
                    }

                    if (lineToDraw == buffer->lineIndex()) {
                        std::string beforeCursor = std::string(buffer->cursorLineIndex(), ' ');
                        int w, h;
                        TTF_SizeUTF8(font, beforeCursor.c_str(), &w, &h);
                        SDL_Rect cursorBar;
                        cursorBar.x = x + w;
                        cursorBar.y = currY;
                        cursorBar.w = 2;
                        cursorBar.h = h;

                        SDL_FillRect(windowSurface, &cursorBar, SDL_MapRGB(windowSurface->format, 255, 255, 255));
                    }
                }
            }
        }
    }

    SDL_UpdateWindowSurface(window);
}

void SDL2Layer::startLoop() {
    capTimer.start();
}

void SDL2Layer::finishLoop() {
    uint32_t frameTicks = capTimer.getTicks();
    if (frameTicks < TICKS_PER_FRAME) {
        SDL_Delay(TICKS_PER_FRAME - frameTicks);
    }
}

std::queue<SDL_Event> SDL2Layer::getEvents() {
    std::queue<SDL_Event> events;
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        events.push(e);
    }

    return events;
}

void SDL2Layer::handleResize() {
    windowSurface = SDL_GetWindowSurface(window);
}

