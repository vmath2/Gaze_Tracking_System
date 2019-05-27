
#ifndef Library_H
#define Library_H


#include <systemc.h>
#include <map>
#include <sstream>
#include <string>

#define TOTAL_TX_PACKETS 30
#define TUPLES_PER_PACKETS 20

struct Tuple_roi {
    int roi;
	int start_time;
    int end_time;
    Tuple_roi();
    Tuple_roi(int a , int b , int c) : roi(a) , start_time(b) , end_time(c) {};
    Tuple_roi(const Tuple_roi &a) : roi(a.roi) , start_time(a.start_time) , end_time(a.end_time) {};
};

struct image_data {
    int image_Id;
    sc_time start_time;
    sc_time end_time;
    image_data(int x , sc_time start , sc_time end) : image_Id(x) , start_time(start) , end_time(end) {};
};

struct point {
    int x;
    int y;
    point(int x , int y) : x(x) , y(y) {};
};

std::map<int , std::vector<point> > roiMap ();

std::map<int , std::vector<point> > Map_roi = roiMap();

std::map<int , std::vector<point> > roiMap () {
  	std::map<int , std::vector<point> > point_map;
    
    point_map[1].push_back(point(50 , 20));
    point_map[1].push_back(point(400 , 320));
    
    point_map[2].push_back(point(50 , 370));
    point_map[2].push_back(point(450 , 1000));
    
    point_map[3].push_back(point(470 , 20));
    point_map[3].push_back(point(600 , 900));
    
    point_map[4].push_back(point(670 , 40));
    point_map[4].push_back(point(950 , 550));
    
    point_map[5].push_back(point(680 , 700));
    point_map[5].push_back(point(1000 , 1000));
  	return point_map;
}

std::vector<image_data> initialize_image_data() {
    std::vector<image_data> data;
    data.push_back(image_data(2 , sc_time(150,SC_SEC) , sc_time(150,SC_SEC)));
    data.push_back(image_data(2 , sc_time(175,SC_SEC) , sc_time(175,SC_SEC)));
    data.push_back(image_data(2 , sc_time(200,SC_SEC) , sc_time(200,SC_SEC)));
    data.push_back(image_data(1 , sc_time(200,SC_SEC) , sc_time(200,SC_SEC)));
    data.push_back(image_data(1 , sc_time(220,SC_SEC) , sc_time(220,SC_SEC)));
    data.push_back(image_data(1 , sc_time(225,SC_SEC) , sc_time(225,SC_SEC)));
    data.push_back(image_data(2 , sc_time(225,SC_SEC) , sc_time(225,SC_SEC)));
    data.push_back(image_data(1 , sc_time(230,SC_SEC) , sc_time(230,SC_SEC)));
    data.push_back(image_data(1 , sc_time(250,SC_SEC) , sc_time(250,SC_SEC)));
    data.push_back(image_data(2 , sc_time(250,SC_SEC) , sc_time(250,SC_SEC)));
    return data;
}
#endif