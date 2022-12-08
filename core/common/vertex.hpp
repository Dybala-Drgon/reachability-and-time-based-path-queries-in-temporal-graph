#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <vector>
#include <ostream>

using ll = long long;
namespace TEngine {
    struct Edge {
        ll end_point;// 对端节点
        int idx;
        int t; // 对应论文中的时间,在origin_vertex中有意义
        int lambda; // λ值
        Edge() {
            end_point = -1;
            idx = -1;
            lambda = 0;
            t = 0;
        }

        Edge(ll endPoint, int t, int lambda) : end_point(endPoint), t(t), lambda(lambda) {}

        int get_end_time() {
            return lambda + t;
        }

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
            os << "end_point: " << edge.end_point << " idx: " << edge.idx << " t: " << edge.t << " lambda: "
               << edge.lambda;
            return os;
        }
    };

    class origin_vertex {
    private:
        ll id;
        std::vector<Edge> out_edges;
        std::vector<Edge> in_edges;
    public:
        ll getId() const {
            return id;
        }

        std::vector<Edge> &getOutEdges() {
            return out_edges;
        }

        std::vector<Edge> &getInEdges() {
            return in_edges;
        }

        void setId(ll id) {
            origin_vertex::id = id;
        }

        friend std::ostream &operator<<(std::ostream &os, const origin_vertex &vertex) {
            os << "id: " << vertex.id << " out_edges: ";
            for (auto &t: vertex.out_edges) {
                os << t << " ";
            }
            os << "\n";
            os << "id: " << vertex.id << " in_edges: ";
            for (auto &t: vertex.in_edges) {
                os << t << " ";
            }
            os << "\n";
            return os;
        }


        void add_out_edge(Edge &e) {
            out_edges.push_back(e);
        }

        void add_in_edge(Edge &e) {
            in_edges.push_back(e);
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
        ll t;
        std::vector<Edge> out_edges; // 第一个元素表示和本map元素的连接，后续元素表示和其他表的连接,对于最后一个节点，使用一个虚拟的dum占位
        std::vector<Edge> in_edges;
        bool is_out;
    public:
        bool isOut() const {
            return is_out;
        }

        void setIsOut(bool isOut) {
            is_out = isOut;
        }

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

        void add_out_edge(Edge &e) {
            out_edges.push_back(e);
        }

        void add_in_edge(Edge &e) {
            in_edges.push_back(e);
        }

        std::vector<Edge> &getOutEdges() {
            return out_edges;
        }

        std::vector<Edge> &getInEdges() {
            return in_edges;
        }

        friend std::ostream &operator<<(std::ostream &os, const vertex &vertex) {
            os << "id: " << vertex.id << " t: " << vertex.t;
            return os;
        }
    };
}

#endif