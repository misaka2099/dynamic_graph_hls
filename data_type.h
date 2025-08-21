#pragma once
struct memory_block_pass
{
    int len_in_byte;//length
    int start_address;
};
struct vec_pass
{
    int len;
    int capacity;
    int vec_len_in_byte;
    int vec_start_address;
};
struct pma_pass 
{
    // vec_pass storage;
    int storage_len;
    int storage_capacity;
    int storage_vec_len_in_byte;
    int storage_vec_start_address;
    int attr_for_edge_len;
    int attr_for_edge_capacity;
    int attr_for_edge_vec_len_in_byte;
    int attr_for_edge_vec_start_address;
    int exist_len;
    int exist_capacity;
    int exist_vec_len_in_byte;
    int exist_vec_start_address;
    int vertex_range_len;
    int vertex_range_capacity;
    int vertex_range_vec_len_in_byte;
    int vertex_range_vec_start_address;
    // vec_pass attributes_for_edge;
    // vec_pass exist;
    // vec_pass vertex_range;
    int segment_size;
    double density_bound;
};
struct graph_pass_struct
{
    //--out_edge--608bits
    int out_edge_storage_len;
    int out_edge_storage_capacity;
    int out_edge_storage_vec_len_in_byte;
    int out_edge_storage_vec_start_address;
    int out_edge_attr_for_edge_len;
    int out_edge_attr_for_edge_capacity;
    int out_edge_attr_for_edge_vec_len_in_byte;
    int out_edge_attr_for_edge_vec_start_address;
    int out_edge_exist_len;
    int out_edge_exist_capacity;
    int out_edge_exist_vec_len_in_byte;
    int out_edge_exist_vec_start_address;
    int out_edge_vertex_range_len;
    int out_edge_vertex_range_capacity;
    int out_edge_vertex_range_vec_len_in_byte;
    int out_edge_vertex_range_vec_start_address;
    int out_edge_segment_size;
    double out_edge_density_bound;

    //--in_edge--608bits
    int in_edge_storage_len;
    int in_edge_storage_capacity;
    int in_edge_storage_vec_len_in_byte;
    int in_edge_storage_vec_start_address;
    int in_edge_attr_for_edge_len;
    int in_edge_attr_for_edge_capacity;
    int in_edge_attr_for_edge_vec_len_in_byte;
    int in_edge_attr_for_edge_vec_start_address;
    int in_edge_exist_len;
    int in_edge_exist_capacity;
    int in_edge_exist_vec_len_in_byte;
    int in_edge_exist_vec_start_address;
    int in_edge_vertex_range_len;
    int in_edge_vertex_range_capacity;
    int in_edge_vertex_range_vec_len_in_byte;
    int in_edge_vertex_range_vec_start_address;
    int in_edge_segment_size;
    double in_edge_density_bound;

    //--attr_for_vertex 128 bits
    int attributes_for_vertex_len;
    int attributes_for_vertex_capacity;
    int attributes_for_vertex_vec_len_in_byte;
    int attributes_for_vertex_vec_start_address;
    // pma_pass out_edge;
    // pma_pass in_edge;
    // vec_pass attributes_for_vertex;
};