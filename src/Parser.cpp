#include "Parser.hpp"

using namespace std;

Parser::AT_base::AT_base(Parser * parser, std::string text, std::string format)
  : text(text), format(format)
{
  this->number = parser->arrAT.size();
  parser->arrAT.push_back(this);
}

Parser::Parser(std::string prefix, 
  void (*write)(const char * buf, size_t size))
  : Write(write), prefix(prefix)
{

}

void Parser::Parse(string str)
{
  AT_base * pAT = nullptr;
  for(auto& at : this->arrAT)
  {
    if(str.compare(0, at->text.length(), at->text) == 0)
    {
      pAT = at;
    }
  }
  if(pAT == nullptr)
  {
    return;
  }

  size_t accumulator = 0, index = 0;
  vector<string> out(pAT->count);
  for(size_t i = 0; i < pAT->format.length(); i++)
  {
    if(pAT->format[i] == '%')
    {
      size_t n = pAT->text.length() + (i - 2 * index) + accumulator;
      while(str[n] != pAT->format[i+2])
      {
        out[index] += str[n++];
        accumulator++;
      }
      index++;
      i += 2;
    }
  }
  pAT->parse(out);
}

void Parser::AT_base::parse(std::vector<std::string>& params)
{

}
