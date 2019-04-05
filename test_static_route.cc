// 通过控制交换机，进而达到在IP层控制路由流向的效果
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-static-routing-helper.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("StaticRoutingSlash32Test");
std::vector<Ptr<PointToPointNetDevice> > point_list;

void control()
{
	std::cout<<std::endl;
	std::cout<<"OK"<<std::endl;
	int point_list_len = point_list.size();
	std::cout<<point_list_len<<std::endl;
	std::cout<<Simulator::Now().GetSeconds()<<std::endl;
	int w = Simulator::Now().GetSeconds();
	std::cout<<w<<std::endl;
	for(int i= 0;i<point_list_len;i++)
	{point_list[i]->SetDataRate(5000);}
}

int test(){

	LogComponentEnable("UdpClient",LOG_LEVEL_ALL);
	LogComponentEnable("UdpServer",LOG_LEVEL_ALL);
	//LogComponentEnable("PointToPointNetDevice",LOG_LEVEL_ALL);

	    NodeContainer terminals;
	    terminals.Create(2);

	    NodeContainer Switchs;
	    Switchs.Create(3);

	    NetDeviceContainer terminalDevices;
	    NetDeviceContainer switchDevices_A;// 交换机的节点
	    NetDeviceContainer switchDevices_B;
	    NetDeviceContainer switchDevices_C;


	    NodeContainer nAnB = NodeContainer(Switchs.Get(0),Switchs.Get(1));
	    NodeContainer nBnC = NodeContainer(Switchs.Get(1),Switchs.Get(2));
	    NodeContainer nAnC = NodeContainer(Switchs.Get(0),Switchs.Get(2));

	    NodeContainer n0nA = NodeContainer(terminals.Get(0),Switchs.Get(0));
	    NodeContainer n1nC = NodeContainer(terminals.Get(1),Switchs.Get(2));



	    PointToPointHelper p2p;
	    p2p.SetDeviceAttribute("DataRate",DataRateValue(50000));//bps
	    NetDeviceContainer dAdB = p2p.Install(nAnB);
	    NetDeviceContainer dBdC = p2p.Install(nBnC);
	    NetDeviceContainer dAdC = p2p.Install(nAnC);
	    NetDeviceContainer d0dA = p2p.Install(n0nA);
	    NetDeviceContainer d1dC = p2p.Install(n1nC);

	    NetDeviceContainer allDevice;

	    allDevice.Add(dAdB);
	    allDevice.Add(dAdC);
	    allDevice.Add(dBdC);
	    allDevice.Add(d0dA);
	    allDevice.Add(d1dC);


	    //Add internet stacks to the terminals
	    InternetStackHelper internet;
	    internet.Install (terminals);
	    internet.Install (Switchs);

	    Ipv4AddressHelper ipv4;
	    ipv4.SetBase ("10.1.1.0", "255.255.255.0");

	    Ipv4InterfaceContainer interface = ipv4.Assign(allDevice);

	    Ptr<Ipv4> ipv4_n1 = terminals.Get(0)->GetObject<Ipv4>();
	    Ptr<Ipv4> ipv4_n2 = terminals.Get(1)->GetObject<Ipv4>();
	    Ptr<Ipv4> ipv4_nA = Switchs.Get(0)->GetObject<Ipv4>();
	    Ptr<Ipv4> ipv4_nB = Switchs.Get(1)->GetObject<Ipv4>();
	    Ptr<Ipv4> ipv4_nC = Switchs.Get(2)->GetObject<Ipv4>();

	    Ipv4StaticRoutingHelper ipv4RoutingHelper;

	    Ptr<Ipv4StaticRouting> staticRouting_nA = ipv4RoutingHelper.GetStaticRouting (ipv4_nA);
	   staticRouting_nA->AddHostRouteTo (Ipv4Address ("10.1.1.9"), Ipv4Address ("10.1.1.2"), 1);
	    Ptr<Ipv4StaticRouting> staticRouting_nB = ipv4RoutingHelper.GetStaticRouting (ipv4_nB);
	     staticRouting_nB->AddHostRouteTo (Ipv4Address ("10.1.1.9"), Ipv4Address ("10.1.1.6"),2);
	   Ptr<Ipv4StaticRouting> staticRouting_nC = ipv4RoutingHelper.GetStaticRouting (ipv4_nC);
	    staticRouting_nC->AddHostRouteTo (Ipv4Address ("10.1.1.9"), Ipv4Address ("10.1.1.9"),3);

	    Ipv4Address now = interface.GetAddress(8);
	    now.Print(std::cout);

	     UdpServerHelper Server(9);
	     ApplicationContainer serverApps = Server.Install(terminals.Get(1));
	     serverApps.Start(Seconds(0));
	     serverApps.Stop(Seconds(100));
	     //must use get
	     UdpClientHelper echoClient(interface.GetAddress(8),9);
	     //   client
	     ApplicationContainer clientApps = echoClient.Install(terminals.Get(0));
	     clientApps.Start(Seconds(0));  //关键是开始时间
	     clientApps.Stop(Seconds(100));
	        AsciiTraceHelper ascii;
	        p2p.EnableAsciiAll (ascii.CreateFileStream ("static-routing-slash32.tr"));
	        p2p.EnablePcapAll ("static-routing-slash32");
	//   Simulator::Schedule(Seconds(1),&control);
	   std::cout<<"sheng"<<std::endl;
	   Simulator::Run ();
	    Simulator::Destroy ();
	    return 0;
}


int
main (int argc, char *argv[])
{
std::cout<<Simulator::Now().GetSeconds()<<std::endl;
test();
std::cout<<"?"<<Simulator::Now().GetSeconds()<<std::endl;
}
