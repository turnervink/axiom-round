#include <pebble.h>
#include "main.h"
#include "main_window.h"

Window *main_window;
Layer *background_layer;
static BitmapLayer *weather_icon_layer;
GFont time_font, med_font, small_font, tiny_font;

void background_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));

  // Draw bottom circle
  graphics_context_set_stroke_width(ctx, 10);
  graphics_context_set_stroke_color(ctx, GColorVividCerulean);
  graphics_draw_circle(ctx, GPoint(bounds.size.w / 2, bounds.size.h / 2), 87);

  // Draw bottom lines
  graphics_context_set_stroke_width(ctx, 12);
  graphics_draw_line(ctx, GPoint(0, 60), GPoint(180, 60));
  graphics_draw_line(ctx, GPoint(0, 180 - 60), GPoint(180, 180 - 60));

  // Draw top circle
  graphics_context_set_stroke_width(ctx, 10);
  graphics_context_set_stroke_color(ctx, GColorBlueMoon);
  graphics_draw_circle(ctx, GPoint(bounds.size.w / 2, bounds.size.h / 2), 90);

  // Draw top lines
  graphics_context_set_stroke_width(ctx, 6);
  graphics_draw_line(ctx, GPoint(0, 60), GPoint(180, 60));
  graphics_draw_line(ctx, GPoint(0, 180 - 60), GPoint(180, 180 - 60));
}

void weather_icon_update_proc(int code) {
  bitmap_layer_set_bitmap(weather_icon_layer, clear_night);
}

static void main_window_load(Window *window) {
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  window_set_background_color(window, GColorBlack);

  background_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_set_update_proc(background_layer, background_update_proc);

  weather_icon_layer = bitmap_layer_create(GRect(20, bounds.size.h / 2 - 8, 16, 16));
  weather_icons = gbitmap_create_with_resource(RESOURCE_ID_WEATHER_ICONS);

  // Create weather icon bitmaps
  clear_day = gbitmap_create_as_sub_bitmap(weather_icons, GRect(0, 0, 16, 16));
  clear_night = gbitmap_create_as_sub_bitmap(weather_icons, GRect(16, 0, 16, 16));
  cloudy = gbitmap_create_as_sub_bitmap(weather_icons, GRect(32, 0, 16, 16));
  pcloudy_day = gbitmap_create_as_sub_bitmap(weather_icons, GRect(48, 0, 16, 16));
  pcloudy_night = gbitmap_create_as_sub_bitmap(weather_icons, GRect(64, 0, 16, 16));
  rain = gbitmap_create_as_sub_bitmap(weather_icons, GRect(0, 16, 16, 16));
  thunder = gbitmap_create_as_sub_bitmap(weather_icons, GRect(16, 16, 16, 16));
  snow = gbitmap_create_as_sub_bitmap(weather_icons, GRect(32, 16, 16, 16));
  fog = gbitmap_create_as_sub_bitmap(weather_icons, GRect(48, 16, 16, 16));
  unknown = gbitmap_create_as_sub_bitmap(weather_icons, GRect(64, 16, 16, 16));


  weather_icon_update_proc(0);

  // Create fonts
  time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TIME_FONT_48));
  med_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MED_FONT_22));
  small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SMALL_FONT_18));
  tiny_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TINY_FONT_16));

  time_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, time_font);

  ampm_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(ampm_layer, GColorClear);
  text_layer_set_text_color(ampm_layer, GColorWhite);
  text_layer_set_font(ampm_layer, small_font);

  date_title_layer = text_layer_create(GRect(0, 8, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(date_title_layer, GColorClear);
  text_layer_set_text_color(date_title_layer, GColorBlueMoon);
  text_layer_set_text_alignment(date_title_layer, GTextAlignmentCenter);
  text_layer_set_font(date_title_layer, tiny_font);
  text_layer_set_text(date_title_layer, "Day");

  date_layer = text_layer_create(GRect(0, 24, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorOrange);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_font(date_layer, med_font);
  text_layer_set_text(date_layer, "01.01.2110");

  temp_title_layer = text_layer_create(GRect(0, 149, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(temp_title_layer, GColorClear);
  text_layer_set_text_color(temp_title_layer, GColorBlueMoon);
  text_layer_set_text_alignment(temp_title_layer, GTextAlignmentCenter);
  text_layer_set_font(temp_title_layer, tiny_font);
  text_layer_set_text(temp_title_layer, "Temp");

  temp_layer = text_layer_create(GRect(5, 127, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(temp_layer, GColorClear);
  text_layer_set_text_color(temp_layer, GColorOrange);
  text_layer_set_text_alignment(temp_layer, GTextAlignmentCenter);
  text_layer_set_font(temp_layer, med_font);
  text_layer_set_text(temp_layer, "--°");

  update_time();

  GSize time_size = text_layer_get_content_size(time_layer);
  GSize ampm_size = text_layer_get_content_size(ampm_layer);
  layer_set_frame(text_layer_get_layer(time_layer), GRect(0, bounds.size.h / 2 - time_size.h / 2 - 8, bounds.size.w, time_size.h));
  layer_set_frame(text_layer_get_layer(ampm_layer), GRect(bounds.size.w - (ampm_size.w + 15), bounds.size.h / 2 - ampm_size.h / 2, ampm_size.w, ampm_size.h + 5));

  layer_add_child(window_get_root_layer(window), background_layer);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(weather_icon_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(ampm_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_title_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(temp_title_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(temp_layer));
}

static void main_window_unload(Window *window) {

}

void main_window_push() {
  main_window = window_create();

  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(main_window, true);
}
