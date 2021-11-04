#ifndef DEAMER_ALGORITHM_TREE_DFS_H
#define DEAMER_ALGORITHM_TREE_DFS_H

#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>

namespace deamer::algorithm::tree
{
	/*!	\class DFS
	 *
	 *	\brief Struct containing meta functions to apply DFS on any tree-like structure.
	 *
	 *	\note It is made for trees, any cycle will cause infinite loops.
	 */
	struct DFS
	{
		enum class Action
		{
			Entry,
			Exit,
		};

		// Algorithms using Heap approach
		// Will not cause stack overflows on large inputs.
		struct Heap
		{
			// This is slower than variant using the Parent function.
			// This is due to required lookup whether a node has been visited already.
			template<typename T, typename ExtensionFunction_>
			static std::vector<std::pair<T*, Action>> Search(T* init,
															 ExtensionFunction_ ExtensionFunction)
			{
				if (init == nullptr)
				{
					return {};
				}
				
				auto* t = init;
				std::stack<T*> ts;
				std::vector<std::pair<T*, Action>> actions;
				std::set<T*> visited;

				ts.push(t);
				while (true)
				{
					if (visited.find(t) == visited.end())
					{
						actions.emplace_back(t, Action::Entry);
						auto subnodes = std::invoke(ExtensionFunction, t);
						for (auto i = std::rbegin(subnodes); i != std::rend(subnodes); ++i)
						{
							ts.push((*i));
						}
					}
					else
					{
						ts.pop();
						actions.emplace_back(t, Action::Exit);
					}

					visited.emplace(t);
					t = ts.top();
					if (t == init)
					{
						actions.emplace_back(t, Action::Exit);
						break;
					}
				}

				return actions;
			}

			template<typename T, typename ParentFunction_, typename ExtensionFunction_>
			static std::vector<std::pair<T*, Action>>
			Search(T* init, ParentFunction_ GetParentFunction, ExtensionFunction_ ExtensionFunction)
			{
				if (init == nullptr)
				{
					return {};
				}
				
				bool complete = false;
				auto* t = init;
				std::stack<std::pair<std::size_t, std::size_t>> size;
				std::vector<std::pair<T*, Action>> actions;

				size.push({std::invoke(ExtensionFunction, t).size(), 0});

				while (!complete)
				{
					decltype(std::invoke(ExtensionFunction, t)) subnodes;
					while (true)
					{
						actions.emplace_back(t, Action::Entry);
						subnodes = std::invoke(ExtensionFunction, t);
						if (subnodes.empty())
						{
							break;
						}

						t = subnodes[0];
						size.push({subnodes.size(), 0});
					}

					while (true)
					{
						const auto tOriginal = t;
						t = std::invoke(GetParentFunction, t);
						if (t == nullptr)
						{
							complete = true;
							break;
						}

						const auto top = size.top();
						if (top.first == top.second + 1)
						{
							size.pop();
							const auto& lastAction = (*--actions.end());
							if (!(lastAction.first == tOriginal &&
								  lastAction.second == Action::Exit))
							{
								actions.emplace_back(tOriginal, Action::Exit);
							}
							actions.emplace_back(t, Action::Exit);
							if (size.empty())
							{
								complete = true;
								break;
							}
							continue;
						}
						// otherwise it is covered via parent logic
						if (std::invoke(ExtensionFunction, tOriginal).empty())
						{
							actions.emplace_back(tOriginal, Action::Exit);
						}

						t = std::invoke(ExtensionFunction, t)[++size.top().second];
						break;
					}
				}
				
				if (actions.size() == 1)
				{
					actions.emplace_back(init, Action::Exit);
				}

				return actions;
			}
		};

		// Algorithms using Stack approach
		// Can cause stack overflows on large inputs
		struct Stack
		{
			template<typename T, typename ExtensionFunction_>
			static std::vector<std::pair<T*, Action>> Search(T* init,
															 ExtensionFunction_ ExtensionFunction)
			{
				if (init == nullptr)
				{
					return {};
				}
				
				std::vector<std::pair<T*, Action>> actions;

				DFS::Stack::SearchLogic(init, ExtensionFunction, actions);

				return actions;
			}

			template<typename T, typename ExtensionFunction_>
			static void SearchLogic(T* init, ExtensionFunction_ ExtensionFunction,
									std::vector<std::pair<T*, Action>>& actions)
			{
				if (init == nullptr)
				{
					return;
				}
				
				actions.emplace_back(init, Action::Entry);

				auto subnodes = std::invoke(ExtensionFunction, init);
				for (auto subnode : subnodes)
				{
					SearchLogic(subnode, ExtensionFunction, actions);
				}

				actions.emplace_back(init, Action::Exit);
			}
		};

		template<typename... Args>
		static inline auto Search(Args&&... args)
			-> decltype(DFS::Heap::Search(std::forward<Args>(args)...))
		{
			return DFS::Heap::Search(std::forward<Args>(args)...);
		}

		// Automatically execute entry and exit functions after search.
		struct Execute
		{
			template<typename T, typename EntryAction_, typename ExitAction_>
			static void ActionExecution(std::vector<std::pair<T*, Action>> actions,
										EntryAction_ EntryAction, ExitAction_ ExitAction)
			{
				for (const auto& [object, action] : actions)
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

			struct Heap
			{
				// EntryAction and ExitAction need to be static functions.
				// Later support gets added to support member-functions
				template<typename T, typename ExtensionFunction_, typename EntryAction_,
						 typename ExitAction_>
				static void Search(T* init, ExtensionFunction_ ExtensionFunction,
								   EntryAction_ EntryAction, ExitAction_ ExitAction)
				{
					ActionExecution(DFS::Heap::Search(init, ExtensionFunction), EntryAction,
									ExitAction);
				}

				template<typename T, typename ParentFunction_, typename ExtensionFunction_,
						 typename EntryAction_, typename ExitAction_>
				static void Search(T* init, ParentFunction_ GetParentFunction,
								   ExtensionFunction_ ExtensionFunction, EntryAction_ EntryAction,
								   ExitAction_ ExitAction)
				{
					ActionExecution(DFS::Heap::Search(init, GetParentFunction, ExtensionFunction),
									EntryAction, ExitAction);
				}
			};

			struct Stack
			{
				template<typename T, typename ExtensionFunction_, typename EntryAction_,
						 typename ExitAction_>
				static void Search(T* init, ExtensionFunction_ ExtensionFunction,
								   EntryAction_ EntryAction, ExitAction_ ExitAction)
				{
					DFS::Execute::ActionExecution(DFS::Stack::Search(init, ExtensionFunction),
												  EntryAction, ExitAction);
				}
			};

			template<typename... Args>
			static inline auto Search(Args&&... args)
				-> decltype(DFS::Execute::Heap::Search(std::forward<Args>(args)...))
			{
				return DFS::Execute::Heap::Search(std::forward<Args>(args)...);
			}
		};
	};
}

#endif // DEAMER_ALGORITHM_TREE_DFS_H
