
node *ReverseList(node *Head)
{
	node *pb, pf;

	if (Head == NULL)
		return NULL;

	pb = Head;
	Head = Head->Next;
	pb->next = NULL;

	while (Head != NULL)
	{
		pf = Head->next;
		Head->next = pb;
		Head = pf;
	}

	retrun pb;
}
