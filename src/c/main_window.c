#include <pebble.h>
#include "main.h"
#include "main_window.h"

Window *main_window;
Layer *background_layer;
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
  graphics_context_set_stroke_color(ctx, GColorBlue);
  graphics_draw_circle(ctx, GPoint(bounds.size.w / 2, bounds.size.h / 2), 90);

  // Draw top lines
  graphics_context_set_stroke_width(ctx, 6);
  graphics_draw_line(ctx, GPoint(0, 60), GPoint(180, 60));
  graphics_draw_line(ctx, GPoint(0, 180 - 60), GPoint(180, 180 - 60));
}

static void main_window_load(Window *window) {
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  window_set_background_color(window, GColorBlack);

  background_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_set_update_proc(background_layer, background_update_proc);

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

  date_title_layer = text_layer_create(GRect(0, 7, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(date_title_layer, GColorClear);
  text_layer_set_text_color(date_title_layer, GColorVividCerulean);
  text_layer_set_text_alignment(date_title_layer, GTextAlignmentCenter);
  text_layer_set_font(date_title_layer, tiny_font);
  text_layer_set_text(date_title_layer, "Day");

  date_layer = text_layer_create(GRect(0, 24, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorOrange);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_font(date_layer, med_font);
  text_layer_set_text(date_layer, "01.01.2110");

  temp_title_layer = text_layer_create(GRect(0, 150, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(temp_title_layer, GColorClear);
  text_layer_set_text_color(temp_title_layer, GColorVividCerulean);
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
  layer_set_frame(text_layer_get_layer(ampm_layer), GRect(bounds.size.w - (ampm_size.w + 10), bounds.size.h / 2 - ampm_size.h / 2, ampm_size.w, ampm_size.h + 5));

  layer_add_child(window_get_root_layer(window), background_layer);
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
