#include <pebble.h>
#include "main.h"
#include "main_window.h"
#include "messaging.h"

void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char time_buffer[] = "00:00";
  static char ampm_buffer[] = "xx";
  static char date_buffer[] = "01.01.2110";

  if (clock_is_24h_style() == true) {
  		strftime(time_buffer, sizeof("00:00:00"), "%H:%M", tick_time);
  } else {
  		strftime(time_buffer, sizeof("00:00:00"), "%l:%M", tick_time);
  }

  strftime(ampm_buffer, sizeof("xx"), "%P", tick_time);

  if (clock_is_24h_style() == false) {
    text_layer_set_text(ampm_layer, ampm_buffer);
  }

  strftime(date_buffer, sizeof("01.01.2110"), "%m.%d.%Y", tick_time);

  text_layer_set_text(time_layer, "12:30");
  text_layer_set_text(date_layer, date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  init_messaging();

  main_window_push();
}

static void deinit() {

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
