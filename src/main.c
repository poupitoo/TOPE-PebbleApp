#include <pebble.h>

static Window *window;

static void window_load(Window *window) {

}

static void window_unload(Window *window) {
	
}

static void init() {
  // Create and Show Window
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
}

static void deinit() {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}