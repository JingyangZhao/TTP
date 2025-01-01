#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cmath>

using namespace std;

// sigma[u] -> sigma[v] on the d-th day
void fight(vector<int> &sigma, int &u, int &v, int &d, vector<vector<int>> &table, int fff)
{
	table[sigma[u]][d] = fff * sigma[v];
	table[sigma[v]][d] = -fff * sigma[u];
}

// \Gamma_{t} t\in {1,..., n-1}
void Gamma(vector<int> &sigma, int t, int &l, int &k, int &n, int d, vector<vector<int>> &table, int FFF)
{

	int m = n / 2;
	int b = ceil((m - l - 1) * 1.0 / k) + 2;

	vector<int> CYCLE(n - 1);

	int position = 2 - t;
	if (position < 0)
	{
		position += n - 1;
	}

	for (int i = 1; i <= n - 1; i++)
	{
		CYCLE[position % (n - 1)] = i;
		position += 2;
	}

	int counter = 1;

	for (int i = 1; i <= b; i++)
	{

		int wit = 1;
		if (i == 1)
		{
			int fff = FFF; // fff = FFF means that the directions of the most edges in the 1-th block are from left to right

			if (((t - 1) / k) % 2 == 1)
			{
				fff = -FFF;
			}

			fight(sigma, n, CYCLE[0], d, table, fff);

			// the speical case: reverse the most left edge only on the 1-st day
			int r = (n - 1) % (2 * k);
			if (t == 1 && r <= k && k < n - 1)
			{
				fight(sigma, n, CYCLE[0], d, table, -FFF);
			}

			continue;
		}
		else if (i == b)
		{
			wit = l;
		}
		else if (i == 2)
		{
			wit = m - 1 - (b - 3) * k - l;
		}
		else
		{
			wit = k;
		}

		int fff = FFF; // fff = FFF means that the directions of the edges in the i-th block are from above to below
		if (i % 2 == 1)
		{
			fff = -FFF;
		}

		for (int j = 1; j <= wit; j++)
		{
			int upper = counter, lower = n - 1 - upper;
			fight(sigma, CYCLE[upper], CYCLE[lower], d, table, fff);
			counter++;
		}

		if (i == 2 && wit == k)
		{
			fight(sigma, CYCLE[1], CYCLE[n - 2], d, table, -fff);
		}
	}
}

int CONS(vector<int> &sigma, int &n, int &l, int &k, vector<vector<int>> &table, int FFF = 1)
{
	// Gamma_1,...., Gamma_{n-1}
	for (int i = 1; i <= n - 1; i++)
	{
		Gamma(sigma, i, l, k, n, i, table, FFF);
	}

	// ~Gamma_{n-2}, ~Gamma_{n-1}, ~Gamma_1,...., ~Gamma_{n-3}
	for (int i = 1; i <= n - 1; i++)
	{
		Gamma(sigma, (n - 4 + i) % (n - 1) + 1, l, k, n, n - 1 + i, table, -FFF);
	}

	return 0;
}

// present the shcedule
void show_table(vector<vector<int>> &table, int n)
{
	printf("/**************table****************/\n");
	for (int i = 1; i <= n; i++, printf("\n"))
	{
		for (int j = 1; j <= 2 * n - 2; j++)
		{
			printf("%3d", table[i][j]);
		}
	}
}

// check the feasibility of the table
int check(int &n, int &k, vector<vector<int>> &table)
{
	// check: missed/repeated games
	vector<vector<int>> flag(n + 1, vector<int>(n + 1));

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (i != j)
			{
				flag[i][j] = 200;
			}
		}
	}
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= 2 * n - 2; j++)
		{
			if (table[i][j] > 0)
			{
				flag[i][table[i][j]] += (-100 + 1);
			}
			else if (table[i][j] < 0)
			{
				flag[i][-table[i][j]] += (-100 - 1);
			}
		}
	}
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (flag[i][j] != 0)
			{
				printf("Missed/Repeated games: %d %d\n", i, j);
				return -1;
			}
		}
	}

	// check: the no-repeat constraint
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= 2 * n - 3; j++)
		{
			if (abs(table[i][j]) == abs(table[i][j + 1]))
			{
				printf("Repeated opponents: %d %d\n", i, j);
			}
		}
	}
	// check: the at-most-k constraint
	int max_consecutive = 0;
	for (int i = 1; i <= n; i++)
	{
		int max_home = 0, max_away = 0;

		for (int d = 1; d <= 2 * n - 2; d++)
		{
			if (table[i][d] > 0)
			{
				max_consecutive = max(max_consecutive, max_home);
				max_home = 0;
				max_away++;
			}
			else
			{
				max_consecutive = max(max_consecutive, max_away);
				max_away = 0;
				max_home++;
			}
		}

		max_consecutive = max(max_consecutive, max_away);
		max_consecutive = max(max_consecutive, max_home);

		if (max_consecutive > k)
		{
			cout << "The table violates the at-most-k constraint" << endl;
			return -1;
		}
	}

	for (int i = 1; i <= n; i++)
	{
		for (int d = 1; d <= n * 2 - 2; d++)
		{
			int opponent = abs(table[i][d]);
			if (abs(table[opponent][d]) != i)
			{
				printf("The table is not symetric: %d %d\n ", i, d);
				return -1;
			}
		}
	}

	return 0;
}

int main()
{
	int n = 20, k = 4, l = 2;
	vector<int> sigma(n + 1);
	vector<vector<int>> table(n + 1, vector<int>(2 * n - 2 + 1));

	for (int i = 1; i <= n; i++)
	{
		sigma[i] = i;
	}

	CONS(sigma, n, l, k, table, 1);

	show_table(table, n);

	check(n, k, table);


	// for (int n = 4; n <= 40; n++, n++)
	// {

	// 	cout << n << endl;

	// 	vector<int> sigma(n + 1);
	// 	vector<vector<int>> table(n + 1, vector<int>(2 * n - 2 + 1));

	// 	for (int i = 1; i <= n; i++)
	// 	{
	// 		sigma[i] = i;
	// 	}
	// 	for (int i = 2; i < n; i++)
	// 	{
	// 		int k = i;
	// 		int t = min(k, n / 2 - 1);
	// 		for (int j = 1; j <= t; j++)
	// 		{
	// 			int l = j;
	// 			CONS(sigma, n, l, k, table, 1);
	// 			check(n, k, l, table);
	// 		}
	// 	}
	// }

	system("pause");

	return 0;
}