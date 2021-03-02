/**
 * @file function.hpp
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Function class header file
 * 
 * The class stores a pointer to the called object by "type erasure"
 */

#pragma once

#include <memory>

#ifndef ASSERT
  #define ASSERT(exp) assert(exp)
#endif

/**
 * @brief Stub class
 */
template <typename Unused>
class function;

/**
 * @ingroup AT_Parser
 * @brief A class for storing a pointer to a callable object
 * 
 * The class takes the signature of the object and stores a pointer to the
 * callable object that matches the given signature
 * 
 * @tparam Return Returned data type
 * @tparam Args Parameter data types that are passed to the function
 */
template<class Return, class ... Args>
class function <Return (Args ...)>
{
  struct function_base;

  /**
   * @brief Data type for creating a smart pointer to a callable object 
   */
  using invoker_t = std::unique_ptr<function_base>;

  /**
   * @brief Helper abstract class
   * 
   * The class is needed to bind the functionality of all its descendants
   */
  struct function_base
  {
    /**
     * @brief Pure virtual function to call a stored object
     * 
     * @param args Parameters to be passed to the called object
     */
    virtual Return call(Args ... args) const = 0;
    /**
     * @brief Pure virtual function for correct copying of an object
     * 
     * @return A copy of the current object
     */
    virtual invoker_t copy(void) const = 0;
  };

  /**
   * @brief A class that stores directly a pointer to a callable object
   * 
   * @tparam FooT The datatype of the pointer of the called object
   */
  template <typename FooT>
  struct function_holder : function_base
  {
    /**
     * @brief Construct a new function_holder object
     * 
     * @param foo Pointer to the callable object to be stored
     */
    function_holder(FooT foo);

    /**
     * @brief Function to call a stored object
     * 
     * @param args Parameters to be passed to the called object
     */
    Return call(Args ... args) const override;
    /**
     * @brief Function for correct copying of an object
     * 
     * @return A copy of the current object
     */
		invoker_t copy(void) const override;
  
  private:
    FooT mFunction; ///< Stored pointer to callable
  };

public:
  invoker_t mInvoker; ///< Callable wrapper object

  /**
   * @brief Default construct a new function object 
   */
  function(void);
  /**
   * @brief Construct a new function object
   * 
   * @tparam FooT The datatype of the pointer of the called object
   * @param f Pointer to the callable object to be stored
   */
  template <typename FooT>
	function(FooT f);
  /**
   * @brief Copy construct a new function object
   * 
   * @param other Reference to the object to be copied
   */
  function(const function & other);

  /**
   * @brief Stored Object Call Operator
   * 
   * @param args Parameters to be passed to the called object
   */
	Return operator()(Args ... args) const;
  /**
   * @brief The assigning constructor of the function object
   * 
   * @param other Reference to the object to be copied
   * @return Reference to the current object
   */
  function & operator = (const function & other);
};

#include "function.ipp"
