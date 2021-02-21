#pragma once

template<int ATsize>
Parser<ATsize>::AT_base::AT_base(std::size_t && count, Parser * const parser,
  command_t && text, format_t && format)
  : number(parser->arrAT.size()), text(text), format(format), count(count)
{
  parser->arrAT.push_back(this);
}

// ============================================================================

template<int ATsize>
struct Parser<ATsize>::string_t : public etl::string<64>
{
  friend std::ostream& operator<<(std::ostream & out, const string_t & str)
  {
    return out << str.c_str();
  }

  template<class ... Args>
  string_t(Args && ... params) : string<string_t::MAX_SIZE>(std::move(params ...))
  {
    
  }

  string_t(void) : string<string_t::MAX_SIZE>()
  {

  }
};

// ============================================================================

template<int ATsize>
Parser<ATsize>::Parser(Parser::prefix_t && prefix, const printf_t write)
  : Write(write), prefix(prefix)
{
  
}

template<int ATsize>
const typename Parser<ATsize>::AT_base * const 
  Parser<ATsize>::findAT(const parseline_t & str)
{
  const AT_base * pAT = nullptr;
  for(auto && at : this->arrAT)
  {
    if(at->format.length() == 0)
    {
      continue;
    }

    const int pos(at->format.find('%'));

    //pos = (pos == -1 && at->text.length() != 0) ? str.length() : pos;
    
    if(str.compare(0, pos, at->format.substr(0, pos)) == 0)
    {
      pAT = at;
    }
  }
  return pAT;
}

template<int ATsize>
void Parser<ATsize>::Parse(parseline_t && str)
{
  const AT_base * const pAT = this->findAT(str);
  if(pAT == nullptr)
  {
    return;
  }

  size_t accumulator = 0, index = 0;
  param_list_t out(pAT->count);
  for(size_t i = 0; i < pAT->format.length() && index < pAT->count; i++)
  {
    if(pAT->format[i] == '%')
    {
      size_t n = accumulator + (i++ - index);
      while(str[n] != pAT->format[i])
      {
        out[index] += str[n++];
        accumulator++;
      }
      index++;
    }
  }
  for(size_t i = index; i < pAT->count; i++)
  {
    out[i] = "0";
  }
  pAT->parse(std::move(out));
  
}

template<int ATsize>
void Parser<ATsize>::print(const int & param) const
{
  char str[string_t::MAX_SIZE] = {0};
  snprintf(str, string_t::MAX_SIZE, "%i", param);
  this->print(str);
}

template<int ATsize>
void Parser<ATsize>::print(const char * str) const
{
  this->Write(str);
}

template<int ATsize>
void Parser<ATsize>::print(const string_t & str) const
{
  this->Write(str.c_str());
}

template<int ATsize>
  template<class First, class Second, class ... Args>
void Parser<ATsize>::print(First && first, Second && sec, Args && ... args) const
{
  if(this->Write == nullptr)
  {
    return;
  }
  this->print(std::move(first));
  this->print(sec, args...);
}
