#include<functional>
#pragma once
class CallbackWaitAll
{
	std::function<void(void)> _cb;
public:
	CallbackWaitAll() :_cb{ []() {} } {};
	CallbackWaitAll(std::function<void(void)> _cb) :_cb{ _cb } {};
	~CallbackWaitAll() {
		_cb();
	}
};
