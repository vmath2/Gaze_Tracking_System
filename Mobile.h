#ifndef Mobile_H
#define Mobile_H

#include <systemc.h>
#include<Library.h>
SC_MODULE(mobile) {

// Inputs
sc_in<bool> clock;
sc_in<sc_uint<1> > server_input1; 
sc_in<sc_uint<1> > server_input2; 
sc_in<sc_uint<1> > server_input3;

// Outputs
sc_out<sc_uint<1> > mobile_output1; 
sc_out<sc_uint<1> > mobile_output2; 
sc_out<int> roi_output; 
sc_out<int> start_output; 
sc_out<int> end_output; 
sc_out<int> memory_output;
sc_out<bool> check_mobSending;

int id;
int txCounter = TOTAL_TX_PACKETS;
int image_memory = 0;
double image_size = 8000000;
double packet_size;
double bandwidth;
int received_image = 0;
std::vector<image_data> images = initialize_image_data();
int number_packets_received = 0; 

int tx_counter = (TOTAL_TX_PACKETS);       

point process_sensor();
int process_convert(struct point a);
std::vector<Tuple_roi> process_compress (int roi , std::vector<Tuple_roi> result , int x);
std::vector<Tuple_roi> process_packetsize ();
void send_packet (std::vector<Tuple_roi> p);
void process_send();
void process_receive();
void process_mobile();


SC_HAS_PROCESS(mobile);
mobile(sc_module_name name , int id , double packetSize , double bw) : sc_module(name) , id(id)  , packet_size(packetSize) , bandwidth(bw) {

SC_THREAD(process_mobile);
sensitive<<clock.pos();
}

};

point mobile:: process_sensor() {

int x = rand() % 1023;
int y = rand() % 1023;
return point(x , y);
}

int mobile:: process_convert(struct point a) {

for(int i = 1 ; i <= 5 ; i++) {

if(a.x >= Map_roi[i][0].x && a.x <= Map_roi[i][1].x && a.y >= Map_roi[i][0].y && a.y <= Map_roi[i][1].y) {
return i;
}
}
return 0;
}

std::vector<Tuple_roi> mobile:: process_compress(int roi , std::vector<Tuple_roi> result , int x) {

if(!result.size()) {
result.push_back(Tuple_roi(roi , 0 , 1));
return result;
}

if(result.back().roi == roi) {
result.back().end_time = x;
}

else {
result.push_back(Tuple_roi(roi , x , x));
}

return result;
}

std::vector<Tuple_roi> mobile:: process_packetsize() {

std::vector<Tuple_roi> result;
int count = 0;
while(result.size() < (TUPLES_PER_PACKETS)) {

point p = process_sensor();
int roi = process_convert(p);
if(roi) {
count++;
result = process_compress(roi , result , count);
}
}

return result;
}

void mobile:: send_packet(std::vector<Tuple_roi> tuple) {

for(int i = 0 ; i < 20 ; i++) {

roi_output.write(tuple[i].roi); 
start_output.write(tuple[i].start_time); 
end_output.write(tuple[i].end_time); 
wait(2 , SC_NS); 
}
}

void mobile:: process_send() {

if(txCounter < received_image * 20) {

if(server_input1.read() == 0b1) { 
return;
}

mobile_output1.write(0b1);
wait(20 , SC_NS);
if(server_input2.read() == 0b0) {
mobile_output1.write(0b0);
return;
}

check_mobSending.write(true);
mobile_output2.write(0b1); 
wait(500/bandwidth , SC_SEC); 
mobile_output2.write(0b0);
check_mobSending.write(false);
mobile_output1.write(0b0);
txCounter++;
}
}

void mobile:: process_receive() {

double number_packets = image_size/packet_size; 
double time = packet_size/bandwidth; 
if(server_input1.read() == 0b0) { 
return;
}
wait(10 , SC_NS);
if(server_input3.read() == 0b0) {
wait(10 , SC_NS);
}
if(server_input3.read() == 0b1) {

wait(time, SC_SEC);
number_packets_received++;
if(number_packets_received >= number_packets) {
image_memory++;
number_packets_received = 0;
received_image++;
memory_output.write(image_memory);
}
}
}

void mobile:: process_mobile() {

while(true) {
wait();
for(int i = 0 ; i < 10 ; i++) {
if(images[i].image_Id == id && sc_time_stamp() == images[i].end_time) {
image_memory--;
memory_output.write(image_memory);
}
}
if(received_image < 5) {
process_receive();
}
if(received_image >= 1) {
process_send();
}
}
}
#endif