/**
 * @file AT_Command.ipp
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Source Code File for AT Command Class
 * 
 */

#pragma once

// ============================================================================

template<int ATsize> 
  template<class ... Types>
Parser<ATsize>::AT_Command<Types ...>::AT_Command(Parser * const parser,
  const char * text, const format_t format)
  : AT_base(sizeof...(Types), parser, std::move(text), ""), 
    parser(parser), func([](Types ...){ })
{
  this->format = this->formating(format);
}

template<int ATsize> 
  template<class ... Types>
Parser<ATsize>::AT_Command<Types ...>::~AT_Command(void)
{
  // There's nothing here
}
// ============================================================================

template<int ATsize> 
  template<class ... Types> 
const typename Parser<ATsize>::format_t
  Parser<ATsize>::AT_Command<Types ...>::formating(const format_t & format) const
{
  format_t newFormat("");
  for(auto & sym : format)
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
void Parser<ATsize>::AT_Command<Types ...>::parse(const param_list_t & params) const
{
  [&]<std::size_t ...I>(std::index_sequence<I ...>)
  {
    this->func(std::move(this->parser->convert(params[I], (Types *)nullptr)) ...);
  }
  (std::make_index_sequence<sizeof...(Types)>());
}

// ============================================================================

template<int ATsize> 
  template<class ... Types> 
    template<class ... Args>
void Parser<ATsize>::AT_Command<Types ...>::operator()(const Args & ... params) const
{
  this->parser->print(this->parser->prefix.c_str(), this->text.c_str(), 
    params ..., "\n\r");
}

template<int ATsize> 
  template<class ... Types>
void Parser<ATsize>::AT_Command<Types ...>::operator>>(const function_t & pFoo)
{
  this->func = std::move(pFoo); 
}