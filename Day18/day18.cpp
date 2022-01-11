#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>

struct Node
{
	int val;
	Node* left;
	Node* right;

	Node(int val) :
		val(val),
		left(0),
		right(0)
	{}

	Node(Node* left, Node* right) :
		val(-1),
		left(left),
		right(right)
	{}

};

int depth = 0;
Node* currentNode;
Node* nodeStack[10];

// Get current node's parent
Node*& getParent()
{
	// If the parent is a left child of higher depth node in stack
	if (nodeStack[depth - 2]->left == nodeStack[depth - 1])
	{
		return nodeStack[depth - 2]->left;
	}

	// Parent is from right child
	else
	{
		return nodeStack[depth - 2]->right;
	}
}

void nextNode()
{
	// Literal node
	if (nodeStack[depth]->val != -1)
	{
		// Fill in the Right node datta
		while (depth > 0 && nodeStack[depth] == nodeStack[depth - 1]->right)
		{
			depth--;
			if (depth == 0)
			{
				currentNode = 0;
				return;
			}
		}
		nodeStack[depth] = nodeStack[depth-1]->right;
	}

	// Add left node tree to stack
	// Go as far left as possible
	while (nodeStack[depth]->left != 0)
	{
		nodeStack[depth + 1] = nodeStack[depth]->left;
		depth++;
	}

	// Set current node
	currentNode = nodeStack[depth];
}

void nodeStart(Node* n)
{
	depth = 0;
	nodeStack[depth] = n;
	nextNode();
}

void printSnailNum(Node* node)
{
	// Connector
	if (node->val == -1)
	{
		printf("[");
		printSnailNum(node->left);
		printf(",");
		printSnailNum(node->right);
		printf("]");
	}
	// Literal
	else
	{
		printf("%d", node->val);
	}
}

Node* createSnailNum(char*& str)
{
	// Do recursion case
	if (*str == '[')
	{
		str++;
		Node* left = createSnailNum(str);
		str++;
		Node* right = createSnailNum(str);
		str++;

		return new Node(left, right);
	}

	// Extract Nums
	else
	{
		int num = 0;
		while (std::isdigit(*str))
		{
			num = (10 * num) + (*str++ - '0');
		}

		return new Node(num);
	}
}

long getSnailMag(Node* n)
{
	if (n->val == -1)
	{
		return (3 * getSnailMag(n->left) + 2 * getSnailMag(n->right));
	}
	else
		return n->val;
}

Node* addSnailNums(Node* n1, Node* n2)
{
	// Create new Node
	Node* sumSnail = new Node(n1, n2);

	// Check for explosions and or splits
	bool checkDone = false;
	while (!checkDone)
	{
		// If we scan and see nothing then end
		checkDone = true;

		Node* prevNode = 0;
		nodeStart(sumSnail);

		// Check for depth great enough for explosion
		while (currentNode != 0 && depth < 5)
		{
			prevNode = currentNode;
			nextNode();
		}
		
		// Do we have an explosion?
		if (depth >= 5)
		{
			// Need another check after explode
			checkDone = false;

			Node*& nodeParent = getParent();
			
			// Left Number
			int leftNum = currentNode->val;
			
			// Right Number
			nextNode();
			int rightNum = currentNode->val;

			// If a leftmost number exists then add value to it
			if (prevNode != 0)
				prevNode->val += leftNum;
			nextNode();

			// If a rightmost number exists then add value to it
			if (currentNode != 0)
				currentNode->val += rightNum;

			// Replacing explosion aftermath with a 0
			nodeParent = new Node(0);
		}

		// Dont check splits if we already indicated an explosion
		if (checkDone == false)
			continue;

		// Check for splits

		// Reset position
		nodeStart(sumSnail);

		// Check for value >= 10
		while (currentNode != 0 && currentNode->val < 10)
			nextNode();

		// Split detected
		if (currentNode != 0)
		{
			checkDone = false;
			int num = currentNode->val;
			int leftNum = num / 2; // floor by default
			int rightNum = num - leftNum; // Ceiling

			// Set new nodes
			currentNode->left = new Node(leftNum);
			currentNode->right = new Node(rightNum);
			currentNode->val = -1; // This node is now a connector node

		}
	}

	return sumSnail;
}

// Copy a node
Node* copyNode(Node* n)
{
	if (n->val == -1)
		return new Node(copyNode(n->left), copyNode(n->right));
	else
		return new Node(n->val);
}

int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day18.txt");



	// Sum Node
	Node* sum = 0;

	std::vector<Node*> nodes;

	// Parse data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		char* data = &str[0];
		Node* n = createSnailNum(data);
		nodes.push_back(n);

	}
	file.close();

	
	long long largest = -1;
	
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < nodes.size(); j++)
		{
			// Make a copy so we don't affect original nodes
			Node* n1 = copyNode(nodes[i]); 
			Node* n2 = copyNode(nodes[j]);

			long long mag = getSnailMag(addSnailNums(n1, n2));
			if (mag > largest)
				largest = mag;
			
		}
	}

	printf("Largest: %lld\n", largest);


	return 0;
}