#include <pebble.h>
#include "main.h"
#include "hands.h"

#define ANTIALIASING true

//WINDOW
Window* window;
//LAYER
Layer* layer;

static Layer *window_layer;

//BACKGROUND
GBitmap* background;

//SETTINGS
bool seconds = true;
bool date = true;

// A struct for our specific settings (see main.h)
ClaySettings settings;

int stringToInt(char *str);

static GPath *s_minute_arrow, *s_hour_arrow;


// Initialize the default settings
static void prv_default_settings() {	
  settings.background = 0;
  settings.secs = true;
  settings.hrcol = GColorRed;
  settings.mincol = GColorBlue;
  settings.seccol = GColorRed;
}

static void tick(struct tm *tick_time, TimeUnits units_changed);

void update_layer(Layer *me, GContext* ctx) 
{
//watchface drawing
		
	//draw background
	graphics_draw_bitmap_in_rect(ctx,background,GRect(0,0,180,180));
	
	//get tick_time
	time_t temp = time(NULL); 
  	struct tm *tick_time = localtime(&temp);
	

	//draw hands
	GPoint center = GPoint(90,90);
	int16_t secondHandLength = 88;

	graphics_context_set_fill_color(ctx, GColorBrass  );
	graphics_fill_circle(ctx, center, 24);
	
	
	GPoint secondHand;

	int32_t second_angle = TRIG_MAX_ANGLE * tick_time->tm_sec / 60;
	secondHand.y = (int16_t)(-cos_lookup(second_angle) * (int32_t)secondHandLength / TRIG_MAX_RATIO) + center.y;
	secondHand.x = (int16_t)(sin_lookup(second_angle) * (int32_t)secondHandLength / TRIG_MAX_RATIO) + center.x;
		
	if((settings.secs) && (seconds == 1)) {
	
		tick_timer_service_subscribe(SECOND_UNIT, tick);
		graphics_context_set_stroke_width(ctx, 1);
//		graphics_context_set_stroke_color(ctx, GColorRed);
		graphics_context_set_stroke_color(ctx, settings.seccol);
		graphics_draw_line(ctx, center, secondHand);
		
	} else {
		tick_timer_service_subscribe(MINUTE_UNIT, tick);
	}			


	graphics_context_set_stroke_width(ctx, 2);
	graphics_context_set_antialiased(ctx, ANTIALIASING);

	
	// minute hand
//	graphics_context_set_stroke_color(ctx, GColorBlue);
	graphics_context_set_stroke_color(ctx, settings.mincol);
	gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * tick_time->tm_min / 60);
	gpath_draw_outline(ctx, s_minute_arrow);

	// hour hand
//	graphics_context_set_stroke_color(ctx, GColorRed);
	graphics_context_set_stroke_color(ctx, settings.hrcol);
	gpath_rotate_to(s_hour_arrow, (TRIG_MAX_ANGLE * (((tick_time->tm_hour % 12) * 6) + (tick_time->tm_min / 10))) / (12 * 6)); // from Pebble SDK example
	gpath_draw_outline(ctx, s_hour_arrow);
	
	

	graphics_context_set_fill_color(ctx, GColorArmyGreen);
	graphics_fill_circle(ctx, center, 6);

}

// Read settings from persistent storage
static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Save the settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  // Update the display based on new settings
  prv_update_display();
}



// Update the display elements
static void prv_update_display() {
	
	switch(settings.background) {
		
	case 0:  // 

	    if (background) {
		gbitmap_destroy(background);
		background = NULL;
	    }
		background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
	    break;		
		
	case 1:  // 

	    if (background) {
		gbitmap_destroy(background);
		background = NULL;
	    }
				background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND1);
	    break;		
	
	case 2:  // 

	    if (background) {
		gbitmap_destroy(background);
		background = NULL;
	    }
		background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND2);
	    break;		

   }
}

int stringToInt(char *str){
    int i=0,sum=0;
    while(str[i]!='\0'){
         if(str[i]< 48 || str[i] > 57){
            // if (DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to convert it into integer.");
          //   return 0;
         }
         else{
             sum = sum*10 + (str[i] - 48);
             i++;
         }
    }
    return sum;
}

// Handle the response from AppMessage
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
	
  // background
  Tuple *bg_t = dict_find(iter, MESSAGE_KEY_background);
  if (bg_t) {
    settings.background = stringToInt((char*) bg_t->value->data);
  }
	
// secs
  Tuple *secs_t = dict_find(iter, MESSAGE_KEY_secs);
  if (secs_t) {
    settings.secs = secs_t->value->int32 == 1;
  }

 // Hour hand Color
  Tuple *hr_color_t = dict_find(iter, MESSAGE_KEY_hrcol);
  if (hr_color_t) {
    settings.hrcol = GColorFromHEX(hr_color_t->value->int32);
  }
	
 // Minute hand Color
  Tuple *min_color_t = dict_find(iter, MESSAGE_KEY_mincol);
  if (min_color_t) {
    settings.mincol = GColorFromHEX(min_color_t->value->int32);
  }
	
 // Second hand Color
  Tuple *sec_color_t = dict_find(iter, MESSAGE_KEY_seccol);
  if (sec_color_t) {
    settings.seccol = GColorFromHEX(sec_color_t->value->int32);
  }

// Save the new settings to persistent storage
  prv_save_settings();
}

void tick(struct tm *tick_time, TimeUnits units_changed) {	

	//redraw every tick
	layer_mark_dirty(layer);
	
}


void init() {
	
  prv_load_settings();
	
  // Listen for AppMessages
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);

  // international support
  setlocale(LC_ALL, "");
	
	//create window
	window = window_create();
	window_set_background_color(window,GColorBlack);
	window_stack_push(window, true);
	Layer* window_layer = window_get_root_layer(window);	
	GRect bounds = layer_get_bounds(window_layer);

	//load background
	background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
	
	//create layer
#if PBL_PLATFORM_CHALK	
	layer = layer_create(GRect(0,0,180,180));
#else
	layer = layer_create(GRect(0,0,144,168));
#endif
	layer_set_update_proc(layer, update_layer);
	layer_add_child(window_layer, layer);	


  prv_update_display();
	

  //subscribe to seconds tick event
  tick_timer_service_subscribe(MINUTE_UNIT, tick);
	
	// init hand paths
	s_minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
	s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);
	GPoint center = grect_center_point(&bounds);
	gpath_move_to(s_minute_arrow, center);
	gpath_move_to(s_hour_arrow, center);
	
	
	// Avoids a blank screen on watch start.
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);  
  tick(tick_time, DAY_UNIT + HOUR_UNIT + MINUTE_UNIT);
	
}

static void deinit(void) {

	layer_destroy(layer);
	
	gbitmap_destroy(background);
	
	window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}