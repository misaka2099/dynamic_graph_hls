#pragma once
#include "hls_stream.h"
#include <ap_int.h>
#include "pma.h"
// void pma_top(hls::stream<ap_uint<32>>& cmd, 
//              hls::stream<ap_uint<32>>& src,
//              hls::stream<ap_uint<32>>& des,
//              hls::stream<double>& val,
//              hls::stream<ap_uint<32>>& cnt,
//              hls::stream<ap_uint<32>>& des_out,
//              hls::stream<double>& val_out,
//              hls::stream<ap_uint<32>>& cnt_out,
//              hls::stream<ap_uint<32>>& status
//             );
void pma_insert_edge(
    graph& G,
    hls::stream<ap_uint<32>> &cnt,
    hls::stream<ap_uint<32>> &src,
    hls::stream<ap_uint<32>> &des
);
void pma_insert_vertex(
    graph& G,
    hls::stream<ap_uint<32>> &cnt
);
void pma_get_out_edge(
    graph& G,
    hls::stream<ap_uint<32>> &src,
    hls::stream<ap_uint<32>> &des_out,
    hls::stream<ap_uint<32>> &cnt_out
);
void pma_get_in_edge(
    graph& G,
    hls::stream<ap_uint<32>> &src,
    hls::stream<ap_uint<32>> &des_out,
    hls::stream<ap_uint<32>> &cnt_out
);
void pma_change_attr(
    graph& G,
    hls::stream<ap_uint<32>> &src,
    hls::stream<double> &val,
    hls::stream<ap_uint<32>> &cnt
);
void pma_read_attr(
    graph& G,
    hls::stream<ap_uint<32>> &src,
    hls::stream<ap_uint<32>> &cnt,
    hls::stream<double> &val_out
);