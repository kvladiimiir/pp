#include "pch.h"
#include "LinkedList.h"

LinkedList::LinkedList()
	: m_listSize(0)
	, m_listHead(nullptr)
{
}

size_t LinkedList::GetListSize()
{
	return m_listSize;
}

void LinkedList::Pop()
{
	if (!(m_listSize == 0))
	{
		m_listHead = m_listHead->next;
		--m_listSize;
	}
}

void LinkedList::Push(string value)
{
	m_listHead = make_shared<Node>(value, m_listHead);
	++m_listSize;
}

string LinkedList::GetListHead()
{
	if (m_listSize == 0)
	{
		throw exception("Empty list");
	}

	return m_listHead->value;
}

void LinkedList::Clear()
{
	while (!(m_listSize == 0))
	{
		Pop();
	}
}

LinkedList::~LinkedList()
{
	Clear();
}