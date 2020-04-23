#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <map>
#include <unordered_map>
#include <list> 
#include <queue>
#include <sstream>

using namespace std;

class Node
{
public:
	Node(int x, int y, char type) : x(x), y(y), type(type) {};

	int x;
	int y;
	char type;
	std::vector<Node*> neighbors;
};

/**
 * Don't let the machines win. You are humanity's last hope...
 **/
int main()
{
	int width; // the number of cells on the X axis
	int height; // the number of cells on the Y axis
	cin >> width; cin.ignore();
	cin >> height; cin.ignore();
	cerr << "input: " << width << endl;
	cerr << "input: " << height << endl;

	vector<vector<string>> vector(height, std::vector<string>(width, "-"));

	for (int i = 0; i < height; i++) {
		string line; // width characters, each either 0 or .
		getline(cin, line);
		cerr << "input: " << line << endl;
		for (size_t j = 0; j < line.size(); j++)
		{
			vector[i][j] = line[j];
		}
	}

	// Debug
	for (auto it : vector)
	{
		for (auto itt : it)
			cerr << itt << " ";
		cerr << endl;
	}

	// Three coordinates: a node, its right neighbor, its bottom neighbor

	for (int i = 0; i < vector.size(); i++)
	{
		for (int j = 0; j < vector[i].size(); j++)
		{
			if (vector[i][j] != ".")
			{
				string result = "";

				// The current node
				result += to_string(j) + " " + to_string(i) + " ";

				// Cloest right neighbor
				int rx = -1;
				for (size_t x = j + 1; x < vector[i].size(); x++)
				{
					if (vector[i][x] != ".")
					{
						rx = x;
						break;
					}
				}

				if (rx != -1)
					result += to_string(rx) + " " + to_string(i) + " ";
				else
					result += to_string(-1) + " " + to_string(-1) + " ";

				// Closest bottom neighbor
				int by = -1;
				for (size_t x = i + 1; x < vector.size(); x++)
				{
					if (vector[x][j] != ".")
					{
						by = x;
						break;
					}
				}

				if (by != -1)
					result += to_string(j) + " " + to_string(by) + " ";
				else
					result += to_string(-1) + " " + to_string(-1) + " ";

				cerr << result.substr(0, result.size() - 1) << endl;
				cout << result.substr(0, result.size() - 1) << endl;
			}
		}
	}


}