#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "scratch/construct_file_for_one.h"

#include<stdlib.h>
#include<algorithm>
#include<string>
using namespace ns3;
using namespace std;

#include<cstring>
#include <iostream>
#include <fstream>
#include<sstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <set>

std::vector<class NodeMessage>           terminal_and_switch_link_table;                // 前者是terminal,后者是交换机
std::vector<class NodeMessage>           switch_and_switch_link_table;                  // switch，根据switch的不同，同时存两份；
std::vector<Ptr<PointToPointNetDevice> > terminal_To_Switch_PointToPointNetDevice_list; // terminal和Switch 之间网络设备的列表；
std::vector<Ptr<UdpServer> >             UdpServer_Ptr_point_list;                      // 服务器指针
std::vector<Ptr<UdpClient> >             UdpClient_Ptr_point_list;                      // 客户端指针

// 数据率随时间变化的情况，有50行，每列代表一个应用
// 发送者和接受者的信息时刻变化表，有50行，每列代表一个应用
//std::string dataRate_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/origion_app_bandwidths_matrix.txt";
//std::string dataRate_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/suggested_app_bandwidths_matrix.txt";
//std::string dataRate_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/origion_app_bandwidths_matrix_500K.txt";
//std::string dataRate_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/origion_app_bandwidths_matrix_M1.txt";
//std::string dataRate_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/origion_app_bandwidths_matrix_M5.txt";
std::string dataRate_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/origion_app_bandwidths_matrix_M25.txt";



void set_dataRate_file(int type)
{
	if(type == 1)
	{
		dataRate_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/suggested_app_bandwidths_matrix.txt";
	}
	else
	{
		dataRate_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/origion_app_bandwidths_matrix.txt";
	}

}



std::vector<std::vector<int> > dataRate_list = get_dataRate(dataRate_file_path);

std::string  traffictrace_file_path = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/traffictrace.txt";
std::string  vrch_file_path         = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/input_data/vrch.txt";
std::vector<std::vector<class sender_and_receiver> > sender_receiver_matrix= get_sender_and_receiver(traffictrace_file_path,vrch_file_path);





void save_Switch_interface(NetDeviceContainer &allDevices,std::vector<class NodeMessage> &switch_and_switch_link_table,int &first_switch,int &second_switch)
{
     int now_len = allDevices.GetN();

     int now_interface_first_switch =  get_interface_position(first_switch , terminal_and_switch_link_table, switch_and_switch_link_table);
     int now_interface_second_switch = get_interface_position(second_switch, terminal_and_switch_link_table, switch_and_switch_link_table);
     if(now_interface_first_switch == -1){std::cout<<"得到错误的接口"<<std::endl;}
     if(now_interface_second_switch == -1){std::cout<<"得到错误的接口"<<std::endl;}

        NodeMessage insert_NodeMessage =NodeMessage();
        insert_NodeMessage.first_node_id =  first_switch;
        insert_NodeMessage.second_node_id = second_switch;
        insert_NodeMessage.first_interface_number = now_interface_first_switch+1;
        insert_NodeMessage.first_position_number =  now_len-2;
        insert_NodeMessage.first_IP = string_add_int("10.1.1.", insert_NodeMessage.first_position_number+1 );
        insert_NodeMessage.second_interface_number =now_interface_second_switch +1;
        insert_NodeMessage.second_position_number = now_len-1 ;
        insert_NodeMessage.second_IP =string_add_int("10.1.1.",insert_NodeMessage.second_position_number+1);
        switch_and_switch_link_table.push_back(insert_NodeMessage);

        insert_NodeMessage = NodeMessage();
        insert_NodeMessage.first_node_id =   second_switch;
        insert_NodeMessage.second_node_id =first_switch;
        insert_NodeMessage.first_interface_number =now_interface_second_switch +1;
        insert_NodeMessage.second_interface_number = now_interface_first_switch+1;
        insert_NodeMessage.first_position_number = now_len-1;
        insert_NodeMessage.second_position_number = now_len-2;
        insert_NodeMessage.second_IP =string_add_int("10.1.1.",insert_NodeMessage.second_position_number+1 );
        insert_NodeMessage.first_IP =string_add_int("10.1.1.",insert_NodeMessage.first_position_number+1);
        switch_and_switch_link_table.push_back(insert_NodeMessage);
}

void Set_Switch_Topology  (NetDeviceContainer &link,  NetDeviceContainer &allDevices,NodeContainer &Switchs,std::string &switch_dataRate,std::vector<class NodeMessage> &switch_and_switch_link_table)
{
PointToPointHelper pointToPoint;
int first_switch;
int second_switch;

pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(0),Switchs.Get(1)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 0;
second_switch = 1;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);

pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(0),Switchs.Get(2)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 0;
second_switch = 2;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);

pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(0),Switchs.Get(7)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 0;
second_switch = 7;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);

pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(1),Switchs.Get(3)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 1;
second_switch = 3;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);

pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(1),Switchs.Get(7)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 1;
second_switch = 7;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);

pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(2),Switchs.Get(3)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 2;
second_switch = 3;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);

pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(2),Switchs.Get(4)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 2;
second_switch = 4;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);

pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(2),Switchs.Get(6)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 2;
second_switch = 6;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);

pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(2),Switchs.Get(7)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 2;
second_switch = 7;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);

pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(3),Switchs.Get(5)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 3;
second_switch = 5;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);


pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(3),Switchs.Get(6)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 3;
second_switch = 6;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);


pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(3),Switchs.Get(7)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 3;
second_switch = 7;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);


pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(4),Switchs.Get(5)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 4;
second_switch = 5;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);

pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(4),Switchs.Get(6)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 4;
second_switch = 6;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);

pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
link = pointToPoint.Install (NodeContainer(Switchs.Get(5),Switchs.Get(6)));
allDevices.Add(link.Get(0));
allDevices.Add(link.Get(1));
first_switch  = 5;
second_switch = 6;
save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);
//5 5 5 0 0 0 0 5              0：1，2，7
//5 5 0 5 0 0 0 5              1：3，7
//5 0 5 5 5 0 5 5              2：3，4，6，7
//0 5 5 5 0 5 5 5              3：5，6，7
//0 0 5 0 5 5 5 0              4：5，6
//0 0 0 5 5 5 5 0              5：6
//0 0 5 5 5 5 5 0
//5 5 5 5 0 0 0 5
}

void Output_NodeMessage_list(std::vector<class NodeMessage> &link_table)
{
 int table1_len = link_table.size();
 for(int t=0;t<table1_len;t++){link_table[t].output();}
}

void save_terminal_interface(NetDeviceContainer &allDevices,std::vector<class NodeMessage> &switch_and_switch_link_table,std::vector<class NodeMessage> &terminal_and_switch_link_table,int &switch_id,int &terminal_id,int &first_position_number_first_list)
{
    int now_len = allDevices.GetN();
    int now_interface_second_switch = get_interface_position(switch_id,terminal_and_switch_link_table,switch_and_switch_link_table);
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
}

void Set_terminals_Topology(NetDeviceContainer &link,NetDeviceContainer &allDevices,NodeContainer &Switchs,NodeContainer &terminals,std::vector<class NodeMessage> &terminal_and_switch_link_table, std::vector<Ptr<PointToPointNetDevice> > &terminal_To_Switch_PointToPointNetDevice_list)
{
    PointToPointHelper pointToPoint;
    int switch_id;
    int terminal_id;
    int terminal_To_Switch_PointToPointNetDevice_list_len;
    int now_len;

    int queue_len = 0;

  //  int first_position_number_first_list;
   // NetDeviceContainer::Iterator sheng_dev_i;
    Ptr<NetDevice> link_device1,link_device2;
    Ptr<PointToPointNetDevice> p2pDev;


    pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(80000000));//bps
    pointToPoint.SetQueue("ns3::DropTailQueue","MaxPackets",UintegerValue(queue_len));
    link = pointToPoint.Install (NodeContainer(Switchs.Get(0),terminals.Get(0)));
    allDevices.Add(link.Get(1)); //31
    allDevices.Add(link.Get(0)); //32
    switch_id  = 0;
    terminal_id = 0;
   now_len = allDevices.GetN();
   link_device1 = allDevices.Get(now_len-2);
   link_device2 = allDevices.Get(now_len-1);
   p2pDev =link_device1->GetObject<ns3::PointToPointNetDevice>();
   terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
   p2pDev =link_device2->GetObject<ns3::PointToPointNetDevice>();
   terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
    terminal_To_Switch_PointToPointNetDevice_list_len = terminal_To_Switch_PointToPointNetDevice_list.size();
    save_terminal_interface(allDevices,switch_and_switch_link_table,terminal_and_switch_link_table,switch_id,terminal_id,terminal_To_Switch_PointToPointNetDevice_list_len);

    pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(80000000));//bps
    pointToPoint.SetQueue("ns3::DropTailQueue","MaxPackets",UintegerValue(queue_len));
    link = pointToPoint.Install (NodeContainer(Switchs.Get(1),terminals.Get(1)));// 1,0
    allDevices.Add(link.Get(1)); //33
    allDevices.Add(link.Get(0)); //34
    switch_id  = 1;
    terminal_id = 1;

      now_len = allDevices.GetN();
      link_device1 = allDevices.Get(now_len-2);
      link_device2 = allDevices.Get(now_len-1);
      p2pDev =link_device1->GetObject<ns3::PointToPointNetDevice>();
      terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
      p2pDev =link_device2->GetObject<ns3::PointToPointNetDevice>();
      terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
      terminal_To_Switch_PointToPointNetDevice_list_len = terminal_To_Switch_PointToPointNetDevice_list.size();
      save_terminal_interface(allDevices,switch_and_switch_link_table,terminal_and_switch_link_table,switch_id,terminal_id,terminal_To_Switch_PointToPointNetDevice_list_len);

    pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(80000000));//bps
    pointToPoint.SetQueue("ns3::DropTailQueue","MaxPackets",UintegerValue(queue_len));
    link = pointToPoint.Install (NodeContainer(Switchs.Get(2),terminals.Get(2)));
    allDevices.Add(link.Get(1)); //35
    allDevices.Add(link.Get(0)); //36
    switch_id  = 2;
    terminal_id = 2;


          now_len = allDevices.GetN();
          link_device1 = allDevices.Get(now_len-2);
          link_device2 = allDevices.Get(now_len-1);
          p2pDev =link_device1->GetObject<ns3::PointToPointNetDevice>();
          terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
          p2pDev =link_device2->GetObject<ns3::PointToPointNetDevice>();
          terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
          terminal_To_Switch_PointToPointNetDevice_list_len = terminal_To_Switch_PointToPointNetDevice_list.size();
          save_terminal_interface(allDevices,switch_and_switch_link_table,terminal_and_switch_link_table,switch_id,terminal_id,terminal_To_Switch_PointToPointNetDevice_list_len);

    pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(80000000));//bps
    pointToPoint.SetQueue("ns3::DropTailQueue","MaxPackets",UintegerValue(queue_len));
    link = pointToPoint.Install (NodeContainer(Switchs.Get(3),terminals.Get(3)));
    allDevices.Add(link.Get(1)); //37
    allDevices.Add(link.Get(0)); //38
    switch_id  = 3;
    terminal_id = 3;

          now_len = allDevices.GetN();
          link_device1 = allDevices.Get(now_len-2);
          link_device2 = allDevices.Get(now_len-1);
          p2pDev =link_device1->GetObject<ns3::PointToPointNetDevice>();
          terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
          p2pDev =link_device2->GetObject<ns3::PointToPointNetDevice>();
          terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
          terminal_To_Switch_PointToPointNetDevice_list_len = terminal_To_Switch_PointToPointNetDevice_list.size();
          save_terminal_interface(allDevices,switch_and_switch_link_table,terminal_and_switch_link_table,switch_id,terminal_id,terminal_To_Switch_PointToPointNetDevice_list_len);


    pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(80000000));//bps
    pointToPoint.SetQueue("ns3::DropTailQueue","MaxPackets",UintegerValue(queue_len));
    link = pointToPoint.Install (NodeContainer(Switchs.Get(4),terminals.Get(4)));// 4,1
    allDevices.Add(link.Get(1)); //39
    allDevices.Add(link.Get(0)); //40
        switch_id  = 4;
    terminal_id = 4;

          now_len = allDevices.GetN();
          link_device1 = allDevices.Get(now_len-2);
          link_device2 = allDevices.Get(now_len-1);
          p2pDev =link_device1->GetObject<ns3::PointToPointNetDevice>();
          terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
          p2pDev =link_device2->GetObject<ns3::PointToPointNetDevice>();
          terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
          terminal_To_Switch_PointToPointNetDevice_list_len = terminal_To_Switch_PointToPointNetDevice_list.size();
          save_terminal_interface(allDevices,switch_and_switch_link_table,terminal_and_switch_link_table,switch_id,terminal_id,terminal_To_Switch_PointToPointNetDevice_list_len);

    pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(80000000));//bps
    pointToPoint.SetQueue("ns3::DropTailQueue","MaxPackets",UintegerValue(queue_len));
    link = pointToPoint.Install (NodeContainer(Switchs.Get(5),terminals.Get(5)));
    allDevices.Add(link.Get(1)); //41
    allDevices.Add(link.Get(0)); //42
    switch_id  = 5;
    terminal_id = 5;


          now_len = allDevices.GetN();
          link_device1 = allDevices.Get(now_len-2);
          link_device2 = allDevices.Get(now_len-1);
          p2pDev =link_device1->GetObject<ns3::PointToPointNetDevice>();
          terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
          p2pDev =link_device2->GetObject<ns3::PointToPointNetDevice>();
          terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
          terminal_To_Switch_PointToPointNetDevice_list_len = terminal_To_Switch_PointToPointNetDevice_list.size();
          save_terminal_interface(allDevices,switch_and_switch_link_table,terminal_and_switch_link_table,switch_id,terminal_id,terminal_To_Switch_PointToPointNetDevice_list_len);


    pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(80000000));//bps
    pointToPoint.SetQueue("ns3::DropTailQueue","MaxPackets",UintegerValue(queue_len));
    link = pointToPoint.Install (NodeContainer(Switchs.Get(6),terminals.Get(6)));
    allDevices.Add(link.Get(1)); //43
    allDevices.Add(link.Get(0)); //44
    switch_id  = 6;
    terminal_id = 6;


          now_len = allDevices.GetN();
          link_device1 = allDevices.Get(now_len-2);
          link_device2 = allDevices.Get(now_len-1);
          p2pDev =link_device1->GetObject<ns3::PointToPointNetDevice>();
          terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
          p2pDev =link_device2->GetObject<ns3::PointToPointNetDevice>();
          terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
          terminal_To_Switch_PointToPointNetDevice_list_len = terminal_To_Switch_PointToPointNetDevice_list.size();
          save_terminal_interface(allDevices,switch_and_switch_link_table,terminal_and_switch_link_table,switch_id,terminal_id,terminal_To_Switch_PointToPointNetDevice_list_len);

    terminal_To_Switch_PointToPointNetDevice_list_len = terminal_To_Switch_PointToPointNetDevice_list.size();
       save_terminal_interface(allDevices,switch_and_switch_link_table,terminal_and_switch_link_table,switch_id,terminal_id,terminal_To_Switch_PointToPointNetDevice_list_len);

    pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(80000000));//bps
    pointToPoint.SetQueue("ns3::DropTailQueue","MaxPackets",UintegerValue(queue_len));
    link = pointToPoint.Install (NodeContainer(Switchs.Get(7),terminals.Get(7)));
    allDevices.Add(link.Get(1)); //45
    allDevices.Add(link.Get(0)); //46
    switch_id  = 7;
    terminal_id = 7;

          now_len = allDevices.GetN();
          link_device1 = allDevices.Get(now_len-2);
          link_device2 = allDevices.Get(now_len-1);
          p2pDev =link_device1->GetObject<ns3::PointToPointNetDevice>();
          terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
          p2pDev =link_device2->GetObject<ns3::PointToPointNetDevice>();
          terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
          terminal_To_Switch_PointToPointNetDevice_list_len = terminal_To_Switch_PointToPointNetDevice_list.size();
          save_terminal_interface(allDevices,switch_and_switch_link_table,terminal_and_switch_link_table,switch_id,terminal_id,terminal_To_Switch_PointToPointNetDevice_list_len);

    terminal_To_Switch_PointToPointNetDevice_list_len = terminal_To_Switch_PointToPointNetDevice_list.size();
    save_terminal_interface(allDevices,switch_and_switch_link_table,terminal_and_switch_link_table,switch_id,terminal_id,terminal_To_Switch_PointToPointNetDevice_list_len);



}

std::string Set_static_route_table(int sender_terminal,int receiver_terminal,std::vector<class NodeMessage> terminal_and_switch_link_table,std::vector<std::vector<int> > route_list,NodeContainer Switchs)
{


     // 得到接受端的IP
        std::string IP = get_object_IP(receiver_terminal,terminal_and_switch_link_table);
        // 得到终端对应的交换机节点
        int sender_switch = get_object_switch_node(sender_terminal,terminal_and_switch_link_table);
        int receiver_switch =get_object_switch_node(receiver_terminal,terminal_and_switch_link_table);
        //Output_NodeMessage_list(terminal_and_switch_link_table);


 //std::cout<<"IP:"<<IP<<std::endl;
 // std::cout<<"sender_terminal"<<sender_terminal<<std::endl;
 // std::cout<<"receiver_terminal"<<receiver_terminal<<std::endl;
 //std::cout<<sender_switch<<std::endl;
 //std::cout<<receiver_switch<<std::endl;


        std::vector<int> result_route;
        result_route = get_object_route(sender_switch,receiver_switch,route_list);
        int result_route_len = result_route.size();
//for(int w =0;w<result_route_len;w++){std::cout<<result_route[w]<<" ";}std::cout<<std::endl;
//if(result_route_len == 1 && result_route[0]==0)
//{ std::cout<< sender_switch << "和" <<receiver_switch << "无通路"<<std::endl;}



        Ipv4StaticRoutingHelper ipv4RoutingHelper;
        for(int route_i = 0; route_i < result_route_len - 1 ; route_i++)
        {
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
        staticRouting_n1->AddHostRouteTo(Ipv4Address (IP.c_str()), Ipv4Address (next_second_IP.c_str()), next_interface);
//std::cout<<"接口的数量"<<ipv4_now->GetNInterfaces()<<std::endl;
//std::cout<<"对应接口"<<ipv4_now->GetInterfaceForAddress(Ipv4Address(now_switch_link.second_IP.c_str()));
//std::cout<<"last"<<" "<<IP.c_str()<<" "<< next_second_IP.c_str()<<" "<< next_interface<<std::endl;
        return IP;

}

void Set_sender_and_receiver(sender_and_receiver this_sender_and_receiver,std::vector<std::vector<int> > route_list,NodeContainer terminals,std::string IP,std::vector<Ptr<UdpServer> >  &UdpServer_Ptr_point_list, std::vector<Ptr<UdpClient> > &UdpClient_Ptr_point_list,int sender_receiver_i)
{
    // 配置应用和服务器
    NodeContainer sender_receiver_container = NodeContainer(terminals.Get(this_sender_and_receiver.sender_terminal_id),
                                                        terminals.Get(this_sender_and_receiver.receiver_terminal_id));

    // 服务器端的安装
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


// ------ 按照原版的方法重置拓扑的设置------
void Set_switch(std::vector<class switch_and_switch> switch_and_switch_list , NetDeviceContainer &link,  NetDeviceContainer &allDevices,NodeContainer &Switchs,std::string &switch_dataRate,std::vector<class NodeMessage> &switch_and_switch_link_table)
{

    int switch_peer_number = switch_and_switch_list.size();

    for(int switch_i = 0;switch_i < switch_peer_number; switch_i ++)
    {
        switch_and_switch this_switch_peer = switch_and_switch_list[switch_i];
        if(this_switch_peer.switch0_node_id == this_switch_peer.switch1_node_id){continue;}

        int first_switchs      = this_switch_peer.switch0_node_id;
        int second_switchs     = this_switch_peer.switch1_node_id;

        NodeContainer switch_peer  = NodeContainer(Switchs.Get(first_switchs),Switchs.Get(second_switchs));

        PointToPointHelper pointToPoint;
        pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(switch_dataRate));//bps
        link = pointToPoint.Install (NodeContainer(Switchs.Get(first_switchs),Switchs.Get(second_switchs)));
        allDevices.Add(link.Get(0));
        allDevices.Add(link.Get(1));

        int first_switch = first_switchs;
        int second_switch = second_switchs;
        save_Switch_interface(allDevices,switch_and_switch_link_table,first_switch,second_switch);
	}
}

void Set_terminal(std::vector<class terminal_switch> terminal_switch_list, NetDeviceContainer &link,NetDeviceContainer &allDevices,NodeContainer &Switchs,NodeContainer &terminals,std::string &switch_dataRate,std::vector<class NodeMessage> &switch_and_switch_link_table)
{
    int terminal_and_switch_number = terminal_switch_list.size();

    for(int application_i =0;application_i < terminal_and_switch_number;application_i++)
    {
        terminal_switch this_terminal_switch = terminal_switch_list[application_i];

        int  terminal_id =  this_terminal_switch.application_id;
        int  switch_id = this_terminal_switch.switch_id;

        std::cout<<terminal_id<<std::endl;
         std::cout<<switch_id<<std::endl;

        PointToPointHelper pointToPoint;
        pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(80000000));//bps
        pointToPoint.SetQueue("ns3::DropTailQueue","MaxPackets",UintegerValue(250));
        link = pointToPoint.Install (NodeContainer(Switchs.Get(switch_id),terminals.Get(terminal_id)));
        allDevices.Add(link.Get(1)); //41
        allDevices.Add(link.Get(0)); //42

      int  now_len = allDevices.GetN();
      Ptr<NetDevice> link_device1 = allDevices.Get(now_len-2);
      Ptr<NetDevice> link_device2 = allDevices.Get(now_len-1);
      Ptr<PointToPointNetDevice> p2pDev =link_device1->GetObject<ns3::PointToPointNetDevice>();
      terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);
      p2pDev =link_device2->GetObject<ns3::PointToPointNetDevice>();
      terminal_To_Switch_PointToPointNetDevice_list.push_back(p2pDev);

      int terminal_To_Switch_PointToPointNetDevice_list_len = terminal_To_Switch_PointToPointNetDevice_list.size();
      save_terminal_interface(allDevices,switch_and_switch_link_table,terminal_and_switch_link_table,switch_id,terminal_id,terminal_To_Switch_PointToPointNetDevice_list_len);
}
}


