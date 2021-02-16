#pragma once

#include "AT_Helper.hpp"

template<class... Types>
struct Parser::AT_Command<Types...>::run
{
  template<class... Args> 
  run(Args&&...)
  {  

  }
};


// ============================================================================

template<class... Types>
Parser::AT_Command<Types...>::AT_Command(Parser * parser, std::string text, 
  std::string format, std::function<void(Types...)> func)
  : AT_base(parser, text, format), parser(parser), func(func)
{
  size_t count = 0;
  for(auto& sym : this->format)
  {
    if(sym == '%')
    {
      count++;
    }
  }

  this->count = count;
}

// ============================================================================

template<class... Types> 
template<typename T>
void Parser::AT_Command<Types...>::print(T param)
{
  this->print(std::to_string(param));
}

template<class... Types>
void Parser::AT_Command<Types...>::print(const char * str)
{
  this->print(std::string(str));
}

template<class... Types>
void Parser::AT_Command<Types...>::print(std::string str)
{
  this->parser->Write(str.c_str(), str.length());
  this->buffer += str;
}

template<class... Types> 
int Parser::AT_Command<Types...>::fill(int, std::string val)
{
  return atoi(val.c_str());
}

template<class... Types> 
std::string Parser::AT_Command<Types...>::fill(std::string param, std::string val)
{
  return val;
}

template<class... Types> 
const char * Parser::AT_Command<Types...>::fill(const char * param, std::string val)
{
  return val.c_str();
}

// ============================================================================

template<class... Types> 
template<class... Args>
void Parser::AT_Command<Types...>::operator()(Args&&... params)
{
  this->buffer.clear();
  this->print(this->parser->prefix.c_str());
  this->print(this->text.c_str());
  RUN(this->print, params);
  this->print("\n\r");
}

template<class... Types> 
void Parser::AT_Command<Types...>::parse(std::vector<std::string>& params)
{
  this->parse(params, typename gens<sizeof...(Types)>::type());
}

template<class... Types> 
template<int ...S>
void Parser::AT_Command<Types...>::parse(std::vector<std::string>& params, seq<S...>)
{
  if(this->func != nullptr)
  {
    this->func(fill(std::get<S>(this->params), params[S])...);
  }
}

template<class... Types>
void Parser::AT_Command<Types...>::operator>>(std::function<void(Types...)> func)
{
  this->func = func;
}
