#ifndef MYGRAPHIC_H
#define MYGRAPHIC_H

#include <unordered_set>
#include <unordered_map>
#include "MyVector.h"
#include "MyQueue.h"
#include "MyStack.h"
#include "MyPriorityQueue.h"

class GraphicNode;

class GraphicEdge {
public:
	int weight;
	GraphicNode* from;
	GraphicNode* to;
	GraphicEdge() {
		from = NULL;
		to = NULL;
	}
	GraphicEdge(int weight, GraphicNode* from, GraphicNode* to) {
		this->weight = weight;
		this->from = from;
		this->to = to;
	}
	~GraphicEdge() {
	}
};

class GraphicNode {
public:
	int value;
	size_t in;
	size_t out;
	MyVector<GraphicNode*> nexts;
	MyVector<GraphicEdge*> edges;
	GraphicNode() {
		value = 0;
		in = 0;
		out = 0;
	}
	GraphicNode(int value) {
		this->value = value;
		in = 0;
		out = 0;
	}
};
//并查集的封装，方便用于Kruskal算法
class MyUnionFind {
private:
	std::unordered_map<GraphicNode*, GraphicNode*> fatherMap;
	std::unordered_map<GraphicNode*, int> sizeMap;
public:
	void makeSets(MyVector<GraphicNode*> nodes) {
		fatherMap.clear();
		sizeMap.clear();
		for (GraphicNode* node : nodes) {
			fatherMap.insert({ node, node });
			sizeMap.insert({ node,1 });
		}
	}
	GraphicNode* findFather(GraphicNode* n) {
		MyStack<GraphicNode*> path;
		while (n != fatherMap[n]) {
			path.push(n);
			n = fatherMap[n];
		}
		while (!path.empty()) {
			fatherMap.insert({ path.top(),n });
			path.pop();
		}
		return n;
	}
	bool isSameSet(GraphicNode* a, GraphicNode* b) {
		return findFather(a) == findFather(b);
	}
	void unionset(GraphicNode* a, GraphicNode* b) {
		if (a == NULL || b == NULL)
			return;
		GraphicNode* aDai = findFather(a);
		GraphicNode* bDai = findFather(b);
		if (aDai != bDai) {
			int aSetSize = sizeMap[aDai];
			int bSetSize = sizeMap[bDai];
			if (aSetSize <= bSetSize) {
				fatherMap[aDai] = bDai;
				sizeMap[bDai] = aSetSize + bSetSize;
				sizeMap.erase(aDai);
			}
			else {
				fatherMap[bDai] = aDai;
				sizeMap[aDai] = aSetSize + bSetSize;
				sizeMap.erase(bDai);
			}
		}
	}
};

class MyCmp {
public:
	bool operator()(GraphicEdge* a, GraphicEdge* b) {
		return a->weight < b->weight;
	}
};

GraphicNode* getMinDistanceAndUnselectedNode(std::unordered_map<GraphicNode*,int> distanceMap,std::unordered_set<GraphicNode*> touchedNodes) {
	GraphicNode* minNode = NULL;
	int minDistance = 0x3f3f3f3f;
	for (auto i : distanceMap) {
		GraphicNode* node = i.first;
		int distance = i.second;
		if (!touchedNodes.count(node) && distance < minDistance) {
			minNode = node;
			minDistance = distance;
		}
	}
	return minNode;
}

class MyGraphic{
public:
	std::unordered_map<int, GraphicNode*> nodes;
	std::unordered_set<GraphicEdge*> edges;
	
	~MyGraphic() {
		for (auto i : edges) 
			delete i;
		for (auto i : nodes)
			delete i.second;
	}

	void add_edge_weight(int from, int to, int weight) {
		if (!nodes.count(from))
			nodes.insert({ from,new GraphicNode(from) });
		if (!nodes.count(to))
			nodes.insert({ to,new GraphicNode(to) });
		GraphicNode* fromNode = nodes[from];
		GraphicNode* toNode = nodes[to];
		GraphicEdge* newEdge = new GraphicEdge(weight, fromNode, toNode);
		fromNode->nexts.push_back(toNode);
		fromNode->out++;
		toNode->in++;
		fromNode->edges.push_back(newEdge);
		edges.insert(newEdge);
	}
	void add_edge(int from, int to) {
		if (!nodes.count(from))
			nodes.insert({ from,new GraphicNode(from) });
		if (!nodes.count(to))
			nodes.insert({ to,new GraphicNode(to) });
		GraphicNode* fromNode = nodes[from];
		GraphicNode* toNode = nodes[to];
		GraphicEdge* newEdge = new GraphicEdge(1, fromNode, toNode);
		fromNode->nexts.push_back(toNode);
		fromNode->out++;
		toNode->in++;
		fromNode->edges.push_back(newEdge);
		edges.insert(newEdge);
	}

	void bfs(GraphicNode* start) {
		if (start == NULL)
			return;
		MyQueue<GraphicNode*> q;
		std::unordered_set<GraphicNode*> s;
		q.push(start);
		s.insert(start);
		while (!q.empty()) {
			GraphicNode* cur = q.front();
			q.pop();
			std::cout << cur->value << std::endl;
			for (GraphicNode* next : cur->nexts) {
				if (!s.count(next)) {
					s.insert(next);
					q.push(next);
				}
			}
		}
	}
	void dfs(GraphicNode* start) {
		if (start == NULL)
			return;
		MyStack<GraphicNode*> stk;
		std::unordered_set<GraphicNode*> s;
		stk.push(start);
		s.insert(start);
		std::cout << start->value << std::endl;
		while (!stk.empty()) {
			GraphicNode* cur = stk.top();
			stk.pop();
			for (GraphicNode* next : cur->nexts) {
				if (!s.count(next)) {
					stk.push(cur);
					stk.push(next);
					s.insert(next);
					std::cout << next->value << std::endl;
					break;
				}
			}
		}
	}
	MyVector<GraphicNode*> topology_sort() {
		std::unordered_map<GraphicNode*, size_t> inMap;
		MyQueue<GraphicNode*> zeroInQueue;
		for (auto node : nodes) {
			inMap.insert({ node.second,node.second->in });
			if (node.second->in == 0)
				zeroInQueue.push(node.second);
		}
		MyVector<GraphicNode*> result;
		while (!zeroInQueue.empty()) {
			GraphicNode* cur = zeroInQueue.front();
			zeroInQueue.pop();
			result.push_back(cur);
			for (GraphicNode* next : cur->nexts) {
				inMap[next]--;
				if (inMap[next] == 0)
					zeroInQueue.push(next);
			}
		}
		return result;
	}
	std::unordered_set<GraphicEdge*> kruskal() {
		MyUnionFind unionFind;
		MyVector<GraphicNode*> values;
		for (auto value : nodes)
			values.push_back(value.second);
		unionFind.makeSets(values);
		MyPriorityQueue<GraphicEdge*, MyVector<GraphicEdge*>, MyCmp > min_q;
		for (GraphicEdge* edge : edges)
			min_q.push(edge);
		std::unordered_set<GraphicEdge*> result;
		while (!min_q.empty()) {
			GraphicEdge* edge = min_q.top();
			min_q.pop();
			if (!unionFind.isSameSet(edge->from, edge->to)) {
				result.insert(edge);
				unionFind.unionset(edge->from,edge->to);
			}
		}
		return result;
	}
	std::unordered_set<GraphicEdge*> prim() {
		MyPriorityQueue<GraphicEdge*, MyVector<GraphicEdge*>, MyCmp> min_q;
		std::unordered_set<GraphicNode*> nodeSet;
		std::unordered_set<GraphicEdge*> result;
		for (auto node : nodes) {
			if (!nodeSet.count(node.second)) {
				nodeSet.insert(node.second);
				for (GraphicEdge* edge : node.second->edges)
					min_q.push(edge);
				while (!min_q.empty()) {
					GraphicEdge* edge = min_q.top();
					min_q.pop();
					GraphicNode* toNode = edge->to;
					if (!nodeSet.count(toNode)) {
						nodeSet.insert(toNode);
						result.insert(edge);
						for (GraphicEdge* nextEdge : toNode->edges)
							min_q.push(nextEdge);
					}
				}
			}
		}
		return result;
	}
	std::unordered_map<GraphicNode*, int> dijkstra(GraphicNode* from) {
		std::unordered_map<GraphicNode*, int> distanceMap;
		distanceMap[from] = 0;
		std::unordered_set<GraphicNode*> selectedNodes;
		GraphicNode* minNode = getMinDistanceAndUnselectedNode(distanceMap, selectedNodes);
		while (minNode != NULL) {
			int distance = distanceMap[minNode];
			for (GraphicEdge* edge : minNode->edges) {
				GraphicNode* toNode = edge->to;
				if (!distanceMap.count(toNode))
					distanceMap[toNode] = distance + edge->weight;
				else
					distanceMap[edge->to] = std::min(distanceMap[toNode], distance + edge->weight);
			}
			selectedNodes.insert(minNode);
			minNode = getMinDistanceAndUnselectedNode(distanceMap, selectedNodes);
		}
		return distanceMap;
	}
};

#endif