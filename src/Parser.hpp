#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <functional>

class Parser
{
  class AT_base;

protected:
  template<class... Types>
  class AT_Command;

private:
  std::vector<AT_base *> arrAT;
  std::string prefix;
  void (*Write)(const char * buf, size_t size);

protected:
  Parser(std::string prefix, void (*write)(const char * buf, size_t size));

public:
  void Parse(std::string);
};

class Parser::AT_base
{
  friend class Parser;
protected:
  uint16_t number;

  std::string text;
  std::string format;
  size_t count = 0;

  virtual void parse(std::vector<std::string>& params);

  AT_base(Parser * parser, std::string text, std::string format);
};

template<class... Types>
class Parser::AT_Command : public AT_base
{    
  Parser * parser;
  struct run;
  
  template<typename T>
  void print(T param);
  void print(const char * param);
  void print(std::string param);

  std::string buffer = "";

public:
  std::function<void(Types...)> foo;
  void parse(std::vector<std::string>& params) override;

  AT_Command(Parser * parser, std::string text, std::string format);

  template<class... Args>
  void operator()(Args&&... param);

  #define RUN(FOO, PARAM) run{ (FOO(PARAM), 1)... }
};

template<class... Types>
struct Parser::AT_Command<Types...>::run
{
  template<class... Args> 
  run(Args&&...)
  {  

  }
};

// -------------------------------------------------------------------------
template<class... Types>
Parser::AT_Command<Types...>::AT_Command(Parser * parser, std::string text, 
  std::string format)
  : AT_base(parser, text, format), parser(parser)
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

template<class... Types> 
template<typename T>
void Parser::AT_Command<Types...>::print(T param)
{
  std::string str(std::to_string(param));
  this->parser->Write(str.c_str(), str.length());
  this->buffer += str;
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
  for(auto& p : params)
  {
    std::cout << p << std::endl;
  }
  /*
    Код вызова foo
    Думаю через рекурсию с пакетом шаблонов сделать
    а в конце, когда счетчик рекурсии досчитает до count,
    вызвать foo распаковав набравшийся пакет
  */
}
