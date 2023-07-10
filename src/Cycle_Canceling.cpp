/**************************
 *	Studien_Projekt
 *
 *      Das Minimum Cost Flow Problem
 *
 *      Visualisierung mit dem Cycle Canceling Algorithmus
 *
 * ************************/

#include <LEDA/graph/graph.h>
#include <LEDA/graphics/graphwin.h>
#include <LEDA/graph/graph_alg.h>

using namespace leda;

// Variables
const color cost_c = red;
const color cap_c = blue;

static GRAPH<int,int> G;

static edge_map<int> cost(G);
static edge_map<int> cap(G);
static node_map<int> supply(G);
static edge_map<int> flow(G);

static edge_map<int> residual_cost(G);
static edge_map<int> rest_cap(G);
map<int, edge> edge_mapper;

static std::vector<edge> negative_cycle_edges;


// Initialize Edge Handler
void init_edge(GraphWin& gw, edge e){
    cap[e] = rand_int(10, 20);
    cost[e] = rand_int(30, 50);
    gw.set_slider_value(e, cap[e] / 100.0, 0);
    gw.set_slider_value(e, cost[e] / 100.0, 1);
    gw.set_label(e,string("cost = %d \n cap = %d", cost[e], cap[e]));
    gw.set_width(e, 4);
}

void new_edge_handler(GraphWin& gw, edge e){
    init_edge(gw,e);
}

// Initialize Node Handler
void init_node(GraphWin& gw, node n){
    gw.set_label_type(n, gw_label_type::data_label);
}

void new_node_handler(GraphWin& gw, node n){
    init_node(gw, n);
}

// Cost Handler & Slider
void cost_slider_handler(GraphWin& gw, edge e, double f){
    cost[e] = int(100 * f);
    gw.set_label(e,string("cost = %d \n cap = %d", cost[e], cap[e]));
}

// Cap Handler & Slider
void cap_slider_handler(GraphWin& gw,edge e, double f){
    cap[e] = int(100 * f);
    gw.set_label(e,string("cost = %d \n cap = %d", cost[e], cap[e]));
}

edge search_edge(node s, node t) {
    edge e;
    forall_edges(e, G) {
        if (source(e) == s && target(e) == t) {
            return e;
        }
    }

    return nil;
}

void calculate_balance(GraphWin& gw) {
    int balance = 0;

    // color nodes regarding the supply
    node v;
    forall_nodes(v,G) {
        if (G.node_data()[v] > 0) {
            gw.set_color(v, green);
        }
        else if (G.node_data()[v] < 0) {
            gw.set_color(v, red);
        }
        else {
            gw.set_color(v, grey1);
        }
    }

    forall_nodes(v,G) {
        balance += G.node_data()[v];
    }

    if (balance != 0) {
        gw.message("Supply/Demand values don't sum up to zero.");
    }
    else {
        gw.message("Correct Supply Values.");
    }
}

void initialize(GraphWin& gw) {
    node v;
    forall_nodes(v, G) supply[v] = G.node_data()[v];

    bool F = FEASIBLE_FLOW(G, supply, cap, flow);

    if (F) gw.message("FEASIBLE FLOW: true");
    else gw.message("FEASIBLE FLOW: false");

    edge e;
    forall_edges(e, G) {
        if (flow[e] != 0) {
            gw.set_label(e, string("cost = %d \n cap = %d \n flow = %d", cost[e], cap[e], flow[e]));
            gw.set_color(e, blue);
            gw.set_width(e, 6);
        }
        else {
            gw.set_color(e, grey1);
        }
    }
}

void build_residual_graph(GraphWin& gw) {
    edge e;
    int key = 0;
    forall_edges(e, G) {
        edge_mapper[key] = e;
        key++;
    }

    int edge_count = key;
    for (int i=0; i < edge_count; i++) {
        edge e = edge_mapper[i];

        // Insert both
        if (flow[e] < cap[e] && flow[e] > 0) {
            // Foward Edge
            residual_cost[e] = cost[e];
            rest_cap[e] = cap[e] - flow[e];
            gw.set_label(e, string("r=%d \n c=%d \n f=%d", rest_cap[e], residual_cost[e], flow[e]));
            gw.set_color(e, blue);
            gw.set_width(e, 6);

            // Backwards Edge
            edge ji = gw.new_edge(target(e), source(e));
            residual_cost[ji] = -cost[e];
            rest_cap[ji] = flow[e];
            gw.set_label(ji, string("r=%d \n c=%d", rest_cap[ji], residual_cost[ji]));
            gw.set_color(ji, pink);
            gw.set_width(ji, 4);
        }

        // Fully saturated
        if (flow[e] == cap[e]) {
            edge ji = gw.new_edge(target(e), source(e));
            residual_cost[ji] = -cost[e];
            rest_cap[ji] = flow[e];
            gw.set_label(ji, string("r=%d \n c=%d", rest_cap[ji], residual_cost[ji]));
            gw.set_color(ji, pink);
            gw.set_width(ji, 4);

            G.del_edge(e);
        }

        // Zero Flow
        if (flow[e] == 0) {
            residual_cost[e] = cost[e];
            rest_cap[e] = cap[e];
            gw.set_label(e, string("r=%d \n c=%d", rest_cap[e], residual_cost[e]));
            gw.set_color(e, black);
            gw.set_width(e, 4);
        }
    }
}

std::vector<node> bellman_ford(GraphWin& gw, node_array<int> dist, node s) {
    edge l;
    forall_edges(l, G) {
        residual_cost[l] = -4;
        gw.set_label(l, string("c = %d, r = %d", residual_cost[l], rest_cap[l]));
    }
    int n = G.number_of_nodes();
    gw.message("Calculating negative cycles...");

    node v;
    forall_nodes(v, G) {
        dist[v] = std::numeric_limits<int>::max();
    }

    dist[s] = 0;
    static node_array<edge> pred(G);

    for (int i = 0; i < n-1; ++i) {
        edge e;
        forall_edges(e, G) {
            if (dist[source(e)] != std::numeric_limits<int>::max() && dist[source(e)] + residual_cost[e] < dist[target(e)]) {
                dist[target(e)] = dist[source(e)] + residual_cost[e];
                pred[target(e)] = e;
            }
        }
    }

    // Check for negative cycle
    edge e;
    forall_edges(e, G) {
        if (dist[source(e)] != std::numeric_limits<int>::max() && dist[source(e)] + residual_cost[e] < dist[target(e)]) {
            std::vector<node> cycle;

            node negativeCycleNode = target(e);
            for(int i = 0; i < n-1; ++i) {
                negativeCycleNode = source(pred[negativeCycleNode]);
            }

            node v = negativeCycleNode;
            do {
                cycle.push_back(v);
                v = source(pred[v]);
                negative_cycle_edges.push_back(pred[v]);
            } while(v != negativeCycleNode);

            return cycle;
        }
    }

    gw.message("Done...");

    return std::vector<node>();
}

int main(){
    node v;
    // Creating Graph Window
    GraphWin gw(G,"Cycle Canceling Algorithm");
    gw.display(window::center, window::center);
    gw.set_directed(true);

    // Setting Handlers
    gw.set_new_edge_handler(new_edge_handler);
    gw.set_new_node_handler(new_node_handler);

    gw.set_edge_slider_handler(cost_slider_handler,1);
    gw.set_edge_slider_color(cost_c,1);

    gw.set_edge_slider_handler(cap_slider_handler,0);
    gw.set_edge_slider_color(cap_c,0);

    while(gw.edit()) {
        gw.message("Calculating node balances...");
        wait(1);
        calculate_balance(gw);

        wait(1);
        gw.message("Calculating feasible flow...");

        // Calculate feasible flow
        wait(1);
        initialize(gw);

        wait(1);
        gw.message("Building residual graph...");

        wait(3);
        //build_residual_graph(gw);
        gw.message("Residual Graph");

        // find negative cycle
        node_array<int> dist(G);
        std::vector<node> cycle = bellman_ford(gw, dist, G.first_node());

        for (edge k : negative_cycle_edges) {
            gw.set_color(k, red);
        }

        while (!cycle.empty()) {
            // Find minimal capacity
            int d = 999;
            for (edge e: negative_cycle_edges) {
                if (rest_cap[e] < d) {
                    d = rest_cap[e];
                }
            }

            gw.message(string("min cap = %d", d));




            break;
        }
    }
    return 0;
}


