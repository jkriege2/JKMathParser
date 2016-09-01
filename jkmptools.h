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

#ifndef JKMATHPARSERTOOLS_H
#define JKMATHPARSERTOOLS_H

#include "jkmplib_imexport.h"
//#include "qftools.h"
#include <cmath>
#include <iostream>

#include <map>
#include <vector>
#include <string>
#include <utility>
#include <stack>
#include "extlibs/MersenneTwister.h"
#include <stdint.h>
#include "jkmpbase.h"
#include "jkmpstringtools.h"

class JKMathParser; // forward



/**
 * \defgroup jkmpultil utilities for JKMathParser function parser class
 * \ingroup jkmplib_mathtools_parser
 */
/*@{*/

#define JKMATHPARSER_VERSION "0.5"


/** possible result types
 *    - \c jkmpDouble \c =0b00000001=0x01: a floating-point number with double precision. This is
 *                     also used to deal with integers
 *    - \c jkmpString \c =0b00000010=0x02: a string of characters
 *    - \c jkmpBool \c =0b00000100=0x04:   a boolean value true|false
 *    - \c jkmpDoubleVector \c =0b00001000=0x08:   a double-vector
 *    - \c jkmpStringVector \c =0b00010000=0x10:   a string-vector
 *    - \c jkmpBoolVector \c =0b00100000=0x20:   a boolean-vector
 *    - \c jkmpVoid \c =0b01000000=0x40:   a VOID value (i.e. no value)
 *    - \c jkmpStruct \c =0b10000000=0x80:   a struct
 *    - \c jkmpList \c =0b100000000=0x100:   a list of values
 *    - \c jkmpDoubleMatrix \c =0b1000000000=0x200:   a 2D matrix of numbers
 *    - \c jkmpBoolMatrix \c =0b10000000000=0x400:   a 2D matrix of booleans
 *  .
 *
 *
 */
enum jkmpResultType {jkmpDouble=0x01,  /*!< \brief a floating-point number with double precision. This is also used to deal with integers */
                     jkmpString=0x02,  /*!< \brief a string of characters */
                     jkmpBool=0x04,   /*!< \brief a boolean value true|false */
                     jkmpDoubleVector=0x08,  /*!< \brief a vector of floating point numbers */
                     jkmpStringVector=0x10, /*!< \brief a vector of strings */
                     jkmpBoolVector=0x20, /*!< \brief a vector of booleans */
                     jkmpVoid=0x40,  /*!< \brief a void/non-evaluatable result */
                     jkmpStruct=0x80,  /*!< \brief a struct datatype, i.e. a map string->jkmpResult */
                     jkmpList=0x100,  /*!< \brief a list of jkmpResult datatype */
                     jkmpDoubleMatrix=0x200,  /*!< \brief a 2D matrix of floating point numbers */
                     jkmpBoolMatrix=0x400, /*!< \brief a 2D matrix of booleans */
                     };
JKMPLIB_EXPORT JKMP::string jkmpResultTypeToString(jkmpResultType type);
struct  jkmpResult; // forward






/** \brief result of any expression  */
struct JKMPLIB_EXPORT jkmpResult {
    public:
        jkmpResult();
        ~jkmpResult();
        jkmpResult(double value);
        jkmpResult(int value);
        jkmpResult(unsigned int value);
        jkmpResult(JKMP::string value);
        jkmpResult(bool value);
        jkmpResult(const std::vector<double> &value);
        jkmpResult(const std::vector<bool> &value);
        jkmpResult(const JKMP::stringVector &value);
        jkmpResult(const jkmpResult &value);
        JKMPLIB_EXPORT jkmpResult& operator=(const jkmpResult &value);

        JKMPLIB_EXPORT void set(const jkmpResult &value);

        JKMPLIB_EXPORT void setInvalid();
        JKMPLIB_EXPORT void setVoid();
        /** \brief convert the value this struct representens into a JKMP::string */
        JKMPLIB_EXPORT JKMP::string toString(int precision=10) const;

        /** \brief convert the value this struct representens into a JKMP::string and adds the name of the datatype in \c [...] */
        JKMPLIB_EXPORT JKMP::string toTypeString(int precision=10) const;

        /** \brief convert the value this struct to an integer */
        JKMPLIB_EXPORT int32_t toInteger() const;
        /** \brief convert the value this struct to an integer */
        JKMPLIB_EXPORT uint32_t toUInt() const;
        JKMPLIB_EXPORT std::vector<uint32_t> toUIntVector() const;
        JKMPLIB_EXPORT std::vector<int32_t> toIntVector() const;
        /** \brief is this result convertible to integer? */
        inline bool isInteger() const {
            return (type==jkmpDouble)&&(fabs(num)<4294967296.0)&&(num==round(num));
        }
        /** \brief is this result convertible to unsigned integer? */
        inline bool isUInt() const {
            return (type==jkmpDouble)&&(fabs(num)<2147483648.0)&&(num>=0)&&(num==round(num));
        }
        /** \brief returns the size of the result (number of characters for string, numbers of entries in vectors, 0 for void and 1 else), same as size() */
        JKMPLIB_EXPORT size_t length() const;
        /** \brief returns the size of the result (number of characters for string, numbers of entries in vectors, 0 for void and 1 else), same as length() */
        JKMPLIB_EXPORT size_t size() const;
        /** \brief returns \c true, if the datatype is a matrix */
        JKMPLIB_EXPORT int dimensions() const;
        /** \brief for a mtrix, returns the x-size/number of columns */
        JKMPLIB_EXPORT size_t sizeX() const;
        /** \brief for a mtrix, returns the y-size/number of rows */
        JKMPLIB_EXPORT size_t sizeY() const;
        /** \brief for a mtrix, returns the x-size/number of columns */
        inline size_t columns() const {
            return sizeX();
        }
        /** \brief for a mtrix, returns the y-size/number of rows */
        inline size_t rows() const {
            return sizeY();
        }
        /** \brief returns the number of dimensions in the datatype */
        JKMPLIB_EXPORT bool isMatrix() const;
        /** \brief is this a double value */
        inline  bool isDouble() const  {
            return type==jkmpDouble;
        }
        inline  bool isDoubleVector() const  {
            return type==jkmpDoubleVector;
        }
        inline  bool isBool() const  {
            return type==jkmpBool;
        }
        inline  bool isBoolVector() const  {
            return type==jkmpBoolVector;
        }
        JKMPLIB_EXPORT  bool isUIntVector() const;
        JKMPLIB_EXPORT  bool isIntVector() const;
        /** \brief is this a double value */
        inline  bool convertsToDouble() const  {
            return (type==jkmpDouble || type==jkmpBool);
        }
        inline  bool convertsToBool() const  {
            return (type==jkmpDouble || type==jkmpBool);
        }
        /** \brief is this a double value */
        inline  bool convertsToInteger() const  {
            return (type==jkmpDouble || type==jkmpBool);
        }
        inline  bool convertsToUInt() const  {
            return ((type==jkmpDouble && num>=0) || type==jkmpBool);
        }

        JKMPLIB_EXPORT void setDouble(double val);
        JKMPLIB_EXPORT void setBoolean(bool val);
        JKMPLIB_EXPORT void setString(const JKMP::stringType& val);
        JKMPLIB_EXPORT void setString(int size=0, JKMP::charType defaultChar=JKMP::charType(' '));
        JKMPLIB_EXPORT void setDoubleVec(const std::vector<double>& val);
        JKMPLIB_EXPORT void setDoubleVec(int size=0, double defaultVal=0.0);
        JKMPLIB_EXPORT void setDoubleMatrix(int size=0, int cols=1, double  defaultVal=0);
        template <typename T>
        inline void setDoubleVec(T* data, size_t size) {
            setDoubleVec(size,0.0);
            for (size_t i=0; i<size; i++) {
                numVec[i]=double(data[i]);
            }
        }

        JKMPLIB_EXPORT void setStruct(const JKMP::stringVector& items=JKMP::stringVector());
        JKMPLIB_EXPORT void setList(int items=0);
        JKMPLIB_EXPORT void setList(const std::vector<jkmpResult>& dat);

        JKMPLIB_EXPORT void setBoolVec(const std::vector<bool>& val);
        JKMPLIB_EXPORT void setBoolVec(int size=0, bool defaultVal=false);
        JKMPLIB_EXPORT void setBoolMatrix(int size=0, int cols=1, bool defaultVal=false);

        template <typename T>
        inline void setBoolVec(T* data, size_t size) {
            setBoolVec(size);
            for (size_t i=0; i<size; i++) {
                boolVec[i]=bool(data[i]);
            }
        }

        JKMPLIB_EXPORT void setStringVec(const JKMP::stringVector& val);
        JKMPLIB_EXPORT void setStringVec(int size=0, const JKMP::string& defaultVal=JKMP::string(""));


        template <typename T>
        inline void setStringVec(T* data, size_t size) {
            setStringVec(size);
            for (size_t i=0; i<size; i++) {
                strVec[i]=data[i];
            }
        }
        /** \brief converst the result to a vector of number (numbers and number vectors are converted!) */
        JKMPLIB_EXPORT std::vector<double> asVector() const;

        /** \brief converst the result to a vector of number (numbers and number vectors are converted!) */
        JKMPLIB_EXPORT JKMP::stringVector asStrVector() const;
        /** \brief converst the result to a vector of number (numbers and number vectors are converted!) */
        JKMPLIB_EXPORT std::vector<bool> asBoolVector() const;
        /** \brief returns \c true, if the result may be converted to a vector of number */
        JKMPLIB_EXPORT bool  convertsToVector() const;
        /** \brief returns \c true, if the result may be converted to a vector of number */
        JKMPLIB_EXPORT bool  convertsToBoolVector() const;
        /** \brief returns \c true, if the result may be converted to a vector of number */
        JKMPLIB_EXPORT bool  convertsToStringVector() const;
        /** \brief converst the result to a vector of integers (numbers and number vectors are converted!) */
        JKMPLIB_EXPORT std::vector<int> asIntVector() const;
        /** \brief returns \c true, if the result may be converted to a vector of integers */
        inline bool  convertsToIntVector() const {
            return convertsToVector();
        }
        /** \brief returns \c true if the result is valid and not void */
        inline bool isUsableResult() const {
            return isValid && (type!=jkmpVoid);
        }
        /** \brief converst the result to a number (numbers are converted!) */
        inline double asNumber() const {
            if (type==jkmpDouble) return num;
            if (type==jkmpBool) return boolean?1:0;
            return NAN;
        }
        /** \brief converst the result to a number (numbers are converted and from a number vector the first element is returned!) */
        inline double asNumberAlsoVector() const {
            if (type==jkmpDouble) return num;
            if (type==jkmpDoubleVector && numVec.size()>0) return numVec[0];
            return NAN;
        }
        /** \brief converst the result to a number (numbers are converted and from a number vector the first element is returned!) */
        inline JKMP::string asStringAlsoVector() const {
            if (type==jkmpString) return str;
            if (type==jkmpStringVector && strVec.size()>0) return strVec[0];
            return JKMP::string();
        }
        /** \brief converst the result to a number (numbers are converted and from a number vector the first element is returned!) */
        inline bool asBooleanAlsoVector() const {
            if (type==jkmpBool) return boolean;
            if (type==jkmpBoolVector && boolVec.size()>0) return boolVec[0];
            return false;
        }
        /** \brief converst the result to a string (strings are converted!) */
        inline JKMP::string asString() const {
            if (type==jkmpString) return str;
            return JKMP::string();
        }
        /** \brief converst the result to a boolean (numbers and booleans are converted!) */
        inline bool asBool() const {
            if (type==jkmpBool) return boolean;
            if (type==jkmpDouble) return num!=0;
            return false;
        }
        /** \brief returns the type */
        inline jkmpResultType getType() const {
            return type;
        }
        /** \brief returns a string, describing the type of this result */
        JKMPLIB_EXPORT JKMP::string typeName() const;

        /** \brief returns an invalid result */
        JKMPLIB_EXPORT static jkmpResult invalidResult();
        /** \brief returns an void result */
        JKMPLIB_EXPORT static jkmpResult voidResult();

        /** \brief returns an entry from a struct */
        JKMPLIB_EXPORT jkmpResult getStructItem(const JKMP::string& item);
        /** \brief returns an entry from a struct */
        JKMPLIB_EXPORT void setStructItem(const JKMP::string& item, const jkmpResult& value);
        /** \brief returns an entry from a struct */
        JKMPLIB_EXPORT void setStruct(const JKMP::map<JKMP::string,jkmpResult>& data);

        /** \brief returns an entry from a jkmptList */
        JKMPLIB_EXPORT jkmpResult getListItem(int item) const;
        /** \brief returns an entry from a jkmptList */
        JKMPLIB_EXPORT jkmpResult getListItem(int item, const jkmpResult& defaultResult) const;
        /** \brief remove an entry from a jkmptList */
        JKMPLIB_EXPORT void removeListItem(int item);
        /** \brief append an entry to a jkmptList */
        JKMPLIB_EXPORT void appendListItem(const jkmpResult& item);
        /** \brief insert an entry to a jkmptList */
        JKMPLIB_EXPORT void insertListItem(int i, const jkmpResult& item);


        JKMPLIB_EXPORT static void add(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void sub(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void mul(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void div(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void mod(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void power(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void bitwiseand(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void bitwiseor(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void logicand(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void logicor(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void logicnot(jkmpResult& result, const jkmpResult& l, JKMathParser *p);
        JKMPLIB_EXPORT static void logicnand(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void logicnor(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void logicxor(jkmpResult& result, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void neg(jkmpResult& result, const jkmpResult& l, JKMathParser* p);
        JKMPLIB_EXPORT static void bitwisenot(jkmpResult& result, const jkmpResult& l, JKMathParser* p);



        JKMPLIB_EXPORT bool operator==(const jkmpResult& other) const;
        JKMPLIB_EXPORT bool operator!=(const jkmpResult& other) const;

        JKMPLIB_EXPORT static void compareequal(jkmpResult& res, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void comparenotequal(jkmpResult& res, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void comparegreater(jkmpResult& res, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void comparegreaterequal(jkmpResult& res, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void comparesmaller(jkmpResult& res, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);
        JKMPLIB_EXPORT static void comparesmallerequal(jkmpResult& res, const jkmpResult& l, const jkmpResult& r, JKMathParser* p);

        bool isValid;
        jkmpResultType type;   /*!< \brief type of the result */
        JKMP::string str;       /*!< \brief contains result if \c type==jkmpString */
        double num;            /*!< \brief contains result if \c type==jkmpDouble */
        bool boolean;          /*!< \brief contains result if \c type==jkmpBool */
        JKMP::vector<double> numVec; /*!< \brief contains result if \c type==jkmpDoubleVector */
        JKMP::stringVector strVec;
        JKMP::vector<bool> boolVec;
        JKMP::map<JKMP::string,jkmpResult> structData;
        JKMP::vector<jkmpResult> listData;
        int matrix_columns;


    private:
};













/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is numeric->numeric, e.g. sin(x). The result is numeric->numeric or num_vector->num_vector

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return ;\
    }\
    if (params[0].type==jkmpDouble) {\
        r.setDouble(CFUNC(params[0].num));\
    } else if(params[0].type==jkmpDoubleVector) {\
        r.setDoubleVec(params[0].numVec.size());\
        for (size_t i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i]);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
}

/** \brief same as JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(FName, CFUNC) JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(FName, CFUNC, CFUNC)



/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is numeric_vector->numeric_vector (std::vector<double> -> std::vector<double>)

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to any number parameter
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_NUMERICVEC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector) {\
        r.setDoubleVec(CFUNC(params[0].numVec));\
    } else {\
        p->jkmpError(JKMP::_("%1(x) argument has to be vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}


/*! \brief This macro allows to easily define functions for JKMathParser from a C template-function that
           is vector->vector it has to accept JKMP::vector and JKMP::vector as arguments! The result of the parser
           function will have the same type as the input!

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to any number parameter
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_VECTOR_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector) {\
        r.setDoubleVec(CFUNC(params[0].numVec));\
    } else if(params[0].type==jkmpStringVector) {\
        r.setStringVec(CFUNC(params[0].strVec));\
    } else if(params[0].type==jkmpBoolVector) {\
        r.setBoolVec(CFUNC(params[0].boolVec));\
    } else {\
        p->jkmpError(JKMP::_("%1(x) argument has to be a vector of numbers/booleans/strings").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}



/*! \brief This macro allows to easily define functions for JKMathParser from a C template-function that
           is vector->vector or string->string it has to accept JKMP::vector and JKMP::vector as arguments! The result of the parser
           function will have the same type as the input!

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to any number parameter
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_VECTORORSTRING_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector) {\
        r.setDoubleVec(CFUNC(params[0].numVec));\
    } else if(params[0].type==jkmpStringVector) {\
        r.setStringVec(CFUNC(params[0].strVec));\
    } else if(params[0].type==jkmpBoolVector) {\
        r.setBoolVec(CFUNC(params[0].boolVec));\
    } else if(params[0].type==jkmpString) {\
        r.setString(CFUNC(params[0].str));\
    } else {\
        p->jkmpError(JKMP::_("%1(x) argument has to be a vector of numbers/booleans/strings, or a string").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is numeric_vector->numeric (std::vector<double> -> double), e.g. qfstatisticsMedian()

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_VECTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].numVec));\
    } else {\
        p->jkmpError(JKMP::_("%1(x) argument has to be a vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (double*,size_t)->numeric (std::vector<double> -> double), e.g. qfstatisticsMedian()

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_VECTONUM_CFUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].numVec.data(), params[0].numVec.size()));\
    } else {\
        p->jkmpError(JKMP::_("%1(x) argument has to be a vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}



/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is numeric_vector->numeric (std::vector<double> -> double), e.g. qfstatisticsMedian()

    This variant also accepts a single double-number and converts it to a std::vector<double> before evaluating the function.

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_VECORNUMTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].numVec));\
    } if(params[0].type==jkmpDouble) {\
        r.setDouble(CFUNC(JKMP::vector<double>::construct(params[0].num)));\
    } else {\
        p->jkmpError(JKMP::_("%1(x) argument has to be a vector of numbers or a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}


/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (double*,size_t)->numeric (std::vector<double> -> double), e.g. qfstatisticsMedian()

    This variant also accepts a single double-number and converts it to a std::vector<double> before evaluating the function.

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_VECORNUMTONUM_CFUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].numVec.data(), params[0].numVec.size()));\
    } if(params[0].type==jkmpDouble) {\
        JKMP::vector<double> d(1, params[0].num); \
        r.setDouble(CFUNC(d.data(), d.size()));\
    } else {\
        p->jkmpError(JKMP::_("%1(x) argument has to be a vector of numbers or a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}


/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is numeric_vector->numeric (std::vector<double> -> double), e.g. qfstatisticsMedian()

    This variant also accepts a single double-number, or a list of numbers and converts it to a std::vector<double> before evaluating the function.

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n<1) {\
        p->jkmpError(JKMP::_("%1(x1, x2, ...) needs at least 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(n==1 && params[0].type==jkmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].numVec));\
    } if( n>=1) {\
        JKMP::vector<double> d; \
        for (unsigned int i=0; i<n; i++) {\
            if (params[i].type==jkmpDouble) { \
                d<<params[i].num;     \
            } else if (params[i].type==jkmpDoubleVector) {\
                d<<params[i].numVec;     \
            } else {\
                p->jkmpError(JKMP::_("%1(x1, x2, ...) argument %2 has to be a vector of numbers or a number").arg(#NAME_IN_PARSER).arg(i+1));\
                r.setInvalid();\
                return; \
            }\
        } \
        r.setDouble(CFUNC(d));\
    } else {\
        p->jkmpError(JKMP::_("%1(x1, x2, ...) argument 1 has to be a vector of numbers or a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}


/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (double*,size_t)->numeric (std::vector<double> -> double), e.g. qfstatisticsMedian()

    This variant also accepts a single double-number, or a list of numbers and converts it to a std::vector<double> before evaluating the function.

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_CFUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n<1) {\
        p->jkmpError(JKMP::_("%1(x1, x2, ...) needs at least 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(n==1 && params[0].type==jkmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].numVec.data(), params[0].numVec.size()));\
    } if( n>=1) {\
        JKMP::vector<double> d; \
        for (unsigned int i=0; i<n; i++) {\
            if (params[i].type==jkmpDouble) { \
                d<<params[i].num;     \
            } else if (params[i].type==jkmpDoubleVector) {\
                d<<params[i].numVec;     \
            } else {\
                p->jkmpError(JKMP::_("%1(x1, x2, ...) argument %2 has to be a vector of numbers or a number").arg(#NAME_IN_PARSER).arg(i+1));\
                r.setInvalid();\
                return; \
            }\
        } \
        r.setDouble(CFUNC(d.data(), d.size()));\
    } else {\
        p->jkmpError(JKMP::_("%1(x1, x2, ...) argument 1 has to be a vector of numbers or a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}



/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric_vector, numeric)->number , e.g. qfstatisticsQuantile()

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAM1VEC_VECTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(x, p) needs exacptly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble) {\
        r.setDouble(CFUNC(params[0].numVec, params[1].num));\
    } else {\
        p->jkmpError(JKMP::_("%1(x, p) argument x has to be a vector of numbers and p a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}



/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (double*,size_t, numeric)->number , e.g. qfstatisticsQuantile()

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAM1VEC_VECTONUM_CFUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(x, p) needs exacptly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble) {\
        r.setDouble(CFUNC(params[0].numVec.data(), params[0].numVec.size(), params[1].num));\
    } else {\
        p->jkmpError(JKMP::_("%1(x, p) argument x has to be a vector of numbers and p a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric_vector, numeric)->number. If the second argument is also a vector, the function
           will be applied element-wise to this vector to build an output vector!

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAM1VEC2POSVEC_VECTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(x, p) needs exacptly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble) {\
        r.setDouble(CFUNC(params[0].numVec, params[1].num));\
    } else if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector) {\
        JKMP::vector<double> res=params[1].numVec;\
        for (size_t i=0; i<params[1].numvec.size(); i++) {\
            res[i]=CFUNC(params[0].numVec, res[i]);\
        }\
        r.setDoubleVec(res);\
    } else {\
        p->jkmpError(JKMP::_("%1(x, p) argument x has to be a vector of numbers and p a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}


/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (double*,size_t, numeric)->number. If the second argument is also a vector, the function
           will be applied element-wise to this vector to build an output vector!
           The C-function expects (double* data, int length) and not a std::vector<double> as parameter!)

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAM1VEC2POSVEC_VECTONUM_CFUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(x, p) needs exacptly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble) {\
        r.setDouble(CFUNC(params[0].numVec.size(), params[0].numVec.size(), params[1].num));\
    } else if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector) {\
        JKMP::vector<double> res=params[1].numVec;\
        for (size_t i=0; i<params[1].numvec.size(); i++) {\
            res[i]=CFUNC(params[0].numVec.data(), params[0].numVec.size(), res[i]);\
        }\
    r.setDoubleVec(res);\
    } else {\
        p->jkmpError(JKMP::_("%1(x, p) argument x has to be a vector of numbers and p a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}


/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric_vector, numeric)->number. If the second argument is also a vector, the function
           will be applied element-wise to this vector to build an output vector!

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAM1NUM2VEC_VECTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(x, p) needs exacptly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDouble && params[1].type==jkmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].num, params[1].numVec));\
    } else if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector) {\
        JKMP::vector<double> res=params[0].numVec;\
        for (size_t i=0; i<res.size(); i++) {\
            res[i]=CFUNC(res[i], params[1].numVec);\
        }\
        r.setDoubleVec(res);\
    } else {\
        p->jkmpError(JKMP::_("%1(x, p) argument x has to be a number or a vector of numbers and p a vector of number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}


/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric, double*,size_t)->number. If the second argument is also a vector, the function
           will be applied element-wise to this vector to build an output vector!
           The C-function expects (double, double* data, int length) and not a std::vector<double> as parameter!)

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAM1NUM2VEC_VECTONUM_CFUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(x, p) needs exacptly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDouble && params[1].type==jkmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].num, params[1].numVec.data(), params[1].numVec.size()));\
    } else if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector) {\
        JKMP::vector<double> res=params[0].numVec;\
        for (size_t i=0; i<res.size(); i++) {\
            res[i]=CFUNC(res[i], params[1].numVec.data(), params[1].numVec.size());\
        }\
        r.setDoubleVec(res);\
    } else {\
        p->jkmpError(JKMP::_("%1(x, p) argument x has to be a number or a vector of numbers and p a vector of number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}



/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric_vector, numeric, numeric)->number

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_3PARAM1VEC_VECTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=3) {\
        p->jkmpError(JKMP::_("%1(x, p1, p2) needs exacptly 3 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble && params[2].type==jkmpDouble) {\
        r.setDouble(CFUNC(params[0].numVec, params[1].num, params[2].num));\
    } else {\
        p->jkmpError(JKMP::_("%1(x, p) argument x has to be a vector of numbers and p a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}



/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (double*,size_t, numeric, numeric)->number

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_3PARAM1VEC_VECTONUM_CFUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=3) {\
        p->jkmpError(JKMP::_("%1(x, p1, p2) needs exacptly 3 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble && params[2].type==jkmpDouble) {\
        r.setDouble(CFUNC(params[0].numVec.data(), params[0].numVec.size(), params[1].num, params[2].num));\
    } else {\
        p->jkmpError(JKMP::_("%1(x, p) argument x has to be a vector of numbers and p a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric_vector, numeric_vector)->numeric, e.g. qfstatisticsCorrelationCoefficient()

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAM2VEC_VECTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(x, y) needs exactly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].asVector(), params[1].asVector()));\
    } else {\
        p->jkmpError(JKMP::_("%1(x, y) arguments x and y have to be a vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (double*,size_t, double*,size_t)->numeric, e.g. qfstatisticsCorrelationCoefficient()

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAM2VEC_VECTONUM_CFUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(x, y) needs exactly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector) {\
        JKMP::vector<double> p1=params[0].asVector(); \
        JKMP::vector<double> p2=params[1].asVector(); \
        r.setDouble(CFUNC(p1.data(), p1.size(), p2.data(), p2.size()));\
    } else {\
        p->jkmpError(JKMP::_("%1(x, y) arguments x and y have to be a vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (double*, double*,size_t)->numeric, e.g. qfstatisticsCorrelationCoefficient()

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAM2VEC_VECTONUM_CFUNC_1N(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(x, y) needs exactly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector) {\
        JKMP::vector<double> p1=params[0].asVector(); \
        JKMP::vector<double> p2=params[1].asVector(); \
        r.setDouble(CFUNC(p1.data(), p2.data(), std::min(p1.size(),p2.size())));\
    } else {\
        p->jkmpError(JKMP::_("%1(x, y) arguments x and y have to be a vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is string->string.  The result is string->string or str_vector->str_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any string parameter
      - apply the C-function item-wise to any string vector parameter item
      - result is also string
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.type=jkmpString;\
    r.isValid=true;\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpString) {\
        r.str=CFUNC(params[0].str);\
    } else if (params[0].type==jkmpStringVector) {\
        r.setStringVec(params[0].strVec.size());\
        for (size_t i=0; i<params[0].strVec.size(); i++) {\
            r.strVec[i]=CFUNC(params[0].strVec[i]);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) argument has to be a string or string-vector").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/** \brief same as JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC_SIMPLE(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC_SIMPLE(FName, CFUNC) JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(FName, CFUNC, CFUNC)


/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is string->str_vector.  The result is string->str_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any string parameter
      - apply the C-function item-wise to any string vector parameter item
      - result is also string
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_STRING2STRINGVEC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.type=jkmpStringVector;\
    r.strVec.clear();\
    r.isValid=true;\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpString) {\
        r.strVec=CFUNC(params[0].str);\
    } else {\
        p->jkmpError(JKMP::_("%1(...) argument has to be a string").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}



/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is numeric->bool.  The result is numeric->bool or num_vector->bool_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any string parameter
      - apply the C-function item-wise to any string vector parameter item
      - result is also string
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_NUM2BOOL_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.type=jkmpBool;\
    r.isValid=false;\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpDouble) {\
        r.setBoolean(CFUNC(params[0].num));\
    } else if (params[0].type==jkmpDoubleVector) {\
        r.setBoolVec(params[0].numVec.size());\
        for (size_t i=0; i<params[0].numVec.size(); i++) {\
            r.boolVec[i]=CFUNC(params[0].numVec[i]);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) argument has to be a number or number-vector").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/** \brief same as JKMATHPARSER_DEFINE_1PARAM_NUM2BOOL_FUNC_SIMPLE(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define JKMATHPARSER_DEFINE_1PARAM_NUM2BOOL_FUNC_SIMPLE(FName, CFUNC) JKMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(FName, CFUNC, CFUNC)



/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is numeric->string.  The result is numeric->string or num_vector->str_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any string parameter
      - apply the C-function item-wise to any string vector parameter item
      - result is also string
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.type=jkmpString;\
    r.isValid=true;\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpDouble) {\
        r.str=CFUNC(params[0].num);\
    } else if (params[0].type==jkmpDoubleVector) {\
        r.setStringVec(params[0].numVec.size());\
        for (size_t i=0; i<params[0].numVec.size(); i++) {\
            r.strVec[i]=CFUNC(params[0].numVec[i]);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) argument has to be a number or number-vector").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}




/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is string->numeric.  The result is string->numeric or str_vector->num_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any string parameter
      - apply the C-function item-wise to any string vector parameter item
      - result is also string
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_STRING2NUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.setInvalid();\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpString) {\
        r.setDouble(CFUNC(params[0].str));\
    } else if (params[0].type==jkmpStringVector) {\
        r.setDoubleVec(params[0].strVec.size());\
        for (size_t i=0; i<params[0].strVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].strVec[i]);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) argument has to be a string or string-vector").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}



/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is string->boolean.  The result is string->boolean or str_vector->boolean_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any string parameter
      - apply the C-function item-wise to any string vector parameter item
      - result is also string
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_STRING2BOOL_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.type=jkmpBool;\
    r.isValid=true;\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpString) {\
        r.boolean=CFUNC(params[0].str);\
    } else if (params[0].type==jkmpStringVector) {\
        r.setBoolVec(params[0].strVec.size());\
        for (size_t i=0; i<params[0].strVec.size(); i++) {\
            r.boolVec[i]=CFUNC(params[0].strVec[i]);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) argument has to be a string or string-vector").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/** \brief same as JKMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC_SIMPLE(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define JKMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC_SIMPLE(FName, CFUNC) JKMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(FName, CFUNC, CFUNC)


/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is bool->string.  The result is bool->string or bool_vector->str_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any bool parameter
      - apply the C-function item-wise to any bool vector parameter item
      - result is also string
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_1PARAM_BOOL2STRING_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.type=jkmpString;\
    r.isValid=true;\
    if (n!=1) {\
        p->jkmpError(JKMP::_("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpBool) {\
        r.str=CFUNC(params[0].boolean);\
    } else if (params[0].type==jkmpBoolVector) {\
        r.setStringVec(params[0].boolVec.size());\
        for (size_t i=0; i<params[0].boolVec.size(); i++) {\
            r.strVec[i]=CFUNC(params[0].boolVec[i]);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) argument has to be a boolean or boolean-vector").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/** \brief same as JKMATHPARSER_DEFINE_1PARAM_BOOL2STRING_FUNC_SIMPLE(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define JKMATHPARSER_DEFINE_1PARAM_BOOL2STRING_FUNC_SIMPLE(FName, CFUNC) JKMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(FName, CFUNC, CFUNC)



/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric, numeric_vector)->numeric.  The result is (numeric, numeric)->numeric or (numeric, num_vector)->num_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAM2VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.type=jkmpDouble;\
    r.isValid=true;\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(...) needs exactly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    if (params[0].type==jkmpDouble) {\
        pa=params[0].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) first argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[1].type==jkmpDouble) {\
        r.num=CFUNC(pa, params[1].num);\
    } else if(params[1].type==jkmpDoubleVector) {\
        r.type=jkmpDoubleVector;\
        r.numVec.resize(params[1].numVec.size());\
        for (size_t i=0; i<params[1].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(pa, params[1].numVec[i]);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) second argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric, numeric)->numeric, e.g. yn(x, p), p is a number, x may be a vector
             The result is (numeric, numeric)->numeric or (num_vector, numeric)->num_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.isValid=true;\
    r.type=jkmpDouble;\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(...) needs exactly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    if (params[1].type==jkmpDouble) {\
        pa=params[1].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) second argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpDouble) {\
        r.num=CFUNC(params[0].num, pa);\
    } else if(params[0].type==jkmpDoubleVector) {\
        r.type=jkmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (size_t i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], pa);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}








/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric, numeric, numeric)->numeric
             The result is (numeric, numeric, numeric)->numeric or (num_vector, numeric, numeric)->num_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_3PARAM1VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.isValid=true;\
    r.type=jkmpDouble;\
    if (n!=3) {\
        p->jkmpError(JKMP::_("%1(...) needs exactly 3 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    double pb=0; \
    if (params[1].type==jkmpDouble) {\
        pa=params[1].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) second argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[2].type==jkmpDouble) {\
        pb=params[2].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) third argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpDouble) {\
        r.num=CFUNC(params[0].num, pa, pb);\
    } else if(params[0].type==jkmpDoubleVector) {\
        r.type=jkmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (size_t i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], pa, pb);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}




/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric, numeric, numeric, numeric)->numeric
             The result is (numeric, numeric, numeric, numeric)->numeric or (num_vector, numeric, numeric, numeric)->num_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_4PARAM1VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.isValid=true;\
    r.type=jkmpDouble;\
    if (n!=4) {\
        p->jkmpError(JKMP::_("%1(...) needs exactly 4 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    double pb=0; \
    double pc=0; \
    if (params[1].type==jkmpDouble) {\
        pa=params[1].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) second argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[2].type==jkmpDouble) {\
        pb=params[2].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) third argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[3].type==jkmpDouble) {\
        pc=params[3].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) fourth argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpDouble) {\
        r.num=CFUNC(params[0].num, pa, pb, pc);\
    } else if(params[0].type==jkmpDoubleVector) {\
        r.type=jkmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (size_t i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], pa, pb, pc);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric, numeric)->numeric, e.g. yn(p, x), p is a number, x may be a vector
             The result is (numeric, numeric)->numeric or (num_vector, numeric)->num_vector.
           The c-function takes the vector-argument as LAST argument!

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAMLVEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.isValid=true;\
    r.type=jkmpDouble;\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(...) needs exactly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    if (params[1].type==jkmpDouble) {\
        pa=params[1].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) second argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpDouble) {\
        r.num=CFUNC(params[0].num, pa);\
    } else if(params[0].type==jkmpDoubleVector) {\
        r.type=jkmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (size_t i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], pa);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric, numeric, numeric)->numeric
             The result is (numeric, numeric, numeric)->numeric or (num_vector, numeric, numeric)->num_vector.
             The c-function takes the vector-argument as LAST argument!

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_3PARAMLVEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.isValid=true;\
    r.type=jkmpDouble;\
    if (n!=3) {\
        p->jkmpError(JKMP::_("%1(...) needs exactly 3 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    double pb=0; \
    if (params[1].type==jkmpDouble) {\
        pa=params[1].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) second argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[2].type==jkmpDouble) {\
        pb=params[2].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) third argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpDouble) {\
        r.num=CFUNC(params[0].num, pa, pb);\
    } else if(params[0].type==jkmpDoubleVector) {\
        r.type=jkmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (size_t i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], pa, pb);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}




/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric, numeric, numeric, numeric)->numeric
             The result is (numeric, numeric, numeric, numeric)->numeric or (num_vector, numeric, numeric, numeric)->num_vector.
             The c-function takes the vector-argument as LAST argument!

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_4PARAMLVEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.isValid=true;\
    r.type=jkmpDouble;\
    if (n!=4) {\
        p->jkmpError(JKMP::_("%1(...) needs exactly 4 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    double pb=0; \
    double pc=0; \
    if (params[1].type==jkmpDouble) {\
        pa=params[1].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) second argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[2].type==jkmpDouble) {\
        pb=params[2].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) third argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[3].type==jkmpDouble) {\
        pc=params[3].num;\
    } else {\
        p->jkmpError(JKMP::_("%1(...) fourth argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpDouble) {\
        r.num=CFUNC(params[0].num, pa, pb, pc);\
    } else if(params[0].type==jkmpDoubleVector) {\
        r.type=jkmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (size_t i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], pa, pb, pc);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}



/*! \brief This macro allows to easily define functions for JKMathParser from a C-function that
           is (numeric,numeric)->numeric, e.g. fmod(x, y).  The result is (numeric, numeric)->numeric or (num_vector, num_vector)->num_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter, i.e. [ fmod(x1,y1), fmod(x2,y2), ... ]
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define JKMATHPARSER_DEFINE_2PARAM12VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){\
    r.isValid=true;\
    r.type=jkmpDouble;\
    if (n!=2) {\
        p->jkmpError(JKMP::_("%1(...) needs exactly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==jkmpDouble && params[1].type==jkmpDouble) {\
    r.num=CFUNC(params[0].num, params[1].num);\
    } else if(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector) {\
        if (params[0].numVec.size()!=params[1].numVec.size()) {\
            p->jkmpError(JKMP::_("%1(x,y) both arguments have to have same length").arg(#NAME_IN_PARSER));\
            r.setInvalid();\
            return; \
        }\
        r.type=jkmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (size_t i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], params[1].numVec[i]);\
        }\
    } else {\
        p->jkmpError(JKMP::_("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/** \brief same as JKMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define JKMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC_SIMPLE(FName, CFUNC) JKMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC(FName, CFUNC, CFUNC)

/** \brief same as JKMATHPARSER_DEFINE_2PARAM2VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define JKMATHPARSER_DEFINE_2PARAM2VEC_NUMERIC_FUNC_SIMPLE(FName, CFUNC) JKMATHPARSER_DEFINE_2PARAM2VEC_NUMERIC_FUNC(FName, CFUNC, CFUNC)

/** \brief same as JKMATHPARSER_DEFINE_2PARAM12VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define JKMATHPARSER_DEFINE_2PARAM12VEC_NUMERIC_FUNC_SIMPLE(FName, CFUNC) JKMATHPARSER_DEFINE_2PARAM12VEC_NUMERIC_FUNC(FName, CFUNC, CFUNC)


/*@}*/

#endif // JKMATHPARSERTOOLS_H

