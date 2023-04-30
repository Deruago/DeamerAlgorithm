#ifndef DEAMER_ALGORITHM_TREE_DFS_H
#define DEAMER_ALGORITHM_TREE_DFS_H

#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include <type_traits>

namespace deamer::algorithm::tree
{
	/*!	\class DFS
	 *
	 *	\brief Struct containing meta functions to apply DFS on any tree-like structure.
	 *
	 *	\note It is made for trees, any cycle will cause infinite loops.
	 *
	 *	\warning This class assumes that the user has gives the following signatures:
	 *	- ExtensionFunction accepts the init object
	 *	- ExtensionFunction returns an STL type such as: vector, or type having "value_type" alias
	 *	- ParentFunction accepts the init object
	 *	- ParentFunction returns raw pointer
	 *
	 *	When static function actions are given, no object has to be given after the actions.
	 *	If the actions are member functions, the corresponding object should be given.
	 *
	 *	In later versions the specific signatures and preconditions might be removed, for now not
	 *	following the above rules, makes the meta function unusable.
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
		// Note the output contains twice the objects that were given as input.
		struct Heap
		{
			// This is slower than variant using the Parent function.
			// This is due to required lookup whether a node has been visited already.
			template<typename T, typename ExtensionFunction_>
			static auto Search(T* init, ExtensionFunction_ ExtensionFunction)
				-> std::vector<std::pair<
					std::conditional_t<
						std::is_const_v<T> ||
							std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
								ExtensionFunction, init))::value_type>>,
						const T*, T*>,
					Action>>
			{
				if (init == nullptr)
				{
					return {};
				}

				std::conditional_t<
					std::is_const_v<T> ||
						std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
							ExtensionFunction, init))::value_type>>,
					const T*, T*>
					t = init;
				std::stack<std::conditional_t<
					std::is_const_v<T> ||
						std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
							ExtensionFunction, init))::value_type>>,
					const T*, T*>>
					ts;
				std::vector<std::pair<
					std::conditional_t<
						std::is_const_v<T> ||
							std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
								ExtensionFunction, init))::value_type>>,
						const T*, T*>,
					Action>>
					actions;
				std::set<std::conditional_t<
					std::is_const_v<T> ||
						std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
							ExtensionFunction, init))::value_type>>,
					const T*, T*>>
					visited;

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
			static auto Search(T* init, ParentFunction_ GetParentFunction,
							   ExtensionFunction_ ExtensionFunction)
				-> std::vector<std::pair<
					std::conditional_t<
						std::is_const_v<T> ||
							std::is_const_v<std::remove_pointer_t<
								decltype(std::invoke(GetParentFunction, init))>> ||
							std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
								ExtensionFunction, init))::value_type>>,
						const T*, T*>,
					Action>>
			{
				using store_T = std::conditional_t<
					std::is_const_v<T> ||
						std::is_const_v<std::remove_pointer_t<decltype(std::invoke(
							GetParentFunction, init))>> ||
						std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
							ExtensionFunction, init))::value_type>>,
					const T*, T*>;

				if (init == nullptr)
				{
					return {};
				}
				std::conditional_t<std::is_const_v<T> ||
									   std::is_const_v<std::remove_pointer_t<decltype(
										   std::invoke(GetParentFunction, init))>> ||
									   std::is_const_v<std::remove_pointer_t<typename decltype(
										   std::invoke(ExtensionFunction, init))::value_type>>,
								   const T*, T*>
					parentPointer = std::invoke(GetParentFunction, init);
				
				bool complete = false;
				std::conditional_t<
					std::is_const_v<T> ||
						std::is_const_v<std::remove_pointer_t<decltype(std::invoke(
							GetParentFunction, init))>> ||
						std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
							ExtensionFunction, init))::value_type>>,
					const T*, T*>
					t = init;
				std::stack<std::pair<std::size_t, std::size_t>> size;
				std::vector<std::pair<
					std::conditional_t<
						std::is_const_v<T> ||
							std::is_const_v<std::remove_pointer_t<decltype(std::invoke(
								GetParentFunction, init))>> ||
							std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
								ExtensionFunction, init))::value_type>>,
						const T*, T*>,
					Action>>
					actions;

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
						if (t == parentPointer) // Checks if the parent is the parent of the starting node.
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

						auto tmp1 = ++size.top().second;
						auto tmp2 = std::invoke(ExtensionFunction, t);
						t = tmp2[tmp1];
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
			static auto Search(T* init, ExtensionFunction_ ExtensionFunction)
			{
				using store_T = std::conditional_t<
					std::is_const_v<T> ||
						std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
							ExtensionFunction, init))::value_type>>,
					const T*, T*>;

				if (init == nullptr)
				{
					return std::vector<std::pair<
						std::conditional_t<
							std::is_const_v<T> ||
								std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
									ExtensionFunction, init))::value_type>>,
							const T*, T*>,
						Action>>{};
				}

				std::vector<std::pair<
					std::conditional_t<
						std::is_const_v<T> ||
							std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
								ExtensionFunction, init))::value_type>>,
						const T*, T*>,
					Action>>
					actions;

				DFS::Stack::SearchLogic(init, ExtensionFunction, actions);

				return actions;
			}

			template<typename T, typename ExtensionFunction_>
			static void SearchLogic(
				T* init, ExtensionFunction_ ExtensionFunction,
				std::vector<std::pair<
					std::conditional_t<
						std::is_const_v<T> ||
							std::is_const_v<std::remove_pointer_t<typename decltype(std::invoke(
								ExtensionFunction, init))::value_type>>,
						const T*, T*>,
					Action>>& actions)
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

			template<typename T, typename EntryAction_, typename ExitAction_, typename T_Action>
			static void ActionExecution(std::vector<std::pair<T*, Action>> actions,
										EntryAction_ EntryAction, ExitAction_ ExitAction,
										T_Action actionObject)
			{
				for (const auto& [object, action] : actions)
				{
					switch (action)
					{
					case Action::Entry: {
						std::invoke(EntryAction, actionObject, object);
						break;
					}
					case Action::Exit: {
						std::invoke(ExitAction, actionObject, object);
						break;
					}
					}
				}
			}

			struct Heap
			{
				template<typename T, typename ExtensionFunction_, typename EntryAction_,
						 typename ExitAction_>
				static void Search(T* init, ExtensionFunction_ ExtensionFunction,
								   EntryAction_ EntryAction, ExitAction_ ExitAction)
				{
					ActionExecution(DFS::Heap::Search(init, ExtensionFunction), EntryAction,
									ExitAction);
				}

				template<
					typename T, typename ExtensionFunction_, typename EntryAction_,
					typename ExitAction_, typename T_Action,
					std::enable_if_t<!std::is_function_v<T>, bool> = true,
					std::enable_if_t<std::is_member_function_pointer_v<ExtensionFunction_>, bool> =
						true,
					std::enable_if_t<std::is_member_function_pointer_v<EntryAction_>, bool> = true,
					std::enable_if_t<std::is_member_function_pointer_v<ExitAction_>, bool> = true,
					std::enable_if_t<!std::is_function_v<T_Action>, bool> = true>
				static void Search(T* init, ExtensionFunction_ ExtensionFunction,
								   EntryAction_ EntryAction, ExitAction_ ExitAction,
								   T_Action actionObject)
				{
					ActionExecution(DFS::Heap::Search(init, ExtensionFunction), EntryAction,
									ExitAction, actionObject);
				}

				template<typename T, typename ParentFunction_, typename ExtensionFunction_,
						 typename EntryAction_, typename ExitAction_,
						 std::enable_if_t<!std::is_function_v<T>, bool> = true,
						 std::enable_if_t<std::is_member_function_pointer_v<ParentFunction_>,
										  bool> = true,
						 std::enable_if_t<std::is_member_function_pointer_v<ExtensionFunction_>,
										  bool> = true,
						 std::enable_if_t<std::is_function_v<EntryAction_>, bool> = true,
						 std::enable_if_t<std::is_function_v<ExitAction_>, bool> = true>
				static void Search(T* init, ParentFunction_ GetParentFunction,
								   ExtensionFunction_ ExtensionFunction, EntryAction_ EntryAction,
								   ExitAction_ ExitAction)
				{
					ActionExecution(DFS::Heap::Search(init, GetParentFunction, ExtensionFunction),
									EntryAction, ExitAction);
				}

				template<
					typename T, typename ParentFunction_, typename ExtensionFunction_,
					typename EntryAction_, typename ExitAction_, typename T_Action,
					std::enable_if_t<!std::is_function_v<T>, bool> = true,
					std::enable_if_t<std::is_member_function_pointer_v<EntryAction_>, bool> = true,
					std::enable_if_t<std::is_member_function_pointer_v<ExitAction_>, bool> = true,
					std::enable_if_t<!std::is_function_v<T_Action>, bool> = true>
				static void Search(T* init, ParentFunction_ GetParentFunction,
								   ExtensionFunction_ ExtensionFunction, EntryAction_ EntryAction,
								   ExitAction_ ExitAction, T_Action actionObject)
				{
					ActionExecution(DFS::Heap::Search(init, GetParentFunction, ExtensionFunction),
									EntryAction, ExitAction, actionObject);
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

				template<typename T, typename ExtensionFunction_, typename EntryAction_,
						 typename ExitAction_, typename T_Action>
				static void Search(T* init, ExtensionFunction_ ExtensionFunction,
								   EntryAction_ EntryAction, ExitAction_ ExitAction,
								   T_Action actionObject)
				{
					DFS::Execute::ActionExecution(DFS::Stack::Search(init, ExtensionFunction),
												  EntryAction, ExitAction, actionObject);
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
