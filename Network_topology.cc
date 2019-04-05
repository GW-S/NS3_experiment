//
//  Network_topology.c
//  sheng
//
//  Created by sheng on 2018/5/31.
//  这个文件只是搭建网络拓扑

// 目的，根据一个网络搭建网络拓扑

/*
      1,2,3,4,5,6,7,8
      _ _ _ _ _ _ _ _
  1, |1 0 1 0 1 0 1 0
  2, |0 1 1 0 1 0 1 0
  3, |0 1 1 0 1 0 1 0
  4, |0 1 1 0 1 0 1 0
  5, |0 1 0 1 1 0 1 0
  6, |0 1 0 1 0 0 0 0
  7, |1 1 0 1 0 1 1 1
  8, |0 1 1 0 1 0 1 0
 
 是否连通，连通的信道信息，连通的节点信息
 */

//用来生成随即数的文件
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<stdlib.h>
#include<algorithm>
#include<string>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"

using namespace std;
using namespace ns3;

class sheng_node
{
public:
	sheng_node(){
		is_join = 0;// 是否连通；
		name = " ";

		OnOffApplication_PacketSize = 1000;
		OnOffApplication_DataRate =   "1Mbps";

		point_to_point_DataRate = "5Mbps";
		point_to_point_Delay = "2ms";

		client_send_port = 9;
		client_start_random_min = 0;//随机启动应用的时间
		client_end_random_max = 1;//随机启动应用最大化时间
		client_start_time = 0;
		client_end_time = 10;

		server_port = 10;
		PacketSink_start_time = 0;
		PacketSink_end_time = 10;}

	int is_join;// 是否连通；
	string name;

	int OnOffApplication_PacketSize;
	string OnOffApplication_DataRate ;

	string point_to_point_DataRate;
	string point_to_point_Delay ;

	int client_send_port;
	int client_start_random_min ;//随机启动应用的时间
	int client_end_random_max;//随机启动应用最大化时间
	int client_start_time;
	int client_end_time;

	int server_port;
	int PacketSink_start_time;
	int PacketSink_end_time;

};

NS_LOG_COMPONENT_DEFINE ("MixedGlobalRoutingExample");

int main (int argc, char *argv[])
{
    LogComponentEnable ("OnOffApplication", LOG_LEVEL_ALL);
    LogComponentEnable ("PacketSink",       LOG_LEVEL_ALL);
    std::cout<<"fake";
    // 创建矩阵
    int n = 8; // 节点的总数
    vector<vector <class sheng_node> > matrix(n);
    for (int i = 0;i < n;i++){matrix[n].resize(n);}
    std::cout<<"fake";

    for (int i = 0;i < n;i++){
    	for(int j =0;j < n;j++)
    	{
    		//matrix[i][j] = new sheng_node();
    		//matrix[i][j] = new sheng_node();
    		matrix[i][j].is_join = std::rand()%2;
    	}
    }
    
    // 创建节点
    NodeContainer c;
    c.Create (n);
    // 安装网络协议
    InternetStackHelper internet;
    internet.Install (c);
    // 设置网络IP
    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");


    for(int i=0; i<n; i++)
    {
    		for(int j =0;j<n ;j++)
    {
    
            // 直接进行单独的构建好了,这个必须一起构建
            if(matrix[i][j].is_join ==0){
                
              sheng_node this_point = matrix[i][j];
                
                // 得到某一对节点
                NodeContainer now  = NodeContainer(c.Get(i),c.Get(j));
                // 针对其设置信道
                PointToPointHelper p2p;
                p2p.SetDeviceAttribute ("DataRate", StringValue(this_point.point_to_point_DataRate));
                p2p.SetChannelAttribute ("Delay", StringValue (this_point.point_to_point_Delay));
                // 设置网络设施
                NetDeviceContainer now_NetDevice = p2p.Install (now);

                // 随意配置一个信道
                Ipv4InterfaceContainer p2pInterfaces;
                p2pInterfaces = ipv4.Assign(now_NetDevice);
                /*_____________________________________________________________________*/
                // 设置发送端
                uint16_t port = this_point.client_send_port;
                OnOffHelper onoff ("ns3::UdpSocketFactory",
                                   InetSocketAddress (p2pInterfaces.GetAddress(1),port));
                onoff.SetConstantRate (DataRate (this_point.OnOffApplication_DataRate));
                onoff.SetAttribute ("PacketSize", UintegerValue (this_point.OnOffApplication_PacketSize));
                ApplicationContainer apps = onoff.Install (now.Get (0));
                // 随机化发生时间
                Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();
                x->SetAttribute ("Min", DoubleValue (this_point.client_start_random_min));
                x->SetAttribute ("Max", DoubleValue (this_point.client_end_random_max));
                double rn = x->GetValue ();
                // 启动应用
                apps.Start (Seconds (this_point.client_start_time + rn));
                apps.Stop (Seconds (this_point.client_end_time));
                
                /*_____________________________________________________________________*/
                // 设置接收端
                PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), this_point.server_port));
                ApplicationContainer apps_sink = sink.Install (now.Get (1));
                apps_sink.Start (Seconds (this_point.PacketSink_start_time));
                apps_sink.Stop (Seconds (this_point.PacketSink_end_time));

                // 输出中间文件
                // 路径的处理，证明，可以进行路径的设置
                string out_put_file_name = "/home/dcn/eclipse-workspace/NS3/ns-3.20/output/version";
                stringstream stream_i;
                stream_i<<i;
                stringstream stream_j;
                stream_j<<j;
                string this_i =stream_i.str();
                string this_j =stream_j.str();
                out_put_file_name = out_put_file_name + this_i + this_j;
                
                p2p.EnablePcapAll (out_put_file_name);
            }
    }
    }
    std::cout<<"your father shengguowei begin out put file"<<endl;
    std::cout<<"file output ok"<<endl;
    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
    }


