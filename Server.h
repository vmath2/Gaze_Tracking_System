#ifndef Server_H
#define Server_H

#include <systemc.h>

//ROI - Region Of Interest
//NAR - Network access Request
//PI - Packet begin/end indicator
//NS - Network Status

SC_MODULE(server) {
//Mobile 1
sc_in<bool> clock;
sc_in<sc_uint<1> > NAR_for_mobile1; 
sc_in<sc_uint<1> > PI_for_mobile1;	
sc_in<int> ROI_input_1; // ROI input data from moblie 1
sc_in<int> start_time_for_ROI1; // start time for ROI input from mobile 1
sc_in<int> end_time_for_ROI1; // end time for ROI input from mobile 1
//Mobile 2
sc_in<sc_uint<1> > NAR_for_mobile2; 
sc_in<sc_uint<1> > PI_for_mobile2;	
sc_in<int> ROI_input_2; // ROI input data from moblie 2
sc_in<int> start_time_for_ROI2; // start time for ROI input from mobile 2
sc_in<int> end_time_for_ROI2; // end time for ROI input from mobile 2
// Outputs
sc_out<sc_uint<1> > NS_to_mobiles; 
sc_out<sc_uint<1> > ACK_server_to_mobile1; // Acknowledgment signal from server to mobile 1
sc_out<sc_uint<1> > ACK_server_to_mobile2; // Acknowledgment signal from server to mobile 2
sc_out<sc_uint<1> > PI_to_mobile1; 
sc_out<sc_uint<1> > PI_to_mobile2; 
sc_out<bool> server_flag; // is high when data is being sent to mobile by network
double image_size = 8000000;
int image_sent = 0;
double delta;
double gamma;
double bandwidth;
double packet_size;
int num_packet_recieved_mobile1 = 0;
int num_packet_recieved_mobile2 = 0;
std::map<int , std::vector<Tuple_roi> > Map_roi;
std::vector<image_data> images = initialize_image_data();
void schedule_transmission();
void receive_packet(int mobile_id);
bool start_receiving();
void send_image (int image_id);
void process_server();
SC_HAS_PROCESS(server);
server(sc_module_name name , double d , double g , double bw , double p) : sc_module(name) , delta(d) , gamma(g) , bandwidth(bw) , packet_size(p) {
SC_THREAD(process_server);
sensitive<<clock.pos();
}
};
void server::schedule_transmission() {
double no_of_packets = image_size/packet_size; // total number of packets per image
sc_time time_taken = sc_time(image_size/bandwidth,SC_SEC); // time taken to transmit 1 packet
sc_time start_time = sc_time(250,SC_SEC);
for(int i = 9 ; i >= 0 ; i--) {
if(start_time.value()>images[i].end_time.value()){
start_time = images[i].end_time;
}
start_time = start_time - sc_time(no_of_packets*delta , SC_SEC) - time_taken;
double test = start_time.value()/1000000000000;
start_time=sc_time(test , SC_SEC);
images[i].start_time = start_time;
}
}
void server:: receive_packet(int mobile_id) {
Tuple_roi tr = Tuple_roi(0 , 0 , 0);
int count = 0;
switch(mobile_id) {
case 1 : 
tr.roi = ROI_input_1.read();
tr.start_time = start_time_for_ROI1.read();
tr.end_time = end_time_for_ROI1.read();
count = ++num_packet_recieved_mobile1;
break;
case 2 :
tr.roi = ROI_input_2.read();
tr.start_time = start_time_for_ROI2.read();
tr.end_time = end_time_for_ROI2.read();
count = ++num_packet_recieved_mobile2;
break;
default :
break;
}
Map_roi[mobile_id].push_back(tr);
}
bool server:: start_receiving() {
if(NAR_for_mobile1.read() == 0b0 && PI_for_mobile2.read() == 0b0) { // if m1 and m2 idle
return false;
}
if(NAR_for_mobile1.read() == 0b1) {
NS_to_mobiles.write(0b1);
ACK_server_to_mobile1.write(0b1);
wait(10 , SC_NS);
if(PI_for_mobile1.read() == 0b0) {
wait(10 , SC_NS);
}
if(PI_for_mobile1.read() == 0b1) {
wait(500/bandwidth, SC_SEC);
}
NS_to_mobiles.write(0b0);
ACK_server_to_mobile1.write(0b0);
} 
else if(NAR_for_mobile2.read() == 0b1) {
NS_to_mobiles.write(0b1);
ACK_server_to_mobile2.write(0b1);
wait(10 , SC_NS);
if(PI_for_mobile2.read() == 0b0) {
wait(10 , SC_NS);
}
if(PI_for_mobile2.read() == 0b1) {
wait(500/bandwidth, SC_SEC);
}
NS_to_mobiles.write(0b0);
ACK_server_to_mobile2.write(0b0);
}
return true;
}
void server:: send_image (int image_id) {
int mobile_id = images[image_id].image_Id;
double number = image_size/packet_size; // number of packets
double tt = packet_size/bandwidth; // time taken to transmit 1 packet
while(number--) {
NS_to_mobiles.write(0b1);
server_flag.write(true);
switch(mobile_id) {
case 1 : 
PI_to_mobile1.write(0b1);
break;
case 2 :
PI_to_mobile2.write(0b1);
break;
default :
break;
}
wait(tt , SC_SEC);
switch(mobile_id) {
case 1 : 
PI_to_mobile1.write(0b0);
break;
case 2 :
PI_to_mobile2.write(0b0);
break;
default :
break;
}
NS_to_mobiles.write(0b0);
server_flag.write(false);
if(image_sent < 1 || !start_receiving()) {
wait(delta , SC_SEC);
}
}
}
void server::  process_server() {
schedule_transmission();
while(true) {
wait();
for(int i = 0 ; i < 10 ; i++) {
if(sc_time_stamp() == images[i].start_time) {
send_image(i);
image_sent++;
}
}
}
}
#endif