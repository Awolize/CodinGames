#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>

class Node
{
public:
	Node() {};
	bool gateway = 0; // is gateway?
	std::set<int> gateways;
	std::set<int> neighbors;
};

std::pair<int, int> BFS(std::vector<Node>& map, int SkynetIndex)
{
	std::queue<int> q; // Nodes to explore
	std::vector<bool> visited(map.size(), false); // Visited Nodes
	
	int selected = -1;

	q.push(SkynetIndex);
	visited[SkynetIndex] = true;

	while (!q.empty())
	{
		int curr = q.front();
		q.pop();
		visited[curr] = true;

		if (map[curr].gateways.size() > 1)
		{
			return std::make_pair(curr, *(map[curr].gateways.begin()));
		}
		else if (map[curr].gateways.size() == 1)
		{
			if (curr == SkynetIndex)
				return std::make_pair(curr, *(map[curr].gateways.begin()));
			else if (selected == -1)
				selected = curr;
		}

		if (selected == -1 || map[curr].gateways.size() == 1)
		{
			for (auto &child : map[curr].neighbors)
			{
				if (visited[child])
					continue;

				q.push(child);
			}
		}
	}
	return std::make_pair(selected, *(map[selected].gateways.begin()));
}

int main()
{
	int N; // the total number of nodes in the level, including the gateways
	int L; // the number of links
	int E; // the number of exit gateways
	std::cin >> N >> L >> E; std::cin.ignore();

	std::vector<Node> map;
	for (int i = 0; i < N; i++)
		map.push_back(Node());


	for (int i = 0; i < L; i++) {
		int N1; // N1 and N2 defines a link between these nodes
		int N2;
		std::cin >> N1 >> N2; std::cin.ignore();

		map[N1].neighbors.insert(N2);
		map[N2].neighbors.insert(N1);
	}
	for (int i = 0; i < E; i++) {
		int EI; // the index of a gateway node
		std::cin >> EI; std::cin.ignore();

		map[EI].gateway = 1;
		for (auto neighbor : map[EI].neighbors)
			map[neighbor].gateways.insert(EI);
	}

	// Game loop
	while (1) {

		int SI; // The index of the node on which the Skynet agent is positioned this turn
		std::cin >> SI; std::cin.ignore();
		std::cerr << "Enemy position: " << SI << std::endl;

		// BFS
		std::pair<int, int> path = BFS(map, SI);

		// Sever links
		map[path.first].gateways.erase(path.second);
		map[path.first].neighbors.erase(path.second);
		map[path.second].neighbors.erase(path.first);

		// Print path (coords)
		std::cerr << "Cutting: " << path.first << " " << path.second << std::endl;
		std::cout << path.first << " " << path.second << std::endl;
	}

	return 0;
}