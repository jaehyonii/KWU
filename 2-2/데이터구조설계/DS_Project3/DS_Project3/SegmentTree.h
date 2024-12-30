#pragma once
#include<vector>

using namespace std;

int init(int node, int start, int end, vector<int>& _seg) {
	// Start : Arr의 시작 index
	// end : Arr의 마지막 index
	// node : segment tree의 노드

	if (start == end) return _seg[node] = 1;
	int mid = (start + end) / 2;
	init(node * 2, start, mid, _seg); // 왼쪽 자식 노드
	init(node * 2 + 1, mid + 1, end, _seg); // 오른쪽 자식 노드
	_seg[node] = _seg[node * 2] + _seg[node * 2 + 1];
}

void update(int node, int start, int end, int target, int diff_value, vector<int>& _seg) {
	// 구간 내에 없을 경우
	if (target < start || target > end) return;

	_seg[node] += diff_value;

	if (start != end) {
		int mid = (start + end) / 2;
		update(node * 2, start, mid, target, diff_value, _seg);
		update(node * 2 + 1, mid + 1, end, target, diff_value, _seg);
	}
}

int sum(int node, int start, int end, int left, int right, vector<int>& _seg) {
	// 구간 내에 없을 경우
	if (left > end || right < start) return 0;

	// 구간 내에 완전히 포함되는 경우
	if (left <= start && end <= right) return _seg[node];

	// 일부분 겹치는 나머지 경우
	int mid = (start + end) / 2;
	return sum(node * 2, start, mid, left, right, _seg) + sum(node * 2 + 1, mid + 1, end, left, right, _seg);
}