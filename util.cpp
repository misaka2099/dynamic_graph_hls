#include "data_type.h"
#include "pma.h"

// template <typename T>
// void mem_to_mem_pass(
//     memory_block<T>& mem,
//     memory_block_pass& mem_pass
// ) 
// {
//     mem_pass.len_in_byte = mem.len_in_byte;
//     mem_pass.start_address = mem.start_address;
// }
// template <typename T>
// void mem_pass_to_mem(
//     memory_block_pass& mem_pass,
//     memory_block<T>& mem
// ) 
// {
//     mem.len_in_byte = mem_pass.len_in_byte;
//     mem.start_address = mem_pass.start_address;
// }


// template <typename T>
// void vec_to_vec_pass(
//     myvector<T>& vec,
//     vec_pass& vec_pass
// ) 
// {
//     vec_pass.len = vec.len;
//     vec_pass.capacity = vec.capacity;
//     mem_to_mem_pass(vec.storage, vec_pass.storage);
// }
// template <typename T>
// void vec_pass_to_vec(
//     vec_pass& vec_pass,
//     myvector<T>& vec
// ) 
// {
//     vec.len = vec_pass.len;
//     vec.capacity = vec_pass.capacity;
//     mem_pass_to_mem(vec_pass.storage, vec.storage);
// }



// void pma_to_pma_pass(
//     pma& P,
//     pma_pass& P_pass
// ) 
// {   
//     vec_to_vec_pass(P.storage, P_pass.storage);
//     vec_to_vec_pass(P.attributes_for_edge, P_pass.attributes_for_edge);
//     vec_to_vec_pass(P.exist, P_pass.exist);
//     vec_to_vec_pass(P.vertex_range, P_pass.vertex_range);
//     P_pass.segment_size = P.segment_size;
//     P_pass.density_bound = P.density_bound;
// }
// void pma_pass_to_pma(
//     pma_pass& P_pass,
//     pma& P
// ) 
// {   
//     vec_pass_to_vec( P_pass.storage,P.storage);
//     vec_pass_to_vec(P_pass.attributes_for_edge,P.attributes_for_edge);
//     vec_pass_to_vec(P_pass.exist, P.exist);
//     vec_pass_to_vec( P_pass.vertex_range, P.vertex_range);
//     P.segment_size = P_pass.segment_size;
//     P.density_bound = P_pass.density_bound;
// }    




void graph_to_graph_pass(
    graph& G,
    graph_pass_struct& G_pass
) 
{
    // out_edge.storage
    G_pass.out_edge_storage_len = G.out_edge.storage.len;
    G_pass.out_edge_storage_capacity = G.out_edge.storage.capacity;
    G_pass.out_edge_storage_vec_len_in_byte = G.out_edge.storage.storage.len_in_byte;
    G_pass.out_edge_storage_vec_start_address = G.out_edge.storage.storage.start_address;
    
    // out_edge.attributes_for_edge
    G_pass.out_edge_attr_for_edge_len = G.out_edge.attributes_for_edge.len;
    G_pass.out_edge_attr_for_edge_capacity = G.out_edge.attributes_for_edge.capacity;
    G_pass.out_edge_attr_for_edge_vec_len_in_byte = G.out_edge.attributes_for_edge.storage.len_in_byte;
    G_pass.out_edge_attr_for_edge_vec_start_address = G.out_edge.attributes_for_edge.storage.start_address;
    
    // out_edge.exist
    G_pass.out_edge_exist_len = G.out_edge.exist.len;
    G_pass.out_edge_exist_capacity = G.out_edge.exist.capacity;
    G_pass.out_edge_exist_vec_len_in_byte = G.out_edge.exist.storage.len_in_byte;
    G_pass.out_edge_exist_vec_start_address = G.out_edge.exist.storage.start_address;
    
    // out_edge.vertex_range
    G_pass.out_edge_vertex_range_len = G.out_edge.vertex_range.len;
    G_pass.out_edge_vertex_range_capacity = G.out_edge.vertex_range.capacity;
    G_pass.out_edge_vertex_range_vec_len_in_byte = G.out_edge.vertex_range.storage.len_in_byte;
    G_pass.out_edge_vertex_range_vec_start_address = G.out_edge.vertex_range.storage.start_address;
    
    // out_edge metadata
    G_pass.out_edge_segment_size = G.out_edge.segment_size;
    G_pass.out_edge_density_bound = G.out_edge.density_bound;
    
    // in_edge.storage
    G_pass.in_edge_storage_len = G.in_edge.storage.len;
    G_pass.in_edge_storage_capacity = G.in_edge.storage.capacity;
    G_pass.in_edge_storage_vec_len_in_byte = G.in_edge.storage.storage.len_in_byte;
    G_pass.in_edge_storage_vec_start_address = G.in_edge.storage.storage.start_address;
    
    // in_edge.attributes_for_edge
    G_pass.in_edge_attr_for_edge_len = G.in_edge.attributes_for_edge.len;
    G_pass.in_edge_attr_for_edge_capacity = G.in_edge.attributes_for_edge.capacity;
    G_pass.in_edge_attr_for_edge_vec_len_in_byte = G.in_edge.attributes_for_edge.storage.len_in_byte;
    G_pass.in_edge_attr_for_edge_vec_start_address = G.in_edge.attributes_for_edge.storage.start_address;
    
    // in_edge.exist
    G_pass.in_edge_exist_len = G.in_edge.exist.len;
    G_pass.in_edge_exist_capacity = G.in_edge.exist.capacity;
    G_pass.in_edge_exist_vec_len_in_byte = G.in_edge.exist.storage.len_in_byte;
    G_pass.in_edge_exist_vec_start_address = G.in_edge.exist.storage.start_address;
    
    // in_edge.vertex_range
    G_pass.in_edge_vertex_range_len = G.in_edge.vertex_range.len;
    G_pass.in_edge_vertex_range_capacity = G.in_edge.vertex_range.capacity;
    G_pass.in_edge_vertex_range_vec_len_in_byte = G.in_edge.vertex_range.storage.len_in_byte;
    G_pass.in_edge_vertex_range_vec_start_address = G.in_edge.vertex_range.storage.start_address;
    
    // in_edge metadata
    G_pass.in_edge_segment_size = G.in_edge.segment_size;
    G_pass.in_edge_density_bound = G.in_edge.density_bound;
    
    // attributes_for_vertex
    G_pass.attributes_for_vertex_len = G.attributes_for_vertex.len;
    G_pass.attributes_for_vertex_capacity = G.attributes_for_vertex.capacity;
    G_pass.attributes_for_vertex_vec_len_in_byte = G.attributes_for_vertex.storage.len_in_byte;
    G_pass.attributes_for_vertex_vec_start_address = G.attributes_for_vertex.storage.start_address;
}
void graph_pass_to_graph(
    graph_pass_struct& G_pass,
    graph& G
) 
{
    // out_edge.storage
    G.out_edge.storage.len = G_pass.out_edge_storage_len;
    G.out_edge.storage.capacity = G_pass.out_edge_storage_capacity;
    G.out_edge.storage.storage.len_in_byte = G_pass.out_edge_storage_vec_len_in_byte;
    G.out_edge.storage.storage.start_address = G_pass.out_edge_storage_vec_start_address;
    
    // out_edge.attributes_for_edge
    G.out_edge.attributes_for_edge.len = G_pass.out_edge_attr_for_edge_len;
    G.out_edge.attributes_for_edge.capacity = G_pass.out_edge_attr_for_edge_capacity;
    G.out_edge.attributes_for_edge.storage.len_in_byte = G_pass.out_edge_attr_for_edge_vec_len_in_byte;
    G.out_edge.attributes_for_edge.storage.start_address = G_pass.out_edge_attr_for_edge_vec_start_address;
    
    // out_edge.exist
    G.out_edge.exist.len = G_pass.out_edge_exist_len;
    G.out_edge.exist.capacity = G_pass.out_edge_exist_capacity;
    G.out_edge.exist.storage.len_in_byte = G_pass.out_edge_exist_vec_len_in_byte;
    G.out_edge.exist.storage.start_address = G_pass.out_edge_exist_vec_start_address;
    
    // out_edge.vertex_range
    G.out_edge.vertex_range.len = G_pass.out_edge_vertex_range_len;
    G.out_edge.vertex_range.capacity = G_pass.out_edge_vertex_range_capacity;
    G.out_edge.vertex_range.storage.len_in_byte = G_pass.out_edge_vertex_range_vec_len_in_byte;
    G.out_edge.vertex_range.storage.start_address = G_pass.out_edge_vertex_range_vec_start_address;
    
    // out_edge metadata
    G.out_edge.segment_size = G_pass.out_edge_segment_size;
    G.out_edge.density_bound = G_pass.out_edge_density_bound;
    
    // in_edge.storage
    G.in_edge.storage.len = G_pass.in_edge_storage_len;
    G.in_edge.storage.capacity = G_pass.in_edge_storage_capacity;
    G.in_edge.storage.storage.len_in_byte = G_pass.in_edge_storage_vec_len_in_byte;
    G.in_edge.storage.storage.start_address = G_pass.in_edge_storage_vec_start_address;
    
    // in_edge.attributes_for_edge
    G.in_edge.attributes_for_edge.len = G_pass.in_edge_attr_for_edge_len;
    G.in_edge.attributes_for_edge.capacity = G_pass.in_edge_attr_for_edge_capacity;
    G.in_edge.attributes_for_edge.storage.len_in_byte = G_pass.in_edge_attr_for_edge_vec_len_in_byte;
    G.in_edge.attributes_for_edge.storage.start_address = G_pass.in_edge_attr_for_edge_vec_start_address;
    
    // in_edge.exist
    G.in_edge.exist.len = G_pass.in_edge_exist_len;
    G.in_edge.exist.capacity = G_pass.in_edge_exist_capacity;
    G.in_edge.exist.storage.len_in_byte = G_pass.in_edge_exist_vec_len_in_byte;
    G.in_edge.exist.storage.start_address = G_pass.in_edge_exist_vec_start_address;
    
    // in_edge.vertex_range
    G.in_edge.vertex_range.len = G_pass.in_edge_vertex_range_len;
    G.in_edge.vertex_range.capacity = G_pass.in_edge_vertex_range_capacity;
    G.in_edge.vertex_range.storage.len_in_byte = G_pass.in_edge_vertex_range_vec_len_in_byte;
    G.in_edge.vertex_range.storage.start_address = G_pass.in_edge_vertex_range_vec_start_address;
    
    // in_edge metadata
    G.in_edge.segment_size = G_pass.in_edge_segment_size;
    G.in_edge.density_bound = G_pass.in_edge_density_bound;
    
    // attributes_for_vertex
    G.attributes_for_vertex.len = G_pass.attributes_for_vertex_len;
    G.attributes_for_vertex.capacity = G_pass.attributes_for_vertex_capacity;
    G.attributes_for_vertex.storage.len_in_byte = G_pass.attributes_for_vertex_vec_len_in_byte;
    G.attributes_for_vertex.storage.start_address = G_pass.attributes_for_vertex_vec_start_address;
}