#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <vector>
#include <ostream>

using ll = long long;
namespace TEngine {
    struct Edge {
        ll end_point;// 对端节点
        double lambda; // λ值
        int t; // 对应论文中的时间,尽在origin_vertex中有意义
        Edge() {
            end_point = 0;
            lambda = 0;
            t = 0;
        }

        Edge(ll endPoint, double lambda, int t) : end_point(endPoint), lambda(lambda), t(t) {}

        // paper中提出，仅当所有member都相同时才可以说相等
        bool operator==(const Edge &rhs) const {
            return end_point == rhs.end_point &&
                   lambda == rhs.lambda &&
                   t == rhs.t;
        }

        bool operator!=(const Edge &rhs) const {
            return !(rhs == *this);
        }

        friend std::ostream &operator<<(std::ostream &os, const Edge &edge) {
            os << "end_point: " << edge.end_point << " lambda: " << edge.lambda << " t: " << edge.t;
            return os;
        }
    };

    class origin_vertex {
    private:
        ll id;
        std::vector<Edge> edges;
    public:
        ll getId() const {
            return id;
        }

        void setId(ll id) {
            origin_vertex::id = id;
        }

        const std::vector<Edge> &getEdges() const {
            return edges;
        }

        void add_edge(Edge &e){
            edges.push_back(e);
        }
    };

    /**
     * 对应转换后的节点
     * eg:
     *  (a,7),(b,5)
     */
    class vertex {
    private:
        ll id;
        std::vector<Edge> edges;
        ll t;
    public:
        ll getId() const {
            return id;
        }

        ll getT() const {
            return t;
        }

        void setT(ll t) {
            vertex::t = t;
        }

        void setId(ll id) {
            vertex::id = id;
        }

        void add_edge(Edge &e){
            edges.push_back(e);
        }
    };
}

#endif