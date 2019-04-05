// author:sheng.GW
// time:2018.8.1
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/openflow-module.h"
#include "ns3/log.h"
#include "ns3/bridge-helper.h"
#include "ns3/bridge-module.h"
#include "ns3/address.h"
#include "ns3/mac48-address.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include <map>
#include <iostream>
#include <string>

#include "scratch/construct_file_for_one.h"
using namespace ns3;


NS_LOG_COMPONENT_DEFINE("ShengSimulator");

std::vector<Ptr<PointToPointNetDevice> > point_list;

std::string dataRate_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/suggested_app_bandwidths_matrix.txt";
// std::string dataRate_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/suggested_app_bandwidths_matrix.txt";

std::vector<std::vector<int> > dataRate_list = get_dataRate(dataRate_file_path);
int dataRate_list_len = dataRate_list.size();
std::vector<class NodeMessage>  terminal_and_switch_link_table; // 前者是terminal,后者是交换机
std::vector<class NodeMessage>  switch_and_switch_link_table;

// 服务器指针
std::vector<Ptr<UdpServer> > UdpServer_Ptr_point_list;
// 客户端指针
std::vector<Ptr<UdpClient> >  UdpClient_Ptr_point_list;

std::string  traffictrace_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/traffictrace.txt";
std::string vrch_file_path          = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/vrch.txt";
std::vector<std::vector<class sender_and_receiver> > sender_receiver_matrix = get_sender_and_receiver(traffictrace_file_path,vrch_file_path);




// 自动得到数据的值的设计思想：
        // 1.维持一个指针列表，指针指向更改的值，
        // 2.维持一个矩阵列表，列表里有要改的值
        // 3.对这个指针的值设置；
// 先得到所有的值，开始时间和结束时间，0-1s 的时间内，50 * 40 * 多播数量* 8
// 然后依次创建所有的应用
// 现在先取得0-1S内的值
// 将其添加到应用中
// 对其值进行动态修改
// 按照秒数对其值进行存储


// 确实是竞争信道
void control()
{
    //50s 每s变换一次数据
   // std::cout<<"现在的时间"<<Simulator::Now().GetSeconds()<<std::endl;
    int now_time = Simulator::Now().GetSeconds();

    int dataRate_list_eachline_len = dataRate_list[0].size();
    std::vector<int> now_time_list = dataRate_list[now_time];//现在的轴；
    for(int i =0; i<dataRate_list_eachline_len; i++)
    {
        //int bps = now_time_list[i];//1000
        // int bps =83886080000;
    	DataRate bps("10Mbps");
    // 找到对应的NodeMessage
        int first_positon_number_point_list = get_object_position_in_point_list(i,terminal_and_switch_link_table);
        point_list[first_positon_number_point_list]->SetDataRate(bps);
    }
   // 更改客户端发送规律
    std::vector<class sender_and_receiver>  sender_receiver_list= sender_receiver_matrix[now_time];
    int  sender_receiver_list_len = sender_receiver_list.size();
    for(int i=0; i< sender_receiver_list_len; i++)
    {
           sender_and_receiver this_sender_and_receiver = sender_receiver_list[i];
           Ptr<UdpClient> this_clientApps = UdpClient_Ptr_point_list[i];
           this_clientApps->SetAttribute("Interval", TimeValue(Seconds(this_sender_and_receiver.interval)));
           this_clientApps->SetAttribute("PacketSize", UintegerValue(this_sender_and_receiver.packet_size));
           this_clientApps->SetAttribute("MaxPackets", UintegerValue(this_sender_and_receiver.packet_number * (now_time + 1)));
           this_clientApps->SetAttribute("ShengApplicationNumber",UintegerValue(this_sender_and_receiver.sender_terminal_id+now_time));
    }
}


void each_step(int sheng_ok)
{
	  // ---------------------------------------读取文件————————————————————————————————————————————
	    std::string switch_and_switch_filePath ="/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/topology.txt";
	    std::string terminal_switch_filePath ="/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/appinf_new.txt";

	    std::string route_file_path =  "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/route_new.txt";

	    std::vector<class terminal_switch> terminal_switch_list  = get_terminal_switch(terminal_switch_filePath);
	    std::vector<class switch_and_switch> switch_and_switch_list = get_switch_and_switch(switch_and_switch_filePath);
	    std::vector<std::vector<int> > route_list = get_route(route_file_path);

	  // ---------------------------------------得到交换机的数量长度————————————————————————————————————————————
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
	  // ---------------------------------------进入NS3的世界————————————————————————————————————————————
	   LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
	   LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);
	// LogComponentEnable ("CsmaHelper",LOG_LEVEL_ALL);

//LogComponentEnable ("UdpClient", LOG_LEVEL_ALL);
//LogComponentEnable ("UdpServer", LOG_LEVEL_ALL);
//LogComponentEnable ("PointToPointChannel", LOG_LEVEL_ALL);
//LogComponentEnable ("PointToPointNetDevice", LOG_LEVEL_ALL);
	  // 创建所有终端的点
	    NodeContainer terminals;
	    terminals.Create(terminals_len);
	  // 创建所有的交换机的点
	    NodeContainer Switchs;
	    Switchs.Create(switchs_len);
	  // 创建交换机拓扑的适合的容器
	    NetDeviceContainer link;
	    NetDeviceContainer allDevices;//所有交换机的存储之地；


	  // ---------------------------------------针对交换机而言————————————————————————————————————————————
	    int switch_peer_number = switch_and_switch_list.size();
	    for(int switch_i = 0;switch_i < switch_peer_number;switch_i ++)
	    {
	        switch_and_switch this_switch_peer = switch_and_switch_list[switch_i];
	        if(this_switch_peer.switch0_node_id == this_switch_peer.switch1_node_id){continue;}
	        int first_switchs      = this_switch_peer.switch0_node_id;
	        int second_switchs = this_switch_peer.switch1_node_id;
	        NodeContainer switch_peer  = NodeContainer(Switchs.Get(first_switchs),Switchs.Get(second_switchs));

	        PointToPointHelper csma;
	        //csma.SetChannelAttribute("DataRate",DataRateValue(this_switch_peer.switch_Bandwidth));
	        csma.SetDeviceAttribute("DataRate",DataRateValue(DataRate("100Gbps")));
			//csma.SetDeviceAttribute("DataRate");
	        link = csma.Install(switch_peer);

	        allDevices.Add(link.Get(0));
	        allDevices.Add(link.Get(1));

//int all_devices = allDevices.GetN();
//for(int zz=all_devices-2;zz!=all_devices;zz++)
//{
//  std::cout<< allDevices.Get(zz)->GetAddress()<<std::endl;
//  std::cout<< allDevices.Get(zz)->GetNode()->GetId()<<std::endl;
//
// std::cout<<first_switchs<<std::endl;
// std::cout<<second_switchs<<std::endl;
//}
// std::cout<<"fake"<<std::endl;



// int table1_len = terminal_and_switch_link_table.size();
// int table2_len = switch_and_switch_link_table.size();
// for(int t=0;t<table1_len;t++){terminal_and_switch_link_table[t].output();}
// for(int t=0;t<table2_len;t++){switch_and_switch_link_table[t].output();}
	        int now_len = allDevices.GetN();
//std::cout<<now_len<<std::endl;
	        int now_interface_first_switch =  get_interface_position(first_switchs,terminal_and_switch_link_table,switch_and_switch_link_table);
	        int now_interface_second_switch = get_interface_position(second_switchs,terminal_and_switch_link_table,switch_and_switch_link_table);
	        if(now_interface_first_switch == -1) {std::cout<<"得到错误的接口"<<std::endl;}
	        if(now_interface_second_switch == -1){std::cout<<"得到错误的接口"<<std::endl;}



            // 建立一个连接好的NodeMessage
	        NodeMessage insert_NodeMessage =NodeMessage();
	        insert_NodeMessage.first_node_id =  first_switchs;
	        insert_NodeMessage.second_node_id = second_switchs;
	        insert_NodeMessage.first_interface_number = now_interface_first_switch+1;
	        insert_NodeMessage.first_position_number =  now_len-2;
	        insert_NodeMessage.first_IP = string_add_int("10.1.1.", insert_NodeMessage.first_position_number+1 );
	        insert_NodeMessage.second_interface_number =now_interface_second_switch +1;
	        insert_NodeMessage.second_position_number = now_len-1 ;
	        insert_NodeMessage.second_IP =string_add_int("10.1.1.",insert_NodeMessage.second_position_number+1);
	        switch_and_switch_link_table.push_back(insert_NodeMessage);

	        insert_NodeMessage = NodeMessage();
	        insert_NodeMessage.first_node_id =   second_switchs;
	        insert_NodeMessage.second_node_id =first_switchs;
	        insert_NodeMessage.first_interface_number =now_interface_second_switch +1;
	        insert_NodeMessage.second_interface_number = now_interface_first_switch+1;
	        insert_NodeMessage.first_position_number = now_len-1;
	        insert_NodeMessage.second_position_number = now_len-2;
	        insert_NodeMessage.second_IP =string_add_int("10.1.1.",insert_NodeMessage.second_position_number+1 );
	        insert_NodeMessage.first_IP =string_add_int("10.1.1.",insert_NodeMessage.first_position_number+1);
	        switch_and_switch_link_table.push_back(insert_NodeMessage);


AsciiTraceHelper ascii;
csma.EnableAsciiAll (ascii.CreateFileStream ("switch_switch.tr"));
csma.EnablePcapAll ("switch_switch");
	    }

	    // ---------------------------------------针对交换机与终端而言————————————————————————————————————————————
	    int terminal_and_switch_number = terminal_switch_list.size();
	    for(int application_i =0;application_i < terminal_and_switch_number;application_i++)
	    {
	        terminal_switch this_terminal_switch = terminal_switch_list[application_i];
	        //this_terminal_switch.output();

	        int  terminal_id =  this_terminal_switch.application_id;
	        int  switch_id =    this_terminal_switch.switch_id;

	        PointToPointHelper csma;
	       //csma.SetDeviceAttribute("DataRate",DataRateValue(DataRate("100Gbps")));
	        csma.SetQueue("ns3::DropTailQueue","MaxPackets",UintegerValue(1000));
	        link = csma.Install(NodeContainer(terminals.Get(terminal_id), Switchs.Get(switch_id)));
	        allDevices.Add(link.Get(0));
	        allDevices.Add(link.Get(1));

//int all_devices = allDevices.GetN();
//for(int zz=all_devices-2;zz!=all_devices;zz++)
//{
//std::cout<< allDevices.Get(zz)->GetAddress()<<std::endl;
//std::cout<< allDevices.Get(zz)->GetNode()->GetId()<<std::endl;
//
//std::cout<<terminal_id<<std::endl;
//std::cout<<switch_id<<std::endl;
//}
//std::cout<<"fake"<<std::endl;


            // 问题1：这里这样push_back进去真的合适吗？
            // 这里的设计思想又是怎样的
               int first_position_number_first_list;
	        NetDeviceContainer::Iterator sheng_dev_i;
	        for(sheng_dev_i = allDevices.Begin();sheng_dev_i!=allDevices.End();++sheng_dev_i)
	        {
                first_position_number_first_list = point_list.size();
                Ptr<PointToPointNetDevice> p2pDev = (*sheng_dev_i)->GetObject<PointToPointNetDevice>();
                point_list.push_back(p2pDev);
	        }
	        int now_len = allDevices.GetN();
/*
	 int table1_len = terminal_and_switch_link_table.size();
	 int table2_len = switch_and_switch_link_table.size();
	 for(int t=0;t<table1_len;t++){terminal_and_switch_link_table[t].output();}
	 for(int t=0;t<table2_len;t++){switch_and_switch_link_table[t].output();}
*/
	        //int now_interface_first_terminal = get_interface_position(terminal_id,terminal_and_switch_link_table,switch_and_switch_link_table);
	        int now_interface_second_switch = get_interface_position(switch_id,terminal_and_switch_link_table,switch_and_switch_link_table);
	        // if(now_interface_first_terminal == -1){std::cout<<"得到错误的接口"<<std::endl;}
	        if(now_interface_second_switch == -1){std::cout<<"得到错误的接口"<<std::endl;}
	        NodeMessage insert_NodeMessage =NodeMessage();
	        insert_NodeMessage.first_node_id =  terminal_id;
	        insert_NodeMessage.second_node_id = switch_id;
	        insert_NodeMessage.first_interface_number = 1;
	        insert_NodeMessage.second_interface_number = now_interface_second_switch + 1;
	        insert_NodeMessage.first_position_number = now_len-2;
	        insert_NodeMessage.second_position_number= now_len-1;
	        insert_NodeMessage.first_IP =string_add_int("10.1.1.",insert_NodeMessage.first_position_number+1);
	        insert_NodeMessage.second_IP =string_add_int("10.1.1.",insert_NodeMessage.second_position_number+1);
               insert_NodeMessage.first_position_number_point_list = first_position_number_first_list;
	        terminal_and_switch_link_table.push_back(insert_NodeMessage);

AsciiTraceHelper ascii;
csma.EnableAsciiAll (ascii.CreateFileStream ("terminal_switch.tr"));
csma.EnablePcapAll ("terminal_switch");
	    }
	    // ---------------------------------------网络协议的配置————————————————————————————————————————————
	    InternetStackHelper internet;
	    internet.Install (terminals);
	    internet.Install (Switchs  );
	    Ipv4AddressHelper ipv4;
	    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
	    Ipv4InterfaceContainer interface = ipv4.Assign(allDevices);

////得到相应的地址
//   int DeviceNumber = interface.GetN();
//	std::cout<<"尝试输出所有的地址"<<std::endl;
//   for(int i = 0;i<DeviceNumber;i++)
//   {
//		Ipv4Address now = interface.GetAddress(i);
//		now.Print(std::cout);
//   std::cout<<"now"<<std::endl;
//   }

	    // ---------------------------------应用与应用之间的配置--------------------------------------------------

        std::vector<class sender_and_receiver> sender_receiver_list  = sender_receiver_matrix[0];

        int sender_receiver_number = sender_receiver_list.size();

         for(int sender_receiver_i = 0; sender_receiver_i <sender_receiver_number;sender_receiver_i++)
	    {
	        sender_and_receiver this_sender_and_receiver = sender_receiver_list[sender_receiver_i];
//this_sender_and_receiver.output();
	        int sender_terminal   = this_sender_and_receiver.sender_terminal_id;
	        int receiver_terminal = this_sender_and_receiver.receiver_terminal_id;
//std::cout<<"fake_sheng"<<std::endl;
//int test_len = terminal_and_switch_link_table.size();
//for(int i=0;i<test_len;i++){terminal_and_switch_link_table[i].output();}
//std::cout<<"fake_sheng"<<std::endl;
//test_len = switch_and_switch_link_table.size();
//for(int i=0;i<test_len;i++){switch_and_switch_link_table[i].output();}
	     // 得到接受端的IP
	         std::string IP = get_object_IP(receiver_terminal,terminal_and_switch_link_table);
	     // 得到终端对应的交换机节点
	         int sender_switch = get_object_switch_node(sender_terminal,terminal_and_switch_link_table);
	         int receiver_switch =get_object_switch_node(receiver_terminal,terminal_and_switch_link_table);
// std::cout<<"IP:"<<IP<<std::endl;
// std::cout<<"sender_terminal"<<sender_terminal<<std::endl;
// std::cout<<"receiver_terminal"<<receiver_terminal<<std::endl;
// std::cout<<sender_switch<<std::endl;
// std::cout<<receiver_switch<<std::endl;
	        std::vector<int> result_route;
	        result_route = get_object_route(sender_switch,receiver_switch,route_list);
	        int result_route_len = result_route.size();
//for(int w =0;w<result_route_len;w++){std::cout<<result_route[w]<<" ";}std::cout<<std::endl;
//if(result_route_len == 1 && result_route[0]==0)
//{ std::cout<< sender_switch << "和" <<receiver_switch << "无通路"<<std::endl;}
	        Ipv4StaticRoutingHelper ipv4RoutingHelper;

       // 交换机前几条
	        for(int route_i = 0; route_i < result_route_len - 1 ; route_i++)
	        {
	            // 得到下一跳
	            int first = result_route[route_i];
	            int second = result_route[route_i+1];
	            NodeMessage now_switch_link= get_node_message(first,second,switch_and_switch_link_table);

	            int switch_id = now_switch_link.first_node_id;
	            int next_interface = now_switch_link.first_interface_number;
	            std::string next_second_IP = now_switch_link.second_IP;

//std::cout<<"switch_number"<<switch_id<<std::endl;

	            Ptr<Ipv4> ipv4_now = Switchs.Get(switch_id)->GetObject<Ipv4>();
	            Ptr<Ipv4StaticRouting> staticRouting_n1 = ipv4RoutingHelper.GetStaticRouting (ipv4_now);

//std::cout<<"接口的数量"<<ipv4_now->GetNInterfaces()<<std::endl;
//std::cout<<"对应接口"<<ipv4_now->GetInterfaceForAddress(Ipv4Address(now_switch_link.first_IP.c_str()));
//std::cout<<"last"<<" "<<IP.c_str()<<" "<< next_second_IP.c_str()<<" "<< next_interface<<std::endl;

	            staticRouting_n1->AddHostRouteTo(Ipv4Address (IP.c_str()), Ipv4Address (next_second_IP.c_str()), next_interface);
	        }

	        NodeMessage now_switch_link= get_node_message(receiver_terminal,receiver_switch,terminal_and_switch_link_table);
	        int switch_id = now_switch_link.second_node_id;
	        int next_interface = now_switch_link.second_interface_number;
	        std::string next_second_IP = now_switch_link.first_IP;

//std::cout<<"terminal_number"<<switch_id<<std::endl;

	        Ptr<Ipv4> ipv4_now = Switchs.Get(switch_id)->GetObject<Ipv4>();
	        Ptr<Ipv4StaticRouting> staticRouting_n1 = ipv4RoutingHelper.GetStaticRouting (ipv4_now);

//std::cout<<"接口的数量"<<ipv4_now->GetNInterfaces()<<std::endl;
//std::cout<<"对应接口"<<ipv4_now->GetInterfaceForAddress(Ipv4Address(now_switch_link.second_IP.c_str()));
//std::cout<<"last"<<" "<<IP.c_str()<<" "<< next_second_IP.c_str()<<" "<< next_interface<<std::endl;
	        staticRouting_n1->AddHostRouteTo(Ipv4Address (IP.c_str()), Ipv4Address (next_second_IP.c_str()), next_interface);


	        // 配置应用和服务器
	        NodeContainer sender_receiver_container = NodeContainer(terminals.Get(this_sender_and_receiver.sender_terminal_id),
	                                                                terminals.Get(this_sender_and_receiver.receiver_terminal_id));


     	       //服务器端的安装
	           UdpServerHelper Server(this_sender_and_receiver.receiver_port);
	           ApplicationContainer serverApps = Server.Install(terminals.Get(this_sender_and_receiver.receiver_terminal_id));
	           Ptr<Application> serverApp_temporary = serverApps.Get(0);
	           Ptr<UdpServer> this_serverApps = serverApp_temporary->GetObject<UdpServer>();
	           this_serverApps->SetAttribute("ShengPeerId",UintegerValue(sender_receiver_i));
               this_serverApps->SetStartTime(Seconds(this_sender_and_receiver.receiver_start_time));
               this_serverApps->SetStopTime(Seconds(this_sender_and_receiver.receiver_end_time));
               //客户端的安装
	           UdpClientHelper echoClient(Ipv4Address (IP.c_str()),this_sender_and_receiver.receiver_port);
               ApplicationContainer clientApps = echoClient.Install(terminals.Get(this_sender_and_receiver.sender_terminal_id));
	           Ptr<Application> clientApp_temporary = clientApps.Get(0);
	           Ptr<UdpClient> this_clientApps = clientApp_temporary->GetObject<UdpClient>();

	      this_sender_and_receiver.output();
	           this_clientApps->SetAttribute("Interval", TimeValue(Seconds(this_sender_and_receiver.interval)));
	           this_clientApps->SetAttribute("PacketSize", UintegerValue(this_sender_and_receiver.packet_size));
                  this_clientApps->SetAttribute("MaxPackets", UintegerValue(this_sender_and_receiver.packet_number * 50));
	           this_clientApps->SetAttribute("ShengApplicationNumber",UintegerValue(this_sender_and_receiver.sender_terminal_id));
	           this_clientApps->SetStartTime(Seconds(this_sender_and_receiver.sender_start_time));
                  this_clientApps->SetStopTime(Seconds(this_sender_and_receiver.sender_end_time));


               //服务器端指针的保存
                UdpServer_Ptr_point_list.push_back(this_serverApps);
               //客户端指针的保存
                UdpClient_Ptr_point_list.push_back(this_clientApps);



	        }

	    // ---------------------------------程序的运行与信息的输出--------------------------------------------------
	    NS_LOG_INFO ("Run simulation");

        for(int i=0; i<dataRate_list_len; i++)
        {
        	std::cout<<"长度,理论值50"<<dataRate_list_len<<std::endl;
        	Simulator::Schedule(Seconds(i),&control);
        }

        Simulator::Run();
        Simulator::Destroy();
}

int main(int argc,char *argv[])
{
each_step(1);
return 0;
}































