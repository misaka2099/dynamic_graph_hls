#include "memory_pool_in_HBM.h"

void MemoryPool::init(char* valid, int MEMORY_POOL_SIZE_IN_BYTE)
{
    // #pragma HLS PIPELINE II=1
    for (int i = 0 ; i < MEMORY_POOL_SIZE_IN_BYTE; i++) 
    {
        // std::cout << "ITERATION " << i << std::endl;
        valid[i] = false; // 初始化VALID数组为false
    }
}
void MemoryPool::allocate(BYTE* storage, int MEMORY_POOL_SIZE_IN_BYTE, char* valid, int size_in_byte, int &res_start_addr)
{
    res_start_addr = -1;
    for (int i = 0 ; i < MEMORY_POOL_SIZE_IN_BYTE - size_in_byte ; i++)
    {
        if (valid[i] == false)
        {
            bool flag = true;
            for (int j = i ; j < i + size_in_byte ; j++)
            {
                if (valid[j] == true)
                {
                    flag = false;
                    i=j;
                    break;
                }
            }
            if (flag)
            {
                for (int j = i ; j < i + size_in_byte ; j++)
                {
                    valid[j] = true;
                }
                res_start_addr = i;
                // {
                //     std::cout << "POOL ALLOCATE " << "FROM "<< res_start_addr << "TO "
                //     << res_start_addr+size_in_byte <<std::endl;
                // }
                return;
            }
            
        }
    }
    // res.storage = nullptr;
    return;
}
void MemoryPool::deallocate(char* valid , int start_addr, int len_in_byte)
{
    for (int i = 0 ; i < len_in_byte ; i++)
    {
        valid[i+start_addr] = false;
    }
    // std::cout << "POOL DEALLOCATE FROM "<< start_addr << " TO "<< start_addr+len_in_byte <<std::endl;

}