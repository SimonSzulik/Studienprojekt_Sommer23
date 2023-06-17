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

using namespace leda;

// Variables
const color cost_c = red;
const color cap_c = blue;

static GRAPH<int,int> G;

static edge_map<int> Gcost(G);
static edge_map<int> Gcap(G);

enum supply_edge  {in, out};

// Get Sum Of In/Out Going Capacities
int get_node_supply_value(node n, supply_edge s) {
    edge e = n->first_in_edge();
    int sum = 0;

    switch(s){
        case in:
            forall_in_edges(e, n){
                sum += Gcap(e);
            }break;
        case out:
            forall_out_edges(e, n){
                sum += Gcap(e);
            }break;
    }
    return sum;
}

// Initialize Edge Handler
void init_edge(GraphWin& gw, edge e){
    Gcap[e] = rand_int(10,20);
    Gcost[e] = rand_int(30,50);
    gw.set_width(e,Gcap[e]/10+5);
    gw.set_slider_value(e,Gcap[e]/100.0,0);
    gw.set_slider_value(e,Gcost[e]/100.0,1);
    gw.set_label(e,string("cost = %d \n cap = %d",Gcost[e],Gcap[e]));

    // Set Suply To Node
    node source_node = source(e);
    node target_node = target(e);

    gw.set_label(source_node, string("%d",get_node_supply_value(source_node, in) - get_node_supply_value(source_node, out)));
    gw.set_label(target_node, string("%d",get_node_supply_value(target_node, in) - get_node_supply_value(target_node, out)));
}

void new_edge_handler(GraphWin& gw, edge e){
    init_edge(gw,e);
}

// Initialize Node Handler
void init_node(GraphWin& gw, node n){
    gw.set_label(n, "0");
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

void end_cap_slider_handler(GraphWin& gw, edge e, double){
    // Set Suply To Node
    node source_node = source(e);
    node target_node = target(e);

    gw.set_label(source_node, string("%d",get_node_supply_value(source_node, in) - get_node_supply_value(source_node, out)));
    gw.set_label(target_node, string("%d",get_node_supply_value(target_node, in) - get_node_supply_value(target_node, out)));
}

int main(){
	// Creating Graph Window
	GraphWin gw(G,"Successive Shortest Path Algorithm");
	gw.display(window::center, window::center);
    gw.set_directed(true);

    // Setting Handlers
    gw.set_new_edge_handler(new_edge_handler);
    gw.set_new_node_handler(new_node_handler);

    gw.set_edge_slider_handler(cost_slider_handler,1);
    gw.set_edge_slider_color(cost_c,1);

    gw.set_end_edge_slider_handler(end_cap_slider_handler,0);
    gw.set_edge_slider_handler(cap_slider_handler,0);
    gw.set_edge_slider_color(cap_c,0);

    while(gw.edit()){

    }
	return 0;
}


