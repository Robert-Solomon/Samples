#include <stdio.h>
#include <stdlib.h>
#include <stack>

typedef struct _Node
{
	struct _Node *L, *R;
	int V;
} Node;

Node *NewNode(int V, Node *L = NULL, Node *R = NULL)
{
	Node *n = new Node;
	n->V = V;
	n->L = L;
	n->R = R;
	return n;
}

void InOrder(Node *Head)
{
	Node *np = Head;
	std::stack <Node *> stk;

	while (np || !stk.empty())
	{
		if (np) // we are visiting this node for the first time
		{
			if (np->L) // it has a left child - put current node on stack and go there first
			{
				stk.push(np);
				np = np->L;
				continue;
			}

			// fall through to printing current node
		}
		else // we are popping up a level
		{
			np = stk.top();
			stk.pop();
		}

		printf("%d ", np->V);

		// continue to right child, or pop up;
		np = np->R;
	}

	printf("\n");
}

main()
{
	Node *Tree = NewNode(1, NewNode(2, NewNode(4), NewNode(5, NULL, NewNode(7))), NewNode(3, NewNode(6)));
	InOrder(Tree);
}