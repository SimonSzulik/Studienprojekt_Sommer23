/**************************
 *	Studien_Projekt
 *
 *      Das Minimum Cost Flow Problem
 *      
 *      Visualisierung mit dem Successive Shortest Path Algorithmus
 *
 * ************************/

#include <LEDA/graph/graph.h>
#include <LEDA/core/queue.h>
#include <LEDA/graphics/graphwin.h>

using namespace leda;

int main(){
	// Creating graph window and setting title
	GraphWin gw("Successive Shortest Path Algorithmus");
	graph& G = gw.get_graph();
	gw.display(window::center, window::center); 

	// While graph is edited, do stuff
	while ( gw.edit() ){

	}
	return 0; 
}


