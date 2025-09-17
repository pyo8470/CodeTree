#define _CRT_SECURE_NO_WARNINGS 
#include <iostream> 
#include <algorithm> 
#include <vector> 
#include <cstring> 
#include <queue> 
#include <set> 
#include<unordered_map> 
#include <unordered_set>
#include <iomanip>
#include <climits> 
#define MAX_N 15 
#define MAX_Q 51 
using namespace std;

int N, Q;
int PLATE[MAX_N][MAX_N];
int union_count[MAX_Q];
bool visited[MAX_N][MAX_N];

struct Virus {
	int ID, size;
	int sX, sY;
	vector<vector<int>> shape;
	bool operator<(const Virus& other) const {
		if (size != other.size) return size > other.size;
		return ID < other.ID;
	}
};
unordered_map<int, Virus> virus_map;

struct State { int x, y; };
int dx[4] = { 0, 0, -1, 1 };
int dy[4] = { -1, 1, 0, 0 };

struct pairHash {
	size_t operator()(const pair<int, int>& p) const {
		return hash<int>{}(p.first) * 131 + hash<int>{}(p.second);
	}
};

void updateShape(Virus& v) {
	if (v.size == 0) { v.shape.clear(); v.sX = v.sY = 0; return; }

	int minY = INT_MAX, minX = INT_MAX;
	int maxY = -1, maxX = -1;

	for (int i = 0; i < v.shape.size(); i++)
		for (int j = 0; j < v.shape[i].size(); j++)
			if (v.shape[i][j]) {
				minY = min(minY, i);
				minX = min(minX, j);
				maxY = max(maxY, i);
				maxX = max(maxX, j);
			}

	vector<vector<int>> newShape(maxY - minY + 1, vector<int>(maxX - minX + 1, 0));
	for (int i = minY; i <= maxY; i++)
		for (int j = minX; j <= maxX; j++)
			if (v.shape[i][j]) newShape[i - minY][j - minX] = v.shape[i][j];

	v.shape = newShape;
	v.sY += minY;
	v.sX += minX;
}

void fillPlate(int ID, int r1, int c1, int r2, int c2) {
	Virus& curVirus = virus_map[ID];
	for (int y = r1; y <= r2; y++) {
		for (int x = c1; x <= c2; x++) {
			int curID = PLATE[y][x];
			if (curID) {
				Virus& prev = virus_map[curID];
				prev.size--;
				prev.shape[y - prev.sY][x - prev.sX] = 0;
				if (prev.size > 0) updateShape(prev);
			}
			curVirus.shape[y - r1][x - c1] = ID;
			PLATE[y][x] = ID;
		}
	}
}

int bfs(int sx, int sy, int ID) {
	queue<State> q; q.push({ sx, sy });
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		visited[cur.y][cur.x] = true;
		for (int i = 0; i < 4; i++) {
			int nx = cur.x + dx[i], ny = cur.y + dy[i];
			if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
			if (visited[ny][nx] || PLATE[ny][nx] != ID) continue;
			visited[ny][nx] = true;
			q.push({ nx, ny });
		}
	}
	return 1;
}

void removeDuplication() {
	for (int y = 0; y < N; y++)
		for (int x = 0; x < N; x++)
			if (!visited[y][x] && PLATE[y][x])
				union_count[PLATE[y][x]] += bfs(x, y, PLATE[y][x]);

	for (int y = 0; y < N; y++)
		for (int x = 0; x < N; x++) {
			int ID = PLATE[y][x];
			if (union_count[ID] > 1) {
				PLATE[y][x] = 0;
				virus_map[ID].size = 0;
				virus_map[ID].shape.clear();
			}
		}
}

bool canPlace(int i, int j, Virus& v) {
	for (int y = 0; y < v.shape.size(); y++)
		for (int x = 0; x < v.shape[0].size(); x++)
			if (v.shape[y][x] && PLATE[i + y][j + x]) return false;
	return true;
}

void moveViruses(vector<int>& ids) {
	int temp[MAX_N][MAX_N];
	memcpy(temp, PLATE, sizeof(PLATE));
	memset(PLATE, 0, sizeof(PLATE));

	for (int ID : ids) {
		Virus& v = virus_map[ID];
		if (v.size == 0) continue;

		bool placed = false;
		for (int i = 0; i <= N - v.shape.size() && !placed; i++)
			for (int j = 0; j <= N - v.shape[0].size() && !placed; j++)
				if (canPlace(i, j, v)) {
					v.sY = i; v.sX = j;
					placed = true;
				}

		if (!placed) { v.size = 0; v.shape.clear(); continue; }

		for (int y = 0; y < v.shape.size(); y++)
			for (int x = 0; x < v.shape[0].size(); x++)
				if (v.shape[y][x]) PLATE[v.sY + y][v.sX + x] = ID;
	}
}

int calculateScore() {
	unordered_set<pair<int, int>, pairHash> pairs;
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			int cur = PLATE[y][x];
			if (!cur) continue;
			for (int i = 0; i < 4; i++) {
				int nx = x + dx[i], ny = y + dy[i];
				if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;

				int other = PLATE[ny][nx];
				if (!other || other == cur) continue;

				int a = max(cur, other), b = min(cur, other);
				pairs.insert({ a, b });
			}
		}
	}
	int ret = 0;
	for (auto& p : pairs) ret += virus_map[p.first].size * virus_map[p.second].size;
	return ret;
}

int main() {
	freopen("input.txt", "r", stdin);
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> N >> Q;
	for (int i = 1; i <= Q; i++) {
		memset(visited, 0, sizeof(visited));
		memset(union_count, 0, sizeof(union_count));

		int r1, c1, r2, c2;
		cin >> r1 >> c1 >> r2 >> c2;
		r2--; c2--;

		Virus v;
		v.ID = i;
		v.size = (r2 + 1 - r1) * (c2 + 1 - c1);
		v.sX = c1; v.sY = r1;
		v.shape.assign(r2 - r1 + 1, vector<int>(c2 - c1 + 1, 0));
		virus_map[i] = v;

		fillPlate(i, r1, c1, r2, c2);
		removeDuplication();

		vector<int> ids;
		for (int j = 1; j <= Q; j++) if (virus_map[j].size > 0) ids.push_back(j);
		sort(ids.begin(), ids.end(), [](int a, int b) {
			if (virus_map[a].size != virus_map[b].size) return virus_map[a].size > virus_map[b].size;
			return a < b;
			});

		moveViruses(ids);

		cout << calculateScore() << '\n';
	}
}
