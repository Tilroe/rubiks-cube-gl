#ifndef WINDOW_H
#define WINDOW_H

int window_init();
void window_cleanup();
int window_should_close();
int window_width();
int window_height();

void poll_events();
void swap_buffers();

#endif // !WINDOW_H
