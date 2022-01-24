#pragma once

#include <vector>

// each "cell" of the grid created is going to have a coorelating NavigationNode
class NavigationNode
{
public:
	/** coordinates that represent the node in grid space ie where it is in the grid */
	FIntVector Coordinates;

	/** vector of other nodes that are neighbors of the "this" node */
	std::vector<NavigationNode*> Neighbors;

	/** fscore is the score that represents this nodes distance to whatever end point its pathing to */
	float FScore = MAX_FLT;
	
};
// node compare is used to sort the std::multiset in the nav volume's find path method
struct NodeCompare
{
	bool operator() (const NavigationNode* lhs, const NavigationNode* rhs) const
	{
		return (lhs->FScore < rhs->FScore);
	}
};
