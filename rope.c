#include "./rope.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define EMPTY ""
#define NMAX 64
#define MIN(a , b) a < b ? a:b

const char *strdup(const char *s);

// Equivalent function for substr
char* substring(const char* s, int start, int end)
{
	char* subs = malloc(end - start + 1);
	DIE(!subs, "Memory error!");

	memcpy(subs, s + start, end - start);
	subs[end - start] = '\0';

	return subs;
}

RopeNode* makeRopeNode(const char* str) {
	RopeNode* node = malloc(sizeof(RopeNode));
	DIE(!node, "Memory error!");

	node->right = NULL;
	node->left = NULL;
	node->str = str;
	node->weight = strlen(str);

	return node;
}

RopeTree* makeRopeTree(RopeNode* root) {
    RopeTree* rt = malloc(sizeof(RopeTree));
	DIE(!rt, "Memory error!");

	rt->root = root;

	return rt;
}

void printRopeNode(RopeNode* rn) {
    if (!rn)
        return;

    if (!(rn->left) && !(rn->right)) {
        printf("%s", rn->str);
        return;
    }

    printRopeNode(rn->left);
    printRopeNode(rn->right);
}

void printRopeTree(RopeTree* rt) {
    if (rt && rt->root) {
        printRopeNode(rt->root);
        printf("%s", "\n");
    }
}

void debugRopeNode(RopeNode* rn, int indent) {
    if (!rn)
        return;

    for (int i = 0; i < indent; ++i)
        printf("%s", " ");

    if (!strcmp(rn->str, EMPTY))
        printf("# %d\n", rn->weight);
    else
        printf("%s %d\n", rn->str, rn->weight);

    debugRopeNode(rn->left, indent+2);
    debugRopeNode(rn->right, indent+2);
}

int getTotalWeight(RopeNode* rt) {
    if (!rt)
    	return 0;

    return rt->weight + getTotalWeight(rt->right);
}

// auxiliary function used for nodes concat
RopeNode* __concat(RopeNode* node1, RopeNode* node2)
{
	RopeNode* root = makeRopeNode(strdup(""));

	root->left = node1;
	root->right = node2;

	// The root weights the same as the whole left subtree
	root->weight = getTotalWeight(node1);

	return root;
}

// Main function for concat
RopeTree* concat(RopeTree* rt1, RopeTree* rt2) {
	RopeNode* root = __concat(rt1->root, rt2->root);
	RopeTree* rt = makeRopeTree(root);

	return rt;
}

// index auxiliary function
char __indexRope(RopeNode* root, int idx)
{
	//Upon reaching a leaf, return the idx-th character
	if(strcmp(root->str, EMPTY))
	{
		return root->str[idx];
	}

	// idx comes from the left subtree
	if(idx < root->weight)
	{
		return __indexRope(root->left, idx);
	}

	// idx comes from the right subtree
	return __indexRope(root->right, idx - root->weight);
}

// Main index function
char indexRope(RopeTree* rt, int idx) {
	return __indexRope(rt -> root, idx);
}

// Main search function
char* search(RopeTree* rt, int start, int end) {
	char* my_substr;
	char ch;
	int len = end - start + 1;

	my_substr = malloc(len * sizeof(char));
	DIE(!my_substr, "Memory error!");

	// Adding to the sequence all letters found from the interval
	for (int i = start; i < end; ++i) {
		ch = indexRope(rt, i);
		my_substr[i - start] = ch;
	}
	my_substr[len - 1] = '\0';

	return my_substr;
}

// Auxiliary split function
RopeNode* __split(RopeNode* root, int idx)
{
	RopeNode* left = root->left;
	RopeNode* right = root->right;

	// Finished iterating through the tree
	if(idx == root->weight)
	{
		root->right = NULL;
		return right;
	}

	// Left move
	if(idx < root->weight)
	{
		if(strcmp(left->str, EMPTY))
		{
			// Breaking the leaf into 2 nodes
			RopeNode* left_side = makeRopeNode(substring(left->str, 0, idx));
			RopeNode* right_side = makeRopeNode(strdup(left->str + idx));

			free((char*)left->str);
			free(left);
			root->left = __concat(left_side, right_side);
		}

		root->right = NULL;

		RopeNode* next_value = __concat(__split(root->left, idx), right);
		root->weight = getTotalWeight(root->left);
		return next_value;
	}

	// Right move
	idx = idx - root->weight;

	if(strcmp(right->str, EMPTY))
	{
		if(!idx)
		{
			root->right = NULL;
			return right;
		}

		// Breaking the leaf into 2 nodes
		RopeNode* left_side = makeRopeNode(substring(right->str, 0, idx));
		RopeNode* right_side = makeRopeNode(strdup(right->str + idx));

		free((char*)right->str);
		free(right);
		root->right = __concat(left_side, right_side);
	}

	return __split(root->right, idx);
}

// Copying the nodes from the tree
void __copy(RopeNode* copy_node, RopeNode* original_node)
{
	// Left nodes
	if(original_node->left)
	{
		copy_node->left = makeRopeNode(strdup(original_node->left->str));
		copy_node->left->weight = original_node->left->weight;
		__copy(copy_node->left, original_node->left);
	}

	// Right nodes
	if(original_node->right)
	{
		copy_node->right = makeRopeNode(strdup(original_node->right->str));
		copy_node->right->weight = original_node->right->weight;
		__copy(copy_node->right, original_node->right);
	}
}

// Copying the tree
RopeNode* copy(RopeNode* root_original)
{
	RopeNode* root_copy = makeRopeNode(strdup(root_original->str));

	root_copy->weight = root_original->weight;

	__copy(root_copy, root_original);

	return root_copy;
}

// Main split function
SplitPair split(RopeTree* rt, int idx) {
	SplitPair pair;
	RopeNode* clone = copy(rt->root);

	if (idx == 0) {
		pair.left = makeRopeNode(strdup(EMPTY));
		pair.right = clone;
		return pair;
	}

	pair.left = clone;
	pair.right = __split(clone, idx);

	return pair;
}

// Insert functions
RopeTree* insert(RopeTree* rt, int idx, const char* str) {
	RopeTree* rt_clone = makeRopeTree(NULL);
	SplitPair sides = split(rt, idx);
	RopeNode* middle = makeRopeNode(str);

	rt_clone->root = __concat(sides.left, middle);
	rt_clone->root = __concat(rt_clone->root, sides.right);

	return rt_clone;
}

// Delete function
RopeTree* delete(RopeTree* rt, int start, int len) {
	int length = getTotalWeight(rt->root);
	RopeTree* rt_clone = makeRopeTree(NULL);
	SplitPair pair1 = split(rt, start);
	SplitPair pair2 = split(rt, MIN(length, start + len));

	rt_clone->root = __concat(pair1.left, pair2.right);

	free(pair1.right);
	free(pair2.left);

	return rt_clone;
}
