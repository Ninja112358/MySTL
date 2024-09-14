#include <iostream>
#include "MyGraphic.h"
#include "MemoryLeakCheck.h"

using namespace std;

int main() {
	//检查是否有内存在程序结束后有没有释放
	//如果有没释放的内存会有提示的
	//如果没有,就没有任何提示
	_SetCheckStatus s;

	MyGraphic graph_direction;
	MyGraphic graph;

	int n;
	//测试数据
	//个数:6
	//顶点:1 4 8 1 2 5 4 5 2 5 2 4 3 4 1 2 3 7
	cout << "请输入有向图的边的个数:";
	cin >> n;
	int a, b, weight;
	cout << "请输入图的(顶点 顶点 权值):" << endl;
	for (int i = 0; i < n; i++) {
		cin >> a >> b >> weight;
		//无向图建立边建两次有向的
		graph.add_edge_weight(a, b, weight);
		graph.add_edge_weight(b, a, weight);
		//有向图
		graph_direction.add_edge_weight(a, b, weight);
	}

	cout << "演示bfs:" << endl;
	graph.bfs(graph.nodes[1]);
	cout << endl;
	cout << "演示dfs:" << endl;
	graph.dfs(graph.nodes[1]);
	cout << endl;
	cout << "演示拓扑排序:" << endl;
	auto v = graph_direction.topology_sort();
	for (auto i : v)
		cout << i->value << " ";
	cout << endl;

	cout << "演示最小生成树(Kruskal):" << endl;
	auto edgeSetKruskal = graph.kruskal();
	for (auto i : edgeSetKruskal)
		cout << i->from->value << "->" << i->to->value << " 权值:" << i->weight << endl;
	cout << endl;
	cout << "演示最小生成树(Prim):" << endl;
	auto edgeSetPrim = graph.prim();
	for (auto i : edgeSetPrim)
		cout << i->from->value << "->" << i->to->value << " 权值:" << i->weight << endl;
	cout << endl;

	cout << "演示最短路径(Dijkstra):" << endl;
	auto edgeSetDijkstra = graph.dijkstra(graph.nodes[1]);
	for (auto i : edgeSetDijkstra)
		cout << graph.nodes[1]->value << "->" << i.first->value << " 最短路径值:" << i.second << endl;
	cout << endl;

	return 0;
}