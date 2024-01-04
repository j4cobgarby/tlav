#pragma once

#include <memory>
#include <vector>
#include <queue>

template<typename V, typename W, typename M>
class Vertex : public std::enable_shared_from_this<Vertex<V,W,M>> {
    using vert = Vertex<V, W, M>;
    using vert_ptr = std::shared_ptr<vert>;
public:
    struct Edge {
        vert_ptr to;
        vert_ptr from;
        W weight;
    };

    struct Msg {
        vert_ptr to;
        vert_ptr from;
        M msg;
    };
    
    Vertex(V val) : val(val) {};

    void recv(Msg msg) { next_queue.push(msg); }
    void send(M msg, vert_ptr to) { to->recv((Msg){to, this->shared_from_this(), msg}); }

    std::vector<Edge>& outgoing() { return out_edges; }

    void add_edge(vert_ptr v, W w) { out_edges.push_back((Edge){ v, this->shared_from_this(), w }); }

    void add_edge_bi(vert_ptr v, W w) {
        this->add_edge(v, w);
        v->add_edge(this->shared_from_this(), w);
    }

    const V& get_value() { return val; }
    void set_value(V v) { val = v; }

    bool process();

    void next_step() {
        std::swap(msg_queue, next_queue);
        next_queue = {};
    }

    bool is_asleep() { return asleep; }
    void sleep() { asleep = true; }
    void wake() { asleep = false; }
protected:
    V val;
    std::vector<Edge> out_edges;
    std::queue<Msg> msg_queue;
    std::queue<Msg> next_queue;
    bool asleep;
};

template<typename V, typename W, typename M>
class Graph {
    using Vert = Vertex<V, W, M>;
    using Vert_ptr = std::shared_ptr<Vert>;
protected:
    std::vector<Vert_ptr> vertices;
public:
    std::vector<Vert_ptr>& get_vertices() { return vertices; }

    void add_vertex(V val) { vertices.push_back(std::make_shared<Vert>(val)); }

    void dir_edge(Vert_ptr a, Vert_ptr b, W weight) { a->add_edge(b, weight); }
    void dir_edge(size_t i_a, size_t i_b, W weight) { dir_edge(vertices[i_a], vertices[i_b], weight); }

    void bi_edge(Vert_ptr a, Vert_ptr b, W weight) { a->add_edge_bi(b, weight); }
    void bi_edge(size_t i_a, size_t i_b, W weight) { bi_edge(vertices[i_a], vertices[i_b], weight); } 

    bool compute_one() {
        bool stopping = true;
        
        for (auto v : vertices) v->next_step();
        for (auto v : vertices) {
            if (!v->process()) stopping = false;
        }

        return stopping;
    }

    void run() { while (!compute_one()); }
};
