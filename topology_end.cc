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
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//用来生成随即数的文件
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <set>

using namespace ns3;

// 数据结构的构造
// 刷新Switch之间的关系类,用来对交换机之间的连接进行控制
class switch_and_switch
{
public:
    switch_and_switch() {
        switch0_node_id = 0;
        //switch0_queue_maxPackets = 1000;
        switch1_node_id = 0;
        //switch1_queue_maxPackets = 1000;
        switch_Bandwidth = "1Mbps";
        switch_delay = "2ms";
        queue_maxpacket= 1000;
        interframeGap = 0.01;
    }
    
    void output() {
        std::cout<<"switch_and_switch";
        std::cout<<switch0_node_id <<"+"<<switch1_node_id<<"+"<<switch_Bandwidth<<"+"<<switch_delay<<"+"<<queue_maxpacket <<"+" <<interframeGap<<std::endl;
    }
    
    
    // 交换机1
    int switch0_node_id;
    //int switch0_queue_maxPackets;
    
    // 交换机2
    int switch1_node_id;
    //int switch1_queue_maxPackets;
    
    // 链路与链路之间的连接
    std::string switch_Bandwidth;
    std::string switch_delay;
    int queue_maxpacket;
    int interframeGap;
    
    
    /*
     * 交换机A 编号
     * 交换机A 最长帧队列
     *
     * 交换机B 编号
     * 交换机B 最长帧队列
     *
     * 交换机链路设置
     *  1.带宽，2.延迟，3.最大包，4.帧平整
     */
    
};

class terminal_switch
{
public:
    terminal_switch()
    {
        application_id = 0;
        application_port = 0;
        
        switch_port = 0;
        switch_id = 0;
        
        DropTailQueue_MaxPackets = 1000;
        
        point_to_point_DataRate = "1Mbps";
        point_to_point_Delay = "2ms";
        
        InterframeGap = 0.01;
    }
    
    void output() {
        std::cout<<"terminal_switch";
        std::cout<<application_id<<"+"<<application_port<<"+"<<switch_port<<"+"<<switch_id<<"+"<< point_to_point_DataRate<<"+"<<point_to_point_Delay<<"+"<<InterframeGap<<"+"<<DropTailQueue_MaxPackets<<std::endl;
    }
    
    int application_id;
    int application_port;   // 连接端口
    
    
    int switch_id;
    int switch_port;
    
    // 延迟和数据传输速率
    std::string point_to_point_DataRate;
    std::string point_to_point_Delay;
    
    // 帧平整，队列
    int InterframeGap;
    int DropTailQueue_MaxPackets;
    
    // std::string InterframeGap = "100000000ns";
    //int object_switch_order_number;   // 目标交换机的序号；
};

class sender_and_receiver
{
public:
    sender_and_receiver() {

        sender_id = 0;
        sender_port = 9;
        sender_start_time = 0.0;
        sender_end_time = 10.0;

        receiver_id = 0;
        receiver_port = 0;
        receiver_start_time = 0.0;
        receiver_end_time = 10.0;

        DataRate = "1Mbps";
        PacketSize = 1024;
        interval = 1.0;
        MaxPackets = 5;


        client_start_random_min = 0;//随机启动应用的时间
        client_end_random_max = 1;//随机启动应用最大化时间

        SendSize = 500;
        MaxBytes = 500;
    }

    void output(){
        std::cout<<"sender_and_receiver";
        std::cout<< sender_id <<"+"<<sender_port<<"+"<< sender_start_time <<"+"<<sender_end_time <<"+"<<receiver_id <<"+"<<receiver_port<<"+"<<receiver_start_time<<"+"<<receiver_end_time<<"+"<<DataRate<<"+"<<PacketSize<<std::endl;
    }

    int client_start_random_min;//随机启动应用的时间
    int client_end_random_max;//随机启动应用最大化时间



    int sender_id;    // 作为发送端的node,ID；
    //std::string sender_IP;
    int sender_port;

    float sender_start_time;
    float sender_end_time;

    int receiver_id; // 作为接收端的node,ID；
    //std::string receiver_IP;
    int receiver_port;

    float receiver_start_time;
    float receiver_end_time;

    int application_type;

    int PacketSize;
    std::string DataRate;

    float interval;
    int MaxPackets;

    // TCP专用的协议
    int SendSize;//每次最多发送的数量；
    int MaxBytes;//最多发送的数量；
};


// C++ 切分字符串
std::vector<std::string> split(const std::string& s,const std::string& delim)
{
    std::vector<std::string> elems;
    size_t pos = 0;
    size_t len = s.length();
    size_t delim_len = delim.length();
    if (delim_len == 0) return elems;
    
    while(pos < len)
    {
        int find_pos = s.find(delim,pos);
        if (find_pos < 0) {elems.push_back(s.substr(pos,len - pos));break;}
        elems.push_back(s.substr(pos,find_pos - pos));
        pos = find_pos + delim_len;
    }
    return elems;
}

int string_to_int(std::string str)
{
    char *p;
    const int len = str.length();
    p = new char[len+1];
    strcpy(p,str.c_str());
    int giveback = atoi(p);
    return giveback;
}

int string_to_float(std::string str)
{
    char *p;
    const int len = str.length();
    p = new char[len+1];
    strcpy(p,str.c_str());
    int giveback = atof(p);
    return giveback;
}

std::string int_add_string(std::string string1,int number)
{
    std::stringstream stream;
    stream<<number;
    std::string this_number =stream.str();
    std::string give_back =  this_number + string1;
    return give_back;
}

// 读取文件，对其按照标准格式进行切分，返回指定拓扑
std::vector<std::vector<std::string> > read_topology_file(std::string file_path)
{
    //std::cout << "1";
    
    std::ifstream adj_mat_file;
    
    adj_mat_file.open(file_path.c_str(),std::ios::in);
    
    if(adj_mat_file.fail()) { std::cout<<"读取文件失败"<<std::endl; }
    
    std::vector<std::vector<std::string> > topology;
    
    int i = 0;
    
    //int n_nodes = 0;
    
    while (!adj_mat_file.eof())
    {
        std::string line;
        
        std::string delim = ",";
        
        std::getline(adj_mat_file,line);
        
        if(line==""){
            //std::cout<<"空行"<<std::endl;

            break;}

        
        // 对字符串进行分割
        //topology[i] = split(line,delim);
        
        std::vector<std::string> temporary = split(line,delim);
        
        topology.push_back(temporary);

        i = i + 1;
        
        //std::cout<<i<<std::endl;
    }
    return topology;
}
// 读取文件，对其按照标准格式
std::vector<std::vector<int> > get_switch_topology(std::string switch_topology_filePath)
{
	std::vector<std::vector<std::string> > topology_file = read_topology_file(switch_topology_filePath);
    int topology_file_len = topology_file.size();
    std::vector<std::vector<int> > switch_topology(topology_file_len);
    for(int i =0;i<topology_file_len;i++){switch_topology.resize(topology_file_len);}
    
    for(int i= 0; i < topology_file_len ; i ++)
    { for(int j =0 ; j < topology_file_len ; j++)
    {
        // 从string转换到int型
        switch_topology[i][j] = string_to_int(topology_file[i][j]);
    }
    }
    return switch_topology;
}

std::vector<class terminal_switch>  get_terminal_switch(std::string switch_terminal_file_path) {
    
    // 得到交换机和终端连接
	std::vector<std::vector<std::string> > switch_terminal_file = read_topology_file(switch_terminal_file_path);
    
    int switch_terminal_number = switch_terminal_file.size();
    
    std::vector<class terminal_switch> vectorterminal_and_switch_list;
    
    for (int i = 0; i < switch_terminal_number; i++)
    {
        // 得到属性的长度
    	std::vector<std::string> this_terminal_switch_attribute;
        
        
        
        this_terminal_switch_attribute  = switch_terminal_file[i];
        
        terminal_switch terminal_switch_peer = terminal_switch();
        
        // 创建新的端口
        terminal_switch_peer.application_id   = string_to_int(this_terminal_switch_attribute[2]);
        terminal_switch_peer.application_port = string_to_int(this_terminal_switch_attribute[3]);
        terminal_switch_peer.switch_id = string_to_int(this_terminal_switch_attribute[0]);
        terminal_switch_peer.switch_port = string_to_int(this_terminal_switch_attribute[1]);
        
        vectorterminal_and_switch_list.push_back(terminal_switch_peer);
    }
    
    // 由于交换机链接一致，我设置其值为默认某值,不必进行更改
    for(int i= 0; i< switch_terminal_number; i++)
    { //vectorterminal_and_switch_list[i].output();
         }
    
    
    return vectorterminal_and_switch_list;
}

std::vector<class switch_and_switch> get_switch_and_switch(std::string switch_and_switch_file_path) {
    
	std::vector<class switch_and_switch> switch_and_switch_list;
    
	std::vector<std::vector<std::string> > topology_file  = read_topology_file(switch_and_switch_file_path);
    
    int topology_file_len = topology_file.size();
    
    std::vector<std::vector<int> > switch_topology;
    
    for(int i= 0; i < topology_file_len ; i ++)
    {   std::vector<int> switch_topology_i;
        for(int j =0 ; j < topology_file_len ; j++)
        {// 从string转换到int型
           switch_topology_i.push_back(string_to_int(topology_file[i][j])); }
        switch_topology.push_back(switch_topology_i);
    }
    
    int switch_len = switch_topology.size();
    
    // 得到缓冲区和两两链接
    for(int i = 0; i < switch_len; i++)
    {
        for(int j = i;j < switch_len; j++)
        {
            if(switch_topology[i][j] == 0) {continue;}
            else{
                switch_and_switch this_switch_and_switch = switch_and_switch();
                this_switch_and_switch.switch_Bandwidth= int_add_string("Mbps",switch_topology[i][j]);


                this_switch_and_switch.switch0_node_id = i;
                this_switch_and_switch.switch1_node_id = j;
                
                switch_and_switch_list.push_back(this_switch_and_switch);
            }
        }
    }
    
    // 由于交换机链接一致，我设置其值为默认某值,不必进行更改
    for(int i= 0; i < int(switch_and_switch_list.size()); i++) {
        //switch_and_switch_list[i].output();
    }
    
    return switch_and_switch_list;
}

std::vector<class sender_and_receiver> get_sender_and_receiver(std::string sender_and_receiver_file_path){
    
	std::vector<class sender_and_receiver> sender_receiver_list;
    
	std::vector<std::vector<std::string> > sender_and_receiver_file = read_topology_file(sender_and_receiver_file_path);
    
    int sender_and_receiver_number = sender_and_receiver_file.size();
    
    for (int i = 0; i < sender_and_receiver_number; i++) {
        // 得到属性的长度
    	std::vector<std::string> this_attribute = sender_and_receiver_file[i];
        
        sender_and_receiver sender_and_receiver_peer = sender_and_receiver();
        
        // 创建新的端口
        sender_and_receiver_peer.application_type = string_to_int(this_attribute[0]);
        
        sender_and_receiver_peer.sender_id =   string_to_int(this_attribute[1]);
        sender_and_receiver_peer.sender_port = string_to_int(this_attribute[2]);
        
        sender_and_receiver_peer.receiver_id   = string_to_int(this_attribute[3]);
        sender_and_receiver_peer.receiver_port = string_to_int(this_attribute[4]);
        
        sender_receiver_list.push_back(sender_and_receiver_peer);
    }
    
    
    // 由于交换机链接一致，我设置其值为默认某值,不必进行更改
    for(int i= 0; i < sender_and_receiver_number; i++) {
        sender_and_receiver  this_sender_and_receiver = sender_receiver_list[i];
        //this_sender_and_receiver.output();
    }
    return sender_receiver_list;
}


NS_LOG_COMPONENT_DEFINE("OpenFlowCsmaSwitch");

int main(int argc,char *argv[])
{
    // ---------------------------------------读取文件————————————————————————————————————————————
    std::string switch_and_switch_filePath = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/scratch/switch_and_switch.txt";
    
    std::string terminal_switch_filePath = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/scratch/terminal_switch.txt";
    
    std::string sender_and_receiver_filePath = "/home/dcn/eclipse-workspace/NS3/ns-allinone-3.20/ns-3.20/scratch/sender_and_receiver.txt";
    
    std::vector<class terminal_switch> terminal_switch_list  = get_terminal_switch(terminal_switch_filePath);
    
    std::vector<class sender_and_receiver> sender_receiver_list =get_sender_and_receiver(sender_and_receiver_filePath);

    std::vector<class switch_and_switch> switch_and_switch_list = get_switch_and_switch(switch_and_switch_filePath);
    
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
    //  LogComponentEnable("BridgeNetDevice",LOG_LEVEL_ALL);
    //  LogComponentEnable ("UdpClient", LOG_LEVEL_ALL);
    //  LogComponentEnable ("UdpServer", LOG_LEVEL_ALL);
    
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    //NS_LOG_INFO ("Create nodes");
    
    // 创建所有终端的点
    NodeContainer terminals;
    terminals.Create(terminals_len);
    
    // 创建所有的交换机的点
    NodeContainer Switchs;
    Switchs.Create(switchs_len);

    // 创建交换机拓扑的适合的容器
    NetDeviceContainer terminalDevices;
    NetDeviceContainer switchDevices;// 交换机的节点
    NetDeviceContainer link;

    // 用来承载所有的NetDeviceContainer
    std::vector <class NetDeviceContainer> switch_NetContainer_list(switchs_len);

     // ---------------------------------------针对交换机而言————————————————————————————————————————————
    int switch_peer_number = switch_and_switch_list.size();
    for(int switch_i = 0;switch_i < switch_peer_number;switch_i ++)
    {
        switch_and_switch this_switch_peer = switch_and_switch_list[switch_i];
        // 如果是同一个交换机，就继续运行，防止出现冲突；
        if(this_switch_peer.switch0_node_id == this_switch_peer.switch1_node_id){continue;}
        // 得到某一对节点
        NodeContainer switch_peer  = NodeContainer(Switchs.Get(this_switch_peer.switch0_node_id),Switchs.Get(this_switch_peer.switch1_node_id));
        // 针对其设置信道,设置帧
        CsmaHelper csma;
        csma.SetChannelAttribute("DataRate",DataRateValue(this_switch_peer.switch_Bandwidth));
        csma.SetChannelAttribute("Delay",StringValue(this_switch_peer.switch_delay));
    csma.SetQueue("ns3::DropTailQueue","MaxPackets",UintegerValue(this_switch_peer.queue_maxpacket));
         //std::string InterframeGap = "100000000ns";这个东西失效了；
       // csma.SetDeviceAttribute ("InterframeGap",StringValue(this_switch_peer.interframeGap));
        
        link = csma.Install(switch_peer);

        // 设定FrameGap
        Ptr<Node> node = Switchs.Get(this_switch_peer.switch0_node_id);
        int Ndevice = node->GetNDevices();
        //std::cout<<Ndevice<<std::endl;
        Ptr<NetDevice> dev = node->GetDevice(Ndevice-1);
        Ptr<CsmaNetDevice> csmaDev = dev ->GetObject<CsmaNetDevice>();
        csmaDev->SetInterframeGap(Seconds(this_switch_peer.interframeGap));
 
        node = Switchs.Get(this_switch_peer.switch1_node_id);
        Ndevice  = node->GetNDevices();
        dev = node->GetDevice(Ndevice-1);
        csmaDev = dev ->GetObject<CsmaNetDevice>();
        csmaDev->SetInterframeGap(Seconds(this_switch_peer.interframeGap));

        switch_NetContainer_list[this_switch_peer.switch0_node_id].Add(link.Get(0));
        switch_NetContainer_list[this_switch_peer.switch1_node_id].Add(link.Get(1));
    }
                                 
    // ---------------------------------------针对交换机与终端而言————————————————————————————————————————————
    int terminal_and_switch_number = terminal_switch_list.size();
    for(int application_i =0;application_i < terminal_and_switch_number;application_i++)
    {
        terminal_switch this_terminal_switch = terminal_switch_list[application_i];
        
        CsmaHelper csma;
    csma.SetChannelAttribute("DataRate",DataRateValue(this_terminal_switch.point_to_point_DataRate));
        csma.SetChannelAttribute("Delay",StringValue(this_terminal_switch.point_to_point_Delay));
    csma.SetQueue("ns3::DropTailQueue","MaxPackets",UintegerValue(this_terminal_switch.DropTailQueue_MaxPackets));

        link = csma.Install(NodeContainer(terminals.Get(this_terminal_switch.application_id), Switchs.Get(this_terminal_switch.switch_id)));

        // 设定FrameGap
        Ptr<Node> node = terminals.Get(this_terminal_switch.application_id);
        Ptr<NetDevice> dev = node->GetDevice(0);
        Ptr<CsmaNetDevice> csmaDev = dev ->GetObject<CsmaNetDevice>();
        csmaDev->SetInterframeGap(Seconds(this_terminal_switch.InterframeGap));
        // 问题来了，这个能这么写吗？ 答案是不能，我要对switch维持一个表,记录这是第几个设备，或者加n-1个device,这个操作没有验证，先用着吧
        node = Switchs.Get(this_terminal_switch.switch_id);
        int Ndevice  = node->GetNDevices();
        dev = node->GetDevice(Ndevice-1);
        csmaDev = dev ->GetObject<CsmaNetDevice>();
        csmaDev->SetInterframeGap(Seconds(this_terminal_switch.InterframeGap));

        switch_NetContainer_list[this_terminal_switch.switch_id].Add(link.Get(1));
        terminalDevices.Add(link.Get(0));
    }

    // ---------------------------------------对网桥和IP进行配置————————————————————————————————————————————
    // std::cout<<"switch"<<std::endl;
    for(int i =0;i<switchs_len ;i++)
    {
        Ptr<Node> OFNode1 = Switchs.Get(i);
        BridgeHelper bridge;
        bridge.Install(OFNode1,switch_NetContainer_list[i]);
    }
    //Add internet stacks to the terminals
    InternetStackHelper internet;
    internet.Install (terminals);
    //Add IP addresses
    //NS_LOG_INFO ("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interface = ipv4.Assign(terminalDevices);

    // ---------------------------------应用与应用之间的配置--------------------------------------------------
    int  sender_receiver_number = sender_receiver_list.size();
    for(int sender_receiver_i = 0; sender_receiver_i < sender_receiver_number;sender_receiver_i++) {
        sender_and_receiver this_sender_and_receiver = sender_receiver_list[sender_receiver_i];

        NodeContainer sender_receiver_container = NodeContainer(terminals.Get(this_sender_and_receiver.sender_id),
                                                                terminals.Get(this_sender_and_receiver.receiver_id));

        // 对应用类型进行判断,如果是0,我们认为其是周期数据
        if (this_sender_and_receiver.application_type == 0) {
            LogComponentEnable("UdpClient", LOG_LEVEL_INFO);
            LogComponentEnable("UdpServer", LOG_LEVEL_INFO);

            //服务器端的安装
            UdpServerHelper Server(this_sender_and_receiver.receiver_port);
            Server.SetAttribute("ShengPeerId",UintegerValue(sender_receiver_i*10 + 0));
            ApplicationContainer serverApps = Server.Install(terminals.Get(this_sender_and_receiver.receiver_id));
            serverApps.Start(Seconds(this_sender_and_receiver.receiver_start_time));
            serverApps.Stop(Seconds(this_sender_and_receiver.receiver_end_time));
            // 客户端的安装  ==== 》 你能保证顺序的一一对应吗？

            UdpClientHelper echoClient(interface.GetAddress(this_sender_and_receiver.receiver_id),
                                       this_sender_and_receiver.receiver_port);
            echoClient.SetAttribute("Interval", TimeValue(Seconds(this_sender_and_receiver.interval)));
            echoClient.SetAttribute("PacketSize", UintegerValue(this_sender_and_receiver.PacketSize));
            echoClient.SetAttribute("MaxPackets", UintegerValue(this_sender_and_receiver.MaxPackets));
            echoClient.SetAttribute("ShengPeerId",UintegerValue(sender_receiver_i*10 + 0));


            //创建了相应的application
            ApplicationContainer clientApps = echoClient.Install(terminals.Get(this_sender_and_receiver.sender_id));
            //这个转化问题始终没有解决
            clientApps.Start(Seconds(this_sender_and_receiver.sender_start_time));  //关键是开始时间
            clientApps.Stop(Seconds(this_sender_and_receiver.sender_end_time));
        }

        // 如果类型是1，我们认为其是突发数据
        if(this_sender_and_receiver.application_type == 1){
            LogComponentEnable("UdpClient", LOG_LEVEL_INFO);
            LogComponentEnable("UdpServer", LOG_LEVEL_INFO);

            //服务器端的安装
            UdpServerHelper Server(this_sender_and_receiver.receiver_port);
            Server.SetAttribute("ShengPeerId",UintegerValue(sender_receiver_i*10 + 1));
            ApplicationContainer serverApps = Server.Install(terminals.Get(this_sender_and_receiver.receiver_id));
            serverApps.Start(Seconds(this_sender_and_receiver.receiver_start_time));
            serverApps.Stop(Seconds(this_sender_and_receiver.receiver_end_time));

            // 客户端的安装
            UdpClientHelper echoClient(interface.GetAddress(this_sender_and_receiver.receiver_id),
                                       this_sender_and_receiver.receiver_port);

            echoClient.SetAttribute("Interval", TimeValue(Seconds(this_sender_and_receiver.interval)));
            echoClient.SetAttribute("PacketSize", UintegerValue(this_sender_and_receiver.PacketSize));
            echoClient.SetAttribute("MaxPackets", UintegerValue(this_sender_and_receiver.MaxPackets));
            echoClient.SetAttribute("ShengPeerId",UintegerValue(sender_receiver_i*10 + 1));


            //创建了相应的application
            ApplicationContainer clientApps = echoClient.Install(terminals.Get(this_sender_and_receiver.sender_id));
            // 随机化发生时间
            Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();
            x->SetAttribute ("Min", DoubleValue (this_sender_and_receiver.client_start_random_min));
            x->SetAttribute ("Max", DoubleValue (this_sender_and_receiver.client_end_random_max));
            double rn = x->GetValue ();
            clientApps.Start(Seconds(this_sender_and_receiver.sender_start_time + rn));  //关键是开始时间
            clientApps.Stop(Seconds(this_sender_and_receiver.sender_end_time));
        }

        // 如果类型是2，我们认为其是流式数据
        if(this_sender_and_receiver.application_type == 2){
            LogComponentEnable ("BulkSendApplication", LOG_LEVEL_ALL);
            LogComponentEnable ("PacketSink", LOG_LEVEL_ALL);

            //服务器端的安装
            PacketSinkHelper sink_Server ("ns3::TcpSocketFactory",InetSocketAddress(interface.GetAddress (this_sender_and_receiver.receiver_id),this_sender_and_receiver.receiver_port));
            sink_Server.SetAttribute("ShengPeerId",UintegerValue(sender_receiver_i*10 + 2));
            ApplicationContainer serverApps = sink_Server.Install (terminals.Get(this_sender_and_receiver.receiver_id));
            serverApps.Start (Seconds (this_sender_and_receiver.receiver_start_time));
            serverApps.Stop (Seconds (this_sender_and_receiver.receiver_end_time));

            // 客户端的安装
            BulkSendHelper Client_helper("ns3::TcpSocketFactory",InetSocketAddress(interface.GetAddress (this_sender_and_receiver.receiver_id),this_sender_and_receiver.receiver_port));
            Client_helper.SetAttribute("ShengPeerId",UintegerValue(sender_receiver_i*10 + 2));

            Client_helper.SetAttribute("SendSize",UintegerValue (this_sender_and_receiver.SendSize));//每一次发送的数量
            Client_helper.SetAttribute("MaxBytes",UintegerValue (this_sender_and_receiver.MaxBytes));//最多发送的数量
            ApplicationContainer clientApps =Client_helper.Install(terminals.Get(this_sender_and_receiver.sender_id));
            //这个转化问题始终没有解决
            clientApps.Start (Seconds(this_sender_and_receiver.sender_start_time));  //关键是开始时间.其实这些该是同一时间加载的，否则结果不会是如预想的一样
            clientApps.Stop (Seconds(this_sender_and_receiver.sender_end_time));
        }
    }


    // ---------------------------------程序的运行与信息的输出--------------------------------------------------
    NS_LOG_INFO ("Run simulation");
    Simulator::Run();
    Simulator::Destroy();
    NS_LOG_INFO ("Done.");
}










/* 交换机拓扑
 *         交换机名
 * 交换机名
 *          1，0，3，0，4，1
 *          1，0，4，2，5，1
 *          0，1，4，2，4，1
 *          0，1，2，4，2，1
 *          0，1，0，0，1，2
 *          0，2，4，2，4，2
*/
// 必须是一个矩形矩阵
// 权值表示拓扑的带宽
// 必须是一个对称矩阵


// 交换机和接口的表
// 权值表示缓冲队列的长度




/* 交换机和终端连接
 *         交换机编号0，交换机接口端口1，终端的编号2，自己的接口port3,和交换机的连接port4，缓冲区的长度5，传输数率，延迟，帧gap
 * 应用编号
 *          1，  0，  3，  0，  4，  1
 *          1，  0，  4，  2，  5，  1
 *          0，  1，  4，  2，  4，  1
 *          0，  1，  2，  4，  2，  1
 *          0，  1，  0，  0，  1，  2
 *          0，  2，  4，  2，  4，  2
 */

/* 应用之间的邻接矩阵表
 *         应用编号
 * 应用编号
 *          1，0，1，0，1，1
 *          1，0，1，1，1，1
 *          0，1，1，2，1，1
 *          0，1，3，1，2，1
 *          0，1，0，0，1，2
 *          0，1，1，1，1，2
*/
/* 应用之间的属性表     代表发送的应用类型

 * 应用发送属性表
 * 应用名称： 发送类型，发送应用，发送接口，接收应用，接受接口，发送应用开始时间，发送应用结束时间，接受应用开始时间，接收应用结束时间，链路传输速率，链路包大小
 *          1，0，1，0，1，1
 *          1，0，1，1，1，1
 *          0，1，1，2，1，1
 *          0，1，2，4，2，1
 *          0，1，0，0，1，2
 *          0，2，4，2，4，2
 */
