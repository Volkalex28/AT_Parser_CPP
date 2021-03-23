/**
 * @file Parser.hpp
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Header file that describes the template class for the AT Command Parser
 *
 */

/**
 * @addtogroup AT_Parser AT Commands Parser
 *
 */

#pragma once

#include "./etl/include/etl/string.h"
#include "./etl/include/etl/to_string.h"
#include "./etl/include/etl/vector.h"
#include "function.hpp"
#include <cstdlib>


#ifndef CPPUTEST
#define CPPUTEST
#endif

/**
 * @ingroup AT_Parser
 * @brief Universal parser of AT commands.
 *
 * A template class that can parse and process AT commands. For each AT command,
 * it is possible to declare a callback function that will be called after
 * the corresponding command is accepted.
 *
 * @tparam ATsize Number of AT commands in the parser
 */
template <int ATsize> class Parser
{
  CPPUTEST; ///< Macro required for testing private methods
protected:
  /**
   * @brief A data type that describes the function of sending data
   * from the parser
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
   * @brief Data type for saving parsing results
   */
  using param_list_t = etl::vector<string_t, 15>;
  /**
   * @brief Data type for saving the text that will be sent when the command
   * is invoked
   */
  using command_t = etl::string<15>;
  /**
   * @brief Data type to store the data format for parsing this command
   */
  using format_t = etl::string<25>;

  /**
   * @brief Helper abstract class.
   *
   * This is the parent class for commands, which is used to preserve the text
   * and data format of the command, and to bind the command to the parser
   */
  class AT_base
  {
    friend class Parser; ///< Declaring the @link Parser @endlink class as friendly

  protected:
    command_t text; ///< The text (name) of the command that will be inserted when sending the data of this command
    /**
     * @brief Parsing data format
     *
     * Here you need to enter the text with which the command will be defined during parsing
     * @verbatim
      % - the place of the parameter to get
      # - instead of this symbol, the command text will be substituted, which will also be used for parsing
      @endverbatim
     */
    format_t format;
    size_t count; ///< The number of parameters to parse

    /**
     * @brief Parameter conversion
     *
     * Pure virtual function that converts a set of string parameters to the required data types
     * and calls the callback function
     *
     * @param[in] list list of parsed, but not converted parameters
     */
    virtual void parse(const param_list_t & list) const = 0;

    /**
     * @brief Construct a new at_base object and initialize all parameters
     *
     * @param[in] count Number of parameters to be parse
     * @param[in] parser Pointer to the parser to which the command will be attached
     * @param[in] text Command text (name)
     * @param[in] format Parsing command format
     */
    AT_base(const std::size_t count, Parser * const parser, const char * text, const char * format);
    /**
     * @brief Destroy the at base object
     */
    virtual ~AT_base(void);
  };

  /**
   * @brief Data type, an instance of which will represent a vector of pointers
   * to commands that are bound to the parser
   */
  using command_list_t = etl::vector<AT_base *, ATsize>;
  /**
   * @brief A data type that will be instantiated to represent a string of text
   * to be output before sending the command
   */
  using prefix_t = etl::string<10>;

protected:
  /**
   * @brief Command class.
   *
   * It is tailored for the answer specific to this command and is engaged
   * in converting string parameters to those specified during creation
   *
   * @tparam Types Parameter data types to which parsed data will be converted
   */
  template <class... Types> class AT_Command : public AT_base
  {
    /**
     * @brief The data type that contains the callback function
     */
    using function_t = function<void(Types...)>;

    Parser * const parser; ///< Pointer to the parser to which the command is bound
    function_t func;       ///< Callback function pointer

    /**
     * @brief Function to replace special characters in a format string
     *
     * The function substitutes instead of special characters in the command format
     * string for parsing their value or magnitude.
     * Currently supported: '#'
     *
     * @param[in] format Format string with special characters
     * @return Converted format string with special characters replaced
     */
    const format_t formating(const format_t & format) const;
    /**
     * @brief Parameter conversion
     *
     * Function that converts a set of string parameters to the required data types
     * and calls the callback function
     *
     * @param[in] list list of parsed, but not converted parameters
     */
    void parse(const param_list_t & list) const override;

  public:
    /**
     * @brief Constructing a new at_command object
     *
     * @param[in] parser Pointer to the parser to which the command will be attached
     * @param[in] text Command text (name)
     * @param[in] format Parsing command format
     */
    AT_Command(Parser * const parser, const char * text, const format_t format);
    /**
     * @brief Destroy the at_command object
     */
    ~AT_Command(void) override;

    /**
     * @brief Operator for sending a command
     *
     * The function sends data in a command-specific format.
     * Sending sequence:
     * Parser prefix, command text, passed parameters, line feed, carriage return
     *
     * @tparam Args Parameter data types that are passed in parentheses
     * @param[in] params The data to be sent.
     * It is possible to transmit an unlimited number of parameters a comma
     */
    template <class... Args> void operator()(const Args &... params) const;
    /**
     * @brief Setting the callback function
     *
     * The operator accepts a pointer to a function that will be called after
     * the command has been received and parsed. Parameters will be passed to it,
     * which are parsed according to a predefined format
     *
     * @param[in] pFoo Callback function pointer
     */
    void operator>>(const function_t & pFoo);
  };

private:
  command_list_t arrAT; ///< An array of commands that are bound to the parser
  prefix_t prefix;      ///< Parser prefix string
  printf_t Write;       ///< Pointer to a function that is used to send databy commands and the parser

  /**
   * @brief String to integer conversion function
   *
   * @param[in] val The string to be converted
   * @param[in] specifier Pointer to the data type to convert the string to.
   * Used only to select the required overload
   * @return Converted number
   */
  const int convert(const string_t & val, int * specifier) const;
  /**
   * @brief String to string conversion function
   *
   * It is necessary for the monotony of calling parameters, so as
   * not to produce unnecessary code
   *
   * @param[in] val The string to be converted
   * @param[in] specifier Pointer to the data type to convert the string to.
   * Used only to select the required overload
   * @return Converted string
   */
  const string_t & convert(const string_t & val, string_t * specifier) const;
  /**
   * @brief String to char pointer conversion function
   *
   * @param[in] val The string to be converted
   * @param[in] specifier Pointer to the data type to convert the string to.
   * Used only to select the required overload
   * @return Pointer to character array
   */
  const char * convert(const string_t & val, const char ** specifier) const;

public:
  /**
   * @brief Multiple send function
   *
   * The function is called recursively, passing @p first to the dispatch each
   * time. A new call is made with fewer parameters until they run out
   *
   * @tparam First The data type of the parameter for which the other dispatch
   * function will be called
   * @tparam Second The data type of the parameter, which is required to separate
   * this function from others
   * @tparam Args Data type package of the rest of the passed parameters
   * @param first Parameter for which the other dispatch function will be called
   * @param sec Parameter, which is required to separate this function from others
   * @param args The rest of the passed parameters
   */
  template <class First, class Second, class... Args>
  void print(const First & first, const Second & sec, const Args &... args) const;

  /**
   * @brief Send function for numeric data types
   *
   * Feature of sending floating point numbers:
   * The accuracy is set to the third decimal place. Therefore, all floating point
   * numbers will be displayed with three decimal places.
   *
   * @tparam T Numeric parameter data type
   * @param param Numeric parameter
   */
  template <class T> void print(const T & param) const;
  /**
   * @brief The function of sending an array of characters
   *
   * @param pStr Pointer to an array of characters to be sent
   */
  void print(const char * pStr) const;
  /**
   * @brief String send function
   *
   * @param rStr Link to the string to be sent
   */
  void print(const string_t & rStr) const;

private:
  /**
   * @brief Search function for the required AT command
   *
   * The function searches for a command in the array of bound commands
   * by command format
   *
   * @param pStr Pointer to the line by which the command will be determined
   * @return Pointer to the found command, or nullptr if the command was not found
   */
  const AT_base * const findAT(const char * const pStr) const;

protected:
  /**
   * @brief Constructing a new Parser object and initialization parameters
   *
   * @param prefix Pointer to parser prefix string
   * @param write Pointer to the function of sending data from the parser and commands
   */
  Parser(const char * prefix, const printf_t & write);

public:
  /**
   * @brief String parsing function
   *
   * Accepts pointer to the string (no feed and carriage return characters)
   * to be parsed
   *
   * @param str Pointer to the string to be parsed
   */
  void Parse(const char * str);
};

#include "Parser.ipp"
#include "Parser_AT.ipp"

