#pragma once

#include <iostream>
#include <cstdlib>
#include <etl/include/etl/string.h>
#include <etl/include/etl/vector.h>
#include "function.hpp"

template<int ATsize>
class Parser
{
protected:
  using printf_t = function<void(const char *)>;

public:
  struct string_t;
  
private:
  using param_list_t = etl::vector<string_t, 15>;
  using command_t = etl::string<15>;
  using format_t = etl::string<25>;

  class AT_base
  {
    friend class Parser;
       
  protected:
    uint16_t number;

    command_t text;
    format_t format;
    size_t count;

    virtual void parse(param_list_t &&) const = 0;

    AT_base(std::size_t && count, Parser * const parser, command_t && text,
      format_t && format);
  };

  using command_list_t = etl::vector<AT_base *, ATsize>;
  using prefix_t = etl::string<5>;
  using parseline_t = etl::string<128>;

protected:
  template<class ... Types>
  class AT_Command : public AT_base
  { 
    using function_t = function<void (Types ...)>;
    
    Parser * const parser;
    function_t func; 

    int convert(string_t && val, int *) const;
    string_t convert(string_t && val, string_t *) const;
    const char * convert(string_t && val, const char **) const;

    const format_t formating(format_t && format) const;

    void parse(param_list_t && param) const override;

  public:
    AT_Command(Parser * const parser, command_t && text,
      format_t && format);

    template<class... Args>
    void operator()(Args && ... param) const;

    void operator>>(function_t pFoo);
  };

private:
  command_list_t arrAT;
  prefix_t prefix;
  printf_t Write;

  void print(const int & param) const;
  void print(const char * str) const;
  void print(const string_t & str) const;
  template<class First, class Second, class ... Args>
  void print(First && first, Second && sec, Args && ... args) const;

  const AT_base * const findAT(const parseline_t &);

protected:
  Parser(prefix_t && prefix, const printf_t write);

public:
  void Parse(parseline_t && str);

  friend class AT_base;
  template<class ... Args>
  friend class AT_Command;
};

#include "ParserCPP.hpp"
#include "AT_Command.hpp"
