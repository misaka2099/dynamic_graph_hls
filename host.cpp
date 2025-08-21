#include "data_type.h"
#include "deprecated/xrt.h"
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::cin, std::cout, std::endl;
using std::fstream;
int size = 1024*1024*64;
int main(int argc, char**argv)
{
    std::string xclbin_name;
    std::string graph_name;


    if (argc < 3)
    {
        cout << "USAGE <graph> <xclbin>" <<endl;
        return 1;
    }
    graph_name = argv[1];
    xclbin_name = argv[2];

    fstream file;
    file.open(graph_name);
    if (!file.is_open())
    {
        std::cout << "ERROR: Cannot open file " << graph_name << std::endl;
        return (1);
    }
    std::vector<std::pair<int, int>> pairs;
    int max_vertex = -1;;
    int a, b;
    while (file >> a >> b) {
        pairs.emplace_back(a, b);
        max_vertex = std::max(std::max(a,b),max_vertex);
    }
    file.close();

    
    int edge_num = pairs.size()*2;
    int vertex_num = max_vertex + 1;
    std::vector<int> src(edge_num), dst(edge_num);
    for (int i = 0; i < pairs.size(); ++i) {
        src[i] = pairs[i].first;
        src[i+pairs.size()] = pairs[i].second;
        dst[i] = pairs[i].second;
        dst[i+pairs.size()] = pairs[i].first;
    }

    graph_pass_struct G_pass;
    
    auto device = xrt::device(0);
    auto id = device.load_xclbin(xclbin_name);
    auto increase_kernel = xrt::kernel(device, id, "increase_graph");
    auto secure_kernel = xrt::kernel(device, id, "secure_graph");
    cout << "CREATED KERNELS" << endl;
    auto run_increase = xrt::run(increase_kernel);
    auto bo_hbm_base_address = xrt::bo(device, sizeof(unsigned char) * size, increase_kernel.group_id(4));
    auto bo_valid_array = xrt::bo(device, sizeof(char) * size, increase_kernel.group_id(5));
    unsigned char* ptr_base  = bo_hbm_base_address.map<unsigned char*>();
    char*          ptr_valid = bo_valid_array.map<char*>();
    std::fill_n(ptr_base,  size, 0u);               // unsigned char 置 0
    std::fill_n(ptr_valid, size, 0); 
    bo_hbm_base_address.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo_valid_array.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    auto bo_src = xrt::bo(device, sizeof(int) * edge_num, increase_kernel.group_id(2));
    auto bo_dst = xrt::bo(device, sizeof(int) * edge_num, increase_kernel.group_id(3));
    auto src_ptr = bo_src.map<int*>();
    auto dst_ptr = bo_dst.map<int*>();
    memcpy(src_ptr, src.data(), sizeof(int) * edge_num);
    memcpy(dst_ptr, dst.data(), sizeof(int) * edge_num);
    bo_src.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo_dst.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    cout << "GRAPH COPIED" <<endl;
    auto G_pass_bo = xrt::bo(device, sizeof(G_pass), increase_kernel.group_id(7));
    auto G_pass_ptr = G_pass_bo.map<graph_pass_struct*>();  // 映射到主机指针
    G_pass_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    // auto bo_gpass = xrt::bo(device, sizeof(graph_pass_struct), increase_kernel.group_id(7));
    // graph_pass_struct* gpass_ptr = bo_gpass.map<graph_pass_struct*>();
    // *gpass_ptr = G_pass; // 初始值

    run_increase.set_arg(0,vertex_num);
    run_increase.set_arg(1,edge_num);
    run_increase.set_arg(2, bo_src);
    run_increase.set_arg(3,bo_dst);
    run_increase.set_arg(4,bo_hbm_base_address);
    run_increase.set_arg(5,bo_valid_array);
    run_increase.set_arg(6,size);
    run_increase.set_arg(7, G_pass_bo);
    run_increase.set_arg(8,true);
    run_increase.start();
    run_increase.wait();
    cout << "CONSTRUCTED GRAPH" <<endl;
    G_pass_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    G_pass = *G_pass_ptr;

    auto bo_respond = xrt::bo(device, sizeof(int)*256, secure_kernel.group_id(5));
    int* respond_ptr = bo_respond.map<int*>();
    int command;
    do 
    {
        cout << "输入指令\n0: 退出\n1:图增量\n2:获取图信息" << endl;
        cin >> command;
        if (command == 1)
        {
            auto run_increase = xrt::run(increase_kernel);
            cout << "输入顶点数" << std::endl;
            int Vnum;
            cin >> Vnum;
            cout << "输入边数" << std::endl;
            int Enum;
            cin >> Enum;
            cout << "输入边" << std::endl;
            std::vector<int>tmp_src;
            std::vector<int>tmp_des;
            tmp_src.resize(2*Enum);
            tmp_des.resize(2*Enum);
            for (int i = 0 ; i < Enum ; i++)
            {
                int u,v;
                cin >> u >> v;
                tmp_src[i] = u;
                tmp_des[i] = v;
                tmp_src[i+Enum] = v;
                tmp_des[i+Enum] = u; 
            }
            memcpy(src_ptr, tmp_src.data(), sizeof(int) * 2*Enum);
            memcpy(dst_ptr, tmp_des.data(), sizeof(int) * 2*Enum);
            bo_src.sync(XCL_BO_SYNC_BO_TO_DEVICE);
            bo_dst.sync(XCL_BO_SYNC_BO_TO_DEVICE);
            run_increase.set_arg(0,Vnum);
            run_increase.set_arg(1,Enum*2);
            run_increase.set_arg(2, bo_src);
            run_increase.set_arg(3,bo_dst);
            run_increase.set_arg(4,bo_hbm_base_address);
            run_increase.set_arg(5,bo_valid_array);
            run_increase.set_arg(6,size);
            run_increase.set_arg(7,G_pass_bo);
            run_increase.set_arg(8,false);
            run_increase.start();
            run_increase.wait();
            G_pass_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
            G_pass = *G_pass_ptr;
            // bo_gpass.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
            // G_pass = *gpass_ptr;
            std::cout << "增量结束" <<std::endl;
        } 
        if (command == 2)
        {
            auto run_secure = xrt::run(secure_kernel);
            cout << "输入查询顶点号" << std::endl;
            int Vrank;
            cin >> Vrank;
            std::vector<int> respond(256,-1);
            
            run_secure.set_arg(0,bo_hbm_base_address);
            run_secure.set_arg(1,bo_valid_array);
            run_secure.set_arg(2,size);
            run_secure.set_arg(3, G_pass_bo);
            run_secure.set_arg(4,Vrank);
            run_secure.set_arg(5, bo_respond);
            bo_respond.sync(XCL_BO_SYNC_BO_TO_DEVICE);
            run_secure.start();
            run_secure.wait();
            bo_respond.sync(XCL_BO_SYNC_BO_FROM_DEVICE); 
            bo_respond.read(
                respond.data()
            );
            // bo_gpass.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
            // G_pass = *gpass_ptr;
            for (int i = 1 ; i <= respond[0] ; i++)
            {
                cout << respond[i] <<" ";
            }
            cout << "查询结束" << endl;
        }   
    }
    while(command != 0);
    return 0;
}