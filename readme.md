### 该git记录着我曾经完成的某项目，由于代码私密，只留下一些片段文件，供后人参考。
*（不可运行）*
_项目中利用到的库，主要是NS3库，有需要学习NS3的同学，可以参考该git.毕竟网上相关内容太少，难以收集资料。_

---

1. point_to_point_simple.cc
    一个最简单的点对点链路,可以用来最初的学习，照着敲一遍。

2. csmatest.cc
    利用csmatest进行交换机开发，输出最后结果。

3. terminal_switch_switch_terminal_example.cc
    在整个过程中构建terminal_switch_switch_terminal的简单结构

4. openflow.cc
    利用openflow进行路由转发的文件功能。

5. test_static_route.cc
    内有可以自定义静态路由表的功能。

6. Network_topology.cc
    内有结构清晰的，构建节点连接的过程。

7. topology_end.cc:
    根据拓扑,创建终端,创建交换机,设定FrameGap,配置突发流量,TCP流量,UDP流量，对帧进行frame规整的功能。

8. file
   - NS3网络模拟器基础与应用.pdf
9. package
   - 简单的仿真平台demo,包括拓扑的读取，突发流量，TCP流量，UDP流量的配置，帧的规整化，交换机队列的控制，对静态路由表控制等等一些列操作。（当然是不可运行的）。
        1. lib.h:自定义库文件;
        2. construct_file_for_one：读取文件;
        3. construct_file_for_one：读取文件;
        4. send_by_one_application.cc：只用一个文件进行发送的测试接口;
        5. 8_terminal_1.cc:主函数;8个交换机，其上可以有数个终端，互相来回发包。
        6. sample:三个输入的样例，仅供参考;
          - suggested_app_bandwidths_matrix.txt;
          - traffictrace.txt; 
          - vrch.txt;

