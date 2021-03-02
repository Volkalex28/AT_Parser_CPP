/**
 * @file function.ipp
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Function class source file
 * 
 * The class stores a pointer to the called object by "type erasure"
 */

#pragma once

template<class Return, class ... Args>
  template <typename FooT>
function<Return (Args ...)>::function_holder<FooT>::function_holder(FooT foo)
  : mFunction(foo)
{

}

template<class Return, class ... Args>
  template <typename FooT>
Return function<Return (Args ...)>::function_holder<FooT>::call(Args ... args) const
{
  return this->mFunction(args ...);
}

template<class Return, class ... Args>
  template <typename FooT>
typename function<Return (Args ...)>::invoker_t 
  function<Return (Args ...)>::function_holder<FooT>::copy(void) const
{
  return invoker_t(new function_holder<FooT>(mFunction));
}

template<class Return, class ... Args>
function<Return (Args ...)>::function(void) : mInvoker()
{

}

template<class Return, class ... Args>
  template <typename FooT>
function<Return (Args ...)>::function(FooT f) 
  : mInvoker(new function_holder<FooT>(f))
{

}

template<class Return, class ... Args>
function<Return (Args ...)>::function(const function & other)
  : mInvoker(other.mInvoker->copy())
{

}

template<class Return, class ... Args>
Return function<Return (Args ...)>::operator()(Args ... args) const
{
  ASSERT((bool)this->mInvoker);

  return this->mInvoker->call(args ...);
}

template<class Return, class... Args> 
function<Return (Args...)> & 
  function<Return (Args...)>::operator=(const function & other)
{
  this->mInvoker = other.mInvoker->copy();
  return *this;
}
