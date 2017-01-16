#include "Editor.h"
#include "SDL2/SDL2Layer.h"

int main() {
    PlatformLayer *platformLayer = new SDL2Layer();

    Editor editor(platformLayer);
    editor.loop();

    delete platformLayer;

    return 0;
}