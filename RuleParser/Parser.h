// Generated by Bisonc++ V2.4.7 on Wed, 02 Dec 2009 16:21:12 +0100
//modified

#ifndef Parser_h_included
#define Parser_h_included

// $insert baseclass
#include "Parserbase.h"
#include "Lexer.h"
#include "errors.h"

namespace RuleParser{

#undef Parser
class Parser: public ParserBase
{
  private:
    DefinitionList *mydefinitions;
    DefinitionList &definitions;
    std::list< RuleParser::AbstractRule * > rules;
    std::list< std::pair< std::string, int > > classes;
      
    std::stringstream begin_stream_header;
    std::stringstream begin_stream_header2;
    std::stringstream begin_stream_source;  
    std::string classname;
    std::string _namespace;
    std::string plugin_name;
    ErrorList problems;
      
      MyLexer lexer;
      
      
      
      std::string NamespacePrefix () const
      {
	return _namespace.empty() ? "" : (_namespace + "::");
      }
      
  public:
      void CreateClass (std::string *classname, int paramcount, std::string *type);
	    
    public:
      Parser (std::istream *input_stream, DefinitionList *list = NULL)
      : lexer(input_stream, NULL), is_included(false), mydefinitions(list ? NULL : new DefinitionList ()), definitions(list ? *list : *mydefinitions)
      {
	
      }
      
      ~Parser ()
      {
	delete mydefinitions;
      }
      
      int ActualLine ()
      {
	return lexer.line();
      }
        int parse();
	/// Semantic parsing and output
	/// this function should be called after parse()
	void WriteIncludeFile (const std::string &headername2, std::ostream &stream);
	void WriteFiles (const std::string& originalfilename, const std::string& headername, const std::string& headername2, std::ostream& cppfile, std::ostream& hfile, std::ostream& hfile2);

    private:
        void error(char const *msg);    // called on (syntax) errors
        int lex()
	{
	  int result = lexer.yylex();
	  d_val__ = lexer.GetType();
	  return result; 
	}
        void print();                   // use, e.g., d_token, d_loc

    // support functions for parse():
        void executeAction(int ruleNr);
        void errorRecovery();
        int lookup(bool recovery);
        void nextToken();
	
	bool is_included;
	void IncludeTypes (const std::string &filename);
	void IncludeRules (const std::string &filename);
	
};

inline void Parser::error(char const *msg)
{
    std::cerr << msg << std::endl;
}

// $insert lex

inline void Parser::print()      // use d_token, d_loc
{}

};

#endif