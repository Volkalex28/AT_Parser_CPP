#pragma once

// ============================================================================

template<int ATsize> 
  template<class ... Types>
Parser<ATsize>::AT_Command<Types ...>::AT_Command(Parser * const parser,
  command_t && text, format_t && format)
  : AT_base(sizeof...(Types), parser, std::move(text), ""), 
    parser(parser), func([](Types ...){ })
{
  this->format = this->formating(std::move(format));
}

// ============================================================================

template<int ATsize> 
  template<class ... Types> 
int Parser<ATsize>::AT_Command<Types ...>::convert(string_t && val, int *) const
{
  return atoi(val.c_str());
}

template<int ATsize> 
  template<class ... Types> 
typename Parser<ATsize>::string_t
  Parser<ATsize>::AT_Command<Types ...>::convert(string_t && val, string_t *) const
{
  return val;
}

template<int ATsize> 
  template<class ... Types> 
const char * Parser<ATsize>::AT_Command<Types ...>::convert(string_t && val,
  const char **) const
{
  return val.c_str();
}

template<int ATsize> 
  template<class ... Types> 
const typename Parser<ATsize>::format_t 
  Parser<ATsize>::AT_Command<Types ...>::formating(format_t && format) const
{
  format_t newFormat("");
  for(auto && sym : format)
  {
    if(sym == '#')
    {
      newFormat += this->text;
      continue;
    }
    newFormat += sym;
  }
  return newFormat;
}

template<int ATsize> 
  template<class ... Types> 
void Parser<ATsize>::AT_Command<Types ...>::parse(param_list_t && params) const
{
  [&]<std::size_t ...I>(std::index_sequence<I ...>)
  {
    this->func(this->convert(std::move(params[I]), (Types *)nullptr) ...);
  }
  (std::make_index_sequence<sizeof...(Types)>());
}
// ============================================================================

template<int ATsize> 
  template<class ... Types> 
    template<class ... Args>
void Parser<ATsize>::AT_Command<Types ...>::operator()(Args && ... params) const
{
  this->parser->print(this->parser->prefix, this->text, params ..., "\n\r");
}

template<int ATsize> 
  template<class ... Types>
void Parser<ATsize>::AT_Command<Types ...>::operator>>(function_t pFoo)
{
  assert(pFoo);

  this->func = pFoo;
}