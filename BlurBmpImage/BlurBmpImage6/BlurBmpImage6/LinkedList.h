#pragma once

class LinkedList
{
public:
	LinkedList();
	void Push(std::string value);
	void Pop();
	void Clear();
	std::string GetListHead();
	size_t GetListSize();

	~LinkedList();
private:
	struct Node
	{
		Node(std::string value, std::shared_ptr<Node>& node)
			: value(value)
			, next(node)
		{}
		std::string value;
		std::shared_ptr<Node> next;
	};

	size_t m_listSize;
	std::shared_ptr<Node> m_listHead;
};