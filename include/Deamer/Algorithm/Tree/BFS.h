#ifndef DEAMER_ALGORITHM_TREE_BFS_H
#define DEAMER_ALGORITHM_TREE_BFS_H

#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include <type_traits>

namespace deamer::algorithm::tree
{
	/*!	\class BFS
	 *
	 *	\brief Struct containing meta functions to apply BFS on any tree-like structure.
	 *
	 *	\note It is made for trees, any cycle will cause infinite loops.
	 */
	struct BFS
	{
		enum class Action
		{
			Entry,
			Exit,
		};

		template<typename T, typename ExtensionFunction_>
		static std::vector<std::pair<T*, Action>> Search(T* init,
														 ExtensionFunction_ ExtensionFunction)
		{
			if (init == nullptr)
			{
				return {};
			}

			auto t = init;
			std::size_t index = 0;
			std::vector<std::pair<T*, Action>> actions;
			actions.emplace_back(t, Action::Entry);

			while (true)
			{
				auto stopIndex = actions.size();

				// Only possible if nothing new is added
				// i.e. we are done
				if (index == stopIndex)
				{
					break;
				}

				for (; index < stopIndex; index++)
				{
					auto subnodes = std::invoke(ExtensionFunction, actions[index].first);
					for (auto subnode : subnodes)
					{
						actions.emplace_back(subnode, Action::Entry);
					}
				}
			}

			auto currentEnding = actions.size();
			for (auto i = currentEnding - 1; i > 0; --i)
			{
				auto object = actions[i].first;
				actions.emplace_back(object, Action::Exit);
			}
			if (!actions.empty())
			{
				auto object = actions[0].first;
				actions.emplace_back(object, Action::Exit);
			}

			return actions;
		}

		struct Execute
		{
			using Heap = BFS::Execute;

			template<typename T, typename ExtensionFunction_, typename EntryAction_,
					 typename ExitAction_>
			static void Search(T* init, ExtensionFunction_ ExtensionFunction,
							   EntryAction_ EntryAction, ExitAction_ ExitAction)
			{
				if (init == nullptr)
				{
					return;
				}

				const auto actions = BFS::Search(init, ExtensionFunction);
				for (auto [object, action] : actions)
				{
					switch (action)
					{
					case Action::Entry: {
						EntryAction(object);
						break;
					}
					case Action::Exit: {
						ExitAction(object);
						break;
					}
					}
				}
			}
		};
	};
}

#endif // DEAMER_ALGORITHM_TREE_BFS_H
