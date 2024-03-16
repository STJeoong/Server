#pragma once
#include <vector>
#include <functional>
template<class... Args>
class Delegate
{
public:
	void operator()(Args&&... args)
	{
		for (auto& func : functions)
			func(std::forward<Args>(args)...);
	}
	void operator+=(const std::function<void(Args...)>& func)
	{
		functions.push_back(std::move(func));
	}
	void operator-=(const std::function<void(Args...)>& func)
	{
		void(* const* funcPtr)(Args...) = func.template target<void(*)(Args...)>();
		if (funcPtr == nullptr)
			return;
		size_t funcHash = func.target_type().hash_code();

		for (auto it = functions.begin(); it != functions.end(); ++it)
		{
			void(* const* ptr)(Args...) = it->template target<void(*)(Args...)>();
			if (*funcPtr == *ptr)
			{
				functions.erase(it);
				return;
			}
		}
	}
private:
	std::vector<std::function<void(Args...)>> functions;
};