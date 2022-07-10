#ifndef __ROPE__
#define __ROPE__
#define EMPTY ""

typedef struct RopeNode RopeNode;

struct RopeNode {
    RopeNode* left;
    RopeNode* right;
    const char* str;
    int weight;
};

typedef struct RopeTree RopeTree;

struct RopeTree {
    RopeNode* root;
};

typedef struct SplitPair SplitPair;

struct SplitPair {
    RopeNode* left;
    RopeNode* right;
};



RopeNode* makeRopeNode(const char* str);

RopeTree* makeRopeTree(RopeNode* root);

void printRopeNode(RopeNode* rn);

void printRopeTree(RopeTree* rt);

void debugRopeNode(RopeNode* rn, int indent);

RopeTree* concat(RopeTree* rt1, RopeTree* rt2);

RopeNode* __concat(RopeNode* node1, RopeNode* node2);

char indexRope(RopeTree* rt, int idx);

char __indexRope(RopeNode* root, int idx);

char* search(RopeTree* rt, int start, int end);

char* __search(RopeNode* root, int start, int end);

SplitPair split(RopeTree* rt, int idx);

RopeNode* __split(RopeNode* root, int idx);

RopeTree* insert(RopeTree* rt, int idx, const char* str);

RopeTree* delete(RopeTree* rt, int start, int len);

#endif  // __ROPE__
