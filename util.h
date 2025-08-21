#pragma once
#include <hls_stream.h>
#include "pma.h"
#include "data_type.h"
#define MAX_DEGREE 256
template<typename T>
T safe_read(hls::stream<T>& stream, T default_value, int timeout = 1000)
{
    int count = 0;
    while (stream.empty() && count < timeout) {
        #pragma HLS PIPELINE II=1
        count++;
        // std::cout << "safe_read timeout after " << timeout << " cycles" << std::endl;
    }
    if (!stream.empty()) {
        return stream.read();
    } else {
        // std::cout << "DEFALT VAL IS READ" << std::endl;
        return default_value;
    }
}   
template<typename T>
void clear_stream(hls::stream<T>& stream)
{
    while (!stream.empty()) {
        #pragma HLS PIPELINE II=1
        stream.read();
    }
}   

void graph_to_graph_pass(
    graph& G,
    graph_pass_struct& G_pass
);
void graph_pass_to_graph(
    graph_pass_struct& G_pass,
    graph& G
);