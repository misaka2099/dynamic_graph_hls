#include "hls_stream.h"
#include "util.h"
#include "data_type.h"
#include "pma.h"
#include "pma_top.h"
extern "C"
{
    void secure_graph(
        unsigned char* hbm_base_address,
        char* valid_array,
        int pool_size,
        graph_pass_struct* G_pass,
        int vertex_rank,
        int *check_result
    )
    {
        #pragma HLS INTERFACE m_axi port=hbm_base_address offset=slave bundle=gmem0
        #pragma HLS INTERFACE m_axi port=valid_array offset=slave bundle=gmem1
        #pragma HLS INTERFACE m_axi port=check_result offset=slave bundle=gmem4
        #pragma HLS INTERFACE m_axi  port=G_pass offset=slave bundle=gmem5
        #pragma HLS INTERFACE s_axilite port=pool_size 
        #pragma HLS INTERFACE s_axilite port=vertex_rank 
        #pragma HLS INTERFACE s_axilite port=return
        graph G(hbm_base_address, valid_array, pool_size);
        graph_pass_to_graph(*G_pass,G);
        hls::stream<ap_uint<32>> src;
        //max ourdegree = 256
        hls::stream<ap_uint<32>,MAX_DEGREE> des_out;
        hls::stream<ap_uint<32>> cnt;
        src.write(vertex_rank);
        pma_get_out_edge(G, src, des_out, cnt);
        int out_degree = cnt.read();
        check_result[0] = out_degree;
        #pragma HLS PIPELINE
        for (int i = 0; i < out_degree; i++)
        {
            ap_uint<32> des = des_out.read();
            check_result[i + 1] = des.to_int();
        }
    }
}