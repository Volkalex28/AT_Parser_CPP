#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <functional>

#include "AT_Helper.hpp"

#define RUN(FOO, PARAM) run{ (FOO(PARAM), 1)... }

class Parser
{
  class AT_base
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

protected:
  template<class... Types>
  class AT_Command : public AT_base
  {    
    Parser * parser;
    struct run;
    
    template<typename T>
    void print(T param);
    void print(const char * param);
    void print(std::string param);

    int fill(int, std::string val);
    std::string fill(std::string param, std::string val);
    const char * fill(const char * param, std::string val);

    std::string buffer = "";
    std::tuple<Types...> params;
    std::function<void(Types...)> func;

    template<int ...S>
    void parse(std::vector<std::string>& params, seq<S...>);
    void parse(std::vector<std::string>& params) override;

  public:
    AT_Command(Parser * parser, std::string text, std::string format,
      std::function<void(Types...)> func = nullptr);

    template<class... Args>
    void operator()(Args&&... param);

    void operator>>(std::function<void(Types...)> func);
  };

private:
  std::vector<AT_base *> arrAT;
  std::string prefix;
  std::function<void(const char *, size_t)> Write;

protected:
  Parser(std::string prefix, void (*write)(const char * buf, size_t size));

public:
  void Parse(std::string);
};

#include "AT_Command.hpp"
