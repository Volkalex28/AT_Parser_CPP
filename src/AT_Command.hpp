#pragma once

// ============================================================================

template<int ATsize>
template<class ... Types>
Parser<ATsize>::AT_Command<Types ...>::AT_Command(Parser * parser, command_t && text, 
  format_t && format)
  : AT_base(sizeof...(Types), parser, command_t(text), format_t(format)),
    parser(parser), func([](Types ...){})
{

}

// ============================================================================

template<int ATsize>
template<class ... Types> 
int Parser<ATsize>::AT_Command<Types ...>::fill(int * ptr, param_t & val)
{
  return atoi(val.c_str());
}

template<int ATsize>
template<class ... Types> 
typename Parser<ATsize>::param_t & Parser<ATsize>::AT_Command<Types ...>::fill(param_t * ptr, param_t & val)
{
  return val;
}

template<int ATsize>
template<class ... Types> 
const char * Parser<ATsize>::AT_Command<Types ...>::fill(const char ** ptr, param_t & val)
{
  return val.c_str();
}

// ============================================================================

template<int ATsize>
template<class ... Types> 
template<class ... Args>
void Parser<ATsize>::AT_Command<Types ...>::operator()(Args && ... params)
{
  this->parser->print(this->parser->prefix.c_str());
  this->parser->print(this->text.c_str());
  RUN(this->parser->print, params);
  this->parser->print("\n\r");
}

template<int ATsize>
template<class ... Types> 
void Parser<ATsize>::AT_Command<Types ...>::parse(param_list_t & params)
{
  this->parse(params, typename gens<sizeof...(Types)>::type());
}

template<int ATsize>
template<class ... Types> 
template<std::size_t ... S>
void Parser<ATsize>::AT_Command<Types ...>::parse(param_list_t & params, seq<S ...>)
{
  //this->func(fill(std::get<S>(this->params), params[S]) ...);
  this->func(fill((Types*)0, params[S]) ...);
}

template<int ATsize>
template<class ... Types>
void Parser<ATsize>::AT_Command<Types ...>::operator>>(function_t pFoo)
{
  this->func = pFoo;
}
