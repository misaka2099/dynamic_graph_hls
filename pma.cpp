#include "pma.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>



#define HERE std::cout << "文件: " << __FILE__ << ", 行: " << __LINE__ << std::endl;

//tobe updated to binary search
std::pair<int, int> pma::find_best_fit_element(int src, int val)//<pos,val>
{
    int pos = vertex_range[src].first * segment_size - 1;
    int res = -1;
    for (int i = vertex_range[src].first * segment_size ;
         i < vertex_range[src].second * segment_size ; i++)
        {
            if (exist[i] && storage[i] == val)
            { return std::pair<int, int>(i, val); }
            if (exist[i] && storage[i] < val)
            {
                pos = i;
                res = storage[i];
            }
            else if (exist[i] && storage[i] > val)
            { break; }
        }
    return std::pair<int, int>(pos, res);
}

bool pma::edge_exist(int src, int des)
{
    auto x = find_best_fit_element(src, des);
    if (x.second == des)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

void pma::insert_element_at(int des, double weight, int pos)
{
    if (exist[pos] == false)
    {
        exist[pos] = true;
        storage[pos] = des;
        attributes_for_edge[pos] = weight;
    }
    else 
    {
        for (int ptr = (int(pos / segment_size) + 1)*segment_size - 1; ptr >= pos ; ptr--)
        {
            if (exist[ptr])
            {
                exist[ptr+1] = true;
                storage[ptr+1] = storage[ptr];
                attributes_for_edge[ptr+1] = attributes_for_edge[ptr];
            }
        }
        exist[pos] = true;
        storage[pos] = des;
        attributes_for_edge[pos] = weight;
    }
}

// void pma::remove_element_at(int pos)
// {
//     exist[pos] = false;
//     elements--;
// }

void pma::insert_edge(int src, int des, double weight)
{
    // if (src == 2 && des == 1)
    // {
        // int idx = 0;
        // for (auto &x : vertex_range)
        // {
        //     std::cout << "vertex :" << idx << " from " << x.first << " to " << x.second<<std::endl; 
        //     idx++;
        // }
    // }
    auto x =  find_best_fit_element(src, des);



    if (x.second == des)
    {
        return;
    }
    int pos = x.first + 1;
    insert_element_at(des, weight, pos);
    rebalance(pos, src);
}

void pma::redistribute(int start, int end)//[start, end)
{
    myvector<int> des(pool, valid, pool_size);
    myvector<double> attr(pool, valid, pool_size); 
    int cur_seg_num = end - start;
    for (int i = start * segment_size; i < end * segment_size; ++i) 
    {
        if (exist[i]) {
            des.push_back(storage[i]);
            attr.push_back(attributes_for_edge[i]);
        }
        exist[i] = false;
    }
    int n = des.size();
    int base = n / cur_seg_num;
    int extra = n % cur_seg_num;
    int idx = 0;
    for (int s = 0; s < cur_seg_num; ++s) 
    {
        int seg_start = (start + s) * segment_size;
        int cnt = base + (s < extra ? 1 : 0); // 前extra个segment多放一个
        for (int i = 0; i < cnt; ++i) {
            storage[seg_start + i] = des[idx];
            attributes_for_edge[seg_start + i] = attr[idx];
            exist[seg_start + i] = true;
            idx++;
        }
        // 剩下的位置清空
        for (int i = cnt; i < segment_size; ++i) {
            exist[seg_start + i] = false;
        }
    }
    return;
}

void pma::rebalance(int pos, int src)
{
    int seg = pos / segment_size;
    int seg_start = vertex_range[src].first;
    int seg_end = vertex_range[src].second;
    int seg_count = seg_end - seg_start;
    int local_seg = seg - seg_start;
    int cur_seg_num = 1;

    while (cur_seg_num <= seg_count)
    {
        int left = (seg_start + int(local_seg / cur_seg_num) * cur_seg_num);
        int right = (seg_start + (int(local_seg / cur_seg_num) + 1) * cur_seg_num);
        double density = get_density(left, right);
        if (density <= density_bound)
        {
            redistribute(left, right);
            return;
        }
        cur_seg_num *= 2;
    }
    // myvector<int> to_insert_int;
    // myvector<bool> to_insert_bool;
    // to_insert_int.resize(segment_size*seg_count,0);
    // to_insert_bool.resize(segment_size*seg_count,false);
    for (int i = 0 ; i < seg_count*segment_size ; i++)
    {
        storage.push_back(0);
        attributes_for_edge.push_back(0);
        exist.push_back(false);
    }
    for (int i = storage.size() - 1  ; i >= seg_end*segment_size + seg_count*segment_size; i--)//从end往后空出来的空间向后移动
    {
        storage[i] = storage[i - seg_count*segment_size];
        attributes_for_edge[i] = attributes_for_edge[i - seg_count*segment_size];
        exist[i] = exist[i - seg_count*segment_size];
    }
    for (int i = 0 ; i < segment_size*seg_count ; i++)
    {
        storage[segment_size*(seg_start+seg_count)+i] = 0;
        attributes_for_edge[segment_size*(seg_start+seg_count)+i] = 0;
        exist[segment_size*(seg_start+seg_count)+i] = false;
    }
    // storage.insert(
    //     storage.begin()+seg_end*segment_size,
    //     to_insert_int.begin(),
    //     to_insert_int.end()
    // );
    // exist.insert(
    //     exist.begin()+seg_end*segment_size,
    //     to_insert_bool.begin(),
    //     to_insert_bool.end()
    // );

    vertex_range[src].second += seg_count;
    for (int i = src+1 ; i < vertex_range.size(); i++)
    {
        vertex_range[i].first += seg_count;
        vertex_range[i].second += seg_count;
    }
    
    redistribute(vertex_range[src].first, vertex_range[src].second);
}

double pma::get_density(int start, int end)//seg号[)
{
    int count = 0;
    for (int i = start * segment_size ; i < end * segment_size; i++)
    {
        if (exist[i])
        count++;
    }
    return double(count) / (segment_size * (end - start));
}
void pma::insert_vertex()
{
    // static int debug = 0;
    int new_first, new_second;
    if (vertex_range.empty()) {
        new_first = 0;
        new_second = 1;
    } else {
        new_first = vertex_range.back().second;
        new_second = new_first+1;
    }
    vertex_range.push_back(std::make_pair(new_first, new_second));
    // myvector<int> to_insert_int(segment_size, 0);
    // storage.insert(
    //     storage.end(),
    //     to_insert_int.begin(),
    //     to_insert_int.end()
    // );
    // myvector<bool> to_insert_bool(segment_size, false);
    // exist.insert(
    //     exist.end(),
    //     to_insert_bool.begin(),
    //     to_insert_bool.end()
    // );
    for (int i = 0 ; i < segment_size ; i++)
    {
        storage.push_back(0);
        attributes_for_edge.push_back(0);
        exist.push_back(false);
    }
    {
        
        // if (debug % 10000 == 0)
        // {
        //     std::cout << "---------------------" <<std::endl;
        //     for (int i = 0 ; i < vertex_range.size() ; i++)
        //     {
        //         std::cout << "VERTEX " << i << " from " << vertex_range[i].first << " to " << vertex_range[i].second << std::endl;
        //     }
        //     std::cout << "---------------------" <<std::endl;
        // }
        // debug+=1;
        if (vertex_range[0].first == vertex_range[0].second)
        {
            std::cout << vertex_range.size() << std::endl;
            abort();
        }
    }
}
pma::pma(
    BYTE* pool,
    char* valid,
    int pool_size)
    : storage(pool, valid, pool_size),
      attributes_for_edge(pool, valid, pool_size),
      exist(pool, valid, pool_size),
      vertex_range(pool, valid, pool_size)
{
    this->pool = pool;
    this->valid = valid;
    this->pool_size = pool_size;
    density_bound = DENSITY_BOUND;
    segment_size = SEGMENT_SIZE;
}
pma::pma(BYTE* pool,
    char* valid,
    int pool_size, int v)
    : storage(pool, valid, pool_size),
      attributes_for_edge(pool, valid, pool_size),
      exist(pool, valid, pool_size),
      vertex_range(pool, valid, pool_size)
{
    density_bound = DENSITY_BOUND;
    segment_size = SEGMENT_SIZE;
    for (int i = 0 ; i < v ; i++)
    {
        insert_vertex();
    }
}
// pma::pma(string filename)
// {
//     density_bound = DENSITY_BOUND;
//     segment_size = SEGMENT_SIZE;
//     std::fstream file;
//     file.open(filename);
//     if (!file.is_open())
//     {
//         exit(1);
//     }
//     std::vector<std::pair<int, int>> pairs;
//     int max_vertex = -1;;
//     int a, b;
//     while (file >> a >> b) {
//         pairs.emplace_back(a, b);
//         max_vertex = std::max(std::max(a,b),max_vertex);
//     }
//     file.close();
    
//     for (int i = 0 ; i <= max_vertex ; i++)
//     {
//         insert_vertex();
//     }
//     for (auto &p : pairs)
//     insert_edge(p.first, p.second);
// }
#ifdef __SYNTHESIS__
void pma::print()
{}
#else
void pma::print()
{
    for (int i = 0 ; i < vertex_range.size() ; i++)
    {
        for (int j = vertex_range[i].first * segment_size ; j < vertex_range[i].second * segment_size ; j++)
        {
            if (exist[j])
            std::cout << "src: "<< i << "   des: "<< storage[j] << std::endl;
        }
    }
}
#endif
// void pma::remove_edge(int src, int des)
// {
//     auto x =  find_best_fit_element(src, des);
//     assert(x.second == des);
//     remove_element_at(des);
// }


void graph::insert_edge(int src, int des, double weight)
{
    out_edge.insert_edge(src, des, weight);
    in_edge.insert_edge(des, src, weight);
}
void graph::insert_vertex()
{
    out_edge.insert_vertex();
    in_edge.insert_vertex();
    attributes_for_vertex.push_back(0);
}
#ifdef __SYNTHESIS__
void graph::print()
{}
#else
void graph::print()
{
    std::cout << "OUT EDGES: " << std::endl;
    out_edge.print();
    std::cout << "IN EDGES: " << std::endl;
    in_edge.print();
}
#endif
graph::graph(BYTE* pool, char* valid, int pool_size)
    : out_edge(pool, valid, pool_size),
      in_edge(pool, valid, pool_size),
      attributes_for_vertex(pool, valid, pool_size)
{}