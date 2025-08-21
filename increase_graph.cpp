#include "data_type.h"
#include "hls_stream.h"
#include "pma.h"
#include "pma_top.h"
#include "util.h"
extern "C"
{
    void increase_graph(
        int vertex_num,
        int edge_num,
        const int* src,
        const int* dst,
        unsigned char* hbm_base_address,
        char* valid_array,
        int pool_size,
        graph_pass_struct* G_pass,
        bool first
    )
    { 
        #pragma HLS INTERFACE m_axi port=hbm_base_address offset=slave bundle=gmem0
        #pragma HLS INTERFACE m_axi port=src offset=slave bundle=gmem2
        #pragma HLS INTERFACE m_axi port=dst offset=slave bundle=gmem3
        #pragma HLS INTERFACE m_axi port=valid_array offset=slave bundle=gmem1

        #pragma HLS INTERFACE m_axi port=G_pass offset=slave bundle=gmem5
        #pragma HLS INTERFACE s_axilite port=first
        #pragma HLS INTERFACE s_axilite port=pool_size
        #pragma HLS INTERFACE s_axilite port=edge_num
        #pragma HLS INTERFACE s_axilite port=vertex_num
        #pragma HLS INTERFACE s_axilite port=return
        graph G(
            hbm_base_address,
            valid_array,
            pool_size
        );
        if (!first)
        {
            graph_pass_to_graph(*G_pass, G);
        }
        if (vertex_num > 0)
        {
            hls::stream<ap_uint<32>> cnt;
            cnt.write(vertex_num);
            pma_insert_vertex(
                G,
                cnt
            );
        }
        if (edge_num > 0)
        {
            hls::stream<ap_uint<32>> stream_src;
            hls::stream<ap_uint<32>> stream_des;
            hls::stream<ap_uint<32>> stream_cnt;
            for (int i = 0; i < edge_num; ++i)
            {
                stream_cnt.write(1);
                stream_src.write(src[i]);
                stream_des.write(dst[i]);
                pma_insert_edge(
                G,
                stream_cnt,
                stream_src,
                stream_des
            );
            }
        }
        if (edge_num > 0 || vertex_num > 0)
        {
            graph_to_graph_pass(G, *G_pass);
        }
    }
    
}