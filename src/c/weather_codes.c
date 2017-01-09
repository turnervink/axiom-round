#include <pebble.h>
#include "main_window.h"
#include "weather_codes.h"

int thunder_codes[7] = {37, 38, 39, 47, 3, 4, 45};
int clearday_codes[3] = {32, 34, 36};
int clearnight_codes[2] = {31, 33};
int pcloudyday_codes[1] = {30};
int pcloudynight_codes[1] = {29};
int cloudy_codes[4] = {26, 27, 28, 44};
int fog_codes[3] = {20, 21, 22};
int snow_codes[9] = {41, 42, 43, 7, 13, 14, 1, 16, 46};
int rain_codes[11] = {5, 6, 7, 9, 10, 11, 12, 17, 18, 35, 40};
int unknown_codes[8] = {19, 0, 1, 2, 23, 24, 25, 3200};

void find_condition_code (int code) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Finding icon for %d", code);
  int display_code;
  int i;

  for (i = 0; i < 11; i++) {
    if (thunder_codes[i] == code) {
      display_code = 1;
      break;
    } else if (clearday_codes[i] == code) {
      display_code = 2;
      break;
    } else if (clearnight_codes[i] == code) {
      display_code = 3;
      break;
    } else if (pcloudyday_codes[i] == code) {
      display_code = 4;
      break;
    } else if (pcloudynight_codes[i] == code) {
      display_code = 5;
      break;
    } else if (cloudy_codes[i] == code) {
      display_code = 6;
      break;
    } else if (fog_codes[i] == code) {
      display_code = 7;
      break;
    } else if (snow_codes[i] == code) {
      display_code = 8;
      break;
    } else if (rain_codes[i] == code) {
      display_code = 9;
      break;
    } else if (unknown_codes[i] == code) {
      display_code = 0;
      break;
    } else {
      display_code = 0;
      break;
    }
  }

  weather_icon_update_proc(display_code);
}
