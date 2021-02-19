#pragma once

template<int ATsize>
Parser<ATsize>::AT_base::AT_base(std::size_t count, Parser * parser, command_t && text, 
  format_t && format)
  : text(text), format(format), count(count)
{
  this->number = parser->arrAT.size();
  parser->arrAT.push_back(this);
}

template<int ATsize>
struct Parser<ATsize>::string_t : public param_t
{
  friend std::ostream& operator<<(std::ostream & out, const string_t & str)
  {
    return out << str.c_str();
  }

  template<class ... Args>
  string_t(Args ... params) : param_t(params ...)
  {
    
  }
};

template<int ATsize>
Parser<ATsize>::Parser(Parser::prefix_t && prefix, printf_t write)
  : Write(write), prefix(prefix)
{
  if(write == nullptr)
  {
    Write = [](const char *){};
  }
}

template<int ATsize>
void Parser<ATsize>::Parse(parseline_t && str)
{
  AT_base * pAT = nullptr;
  for(auto & at : this->arrAT)
  {
    if(str.compare(0, at->text.length(), at->text) == 0)
    {
      pAT = at;
      break;
    }
  }
  if(pAT == nullptr)
  {
    return;
  }
  //std::cout << pAT->text.length() << std::endl;

  size_t accumulator = 0, index = 0;
  param_list_t out(pAT->count);
  for(size_t i = 0; i < pAT->format.length() && index < pAT->count; i++)
  {
    if(pAT->format[i] == '%')
    {
      size_t n = pAT->text.length() + accumulator + (i++ - index);
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
  pAT->parse(out);
}

template<int ATsize>
void Parser<ATsize>::AT_base::parse(param_list_t & params)
{

}

template<int ATsize>
void Parser<ATsize>::print(int param)
{
  char str[param_t::MAX_SIZE] = {0};
  snprintf(str, param_t::MAX_SIZE, "%i", param);
  this->print(str);
}

template<int ATsize>
void Parser<ATsize>::print(const char * str)
{
  this->Write(str);
}

template<int ATsize>
void Parser<ATsize>::print(param_t& str)
{
  this->Write(str.c_str());
}
