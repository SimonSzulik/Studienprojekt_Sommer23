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

// Initialize Edge Handler
void init_edge(GraphWin& gw, edge e){
    cap[e] = rand_int(10, 20);
    cost[e] = rand_int(30, 50);
    gw.set_width(e, cap[e] / 10 + 5);
    gw.set_slider_value(e, cap[e] / 100.0, 0);
    gw.set_slider_value(e, cost[e] / 100.0, 1);
    gw.set_label(e,string("cost = %d \n cap = %d", cost[e], cap[e]));
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
    gw.set_width(e, cap[e]/25 + 3);
    gw.set_label(e,string("cost = %d \n cap = %d", cost[e], cap[e]));
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

bool bellman_ford(){

    return false;
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
            gw.set_label(e, string("rest_cap = %d \n cost = %d", rest_cap[e], cost[e]));
            gw.set_color(e, black);

            // Backwards Edge
            edge ji = gw.new_edge(target(e), source(e));
            residual_cost[ji] = -cost[e];
            rest_cap[ji] = flow[e];
            gw.set_label(ji, string("rest_cap = %d \n cost = %d", rest_cap[ji], cost[ji]));
            gw.set_color(ji, pink);

            // iterate graph, look for edge in other direction, get width
            edge g;
            float w = 0;
            forall_edges(g,G){
                if(source(g) == target(ji) && target(g) == source(ji)){
                    w = gw.get_width(g);
                }
            }
            gw.set_width(ji, w);
        }

        // Fully saturated
        if (flow[e] == cap[e]) {
            edge ji = gw.new_edge(target(e), source(e));
            residual_cost[ji] = -cost[e];
            rest_cap[ji] = flow[e];
            gw.set_label(ji, string("rest_cap = %d \n cost = %d", rest_cap[ji], cost[ji]));
            gw.set_color(ji, pink);
        }

        // Zero Flow
        if (flow[e] == 0) {
            residual_cost[e] = cost[e];
            rest_cap[e] = cap[e];
            gw.set_label(e, string("rest_cap = %d \n cost = %d", rest_cap[e], cost[e]));
            gw.set_color(e, black);
        }
    }
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



    while(gw.edit()){
        gw.message("Calculating node balances...");
        wait(2);
        calculate_balance(gw);
        wait(2);
        gw.message("Calculating feasible flow...");
        wait(2);
        initialize(gw);
        wait(2);
        gw.message("Building residual graph...");
        wait(5);
        build_residual_graph(gw);
        gw.message("Residual Graph");
        
    }

    return 0;
}

