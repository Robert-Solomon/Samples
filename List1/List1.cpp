#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

typedef struct DblList
{
	int Val;
	struct DblList *fwd;
	struct DblList *bak;
} sDblList, *psDblList;

class SortedDblList
{
private:
	psDblList Head;

	psDblList FindpLgEq(int Val)
	{
		// for empty list we return NULL
		if (IsEmpty())
			return NULL;
		// if all items are smaller - return NULL
		if (Head->Val < Val)
			return NULL;
		// if all items are larger, return the smallest
		if (Head->bak->Val > Val)
			return Head->bak;
		psDblList p = Head;
		while (p->fwd->Val >= Val)
			p = p->fwd;
		return p;
	}

public:
	SortedDblList() { Head = NULL; }
	~SortedDblList() { while (!IsEmpty()) Pop(); }
	bool IsEmpty() { return Head == NULL; }
	
	void Pop() {
		if (IsEmpty())
			throw "list is empty";

		psDblList p = Head->fwd;
		if (p != Head) {
			p->bak = Head->bak;
			Head->bak->fwd = p;
		}
		else {
			p = NULL;
		}

		delete Head;
		Head = p;
	}

	bool Find(int Val)
	{
		psDblList p = FindpLgEq(Val);

		return p != NULL && p->Val == Val;
	}

	int First()
	{
		if (IsEmpty())
			throw "list is empty";
		return Head->Val;
	}

	int Last()
	{
		if (IsEmpty())
			throw "list is empty";
		return Head->bak->Val;
	}

	bool Delete(int Val)
	{
		psDblList p = FindpLgEq(Val);
		if (p == NULL || p->Val != Val)
			return false;

		// if this is the last item...
		if (p->fwd == p)
		{
			delete p;
			Head = NULL;
			return true;
		}

		p->fwd->bak = p->bak;
		p->bak->fwd = p->fwd;

		//if p was the head item
		if (Head == p)
			Head = p->fwd;

		delete p;

		return true;
	}

	bool Add(int Val)
	{
		psDblList p = FindpLgEq(Val);

		// if we already have this value - no need to add it.
		if (p != NULL && p->Val == Val)
			return false;

		// create a new node
		psDblList np = new(sDblList);
		if (np == NULL)
			throw "new node creation failed";

		np->Val = Val;

		// if this is the first element - easy :)
		if (Head == NULL)
		{
			Head = np->fwd = np->bak = np;
			return true;
		}

		// if we did not find an item larger than our new value - this will be the new head
		if (p == NULL)
		{
			np->fwd = Head;
			np->bak = Head->bak;
			np->fwd->bak = np->bak->fwd = Head = np;
			return true;
		}

		//finally, we adding somewhere along the list....
		np->bak = p;
		np->fwd = p->fwd;
		np->fwd->bak = np->bak->fwd = np;
		return true;
	}

	void ShowList()
	{
		std::cout << "List: ";
		if (IsEmpty())
			std::cout << "Empty\n";
		else {
			psDblList p = Head;
			do
			{
				std::cout << ' ' << p->Val;
				p = p->fwd;
			} while (p != Head);
		}
		std::cout << '\n';
	}

	bool Validate()
	{
		int val;
		psDblList p;
		int cnt = 0; // will count up forward, down back, and must reach ZERO
		bool AllGood = true;

		if (Head == NULL)
			return true;

		// validate forward
		p = Head;
		while (p->fwd != Head)
		{
			if (p->Val <= p->fwd->Val)
				AllGood = false;
			p = p->fwd;
			cnt++;
		}

		// validate backward
		p = Head;
		while (p->bak != Head)
		{
			p = p->bak;
			if (p->Val >= p->bak->Val)
				AllGood = false;
			cnt--;
		}

		if (cnt != 0)
			AllGood = false;

		return AllGood;
	}
};

int main()
{
	class SortedDblList lst;
	std::__cxx11::string input_s;
	char cmd;
	int val;
	bool loop = true;

	do
	{
		std::cout << "cmd> ";
		getline(std::cin, input_s);
		std::stringstream ss(input_s);
		ss >> cmd;
		switch (cmd)
		{
			case 'a': 
			case 'A':
				ss >> val;
				std::cout << "---> Add(" << val << "): " << (lst.Add(val) ? "OK\n" : "FAIL\n");
				break;

			case 'd': 
			case 'D':
				if (lst.IsEmpty())
				{
					std::cout << "---> cannot delete from empty list";
					continue;
				}
				ss >> val;
				std::cout << "---> Delete(" << val << "): " << (lst.Delete(val) ? "OK\n" : "FAIL\n");
				break;

			case 'p':
			case 'P':
				if (lst.IsEmpty())
				{
					std::cout << "---> cannot pop from empty list\n";
					break;
				}
				lst.Pop();
				std::cout << "---> Pop(): OK\n";
				break;

			case 'f':
			case 'F':
				ss >> val;
				std::cout << "---> Find(" << val << "): " << (lst.Find(val) ? "Found\n" : "Not Found\n");
				break;

			case 's':
			case 'S':
				ss >> cmd;
				if (cmd == 'l')
				{
					std::cout << "---> ";
					lst.ShowList();
				}
				else if (cmd == 'h')
				{
					if (lst.IsEmpty())
					{
						std::cout << "---> list is empty\n";
						break;
					}

					std::cout << "---> Head: " << lst.First() << '\n';
				}
				else if (cmd == 't')
				{
					if (lst.IsEmpty())
					{
						std::cout << "---> list is empty\n";
						break;
					}

					std::cout << "---> Tail: " << lst.Last() << '\n';
				}
				else
				{
					std::cout << "---> unknown Show command\n";
				}
				break;

			case 'h':
			case 'H':
				std::cout << "---> Help - supported commands\n";
				std::cout << "a <num>            Add <num> to the list\n";
				std::cout << "d <num>            Delete <num> from the list\n";
				std::cout << "p                  Pop head item";
				std::cout << "f <num>            Find <num> in list\n";
				std::cout << "s l                show list\n";
				std::cout << "s h                show item at front\n";
				std::cout << "s t                show item at tail\n";
				std::cout << "v                  Validate list\n";
				std::cout << "h                  Help\n";
				std::cout << "q                  Quit\n";
				break;

			case 'v': 
			case 'V':
				std::cout << "---> Validate: " << (lst.Validate() ? "PASS\n" : "FAIL\n");
				break;

			case 'q':
			case 'Q':
				loop = false;
				break;

			default:
				std::cout << "Unknown command\n";
		}
	} while (loop);
}
