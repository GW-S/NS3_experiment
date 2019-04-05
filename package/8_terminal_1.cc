#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "scratch/construct_file_for_one1.h"
#include "scratch/lib.h"

#include<stdlib.h>
#include<algorithm>
#include<string>
using namespace ns3;
using namespace std;
NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

void control()
{
    //Output_NodeMessage_list(terminal_and_switch_link_table);
    //50s 每s变换一次数据
    int now_time = Simulator::Now().GetSeconds();
    std::vector<int> now_time_list = dataRate_list[now_time];
    int dataRate_list_eachline_len = dataRate_list[0].size();

    //std::cout<<dataRate_list_eachline_len<<std::endl;
    //std::cout<<terminal_To_Switch_PointToPointNetDevice_list.size()<<std::endl;

    for(int i =0; i<dataRate_list_eachline_len; i++)
    {
        int bps = now_time_list[i];//1000
        //bps =83886080000;
        bps = now_time_list[i] * 10000;
    	 //DataRate bps("10Mbps");
        int first_position_number_point_list = get_object_position_in_point_list(i,terminal_and_switch_link_table);
        terminal_To_Switch_PointToPointNetDevice_list[first_position_number_point_list-2]->SetDataRate(bps);
        int second_position_number_point_list =      first_position_number_point_list + 1;
        terminal_To_Switch_PointToPointNetDevice_list[second_position_number_point_list-2]->SetDataRate(bps);
    }

     //
     std::vector<class sender_and_receiver> sender_receiver_list= sender_receiver_matrix[now_time];
     int sender_receiver_number = sender_receiver_list.size();
     //std::cout<<"kehuduan"<<sender_receiver_number<<std::endl;
     for(int sender_receiver_i = 0; sender_receiver_i <sender_receiver_number;sender_receiver_i++)
     {
        sender_and_receiver this_sender_and_receiver = sender_receiver_list[sender_receiver_i];
        //this_sender_and_receiver.output();
        Ptr<UdpClient> this_clientApps = UdpClient_Ptr_point_list[sender_receiver_i];
        this_clientApps->SetAttribute("Interval", TimeValue(Seconds(this_sender_and_receiver.interval)));
        this_clientApps->SetAttribute("PacketSize", UintegerValue(this_sender_and_receiver.packet_size));
        this_clientApps->SetAttribute("MaxPackets", UintegerValue(this_sender_and_receiver.packet_number));
     }
}

int main(int argc,char *argv[])
{
	//set_dataRate_file(1);


   // ---------------------------------------读取文件————————————————————————————————————————————
    std::string switch_and_switch_filePath ="/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/topology.txt";
    std::string terminal_switch_filePath ="/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/appinf_new.txt";
    std::string  traffictrace_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/traffictrace.txt";
    std::string vrch_file_path =	 "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/vrch.txt";
    std::string route_file_path =  "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/route_new.txt";

    std::cout<<"     1"<<std::endl;
    std::vector<class terminal_switch> terminal_switch_list  = get_terminal_switch(terminal_switch_filePath);
    std::cout<<"     1"<<std::endl;
    std::vector<class switch_and_switch> switch_and_switch_list = get_switch_and_switch(switch_and_switch_filePath);
    std::cout<<"     2"<<std::endl;
    std::vector<std::vector<int> > route_list = get_route(route_file_path);
    std::cout<<"     3"<<std::endl;

   LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
   LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);
    // LogComponentEnable("PointToPointChannel",LOG_LEVEL_ALL);
    // LogComponentEnable("PointToPointNetDevice",LOG_LEVEL_ALL);


    // 得到switch的长度
	    std::set<int> terminal_len_set;
	    std::set<int> switch_len_set;
	    for(int i = 0;i < int(terminal_switch_list.size()); i++)
	    {
	        terminal_switch this_terminal_switch = terminal_switch_list[i];
	        terminal_len_set.insert(this_terminal_switch.application_id);
	        switch_len_set.insert(this_terminal_switch.switch_id);
	    }
	    int terminals_len = terminal_len_set.size();
	    int switchs_len = switch_len_set.size();

	    	  // 创建所有终端的点
	    NodeContainer terminals;
	    terminals.Create(terminals_len);
	  // 创建所有的交换机的点
	    NodeContainer Switchs;
	    Switchs.Create(switchs_len);
	  // 创建交换机拓扑的适合的容器
	    NetDeviceContainer link;
	    NetDeviceContainer allDevices;//所有交换机的存储之地；

//  // 创建所有终端的点
//    NodeContainer terminals;
//    terminals.Create(8);
//
//  // 创建所有的交换机的点
//    NodeContainer Switchs;
//    Switchs.Create(8);
//
//  // 创建交换机拓扑的适合的容器
//    NetDeviceContainer link;
//    NetDeviceContainer allDevices;

    std::string switch_dataRate = "1Gbps";


    // Set_Switch_Topology(link,allDevices,Switchs,switch_dataRate,switch_and_switch_link_table);

    Set_switch(switch_and_switch_list,link,allDevices,Switchs,switch_dataRate,switch_and_switch_link_table);


 //   Set_terminals_Topology(link,allDevices,Switchs,terminals,terminal_and_switch_link_table,terminal_To_Switch_PointToPointNetDevice_list);
    Set_terminal(terminal_switch_list,link,allDevices,Switchs,terminals,switch_dataRate,switch_and_switch_link_table);



    // 终端，1号交换机向4号交换机
    // 1 0 2 4
    InternetStackHelper stack;
    stack.Install (Switchs);
    stack.Install (terminals);
    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign (allDevices);


     std::vector<class sender_and_receiver> sender_receiver_list  = sender_receiver_matrix[0];
     int sender_receiver_number = sender_receiver_list.size();

     //std::cout<<sender_receiver_number<<std::endl;

     for(int sender_receiver_i = 0; sender_receiver_i <sender_receiver_number;sender_receiver_i++)
	 {
        sender_and_receiver this_sender_and_receiver = sender_receiver_list[sender_receiver_i];

        int sender_terminal   = this_sender_and_receiver.sender_terminal_id;
        int receiver_terminal = this_sender_and_receiver.receiver_terminal_id;

        std::string IP = Set_static_route_table(sender_terminal,receiver_terminal,terminal_and_switch_link_table, route_list,Switchs);
        //this_sender_and_receiver.output();
        Set_sender_and_receiver(this_sender_and_receiver,route_list,terminals,IP,UdpServer_Ptr_point_list,UdpClient_Ptr_point_list,sender_terminal);
	 }

    // ---------------------------------程序的运行与信息的输出--------------------------------------------------
    //打印路由表
    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    AsciiTraceHelper ascii1;
    Ptr<OutputStreamWrapper> routingStream =ascii1.CreateFileStream ("static-routing.tr");
    ipv4RoutingHelper.PrintRoutingTableAllEvery(Seconds(0.0001),routingStream);
    // 输出IP路径
    AsciiTraceHelper ascii;
    PointToPointHelper pointToPoint;
    pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("static-routing-slash32.tr"));
    pointToPoint.EnablePcapAll ("static-routing-slash32");
    // 栈内信息输出
    //  stack.EnableAsciiIpv4All(ascii.CreateFileStream("8_TERMINAL.tr"));
    //stack.EnablePcapIpv4All("8_terminal");

    NS_LOG_INFO ("Run simulation");
    int data_list_len = dataRate_list.size();
    for(int i=0; i<data_list_len; i++)
    {
       //std::cout<<"长度,理论值50"<<data_list_len<<std::endl;
       Simulator::Schedule(Seconds(i),&control);
    }

    Simulator::Run();
    Simulator::Destroy();

  std::cout<<"over"<<std::endl;
    return 0;
}
