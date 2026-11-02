#include <bits/stdc++.h>
#include <chrono>
using namespace std;

#define fastio() {ios::sync_with_stdio(0); cin.tie(0);}

const int MAXN = 11, INF = 1e9;
int N, M, S;

struct Edge {
	int v, w;
	Edge() {}
	Edge(int v, int w) :v(v), w(w) {}
};
vector<Edge> G[MAXN];
int Gstar[MAXN][MAXN]; // = G*
int C[MAXN];

void init() {
	fastio();
	cin >> N >> M;
	cin >> S;
	int u, v, w;
	for (int i = 0; i < M; i++) {
		cin >> u >> v >> w;
		G[u].emplace_back(v, w);
		G[v].emplace_back(u, w);
	}

	for (int i = 1; i <= N; i++) cin >> C[i];
}


void Floyd_Warshall() {
	for (int u = 1; u <= N; u++) {
		fill(Gstar[u], Gstar[u] + MAXN, INF);
		Gstar[u][u] = 0;
	}

	for (int u = 1; u <= N; u++) {
		for (auto [v, w] : G[u]) {
			Gstar[u][v] = min(Gstar[u][v], w);
		}
	}

	for (int w = 1; w <= N; w++) {
		for (int u = 1; u <= N; u++) {
			for (int v = 1; v <= N; v++) {
				Gstar[u][v] = min(Gstar[u][v], Gstar[u][w] + Gstar[w][v]);
			}
		}
	}
}

int dp[1 << MAXN][MAXN];
int bitmask(int S) { // bitmask dp
	for (int bit = 0; bit < (1 << N); bit++) fill(dp[bit], dp[bit] + MAXN, INF);

	dp[1 << (S - 1)][S] = 0;

	for (int bit = 0; bit < (1 << N); bit++) {
		for (int u = 1; u <= N; u++) {
			if (!(bit & (1 << (u - 1))) || dp[bit][u] == INF) continue;

			for (int v = 1; v <= N; v++) {
				if (bit & (1 << (v - 1))) continue;

				int nbit = bit | (1 << (v - 1));
				int nw = dp[bit][u] + Gstar[u][v];

				if (nw < dp[nbit][v]) dp[nbit][v] = nw;
			}
		}
	}

	int ans = INF;
	for (int u = 1; u <= N; u++) ans = min(ans, dp[(1 << N) - 1][u]);

	for (int i = 1; i <= N; i++) ans += C[i];

	return ans;
}

bool vis[MAXN];
int HeuristicSearch(int S) {
	int res = 0;
	for (int i = 1; i <= N; i++) res += C[i];
	int u = S;
	for (int i = 1; i < N; i++) {
		vis[u] = true;
		int minv = 0;
		for (int v = 1; v <= N; v++) {
			if (vis[v]) continue;
			if (Gstar[u][v] < Gstar[u][minv]) minv = v;
		}
		res += Gstar[u][minv];
		u = minv;
	}

	return res;
}

int main() {
	init();
	Floyd_Warshall();

	// 완전 탐색
	cout << "<Brute Force>\n";
	auto start = chrono::steady_clock::now();
	cout << "Result: " << bitmask(S) << '\n';
	auto end = chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> duration = end - start;
	cout << "Time Spent: " << duration.count() << " ms\n";


	// 휴리스틱
	cout << "<Heuristic>\n";
	start = chrono::steady_clock::now();
	cout << "Result: " << HeuristicSearch(S) << '\n';
	end = chrono::steady_clock::now();
	duration = end - start;
	cout << "Time Spent: " << duration.count() << " ms\n";


	return 0;
}
