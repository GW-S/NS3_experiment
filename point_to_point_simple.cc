// 一个单纯的点对点，中间加一个Switch的链路
// 对其进行观察，看是否发送成功
// terminal -- switch --- terminal
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include<stdlib.h>
#include<algorithm>
#include<string>
using namespace ns3;
using namespace std;
NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int main(int argc,char *argv[])
{
    LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);
LogComponentEnable ("UdpClient", LOG_LEVEL_ALL);
LogComponentEnable ("UdpServer", LOG_LEVEL_ALL);
    // 创建所有终端的点
    NodeContainer terminals;
    terminals.Create(2);
    // 创建所有的交换机的点
    NodeContainer Switchs;
    Switchs.Create(1);
    // 创建交换机拓扑的适合的容器
    NetDeviceContainer link;
    NetDeviceContainer allDevices;
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(50000));//bps
    link = pointToPoint.Install (NodeContainer(Switchs.Get(0),terminals.Get(0)));
    allDevices.Add(link.Get(0));
    allDevices.Add(link.Get(1));

    pointToPoint.SetDeviceAttribute("DataRate",DataRateValue(50000));//bps
    link = pointToPoint.Install (NodeContainer(Switchs.Get(0),terminals.Get(1)));
    allDevices.Add(link.Get(0));
    allDevices.Add(link.Get(1));

    InternetStackHelper stack;
    stack.Install (terminals);
    stack.Install (Switchs);

    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign (allDevices);

  //服务器端的安装
    UdpServerHelper Server(9);
    ApplicationContainer serverApps = Server.Install(terminals.Get(1));
    serverApps.Start(Seconds(0.0));
    serverApps.Stop(Seconds(100.0));
    UdpClientHelper echoClient(Ipv4Address ("10.1.1.4"),9);

       echoClient.SetAttribute("Interval", TimeValue(Seconds(0.001)));
	echoClient.SetAttribute("PacketSize", UintegerValue(1024));
	echoClient.SetAttribute("MaxPackets", UintegerValue(1000));
echoClient.SetAttribute("MaxPackets", UintegerValue(1));

	//创建了相应的application
	ApplicationContainer clientApps = echoClient.Install(terminals.Get(0));
	clientApps.Start(Seconds(0.0));
	clientApps.Stop(Seconds(100.0));

    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}



