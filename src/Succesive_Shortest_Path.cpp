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

const color cost_c = red;
const color cap_c = blue;

static GRAPH<int,int> G;

static edge_map<int> Gcost(G);
static edge_map<int> Gcap(G);

void init_edge(GraphWin& gw, edge e)
{ Gcap[e] = rand_int(10,50);
    Gcost[e] = rand_int(10,75);
    gw.set_slider_value(e,Gcap[e]/100.0,0);
    gw.set_slider_value(e,Gcost[e]/100.0,1);
}

void new_edge_handler(GraphWin& gw, edge e)
{ init_edge(gw,e);
}

void start_cost_slider_handler(GraphWin& gw, edge, double)
{ gw.message("\\bf\\red Change Edge Cost"); }

void cost_slider_handler(GraphWin& gw, edge e, double f)
{ Gcost[e] = int(100*f);
    gw.set_label_color(e,cost_c);
    gw.set_label(e,string("cost = %d",Gcost[e]));
}

void start_cap_slider_handler(GraphWin& gw, edge, double)
{ gw.message("\\bf\\blue Change Edge Capacity"); }

void cap_slider_handler(GraphWin& gw,edge e, double f)
{ Gcap[e] = int(100*f);
    gw.set_label_color(e,cap_c);
    gw.set_label(e,string("cap = %d",Gcap[e]));
}

int main(){
	// Creating graph window and setting title
	GraphWin gw(G,"Successive Shortest Path Algorithm");
	gw.display(window::center, window::center);

    gw.set_new_edge_handler(new_edge_handler);

    gw.set_start_edge_slider_handler(start_cost_slider_handler,0);
    gw.set_edge_slider_handler(cost_slider_handler,1);
    gw.set_edge_slider_color(cost_c,1);

    gw.set_start_edge_slider_handler(start_cap_slider_handler,0);
    gw.set_edge_slider_handler(cap_slider_handler,0);
    gw.set_edge_slider_color(cap_c,0);

    gw.set_directed(true);

    while(gw.edit()){

    }
	return 0;
}


