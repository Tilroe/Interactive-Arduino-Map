//---------------------------------------------------
// Name: Filippo Ciandy, Rowan Tilroe
// ID:1641346,1629172
// CMPUT 275, Winter 2020
// Assingment 2 Part 1: Directions
//---------------------------------------------------
#include "dijkstra.h"
#include "wdigraph.h"
#include "serialport.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

struct Point {
	long long lat;
	long long lon;
};


long long manhattan(const Point& pt1, const Point &pt2) {
	return abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
}
	

void ReadGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points) {
	ifstream myfile;
	myfile.open(filename);

	string type, v1_str, v2_str, lat_str, lon_str, trash;
	string V = "V";

	while (!myfile.eof()) {
		// Read Type (vertex or edge)
		getline(myfile, type, ',');

		if (type.compare(V) == 0) {
			// Vertex
			getline(myfile, v1_str, ',');
			int v1 = stoi(v1_str); // identifier

			// Coordinates
			getline(myfile, lat_str, ',');
			getline(myfile, lon_str, '\n');
			double lat = stod(lat_str);
			double lon = stod(lon_str);
			lat = static_cast <long long> (lat*100000);
			lon = static_cast <long long> (lon*100000);
			Point point;
			point.lat = lat;
			point.lon = lon;

			// Insert vertex and point into their respective data structures
			graph.addVertex(v1);
			points[v1] = point;
		}
		else {
			// Edge
			getline(myfile, v1_str, ',');
			getline(myfile, v2_str, ',');
			int v1 = stoi(v1_str);
			int v2 = stoi(v2_str);
			long long weight = manhattan(points[v1], points[v2]);

			graph.addEdge(v1, v2, weight);
			// Read unuseful text at the end of the line
			getline(myfile, trash);
		}


	}

	myfile.close();
}
	
// Function to find the two closest vertices to two given points on a graph
void find_closest_vertices(int &v1, int &v2, Point p1, Point p2, WDigraph &graph, unordered_map<int, Point> &points) {
	long long min_dist_to_1 = 1000000; // Arbitrary large distance
	long long min_dist_to_2 = 1000000;

	// Find the closest vertecies to the given points
	for (auto vertex : graph.vertices()) {
		if (manhattan(points[vertex], p1) < min_dist_to_1) {
			v1 = vertex;
			min_dist_to_1 = manhattan(points[vertex], p1);
		}
		if (manhattan(points[vertex], p2) < min_dist_to_2) {
			v2 = vertex;
			min_dist_to_2 = manhattan(points[vertex], p2);
		}
	}
	// Now v1 = closest vertex to start, v2 = closest point to end
}

int main(int argc, char *argv[]) {

	// Construct weighted graph & map of points
	WDigraph graph;
	unordered_map<int, Point> points;
	ReadGraph("edmonton-roads-2.0.1.txt", graph, points);

	// Creating an instance of the SerialPort class
	SerialPort Serial("/dev/ttyACM0");

	// The different states the server will find itself in
	enum states {waiting_for_request, waiting_for_A} state;
	state = waiting_for_request;

	// Initialize variables for info we are to send
	vector<int> vertex_path; // first element = end, last element = start
	int num_waypoints;

	
	// Main request processing loop
	while (true) {

		// Waiting for request phase
		while (state == waiting_for_request) {
			// Read entire line from serial port & look for "R"
			string line = Serial.readline(1000);
			string r;
			if (line.compare("") != 0) {
				r = line.substr(0,1);
			}

			if (r.compare("R") == 0) {
				// "R" was read, get lats and lons and put into 2 point objects

				string lat1_str, lon1_str, lat2_str, lon2_str;
				lat1_str = line.substr(2,7);
				lon1_str = line.substr(10,9);
				lat2_str = line.substr(20,7);
				lon2_str = line.substr(28,9);

				double lat1 = stod(lat1_str);
				double lon1 = stod(lon1_str);
				double lat2 = stod(lat2_str);
				double lon2 = stod(lon2_str);

				lat1 = static_cast <long long> (lat1);
				lon1 = static_cast <long long> (lon1);
				lat2 = static_cast <long long> (lat2);
				lon2 = static_cast <long long> (lon2);

				Point point1, point2; // point1 = start, point2 = end
			 	point1.lat = lat1;
				point1.lon = lon1;
				point2.lat = lat2;
				point2.lon = lon2;

				// Find closest vertecies to start and end
				int start_vertex, end_vertex;
				find_closest_vertices(start_vertex, end_vertex, point1, point2, graph, points);

				// Construct search tree
				unordered_map<int, int> search_tree; // Filled in Dijkstra's Algorithm
																						 // Key - vertex, item - predecessor
				dijkstra(graph, start_vertex, search_tree);

				// Construct path from start to end in a vector
				int current_vertex = end_vertex;
				vertex_path.clear();
				vertex_path.push_back(current_vertex);

				while (current_vertex != start_vertex) {
					current_vertex = search_tree[current_vertex];
					vertex_path.push_back(current_vertex);
				}
				num_waypoints = vertex_path.size(); 

				// Change to information sending phase
				state = waiting_for_A;
			}
		}

		// Waiting for "A" phase
		while (state == waiting_for_A) {
			// First, send the number of waypoints
			string num_waypoints_str = to_string(num_waypoints);
			string num_waypoints_line = "N " + num_waypoints_str + "\n";
			while(!Serial.writeline(num_waypoints_line)){ /* Keep trying to write until success */ }

			if (num_waypoints == 1) {
				state = waiting_for_request;
				break; // Go back to waiting for request if there is no path
			}

			// Now wait for A before sending info
			for (int i = num_waypoints-1 ; i >= -1; i--) {
				// Read for A
				string line = Serial.readline(1000);
				// If timeout or bad character recieved, go back to waiting for request
				if (line.compare("") == 0 || line.compare("A\n") != 0) {
					state = waiting_for_request;
					cout << line;
					break;
				}	

				// Send a waypoint
				if (i >= 0) {
					string point_lat = to_string(points[vertex_path[i]].lat);
					string point_lon = to_string(points[vertex_path[i]].lon);
					string waypoint_line = "W " + point_lat + " " + point_lon + "\n";
					while(!Serial.writeline(waypoint_line)){ /* Keep trying to write until success */ }
					cout << waypoint_line;
				}
				// Send E
				else{
					while(!Serial.writeline("E\n")){ /* Keep trying to write until success */ }
					state = waiting_for_request;
					cout << 'E' <<endl;
				}
			}
		}
	}	
	return 0;
}
