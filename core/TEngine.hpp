#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "common/vertex.hpp"
#include "common/resp.hpp"
#include "common/type.h"
#include "component/preprocessor.hpp"
#include "component/computation.hpp"
#include <vector>
#include <algorithm>
#include <ctime>
#include <sstream>

using namespace std;
clock_t start_time, end_time;
namespace TEngine {

    class Temporal_Engine {
    private:
        unordered_map<id_t, origin_vertex> origin_map;
        unordered_map<id_t, vector<vertex>> vertex_map_in;
        unordered_map<id_t, vector<vertex>> vertex_map_out;
        Processor processor;
        Computation computation;

        void show();// for debug
    public:
        void load(std::string path);

        void transform(); // 把时态图变成static graph
        void dump();// dump出图的结构
        /**
         *
         * @param id 查询的节点
         * @param lo 时间下限，闭区间
         * @param hi 时间上限 开区间
         * @return
         */
        resp_body get_earliest_arrival_time(id_t id, TEngine::ll lo, TEngine::ll hi);

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
        // TODO: 释放origin的空间
//        this->show();
    }

    resp_body Temporal_Engine::get_earliest_arrival_time(id_t id, TEngine::ll lo, TEngine::ll hi) {
        start_time = clock();
        context ctx;
        ctx.vertex_map_out = &vertex_map_out;
        ctx.vertex_map_in = &vertex_map_in;
        resp_body res;
        res.dist = computation.get_earliest_arrival_time(id, lo, hi, ctx);
        res.id = id;
        end_time = clock();
        double endtime = (double) (end_time - start_time) / CLOCKS_PER_SEC;
        cout << "========= get_earliest_arrival_time done, time: " << endtime * 1000 << " =========" << endl;
        return res;
    }

    // for debug
    void Temporal_Engine::show() {
        for (auto &it: origin_map) {
            stringstream ss;
            ss << "id: " << it.first << "\n";
            ss << "Tin列表: \n";
            auto &in_vec = vertex_map_in[it.first];
            for (auto &vertex: in_vec) {
                ss << "(" << vertex.getId() << "," << vertex.getT() << ")" << " ";
            }
            ss << "\n";

            for (auto &vertex: in_vec) {
                ss << "节点标识: " << vertex << "\n";
                for (auto edge: vertex.getOutEdges()) {
                    ss << edge << "\n";
                }
            }
            ss << "\n";
            ss << "Tout :\n";
            auto &out_vec = vertex_map_out[it.first];
            for (auto &vertex: out_vec) {
                ss << "(" << vertex.getId() << "," << vertex.getT() << ")" << " ";
            }
            ss << "\n";
            for (auto &vertex: out_vec) {
                ss << "节点标识: " << vertex << "\n";
                for (auto edge: vertex.getOutEdges()) {
                    ss << edge << "\n";
                }
            }
            cout << ss.str() << endl;
        }
    }

}


#endif