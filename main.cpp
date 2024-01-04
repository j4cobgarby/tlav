#include "graph.hpp"
#include <cmath>
#include <iostream>
#include <raylib.h>

template<typename V, typename W, typename M>
bool Vertex<V,W,M>::process() {
    bool vote = !msg_queue.empty();

    for (; !msg_queue.empty(); msg_queue.pop()) {
        if (msg_queue.front().msg < val) {
            val = msg_queue.front().msg;
            vote = false;
        }
    }

    for (auto [to, from, weight] : out_edges) {
        send(val + weight, to);
    }

    return vote;
}

int main(int argc, char **argv) {
    Graph<float, float, float> graph;

    for (int i = 0; i < 6; i++)
        graph.add_vertex(INFINITY);

    auto verts = graph.get_vertices();
    verts[0]->set_value(0);

    graph.bi_edge(0, 1, 10);
    graph.bi_edge(0, 2, 4);
    graph.bi_edge(0, 4, 3);
    graph.bi_edge(1, 2, 2);
    graph.bi_edge(1, 3, 2);
    graph.bi_edge(1, 5, 8);
    graph.bi_edge(2, 4, 1);
    graph.bi_edge(2, 5, 6);
    graph.bi_edge(3, 5, 1);
    graph.bi_edge(4, 5, 6);

    graph.run();

    for (auto v : verts) {
        std::cout << v->get_value() << ", ";
    }
    std::cout << "\n";
    
    // InitWindow(800, 640, "TLAV Graph Algorithms");
    // SetTargetFPS(60);

    // while (!WindowShouldClose()) {
    //     BeginDrawing();
    //     ClearBackground(RAYWHITE);
    //     DrawText("Hello, world!", 10, 10, 20, BLACK);
    //     EndDrawing();
    // }
}
