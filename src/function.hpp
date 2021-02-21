#pragma once

#include <memory>

template <typename Unused>
class function;

template<class Return, class ... Args>
class function <Return (Args ...)>
{
  struct function_base;

  using invoker_t = std::unique_ptr<function_base>;

  struct function_base
  {
    virtual Return call(Args ...) const = 0;
    virtual invoker_t copy(void) const = 0;
  };

  template <typename FooT>
  struct function_holder : function_base
  {
    function_holder(FooT foo);

    Return call(Args ... args) const override;
		invoker_t copy(void) const override;
  
  private:
    FooT mFunction;
  };

public:
  invoker_t mInvoker;

  function(void);
  
  template <typename FooT>
	function(FooT f);

  function(const function & other);

	Return operator()(Args ... args) const;
  function & operator = (const function & other);
};

#include "functionCPP.hpp"
