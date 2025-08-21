#include "hls_stream.h"
#include "pma.h"
#include "pma_top.h"
#include <iostream>
#include "util.h"
/*
cmd: 1: add_edge src, des, val=1, cnt 
     2: add_vertex , src, cnt
     3: get src neighbor des_out, val_out

*/
//adding weighted edge is a todo



// #define HERE std::cout << "文件: " << __FILE__ << ", 行: " << __LINE__ << std::endl;

// void pma_top(hls::stream<ap_uint<32>>& CMD, 
//              hls::stream<ap_uint<32>>& SRC,
//              hls::stream<ap_uint<32>>& DES,
//              hls::stream<double>& VAL,
//              hls::stream<ap_uint<32>>& CNT,
//              hls::stream<ap_uint<32>>& DES_OUT,
//              hls::stream<double>& VAL_OUT,
//              hls::stream<ap_uint<32>>& CNT_OUT,
//              hls::stream<ap_uint<32>>& STATUS
//             )
// {
//     static graph G;
//     int command = CMD.read();
//     switch (command) 
//     {
//         case INSERT_EDGE:
//             {
//                 int count = CNT.read();
//                 for (int i = 0 ; i < count ; i++)
//                 {
//                     int u = SRC.read();
//                     int v = DES.read();
//                     // int weight = val.read();
//                     G.insert_edge(u,v);
//                 }
//                 STATUS.write(0);
//                 break;
//             }
//         case INSERT_VERTEX:
//             {
//                 int count = CNT.read();
//                 for (int i = 0 ; i < count ; i++)
//                     G.insert_vertex();
//                 STATUS.write(0);
//                 // {
//                 //     static int vertex_cnt;
//                 //     vertex_cnt+=count;
//                 //     std::cout << "--------------------------------------" << std::endl;
//                 //     for(int i = 0 ; i < vertex_cnt ; i++)
//                 //     {
//                 //         std::cout << "VERTEX " << i << " from "<<  G.out_edge.vertex_range[i].first << " to " << G.out_edge.vertex_range[i].second << std::endl;    
//                 //     }
//                 //     std::cout << "--------------------------------------" << std::endl;
//                 // }
//                 break;
//             }
//         case GET_OUT_EDGE:
//             {
//                 // G.print();
//                 int res_cnt = 0;
//                 int u = SRC.read();
//                 for (int i = G.out_edge.vertex_range[u].first * G.out_edge.segment_size ; i < G.out_edge.vertex_range[u].second * G.out_edge.segment_size ; i++)
//                 {
//                     if (G.out_edge.exist[i])
//                     {
//                         res_cnt++;
//                         DES_OUT.write(G.out_edge.storage[i]);
//                     }
//                 }
//                 CNT_OUT.write(res_cnt);
//                 STATUS.write(0);
//                 break;
//             }
//         case GET_IN_EDGE:
//             {
//                 // G.print();
//                 int res_cnt = 0;
//                 int u = SRC.read();
//                 for (int i = G.in_edge.vertex_range[u].first * G.in_edge.segment_size ; i < G.in_edge.vertex_range[u].second * G.in_edge.segment_size ; i++)
//                 {
//                     if (G.in_edge.exist[i])
//                     {
//                         res_cnt++;
//                         DES_OUT.write(G.in_edge.storage[i]);
//                     }
//                 }
//                 CNT_OUT.write(res_cnt);
//                 STATUS.write(0);
//                 break;
//             }
        
//         case CHANGE_ATTR: 
//             {
//                 int cnt = 0;
//                 cnt = CNT.read(); 
//                 for (int i = 0 ; i < cnt ; i++)
//                 {
//                     double x = VAL.read();
//                     int src = SRC.read();
//                     G.attributes_for_vertex[src] = x;
//                 }
//                 STATUS.write(0);
//                 break;
//             }
//         case READ_ATTR:
//             {
//                 int cnt = 0;
//                 cnt = CNT.read();
//                 for (int i = 0 ; i < cnt ; i++)
//                 {
//                     int src = SRC.read();
//                     VAL_OUT.write(G.attributes_for_vertex[src]);
//                 }
//                 STATUS.write(0);
//                 break;
//             }
//         default:
//             STATUS.write(1);
//             break;
//     }
// }




void check_graph(graph &G, int num_vertex)
{
    hls::stream<ap_uint<32>> src;
    hls::stream<ap_uint<32>> des_out;
    hls::stream<ap_uint<32>> cnt_out;
    for (int i = 0 ; i < num_vertex ; i++)
    {
        src.write(i);
        pma_get_out_edge(G, src, des_out, cnt_out);
        int cnt = cnt_out.read();
        std::cout << "VERTEX: " << i << " " << cnt << " NEIS " <<std::endl;
        for (int j = 0 ; j < cnt ; j++)
        {
            std::cout << des_out.read() << " ";
        } 
        std::cout << std::endl;
    }
}

void pma_insert_edge(
    graph& G,
    hls::stream<ap_uint<32>> &cnt,
    hls::stream<ap_uint<32>> &src,
    hls::stream<ap_uint<32>> &des
) 
{
    // int count = cnt.read();
    int count = safe_read<ap_uint<32>>(cnt, 0, 1000);
    for (int i = 0 ; i < count ; i++)
    {
        int u = src.read();
        int v = des.read();
        // int weight = val.read();
        G.insert_edge(u,v);
        std::cout << "INSERTED " << u << " " << v <<std::endl;
        // check_graph(G, 32);
    }
}
void pma_insert_vertex(
    graph& G,
    hls::stream<ap_uint<32>> &cnt
)
{
    // int count = cnt.read();
    int count = safe_read<ap_uint<32>>(cnt, 0, 1000);
    for (int i = 0 ; i < count ; i++)
        G.insert_vertex();
}

void pma_get_out_edge(
    graph& G,
    hls::stream<ap_uint<32>> &src,
    hls::stream<ap_uint<32>> &des_out,
    hls::stream<ap_uint<32>> &cnt_out    
)
{
    // G.print();
    int res_cnt = 0;
    // int u = src.read();
    int u = safe_read<ap_uint<32>>(src, -1, 1000);
    if (u == -1)
    {
        // std::cout << "[DBG] ERROR NO SRC" <<std::endl;
        return;
    }
    // std::cout << "[DBG] VERTEX RANGE FROM " << G.out_edge.vertex_range[u].first << " TO " <<G.out_edge.vertex_range[u].second << std::endl;
    for (int i = G.out_edge.vertex_range[u].first * G.out_edge.segment_size ; i < G.out_edge.vertex_range[u].second * G.out_edge.segment_size ; i++)
    {
        if (G.out_edge.exist[i])
        {
            res_cnt++;
            des_out.write(G.out_edge.storage[i]);
        }
    }
    cnt_out.write(res_cnt);
}

void pma_get_in_edge(
    graph& G,
    hls::stream<ap_uint<32>> &src,
    hls::stream<ap_uint<32>> &des_out,
    hls::stream<ap_uint<32>> &cnt_out
)
{
    // G.print();
    int res_cnt = 0;
    // int u = src.read();
    int u = safe_read<ap_uint<32>>(src, -1, 1000);
    for (int i = G.in_edge.vertex_range[u].first * G.in_edge.segment_size ; i < G.in_edge.vertex_range[u].second * G.in_edge.segment_size ; i++)
    {
        if (G.in_edge.exist[i])
        {
            res_cnt++;
            des_out.write(G.in_edge.storage[i]);
        }
    }
    if (res_cnt == 0)
    {
        des_out.write(-1);
    }
    cnt_out.write(res_cnt);
}
void pma_change_attr(
    graph& G,
    hls::stream<ap_uint<32>> &src,
    hls::stream<double> &val,
    hls::stream<ap_uint<32>> &cnt
)
{
    // int cnt_num = cnt.read();
    int cnt_num = safe_read<ap_uint<32>>(cnt, 0, 1000);
    for (int i = 0 ; i < cnt_num ; i++)
    {
        // double x = val.read();
        double x = safe_read<double>(val, 0.0, 1000);
        // int src_num = src.read();
        int src_num = safe_read<ap_uint<32>>(src, -1, 1000);
        G.attributes_for_vertex[src_num] = x;
    }
}

void pma_read_attr(
    graph& G,
    hls::stream<ap_uint<32>> &src,
    hls::stream<ap_uint<32>> &cnt,
    hls::stream<double> &val_out
)
{
    // int cnt_num = cnt.read();
    int cnt_num = safe_read<ap_uint<32>>(cnt, 0, 1000);
    for (int i = 0 ; i < cnt_num ; i++)
    {
        // int src_num = src.read();
        int src_num = safe_read<ap_uint<32>>(src, -1, 1000);
        val_out.write(G.attributes_for_vertex[src_num]);
    }
}  
