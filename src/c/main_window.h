#pragma once

TextLayer *time_layer, *ampm_layer, *date_title_layer, *date_layer, *temp_title_layer, *temp_layer;
GBitmap *weather_icons, *clear_day, *clear_night, *cloudy, *pcloudy_day, *pcloudy_night, *rain, *thunder, *snow, *fog, *unknown;

void weather_icon_update_proc(int code);
void main_window_push();
