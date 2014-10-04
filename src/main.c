#include <pebble.h>

static Window *window;
static TextLayer *info_layer;

static const uint8_t MESSAGE_TYPE_APP_START = 0;
static const uint8_t MESSAGE_TYPE_TOPE_EVENT = 1;

static void window_load(Window *window) {
	info_layer = text_layer_create(GRect(0, 5, 144, 20));
	text_layer_set_font(info_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text(info_layer, "...");
	text_layer_set_text_alignment(info_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(info_layer));
}

static void window_unload(Window *window) {
	text_layer_destroy(info_layer);
}

void out_sent_handler(DictionaryIterator *sent, void *context) {
	// outgoing message was delivered
}


void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	// outgoing message failed
}


void in_received_handler(DictionaryIterator *received, void *context) {
	// Check for fields you expect to receive
	Tuple *text_tuple = dict_find(received, 0);
	// Act on the found fields received
	if (text_tuple) {
		text_layer_set_text(info_layer, text_tuple->value->cstring);
	}
}


void in_dropped_handler(AppMessageResult reason, void *context) {
	// incoming message dropped
}

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
	text_layer_set_text(info_layer, "...");
}

void accel_tap_handler(AccelAxisType axis, int32_t direction) {
	
	time_t time_tope = time(NULL);
	// Change Display
	text_layer_set_text(info_layer, "Tope!");
	// Send Message to Mobile App
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	dict_write_uint8( iter, 0, MESSAGE_TYPE_TOPE_EVENT);
	dict_write_uint16( iter, 1, time_tope);
	app_message_outbox_send();
}


void config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
}

static void init() {
	// Create and Show Window
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});
	window_stack_push(window, true);
	// Associate Click Config Provider 
	window_set_click_config_provider(window, config_provider);
	// App Message Handler
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_register_outbox_sent(out_sent_handler);
	app_message_register_outbox_failed(out_failed_handler);

	const uint32_t inbound_size = 64;
	const uint32_t outbound_size = 64;
	app_message_open(inbound_size, outbound_size);
	// 
	accel_tap_service_subscribe(accel_tap_handler);
}

static void deinit() {
	window_destroy(window);
	accel_tap_service_unsubscribe();
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}