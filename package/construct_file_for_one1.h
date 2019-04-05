#ifndef CONSTRUCTFILE_H
#define CONSTRUCTFILE_H

#include<cstring>
#include <iostream>
#include <fstream>
#include<sstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <set>

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
        //switch_delay = "2ms";
        //queue_maxpacket= 1000;
        //interframeGap = 0.01;
    }
    void output() {
        std::cout<<"switch_and_switch";
        std::cout<<"交换机1号"<<switch0_node_id <<"+"<<"交换机2号"<<switch1_node_id<<"交换机之间的链路带宽"<<switch_Bandwidth<<std::endl;
    }

    // 交换机1
    int switch0_node_id;
    //int switch0_queue_maxPackets;

    // 交换机2
    int switch1_node_id;
    //int switch1_queue_maxPackets;

    // 链路与链路之间的连接
    std::string switch_Bandwidth;
    //std::string switch_delay;
    //int queue_maxpacket;
    //int interframeGap;


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
        application_port = 9;

        switch_id = 0;
        //switch_port = 9;


        //DropTailQueue_MaxPackets = 1000;

        //point_to_point_DataRate = "1Mbps";
        //point_to_point_Delay = "2ms";

        //InterframeGap = 0.01;
    }

    void output() {
        std::cout<<"terminal_switch"<<std::endl;
        std::cout<<"应用id" << application_id<<"+"<<"应用端口"<<application_port<<"+"<<"交换机"<<switch_id<<"+" << std::endl;

        //<<"+"<< point_to_point_DataRate<<"+"<<point_to_point_Delay<<"+"<<InterframeGap<<"+"<<DropTailQueue_MaxPackets<<std::endl;
    }

    int application_id;
    int application_port;   // 连接端口
    int switch_id;
    //int switch_port;

    // 延迟和数据传输速率
    //std::string point_to_point_DataRate;
    //std::string point_to_point_Delay;

    // 帧平整，队列
    //int InterframeGap;
    //int DropTailQueue_MaxPackets;

    // std::string InterframeGap = "100000000ns";
    //int object_switch_order_number;   // 目标交换机的序号；
};

class sender_and_receiver
{
public:
    sender_and_receiver()
    {

        type = 0;

        sender_terminal_id = 0;
        sender_port = 9;

        receiver_terminal_id = 0;
        receiver_port = 9;

        sender_start_time = 0.0;
        sender_end_time = 1000.0;

        receiver_start_time = 0.0;
        receiver_end_time = 1000.0;

        packet_size = 1024;
        packet_number = 0;
        interval = 0;

        //DataRate = "1Mbps";
        //PacketSize = 1024;
        //interval = 1.0;
        //MaxPackets = 5;

        //client_start_random_min = 0;//随机启动应用的时间
        //client_end_random_max = 1;//随机启动应用最大化时间

        //SendSize = 500;
        //MaxBytes = 500;
    }

    void output(){
        std::cout<<"sender_and_receiver";
        std::cout<<"发送的类型"<<type <<"+"
                 <<"发送终端节点"<< sender_terminal_id <<"+"
                 <<"发送端口"<<sender_port<<"+"
                 <<"发送起始时间"<<sender_start_time <<"+"
                 <<"发送结束时间"<<sender_end_time   <<"+"
                 <<"接收端ID"<<receiver_terminal_id <<"+"
                 <<"接收端端口"<<receiver_port<<"+"
                 <<"接受开始时间"<<receiver_start_time<<"+"
                 <<"接受结束时间"<<receiver_end_time<<"+"
                 <<"包的大小"<<packet_size<<"+"
                 <<"包的数量"<<packet_number<<"+"
                 <<"发包间隔"<<interval<<std::endl;


        //<<"+"<<DataRate<<"+"<<PacketSize<<std::endl;
    }

    //int client_start_random_min;//随机启动应用的时间
    //int client_end_random_max;//随机启动应用最大化时间



      int type;
      int sender_terminal_id;
      int sender_port;

      int receiver_terminal_id;
      int receiver_port;

      double sender_start_time;
      double sender_end_time;

      double  receiver_start_time;
      double  receiver_end_time;

      int  packet_size;
      int  packet_number;
      float  interval;

    //int PacketSize;
    //std::string DataRate;

    //float interval;
    //int MaxPackets;

    // TCP专用的协议
    //int SendSize;//每次最多发送的数量；
    //int MaxBytes;//最多发送的数量；
};

// 8月2日：终端和交换机的关系表
class NodeMessage
{
public:
	NodeMessage(){
        first_node_id = -1;
        second_node_id = -1;

        first_interface_number = 0;
        first_position_number = -1;
        first_IP = " ";

        second_interface_number = 0;
        second_position_number = -1;
        second_IP = " ";
        first_position_number_point_list = -1;

    }

    	void output(){
        std::cout<<"NodeMessage";
        std::cout<<"第一个节点号"<<first_node_id <<"+"
                 <<"接口序号"<< first_interface_number<<"+"
                 <<"容器位置"<<first_position_number<<"+"
                 <<"IP编号"<< first_IP<<"+"
                 <<"第二个节点号"<<second_node_id <<"+"
                 <<"接收序号"<<second_interface_number <<"+"
                 <<"容器位置"<<second_position_number<<"+"
                 <<"IP编号"<< second_IP
				 <<"position:"<< first_position_number_point_list
                  <<std::endl;
    };

        int first_node_id;         //从0开始的顺序编号,1是终端的节点
        int second_node_id;

        int first_interface_number;// 在自己节点中的接口序号
        int first_position_number; // 在所有容器中的位置
        std::string first_IP;           // IP的列表

        int second_interface_number;// 在自己节点中的接口序号
        int second_position_number; // 在所有容器中的位置
        std::string second_IP;            // IP的列表


        int first_position_number_point_list;   // 在point_list中的位置,只要记住一个就好了，目前也只改这一个


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

    //std::cout<<"enheng"<<len<<std::endl;



    p = new char[len+1];
    strcpy(p,str.c_str());
    int giveback = atoi(p);

    free(p);
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
// 8月3日：在string后加上int
std::string string_add_int(std::string string1,int number)
{
    std::stringstream stream;
    stream<<number;
    std::string this_number =stream.str();
    std::string give_back =  string1 + this_number;
    return give_back;
}

// 读取文件，对其按照标准格式进行切分，返回指定拓扑
std::vector<std::vector<std::string> > read_topology_file(std::string file_path)
{
    std::ifstream adj_mat_file;

    adj_mat_file.open(file_path.c_str(),std::ios::in);

    if(adj_mat_file.fail()) { std::cout<<"读取文件失败"<<std::endl; }

    std::vector<std::vector<std::string> > topology;

    int i = 0;

    while (!adj_mat_file.eof())
    {
        std::string line;

        std::string delim = " ";

        std::getline(adj_mat_file,line);

        if(line==""){
            //std::cout<<"空行"<<std::endl;
            break;}


        // 对字符串进行分割
        //topology[i] = split(line,delim);

        std::vector<std::string> temporary = split(line,delim);

        topology.push_back(temporary);

        i = i + 1;

       // std::cout<<i<<std::endl;
    }

    adj_mat_file.close();
    return topology;
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

        // 终端到switch的连接
        terminal_switch_peer.application_id  = i;
        terminal_switch_peer.switch_id = string_to_int(this_terminal_switch_attribute[2]);

       // terminal_switch_peer.output();

        vectorterminal_and_switch_list.push_back(terminal_switch_peer);
    }
    return vectorterminal_and_switch_list;
}

std::vector<class switch_and_switch> get_switch_and_switch(std::string switch_and_switch_file_path) {

	std::vector<class switch_and_switch> switch_and_switch_list;

	std::vector<std::vector<std::string> > topology_file  = read_topology_file(switch_and_switch_file_path);

    int topology_file_len = topology_file.size();

    std::vector<std::vector<int> > switch_topology;

    for(int i= 0; i < topology_file_len ; i ++)
    {
        std::vector<int> switch_topology_i;
        for(int j =0 ; j < topology_file_len ; j++)
        {
         // 从string转换到int型
         switch_topology_i.push_back(string_to_int(topology_file[i][j]));
        }
         switch_topology.push_back(switch_topology_i);
    }

    int switch_len = switch_topology.size();

    // 得到缓冲区和两两链接
    for(int i = 0; i < switch_len; i++)
    {
        for(int j = i;j < switch_len; j++)
        {

            if(switch_topology[i][j] == 0) {continue;}

            if(i == j) {continue;}

            else
            {
                switch_and_switch this_switch_and_switch = switch_and_switch();

                this_switch_and_switch.switch_Bandwidth= int_add_string("Mbps",switch_topology[i][j]);

                this_switch_and_switch.switch0_node_id = i;
                this_switch_and_switch.switch1_node_id = j;

                //this_switch_and_switch.output();
                switch_and_switch_list.push_back(this_switch_and_switch);
            }
        }
    }
    return switch_and_switch_list;
}



// 创建一个新的对,统计每一列包的数量
std::vector<int>  get_MaxPacket_number(std::string traffictrac_file_path)
{
     // return
    std::vector<int> traffictrace_MaxPacket_list;
     //input
    std::vector<std::vector<std::string> > traffictrace_file = read_topology_file( traffictrac_file_path);


    int row_len    = traffictrace_file.size();
    int column_len = traffictrace_file[0].size();

    for(int column =0; column < column_len; column++)
    {
        int number_sum = 0;
        for(int row = 0; row < row_len; row ++)
        {
           int type = string_to_int(traffictrace_file[row][column]);

           if(type == 1024)
           {
              number_sum = number_sum + 1024;
           }
           if(type == 144)
           {
              number_sum = number_sum + 144;
           }
           if(type == 50)
           {
              number_sum = number_sum + 50;
           }
        }
        if(number_sum==0)
        {
        	 traffictrace_MaxPacket_list.push_back(1);

        }
        else{

        	  traffictrace_MaxPacket_list.push_back(number_sum);
        }

    }
    return traffictrace_MaxPacket_list;
}



// 每秒构建一个peer对
std::vector<std::vector<class sender_and_receiver> > get_sender_and_receiver(std::string traffictrac_file_path,std::string vrch_file_path)
{
    //return
    std::vector<std::vector<class sender_and_receiver> > sender_receiver_matrix;

    //input
    std::vector<std::vector<std::string> > traffictrace_file = read_topology_file(traffictrac_file_path);
    std::vector<std::vector<std::string> > vrch_file = read_topology_file(vrch_file_path);// 多播矩阵
    std::vector<int> traffictrace_MaxPacket_list = get_MaxPacket_number(traffictrac_file_path);

    int vrch_number = vrch_file.size();
    int traffictrace_file_column_len = traffictrace_file[0].size();
    int traffictrac_len =traffictrace_file.size();


    std::vector<int> port_record(traffictrace_file_column_len);
    for(int r_i = 0;r_i< traffictrace_file_column_len; r_i++){port_record[r_i]=9;}

       // 在每一秒内
    for(int traffictrac_i = 0; traffictrac_i < traffictrac_len; traffictrac_i++)
    {
        std::vector<class sender_and_receiver> sender_and_receiver_list;

        double start_time = traffictrac_i;

        double end_time = traffictrac_i + 1.0;

        // 对每个终端
        for(int traffictrac_j = 0; traffictrac_j < traffictrace_file_column_len;traffictrac_j++)
        {

                int Max_packet_number = traffictrace_MaxPacket_list[traffictrac_j];

                // 此时trafficctrac化作了多播矩阵的i;
                for(int j = 0;j <vrch_number ;j++)
                {
                if(traffictrac_j == j){continue;}

                if(string_to_int(vrch_file[traffictrac_j][j])==0){continue;}

                sender_and_receiver sender_and_receiver_peer = sender_and_receiver();

                // 某1S的时间段内
                sender_and_receiver_peer.type = string_to_int(traffictrace_file[traffictrac_i][traffictrac_j]);

                // 发送端的终端号
                sender_and_receiver_peer.sender_terminal_id   = traffictrac_j;
                sender_and_receiver_peer.receiver_terminal_id = j;

                // 发送端开始时间和结束时间
                sender_and_receiver_peer.sender_start_time = start_time;
                sender_and_receiver_peer.sender_end_time = end_time + 1000.0;

                sender_and_receiver_peer.receiver_start_time =start_time ;
                sender_and_receiver_peer.receiver_end_time =start_time + 1000.0;


                // port 是用来维持端口号的一个东西，现在就不需要了
                 sender_and_receiver_peer.receiver_port= port_record[sender_and_receiver_peer.receiver_terminal_id] + 1;
                 port_record[sender_and_receiver_peer.receiver_terminal_id] = port_record[sender_and_receiver_peer.receiver_terminal_id] +1;

                // 决定数据量和间隔
                if(sender_and_receiver_peer.type== 0)
                {
                    sender_and_receiver_peer.packet_size = 1024;
                    sender_and_receiver_peer.packet_number = Max_packet_number;
                    sender_and_receiver_peer.interval    = 1;
                }
                if(sender_and_receiver_peer.type == 1024)
                {
                    // 数据量为1024KB
                    sender_and_receiver_peer.packet_size = 1024;
                    sender_and_receiver_peer.packet_number = Max_packet_number;
                    sender_and_receiver_peer.interval = 0.001;
                }
                if(sender_and_receiver_peer.type == 50)
                {
                    sender_and_receiver_peer.packet_size = 1024;
                    sender_and_receiver_peer.packet_number = Max_packet_number;
                    sender_and_receiver_peer.interval = 0.02;;
                }
                if(sender_and_receiver_peer.type == 144)
                {
                    sender_and_receiver_peer.packet_size = 1024;
                    sender_and_receiver_peer.packet_number = Max_packet_number;
                    sender_and_receiver_peer.interval = 0.007;
                }
                sender_and_receiver_list.push_back(sender_and_receiver_peer);
                }
        }
        sender_receiver_matrix.push_back(sender_and_receiver_list);
    }
    return sender_receiver_matrix;
}





// 8月2日：读取路由中的路径
std::vector<std::vector<int> > get_route(std::string route_file_path)
{
    // return
    std::vector<std::vector<int> > route_list;

    std::vector<std::vector<std::string> > route_file = read_topology_file(route_file_path);
    int route_number = route_file.size();
    for (int i = 0; i < route_number; i++)
    {
        int line_len = route_file[i].size();
        std::vector<int> line_list;
        for(int line_index = 0; line_index < line_len; line_index++)
        {
            line_list.push_back(string_to_int(route_file[i][line_index]));
        }
        route_list.push_back(line_list);
    }
    return route_list;
}

std::vector<std::vector<int> > get_dataRate(std::string dataRate_file_path)
{
    // return
    std::vector<std::vector<int> > dataRate_list;

    std::vector<std::vector<std::string> > dataRate_file = read_topology_file(dataRate_file_path);

    int dataRate_number = dataRate_file.size();
    for(int i=0; i<dataRate_number; i++)
    {
        int line_len = dataRate_file[i].size();
        std::vector<int> line_list;
        for(int line_index = 0;line_index < line_len;line_index++)
        {
             double number =  string_to_float(dataRate_file[i][line_index]);
             line_list.push_back((int)(number));
        }
        dataRate_list.push_back(line_list);
    }
    return dataRate_list;
}

// 8月2日：根据两个交换机端节点，找到对应的路由
std::vector<int > get_object_route(int terminal1,int terminal2,std::vector<std::vector<int> > route_list)
{
    std::vector<int> object_route;

    int route_number = route_list.size();
    for(int i = 0; i < route_number; i++)
    {
        std::vector<int> this_list = route_list[i];
        int this_list_len = this_list.size();
        int first = this_list[0];
        int end = this_list[this_list_len-1];

        if(first==terminal1&&end==terminal2)
        {
        return this_list;
        }
    }
    object_route.push_back(0);
    return object_route;
}

// 8月2日得到接口的位置
int get_interface_position(int switchs,std::vector<class NodeMessage> terminal_and_switch_link_table,std::vector<class NodeMessage> switch_and_switch_link_table)
{
    int terminal_and_switch_link_table_len = terminal_and_switch_link_table.size();
    int switch_and_switch_link_table_len =   switch_and_switch_link_table.size();

    int all_node = 0;
    int max_interface = 0;

    for(int i= 0;i < terminal_and_switch_link_table_len; i++)
    {
        NodeMessage this_nodeMessage = terminal_and_switch_link_table[i];
        if(this_nodeMessage.second_node_id == switchs)
        {
            all_node ++;
            if(max_interface <= this_nodeMessage.second_interface_number)
            {
                max_interface = this_nodeMessage.second_interface_number;
            }
        }
    }

    for(int j=0; j < switch_and_switch_link_table_len; j++)
    {
        NodeMessage this_nodeMessage = switch_and_switch_link_table[j];
        if(this_nodeMessage.first_node_id == switchs)
        {
            all_node ++;
            if(max_interface <= this_nodeMessage.first_interface_number)
            {
                max_interface = this_nodeMessage.first_interface_number;
            }
        }
    }

    if(all_node == max_interface)
    {
        return all_node;
    }
    else{
    //	std::cout<<all_node<<std::endl;
    //std::cout<<max_interface<<std::endl;
        return -1;
    }
}

// 8月2日：根据一个节点端号，得到一个对应的节点的IP
std::string get_object_IP(int terminal_id,std::vector<class NodeMessage>  terminal_and_switch_link_table)
{

    int terminal_and_switch_link_table_len = terminal_and_switch_link_table.size();

    for(int i=0;i<terminal_and_switch_link_table_len;i++)
    {
        NodeMessage this_terminal_and_switch_link = terminal_and_switch_link_table[i];

        if(this_terminal_and_switch_link.first_node_id==terminal_id)
        {
            return this_terminal_and_switch_link.first_IP;
        }
    }
   return "fault";
}

// 8月2日：根据终端，得到对应的node的编号
int  get_object_switch_node(int terminal_id,std::vector<class NodeMessage> terminal_and_switch_link_table)
{
    int terminal_and_switch_link_table_len = terminal_and_switch_link_table.size();

    for(int i=0;i<terminal_and_switch_link_table_len;i++)
    {
        NodeMessage this_terminal_and_switch_link = terminal_and_switch_link_table[i];

        if(this_terminal_and_switch_link.first_node_id==terminal_id)
        {
            return this_terminal_and_switch_link.second_node_id;
        }
    }
    return -1;
}

// 根据终端，得到对应的指针
int  get_object_position_in_point_list(int terminal_id,std::vector<class NodeMessage> terminal_and_switch_link_table)
{
    int terminal_and_switch_link_table_len = terminal_and_switch_link_table.size();

    for(int i=0;i<terminal_and_switch_link_table_len;i++)
    {
        NodeMessage this_terminal_and_switch_link = terminal_and_switch_link_table[i];

        if(this_terminal_and_switch_link.first_node_id==terminal_id)
        {
            return this_terminal_and_switch_link.first_position_number_point_list;
        }
    }
    return -1;
}

// 根据两个交换机编号，查找两个交换机之间的连接；返回连接;
NodeMessage
get_node_message(int switch1,int switch2,std::vector<class NodeMessage>  switch_and_switch_link_table)
{
    int switch_and_switch_link_table_len = switch_and_switch_link_table.size();
    for(int route_i = 0 ; route_i < switch_and_switch_link_table_len; route_i ++)
    {
      int first = switch_and_switch_link_table[route_i].first_node_id;
      int second = switch_and_switch_link_table[route_i].second_node_id;

      if (switch1 == first && switch2 == second)
      {
        return switch_and_switch_link_table[route_i];
      }
    }

  return NodeMessage();
}

#endif
