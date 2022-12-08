#ifndef COMPUTATION_HPP
#define COMPUTATION_HPP

#include <unordered_map>
#include "common/resp.hpp"
#include "common/type.h"
#include "common/vertex.hpp"
#include "queue"
#include <algorithm>

namespace TEngine {
    using namespace std;

    class Computation {
    public:
        unordered_map<id_t, ll> get_earliest_arrival_time(id_t id, ll lo, ll hi, TEngine::context &ctx);
    };

    static int bsearch(ll time, vector<vertex> &vec);

    unordered_map<id_t, ll> Computation::get_earliest_arrival_time(id_t id, ll lo, ll hi, TEngine::context &ctx) {
        unordered_map<id_t, ll> res;
        // 使用bfs扫描即可
        queue<vertex> q;
        // 1. 使用二分找到第一个大于lo的时间out
        auto &out_map = ctx.vertex_map_out;
        auto &in_map = ctx.vertex_map_in;
        if (out_map->find(id) == out_map->end()) {
            // 找不到查询的节点，因此直接返回
            // TODO 抛出错误
            return res;
        }
        auto &start_vec = out_map->at(id);
        int idx = bsearch(lo, start_vec);
        if (idx == start_vec.size()) {
            // 超出，可以有化成提前判断
            return res;
        }
        q.push(start_vec[idx]);

        // 开始bfs
        while (!q.empty()) {
            vertex tmp = q.front();
            q.pop();
            if (tmp.getT() >= hi)continue;
            if (res.find(tmp.getId()) == res.end()){
                res[tmp.getId()] = tmp.getT();
            }
            auto key = tmp.getId();
            res[key] = min(res[key],tmp.getT());
            auto &out_vec = tmp.getOutEdges();
            if (out_vec[0].end_point != -1) {
                if (tmp.isOut()) {
                    q.push(out_map->at(tmp.getId()).at(out_vec[0].idx));
                } else {
                    q.push(in_map->at(tmp.getId()).at(out_vec[0].idx));
                }
            }
            int cnt = out_vec.size();
            for (int i = 1; i < cnt; i++) {
                Edge edg = out_vec[i];
                if (edg.end_point == tmp.getId()) {
                    // 肯定是in指向out
                    q.push(out_map->at(tmp.getId()).at(edg.idx));
                } else {
                    // 肯定是out指向in
                    q.push(in_map->at(edg.end_point).at(edg.idx));
                }
            }

        }

        return res;
    }

    static int bsearch(ll time, vector<vertex> &vec) {
        int lo = 0;
        int hi = vec.size();
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            ll mid_time = vec[mid].getT();
            if (mid_time == time) {
                return mid;
            } else if (mid_time < time) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        return lo;
    }
}

#endif