#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>

class Node
{
public:
	Node(int index) : index(index) {};
	void addNeighbor(std::vector<Node>& map, int index);

	int index;
	int type = 0;

	std::vector<Node*> neighbors;

	bool operator==(const Node& rhs) const
	{
		return (index == rhs.index) && (index == rhs.index);
	}

	bool operator!=(const Node& rhs) const
	{
		return !(*this == rhs);
	}

	bool operator<(const Node& rhs) const
	{
		return (index < rhs.index);
	}

	friend std::ostream& operator<<(std::ostream& out, const Node& node);
	friend std::ostream& operator<<(std::ostream& out, Node* node);
};

inline std::ostream& operator<<(std::ostream& out, const Node& node) {
	return out << node.index << " (" << node.type << ")";
}

inline std::ostream& operator<<(std::ostream& out, Node* node) {
	return out << *node;
};

Node& getNode(std::vector<Node>& map, int index)
{
	return map[index];
}

void Node::addNeighbor(std::vector<Node>& map, int index)
{
	// neighboring nodes
	Node& child = getNode(map, index);
	neighbors.push_back(&child);
}

std::vector<Node> buildPath(std::map<Node*, Node*> path, Node* goalNode)
{
	std::vector<Node> constructedPath;
	Node* curr = goalNode;
	constructedPath.push_back(*curr);

	while (path[curr] != curr)
	{
		curr = path[curr];
		constructedPath.push_back(*curr);
	}

	constructedPath.pop_back();
	std::reverse(begin(constructedPath), end(constructedPath));
	return constructedPath;
}

std::vector<Node> BFS(std::vector<Node>& map, int SkynetIndex)
{
	std::queue<Node*> q; // Nodes to explore
	std::vector<Node*> visited; // Visited Nodes
	std::map<Node*, Node*> path; // path map (Node -> Node)

	Node* curr = &getNode(map, SkynetIndex);

	q.push(curr);
	path[curr] = curr;
	visited.push_back(curr);

	while (!q.empty())
	{
		curr = q.front();
		q.pop();

		if (curr->type == 1)
		{
			std::cerr << "Closest Gateway found on: " << *curr << std::endl;
			return buildPath(path, curr);
		}

		for (auto child : curr->neighbors)
		{
			// visited contains child
			if (std::find(visited.begin(), visited.end(), child) != visited.end())
				continue;

			visited.push_back(child);

			path[child] = curr;
			q.push(child);
		}
	}

	std::cerr << "BFS: Could not find any gateways" << std::endl;
	std::vector<Node> constructedPath = {};
	return constructedPath;
}

void printMap(std::vector<Node>& map) {
	for (size_t i = 0; i < map.size(); i++)
	{
		std::cerr << map[i].type << " ";
	}
	std::cerr << std::endl;
}

int main() 
{
	int N; // the total number of nodes in the level, including the gateways
	int L; // the number of links
	int E; // the number of exit gateways
	std::cin >> N >> L >> E; std::cin.ignore();

	std::vector<Node> map;
	for (int i = 0; i < N; i++)
		map.push_back(Node(i));

	
	for (int i = 0; i < L; i++) {
		int N1; // N1 and N2 defines a link between these nodes
		int N2;
		std::cin >> N1 >> N2; std::cin.ignore();

		getNode(map, N1).addNeighbor(map, N2);
		getNode(map, N2).addNeighbor(map, N1);
	}
	for (int i = 0; i < E; i++) {
		int EI; // the index of a gateway node
		std::cin >> EI; std::cin.ignore();
		getNode(map, EI).type = 1;
	}

	printMap(map);

	// Game loop
	while (1) {

		int SI; // The index of the node on which the Skynet agent is positioned this turn
		std::cin >> SI; std::cin.ignore();

		printMap(map);

		std::vector<Node> path = BFS(map, SI);

		// Print path (coords)
		std::cerr << "Path: " << path.size() << " cells, ";
		for (auto p : path)
		{
			std::cerr << " -> " << p;
		}
		std::cerr << std::endl;

		// Sever links
		Node * node1 = &getNode(map, path[1].index);
		Node * node2 = &getNode(map, path[0].index);

		node1->neighbors.erase(std::remove_if(node1->neighbors.begin(), node1->neighbors.end(),
			[&](Node* n) {return (n->index == node2->index); }), node1->neighbors.end());

		node2->neighbors.erase(std::remove_if(node2->neighbors.begin(), node2->neighbors.end(),
			[&](Node* n) {return (n->index == node1->index); }), node2->neighbors.end());

		std::cout << path[0].index << " " << path[1].index << std::endl;   	
	}

	return 0;
}