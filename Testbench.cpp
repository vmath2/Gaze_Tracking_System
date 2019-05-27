#include<systemc.h>
#include<Library.h>
#include<Server.h>
#include<Mobile.h>

SC_MODULE(gaze_monitor) {

//Inputs
sc_signal<bool> clock;
sc_signal<sc_uint<1> > NAR_for_mobile1 , PI_for_mobile1 , NAR_for_mobile2 , PI_for_mobile2;
sc_signal<int> ROI_input_1 , start_time_for_ROI1 , end_time_for_ROI1 , ROI_input_2 , start_time_for_ROI2 , end_time_for_ROI2;
// Outputs
sc_signal<sc_uint<1> > NS_to_mobiles , ACK_server_to_mobile1 , ACK_server_to_mobile2 , PI_to_mobile1 , PI_to_mobile2;
sc_signal<int> memory_output1 , memory_output2;
sc_signal<bool> check_mobSending1 , check_mobSending2 , server_flag;
server* ser;
mobile* mob1;
mobile* mob2;
void clock_signal();
SC_CTOR(gaze_monitor){
double DELTA =0.00009;
double GAMMA =0.000009;
double BANDWIDTH= 10000000;
double PACKET_SIZE =1000000;
ser = new server ("Server" , DELTA , GAMMA , BANDWIDTH , PACKET_SIZE);
mob1 = new mobile ("Mobile_1" , 1 , PACKET_SIZE , BANDWIDTH);
mob2 = new mobile ("Mobile_2" , 2 , PACKET_SIZE , BANDWIDTH);
ser->server_flag(server_flag);
//Server to Mobile		
ser->NS_to_mobiles(NS_to_mobiles);
mob1->server_input1(NS_to_mobiles);
mob2->server_input1(NS_to_mobiles);
ser->ACK_server_to_mobile1(ACK_server_to_mobile1);
mob1->server_input2(ACK_server_to_mobile1);
ser->ACK_server_to_mobile2(ACK_server_to_mobile2);
mob2->server_input2(ACK_server_to_mobile2);
ser->PI_to_mobile1(PI_to_mobile1);
mob1->server_input3(PI_to_mobile1);
ser->PI_to_mobile2(PI_to_mobile2);
mob2->server_input3(PI_to_mobile2);
mob1->check_mobSending(check_mobSending1);
mob2->check_mobSending(check_mobSending2);
//Mobile to Server
mob1->memory_output(memory_output1);
mob1->mobile_output1(NAR_for_mobile1);
ser->NAR_for_mobile1(NAR_for_mobile1);
mob1->mobile_output2(PI_for_mobile1);
ser->PI_for_mobile1(PI_for_mobile1);
mob1->roi_output(ROI_input_1);
ser->ROI_input_1(ROI_input_1);
mob1->start_output(start_time_for_ROI1);
ser->start_time_for_ROI1(start_time_for_ROI1);
mob1->end_output(end_time_for_ROI1);
ser->end_time_for_ROI1(end_time_for_ROI1);
mob2->memory_output(memory_output2);
mob2->mobile_output1(NAR_for_mobile2);
ser->NAR_for_mobile2(NAR_for_mobile2);
mob2->mobile_output2(PI_for_mobile2);
ser->PI_for_mobile2(PI_for_mobile2);
mob2->roi_output(ROI_input_2);
ser->ROI_input_2(ROI_input_2);
mob2->start_output(start_time_for_ROI2);
ser->start_time_for_ROI2(start_time_for_ROI2);
mob2->end_output(end_time_for_ROI2);
ser->end_time_for_ROI2(end_time_for_ROI2);
//Clock to Mobile and Server
ser->clock(clock);
mob1->clock(clock);
mob2->clock(clock);
SC_THREAD(clock_signal);
}

};
void gaze_monitor::clock_signal() {
while (true){
wait(0.5 , SC_MS);
clock = false;
wait(0.5 , SC_MS);
clock = true;
}
}

int sc_main(int argc , char* argv[]) {
gaze_monitor* gaze = new gaze_monitor("Gaze Monitor");
sc_trace_file* VCDFile;
VCDFile = sc_create_vcd_trace_file("Gaze Simulation");
cout<<"@ "<<sc_time_stamp()<<"~~~~~~~~~~~~VCD File Created~~~~~~~~~~~~"<<endl;
sc_trace(VCDFile, gaze->clock, "Clock");
sc_trace(VCDFile, gaze->NS_to_mobiles, "Network_Status");
sc_trace(VCDFile, gaze->server_flag, "Server_is_sending_image_to_mobile");
sc_trace(VCDFile, gaze->check_mobSending1, "Mobile_1_is_sending_image_to_server");
sc_trace(VCDFile, gaze->check_mobSending2, "Mobile_2_is_sending_image_to_server");
sc_trace(VCDFile, gaze->memory_output1, "Number_of_images_in_Mobile_1");
sc_trace(VCDFile, gaze->memory_output2, "Number_of_images_in_Mobile_2");
sc_start(250000 , SC_MS);
return 0;
}