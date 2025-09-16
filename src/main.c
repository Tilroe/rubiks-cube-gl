#include <stdlib.h>
#include <stdio.h>

#include "window.h"
#include "renderer.h"

int main() {
    window_init();
    renderer_init();
    
    // Render loop
    // -------------------------------
    while (!window_should_close()) {
        draw();
        poll_events();
        swap_buffers();
    }

    window_cleanup();

    return 0;
}
