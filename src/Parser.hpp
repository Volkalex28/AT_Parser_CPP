#pragma once

#include <iostream>
#include <cstdlib>
#include <etl/include/etl/string.h>
#include <etl/include/etl/vector.h>
#include <functional>

#include "AT_Helper.hpp"

#define RUN(FOO, PARAM) run{ (FOO(PARAM), 1) ... }

template<int ATsize>
class Parser
{
  using command_t = etl::string<15>;
  using format_t = etl::string<25>;
  using param_t = etl::string<64>;
  using param_list_t = etl::vector<param_t, 15>;

protected:
  using printf_t = std::function<void(const char *)>;

public:
  struct string_t;
  
private:
  class AT_base
  {
    friend class Parser<ATsize>;
       
  protected:
    uint16_t number;

    command_t text;
    format_t format;
    size_t count;

    virtual void parse(param_list_t & params);

    AT_base(std::size_t count, Parser * parser, command_t && text, format_t && format);
  };

  using command_list_t = etl::vector<AT_base *, ATsize>;
  using prefix_t = etl::string<10>;
  using parseline_t = etl::string<128>;

protected:
  template<class ... Types>
  class AT_Command : public AT_base
  { 
    using function_t = std::function<void(Types ...)>;
    
    Parser * parser;

    int fill(int *, param_t & val);
    param_t & fill(param_t *, param_t & val);
    const char * fill(const char **, param_t & val);

    function_t func;  

    template<std::size_t ...S>
    void parse(param_list_t & params, seq<S ...>);
    void parse(param_list_t & params) override;

  public:
    AT_Command(Parser * parser, command_t && text, format_t && format);

    template<class... Args>
    void operator()(Args && ... param);

    void operator>>(function_t pFoo);
  };

private:
  command_list_t arrAT;
  prefix_t prefix;
  printf_t Write;

  void print(int param);
  void print(const char * str);
  void print(param_t & str);

protected:
  Parser(prefix_t && prefix, printf_t write);

public:
  void Parse(parseline_t && str);

  friend class AT_base;
  template<class ... Args>
  friend class AT_Command;
};

#include "ParserCPP.hpp"
#include "AT_Command.hpp"
