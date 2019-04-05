#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/openflow-module.h"
#include "ns3/log.h"
#include "ns3/bridge-helper.h"


#include <iostream>
#include <fstream>
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("OpenFlowCsmaSwitch");

int main(int argc,char *argv[])
{
    
    LogComponentEnable ("OpenFlowCsmaSwitch", LOG_LEVEL_INFO);
    LogComponentEnable ("OpenFlowInterface", LOG_LEVEL_INFO);
    LogComponentEnable ("OpenFlowSwitchNetDevice", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpClient", LOG_LEVEL_ALL);
    LogComponentEnable ("UdpServer", LOG_LEVEL_ALL);
    
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    NS_LOG_INFO ("Create nodes");
    
    NodeContainer terminals;
    terminals.Create(2);
    
    NodeContainer ofSwitch;
    ofSwitch.Create(2);
    
    NS_LOG_INFO ("Create Topology");
    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate",DataRateValue(5000000));
    csma.SetChannelAttribute("Delay",TimeValue(MilliSeconds(2)));
    
    NetDeviceContainer terminalDevices;
    NetDeviceContainer switchDevice1,switchDevice2;
    NetDeviceContainer link;
    
    //Connect ofSwitch1 to ofSwitch2
    link = csma.Install(NodeContainer(ofSwitch.Get(0),ofSwitch.Get(1)));
    
    switchDevice1.Add(link.Get(0));
    switchDevice2.Add(link.Get(1));
    
    //Connect terminal1 and terminal2 to ofSwitch1
   
        link = csma.Install(NodeContainer(terminals.Get(0),ofSwitch.Get(0)));
        terminalDevices.Add(link.Get(0));
        switchDevice1.Add(link.Get(1));
    
    
    //Connect terminal3 and terminal4 to ofSwitch2
 
        link = csma.Install(NodeContainer(terminals.Get(1),ofSwitch.Get(1)));
        terminalDevices.Add(link.Get(0));
        switchDevice2.Add(link.Get(1));
    
    
    //Create the switch netdevice,which will do the packet switching
    Ptr<Node> OFNode1 = ofSwitch.Get(0);
    Ptr<Node> OFNode2 = ofSwitch.Get(1);
    
    OpenFlowSwitchHelper ofswHelper;

    

        //Ptr<ns3::ofi::DropController> controller1 = CreateObject<ns3::ofi::DropController> ();
        //ofswHelper.Install (OFNode1, switchDevice1, controller1);
        //Ptr<ns3::ofi::DropController> controller2 = CreateObject<ns3::ofi::DropController> ();
        //ofswHelper.Install (OFNode2, switchDevice2, controller2);
    
       Ptr<ns3::ofi::LearningController> controller1 = CreateObject<ns3::ofi::LearningController>();
       controller1->SetAttribute("ExpirationTime",TimeValue(Seconds(1)));
       ofswHelper.Install (OFNode1, switchDevice1, controller1);

       Ptr<ns3::ofi::LearningController> controller2 = CreateObject<ns3::ofi::LearningController> ();
       controller2->SetAttribute("ExpirationTime",TimeValue(Seconds(1)));
       ofswHelper.Install (OFNode2, switchDevice2, controller2);


    
    //Add internet stacks to the terminals
    InternetStackHelper internet;
    internet.Install (terminals);
    
    //Add IP addresses
    NS_LOG_INFO ("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interface = ipv4.Assign(terminalDevices);
    
    //Add applications
    NS_LOG_INFO ("Create an application");
    UdpServerHelper echoServer (9);
    ApplicationContainer serverApps = echoServer.Install (terminals.Get(0));
    serverApps.Start (Seconds(0.0));
    serverApps.Stop (Seconds(10.0));
    
    UdpClientHelper echoClient (Ipv4Address("10.1.1.1"),9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (5));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
    ApplicationContainer clientApps = echoClient.Install(terminals.Get(1));
    clientApps.Start (Seconds(0.0));
    clientApps.Stop (Seconds(10.0));
    
    NS_LOG_INFO ("Run simulation");
    Simulator::Run();
    Simulator::Destroy();
    NS_LOG_INFO ("Done.");

    
}
