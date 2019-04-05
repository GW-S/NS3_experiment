#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/node.h"
//用来生成随即数的文件
#include<stdlib.h>
#include<algorithm>
#include<string>


#include<
using namespace ns3;
using namespace std;

//NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

// 周期数据
int main(){
    LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);

    NodeContainer p2pNodes;
    p2pNodes.Create (2);
  
    CsmaHelper csma;

//  PointToPointHelper csma;
    
    NetDeviceContainer csma_device;
    csma_device = csma.Install (p2pNodes);

    InternetStackHelper stack;
    stack.Install (p2pNodes);
    
    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign (csma_device);

    //Ptr<CsmaNetDevice> thiswitch =p2pNodes.Get(0)->GetObject<CsmaNetDevice>();
    //std::cout<<"sheng guowei"<< thiswitch<<std::endl;

    Ptr<PointToPointNetDevice> thiswitch =p2pNodes.Get(0)->GetObject<PointToPointNetDevice>();
        std::cout<<"sheng guowei11111"<< thiswitch<<std::endl;

    Ptr<Node>   	me =   p2pNodes.Get(0);
    std::cout<<me->GetNDevices()<<std::endl;
  //std::cout<<me->G

    std::cout<<"hh"<<  me->GetObject<CsmaNetDevice>();

    Ptr<Node> node = p2pNodes.Get(0);
  Ptr<NetDevice> dev = node->GetDevice(0);
  //Ptr<PointToPointNetDevice> csmaDev = dev ->GetObject<PointToPointNetDevice>();

 Ptr<CsmaNetDevice> csmaDev = dev ->GetObject<CsmaNetDevice>();



  std::cout<<"third"<<csmaDev<<std::endl;

    
  //服务器端的安装
    UdpServerHelper Server (9);
    ApplicationContainer serverApps = Server.Install (p2pNodes.Get(1));
    serverApps.Start (Seconds (0.0));
    serverApps.Stop (Seconds (10.0));
    
    UdpClientHelper echoClient (p2pInterfaces.GetAddress (1), 9);
   //创建了相应的application
    ApplicationContainer clientApps = echoClient.Install(p2pNodes.Get (0));
    
   //这个转化问题始终没有解决
    clientApps.Start (Seconds(0));  //关键是开始时间
    clientApps.Stop (Seconds(10));

    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}
