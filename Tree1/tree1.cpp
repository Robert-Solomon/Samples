#include <stdio.h>
#include <iostream>
#include <stack>
#include <queue>

typedef struct _treenode
{
	int val;
	struct _treenode *llink;
	struct _treenode *rlink;
} tTreeNode, *ptTreeNode;

//                                   1
//                               /       \
//                              2          3
//                             /        /      \
//                            4         5        6
//                         /     \       \     /    \
//                        7      8        9   10    11
//                        \    /   \    /   \      /   \
//                        12  13   14  15   16    17   18
//                             \
//                             19
//

tTreeNode                               t19 = { 19, NULL, NULL };
tTreeNode t12 = { 12, NULL, NULL }, t13 = { 13, NULL, &t19 }, t14 = { 14, NULL, NULL }, t15 = { 15, NULL, NULL }, t16 = { 16, NULL, NULL }, t17 = { 17, NULL, NULL }, t18 = { 18, NULL, NULL };
tTreeNode     t7 = { 7, NULL, &t12},   t8 = { 8, &t13, &t14},   t9 = { 9, &t15, &t16},    t10 = { 10, NULL, NULL}, t11 = { 11, &t17, &t18};
tTreeNode              t4 = { 4, &t7, &t8},           t5 = { 5, NULL, &t9},                    t6 = { 6, &t10, &t11};
tTreeNode                       t2 = {2, &t4, NULL},                         t3 = {3, &t5, &t6};
tTreeNode                                 t1 = { 1, &t2, &t3 };

void BFS(ptTreeNode p)
{
	// initialize queue with tree root, later reusing p as local variable
	std::queue<ptTreeNode> que;
	que.push(p);

	std::cout << "BFS order:";
	
	while (!que.empty())
	{
		// extract node at HEAD of queue, 'visit' the node, and push the Left, Right (non NULL) children at the TAIL
		p = que.front(); que.pop();
		std::cout << ' ' << p->val;
		if (p->llink) que.push(p->llink);
		if (p->rlink) que.push(p->rlink);
	}
	std::cout << '\n';
}

void DFS(ptTreeNode p)
{
	// initialize stack with tree root, later reusing p as local variable
	std::stack<ptTreeNode> stk;
	stk.push(p);

	std::cout << "DFS order:";

	while (!stk.empty())
	{
		// pop the TOP node on the stack, 'visit' the node, then push the Right, Left (non NULL) children back on TOP.
		// note: right child is pushed first, so left gets explored first.
		p = stk.top();  stk.pop();
		std::cout << ' ' << p->val;
		if (p->rlink) stk.push(p->rlink);
		if (p->llink) stk.push(p->llink);
	}
	std::cout << '\n';
}

void ZigZag(ptTreeNode p)
{
	// initialize stack with tree root, later reusing p as local variable
	std::stack<ptTreeNode> stk_l, stk_r;
	stk_l.push(p);
	bool direction_left = true;

	std::cout << "ZigZag order:";

	do 
	{
		if (direction_left)
		{
			while (!stk_l.empty())
			{
				// pop the TOP node on the stack, 'visit' the node, then push the Left, Right (non NULL) children onto the stk_r stack.
				// note: left child is pushed first, so right gets explored first.
				p = stk_l.top();  stk_l.pop();
				std::cout << ' ' << p->val;
				if (p->llink) stk_r.push(p->llink);
				if (p->rlink) stk_r.push(p->rlink);
			}
			if (stk_r.empty())
				break;
			direction_left = false;
		}
		else
		{
			while (!stk_r.empty())
			{
				// pop the TOP node on the stack, 'visit' the node, then push the Right, Left (non NULL) children onto the stk_l stack.
				// note: right child is pushed first, so left gets explored first.
				p = stk_r.top();  stk_r.pop();
				std::cout << ' ' << p->val;
				if (p->rlink) stk_l.push(p->rlink);
				if (p->llink) stk_l.push(p->llink);
			}
			if (stk_l.empty())
				break;
			direction_left = true;
		}
	} while (true);
	std::cout << '\n';
}

int main()
{
	ptTreeNode Root = &t1;

	BFS(Root);
	DFS(Root);
	ZigZag(Root);

	return 0;
}