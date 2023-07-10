/**************************
 *	Studien_Projekt
 *
 *      Das Minimum Cost Flow Problem
 *
 *      Visualisierung mit dem Successive Shortest Path Algorithmus
 *
 * ************************/

#include <LEDA/graph/graph.h>
#include <LEDA/graphics/graphwin.h>
#include <LEDA/core/set.h>

using namespace leda;

// Variables
const color cost_c = red;
const color cap_c = blue;

static GRAPH<int,int> G;

static edge_map<int> Gcost(G);
static edge_map<int> Gcap(G);

// Initialize Edge Handler
void init_edge(GraphWin& gw, edge e){
    Gcap[e] = rand_int(10,20);
    Gcost[e] = rand_int(30,50);
    gw.set_width(e,Gcap[e]/10+5);
    gw.set_slider_value(e,Gcap[e]/100.0,0);
    gw.set_slider_value(e,Gcost[e]/100.0,1);
    gw.set_label(e,string("cost = %d \n cap = %d",Gcost[e],Gcap[e]));
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
    Gcost[e] = int(100*f);
    gw.set_label(e,string("cost = %d \n cap = %d",Gcost[e],Gcap[e]));
}

// Cap Handler & Slider
void cap_slider_handler(GraphWin& gw,edge e, double f){
    Gcap[e] = int(100*f);
    gw.set_width(e,Gcap[e]/10+5);
    gw.set_label(e,string("cost = %d \n cap = %d",Gcost[e],Gcap[e]));
}

// Algorithm functions
void initialize_excess_demand(node_array<int>& ex, set<node>& E, set<node>& D) {
    node v;
    forall_nodes(v, G) {
        ex[v] = G.node_data()[v];
        if (ex[v] < 0) {
            D.insert(v);
        }
        else if (ex[v] > 0) {
            E.insert(v);
        }
    }
}

void visualize_ssp(GraphWin& gw) {
    // Initialize variables
    node v;
    node e;
    node d;
    edge_array<int> flow(G);
    node_array<int> potential(G);
    node_array<int> excess(G);
    set<node> EXCESS;
    set<node> DEMAND;

    initialize_excess_demand(excess, EXCESS, DEMAND);

    // Visualize initialized state
    gw.message("Initialized: Set E green, Set D red");
    forall_nodes(v,G) {
        if (EXCESS.member(v)) {
            gw.set_color(v, green);
        }
        else if (DEMAND.member(v)) {
            gw.set_color(v, red);
        }
        else {
            gw.set_color(v, grey1);
        }
    }
    leda_sleep(2);

    // Main loop
    while(!EXCESS.empty()) {
        e = EXCESS.choose();
        d = DEMAND.choose();

        gw.set_color(e, yellow);
        gw.set_color(d, brown);

        gw.message("Choose one excess and one demand node");
        leda_sleep(2);
        break;
    }

}

int main(){
    node v;

    // Creating Graph Window
    GraphWin gw(G,"Successive Shortest Path Algorithm");
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
        int balance = 0;

        forall_nodes(v,G) {
            balance += G.node_data()[v];
        }
      
        if (balance != 0) {
            gw.message("Supply/Demand values don't sum up to zero.");
        }
        else {
            gw.message("Correct Supply Values.");
            visualize_ssp(gw);
        }
    }

    return 0;
}
