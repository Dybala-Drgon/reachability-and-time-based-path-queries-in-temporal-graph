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
    };

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
                if (first.id2 == second.id2) {
                    return first.t < second.t;
                } else {
                    return first.id2 < second.id2;
                }
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
            Edge edge(t.id2,t.t,t.lambda);
            map[t.id1].add_edge(edge);
        }

        auto one = map[1];
        for(auto t:one.getEdges()){
            cout<<t<<endl;
        }
    }
}

#endif