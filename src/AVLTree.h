#pragma once
#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <iostream>
#include <stdexcept>
#include <functional>

/**
* AVL Ʈ���� ���
*/
template <typename T>
struct AVLTreeNode
{
	T data;
	AVLTreeNode<T>* left;
	AVLTreeNode<T>* right;
	int height;
};

template <typename T>
class AVLTreeIterator;
/**
* AVL Ʈ�� Ŭ����
* @date 2018.11.21
* @author �輺��
*/
template <typename T>
class AVLTree
{
private:
	friend class AVLTreeIterator<T>;

	AVLTreeNode<T>* root;

	void CopyNode(AVLTreeNode<T>*& node, AVLTreeNode<T>* copy);

	void ClearNode(AVLTreeNode<T>*& node);

	int CountNode(const AVLTreeNode<T>* node) const;

	int AddNode(AVLTreeNode<T>*& node, const T& data);

	int DeleteNode(AVLTreeNode<T>*& node, const T& data);

	int ReplaceNode(AVLTreeNode<T>*& node, const T& data);

	int GetNode(AVLTreeNode<T>*& node, T& data);

	void CalculateHeight(AVLTreeNode<T>*& node);

	int height(AVLTreeNode<T>*& node);

	AVLTreeNode<T>* SingleLeftRotate(AVLTreeNode<T>* node);

	AVLTreeNode<T>* SingleRightRotate(AVLTreeNode<T>* node);

	AVLTreeNode<T>* DoubleLeftRotate(AVLTreeNode<T>* node);

	AVLTreeNode<T>* DoubleRightRotate(AVLTreeNode<T>* node);

	void findNode(AVLTreeNode<T>*& node, const std::function<bool(T&)>& search, const std::function<void(T&)>& todo);

	void doNode(AVLTreeNode<T>*& node, const std::function<void(T&)>& todo);

	std::function<int(const T&, const T&)> compareFunc; ///< ������ �� �Լ�

public:

	AVLTree();

	AVLTree(const AVLTree& tree);

	~AVLTree();

	void MakeEmpty();

	bool IsEmpty() const;

	bool IsFull();

	int Add(const T& data);

	int Delete(const T& data);

	int Replace(const T& data);

	int Get(T& data);

	int GetLength() const;

	AVLTree<T>& operator=(const AVLTree<T>& tree);

	void Find(const std::function<bool(T&)>& search, const std::function<void(T&)>& todo);

	void Do(const std::function<void(T&)>& todo);

	/**
	*	@brief	������ �� �Լ��� �����Ѵ�.
	*	@pre	�� �Լ��� t1, t2�� ���� �� t1 > t2�̸� ���, t1 = t2�̸� 0, t1 < t2�̸� ������ ��ȯ�ؾ� �Ѵ�.
	*	@post	������ �� �Լ��� parameter�� �Լ��� �����ȴ�.
	*	@param	func	������ ������ �� �Լ�
	*/
	void SetCompareFunction(const std::function<int(const T&, const T&)>& func);
};

template <typename T>
AVLTree<T>::AVLTree()
{
	root = nullptr;
	compareFunc = [](const T& t1, const T& t2) {
		return (t1 > t2) - (t1 < t2);
	};
}

template <typename T>
AVLTree<T>::AVLTree(const AVLTree<T>& tree)
{
	MakeEmpty();
	CopyNode(root, tree.root);
}

template <typename T>
AVLTree<T>::~AVLTree()
{
	MakeEmpty();
}

template <typename T>
void AVLTree<T>::MakeEmpty()
{
	ClearNode(root);
}

template <typename T>
bool AVLTree<T>::IsEmpty() const
{
	return root == nullptr;
}

template <typename T>
bool AVLTree<T>::IsFull()
{
	try
	{
		AVLTreeNode<T>* node = new AVLTreeNode<T>;
		delete node;
		return false;
	}
	catch (std::bad_alloc& e)
	{
		std::cerr << "bad alloc exception: " << e.what() << std::endl;
		return true;
	}
}

template <typename T>
int AVLTree<T>::Add(const T& data)
{
	return AddNode(root, data);
}

template <typename T>
int AVLTree<T>::Delete(const T& data)
{
	return DeleteNode(root, data);
}

template <typename T>
int AVLTree<T>::Replace(const T& data)
{
	return ReplaceNode(root, data);
}

template <typename T>
int AVLTree<T>::Get(T& data)
{
	return GetNode(root, data);
}

template <typename T>
int AVLTree<T>::GetLength() const
{
	return CountNode(root);
}

template <typename T>
AVLTree<T>& AVLTree<T>::operator=(const AVLTree<T>& tree)
{
	MakeEmpty();
	CopyNode(root, tree.root);
	return *this;
}

template <typename T>
void AVLTree<T>::Find(const std::function<bool(T&)>& search, const std::function<void(T&)>& todo)
{
	findNode(root, search, todo);
}

template <typename T>
void AVLTree<T>::Do(const std::function<void(T&)>& todo)
{
	doNode(root, todo);
}

template <typename T>
void AVLTree<T>::SetCompareFunction(const std::function<int(const T&, const T&)>& func)
{
	compareFunc = func;
}

/* -------- recursive functions -------- */
template <typename T>
void AVLTree<T>::CopyNode(AVLTreeNode<T>*& node, AVLTreeNode<T>* copy)
{
	if (copy == nullptr) return;
	node = new AVLTreeNode<T>;
	node->data = copy->data;
	node->left = nullptr;
	node->right = nullptr;
	node->height = copy->height;
	CopyNode(node->left, copy->left);
	CopyNode(node->right, copy->right);
}

template <typename T>
void AVLTree<T>::ClearNode(AVLTreeNode<T>*& node)
{
	if (node == nullptr) return;
	ClearNode(node->left);
	ClearNode(node->right);
	delete node;
	node = nullptr;
}

template <typename T>
int AVLTree<T>::CountNode(const AVLTreeNode<T>* node) const
{
	if (node == nullptr) return 0;
	return 1 + CountNode(node->left) + CountNode(node->right);
}

template <typename T>
int AVLTree<T>::AddNode(AVLTreeNode<T>*& node, const T& data)
{
	if (node == nullptr)
	{
		node = new AVLTreeNode<T>;
		node->data = data;
		node->left = nullptr;
		node->right = nullptr;
		node->height = 0;
		return 1;
	}
	
	int compare = compareFunc(node->data, data);
	if (compare > 0) //data�� ���� ���
	{
		int success = AddNode(node->left, data); //���� subtree ������Ʈ

		if ((height(node->left) - height(node->right)) == 2) //�ұ����� �߻��� ���
		{
			if (data < node->left->data) node = SingleRightRotate(node); //data�� ���� subtree�� ���ʿ� �߰��� ��� = �ܼ� right rotation
			else node = DoubleRightRotate(node); //data�� ���� subtree�� �����ʿ� �߰��� ��� = ���� right rotation
		}

		CalculateHeight(node);
		return success;
	}
	else if (compare < 0) //data�� ū ���
	{
		int success = AddNode(node->right, data); //������ subtree ������Ʈ

		if ((height(node->right) - height(node->left)) == 2) //�ұ����� �߻��� ���
		{

			if (data > node->right->data) node = SingleLeftRotate(node); //data�� ������ subtree�� �����ʿ� �߰��� ��� = �ܼ� left rotation
			else node = DoubleLeftRotate(node); //data�� ������ subtree�� ���ʿ� �߰��� ��� = ���� left rotation
		}

		CalculateHeight(node);
		return success;
	}
	
	return 0; //same data found
}

template <typename T>
int AVLTree<T>::DeleteNode(AVLTreeNode<T>*& node, const T& data)
{
	if (node == nullptr) return 0;
	int success = 0;

	int compare = compareFunc(node->data, data);

	if (compare > 0) //data�� ���� ���
		success = DeleteNode(node->left, data);
	else if (compare < 0) //data�� ū ���
		success = DeleteNode(node->right, data);
	else //ã�� ���
	{
		if (node->left && node->right) //child�� ���� ���
		{
			AVLTreeNode<T>* r = node->right;
			AVLTreeNode<T>* n = r; //r�� �θ� ���
			while (r->left)
			{
				n = r;
				r = r->left;
			}
			//���� data���� ū �� �� ���� ���� ���� ã�´�. (�ǵ��� complete tree�� ����� ����)
			//r�� leaf node���� ����ȴ�.

			node->data = r->data;
			delete r;

			n->left = nullptr; //�θ� ��尡 dangling pointer�� ����Ű�� �ʵ��� ����
		}
		else //child�� ���ų� �ϳ��� ���
		{
			AVLTreeNode<T>* temp = node;
			if (node->left == nullptr) node = node->right;
			else if (node->right == nullptr) node = node->left;
			delete temp;

			if (root == temp) root = nullptr; //�ֻ��� ����� ��� root�� dangling pointer�� ����Ű�� �ʵ��� ���� (��尡 root �ϳ� ���̹Ƿ�)
			//�̿� �߻��ϴ� dangling pointer�� ȸ�� �������� �ذ�ȴ�.

			return 1; //child�� �ϳ��� ��쿣 node�� overwrite�ǰ�, �� node�� leaf node�� �ǹǷ� ȸ�� ���� ���ʿ�
			//child�� ���� ��쿣 node�� delete�ǰ� nullptr�� ��. ȸ�� ������ ���ʿ��� �Ӵ���, ���� ��ü�� �Ұ���
		}
	}

	CalculateHeight(node); //���� ���� �� height�� ���

	if (height(node->left) - height(node->right) == 2) //���� �ұ���
	{
		if (height(node->left->left) - height(node->left->right) == 1) //������ ���� (left left case)
			node = SingleRightRotate(node);
		else //������ ������ (left right case)
			node = DoubleRightRotate(node);
	}
	else if (height(node->right) - height(node->left) == 2) //������ �ұ���
	{
		if (height(node->right->right) - height(node->right->left) == 1) //�������� ������ (right right case)
			node = SingleLeftRotate(node);
		else //�������� ���� (right left case)
			node = DoubleLeftRotate(node);
	}

	return success;
}

template <typename T>
int AVLTree<T>::ReplaceNode(AVLTreeNode<T>*& node, const T& data)
{
	if (node == nullptr) return 0;

	int compare = compareFunc(node->data, data);
	if (compare > 0) //�����Ͱ� ���� ���
		return ReplaceNode(node->left, data);
	else if (compare < 0) //�����Ͱ� ū ���
		return ReplaceNode(node->right, data);

	//same data found
	node->data = data;
	return 1;
}

template <typename T>
int AVLTree<T>::GetNode(AVLTreeNode<T>*& node, T& data)
{
	if (node == nullptr) return 0;

	int compare = compareFunc(node->data, data);
	if (compare > 0) //�����Ͱ� ���� ���
		return GetNode(node->left, data);
	else if (compare < 0) //�����Ͱ� ū ���
		return GetNode(node->right, data);

	//same data found
	data = node->data;
	return 1;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::SingleLeftRotate(AVLTreeNode<T>* node)
{
	//�ܼ� left rotation. ������ subtree�� ������ node�� �ұ����� �߻��� ���. (outside, right right case)
	AVLTreeNode<T>* right = node->right;
	node->right = right->left;
	right->left = node;
	CalculateHeight(node);
	CalculateHeight(right);
	return right;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::SingleRightRotate(AVLTreeNode<T>* node)
{
	//�ܼ� right rotation. ���� subtree�� ���� node�� �ұ����� �߻��� ���. (outside, left left case)
	AVLTreeNode<T>* left = node->left;
	node->left = left->right;
	left->right = node;
	CalculateHeight(node);
	CalculateHeight(left);
	return left;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::DoubleLeftRotate(AVLTreeNode<T>* node)
{
	//���� left rotation. ������ subtree�� ���� node�� �ұ����� �߻��� ���. (inside, right left case)
	node->right = SingleRightRotate(node->right);
	return SingleLeftRotate(node);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::DoubleRightRotate(AVLTreeNode<T>* node)
{
	//���� right rotation. ���� subtree�� ������ node�� �ұ����� �߻��� ���. (inside, left right case)
	node->left = SingleLeftRotate(node->left);
	return SingleRightRotate(node);
}

template <typename T>
void AVLTree<T>::CalculateHeight(AVLTreeNode<T>*& node)
{
	//�ش� ����� height�� ���
	if (node == nullptr) return; //null�̸� return

	int leftH = -1; //�⺻�� -1 (left�� null�� ���)
	int rightH = -1; //�⺻�� -1 (right�� null�� ���)

	if (node->left != nullptr) leftH = node->left->height; //null�� �ƴϸ� left�� height�� ������
	if (node->right != nullptr) rightH = node->right->height; //null�� �ƴϸ� right�� height�� ������

	node->height = 1 + (leftH > rightH ? leftH : rightH); //�� �� �� ū �� + 1�� ���� (null�� -1, leaf node�� 0�̱� ����)
}

template <typename T>
int AVLTree<T>::height(AVLTreeNode<T>*& node)
{
	return node == nullptr ? -1 : node->height; //node->height�� nullptr�� ��� ������ �߻��ϹǷ� �б� �Ǵ��Ͽ� -1 Ȥ�� height�� ��ȯ��
}

template <typename T>
void AVLTree<T>::findNode(AVLTreeNode<T>*& node, const std::function<bool(T&)>& search, const std::function<void(T&)>& todo)
{
	if (node == nullptr) return; //node�� null�̸� return

	findNode(node->left, search, todo); //���� ��忡 ���� search and do something ����
	if (search(node->data)) todo(node->data); //search function�� parameter�� ���� ����� �������� �ְ� ����. ����� true�� todo �Լ��� ����.
	findNode(node->right, search, todo); //������ ��忡 ���� search and do something ����
}

template <typename T>
void AVLTree<T>::doNode(AVLTreeNode<T>*& node, const std::function<void(T&)>& todo)
{
	if (node == nullptr) return;
	doNode(node->left, todo);
	todo(node->data);
	doNode(node->right, todo);
}
#endif