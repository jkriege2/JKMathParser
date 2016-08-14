/*
    Copyright (c) 2008-2016 Jan W. Krieger (<jan@jkrieger.de>)


    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*! \defgroup jkmplib_mathtools_parser parser for mathematical expressions
    \ingroup jkmplib_mathtools

 In this group there are classes that form a parser and evaluator for mathematical expressions.
 In the context of the sequencer program this is a tool class that can be used by the classes
 in the project. E.g. by jkiniparser which allows for mathematical expressions in the configuration
 files.

 */

/** \file JKMathParser.h
 *  \ingroup jkmplib_mathtools_parser
 */
#ifndef JKMATHPARSER_H
#define JKMATHPARSER_H

#include "jkmplib_imexport.h"

#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <set>

#include "jkmptools.h"
#include "jkmpbase.h"
#include "jkmpstringtools.h"

#include "extlibs/MersenneTwister.h"


/**
 * \defgroup jkmpmain main function parser class
 *  \ingroup jkmplib_mathtools_parser
 */
/*@{*/



/*! \brief A simple function parser to parse (build memory tree representation) and
           evaluate simple mathematical expressions
    \ingroup jkmplib_mathtools_parser
 This class implements a simple function parser which can parse
 mathematical expressions like <code> z=a*3+2.34^2*sin(pi*sqrt(x))</code> .
 More than one expression can be separated by semicolon ';'. The result of
 a parse operation will be a memory structure (tree) representing the given
 expression.
 The parser can cope with constants and (user defined) variables and supports
 the data types number (double precision floating point), boolean (true/false)
 and string. It already contains a lot of fundamental mathematical functions
 (i.e. nearly all the functions from C StdLib).

 \section jkmp_constantsAndVars constants and variables:
 This class provides management utilities for constants and variables. Variables
 can also be defined as external variables, when a pointer to corresponding
 memory is provided by the calling program. The parser supports a variable
 assign operation \code a=<expression>\endcode  which allows to define new
 variables during evaluation. There are some mathematical standard constants
 registered by calling JKMathParser::addStandardVariables():
   - \c pi = \f$ \pi \f$
   - \c e = \f$ \exp(1) \f$
   - \c sqrt2 = \f$ \sqrt{2} \f$
   - \c version = the parser version
   - \c log2e = \f$ \log_2(e) \f$
   - \c log10e = \f$ \log_{10}(e) \f$
   - \c ln2 = \f$ \ln(2)=\log_{e}(2) \f$
   - \c ln10 = \f$ \ln(10)=\log_{e}(10) \f$
   - \c h = \f$ 6.6260689633\cdot 10^{-34}\;\mathrm{J\cdot s} \f$ (planck constant)
   - \c hbar = \f$ \hbar=\frac{h}{2\pi}= 1.05457162853\cdot 10^{-34}\;\mathrm{J\cdot s} \f$ (planck constant)
   - \c epsilon0 = \f$ \epsilon_0= 8.854187817\cdot 10^{-12}\;\mathrm{\frac{F}{m}} \f$ (electric constant)
   - \c mu0 = \f$ \mu_0=2\pi\cdot 10^{-7}= 12.566370614\cdot 10^{-7}\;\mathrm{\frac{N}{A^2}} \f$ (magnetic constant)
   - \c c = \f$ 299792458\;\mathrm{\frac{m}{s}} \f$ (speed of light in vacuum)
   - \c ce = \f$ 1.60217648740\cdot 10^{-19}\;\mathrm{C}\f$ (elementary charge)
   - \c muB = \f$ \mu_B= 927.40091523\cdot 10^{-26}\;\mathrm{\frac{J}{T}} \f$ (Bohr magneton)
   - \c muB_eV = \f$ \mu_B=5.788381755579\cdot 10^{-5}\;\mathrm{\frac{eV}{T}}  \f$ (Bohr magneton)
   - \c muN = \f$ \mu_N=5.0507832413\cdot 10^{-27}\;\mathrm{\frac{J}{T}}  \f$ (nuclear magneton)
   - \c muN_eV = \f$ \mu_N=3.152451232645\cdot 10^{-8}\;\mathrm{\frac{eV}{T}}  \f$ (nuclear magneton)
   - \c me = \f$ m_e= 9.1093821545\cdot 10^{-31}\;\mathrm{kg} \f$ (mass of electron)
   - \c mp = \f$ m_p= 1.67262163783\cdot 10^{-27}\;\mathrm{kg} \f$ (mass of proton)
   - \c mn = \f$ m_n= 1.67492721184\cdot 10^{-27}\;\mathrm{kg} \f$ (mass of neutron)
   - \c NA = \f$ N_A= 6.0221417930\cdot 10^{23} \f$ (Avogadro constant = particles in 1 mol)
   - \c kB = \f$ k_B=1.380650424\cdot 10^{-23}\;\mathrm{\frac{J}{K}}  \f$ (Boltzman constant)
   - \c kB_eV = \f$ k_B=8.61734315\cdot 10^{-5}\;\mathrm{\frac{eV}{K}}  \f$ (Boltzman constant)
.
 You can add user-defined contants by calling JKMathParser::addVariableDouble()
 JKMathParser::addVariableBoolean() or JKMathParser::addVariableString()


 \section jkmp_functions functions:
 this class provides a wide range of (mathematical) functions:
   - sinc, gauss, slit, theta, tanc, sigmoid
   - asin, acos, atan, atan2,
   - sin, cos, tan,
   - sinh, cosh, tanh
   - log, log2, log10,
   - exp, sqr, sqrt
   - abs, sign
   - if
   - erf, erfc, lgamma, tgamma, j0, j1, jn, y0, y1, yn
   - rand, srand
   - ceil, floor, trunc, round,
   - fmod, min, max
   - floattostr, boolToJKMP::string
   - ...
 .

 these functions are registere by calling JKMathParser::addStandardFunctions().
 you can add new functions by calling JKMathParser::addFunction();

 \section jkmp_resultsofparsing result of parsing and evaluation:
 The result of calling JKMathParser::parse()
 will be a tree-like structure in memory. The parse() function will return
 a pointer to the root node of this structure. All nodes inherit from
 jkmpNode class. To evaluate such a structure simply call jkmpNode::evaluate()
 of the root node. This will then return a jkmpResult structure which contains
 the result. This scheme allows for once parsing and multiply evaluating an expression.
 So if you want to define a function by an expression you can provide an
 external variable x as the argument and then evaluate the function for
 each x.

 Evaluation can either be done with the results being returned as return-values (~5-10% slower)
 or with call-by-reference return-values.
 The third possibility is to translate the memory tree into an internal bytecode:
 \code
JKMathParser parser
parser.resetErrors();
JKMathParser::jkmpNode* n=parser.parse("sin(2*pi*(x^2+y^2))");
jkmpResult r=n->evaluate();
qDebug()<<expr<<"       =  "<<r.toTypeString()<<"\n";
JKMathParser::ByteCodeProgram bprog;
JKMathParser::ByteCodeEnvironment bcenv(&parser);
bcenv.init(&parser);
if (n->createByteCode(bprog, &bcenv)) {
    double rr=NAN;
    qDebug()<<expr<<"  =[BC]=  "<<(rr=parser.evaluateBytecode(bprog))<<"\n";
    qDebug()<<"\n-----------------------------------------------------------\n"<<JKMathParser::printBytecode(bprog)<<"\n-----------------------------------------------------------\n";
}
 \endcode
 This allows to evaluate the expression ~10-20 times fast, but not all features are supported, see \link jkmpbytecode bytecode (only number- and boolean values, limited support for
 variables, limitied support for user-defined functions (no recursion), ...).

 Here are some test-results (AMD QuadCore, 32-bit, 10000 evaluations each, gcc 4.4 no optimization, debug-build):
   - \c sqrt(a+b)+sin(b+c)+sqrt(a-c)+sin(b+a)+cos(a+c)+sqrt(b+b)+cos(a+a)+sqrt(-c)+sin(a*c)+cos(b+5.0) native: 8.7ms, return-value-evaluation: 945ms, call-by-reference: 891ms, bytecode: 40ms
   - \c cos(sqrt(a+b)+c*a) native: 1.8ms, return-value-evaluation: 186ms, call-by-reference: 177ms, bytecode: 13ms
 .
 Note that the speed-loss compared to the native execution depends on the compiler, optimization and especially the expression itself. Using a -O2 release build, the results
 of the parser  are improved ~2-fold.


 \section jkmp_ebnf EBNF definition of the parsed expressions

<pre> logical_expression ->  logical_term
                      | logical_expression <b>or</b> logical_term
                      | logical_expression <b>||</b> logical_term</pre>
<pre> logical_term    ->  comp_expression
                      | logical_term <b>and</b> comp_expression
                      | logical_term <b>&amp;&amp;</b> comp_expression</pre>
<pre> comp_expression  ->  math_expression
                      | expression <b>==</b> math_expression
                      | expression <b>!=</b> math_expression
                      | expression <b>&gt;=</b> math_expression
                      | expression <b>&lt;=</b> math_expression
                      | expression <b>&gt;</b> math_expression
                      | expression <b>&lt;</b> math_expression</pre>
<pre>  math_expression ->  term
                      | math_expression <b>+</b> math_term
                      | math_expression <b>-</b> math_term
                      | math_expression <b>|</b> math_term</pre>
<pre>  math_term       ->  primary_lop
                      | math_term <b>*</b> primary_lop
                      | math_term <b>/</b> primary_lop
                      | math_term <b>&amp;</b> primary_lop
                      | math_term ( <b>%</b> | <b>mod</b> ) primary_lop</pre>
<pre>  primary_lop     ->  vec_primary
                      | <b>+</b> vec_primary | <b>-</b> vec_primary | <b>!</b> vec_primary | <b>not</b> vec_primary | <b>~</b> vec_primary</pre>
<pre>  vec_primary     ->  primary_op
                      | primary_op <b>:</b> primary_op
                      | primary_op <b>:</b> primary_op <b>:</b> primary_op</pre>
<pre>  primary_op      ->  primary
                      | primary_op <b>^</b> primary</pre>
                      | primary_op <b>[</b> logical_expression <b>]</b>
                      | primary_op <b>.</b> NAME</pre>
<pre>  primary         ->  <b>true</b> | <b>false</b>
                      | string_constant
                      | NUMBER
                      | NAME
                      | <b>sum</b> | <b>prod</b> <b>(</b> logical_expression <b>)</b>
                      | vectorops_name <b>(</b> NAME, logical_expression, logical_expression, logical_expression, logical_expression <b>)</b>
                      | vectorops_name <b>(</b> NAME, logical_expression, logical_expression, logical_expression <b>)</b>
                      | vectorops_name <b>(</b> NAME, logical_expression, logical_expression <b>)</b>
                      | NAME <b>=</b> logical_expression
                      | NAME<b>[</b> logical_expression <b>] =</b> logical_expression
                      | NAME<b>[</b> logical_expression <b>]</b>
                      | NAME<b>(</b> parameter_list <b>)</b>
                      | NAME(parametername_list) <b>=</b> logical_expression
                      | <b>(</b> logical_expression <b>)</b></pre>

<pre>  string_constant -> <b>&quot;</b> STRING <b>&quot;</b> | <b>&apos;</b> STRING <b>&apos;</b></pre>
<pre>  parameter_list  ->  \f$ \lambda \f$ | logical_expression | logical_expression <b>,</b> parameter_list</pre>
<pre>  parametername_list  ->  \f$ \lambda \f$ | NAME | NAME <b>,</b> parametername_list</pre>
<pre>  vector_list  ->  logical_expression | logical_expression <b>,</b> vector_list</pre>
<pre>  vectorops_name  ->  <b>sum</b> | <b>prod</b> | <b>for</b> </pre>




  \section jkmp_example Simple Example of Usage
     \code
        JKMathParser mp; // instanciate
        jkmpNode* n;
        jkmpResult r;
        // parse some numeric expression
        n=mp.parse("pi^2+4*sin(65*pi/exp(3.45))");
        r=n->evaluate();
        cout<<r.num<<endl;
        //delete n;

        // parse some boolean expression
        n=mp.parse("true==false");
        if (!mp.hasErrorOccured()) {
            r=n->evaluate();
            if (r.type==jkmpBool) {
                if (r.boolean) cout<<"true";
                else cout<<"false";
            }
            if (r.type==jkmpDouble) cout<<r.num<<endl;
            if (r.type==jkmpString) cout<<r.str<<endl;
        }
        if (mp.hasErrorOccured()) {
            qDebug()<<"ERROR: "<<mp.getLastError();
        }
        if (n) delete n;

        // parse some string expression
        n=mp.parse("var1='false'; var1+'true'");
        if (!mp.hasErrorOccured()) {
            r=n->evaluate();
            if (r.type==jkmpString) cout<<r.str<<endl;
        }
        if (mp.hasErrorOccured()) {
            qDebug()<<"ERROR: "<<mp.getLastError();
        }
        if (n) delete n;
     \endcode



 */
class JKMPLIB_EXPORT JKMathParser
{
    protected:

        /** \brief the possible tokens that can be recognized by the tokenizer in JKMathParser::getToken() */
        enum jkmpTokenType {
            END,                /*!< \brief end token */
            PRINT,              /*!< \brief a semicolon ';' */
            DOT,                /*!< \brief a dot '.' */
            COMMA,              /*!< \brief a comma ',' between two function parameters */
            STRING_DELIM,       /*!< \brief a string delimiter ' or " */
            NAME,               /*!< \brief a name (consisting of characters) of a variable or function */
            NUMBER,             /*!< \brief a number in scientific notation */
            PLUS,               /*!< \brief a plus operator '+' */
            MINUS,              /*!< \brief a minus operator '-' */
            MUL,                /*!< \brief a multiplication operator '*' */
            DIV,                /*!< \brief a division operator '/' */
            MODULO,             /*!< \brief a modulo operator '%' */
            ASSIGN,             /*!< \brief a variable assignment = */
            LPARENTHESE,        /*!< \brief left parentheses '(' */
            RPARENTHESE,        /*!< \brief right parentheses ')' */
            LBRACKET,           /*!< \brief left bracket '[' */
            RBRACKET,           /*!< \brief right bracket ']' */
            LBRACE,             /*!< \brief left brace '{' */
            RBRACE,             /*!< \brief right brace '}' */
            POWER,              /*!< \brief a power operator '^' */
            COLON,              /*!< \brief a colon ':' */
            TILDE,              /*!< \brief a tilde symbol '~' */
            FACTORIAL_LOGIC_NOT, /*!< \brief a factorial operator or a logical NOT '!' */
            LOGIC_NOT,          /*!< \brief a logical NOT '!' / 'not' */
            LOGIC_AND,          /*!< \brief a logical AND operator '&&' / 'and' */
            LOGIC_OR,           /*!< \brief a logical OR operator '||' / 'or' */
            LOGIC_XOR,          /*!< \brief a logical XOR operator 'xor' */
            LOGIC_NOR,          /*!< \brief a logical NOR operator 'nor' */
            LOGIC_NAND,         /*!< \brief a logical NAND operator 'nand' */
            LOGIC_TRUE,         /*!< \brief 'true' */
            LOGIC_FALSE,        /*!< \brief  'false' */
            BINARY_AND,          /*!< \brief a binary AND operator '&' */
            BINARY_OR,           /*!< \brief a binary OR operator '|' */
            COMP_EQUALT,        /*!< \brief equals operation '==' */
            COMP_UNEQUAL,       /*!< \brief unequal operation '!=' */
            COMP_GREATER,       /*!< \brief greater than operation '>' */
            COMP_SMALLER,       /*!< \brief smaller than operation '<' */
            COMP_GEQUAL,        /*!< \brief greater than or equal operation '>=' */
            COMP_SEQUAL,        /*!< \brief smaller than or equal operation '<=' */
        };



        /** \brief internal names for logic/binary operations */
        enum {
            jkmpLOPand='a',
            jkmpLOPor='o',
            jkmpLOPxor='x',
            jkmpLOPnor='r',
            jkmpLOPnand='d',
            jkmpLOPnot='n'

        };


        /** \brief jkmpCOMPdefs internal names for compare operations */
        enum {
            jkmpCOMPequal='=',
            jkmpCOMPnequal='!',
            jkmpCOMPlesser='<',
            jkmpCOMPgreater='>',
            jkmpCOMPlesserequal='a',
            jkmpCOMPgreaterequal='b'
        };





        /*! \defgroup jkmpbytecode utilities for JKMathParser function parser class, that allow to build a bytecode program from a parse tree
            \ingroup jkmplib_mathtools_parser

            Sometimes the evaluation of the expression can be done faster, if the tree structure is translated into a bytecode run on a simple
            stack machine. To do so, the program has to be translated into that bytecode and has to meet certain conditions:
               # Only numbers (and booleans, as \c false==(number!=0)) may be used, no strings or number vectors
               # For evaluated functions, the C-function call of the form double name([double[, double[, ...]]]) should be known. If not, the parser will try to call
                 the jkmpResultFunction, but this will be very expensive, compared to calling the C-function!
               # variables may NOT hange their adress, i.e. if an external variable is defined as a pointer during compile,
                 the pointr may not change until the evaluation, as the pointer is hard-coded into the program
               # no recursion
               # functions may only be defined in the global scope
            .
            So not all expressions may be translated into ByteCode. the method jkmpNode::createByteCode() thus returns true on success
            and false else.

            \warning The bytecode utilities are NOT YET COMPLETE ... and not guaranteed to be faster than the evaluate(r) call!
         */
        /*@{*/

    public:
        /** \brief possible bytecodes */
        enum ByteCodes {
            bcNOP,
            bcPush,
            bcPop,
            bcVarRead,
            bcVarWrite,
            bcHeapRead,
            bcHeapWrite,

            bcAdd,
            bcMul,
            bcDiv,
            bcSub,
            bcMod,
            bcPow,
            bcNeg,


            bcBitAnd,
            bcBitOr,
            bcBitNot,

            bcLogicAnd,
            bcLogicOr,
            bcLogicNot,
            bcLogicXor,

            bcCmpEqual,
            bcCmpLesser,
            bcCmpLesserEqual,

            bcCallCFunction,
            bcCallCMPFunction,
            bcCallResultFunction,

            bcJumpRel,
            bcJumpCondRel,

            bcBJumpRel,
            bcBJumpCondRel

        };

        enum {
            ByteCodeInitialHeapSize=128
        };

        struct JKMPLIB_EXPORT ByteCodeInstruction {
            public:
                ByteCodeInstruction(ByteCodes opcode=bcNOP);
                ByteCodeInstruction(ByteCodes opcode, double numpar);
                ByteCodeInstruction(ByteCodes opcode, int intpar);
                ByteCodeInstruction(ByteCodes opcode, void* pntpar);
                ByteCodeInstruction(ByteCodes opcode, void* pntpar, int intpar);
                ByteCodeInstruction(ByteCodes opcode, JKMP::string strpar, int intpar);
                ByteCodes opcode;
                double numpar;
                int intpar;
                void* pntpar;
                JKMP::string strpar;
        };

        class jkmpNode; // forward

        struct JKMPLIB_EXPORT ByteCodeEnvironment {
                ByteCodeEnvironment(JKMathParser* parser=NULL);

                void init(JKMathParser* parser);

                JKMathParser* parser;
                int heapItemPointer;
                JKMP::map<JKMP::string, JKMP::vector<int> > heapVariables;
                int pushVar(const JKMP::string& name);
                void popVar(const JKMP::string& name);
                JKMP::map<JKMP::string, std::pair<JKMP::stringVector, jkmpNode*> > functionDefs;
                std::set<JKMP::string> inFunctionCalls;
        };

        friend struct ByteCodeEnvironment;

        typedef JKMP::vector<ByteCodeInstruction> ByteCodeProgram;

        double evaluateBytecode(const ByteCodeProgram &program);
        static JKMP::string printBytecode(const ByteCodeInstruction& instruction);
        static JKMP::string printBytecode(const ByteCodeProgram& program);

        /*@}*/
    public:


        /**
         * \defgroup jkmpultil utilities for JKMathParser function parser class
         * \ingroup jkmplib_mathtools_parser
         */
        /*@{*/

        static JKMP::string resultTypeToString(jkmpResultType type);


        static JKMP::string resultToString(const jkmpResult &r);
        static JKMP::string resultToTypeString(const jkmpResult &r);

        static jkmpResult getInvalidResult();


        /** \brief This struct is for managing variables. Unlike jkmpResult this struct
          * only contains pointers to the data
          */
        struct JKMPLIB_EXPORT jkmpVariable {
                friend struct ByteCodeEnvironment;
            public:
                jkmpVariable();
                jkmpVariable(double* ref);
                jkmpVariable(JKMP::string* ref);
                jkmpVariable(bool* ref);
                jkmpVariable(JKMP::vector<double>* ref);
                jkmpVariable(JKMP::vector<bool>* ref);
                jkmpVariable(JKMP::vector<double>* ref, int* columnref);
                jkmpVariable(JKMP::vector<bool>* ref, int* columnref);
                jkmpVariable(JKMP::stringVector* ref);
                jkmpVariable(JKMP::map<JKMP::string,jkmpResult>* ref);
                jkmpVariable(JKMP::vector<jkmpResult>* ref);
                ~jkmpVariable();
                JKMPLIB_EXPORT void clearMemory();
                JKMPLIB_EXPORT jkmpResult toResult() const;
                JKMPLIB_EXPORT void toResult(jkmpResult& r) const;
                JKMPLIB_EXPORT bool isInternal() const;
                JKMPLIB_EXPORT void set(const jkmpResult& result);
                inline  jkmpResultType getType() const { return type; }
                inline JKMP::string* getStr() const { return str; }
                inline double* getNum() const { return num; }
                inline bool* getBoolean() const { return boolean; }
                inline JKMP::vector<double>* getNumVec() const { return numVec; }
                inline JKMP::vector<bool>* getBoolVec() const { return boolVec; }
                inline JKMP::stringVector* getStrVec() const { return strVec; }
                inline JKMP::map<JKMP::string,jkmpResult>* getStructData() const { return structData; }
                inline JKMP::vector<jkmpResult>* getListData() const { return listData; }
                inline int* getColumns() const { return matrix_columns; }


            protected:
                jkmpResultType type;     /*!< \brief type of the variable */
                bool internal;           /*!< \brief this is an internal variable */
                JKMP::string *str;        /*!< \brief this points to the variable data if \c type==jkmpString */
                double *num;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
                bool *boolean;           /*!< \brief this points to the variable data if \c type==jkmpBool */
                JKMP::vector<double>* numVec; /*!< \brief this points to the variable data if \c type==jkmpDoubleVector */
                JKMP::vector<bool>* boolVec; /*!< \brief this points to the variable data if \c type==jkmpBoolVector */
                JKMP::stringVector* strVec; /*!< \brief this points to the variable data if \c type==jkmpStringVector */
                JKMP::map<JKMP::string,jkmpResult>* structData;
                JKMP::vector<jkmpResult>* listData;
                int* matrix_columns;
        };





        /*@}*/


        /**
         * \defgroup jkmpNodes memory representation of expressions
         * \ingroup jkmplib_mathtools_parser
         */
        /*@{*/

        /**
         * \brief This class is the abstract base class for nodes.
         * All allowed node types must inherit from jkmpNode
         */
        class JKMPLIB_EXPORT jkmpNode {
          protected:
            JKMathParser* parser;  /*!< \brief points to the parser object that is used to evaluate this node */
            jkmpNode* parent;      /*!< \brief points to the parent node */
          public:
            explicit jkmpNode(JKMathParser* parser, jkmpNode* parent=NULL) { this->parser=parser;  this->parent=parent; }
            /** \brief virtual class destructor */
            virtual ~jkmpNode() {}

            /** \brief evaluate this node */
            virtual jkmpResult evaluate();

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result)=0;
            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(JKMP::vector<jkmpResult>& result);

            /** \brief return a pointer to the JKMathParser  */
            inline JKMathParser* getParser(){ return parser; }

            /** \brief set the JKMathParser  */
            inline void setParser(JKMathParser* mp){ parser=mp; }

            /** \brief returns a pointer to the parent node */
            inline jkmpNode* getParent(){ return parent; }

            /** \brief sets the parent node  */
            inline void setParent(jkmpNode* par) { parent=par; }

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) =0;

            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& /*program*/, ByteCodeEnvironment* /*environment*/) { ;return false; }

            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;
        };


        /**
         * \brief This class represents a binary arithmetic operation:
         *  add (+), subtract (-), multiply (*), divide (/), a to the power of b (a^b),
         *  binary and (&), binary or (|)
         */
        class JKMPLIB_EXPORT jkmpBinaryArithmeticNode: public jkmpNode {
          private:
            jkmpNode* left, *right;
            char operation;
          public:
            /** \brief constructor for a jkmpBinaryArithmeticNode
             *  \param op the operation to be performed: add (+), subtract (-), multiply (*), divide (/), a to the power of b (a^b)
             *  \param l left child node/operand
             *  \param r right child node/operand
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpBinaryArithmeticNode(char op, jkmpNode* l, jkmpNode* r, JKMathParser* p, jkmpNode* par);

            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpBinaryArithmeticNode();

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment);

            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;

        };

        /**
         * \brief This class represents a binary boolean operation: and, or, xor, nor, nand
         */
        class JKMPLIB_EXPORT jkmpBinaryBoolNode: public jkmpNode {
          private:
            jkmpNode* left, *right;
            char operation;
          public:
            /** \brief constructor for a jkmpBinaryBoolNode
             *  \param op the operation to be performed: (a)nd, (o)r, (x)or, (n)or, nand (A)
             *  \param l left child node/operand
             *  \param r right child node/operand
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpBinaryBoolNode(char op, jkmpNode* l, jkmpNode* r, JKMathParser* p, jkmpNode* par);

            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpBinaryBoolNode();

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment);
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;
            JKMP::string opAsString() const;

        };

        /**
         * \brief This class represents a binary compare operation: !=, ==, >=, <=, >, <
         */
        class JKMPLIB_EXPORT jkmpCompareNode: public jkmpNode {
          private:
            jkmpNode* left, *right;
            char operation;
          public:
            /** \brief constructor for a jkmpCompareNode
             *  \param op the operation to be performed: != (!), == (=), >= (b), <= (a), (>), (<)
             *  \param l left child node/operand
             *  \param r right child node/operand
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpCompareNode(char op, jkmpNode* l, jkmpNode* r, JKMathParser* p, jkmpNode* par);

            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpCompareNode ();

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment);
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;

            JKMP::string opAsString() const;

        };

        /**
         * \brief This class represents a unary operations: ! (bool negation), - (arithmetic negation)
         */
        class JKMPLIB_EXPORT jkmpUnaryNode: public jkmpNode {
          private:
            jkmpNode* child;
            char operation;
          public:
            /** \brief constructor for a jkmpUnaryNode
             *  \param op the operation to be performed: (!), (-)
             *  \param c child node/operand
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpUnaryNode(char op, jkmpNode* c, JKMathParser* p, jkmpNode* par);

            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpUnaryNode();

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL);
            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment);
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;

        };

        /**
         * \brief This class represents a vector element access to a variable
         */
        class JKMPLIB_EXPORT jkmpVariableVectorAccessNode: public jkmpNode {
          protected:
            jkmpNode* index;
            JKMP::string variable;
          public:
            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpVariableVectorAccessNode() ;

            /** \brief constructor for a jkmpVariableAssignNode
             *  \param var name of the variable to assign to
             *  \param index index child node
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpVariableVectorAccessNode(JKMP::string var, jkmpNode* index, JKMathParser* p, jkmpNode* par);

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;

        };


        /**
         * \brief This class represents a vector element access to an arbitrary expression
         */
        class JKMPLIB_EXPORT jkmpVectorAccessNode: public jkmpNode {
          protected:
            jkmpNode* index;
            jkmpNode* left;
          public:
            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpVectorAccessNode() ;

            /** \brief constructor for a jkmpVariableAssignNode
             *  \param left left-hand-side expression
             *  \param index index child node
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpVectorAccessNode(jkmpNode* left, jkmpNode* index, JKMathParser* p, jkmpNode* par);

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;

        };
        /**
         * \brief This class represents a struct element access to an arbitrary expression
         */
        class JKMPLIB_EXPORT jkmpStructAccessNode: public jkmpNode {
          protected:
            JKMP::string index;
            jkmpNode* left;
          public:
            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpStructAccessNode() ;

            /** \brief constructor for a jkmpVariableAssignNode
             *  \param left left-hand-side expression
             *  \param index index child node
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpStructAccessNode(jkmpNode* left, const JKMP::string& index, JKMathParser* p, jkmpNode* par);

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;

        };

        /**
         * \brief This class represents a variable assignment (a = expression)
         */
        class JKMPLIB_EXPORT jkmpVariableAssignNode: public jkmpNode {
          protected:
            jkmpNode* child;
            JKMP::string variable;
          public:
            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpVariableAssignNode() ;

            /** \brief constructor for a jkmpVariableAssignNode
             *  \param var name of the variable to assign to
             *  \param c child node/right-hand-side expression
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpVariableAssignNode(JKMP::string var, jkmpNode* c, JKMathParser* p, jkmpNode* par);

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment);
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;


        };

        /**
         * \brief This class represents a vector element assignment (a[index] = expression)
         */
        class JKMPLIB_EXPORT jkmpVectorElementAssignNode: public jkmpVariableAssignNode {
          protected:
            jkmpNode* index;
          public:
            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpVectorElementAssignNode() ;

            /** \brief constructor for a jkmpVectorElementAssignNode
             *  \param var name of the variable to assign to
             *  \param index index node
             *  \param expression child node/right-hand-side expression
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpVectorElementAssignNode(JKMP::string var, jkmpNode* index, jkmpNode* expression, JKMathParser* p, jkmpNode* par);

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;

        };


        /**
         * \brief This class represents a variable assignment (a = expression)
         */
        class JKMPLIB_EXPORT jkmpFunctionAssignNode: public jkmpNode {
          private:
            jkmpNode* child;
            JKMP::string function;
            JKMP::stringVector parameterNames;
          public:
            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpFunctionAssignNode() ;

            /** \brief constructor for a jkmpVariableAssignNode
             *  \param function name of the function to assign to
             *  \param parameterNames list of parameters of the new function
             *  \param c child node/right-hand-side expression
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpFunctionAssignNode(JKMP::string function, JKMP::stringVector parameterNames, jkmpNode* c, JKMathParser* p, jkmpNode* par);

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment);
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;

        };

        /**
         * \brief This class represents a number, a string contant or a boolean contant (true/false)
         */
        class JKMPLIB_EXPORT jkmpConstantNode: public jkmpNode {
          private:
            jkmpResult data;
          public:
            /** \brief constructor for a jkmpConstantNode
             *  \param d the value of the constant
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpConstantNode(jkmpResult d, JKMathParser* p, jkmpNode* par):jkmpNode(p, par) { data=d; }

            /** \brief evaluate this node */
            virtual jkmpResult evaluate() ;
            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment);
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;

        };

        /**
         * \brief This class represents a variable.
         */
        class JKMPLIB_EXPORT jkmpVariableNode: public jkmpNode {
          private:
            JKMP::string var;
          public:
            /** \brief constructor for a jkmpVariableNode
             *  \param name name of the variable
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpVariableNode(JKMP::string name, JKMathParser* p, jkmpNode* par);

            /** \brief evaluate this node */
            virtual jkmpResult evaluate();
            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            inline JKMP::string getName() const { return var; }

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment *environment);
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;

        };


        /**
         * \brief This class represents a an invalid node that always resturns an invalid value
         */
        class JKMPLIB_EXPORT jkmpInvalidNode: public jkmpNode {
          public:
            /** \brief constructor for a jkmpConstantNode
             *  \param d the value of the constant
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpInvalidNode(JKMathParser* p, jkmpNode* par):jkmpNode(p, par) { };

            /** \brief evaluate this node */
            virtual jkmpResult evaluate();
            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
                /** \brief print the expression */
                virtual JKMP::string print() const;
                /** \brief print the expression tree */
                virtual JKMP::string printTree(int level=0) const;

        };




        /** \brief This is a function prototype for adding new mathematical functions
         *         to the parser
         *
         * If you want to add more math functions (like sin, cos , abs ...) to the
         * parser, you will have to implement it with this prototype and then register
         * it with JKMathParser::addFunction(). The first parameter points to an array
         * containing the input parameters while the second one specifies the number
         * of supplied parameters. The result has to be of type jkmpResult.
         *
         * All error handling has to be done inside the function definition. Here is a
         * simple example:
         * \code
         * jkmpResult Abs(jkmpResult* params, unsigned char n){
         *   jkmpResult r;
         *   r.type=jkmpDouble;
         *   if (n!=1) jkmpError("abs accepts 1 argument");
         *   if (params[0].type!=jkmpDouble) jkmpError("abs needs double argument");
         *   r.num=fabs(params[0].num);
         *   return r;
         * }
         * \endcode
         */
        typedef jkmpResult (*jkmpEvaluateFunc)(const jkmpResult*, unsigned int, JKMathParser*);
        typedef double (*jkmpEvaluateFuncSimple0Param)();
        typedef double (*jkmpEvaluateFuncSimple1Param)(double);
        typedef double (*jkmpEvaluateFuncSimple2Param)(double,double);
        typedef double (*jkmpEvaluateFuncSimple3Param)(double,double,double);
        typedef double (*jkmpEvaluateFuncSimple0ParamMP)(JKMathParser*);
        typedef double (*jkmpEvaluateFuncSimple1ParamMP)(double, JKMathParser*);
        typedef double (*jkmpEvaluateFuncSimple2ParamMP)(double,double, JKMathParser*);
        typedef double (*jkmpEvaluateFuncSimple3ParamMP)(double,double,double, JKMathParser*);

        /** \brief This is a function prototype like jkmpEvaluateFunc but returns its result with call by reference
         */
        typedef void (*jkmpEvaluateFuncRefReturn)(jkmpResult&, const jkmpResult*, unsigned int, JKMathParser*);

        /** \brief This is a function prototype like jkmpEvaluateFunc but returns its result with call by reference. the parameters are not evaluated jkmpResults, but pointers to jkmpNode
         */
        typedef void (*jkmpEvaluateFromNodesFuncRefReturn)(jkmpResult&, jkmpNode**, unsigned int, JKMathParser*);

        /** \brief types of functions */
        enum jkmpFunctiontype {
            functionC,
            functionCRefReturn,
            functionNode,
            functionFromNode,
            functionInvalid
        };

        /** \brief description of a user registered function */
        struct JKMPLIB_EXPORT jkmpFunctionDescriptor {
            explicit jkmpFunctionDescriptor();
            //virtual ~jkmpFunctionDescriptor();
            JKMPLIB_EXPORT void clearMemory();
            jkmpEvaluateFunc function;    /*!< \brief a pointer to the function implementation */
            jkmpEvaluateFuncRefReturn functionRR;    /*!< \brief a pointer to the function implementation */
            jkmpEvaluateFromNodesFuncRefReturn functionFN;    /*!< \brief a pointer to the function implementation */
            JKMP::string name;             /*!< \brief name of the function */
            jkmpFunctiontype type;  /*!< \brief type of the function */
            jkmpNode* functionNode;   /*!< \brief points to the node definig the function */
            JKMP::stringVector parameterNames;  /*!< \brief a list of the function parameters, if the function is defined by a node */

            JKMP::map<int, void*> simpleFuncPointer; /*!<  \brief points to the simple implementation of the function, e.g. of type jkmpEvaluateFuncSimple0Param or jkmpEvaluateFuncSimple0ParamMP, the integer-key indexes the function as its number of parameters for a simple call to simpleFuncPointer ... values >100 indicate the use of a MP-variant, i.e. 102 means a call to jkmpEvaluateFuncSimple2ParamMP whereas 1 means a call to  jkmpEvaluateFuncSimple1Param */

            JKMPLIB_EXPORT void evaluate(jkmpResult& res, const JKMP::vector<jkmpResult> &parameters, JKMathParser *parent) const;
            JKMPLIB_EXPORT void evaluate(jkmpResult& res, JKMP::vector<jkmpNode *> parameters, JKMathParser *parent) const;
            JKMPLIB_EXPORT JKMP::string toDefString() const;
        };

        /**
         * \brief This class represents an arbitrary function.
         *
         * When initialized this class will get the function description that is
         * linked to the supplied function name from JKMathParser object. This
         * information is saved locally and won't be changed when evaluating!
         *
         * Functions may have 8 parameters at the most.
         */
        class JKMPLIB_EXPORT jkmpFunctionNode: public jkmpNode {
          private:
            JKMP::string fun;
            JKMP::vector<JKMathParser::jkmpNode*> child;
          public:
            /** \brief constructor for a jkmpFunctionNode
             *  \param name name of the function
             *  \param c a pointer to an array of jkmpNode objects that represent the parameter expressions
             *  \param num number of children in c
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpFunctionNode(JKMP::string name, JKMP::vector<JKMathParser::jkmpNode*> params, JKMathParser* p, jkmpNode* par);

            /** \brief standard destructor, also destroy the children (recursively) */
            virtual ~jkmpFunctionNode();

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);


            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;


            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment) ;
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;
        };

        /**
         * \brief This class represents a list of jkmpNode.
         *
         * when evaluating the result will be the result of the last node in the list.
         */
        class JKMPLIB_EXPORT jkmpNodeList: public jkmpNode {
          protected:
            JKMP::vector<jkmpNode*> list;
          public:
            /** \brief constructor for a jkmpNodeList
             *  \param p a pointer to a JKMathParser object
             */
            explicit jkmpNodeList(JKMathParser* p, jkmpNode* par=NULL);

            /** \brief standard destructor, also destroy the children (recursively) */
            virtual ~jkmpNodeList();

            /** \brief add a jkmpNode n to the list */
            void add(jkmpNode* n);
            /** \brief remove the first node from the list and only delet it if \a deleteObject is \c true, returns a pointer to the first node, if \a deleteObject is \c false otherwise returns \c NULL */
            jkmpNode* popFirst(bool deleteObject=false);
            /** \brief remove the first node from the list and only delet it if \a deleteObject is \c true, returns a pointer to the first node, if \a deleteObject is \c false otherwise returns \c NULL */
            jkmpNode* popLast(bool deleteObject=false);

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);
            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(JKMP::vector<jkmpResult>& result);

            /** \brief get the number of nodes in the list */
            int getCount() {return list.size();};

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment) ;
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;
        };

        /**
         * \brief This class represents a vector of jkmpNode.
         *
         * when evaluating the result will be the result of the last node in the list.
         */
        class JKMPLIB_EXPORT jkmpVectorMatrixConstructionList: public jkmpNodeList {
            public:
                /** \brief constructor for a jkmpNodeList
                 *  \param p a pointer to a JKMathParser object
                 */
                explicit jkmpVectorMatrixConstructionList(JKMathParser* p, jkmpNode* par=NULL): jkmpNodeList(p, par) { m_isMatrix=false; setParser(p); setParent(par); }

                /** \brief standard destructor, also destroy the children (recursively) */
                virtual ~jkmpVectorMatrixConstructionList() {}


                /** \brief evaluate this node, return result as call-by-reference (faster!) */
                virtual void evaluate(jkmpResult& result);

                /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
                virtual jkmpNode* copy(jkmpNode* par=NULL) ;
                /** \brief create bytecode that evaluates the current node */
                virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment) ;
                /** \brief print the expression */
                virtual JKMP::string print() const;
                /** \brief print the expression tree */
                virtual JKMP::string printTree(int level=0) const;

                inline bool isMatrix() const {
                    return m_isMatrix;
                }
                inline void setIsMatrix(bool en) {
                    m_isMatrix=en;
                }

            protected:
                bool m_isMatrix;
        };

        /**
         * \brief This class represents a list of jkmpNode.
         *
         * when evaluating the result will be the result of the last node in the list.
         */
        class JKMPLIB_EXPORT jkmpListConstruction: public jkmpNodeList {
          public:
            /** \brief constructor for a jkmpNodeList
             *  \param p a pointer to a JKMathParser object
             */
            explicit jkmpListConstruction(JKMathParser* p, jkmpNode* par=NULL): jkmpNodeList(p, par) { setParser(p); setParent(par); };

            /** \brief standard destructor, also destroy the children (recursively) */
            virtual ~jkmpListConstruction() {};


            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment) ;
                /** \brief print the expression */
                virtual JKMP::string print() const;
                /** \brief print the expression tree */
                virtual JKMP::string printTree(int level=0) const;
        };

        /**
         * \brief This class represents a vector construction in the form start:end or start:step:end
         */
        class JKMPLIB_EXPORT jkmpVectorConstructionNode: public jkmpNode {
          private:
            jkmpNode* start, *end, *step;
          public:
            /** \brief constructor for a jkmpCompareNode
             *  \param start start child node/operand
             *  \param end end child node/operand
             *  \param step step child node/operand
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpVectorConstructionNode(jkmpNode* start, jkmpNode* end, jkmpNode* step, JKMathParser* p, jkmpNode* par);

            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpVectorConstructionNode ();

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;
        };

        /**
         * \brief This class represents a cases or if construction
         */
        class JKMPLIB_EXPORT jkmpCasesNode: public jkmpNode {
          private:
            jkmpNode* elseNode;
            JKMP::vector<std::pair<jkmpNode*, jkmpNode*> > casesNodes;
          public:
            /** \brief constructor for a jkmpCompareNode
             *  \param start start child node/operand
             *  \param end end child node/operand
             *  \param step step child node/operand
             *  \param p a pointer to a JKMathParser object
             *  \param par a pointer to the parent node
             */
            explicit jkmpCasesNode(JKMathParser* p, jkmpNode* par);

            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpCasesNode ();

            void setElse(jkmpNode* elseNode);
            void addCase(jkmpNode* decision, jkmpNode* value);

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment) ;

            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;
        };

        /**
         * \brief This class represents a for, savefor, sum, prod construct
         */
        class JKMPLIB_EXPORT jkmpVectorOperationNode: public jkmpNode {
          private:
            JKMP::string operationName;
            JKMP::string variableName;
            jkmpNode* items;
            jkmpNode* start;
            jkmpNode* end;
            jkmpNode* delta;
            jkmpNode* expression;
            jkmpNode* defaultValue;
            JKMP::vector<std::pair<jkmpNode*, jkmpNode*> > casesNodes;
          public:
            /** \brief constructor for a jkmpVectorOperationNode
             */
            explicit jkmpVectorOperationNode(const JKMP::string& operationName, const JKMP::string& variableName, jkmpNode* items, jkmpNode* expression, JKMathParser* p, jkmpNode* par, jkmpNode* defaultValue=NULL);
            explicit jkmpVectorOperationNode(const JKMP::string& operationName, const JKMP::string& variableName, jkmpNode* start, jkmpNode* end, jkmpNode* delta, jkmpNode* expression, JKMathParser* p, jkmpNode* par, jkmpNode* defaultValue=NULL);

            /** \brief standard destructor, also destroy the children (recursively)  */
            virtual ~jkmpVectorOperationNode ();

            /** \brief evaluate this node, return result as call-by-reference (faster!) */
            virtual void evaluate(jkmpResult& result);
            /** \brief create bytecode that evaluates the current node */
            virtual bool createByteCode(ByteCodeProgram& program, ByteCodeEnvironment* environment) ;

            /** \brief returns a copy of the current node (and the subtree). The parent is set to \a par */
            virtual jkmpNode* copy(jkmpNode* par=NULL) ;
            /** \brief print the expression */
            virtual JKMP::string print() const;
            /** \brief print the expression tree */
            virtual JKMP::string printTree(int level=0) const;
        };

        /*@}*/



        class JKMPLIB_EXPORT executionEnvironment {
            protected:
                /** \brief map to manage all currently defined variables */
                JKMP::map<JKMP::string, JKMP::vector<std::pair<int, jkmpVariable> > > variables;

                /** \brief map to manage all currently rtegistered functions */
                JKMP::map<JKMP::string, JKMP::vector<std::pair<int, jkmpFunctionDescriptor> > > functions;

                int currentLevel;

                JKMathParser* parent;
            public:
                executionEnvironment(JKMathParser* parent=NULL);
                ~executionEnvironment();

                JKMPLIB_EXPORT void setParent(JKMathParser* parent);

                inline int getBlockLevel() const  {
                    return currentLevel;
                }
                inline void enterBlock() {
                    currentLevel++;
                }
                inline void leaveBlock(){
                    if (currentLevel>0) {
                        currentLevel--;
                        JKMP::stringVector keys=variables.keys();
                        for (size_t i=0; i<keys.size(); i++) {
                            while (!(variables[keys[i]].is_empty())&&variables[keys[i]].back().first>currentLevel) {
                                variables[keys[i]].back().second.clearMemory();
                                variables[keys[i]].pop_back
();
                            }
                            //if (keys[i]=="x") //qDebug()<<"**LEAVE_BLOCK "<<currentLevel+1<<": var:"<<keys[i]<<"   levels="<<variables[keys[i]].size();
                            if (variables[keys[i]].is_empty()) variables.erase(keys[i]);
                        }

                        keys=functions.keys();
                        for (size_t i=0; i<keys.size(); i++) {
                            while ((!functions[keys[i]].is_empty()) && functions[keys[i]].back().first>currentLevel) {
                                functions[keys[i]].back().second.clearMemory();
                                functions[keys[i]].pop_back
();
                            }
                            if (functions[keys[i]].is_empty()) functions.erase(keys[i]);
                        }
                    } else {
                        parent->jkmpError(JKMP::_("cannot leave toplevel block!"));
                    }
                 }

                JKMPLIB_EXPORT int getBlocklevel() const;
                JKMPLIB_EXPORT void clearVariables();
                JKMPLIB_EXPORT void clearFunctions();
                JKMPLIB_EXPORT void clear();

                JKMPLIB_EXPORT void addVariable(const JKMP::string& name, const jkmpVariable& variable);
                JKMPLIB_EXPORT int getVariableLevels(const JKMP::string& name) const;
                JKMPLIB_EXPORT void setFunction(const JKMP::string& name, const jkmpFunctionDescriptor& function);
                JKMPLIB_EXPORT void addFunction(const JKMP::string& name, const JKMP::stringVector& parameterNames, jkmpNode* function);

                /** \brief  tests whether a variable exists */
                inline bool variableExists(const JKMP::string& name){ return (variables.find(name)!=variables.end()); }

                /** \brief  tests whether a function exists */
                inline bool functionExists(const JKMP::string& name){ return !(functions.find(name)==functions.end()); }

                inline jkmpResult getVariable(const JKMP::string& name) const {
                    jkmpResult res;
                    res.isValid=false;
                    if (variables.contains(name) && variables[name].size()>0) {
                        res=variables[name].back().second.toResult();
                        res.isValid=true;
                        //qDebug()<<"getVariable("<<name<<"): "<<res.toTypeString();
                        return res;
                    }
                    if (parent) parent->jkmpError(JKMP::_("the variable '%1' does not exist").arg(name));
                    return res;
                }
                inline bool getVariableDef(const JKMP::string& name, jkmpVariable& vardef) const {
                    if (variables.contains(name) && variables[name].size()>0) {
                        vardef=variables[name].back().second;
                        return true;
                    }
                    if (parent) parent->jkmpError(JKMP::_("the variable '%1' does not exist").arg(name));
                    return false;
                }
                inline int getVariableLevel(const JKMP::string& name) const {
                    if (variables.contains(name) && variables[name].size()>0) {
                        return variables[name].back().first;
                    }
                    if (parent) parent->jkmpError(JKMP::_("the variable '%1' does not exist").arg(name));
                    return -1;
                }

                inline jkmpResult evaluateFunction(const JKMP::string& name, const JKMP::vector<jkmpResult> &parameters) const{
                    jkmpResult res;
                    if (functions.contains(name) && functions[name].size()>0) {
                        functions[name].back().second.evaluate(res, parameters, parent);
                    } else {
                        if (parent) parent->jkmpError(JKMP::_("the function '%1' does not exist").arg(name));
                        res.setInvalid();
                    }
                    return res;
                }

                inline void evaluateFunction(jkmpResult& res, const JKMP::string& name, const JKMP::vector<jkmpResult> &parameters) const{
                    if (functions.contains(name) && functions[name].size()>0) {
                        functions[name].back().second.evaluate(res, parameters, parent);
                    } else {
                        if (parent) parent->jkmpError(JKMP::_("the function '%1' does not exist").arg(name));
                        res.setInvalid();
                    }
                }

                inline jkmpResult evaluateFunction(const JKMP::string& name, JKMP::vector<jkmpNode*> parameters) const{
                    jkmpResult res;
                    if (functions.contains(name) && functions[name].size()>0) {
                        functions[name].back().second.evaluate(res, parameters, parent);
                    } else {
                        if (parent) parent->jkmpError(JKMP::_("the function '%1' does not exist").arg(name));
                        res.setInvalid();
                    }
                    return res;
                }

                inline void evaluateFunction(jkmpResult& res, const JKMP::string& name, JKMP::vector<jkmpNode*> parameters) const{
                    if (functions.contains(name) && functions[name].size()>0) {
                        functions[name].back().second.evaluate(res, parameters, parent);
                    } else {
                        if (parent) parent->jkmpError(JKMP::_("the function '%1' does not exist").arg(name));
                        res.setInvalid();
                    }
                }

                inline bool getFunctionDef(const JKMP::string& name, jkmpFunctionDescriptor& vardef) const {
                    if (functions.contains(name) && functions[name].size()>0) {
                        vardef=functions[name].back().second;
                        return true;
                    }
                    if (parent) parent->jkmpError(JKMP::_("the function '%1' does not exist").arg(name));
                    return false;
                }

                inline jkmpFunctiontype getFunctionType(const JKMP::string& name) const {
                    if (functions.contains(name) && functions[name].size()>0) {
                         return functions[name].back().second.type;
                    }
                    if (parent) parent->jkmpError(JKMP::_("the function '%1' does not exist").arg(name));
                    return functionInvalid;
                }

                inline int getFunctionLevel(const JKMP::string& name) const {
                    if (functions.contains(name) && functions[name].size()>0) {
                        return functions[name].back().first;
                    }
                    if (parent) parent->jkmpError(JKMP::_("the function '%1' does not exist").arg(name));
                    return -1;
                }

                inline void getVariable(jkmpResult& res, const JKMP::string& name) const{
                    if (variables.contains(name) && variables[name].size()>0) {
                        variables[name].back().second.toResult(res);
                    } else {
                        if (parent) parent->jkmpError(JKMP::_("the variable '%1' does not exist").arg(name));
                        res.setInvalid();
                    }
                    //qDebug()<<"getVariable(res, "<<name<<", "<<res.toTypeString()<<")";
                }

                inline void addVariable(const JKMP::string& name, const jkmpResult& result){
                    bool add=true;
                    if (variables.contains(name)) {
                        if (variables[name].size()>0) {
                            //qDebug()<<"addVar("<<name<<") compare level:  level="<<variables[name].back().first<<"  currentLevel="<<currentLevel;
                            if (variables[name].back().first==currentLevel) {
                                variables[name].back().second.set(result);
                                add=false;
                                return;
                            }
                        }
                    }
                    if (add) {
                        JKMathParser::jkmpVariable v;
                        v.set(result);

                        if (variables.contains(name)) {
                            variables[name].push_back(std::make_pair(currentLevel, v));
                        } else {
                            JKMP::vector<std::pair<int, jkmpVariable> > l;
                            l.push_back(std::make_pair(currentLevel, v));
                            variables.insert(std::make_pair(name, l));
                        }

                        //qDebug()<<"addVar("<<name<<") add new:  level="<<currentLevel<<"  val="<<v.toResult().toTypeString();

                    }

                }

                inline void setVariable(const JKMP::string& name, const jkmpResult& result){
                    //qDebug()<<"setVariable("<<name<<", "<<result.toTypeString()<<")";
                    if (variables.contains(name) && variables[name].size()>0) {
                        variables[name].back().second.set(result);
                    } else {
                        JKMathParser::jkmpVariable v;
                        v.set(result);
                        JKMP::vector<std::pair<int, jkmpVariable> > l;
                        l.push_back(std::make_pair(currentLevel, v));
                        variables[name]=l;
                    }
                    //qDebug()<<"setVariable("<<name<<", "<<result.toTypeString()<<")  =>  "<<variables[name].back().second.toResult().toTypeString();
                }

                JKMPLIB_EXPORT void setVariableDouble(const JKMP::string& name, double result);
                JKMPLIB_EXPORT void setVariableDoubleVec(const JKMP::string& name, const JKMP::vector<double>& result);
                JKMPLIB_EXPORT void setVariableStringVec(const JKMP::string& name, const JKMP::stringVector& result);
                JKMPLIB_EXPORT void setVariableBoolVec(const JKMP::string& name, const JKMP::vector<bool>& result);
                JKMPLIB_EXPORT void setVariableString(const JKMP::string& name, const JKMP::string& result);
                JKMPLIB_EXPORT void setVariableBoolean(const JKMP::string& name, bool result);
                JKMPLIB_EXPORT void addVariableDouble(const JKMP::string& name, double result);
                JKMPLIB_EXPORT void addVariableDoubleVec(const JKMP::string& name, const JKMP::vector<double>& result);
                JKMPLIB_EXPORT void addVariableStringVec(const JKMP::string& name, const JKMP::stringVector& result);
                JKMPLIB_EXPORT void addVariableBoolVec(const JKMP::string& name, const JKMP::vector<bool>& result);
                JKMPLIB_EXPORT void addVariableString(const JKMP::string& name, const JKMP::string& result);
                JKMPLIB_EXPORT void addVariableBoolean(const JKMP::string& name, bool result);
                JKMPLIB_EXPORT void deleteVariable(const JKMP::string& name);

                JKMP::string printVariables() const;
                JKMP::string printFunctions() const;
                JKMP::vector<std::pair<JKMP::string, jkmpVariable> > getVariables() const;
                JKMP::vector<std::pair<JKMP::string, jkmpFunctionDescriptor> > getFunctions() const;


        };

    public:
        /**
         * \defgroup jkmpErrorhandling error handling
         * \ingroup jkmplib_mathtools_parser
         */
        /*@{*/






        /** \brief macro that throws an exception or calls an error handler */
        void jkmpError(JKMP::string st);


        /*@}*/

    protected:
        /** \brief return the given token as human-readable string */
        JKMP::string tokentostring(jkmpTokenType token);

        /** \brief return the current token as human-readable string */
        JKMP::string currenttokentostring();

		/** \brief Tokenizer: extract the next token from the input */
        jkmpTokenType getToken();

        /** \brief eat multi-line comment from input */
        void eatMultilineComment();
        /** \brief eat single-line comment from input */
        void eatSinglelineComment();

		/** \brief return a delimited text, i.e. extract the texte between the delimiters <code>"</code> in: of <code>"Hallo!"</code>, i.e. returns <code> Hallo!</code>
         *         This is used to parse string constants.
         *
         * This functions actually reads pascal style delimited string constants. So if you want to use the delimiter as part of the string you will have to
         * write it as doubled character. So <code>'Jan''s Test'</code> stands for <code>Jan's Test</code>.
         */
        JKMP::string readDelim(char delimiter);

		/** \brief recognizes an compExpression while parsing. If \a get ist \c true, this function first retrieves a new token by calling getToken() */
        jkmpNode* compExpression(bool get);

		/** \brief recognizes a logicalExpression while parsing. If \a get ist \c true, this function first retrieves a new token by calling getToken() */
        jkmpNode* logicalExpression(bool get);

		/** \brief recognizes a logicalTerm while parsing. If \a get ist \c true, this function first retrieves a new token by calling getToken() */
        jkmpNode* logicalTerm(bool get);

		/** \brief recognizes a mathExpression while parsing. If \a get ist \c true, this function first retrieves a new token by calling getToken() */
        jkmpNode* mathExpression(bool get);

		/** \brief recognizes a term while parsing. If \a get ist \c true, this function first retrieves a new token by calling getToken() */
        jkmpNode* mathTerm(bool get);

        /** \brief recognizes a vector_primary while parsing. If \a get ist \c true, this function first retrieves a new token by calling getToken() */
        jkmpNode* vectorPrimary(bool get);

        /** \brief recognizes a primary_lop while parsing. If \a get ist \c true, this function first retrieves a new token by calling getToken() */
        jkmpNode* primaryLOp(bool get);
        /** \brief recognizes a primary_op while parsing. If \a get ist \c true, this function first retrieves a new token by calling getToken() */
        jkmpNode* primaryOp(bool get);
        /** \brief recognizes a primary while parsing. If \a get ist \c true, this function first retrieves a new token by calling getToken() */
        jkmpNode* primary(bool get);

		/** \brief this stream is used to read in the program. An object is created and assigned
  		 * (and destroyed) by the parse()-function */
        std::istringstream * program;
        std::string progStr;

        MTRand rng;

        double readNumber();
        double readBin();
        double readHex();
        double readOct();
        double readDec();



        executionEnvironment environment;

        /** \brief the current token while parsing a string */
        jkmpTokenType CurrentToken;

        /** \brief the string value of the current token (when applicable) during the parsing step */
        JKMP::string StringValue;

        /** \brief the string value of the current token (when applicable) during the parsing step */
        double NumberValue;


        void* data;

        JKMP::stringVector lastError;
        int errors;

        char peekStream(std::istream* stream);
        void putbackStream(std::istream* stream, char ch);
        bool getFromStream(std::istream* stream, char& ch);

        JKMP::map<JKMP::string, void*> m_generalData;

	public:
        /** \brief class constructor */
        JKMathParser();

        /** \brief class destructor */
        virtual ~JKMathParser();

        void setGeneralData(const JKMP::string& id, const JKMP::GeneralData& data);
        JKMP::GeneralData getGeneraldata(const JKMP::string& id, const JKMP::GeneralData& defaultData=JKMP::GeneralDataDefault);

        /** \brief returns whether an error has occured since the last call to resetError() */
        bool hasErrorOccured() const;
        /** \brief retuns the last error */
        JKMP::string getLastError() const;
        /** \brief retuns the last error */
        JKMP::string getFirstError() const;
        /** \brief return a list of the last errors */
        JKMP::stringVector getLastErrors() const;
        /** \brief returns the number of errors since the last call to resetErrors() */
        int getLastErrorCount();
        /** \brief resets the error state to no errors */
        void resetErrors();

        void* get_data() const { return data; }
        void set_data(void* _data) { data=_data; }

        MTRand* get_rng() { return &rng; }


        /** \brief  register a new function
		 * \param name name of the new function
		 * \param function a pointer to the implementation
		 */
        void addFunction(const JKMP::string &name, jkmpEvaluateFunc function);
        void addFunction(const JKMP::string &name, jkmpEvaluateFunc function, jkmpEvaluateFuncSimple0Param f0, jkmpEvaluateFuncSimple1Param f1=NULL, jkmpEvaluateFuncSimple2Param f2=NULL, jkmpEvaluateFuncSimple3Param f3=NULL);
        void addFunction(const JKMP::string &name, jkmpEvaluateFunc function, jkmpEvaluateFuncSimple0ParamMP f0, jkmpEvaluateFuncSimple1ParamMP f1=NULL, jkmpEvaluateFuncSimple2ParamMP f2=NULL, jkmpEvaluateFuncSimple3ParamMP f3=NULL);


        /** \brief  register a new function
         * \param name name of the new function
         * \param function a pointer to the implementation
         */
        void addFunction(const JKMP::string &name, jkmpEvaluateFuncRefReturn function);
        void addFunction(const JKMP::string &name, jkmpEvaluateFuncRefReturn function, jkmpEvaluateFuncSimple0Param f0, jkmpEvaluateFuncSimple1Param f1=NULL, jkmpEvaluateFuncSimple2Param f2=NULL, jkmpEvaluateFuncSimple3Param f3=NULL);
        void addFunction(const JKMP::string &name, jkmpEvaluateFuncRefReturn function, jkmpEvaluateFuncSimple0ParamMP f0, jkmpEvaluateFuncSimple1ParamMP f1=NULL, jkmpEvaluateFuncSimple2ParamMP f2=NULL, jkmpEvaluateFuncSimple3ParamMP f3=NULL);
        void addFunction(const JKMP::string &name, jkmpEvaluateFromNodesFuncRefReturn function);

        inline void addFunction(const JKMP::string& name, const JKMP::stringVector& parameterNames, jkmpNode* function){
            environment.addFunction(name, parameterNames, function);
        }


        /** \brief set the defining struct of the given variable */
        inline void addVariable(const JKMP::string& name, const jkmpVariable &value) {
            environment.addVariable(name, value);
        }

        /** \brief get number of levels, on which this variable exists */
        inline int getVariableLevels(const JKMP::string& name) const {
            return environment.getVariableLevels(name);
        }


        /** \brief  register a new internal variable of type double
		 * \param name name of the new variable
		 * \param v initial value of this variable
		 */
        inline void addVariableDouble(const JKMP::string& name, double v) {
            environment.addVariableDouble(name, v);
        }

        /** \brief  register a new internal variable of type string
		 * \param name name of the new variable
		 * \param v initial value of this variable
		 */
        inline void addVariableString(const JKMP::string &name, const JKMP::string& v) {
            environment.addVariableString(name, v);
        }

        /** \brief  register a new internal variable of type boolean
         * \param name name of the new variable
         * \param v initial value of this variable
         */
        inline void addVariableBoolean(const JKMP::string& name, bool v) {
            environment.addVariableBoolean(name, v);
        }
        /** \brief  register a new internal variable of type boolean vector
         * \param name name of the new variable
         * \param v initial value of this variable
         */
        inline void addVariableBoolVector(const JKMP::string& name, const JKMP::vector<bool>& v) {
            environment.addVariableBoolVec(name, v);
        }
        /** \brief  register a new internal variable of type boolean vector
         * \param name name of the new variable
         * \param v initial value of this variable
         */
        inline void addVariableDoubleVector(const JKMP::string& name, const JKMP::vector<double>& v) {
            environment.addVariableDoubleVec(name, v);
        }
        /** \brief  register a new internal variable of type boolean vector
         * \param name name of the new variable
         * \param v initial value of this variable
         */
        inline void addVariableStringVector(const JKMP::string& name, const JKMP::stringVector& v) {
            environment.addVariableStringVec(name, v);
        }

        /** \brief  register a new internal variable of type boolean
         * \param name name of the new variable
         * \param v initial value of this variable
         */
        inline void addVariable(const JKMP::string &name, const jkmpResult &result) {
            environment.addVariable(name, result);
        }

        /** \brief  register a new internal variable if the given variable does not exist, otherwise set the axisting variable to the given value
         * \param name name of the new variable
         * \param v initial value of this variable
         */
        inline void setVariable(const JKMP::string &name, const jkmpResult &result) {
            environment.setVariable(name, result);
        }

        inline void setVariableDouble(const JKMP::string& name, double result) {
            environment.setVariableDouble(name, result);
        }

        inline void setVariableDoubleVec(const JKMP::string& name, const JKMP::vector<double>& result) {
            environment.setVariableDoubleVec(name, result);
        }
        inline void setVariableStringVec(const JKMP::string& name, const JKMP::stringVector& result) {
            environment.setVariableStringVec(name, result);
        }
        inline void setVariableBoolVec(const JKMP::string& name, const JKMP::vector<bool>& result) {
            environment.setVariableBoolVec(name, result);
        }
        inline void setVariableString(const JKMP::string& name, const JKMP::string& result) {
            environment.setVariableString(name, result);
        }
        inline void setVariableBoolean(const JKMP::string& name, bool result) {
            environment.setVariableBoolean(name, result);
        }


        inline void deleteVariable(const JKMP::string& name) {
            environment.deleteVariable(name);
        }

        /** \brief  returns the value of the given variable */
        inline void getVariable(jkmpResult& r, JKMP::string name) {
            environment.getVariable(r, name);
        }

        /** \brief  returns the value of the given variable */
        inline void getVariableOrInvalid(jkmpResult& r, JKMP::string name) {
            if (environment.variableExists(name)) {
                environment.getVariable(r, name);
            } else {
                r.setInvalid();
            }
        }



        /** \brief  evaluates a registered function
         * \param name name of the (registered function) to be evaluated
         * \param params array of the input parameters
         */
        inline void evaluateFunction(jkmpResult& r, const JKMP::string &name, const JKMP::vector<jkmpResult>& params) {
            environment.evaluateFunction(r, name, params);
        }

        /** \brief  evaluates a registered function
         * \param name name of the (registered function) to be evaluated
         * \param params array of the input parameters
         */
        inline void evaluateFunction(jkmpResult& r, const JKMP::string &name,  JKMP::vector<jkmpNode*> params) {
            environment.evaluateFunction(r, name, params);
        }



        /** \brief  returns the value of the given variable */
        inline jkmpResult getVariable(JKMP::string name) {
            return environment.getVariable(name);
        }

        /** \brief  returns the value of the given variable */
        inline jkmpResult getVariableOrInvalid(JKMP::string name) {
            if (environment.variableExists(name)) return environment.getVariable(name);
            return jkmpResult::invalidResult();

        }



        /** \brief  evaluates a registered function
         * \param name name of the (registered function) to be evaluated
         * \param params array of the input parameters
         */
        inline jkmpResult evaluateFunction(const JKMP::string &name, const JKMP::vector<jkmpResult>& params) {
            return environment.evaluateFunction(name, params);
        }



        /** \brief  evaluates a registered function
         * \param name name of the (registered function) to be evaluated
         * \param params array of the input parameters
         */
        inline jkmpResult evaluateFunction(const JKMP::string &name,  JKMP::vector<jkmpNode*> params) {
            return environment.evaluateFunction(name, params);
        }



        /** \brief  tests whether a variable exists */
        inline bool variableExists(const JKMP::string& name){ return environment.variableExists(name); }

        /** \brief  tests whether a function exists */
        inline bool functionExists(const JKMP::string& name){ return environment.functionExists(name); }

        inline void enterBlock() {
            environment.enterBlock();
        }
        inline void leaveBlock() {
            environment.leaveBlock();
        }
        inline int getBlockLevel() const {
            return environment.getBlockLevel();
        }

        /** \brief  deletes all defined variables. the memory of internal variables
         * will be released. the external memory will not be released.
         */
        inline void clearVariables(){
            environment.clearVariables();
        };

        /** \brief  clears the list of internal functions*/
        inline void clearFunctions(){
            environment.clearFunctions();
        };

        /** \brief  registers standard variables*/
        void addStandardVariables();

        /** \brief  registers standard functions*/
        void addStandardFunctions();

        /** \brief  parses the given expression*/
        jkmpNode* parse(std::string prog);

        /** \brief evaluate the given expression */
        jkmpResult evaluate(std::string prog);

        /** \brief  prints a list of all registered variables */
        inline JKMP::string printVariables(){
            return environment.printVariables();

        }

       inline  JKMP::vector<std::pair<JKMP::string, jkmpVariable> > getVariables(){
            return environment.getVariables();
        }

        /** \brief  prints a list of all registered variables */
        inline JKMP::string printFunctions(){
            return environment.printFunctions();
        }

        inline JKMP::vector<std::pair<JKMP::string, jkmpFunctionDescriptor> > getFunctions(){
            return environment.getFunctions();
        }


        /**
         * \defgroup jkmpglobal Tools to globaly extend JKMathParser
         * \ingroup jkmplib_mathtools_parser
         */
        /*@{*/
    protected:
        static JKMP::vector<std::pair<JKMP::string, jkmpEvaluateFunc> > externalGlobalFunctions;
        static JKMP::vector<std::pair<JKMP::string, jkmpEvaluateFuncRefReturn> > externalGlobalFunctionsRR;
        static JKMP::vector<std::pair<JKMP::string, jkmpEvaluateFromNodesFuncRefReturn> > externalGlobalFunctionsFN;
        static JKMP::vector<std::pair<JKMP::string, jkmpResult> > externalGlobalVariables;
    public:
        /** \brief  add a function to the list of global functions that are defined by a call to addStandardFunctions() (or the constructor) */
        static void addGlobalFunction(const JKMP::string& name, jkmpEvaluateFunc function);
        /** \brief  add a function to the list of global functions that are defined by a call to addStandardFunctions() (or the constructor) */
        static void addGlobalFunction(const JKMP::string& name, jkmpEvaluateFuncRefReturn function);
        /** \brief  add a function to the list of global functions that are defined by a call to addStandardFunctions() (or the constructor) */
        static void addGlobalFunction(const JKMP::string& name, jkmpEvaluateFromNodesFuncRefReturn function);
        /** \brief  add a variablen to the list of global variables that are defined by a call to addStandardVariables() (or the constructor) */
        static void addGlobalVariable(const JKMP::string& name, double value);
        /** \brief  add a variablen to the list of global variables that are defined by a call to addStandardVariables() (or the constructor) */
        static void addGlobalVariable(const JKMP::string& name, const JKMP::vector<double>& value);
        /** \brief  add a variablen to the list of global variables that are defined by a call to addStandardVariables() (or the constructor) */
        static void addGlobalVariable(const JKMP::string& name, const JKMP::vector<bool>& value);
        /** \brief  add a variablen to the list of global variables that are defined by a call to addStandardVariables() (or the constructor) */
        static void addGlobalVariable(const JKMP::string& name, const JKMP::stringVector& value);
        /** \brief  add a variablen to the list of global variables that are defined by a call to addStandardVariables() (or the constructor) */
        static void addGlobalVariable(const JKMP::string& name, const JKMP::string& value);
        /** \brief  add a variablen to the list of global variables that are defined by a call to addStandardVariables() (or the constructor) */
        static void addGlobalVariable(const JKMP::string& name, bool value);
        /*@}*/

};

#endif // JKMATHPARSER_H
/*@}*/
