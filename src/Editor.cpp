#include "Editor.h"
#include "python.h"

Editor::Editor(PlatformLayer *platformLayer) : platformLayer(platformLayer) {
    Viewport vp = platformLayer->getViewport();
    int fontWidth, fontHeight;
    platformLayer->getFontDimensions(&fontWidth, &fontHeight);
    visibleBuffers.push_back(new Buffer("assets/testFileText", fontWidth, fontHeight, vp));
    activeBuffer = visibleBuffers.begin();
    start_python();
}

void Editor::loop() {
    bool running = true;
    while (running) {
        platformLayer->startLoop();
        platformLayer->render(visibleBuffers);
        std::queue<SDL_Event> events = platformLayer->getEvents();
        while (!events.empty()) {
            SDL_Event e = events.front();
            switch (e.type) {
                case SDL_QUIT: {
                    running = false;
                } break;

                case SDL_KEYDOWN: {
                    switch (e.key.keysym.sym) {
                        case SDLK_BACKSPACE: {
                            (*activeBuffer)->remove();
                        } break;

                        case SDLK_DELETE: {
                            (*activeBuffer)->moveCursorForward();
                            (*activeBuffer)->remove();
                        } break;

                        case SDLK_RETURN: {
                            (*activeBuffer)->insert('\n');
                        } break;

                        case SDLK_LEFT: {
                            (*activeBuffer)->moveCursorBackward();
                        } break;

                        case SDLK_RIGHT: {
                            (*activeBuffer)->moveCursorForward();
                        } break;

                        case SDLK_UP: {
                            (*activeBuffer)->moveCursorUp();
                        } break;

                        case SDLK_DOWN: {
                            (*activeBuffer)->moveCursorDown();
                        } break;

                        default: {

                        } break;
                    }
                } break;

                case SDL_TEXTINPUT: {
                    int i = 0;
                    while (char c = e.text.text[i++]) {
                        (*activeBuffer)->insert(c);
                    }

                }

                case SDL_WINDOWEVENT: {
                    switch (e.window.event) {
                        case SDL_WINDOWEVENT_RESIZED: {
                            platformLayer->handleResize();
                            // TODO: Handle more than 1 buffer on screen?
                            (*activeBuffer)->updateViewport(platformLayer->getViewport());
                        } break;

                        default: {

                        } break;
                    }
                } break;

                default: {

                } break;
            }
            events.pop();
        }
        platformLayer->finishLoop();
    }
}
