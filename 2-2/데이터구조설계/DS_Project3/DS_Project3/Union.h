#pragma once

class Union {
private: 
	int* parent;

public:
	Union(int size) {
		parent = new int[size + 1];
		for (int i = 0; i <= size; i++)
			parent[i] = -1;
	}
	~Union() {
		delete[] parent;
	}

	// Union Function
	void Merge(int x, int y) {
		parent[x] = y;
	}

	// Find
	int Find(int vertex) {
		while (parent[vertex] >= 0)
			vertex = parent[vertex];
		return vertex;
	}

	// check if x, y are in the same union
	bool isUnion(int x, int y) {
		x = Find(x);
		y = Find(y);

		if (x == y)
			return true;
		else return false;
	}
};