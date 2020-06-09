#pragma once
#include<functional>
#include<memory>
#include<atomic>
auto _do_nothing = []() {};
class CallbackWaitAll;
using CallbackWaitAll_counter = std::shared_ptr<CallbackWaitAll>;
class CallbackWaitAll
{
	std::function<void(void)> _cb;
public:
	CallbackWaitAll() :_cb{ _do_nothing } {};
	CallbackWaitAll(std::function<void(void)> _cb) :_cb{ _cb } {};
	static CallbackWaitAll_counter create() {
		return std::make_shared<CallbackWaitAll>();
	}
	static CallbackWaitAll_counter create(std::function<void(void)> _cb) {
		return std::make_shared<CallbackWaitAll>(_cb);
	}
	~CallbackWaitAll() {
		_cb();
	}
};
