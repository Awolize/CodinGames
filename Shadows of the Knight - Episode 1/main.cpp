#include <iostream>
#include <string>

using namespace std;

int main()
{
	int W; // width of the building.
	int H; // height of the building.
	cin >> W >> H; cin.ignore();
	int N; // maximum number of turns before game over.
	cin >> N; cin.ignore();
	int x;
	int y;
	cin >> x >> y; cin.ignore();

	// limits
	int minX = 0;
	int minY = 0;
	int maxX = W - 1;
	int maxY = H - 1;

	// game loop
	while (1) {
		string bombDir; // the direction of the bombs from batman's current location (U, UR, R, DR, D, DL, L or UL)
		cin >> bombDir; cin.ignore();

		if (bombDir.find("R") != string::npos) minX = x + 1;
		if (bombDir.find("L") != string::npos) maxX = x - 1;
		if (bombDir.find("D") != string::npos) minY = y + 1;
		if (bombDir.find("U") != string::npos) maxY = y - 1;

		x = minX + (maxX - minX) / 2;
		y = minY + (maxY - minY) / 2;

		cout << x << " " << y << endl;
	}
}