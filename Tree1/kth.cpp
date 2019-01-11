#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <fstream>
#include <cstdlib>
#include <map>

template <class T>
class TreeNode {
public:
	TreeNode() : right(NULL), left(NULL) {}
	TreeNode(const T& v) : value(v), right(NULL), left(NULL) {}

	T value;
	TreeNode<T>* right;
	TreeNode<T>* left;
};


void inorder(TreeNode<int>* head) {
	if (!head) return;
	inorder(head->left);
	std::cout << head->value;
	inorder(head->right);
}

void preorder(TreeNode<int>* head) {
	if (!head) return;
	std::cout << head->value;
	preorder(head->left);
	preorder(head->right);
}

void postorder(TreeNode<int>* head) {
	if (!head) return;
	postorder(head->left);
	postorder(head->right);
	std::cout << head->value;
}


int KthLargest(TreeNode<int>* np, int k) {
	
	// if this is an empty node, just say so.
	if (!np) 
		return 0;

	if (k <= 0) {
		std::cout << "invalid input" << std::endl;
		return -1;
	}

	int kr, kl;

	// how many nodes are to my right?
	kr = KthLargest(np->right, k);

	// exactly k-1? lucky me, BINGO!
	if (kr == (k - 1))
	{
		std::cout << np->value << std::endl;
		return k;
	}

	// was it already found in the right tree somewhere?
	if (kr >= k)
	{
		// this is not acurate, but given our logic, nobody up there cares...
		return kr + 1;
	}

	// if we get here, then kr < k-1, so need to look in the left child, while we know there are kr+1 nodes to its right.
	kl = KthLargest(np->left, k-(kr+1));

	// if the element was found in the left node, then kl is minimum k-kr-1. if the element is not found, we get a precise size of the subtree.
	return kl + kr + 1; // if the node was found, this is guaranteed to be >= k

}


int main() {
	TreeNode<int>* one = new TreeNode<int>;
	TreeNode<int>* two = new TreeNode<int>;
	TreeNode<int>* three = new TreeNode<int>;
	TreeNode<int>* four = new TreeNode<int>;
	TreeNode<int>* five = new TreeNode<int>;

	one->value = 1;
	two->value = 2;
	three->value = 3;
	four->value = 4;
	five->value = 5;

	four->left = two;
	four->right = five;
	two->left = one;
	two->right = three;

	for (int i = 0; i < 7; i++)
	{
		std::cout << "---" << i << "---" << std::endl;
		if (KthLargest(four, i) < i)
			std::cout << "input value more than number of elements" << std::endl;
	}
}