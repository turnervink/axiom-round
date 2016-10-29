#include <pebble.h>
#include "main_window.h"
#include "messaging.h"

char temp_buffer[15];
char tempc_buffer[15];


static void inbox_recv_handler(DictionaryIterator *iter, void *ctx) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Incoming mesage received!");

  Tuple *t = dict_read_first(iter);
  while(t) {

    if (t->key == MESSAGE_KEY_MsgKeyTemp) {
      APP_LOG(APP_LOG_LEVEL_INFO, "Received temp");
      snprintf(temp_buffer, sizeof(temp_buffer), "%d°", (int)t->value->int32);
    }

    if (t->key == MESSAGE_KEY_MsgKeyTempC) {
      APP_LOG(APP_LOG_LEVEL_INFO, "Received tempc");
      snprintf(tempc_buffer, sizeof(tempc_buffer), "%d°", (int)t->value->int32);
    }

    if (t->key == MESSAGE_KEY_MsgKeyCondCode) {
      APP_LOG(APP_LOG_LEVEL_INFO, "Received condcode");
    }

    if (t->key == MESSAGE_KEY_MsgKeyError) {
      APP_LOG(APP_LOG_LEVEL_INFO, "Received error");
    }

    t = dict_read_next(iter);
  }

  APP_LOG(APP_LOG_LEVEL_INFO, "Setting temp text");
  text_layer_set_text(temp_layer, tempc_buffer); // Display weather after all keys received
}

void inbox_failed_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Incoming message dropped! - %d", reason);
}

void outbox_sent_handler(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void outbox_failed_handler(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void init_messaging() {
  app_message_register_inbox_received(inbox_recv_handler);
  app_message_register_inbox_dropped(inbox_failed_handler);
  app_message_register_outbox_sent(outbox_sent_handler);
  app_message_register_outbox_failed(outbox_failed_handler);
  app_message_open(32, 32);
}
