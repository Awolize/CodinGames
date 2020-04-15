#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>

class Node
{
public:
	Node(int x, int y, char type) : x(x), y(y), type(type) {};
	void addNeighbors(std::vector<Node>& map, int X, int Y);
	int x = 0;
	int y = 0;
	char type;
	std::vector<Node*> neighbors;

	bool operator==(const Node& rhs) const
	{
		return (x == rhs.x) && (y == rhs.y);
	}

	bool operator!=(const Node& rhs) const
	{
		return !(*this == rhs);
	}

	bool operator<(const Node& rhs) const
	{
		return (x < rhs.x) && (y < rhs.y);
	}

	friend std::ostream& operator<<(std::ostream& out, const Node& node);
	friend std::ostream& operator<<(std::ostream& out, Node* node);
};

inline std::ostream& operator<<(std::ostream& out, const Node& node) {
	return out << "(" << node.x << ", " << node.y << ")";
}

inline std::ostream& operator<<(std::ostream& out, Node* node) {
	return out << *node;
};

Node& getNode(std::vector<Node>& map, int x, int y)
{
	for (auto& node : map)
	{
		if (node.x == x && node.y == y)
			return node;
	}
}

void Node::addNeighbors(std::vector<Node>& map, int maxX, int maxY)
{
	// neighboring nodes
	std::vector<std::vector<int>> neighborsCoords{ {0,1}, {0,-1}, {1,0}, {-1,0} };
	for (size_t i = 0; i < neighborsCoords.size(); i++)
	{
		if (x + neighborsCoords[i][0] >= 0 && y + neighborsCoords[i][1] >= 0 && x + neighborsCoords[i][0] < maxX && y + neighborsCoords[i][1] < maxY)
		{
			Node& child = getNode(map, x + neighborsCoords[i][0], y + neighborsCoords[i][1]);
			neighbors.push_back(&child);
		}
	}
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

std::vector<Node> BFS(std::vector<Node>& map, int posX, int posY, char goal)
{
	std::queue<Node*> q; // Nodes to explore
	std::vector<Node*> visited; // Visited Nodes
	std::map<Node*, Node*> path; // path map (Node -> Node)

	Node* curr = &getNode(map, posX, posY);

	q.push(curr);
	path[curr] = curr;
	visited.push_back(curr);

	while (!q.empty())
	{
		curr = q.front();
		q.pop();

		if (curr->type == goal)
		{
			std::cerr << "current pos: " << *curr << std::endl;
			std::cerr << "Target: '" << goal << "'" << std::endl;
			std::cerr << "Goal '" << goal << "' was found on: " << curr << std::endl;
			return buildPath(path, curr);
		}

		for (auto child : curr->neighbors)
		{
			// visited contains child
			if (std::find(visited.begin(), visited.end(), child) != visited.end())
				continue;

			visited.push_back(child);

			if ((*child).type == '#') // wall
				continue;

			if (goal == '?') // skip 'C' if there is more to explore
				if ((*child).type == 'C')
					continue;

			if (goal == 'C' || goal == 'T') // skip '?' if goal is 'C' or 'T'
				if ((*child).type == '?')
					continue;

			path[child] = curr;
			q.push(child);
		}
	}

	std::vector<Node> pathToC = BFS(map, posX, posY, 'C');
	if (pathToC.size())
		return pathToC;

	std::cerr << "BFS: Could not find '" << goal << "'" << std::endl;
	std::vector<Node> constructedPath = {};
	return constructedPath;
}

void printMap(std::vector<Node>& map, int maxX) {
	for (size_t i = 0; i < map.size(); i++)
	{
		std::cerr << map[i].type;
		if (i % maxX == (maxX - 1))
			std::cerr << std::endl;
	}
	std::cerr << std::endl;
}

std::string move(int posX, int posY, Node path) {
	if (path.x < posX)
	{
		return "LEFT";
	}
	if (path.x > posX)
	{
		return "RIGHT";
	}
	if (path.y < posY)
	{
		return "UP";
	}
	if (path.y > posY)
	{
		return "DOWN";
	}
}

int main() {

	std::vector<Node> map;

	int Y; // number of rows.
	int X; // number of columns.
	int A; // number of rounds between the time the alarm countdown is activated and the time the alarm goes off.

	std::cin >> Y >> X >> A;

	for (int i = 0; i < Y; i++)
		for (int j = 0; j < X; j++)
			map.push_back(Node(j, i, '?'));

	for (Node& n : map)
		n.addNeighbors(map, X, Y);

	// activate exploring mode, explores until everything is discovered. 
	char goal = '?';

	// game loop
	while (1) {
		int posY; // row where Kirk is located.
		int posX; // column where Kirk is located.

		std::cin >> posY >> posX; std::cin.ignore();

		for (int i = 0; i < Y; i++) {
			std::string row; // C of the characters in '#.TC?' (i.e. one line of the ASCII maze).
			std::getline(std::cin, row);
			for (size_t j = 0; j < row.size(); j++)
			{
				getNode(map, j, i).type = row[j];
			}
		}
		printMap(map, X);

		// hit the goal, going back home now
		if (getNode(map, posX, posY).type == 'C')
			goal = 'T';

		std::vector<Node> path = BFS(map, posX, posY, goal);

		// Print path (coords)
		std::cerr << "Path: " << path.size() << " cells, ";
		for (auto p : path)
		{
			std::cerr << " -> " << p;
		}
		std::cerr << std::endl;

		// Translate coords to 'UP', 'DOWN', 'LEFT' or 'RIGHT'
		std::string dir = move(posX, posY, path[0]);
		std::cout << dir << std::endl; // Kirk's next move (UP DOWN LEFT or RIGHT).
	}


	return 0;
}