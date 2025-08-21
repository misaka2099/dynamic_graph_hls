#pragma once
#define BYTE unsigned char
struct MemoryPool {
    static void init(char* valid,int pool_size);
    static void allocate(unsigned char* pool, int pool_size, char* valid, int size, int& addr);
    static void deallocate(char* valid, int addr, int size);
};
template<typename T>
struct memory_block
{
    BYTE* pool;
    int len_in_byte;//length
    int start_address;
    T& operator[](int idx)//check index before this funtion
    {
        return *reinterpret_cast<T*>(&(pool[start_address + idx * sizeof(T)]));
    }
    // T& get_element(BYTE* pool, bool* valid ,int idx)//check index before this funtion
    // {
    //     #pragma HLS INLINE
    //     // if (idx < 0 || idx >= len_in_byte / sizeof(T)) {
    //     //     throw std::out_of_range("Index out of range");
    //     // }
    //     // if (!valid[start_address + idx * sizeof(T)]) {
    //     //     throw std::runtime_error("Memory not allocated at this index");
    //     // }
    //     return *reinterpret_cast<T*>(&(pool[start_address + idx * sizeof(T)]));
    // }
    memory_block(BYTE* pool, int len = -1, int start = -1)
    {
        this->pool = pool;
        len_in_byte = len;
        start_address = start;
    }
};