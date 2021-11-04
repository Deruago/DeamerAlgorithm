#include "Deamer/Algorithm/Tree/BFS.h"
#include <gtest/gtest.h>
#include <memory>

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

	Node* GetParent() const
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

	std::vector<Node*> GetSubNodes() const
	{
		std::vector<Node*> subnodes;
		for (const auto& subnode : subNodes)
		{
			subnodes.push_back(subnode.get());
		}
		return subnodes;
	}
};

class TestBFS : public testing::Test
{
protected:
	TestBFS()
	{
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

	virtual ~TestBFS() = default;

protected:
	std::unique_ptr<Node> tree;
};

static void TEST_ACTIONS_ARE_CORRECT(
	Node* tree, const std::vector<std::pair<Node*, deamer::algorithm::tree::BFS::Action>>& actions);

TEST_F(TestBFS, Search_CorrectlyCallInAndOutFunctions)
{
	const auto actions = deamer::algorithm::tree::BFS::Search(tree.get(), &Node::GetSubNodes);

	TEST_ACTIONS_ARE_CORRECT(tree.get(), actions);
}

static void TEST_ACTIONS_ARE_CORRECT(
	Node* tree, const std::vector<std::pair<Node*, deamer::algorithm::tree::BFS::Action>>& actions)
{
	EXPECT_EQ(12, actions.size());
	{
		auto pair = std::pair<Node*, deamer::algorithm::tree::BFS::Action>{
			tree, deamer::algorithm::tree::BFS::Action::Entry};
		EXPECT_EQ(actions[0], pair);
	}
	{
		auto pair = std::pair<Node*, deamer::algorithm::tree::BFS::Action>{
			tree->GetSubNodes()[0], deamer::algorithm::tree::BFS::Action::Entry};
		EXPECT_EQ(actions[1], pair);
	}
	{
		auto pair = std::pair<Node*, deamer::algorithm::tree::BFS::Action>{
			tree->GetSubNodes()[1], deamer::algorithm::tree::BFS::Action::Entry};
		EXPECT_EQ(actions[2], pair);
	}
	{
		auto pair = std::pair<Node*, deamer::algorithm::tree::BFS::Action>{
			tree->GetSubNodes()[2], deamer::algorithm::tree::BFS::Action::Entry};
		EXPECT_EQ(actions[3], pair);
	}
	{
		auto pair = std::pair<Node*, deamer::algorithm::tree::BFS::Action>{
			tree->GetSubNodes()[1]->GetSubNodes()[0], deamer::algorithm::tree::BFS::Action::Entry};
		EXPECT_EQ(actions[4], pair);
	}
	{
		auto pair = std::pair<Node*, deamer::algorithm::tree::BFS::Action>{
			tree->GetSubNodes()[2]->GetSubNodes()[0], deamer::algorithm::tree::BFS::Action::Entry};
		EXPECT_EQ(actions[5], pair);
	}
	{
		auto pair = std::pair<Node*, deamer::algorithm::tree::BFS::Action>{
			tree->GetSubNodes()[2]->GetSubNodes()[0], deamer::algorithm::tree::BFS::Action::Exit};
		EXPECT_EQ(actions[6], pair);
	}
	{
		auto pair = std::pair<Node*, deamer::algorithm::tree::BFS::Action>{
			tree->GetSubNodes()[1]->GetSubNodes()[0], deamer::algorithm::tree::BFS::Action::Exit};
		EXPECT_EQ(actions[7], pair);
	}
	{
		auto pair = std::pair<Node*, deamer::algorithm::tree::BFS::Action>{
			tree->GetSubNodes()[2], deamer::algorithm::tree::BFS::Action::Exit};
		EXPECT_EQ(actions[8], pair);
	}
	{
		auto pair = std::pair<Node*, deamer::algorithm::tree::BFS::Action>{
			tree->GetSubNodes()[1], deamer::algorithm::tree::BFS::Action::Exit};
		EXPECT_EQ(actions[9], pair);
	}
	{
		auto pair = std::pair<Node*, deamer::algorithm::tree::BFS::Action>{
			tree->GetSubNodes()[0], deamer::algorithm::tree::BFS::Action::Exit};
		EXPECT_EQ(actions[10], pair);
	}
	{
		auto pair = std::pair<Node*, deamer::algorithm::tree::BFS::Action>{
			tree, deamer::algorithm::tree::BFS::Action::Exit};
		EXPECT_EQ(actions[11], pair);
	}
}
