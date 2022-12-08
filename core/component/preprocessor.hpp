#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <set>


using namespace std;
namespace TEngine {
    class Processor {

    public:
        void load(std::string path, unordered_map <id_t, origin_vertex> &map);

        ll transform(unordered_map <id_t, origin_vertex> &origin, unordered_map <id_t, vector<vertex>> &in_res,
                     unordered_map <id_t, vector<vertex>> &out_res);
    };

    static vector <vertex> build_in_vertex(id_t id, vector <Edge> &vec);

    static int bsearch(int t, vector <vertex> &vec);

    static vector <vertex> build_out_vertex(id_t id, vector <Edge> &vec);

    void Processor::load(std::string path, unordered_map <id_t, origin_vertex> &map) {
        // 1. read
        struct read_obj {
            ll id1;
            ll id2;
            ll t;
            int lambda;

            read_obj(ll id1, ll id2, ll t, int lambda) : id1(id1), id2(id2), t(t), lambda(lambda) {}

            read_obj() {}

            bool operator<(const read_obj &other) const {
                if (id1 == other.id1 && id2 == other.id2 && t == other.t && lambda == other.lambda) {
                    return false;
                }
                return true;
            }
        };

        vector<read_obj> vec;
        ifstream ifs(path);
        if (!ifs.is_open()) {
            cerr << "========= load data failed =========" << endl;
            exit(1);
        }
        set<read_obj> ss;
        read_obj obj;
        while (ifs >> obj.id1 >> obj.id2 >> obj.t >> obj.lambda) {
            ss.emplace(obj);
        }
        ifs.close();
        // 去重，必做
        // 2. resort
        auto cmp = [](read_obj &first, read_obj &second) {
            if (first.id1 == second.id1) {
                return first.t < second.t;
            } else {
                return first.id1 < second.id1;
            }
        };
        vec.assign(ss.begin(), ss.end());
        sort(vec.begin(), vec.end(), cmp);


        for (auto &t: vec) {
            if (map.find(t.id1) == map.end()) {
                origin_vertex tmp;
                tmp.setId(t.id1);
                map[t.id1] = tmp;
            }

            Edge edge(t.id2, t.t, t.lambda);
            map[t.id1].add_out_edge(edge);
            // 入边也要存储
            if (map.find(t.id2) == map.end()) {
                origin_vertex tmp;
                tmp.setId(t.id2);
                map[t.id2] = tmp;
            }
            edge.end_point = t.id1;
            map[t.id2].add_in_edge(edge);
        }
    }

    /**
     * 将时态图转换成静态图，由此来计算可达性和路径问题
     * 返回新构造的节点的数量
     */
    ll Processor::transform(unordered_map <TEngine::id_t, TEngine::origin_vertex> &origin,
                            unordered_map <TEngine::id_t, vector<TEngine::vertex>> &in_res,
                            unordered_map <TEngine::id_t, vector<vertex>> &out_res) {
        ll cnt = 0;
        auto in_edge_cmp = [](Edge &first, Edge &second) -> bool {
            return first.get_end_time() < second.get_end_time();
        };

        for (auto &it: origin) {
            // 以in_edge的end_time创建一个节点
            auto &in_vec = it.second.getInEdges();
            sort(in_vec.begin(), in_vec.end(), in_edge_cmp);
            // 根据inedge创建static 节点
            in_res[it.first] = build_in_vertex(it.first, in_vec);
            cnt += in_res[it.first].size();

            auto &out_vec = it.second.getOutEdges();
            out_res[it.first] = build_out_vertex(it.first, out_vec);
            cnt += out_res[it.first].size();
        }
        //================节点构造好了，开始连接=============
        // 由于可能需要反向bfs，所以同时记录incoming和outgoing
        // 两个map中的vector都根据time排好序了，可以使用二分查找

        // 1. 遍历in vertex,建立索引关系
        for (auto &it: in_res) {
            auto &vec = it.second;
            if (vec.empty())continue;
            for (int i = 0; i < vec.size() - 1; i++) {
                Edge tmp;
                tmp.end_point = it.first;
                tmp.idx = i + 1;
                vec[i].add_out_edge(tmp);
                vec[i].setIsOut(false);
            }
            Edge dum;
            dum.end_point = -1;
            dum.idx = -1;
            vec[vec.size() - 1].add_out_edge(dum);
        }


        // 2. 遍历out vertex,建立索引关系
        for (auto &it: out_res) {
            auto &vec = it.second;
            if (vec.empty())continue;
            for (int i = 0; i < vec.size() - 1; i++) {
                Edge tmp;
                tmp.end_point = it.first;
                tmp.idx = i + 1;
                vec[i].add_out_edge(tmp);
                vec[i].setIsOut(true);
            }
            Edge dum;
            dum.end_point = -1;
            dum.idx = -1;
            vec[vec.size() - 1].add_out_edge(dum);
        }


        // 3. in和out之间建立连接,可以合并到第一步
        for (auto &it: in_res) {
            id_t id = it.first;
            auto &out_vec = out_res[id];
            auto &in_vec = in_res[id];
            int last = -1;
            for (int i = in_vec.size() - 1; i >= 0; i--) {
                auto tmp = in_vec[i];
                int idx = bsearch(tmp.getT(), out_vec);

                if (idx >= out_vec.size() || idx == last)continue;
                Edge edg;
                edg.end_point = id;
                edg.idx = idx;
                edg.t=out_vec[idx].getT();
                in_vec[i].add_out_edge(edg);

                last = idx;
                if (last == 0) break;
            }
        }

        // 4. 不同节点之间的连接
        for (auto &it: origin) {
            id_t id = it.first;
            origin_vertex originVertex = it.second;
            auto &out_vec = out_res[id];
            if (out_vec.empty())continue;
            for (auto &edge: originVertex.getOutEdges()) {
                // 先找到对应的out_edge索引
                int idx = bsearch(edge.t, out_vec);
                if (out_vec[idx].getT() != edge.t) {
                    cerr << "out 节点不存在" << endl;
                    exit(2);
                }

                // 找到连接的in_edge索引
                auto &in_vec = in_res[edge.end_point];
                int in_idx = bsearch(edge.get_end_time(), in_vec);
                if (in_vec[in_idx].getT() != edge.get_end_time() || in_vec[in_idx].getId()!=edge.end_point){
                    cerr<<"in 节点不存在"<<endl;
                    exit(2);
                }
                Edge tmp(edge);
                tmp.idx = in_idx;
                out_vec[idx].add_out_edge(tmp);
            }
        }

        return cnt;
    }

    static int bsearch(int t, vector <vertex> &vec) {
        int lo = 0;
        int hi = vec.size() - 1;
        while (lo <= hi) {
            int mid = (hi + lo) / 2;
            if (vec[mid].getT() == t)return mid;
            if (vec[mid].getT() < t) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        return lo;
    }

    static vector <vertex> build_out_vertex(id_t id, vector <Edge> &vec) {
        vector<vertex> res;
        if (vec.empty()) {
            return res;
        }
        vertex tmp;
        tmp.setId(id);
        tmp.setT(vec[0].t);
        res.push_back(tmp);
        if (vec.size() == 1)return res;
        for (int i = 1; i < vec.size(); i++) {
            if (vec[i].t != vec[i - 1].t) {
                tmp.setT(vec[i].t);
                res.push_back(tmp);
            }
        }
        return res;
    }

    static vector <vertex> build_in_vertex(id_t id, vector <Edge> &vec) {
        vector<vertex> res;
        if (vec.empty())return res;

        vertex tmp;
        tmp.setId(id);
        tmp.setT(vec[0].get_end_time());
        res.push_back(tmp);

        if (vec.size() == 1) {
            return res;
        }
        for (int i = 1; i < vec.size(); i++) {
            if (vec[i].get_end_time() != vec[i - 1].get_end_time()) {
                tmp.setT(vec[i].get_end_time());
                res.push_back(tmp);
            }
        }
        return res;
    }
}

#endif