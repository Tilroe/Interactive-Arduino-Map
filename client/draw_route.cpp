#include "draw_route.h"
#include "map_drawing.h"
#define MAGENTA 0xF81F

extern shared_vars shared;


void draw_route() {
  //Display NO PATH message and return if theres no path
  if (shared.num_waypoints == 1){
  	status_message("NO PATH!");
  	delay(1000);
  	status_message("FROM?");
  	return;	
  }
  //Otherwise there is a path to be drawn

  //The top left coordinates currently being shown
  //on the display
  int32_t min_x = (shared.map_coords).x;
  int32_t min_y = (shared.map_coords).y;

  //Iterate through all the waypoints and check if we could draw it
  for (int i=1;i<shared.num_waypoints;i++){
  	int32_t lat1,lon1,lat2,lon2;
  	lon1 = shared.waypoints[i-1].lon;
  	lon2 = shared.waypoints[i].lon;
  	lat1 = shared.waypoints[i-1].lat;
  	lat2 = shared.waypoints[i].lat;

  	//Finding the X and Y in tft display coordinates
  	int32_t x1 = longitude_to_x(shared.map_number,lon1) - min_x;
  	int32_t y1 = latitude_to_y(shared.map_number,lat1) - min_y;
  	int32_t x2 = longitude_to_x(shared.map_number,lon2) - min_x;
  	int32_t y2 = latitude_to_y(shared.map_number,lat2) - min_y;

  	(shared.tft)->drawLine(x1,y1,x2,y2,MAGENTA);
  }
}
