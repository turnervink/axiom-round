#include <pebble.h>
#include "main.h"
#include "main_window.h"

void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char time_buffer[] = "00:00";
  static char ampm_buffer[] = "xx";

  if (clock_is_24h_style() == true) {
  		strftime(time_buffer, sizeof("00:00:00"), "%H:%M", tick_time);
  } else {
  		strftime(time_buffer, sizeof("00:00:00"), "%I:%M", tick_time);
  }

  strftime(ampm_buffer, sizeof("xx"), "%p", tick_time);

  if (clock_is_24h_style() == false) {
    text_layer_set_text(ampm_layer, ampm_buffer);
  }

  text_layer_set_text(time_layer, time_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  main_window_push();
}

static void deinit() {

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
