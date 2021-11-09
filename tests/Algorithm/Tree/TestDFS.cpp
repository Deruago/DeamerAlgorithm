#include "Deamer/Algorithm/Tree/DFS.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

struct Data
{
	Data(int a_) : a(a_)
	{
	}

	int a;
};

struct Node
{
	Data data;
	Node* parent;
	std::vector<std::unique_ptr<Node>> subNodes;

	Node(const Data& data_, Node* parent_ = nullptr) : data(data_), parent(parent_)
	{
	}

	const Node* GetParent() const
	{
		return parent;
	}

	const Data& GetData() const
	{
		return data;
	}

	void AddSubNode(Node* newSubNode)
	{
		subNodes.push_back(std::unique_ptr<Node>(newSubNode));
	}

	void AddSubNode(std::unique_ptr<Node> newSubNode)
	{
		subNodes.push_back(std::move(newSubNode));
	}

	std::vector<const Node*> GetSubNodes() const
	{
		std::vector<const Node*> subnodes;
		for (const auto& subnode : subNodes)
		{
			subnodes.push_back(subnode.get());
		}
		return subnodes;
	}
};

class TestDFS : public testing::Test
{
protected:
	TestDFS()
	{
		// in 1 in 11 out 11 in 12 in 121 out 121 out 12 in 13 in 131 out 131 out 13 out 1
		tree = std::make_unique<Node>(Data(10));
		auto tree_1_1 = std::make_unique<Node>(Data(101), tree.get());
		auto tree_1_2 = std::make_unique<Node>(Data(102), tree.get());
		auto tree_1_3 = std::make_unique<Node>(Data(103), tree.get());
		auto tree_1_2_1 = std::make_unique<Node>(Data(1021), tree_1_2.get());
		auto tree_1_3_1 = std::make_unique<Node>(Data(1031), tree_1_3.get());

		tree_1_2->AddSubNode(std::move(tree_1_2_1));
		tree_1_3->AddSubNode(std::move(tree_1_3_1));

		tree->AddSubNode(std::move(tree_1_1));
		tree->AddSubNode(std::move(tree_1_2));
		tree->AddSubNode(std::move(tree_1_3));
	}

	virtual ~TestDFS() = default;

protected:
	std::unique_ptr<Node> tree;
};

static void TEST_ACTIONS_ARE_CORRECT(
	const Node* tree,
	const std::vector<std::pair<const Node*, deamer::algorithm::tree::DFS::Action>>& actions);

TEST_F(TestDFS, HeapSearch_CorrectlyCallInAndOutFunctions)
{
	const auto actions = deamer::algorithm::tree::DFS::Heap::Search(tree.get(), &Node::GetSubNodes);

	TEST_ACTIONS_ARE_CORRECT(tree.get(), actions);
}

TEST_F(TestDFS, HeapParentSearch_CorrectlyCallInAndOutFunctions)
{
	const auto actions = deamer::algorithm::tree::DFS::Heap::Search(tree.get(), &Node::GetParent,
																	&Node::GetSubNodes);

	TEST_ACTIONS_ARE_CORRECT(tree.get(), actions);
}

TEST_F(TestDFS, StackSearch_CorrectlyCallInAndOutFunctions)
{
	const auto actions =
		deamer::algorithm::tree::DFS::Stack::Search(tree.get(), &Node::GetSubNodes);

	TEST_ACTIONS_ARE_CORRECT(tree.get(), actions);
}

TEST_F(TestDFS, HeapSearch_EmptyTree_CorrectlyCallInAndOutFunctions)
{
	const auto actions =
		deamer::algorithm::tree::DFS::Heap::Search((Node*)nullptr, &Node::GetSubNodes);

	EXPECT_TRUE(actions.empty());
}

TEST_F(TestDFS, HeapParentSearch_EmptyTree_CorrectlyCallInAndOutFunctions)
{
	const auto actions = deamer::algorithm::tree::DFS::Heap::Search(
		(Node*)nullptr, &Node::GetParent, &Node::GetSubNodes);

	EXPECT_TRUE(actions.empty());
}

TEST_F(TestDFS, StackSearch_EmptyTree_CorrectlyCallInAndOutFunctions)
{
	const auto actions =
		deamer::algorithm::tree::DFS::Stack::Search((Node*)nullptr, &Node::GetSubNodes);

	EXPECT_TRUE(actions.empty());
}

TEST_F(TestDFS, HeapSearch_TreeSingleNode_CorrectlyCallInAndOutFunctions)
{
	auto treeSingleNode = std::make_unique<Node>(Data(1));
	const auto actions =
		deamer::algorithm::tree::DFS::Heap::Search(treeSingleNode.get(), &Node::GetSubNodes);

	ASSERT_EQ(2, actions.size());
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode.get(), deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(pair, actions[0]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode.get(), deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(pair, actions[1]);
	}
}

TEST_F(TestDFS, HeapParentSearch_TreeSingleNode_CorrectlyCallInAndOutFunctions)
{
	auto treeSingleNode = std::make_unique<Node>(Data(1));
	const auto actions = deamer::algorithm::tree::DFS::Heap::Search(
		treeSingleNode.get(), &Node::GetParent, &Node::GetSubNodes);

	ASSERT_EQ(2, actions.size());
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode.get(), deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(pair, actions[0]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode.get(), deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(pair, actions[1]);
	}
}

TEST_F(TestDFS, StackSearch_TreeSingleNode_CorrectlyCallInAndOutFunctions)
{
	auto treeSingleNode = std::make_unique<Node>(Data(1));
	const auto actions =
		deamer::algorithm::tree::DFS::Stack::Search(treeSingleNode.get(), &Node::GetSubNodes);

	ASSERT_EQ(2, actions.size());
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode.get(), deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(pair, actions[0]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode.get(), deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(pair, actions[1]);
	}
}

TEST_F(TestDFS, HeapParentSearch_TreeSingleChild_CorrectlyCallInAndOutFunctions)
{
	auto treeSingleNode = std::make_unique<Node>(Data(1));
	auto treeSingleChild = std::make_unique<Node>(Data(11), treeSingleNode.get());
	treeSingleNode->AddSubNode(std::move(treeSingleChild));
	const auto actions = deamer::algorithm::tree::DFS::Heap::Search(
		treeSingleNode.get(), &Node::GetParent, &Node::GetSubNodes);

	ASSERT_EQ(4, actions.size());
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode.get(), deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(pair, actions[0]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode->GetSubNodes()[0], deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(pair, actions[1]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode->GetSubNodes()[0], deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(pair, actions[2]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode.get(), deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(pair, actions[3]);
	}
}

TEST_F(TestDFS, HeapParentSearch_TreeDoubleChild_CorrectlyCallInAndOutFunctions)
{
	auto treeSingleNode = std::make_unique<Node>(Data(1));
	auto treeSingleChild = std::make_unique<Node>(Data(11), treeSingleNode.get());
	auto treeDoubleChild = std::make_unique<Node>(Data(12), treeSingleNode.get());
	treeSingleNode->AddSubNode(std::move(treeSingleChild));
	treeSingleNode->AddSubNode(std::move(treeDoubleChild));
	const auto actions = deamer::algorithm::tree::DFS::Heap::Search(
		treeSingleNode.get(), &Node::GetParent, &Node::GetSubNodes);

	ASSERT_EQ(6, actions.size());
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode.get(), deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(pair, actions[0]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode->GetSubNodes()[0], deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(pair, actions[1]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode->GetSubNodes()[0], deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(pair, actions[2]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode->GetSubNodes()[1], deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(pair, actions[3]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode->GetSubNodes()[1], deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(pair, actions[4]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode.get(), deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(pair, actions[5]);
	}
}

TEST_F(TestDFS, HeapParentSearch_TreeTripleChild_CorrectlyCallInAndOutFunctions)
{
	auto treeSingleNode = std::make_unique<Node>(Data(1));
	auto treeSingleChild = std::make_unique<Node>(Data(11), treeSingleNode.get());
	auto treeDoubleChild = std::make_unique<Node>(Data(12), treeSingleNode.get());
	auto treeTripleChild = std::make_unique<Node>(Data(12), treeSingleNode.get());
	treeSingleNode->AddSubNode(std::move(treeSingleChild));
	treeSingleNode->AddSubNode(std::move(treeDoubleChild));
	treeSingleNode->AddSubNode(std::move(treeTripleChild));

	const auto actions =
		deamer::algorithm::tree::DFS::Heap::Search(treeSingleNode.get(), &Node::GetSubNodes);

	ASSERT_EQ(8, actions.size());
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode.get(), deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(pair, actions[0]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode->GetSubNodes()[0], deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(pair, actions[1]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode->GetSubNodes()[0], deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(pair, actions[2]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode->GetSubNodes()[1], deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(pair, actions[3]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode->GetSubNodes()[1], deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(pair, actions[4]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode->GetSubNodes()[2], deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(pair, actions[5]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode->GetSubNodes()[2], deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(pair, actions[6]);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			treeSingleNode.get(), deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(pair, actions[7]);
	}
}

static void TEST_ACTIONS_ARE_CORRECT(
	const Node* tree,
	const std::vector<std::pair<const Node*, deamer::algorithm::tree::DFS::Action>>& actions)
{
	EXPECT_EQ(12, actions.size());
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			tree, deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(actions[0], pair);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			tree->GetSubNodes()[0], deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(actions[1], pair);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			tree->GetSubNodes()[0], deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(actions[2], pair);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			tree->GetSubNodes()[1], deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(actions[3], pair);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			tree->GetSubNodes()[1]->GetSubNodes()[0], deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(actions[4], pair);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			tree->GetSubNodes()[1]->GetSubNodes()[0], deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(actions[5], pair);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			tree->GetSubNodes()[1], deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(actions[6], pair);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			tree->GetSubNodes()[2], deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(actions[7], pair);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			tree->GetSubNodes()[2]->GetSubNodes()[0], deamer::algorithm::tree::DFS::Action::Entry};
		EXPECT_EQ(actions[8], pair);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			tree->GetSubNodes()[2]->GetSubNodes()[0], deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(actions[9], pair);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			tree->GetSubNodes()[2], deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(actions[10], pair);
	}
	{
		auto pair = std::pair<const Node*, deamer::algorithm::tree::DFS::Action>{
			tree, deamer::algorithm::tree::DFS::Action::Exit};
		EXPECT_EQ(actions[11], pair);
	}
}
