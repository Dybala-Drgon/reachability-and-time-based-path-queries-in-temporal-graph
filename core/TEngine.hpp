#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "common/vertex.hpp"
#include "common/resp.hpp"
#include "common/type.h"
#include "preprocessor/processor.hpp"
#include <vector>
#include <algorithm>

using namespace std;
namespace TEngine {

    class Temporal_Engine {
    private:
        unordered_map<id_t,origin_vertex> origin_map;
        Processor processor;
    public:
        void load(std::string path);
        void dump();// dump出图的结构
        vector<resp_body> get_earliest_arrival_time(int id);
        vector<resp_body> get_latest_departure_time(int id);
        vector<resp_body> get_fastest_path_duration(int id);
        vector<resp_body> get_shortest_path_distance(int id);
    };

    // 加载文件数据,使用数据预处理器
    void Temporal_Engine::load(std::string path) {
        processor.load(path,origin_map);
    }
}


#endif