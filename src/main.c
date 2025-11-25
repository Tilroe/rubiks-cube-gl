#include <time.h>
#include <stdio.h>

#include "window.h"
#include "renderer.h"
#include "kernels.h"

#define FPS 144
#define MS_PER_UPDATE (1.0f / (FPS) * 1000)

#ifdef _WIN32
#include <windows.h>
#define SLEEP_MS(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP_MS(ms) usleep((ms) * 1000)
#endif

int main() {
    if (!window_init()) goto cleanup;
    if (!renderer_init()) goto cleanup;
    if (!opencl_init()) goto cleanup;
    
    // Render loop
    // -------------------------------
    while (!window_should_close()) {
        clock_t start = clock();
        draw();
        poll_events();
        swap_buffers();
        clock_t dt = clock() - start;
        int ms = dt / (CLOCKS_PER_SEC / 1000);
        if (MS_PER_UPDATE > dt) SLEEP_MS(MS_PER_UPDATE - dt);
    }

    cleanup:
    window_cleanup();

    return 0;
}
