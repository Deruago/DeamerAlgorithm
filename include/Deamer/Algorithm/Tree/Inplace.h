#ifndef DEAMER_ALGORITHM_TREE_INPLACE_H
#define DEAMER_ALGORITHM_TREE_INPLACE_H

#include <vector>
#include <set>
#include <map>
#include <stack>
#include <iostream>
#include <functional>

namespace deamer::algorithm::tree
{
	/*!	\class Inplace
	 *
	 *	\brief Struct containing meta functions to apply the following recursive pattern:
	 *	```
	 *	if (Condition) // Default check if parent is not nullptr
	 *	{
	 *		Operation(Function(), Recurse());
	 *	}
	 *	else
	 *	{
	 *		Function();
	 *	}
	 *	```
	 *	\details These meta functions, construct a sequence of calls. These calls
	 *	can be executed in order, with a given operator or function in between return values.
	 */
	struct Inplace
	{
		struct Heap
		{
			template<typename T, typename ParentFunction_>
			static std::vector<T*> RequiredCalls(T* t, ParentFunction_ GetParentFunction)
			{
				std::vector<T*> ts;
				while (t != nullptr)
				{
					ts.push_back(t);
					t = std::invoke(GetParentFunction, t);
				}
				
				return ts;
			}
			
			template<typename T, typename ParentFunction_, typename Conditional_>
			static std::vector<T*> RequiredCalls(T* t, ParentFunction_ GetParentFunction, Conditional_ ConditionalRecurseFunction)
			{
				std::vector<T*> ts;
				while (std::invoke(ConditionalRecurseFunction, t))
				{
					ts.push_back(t);
					t = std::invoke(GetParentFunction, t);
				}
				
				return ts;
			}
		};
		
		struct Stack
		{
		};

		struct Execute
		{
			struct Heap
			{
				template<typename T, typename ParentFunction_, typename Action>
				static void Construction(T* t, ParentFunction_ GetParentFunction, Action action)
				{
					auto requiredCalls = Inplace::Heap::RequiredCalls(t, GetParentFunction);
					for (auto call : requiredCalls)
					{
						action(call);
					}
				}
			};
		};
	};
}

#endif // DEAMER_ALGORITHM_TREE_INPLACE_H