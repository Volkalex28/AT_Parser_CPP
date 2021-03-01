/**
 * @file Parser.hpp
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Header file that describes the template class for the AT Command Parser
 * @version 0.1
 * @date 2021-02-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/**
 * @addtogroup AT_Parser AT Commands Parser
 * 
 */

#pragma once

#include <cstdlib>
#include "./etl/include/etl/string.h"
#include "./etl/include/etl/vector.h"
#include "function.hpp"

#ifndef CPPUTEST
  #define CPPUTEST
#endif

/**
 * @ingroup AT_Parser
 * @brief Universal parser of AT commands
 * 
 * A template class that can parse and process AT commands. For each AT command,
 * it is possible to declare a callback function that will be called after
 * the corresponding command is accepted.
 * 
 * @tparam ATsize Number of AT commands in the parser
 */
template<int ATsize>
class Parser
{
  CPPUTEST;
protected:
  /**
   * @brief A data type that describes the function of sending data from the parser 
   */
  using printf_t = function<void(const char *)>;

public:
  /**
   * @brief Data type that is used for strings and string parameters
   * inside the parser
   */
  using string_t = etl::string<60>;
  
private:
  /**
   * @brief 
   */
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

    virtual void parse(const param_list_t &) const = 0;

    AT_base(const std::size_t count, Parser * const parser, const char * text,
      const char * format);
    virtual ~AT_base(void);
  };

  using command_list_t = etl::vector<AT_base *, ATsize>;
  using prefix_t = etl::string<10>;

protected:
  template<class ... Types>
  class AT_Command : public AT_base
  { 
    using function_t = function<void (Types ...)>;
    
    Parser * const parser;
    function_t func; 

    const format_t formating(const format_t & format) const;

    void parse(const param_list_t & param) const override;

  public:
    AT_Command(Parser * const parser, const char * text, format_t format);
    ~AT_Command(void) override;

    template<class... Args>
    void operator()(const Args & ... param) const;

    void operator>>(const function_t & pFoo);
  };

private:
  command_list_t arrAT;
  prefix_t prefix;
  printf_t Write;

  const int convert(const string_t & val, int *) const;
  const string_t & convert(const string_t & val, string_t *) const;
  const char * convert(const string_t & val, const char **) const;

  void print(const char * val) const;
  void print(const string_t & val) const;
  void print(const int & val) const;
  template<class First, class Second, class ... Args>
  void print(const First & first, const Second & sec, const Args & ... args) const;

  const AT_base * const findAT(const char * const) const;

protected:
  Parser(const char * prefix, const printf_t & write);

public:
  void Parse(const char * str);
};

#include "Parser_AT.ipp"
#include "Parser.ipp"
