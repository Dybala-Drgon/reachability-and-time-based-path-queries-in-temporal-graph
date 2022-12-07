#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "common/vertex.hpp"
#include "common/resp.hpp"
#include "common/type.h"
#include "preprocessor/processor.hpp"
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;
clock_t start_time, end_time;
namespace TEngine {

    class Temporal_Engine {
    private:
        unordered_map<id_t, origin_vertex> origin_map;
        unordered_map<id_t, vector<vertex>> vertex_map_in;
        unordered_map<id_t, vector<vertex>> vertex_map_out;
        Processor processor;
    public:
        void load(std::string path);

        void transform(); // 把时态图变成static graph
        void dump();// dump出图的结构
        vector<resp_body> get_earliest_arrival_time(int id);

        vector<resp_body> get_latest_departure_time(int id);

        vector<resp_body> get_fastest_path_duration(int id);

        vector<resp_body> get_shortest_path_distance(int id);
    };

    // 加载文件数据,使用数据预处理器
    void Temporal_Engine::load(std::string path) {
        start_time = clock();
        processor.load(path, origin_map);
        end_time = clock();
        double endtime = (double) (end_time - start_time) / CLOCKS_PER_SEC;
        cout << "========= load done, time " << endtime * 1000 << " =========" << endl;
    }

    void Temporal_Engine::transform() {
        cout << "========= start transform, wait =========" << endl;
        start_time = clock();
        auto res = processor.transform(origin_map, vertex_map_in, vertex_map_out);
        end_time = clock();
        double endtime = (double) (end_time - start_time) / CLOCKS_PER_SEC;
        cout << "========= transform done, vertex sum: " << res << " time: " << endtime * 1000 << " =========" << endl;


//        for (auto it: vertex_map_in) {
//            cout << it.first << "== ";
//            for (auto t: it.second) {
//                auto &tt = t.getOutEdges();
//                int num = tt.size();
//                cout<<t<<": ";
//                for (int i = 1; i < num; i++) {
//                    cout<<" joint "<<vertex_map_out[tt[i].end_point].at(tt[i].idx)<<endl;
//                }
//                cout<<endl;
//            }
//            cout << endl;
//        }

    }
}


#endif