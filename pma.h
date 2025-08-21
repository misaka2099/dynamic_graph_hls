#pragma once
#include <cstdlib>
#include <utility>
#include "memory_pool_in_HBM.h"
#define SEGMENT_SIZE 64
#define DENSITY_BOUND 0.75
#define INIT_BLOCK_STORAGE_IN_BYTE 1024

template <typename T>
class myvector
{
    public:
    memory_block<T> storage;
    int capacity;
    int len;
    BYTE* pool;
    char* valid;
    int pool_size;
    myvector(BYTE* pool_ptr, char* valid_ptr, int pool_size)
        : pool(pool_ptr), valid(valid_ptr), pool_size(pool_size), storage(pool_ptr)
    {
        int addr;
        storage.len_in_byte = INIT_BLOCK_STORAGE_IN_BYTE;
        MemoryPool::allocate(pool,pool_size, valid,INIT_BLOCK_STORAGE_IN_BYTE, addr);
        storage.start_address = addr;
        len = 0;
        capacity = INIT_BLOCK_STORAGE_IN_BYTE / sizeof(T);
        // if (addr == -1)
        // {
        //     std::cout << "ERROR AT MEMORY_POOL"<<std::endl;
        // }
    }
    ~myvector()
    {
        MemoryPool::deallocate(valid, storage.start_address, storage.len_in_byte);
    }
    int size()
    {
        return len;
    }
    void push_back(T val)
    {
        if (len >= capacity)
        {
            enlarge();
        }
        storage[len] = val;
        len++;
    }
    //check index before calling []
    T& operator[](int idx)
    {
        return storage[idx];
    }
    bool empty()
    {
        return len == 0;
    }
    void pop_back()
    {
        if (len > 0)
        len--;
    }
    // void resize(int new_size_in_element, T element)
    // {
    //     pool.deallocate(storage.start_address, storage.len_in_byte);
    //     int addr;
    //     pool.allocate(new_size_in_element * sizeof(T), addr);
    //     storage.len_in_byte = new_size_in_element * sizeof(T);
    //     storage.start_address = addr;
    //     for (int i = 0 ; i < new_size_in_element ; i++)
    //     {
    //         storage[i] = element;
    //     }
    // }
    // void resize(int new_size_in_element)
    // {
    //     pool.deallocate(storage.start_address, storage.len_in_byte);
    //     int addr;
    //     pool.allocate(new_size_in_element * sizeof(T), addr);
    //     storage.len_in_byte = new_size_in_element * sizeof(T);
    //     storage.start_address = addr;
    // }
    T& back()
    {
        return storage[len-1];
    }
    void enlarge()
    {
        int addr;
        int new_size = storage.len_in_byte*2;
        MemoryPool::allocate(pool,pool_size,valid, new_size, addr);
        // std::cout << "ENLARGE " << "NEW ADDR RANGE IS " << addr << " --> " << addr + new_size << std::endl;
        if (addr == -1)
        {
            return;
        }
        // {
        //     for (int i = addr ; i < new_size + addr ; i++)
        //     {
        //         if (pool.valid[i] == false)
        //         {
        //             std::cout << "ALLOCATE ERROR" <<std::endl;
        //             abort();
        //         }
        //     }
        // }
        memory_block<T> new_block(pool);
        new_block.start_address = addr;
        new_block.len_in_byte = new_size;
        for (int i = 0 ; i < storage.len_in_byte / sizeof(T) ; i++)
        {
            #pragma HLS PIPELINE
            new_block[i] = storage[i];
        }
        MemoryPool::deallocate(valid,storage.start_address, storage.len_in_byte);
        storage.start_address = addr;
        storage.len_in_byte = new_size;
        capacity *= 2;
    }
    // easy iterator
    
    
};

class pma 
{
    BYTE* pool;
    char* valid;
    int pool_size;
    public:
    myvector<int> storage; 
    myvector<double> attributes_for_edge;
    myvector<char> exist;
    myvector<std::pair<int, int>> vertex_range;//<i,j> i_th segemnt to j_th segment [i,j)
    // int elements;
    double density_bound;
    int segment_size;

    pma(BYTE* pool,
    char* valid,
    int pool_size);
    pma(BYTE* pool,
    char* valid,
    int pool_size,int v);
    pma(const pma &) = default;
    pma(pma &&) = default;
    pma &operator=(const pma &) = default;
    pma &operator=(pma &&) = default;
    
    void insert_edge(int src, int des, double weight = 1);
    void insert_vertex();
    void remove_edge(int src, int des);//todo
    void remove_vertex(int src);//todo
    int size();
    bool edge_exist(int src, int des);
    void print();

    private:
    void insert_element_at(int des, double weight, int pos);
    void remove_element_at(int pos);//todo
    std::pair<int, int> find_best_fit_element(int src, int val);//<pos,val>
    void rebalance(int pos, int src);
    double get_density(int start, int end);
    void redistribute(int start, int end);
    
};


class graph 
{
    public:
    graph(BYTE* pool, char* valid, int pool_size);
    pma out_edge;
    pma in_edge;
    void insert_edge(int src, int des, double weight = 1);
    void insert_vertex();
    void remove_edge(int src, int des);//todo
    void remove_vertex(int src);//todo
    int size();
    bool edge_exist(int src, int des);
    void print();
    myvector<double> attributes_for_vertex;
};