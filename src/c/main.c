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
  		strftime(time_buffer, sizeof("00:00:00"), "%I:%M", tick_time);
  }

  strftime(ampm_buffer, sizeof("xx"), "%P", tick_time);

  if (clock_is_24h_style() == false) {
    text_layer_set_text(ampm_layer, ampm_buffer);
  }

  strftime(date_buffer, sizeof("01.01.2110"), "%m.%d.%Y", tick_time);

  text_layer_set_text(time_layer, time_buffer+(('0' == time_buffer[0])?1:0));
  text_layer_set_text(date_layer, date_buffer);

  size_layers();
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();

  if (tick_time->tm_min == 0) { // Update weather every hour
    bitmap_layer_set_bitmap(weather_icon_layer, NULL);

    DictionaryIterator * iter;
    app_message_outbox_begin(&iter);

    dict_write_uint8(iter, MESSAGE_KEY_MsgKeyCondCode, 0);
    app_message_outbox_send();
  }
}

static void init() {
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  init_messaging();

  main_window_push();
}

static void deinit() {
  persist_write_int(MESSAGE_KEY_MsgKeyCelsius, use_celsius);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
