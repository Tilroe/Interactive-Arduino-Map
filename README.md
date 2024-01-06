# Interactive Arduino Map

An assignment from the winter 2020 semester of CMPUT 275

## Info
* **Names**: 	Rowan Tilroe, Filippo Ciandy

* **IDs**: 	1629172, 1641346

* **Semester** CMPUT 275 WN2020

Assignment 2 Part 2: Driving Route Finder

## Included files:
- server:
  - digraph.cpp, digraph.h
  - dijkstra.cpp, dijkstra.h
  - heap.h
  - Makefile
  - edmonton-roads-2.0.1.txt
  - serialport.cpp, serialport.h
  - server.cpp
  - wdigraph.h
- client:
  - Makefile
  - client.cpp
  - consts_and_types.h
  - draw_route.cpp, draw_route.h
  - instructions.txt
  - lcd_image.cpp, lcd_image.h
  - map_drawing.cpp, map_drawing.h
  - serial_handling.cpp, serial_handling.h
  - README

## Accessories
* Arduino
* Breadboard
* Joystick
* Wires
* 2 Buttons
* TFT display
* USB cable

## Wiring Instructions
* Pin A8 <---> VRy on joystick
* Pin A9 <---> VRx on joystick
* GND <---> GRD on joystick
* 5V <---> 5V on joystick
* Digital Pin 53 <---> SW on joystick
* Zoom Out Button <--> Digital Pin 47
* Zoom In Button  <--> Digital Pin 45
* Zoom Out Button (other side) <--> GRD
* Zoom In Button (other side) <--> GRD


## Running instructions
1. Open two terminals, one in the server folder and one in the client folder
2. In either terminal type "arduino-port-select" and enter two "1"s to connect the Arduino
3. In the client folder, type "make upload" to upload client.cpp to the arduino
4. Once client.cpp is uploaded to the arduino, type "make" followed by "./server" in the server folder to start up the server
5. Use the joystick to move the cursor, use the buttons to zoom the map in or out
6. Pressing the joystick down in one locations and again in another location will provide a driving route between the two selected locations.


## Notes & Assumptions:
* server.cpp:
  * enums were used to control the flow of server.cpp, with two while loops running so long as its respective "state" is the current state. A while(true) loop envelops the state while loops so the server runs continuously after construting the graph.
  * Should an event occur that the states needs to be reset from the current state to the default waiting_for_request state such as a bad character read or an E was sent through the serial port, the code sets its state to waiting_for_request then breaks out of the current state while loop.

* serial_handling.cpp:
  * serial_handling.cpp is broken into two main phases: "Waiting for number of waypoints" and "Waiting for waypoint of E".
  * If at any point something unexpected happens, such as a bad character read, the function returns 0 so that it is called again within client.cpp to send another request.
  * There character array buffer in which serial port characters are read is transformed into a Arduino String once the buffer is fully read into. This is to gain access to useful methods such as toInt and substring.

* draw_route.cpp:
  * The function immediately returns if it is seent that there is only one waypoint (i.e no path).
  * The top left coordinates of the map are used to offset the coordinates of the waypoints onto the tft screen. 

## Makefile targets (server):
* server: Links server.o, digraph.o, dijkstra.o, serial_port.o to create server executable
* server.o: Creates the object file of server.cpp
* digraph.o: Creates the object file of digraph.cpp
* dijkstra.o: Creates the object file of dijkstra.cpp
* serial_port.o: Creates the object file of serial_port.cpp
* clean: Removes the executable and object files listed above
