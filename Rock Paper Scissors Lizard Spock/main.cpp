#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iterator>
#include <sstream>

using namespace std;

class Player {
public:
	Player(int id, string signplay) : id(id), signplay(signplay) {};
	int id = 0;
	string signplay = "";
};

int main()
{
	vector<Player> players;
	unordered_map<int, vector<int>> match_history;

	int N;
	cin >> N; cin.ignore();
	for (int i = 0; i < N; i++) {
		int NUMPLAYER;
		string SIGNPLAYER;
		cin >> NUMPLAYER >> SIGNPLAYER; cin.ignore();
		players.push_back(Player(NUMPLAYER, SIGNPLAYER));
	}

	// Loop until there is only 1 player left
	while (players.size() > 1)
	{
		vector<Player> winners;
		// Face each player against each other
		for (int i = 0; i < players.size(); i += 2)
		{
			Player p1 = players[i];
			Player p2 = players[i + 1];

			match_history[p1.id].push_back(p2.id);
			match_history[p2.id].push_back(p1.id);

			// TIE, base winner on lowest id
			if (p1.signplay == p2.signplay)
			{
				if (p1.id < p2.id)
					winners.push_back(p1);
				else
					winners.push_back(p2);
			}
			// Did p1 win?
			else if (
				(p1.signplay == "C" && p2.signplay == "P") ||
				(p1.signplay == "P" && p2.signplay == "R") ||
				(p1.signplay == "R" && p2.signplay == "L") ||
				(p1.signplay == "L" && p2.signplay == "S") ||
				(p1.signplay == "S" && p2.signplay == "C") ||
				(p1.signplay == "C" && p2.signplay == "L") ||
				(p1.signplay == "L" && p2.signplay == "P") ||
				(p1.signplay == "P" && p2.signplay == "S") ||
				(p1.signplay == "S" && p2.signplay == "R") ||
				(p1.signplay == "R" && p2.signplay == "C"))

				winners.push_back(p1);

			// Then p2 won
			else
				winners.push_back(p2);
		}
		players = winners;
	}

	// Output winner
	cout << players[0].id << endl;

	// Extract match history and format
	std::stringstream ss;
	std::ostream_iterator<int> out_it(ss, " ");
	copy(match_history[players[0].id].begin(), match_history[players[0].id].end(), out_it);
	string output = ss.str();
	cout << output.substr(0, output.size() - 1) << endl;
}