#include "serial_handling.h"

extern shared_vars shared;

// Function to print a request
void print_request(lon_lat_32 start, lon_lat_32 end) {
  Serial.print("R ");
  Serial.print(start.lat);
  Serial.print(" ");
  Serial.print(start.lon);
  Serial.print(" ");
  Serial.print(end.lat);
  Serial.print(" ");
  Serial.print(end.lon);
  Serial.print("\n");
}
const uint16_t buf_size = 256;

char* buffer = (char *) malloc(buf_size); // buffer for reading characters

uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
  // First, send request
  print_request(start, end);

  
  // ########################## Waiting for number of waypoints #############################

  // timeout
  auto time = millis();
  while ((millis() - time <= 10000) && (Serial.available() < 1)) {
    // Wait
  }

  // Resend request if timeout
  if (millis() - time > 10000) {
    return 0;
  }

  // There are characters to be read - clear buffer
  char in_char = NULL;
  int idx = 0;

  // Reads line into character array;
  while (in_char != '\n') {
    while (Serial.available()>0){
      in_char = Serial.read();
      buffer[idx] = in_char;
      idx++;
    }
  }

  // First index of buffer should be 'N'
  if (buffer[0] != 'N') {
    return 0;
  }

  // Convert buffer to string so we can easily extract num_waypoints
  String buffer_str;
  String num_waypoints_str;

  buffer_str = String(buffer);
  //buffer_str.remove(idx-1);
  num_waypoints_str = buffer_str.substring(2,idx-1);
  int16_t num_waypoints = (int16_t)num_waypoints_str.toInt();
  if (num_waypoints > 500) {
    // How we handle too many waypoints
    shared.num_waypoints = 1;
    return 1;
  }
  else if (num_waypoints==1){
    // How we handle no path
    shared.num_waypoints = 1;
    return 1;
  }
  //num_waypoints_str = String(buffer+2);
  //num_waypoints = num_waypoints_str.toInt();
  shared.num_waypoints = num_waypoints;


  // ########################## Waiting for waypoint or E #############################
  for (int i = 0; i<=num_waypoints; i++) {
    Serial.print("A\n");
    Serial.flush();
    // timeout
    auto time = millis();
    while ((millis() - time <= 1000) && (Serial.available() < 1)) {
      // Wait
    }

    // Resend request if timeout
    if (millis() - time > 1000) {
      return 0;
    }

    // There are characters to be read - clear buffer
    char in_char = NULL;
    int idx = 0;
    
    // Reads line into character array;
    while (in_char != '\n') {
      while (Serial.available()>0){
        in_char = Serial.read();
        buffer[idx] = in_char;
        idx++;       
      }

    }
    
    // Check if E was sent to signal end of waypoints
    if (buffer[0] == 'E') {
      return 1;
    }
    // Check if bad character was sent
    else if (buffer[0] != 'W') {
      return 0;
    }

    // Convert buffer to string to more easily extract waypoints
    buffer_str = String(buffer);
    String lon_str = buffer_str.substring(2, 9);
    String lat_str = buffer_str.substring(10, 19);
    int32_t lat, lon;
    lat = (int32_t)lat_str.toInt();
    lon = (int32_t)lon_str.toInt();

    lon_lat_32 waypoint(lat,lon);
    shared.waypoints[i] = waypoint;

  }
  return 1;

  // 1 indicates a successful exchange, of course you should only output 1
  // in your final solution if the exchange was indeed successful
  // (otherwise, return 0 if the communication failed)
}
