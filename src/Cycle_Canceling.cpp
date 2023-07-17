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
static std::vector<edge> original_graph;

static edge_map<int> cost(G);
static edge_map<int> cap(G);
static node_map<int> supply(G);
static edge_map<int> flow(G);

static map<edge, int> cost_copy;
static map<edge, int> cap_copy;
static map<edge, int> flow_copy;

static edge_map<int> residual_cost(G);
static edge_map<int> rest_cap(G);
map<int, edge> edge_mapper;

static std::vector<edge> negative_cycle_edges;
static std::vector<edge> residual_edges;


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
void cap_slider_handler(GraphWin& gw, edge e, double f){
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
            gw.set_label_color(e, blue);
            gw.set_color(e, blue);
            gw.set_width(e, 6);
        }
        else {
            gw.set_color(e, grey1);
        }
    }

    for (edge m : original_graph) {
        flow_copy[m] = flow[m];
    }
}

void build_residual_graph(GraphWin& gw) {
    int key = 0;
    edge_mapper.clear();
    for (edge e : original_graph) {
        edge_mapper[key] = e;
        key++;
    }

    int edge_count = key;
    for (int i=0; i < edge_count; i++) {
        edge e = edge_mapper[i];

        // Insert both
        if (flow_copy[e] < cap_copy[e] && flow_copy[e] > 0) {
            // Foward Edge
            residual_cost[e] = cost_copy[e];
            rest_cap[e] = cap_copy[e] - flow_copy[e];
            gw.set_label(e, string("c=%d\nr=%d", residual_cost[e], rest_cap[e]));
            gw.set_label_color(e, black);
            gw.set_color(e, black);
            gw.set_width(e, 4);

            // Backwards Edge
            edge ji = gw.new_edge(target(e), source(e));
            residual_cost[ji] = -cost_copy[e];
            rest_cap[ji] = flow_copy[e];
            gw.set_label(ji, string("c=%d\nr=%d", residual_cost[ji], rest_cap[ji]));
            gw.set_label_color(ji, pink);
            gw.set_color(ji, pink);
            gw.set_label_color(ji, pink);
            gw.set_width(ji, 4);
        }

        // Fully saturated
        if (flow_copy[e] == cap_copy[e]) {
            edge ji = gw.new_edge(target(e), source(e));
            residual_cost[ji] = -cost_copy[e];
            rest_cap[ji] = flow_copy[e];
            gw.set_label(ji, string("c=%d\nr=%d", residual_cost[ji], rest_cap[ji]));
            gw.set_label_color(ji, pink);
            gw.set_color(ji, pink);
            gw.set_label_color(ji, pink);
            gw.set_width(ji, 4);

            G.del_edge(e);
        }

        // Zero Flow
        if (flow_copy[e] == 0) {
            residual_cost[e] = cost_copy[e];
            rest_cap[e] = cap_copy[e];
            gw.set_label(e, string("c=%d\nr=%d", residual_cost[e], rest_cap[e]));
            gw.set_label_color(e, black);
            gw.set_color(e, black);
            gw.set_width(e, 4);
        }
    }

    edge k;
    forall_edges(k, G) {
        residual_edges.push_back(k);
    }
}

std::vector<node> bellman_ford(GraphWin& gw, node_array<int> dist, node s) {
    int n = G.number_of_nodes();
    gw.message("Calculating negative cycles...");

    node v;
    forall_nodes(v, G) {
        dist[v] = std::numeric_limits<int>::max();
    }

    dist[s] = 0;
    static node_array<edge> pred(G);

    for (int i = 0; i < n; i++) {
        edge e;
        forall_edges(e, G) {
            if (dist[source(e)] != std::numeric_limits<int>::max() && dist[source(e)] + residual_cost[e] < dist[target(e)]) {
                dist[target(e)] = dist[source(e)] + residual_cost[e];
                pred[target(e)] = e;
            }
        }
    }

    // Check for negative cycle
    negative_cycle_edges.clear();
    edge e;
    forall_edges(e, G) {
        if (dist[source(e)] != std::numeric_limits<int>::max() && dist[source(e)] + residual_cost[e] < dist[target(e)]) {
            std::vector<node> cycle;

            node negativeCycleNode = target(e);
            for(int i = 0; i < n; i++) {
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

    return std::vector<node>();
}

bool edge_direction_is_equal(edge e) {
    for (edge s : original_graph) {
        if (source(s) == source(e) && target(s) == target(e)) {
            return true;
        }
    }
    return false;
}

bool edge_direction_is_reversed(edge e) {
    for (edge s : original_graph) {
        if (source(s) == target(e) && target(s) == source(e)) {
            return true;
        }
    }
    return false;
}

bool is_original(edge r) {
    for (edge o : original_graph) {
        if (source(o) == source(r) && target(o) == target(r)) {
            return true;
        }
    }
    return false;
}

void rebuild_original_graph(GraphWin& gw) {
    for (edge r : residual_edges) {
        if (!is_original(r)) {
            G.del_edge(r);
        }
    }

    for (edge e : original_graph) {
        if (flow_copy[e] > 0) {
            gw.set_color(e, blue);
            gw.set_width(e, 6);
            gw.set_label(e, string("cost = %d \n cap = %d \n flow = %d", cost_copy[e], cap_copy[e], flow_copy[e]));
            gw.set_label_color(e, blue);
        }
        else {
            gw.set_label(e, string("cost = %d \n cap = %d", cost_copy[e], cap_copy[e]));
            gw.set_color(e, black);
        }
    }
    residual_edges.clear();
    gw.message("Original Graph");
}

edge get_original_of_reverse(edge r) {
    for (edge o : original_graph) {
        if (source(r) == target(o) && target(r) == source(o)) {
            return o;
        }
    }
    return nil;
}

bool is_already_used(std::vector<edge> used, edge e) {
    for (edge u : used) {
        if (source(u) == source(e) && target(u) == target(e)) {
            return true;
        }
    }

    return false;
}

int total_cost() {
    int min_cost = 0;

    for (edge e : original_graph) {
        int flow = flow_copy[e];
        if (flow > 0) {
            int cost = cost_copy[e];
            min_cost += flow * cost;
        }
    }

    return min_cost;
}

int main(){
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
        //build_test_graph(gw);
        // Save original graph
        edge r;
        forall_edges(r, G) {
            original_graph.push_back(r);
            cap_copy[r] = cap[r];
            cost_copy[r] = cost[r];
        }

        gw.message("Calculating node balances...");
        sleep(1);
        calculate_balance(gw);

        gw.edit();

        gw.message("Calculating feasible flow...");
        sleep(1);

        // Calculate feasible flow
        initialize(gw);
        gw.message(string("Total Cost of Flow: %d --> Click \"done\".", total_cost()));
        gw.edit();

        gw.message("Building Residual Graph...");
        sleep(1);
        build_residual_graph(gw);
        gw.message("Generated Residual Graph. --> Click \"done\".");

        gw.edit();

        // find negative cycle
        gw.message("Detect negative cycles in residual graph...");
        sleep(2);
        node_array<int> dist(G);
        std::vector<node> cycle = bellman_ford(gw, dist, G.first_node());

        node x;
        forall_nodes(x, G) {
            cycle = bellman_ford(gw, dist, G.first_node());

            if (!cycle.empty()) {
                break;
            }
        }

        for (edge e : negative_cycle_edges) {
            gw.set_color(e, red);
            gw.set_label_color(e, red);
        }

        if (cycle.empty()) {
            gw.message("No negative cycles detected.");
            sleep(3);
        }

        while (!cycle.empty()) {
            // Find minimal capacity as flow
            int d = 999;
            for (edge e: negative_cycle_edges) {
                if (rest_cap[e] < d) {
                    d = rest_cap[e];
                }
            }

            gw.message(string("Augment %d Flow Units through the negative cycle --> Click \"done\".", d));
            gw.edit();

            std::vector<edge> used;

            // adjust flow with delta
            for (edge e : negative_cycle_edges) {
                if (!is_already_used(used, e)) {
                    if (edge_direction_is_equal(e)) {
                        gw.set_color(e, green2);
                        gw.message(string("Old Flow: %d --> Click \"done\".", flow_copy[e]));
                        gw.edit();
                        flow_copy[e] += d;
                        gw.message(string("New Flow: %d --> Click \"done\".", flow_copy[e]));
                        gw.edit();
                    }

                    else {
                        edge o = get_original_of_reverse(e);
                        gw.set_color(o, green2);
                        gw.message(string("Old Flow: %d --> Click \"done\".", flow_copy[o]));
                        gw.edit();
                        flow_copy[o] -= d;
                        gw.message(string("New Flow: %d --> Click \"done\".", flow_copy[o]));
                        gw.edit();
                    }

                    used.push_back(e);
                }
            }

            used.clear();

            gw.message("Original Graph Adjustments...");
            sleep(2);
            rebuild_original_graph(gw);

            for (edge e : original_graph) {
                if (flow_copy[e] > 0) {
                    gw.set_color(e, blue);
                    gw.set_width(e, 6);
                    gw.set_label(e, string("cost = %d \n cap = %d \n flow = %d", cost_copy[e], cap_copy[e], flow_copy[e]));
                    gw.set_label_color(e, blue);
                }
                else {
                    gw.set_label(e, string("cost = %d \n cap = %d", cost_copy[e], cap_copy[e]));
                    gw.set_label_color(e, black);
                }
            }

            gw.message(string("Total Cost of Flow: %d --> Click \"done\" to minimize costs.", total_cost()));

            gw.edit();

            build_residual_graph(gw);

            gw.message("Finding negative cycles in residual graph --> Click \"done\"..");

            gw.edit();

            node x;
            forall_nodes(x, G) {
                cycle = bellman_ford(gw, dist, G.first_node());

                if (!cycle.empty()) {
                    break;
                }
            }

            for (edge e : negative_cycle_edges) {
                gw.set_color(e, red);
            }
        }

        rebuild_original_graph(gw);

        gw.message(string("Algorithm terminated. Minimal Cost of Flow: %d", total_cost()));
    }
    return 0;
}


