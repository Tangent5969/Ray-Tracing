#ifndef BVH_H
#define BVH_H

#include "Object.h"
#include <iostream>

#define BVHStackSize 32

struct BVHNode {
	glm::vec3 min; // 12
	int leftIndex = 0; // 4
	glm::vec3 max; // 12
	int triIndex; // 4
	int triCount; // 4
	float pad1; // 4
	float pad2; // 4
	float pad3; // 4
};

class BVH {


public:
	std::vector<BVHNode> nodes;
	std::vector<glm::vec4> BVHTriangles;

	BVH(Model model, std::vector<Triangle> triangles);

private:
	std::vector<Triangle> modelTriangles;

	void updateAABB(int index);
	void split(int index, int depth);

};

static void addBVH(Model& model, std::vector<BVHNode>& nodes, std::vector<int>& nodeTriIndex, std::vector<Triangle>& triangles) {
	model.BVHIndex = nodes.size();
	int triSize = nodeTriIndex.size();
	BVH bvh(model, triangles);

	for (BVHNode& node : bvh.nodes) {
		node.leftIndex += model.BVHIndex;
		node.triIndex += triSize;
		nodes.push_back(node);
	}
	for (int i = 0; i < bvh.BVHTriangles.size(); i++) {
		nodeTriIndex.push_back(bvh.BVHTriangles[i].w);
	}
}

static void deleteBVH(int index, int nextIndex, std::vector<BVHNode>& nodes, std::vector<int>& nodeTriIndex) {
	// last model
	if (nextIndex == -1) {
		nodeTriIndex.erase(nodeTriIndex.begin() + nodes[index].triIndex, nodeTriIndex.end());
		nodes.erase(nodes.begin() + index, nodes.end());
	}
	else {
		int indexDiff = nextIndex - index;
		int triDiff = nodes[nextIndex].triIndex - nodes[index].triIndex;

		nodeTriIndex.erase(nodeTriIndex.begin() + nodes[index].triIndex, nodeTriIndex.begin() + nodes[nextIndex].triIndex);
		nodes.erase(nodes.begin() + index, nodes.begin() + nextIndex);

		for (int i = index; i < nodes.size(); i++) {
			nodes[i].leftIndex -= indexDiff;
			nodes[i].triIndex -= triDiff;
		}
	}
}

#endif