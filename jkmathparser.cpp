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

/** \file jkmathparser.cpp
 *  \ingroup jkmp
 */

#define COMPILING_THIS_JKMATHPARSER
#include "jkmathparser.h" // class's header file
#include <iostream>
#include <float.h>
#include <stdint.h>
#include <limits>
#include <cctype>
#include <locale>
#include <algorithm>
#include "jkmpdefaultlib.h"




void JKMathParser::addStandardVariables(){
    if (!JKMathParser_DefaultLib::hasDefaultVariables(this)) {
        JKMathParser_DefaultLib::addDefaultVariables(this);
    }


    for (size_t i=0; i<externalGlobalVariables.size(); i++) {
        addVariable(externalGlobalVariables[i].first, externalGlobalVariables[i].second);
    }

}

void JKMathParser::addStandardFunctions(){
    if (!JKMathParser_DefaultLib::hasDefaultFunctions(this)) {
        JKMathParser_DefaultLib::addDefaultFunctions(this);
    }

    addVariableString("version", JKMATHPARSER_VERSION);

    for (size_t i=0; i<externalGlobalFunctions.size(); i++) {
        addFunction(externalGlobalFunctions[i].first, externalGlobalFunctions[i].second);
    }
    for (size_t i=0; i<externalGlobalFunctionsRR.size(); i++) {
        addFunction(externalGlobalFunctionsRR[i].first, externalGlobalFunctionsRR[i].second);
    }
    for (size_t i=0; i<externalGlobalFunctionsFN.size(); i++) {
        addFunction(externalGlobalFunctionsFN[i].first, externalGlobalFunctionsFN[i].second);
    }
}
























JKMP::string JKMathParser::resultTypeToString(jkmpResultType type)
{
    return jkmpResultTypeToString(type);
}

JKMP::string JKMathParser::resultToString(const jkmpResult& r)
{
    return r.toString();
}

JKMP::string JKMathParser::resultToTypeString(const jkmpResult& r)
{
    return r.toTypeString();
}

jkmpResult JKMathParser::getInvalidResult()
{    
    return jkmpResult::invalidResult();
}

void JKMathParser::jkmpError(JKMP::string st)
{
    //qDebug()<<"##ERROR: "<<st;
    lastError.push_back(st);
    errors++;
}

JKMP::string JKMathParser::tokentostring(JKMathParser::jkmpTokenType token) {
    switch(token) {
        case END: return "END";
        case PRINT: return "PRINT (;)";
        case COMMA: return "COMMA (,)";
	    case STRING_DELIM: return "STRING_DELIM (' or \")";
	    case NAME: return "NAME";
	    case NUMBER: return "NUMBER";
	    case PLUS: return "PLUS (+)";
	    case MINUS: return "MINUS (-)";
	    case MUL: return "MUL (*)";
	    case DIV: return "DIV (/)";
	    case MODULO: return "MODULO (%)";
	    case ASSIGN: return "ASSIGN (=)";
        case LPARENTHESE: return "LPARENTHESE '('";
        case RPARENTHESE: return "RPARENTHESE ')'";
        case LBRACKET: return "LBRACKET '['";
        case COLON: return "COLON ':'";
        case RBRACKET: return "RBRACKET ']'";
        case LBRACE: return "LBRACE '{'";
        case RBRACE: return "RBRACE '}'";
        case POWER: return "POWER (^)";
        case DOT: return "DOT (.)";
        case TILDE: return "TILDE (~)";
        case FACTORIAL_LOGIC_NOT: return "FACTORIAL_LOGIC_NOT (!)";
	    case LOGIC_NOT: return "LOGIC_NOT (!/not)";
	    case LOGIC_AND: return "LOGIC_AND (&&/and)";
	    case LOGIC_OR: return "LOGIC_OR (||/or)";
        case BINARY_AND: return "BINARY_AND (&)";
        case BINARY_OR: return "BINARY_OR (|)";
        case LOGIC_XOR: return "LOGIC_XOR (xor)";
	    case LOGIC_NOR: return "LOGIC_NOR (nor)";
	    case LOGIC_NAND: return "LOGIC_NAND (nand)";
	    case LOGIC_TRUE: return "LOGIC_TRUE (true)";
	    case LOGIC_FALSE: return "LOGIC_FALSE (false)";
	    case COMP_EQUALT: return "COMP_EQUALT (==)";
	    case COMP_UNEQUAL: return "COMP_UNEQUAL (!=)";
	    case COMP_GREATER: return "COMP_GREATER (>)";
	    case COMP_SMALLER: return "COMP_SMALLER (<)";
	    case COMP_GEQUAL: return "COMP_GEQUAL (>=)";
	    case COMP_SEQUAL: return "COMP_SEQUAL (<=)";
    }
    return "unknown";
}

JKMP::string JKMathParser::currenttokentostring() {
    return tokentostring(CurrentToken);
}



/******************************************************************************************
 * jkMathParser
 ******************************************************************************************/

// class constructor
JKMathParser::JKMathParser() {
    //qDebug()<<"constructing JKMathParser";
    environment.setParent(this);
    //qDebug()<<"constructing JKMathParser: adding functions";
    addStandardFunctions();
    //qDebug()<<"constructing JKMathParser: adding variables";
    addStandardVariables();
    //qDebug()<<"constructing JKMathParser: resetting errors";
    resetErrors();
    //qDebug()<<"done";
}


// class destructor
JKMathParser::~JKMathParser()
{
	clearFunctions();
    clearVariables();
}

void JKMathParser::setGeneralData(const JKMP::string &id, const JKMP::GeneralData &data)
{
    m_generalData[id]=data;
}

JKMP::GeneralData JKMathParser::getGeneraldata(const JKMP::string &id, const JKMP::GeneralData &defaultData)
{
    auto it=m_generalData.find(id);
    if (it==m_generalData.end()) return defaultData;
    else return it->second;
}

bool JKMathParser::hasErrorOccured() const
{
    return errors>0;
}







JKMP::vector<std::pair<JKMP::string, JKMathParser::jkmpEvaluateFunc> > JKMathParser::externalGlobalFunctions= JKMP::vector<std::pair<JKMP::string, JKMathParser::jkmpEvaluateFunc> >() ;
JKMP::vector<std::pair<JKMP::string, JKMathParser::jkmpEvaluateFuncRefReturn> > JKMathParser::externalGlobalFunctionsRR= JKMP::vector<std::pair<JKMP::string, JKMathParser::jkmpEvaluateFuncRefReturn> >() ;
JKMP::vector<std::pair<JKMP::string, JKMathParser::jkmpEvaluateFromNodesFuncRefReturn> > JKMathParser::externalGlobalFunctionsFN= JKMP::vector<std::pair<JKMP::string, JKMathParser::jkmpEvaluateFromNodesFuncRefReturn> >() ;
JKMP::vector<std::pair<JKMP::string, jkmpResult> > JKMathParser::externalGlobalVariables= JKMP::vector<std::pair<JKMP::string, jkmpResult> >();


void JKMathParser::addGlobalFunction(const JKMP::string &name, JKMathParser::jkmpEvaluateFunc function)
{
    externalGlobalFunctions.push_back(std::make_pair(name, function));
}

void JKMathParser::addGlobalFunction(const JKMP::string &name, JKMathParser::jkmpEvaluateFuncRefReturn function)
{
    externalGlobalFunctionsRR.push_back(std::make_pair(name, function));
}

void JKMathParser::addGlobalFunction(const JKMP::string &name, JKMathParser::jkmpEvaluateFromNodesFuncRefReturn function)
{
    externalGlobalFunctionsFN.push_back(std::make_pair(name, function));

}

void JKMathParser::addGlobalVariable(const JKMP::string &name, double value)
{
    externalGlobalVariables.push_back(std::make_pair(name, jkmpResult(value)));
}

void JKMathParser::addGlobalVariable(const JKMP::string &name, const JKMP::vector<double> &value)
{
    externalGlobalVariables.push_back(std::make_pair(name, jkmpResult(value)));
}

void JKMathParser::addGlobalVariable(const JKMP::string &name, const JKMP::vector<bool> &value)
{
    externalGlobalVariables.push_back(std::make_pair(name, jkmpResult(value)));
}

void JKMathParser::addGlobalVariable(const JKMP::string &name, const JKMP::stringVector &value)
{
    externalGlobalVariables.push_back(std::make_pair(name, jkmpResult(value)));
}

void JKMathParser::addGlobalVariable(const JKMP::string &name, const JKMP::string &value)
{
    externalGlobalVariables.push_back(std::make_pair(name, jkmpResult(value)));
}

void JKMathParser::addGlobalVariable(const JKMP::string &name, bool value)
{
    externalGlobalVariables.push_back(std::make_pair(name, jkmpResult(value)));
}










char JKMathParser::peekStream(std::istream *stream)
{
    if (!stream) return char(0);
    return stream->peek();
}

void JKMathParser::putbackStream(std::istream *stream, char ch)
{
    if (ch!=0 && stream) stream->putback(ch);
}

bool JKMathParser::getFromStream(std::istream *stream, char &c)
{
    if (!stream) return false;
    c=char(0);
    if (stream->eof()) {
        //qDebug()<<"read "<<c<<": false";
        return false;
    }
    c=stream->get();
    //qDebug()<<"read "<<c<<": true";
    return true;
}






void JKMathParser::addFunction(const JKMP::string &name, JKMathParser::jkmpEvaluateFunc function)
{
    jkmpFunctionDescriptor f;
    f.function=function;
    f.name=name;
    f.type=JKMathParser::functionC;
    environment.setFunction(name, f);
}

void JKMathParser::addFunction(const JKMP::string &name, JKMathParser::jkmpEvaluateFuncRefReturn function)
{
    jkmpFunctionDescriptor f;
    f.functionRR=function;
    f.name=name;
    f.type=JKMathParser::functionCRefReturn;
    environment.setFunction(name, f);
}

void JKMathParser::addFunction(const JKMP::string &name, JKMathParser::jkmpEvaluateFunc function, JKMathParser::jkmpEvaluateFuncSimple0Param f0, JKMathParser::jkmpEvaluateFuncSimple1Param f1, JKMathParser::jkmpEvaluateFuncSimple2Param f2, JKMathParser::jkmpEvaluateFuncSimple3Param f3)
{
    jkmpFunctionDescriptor f;
    f.function=function;
    f.name=name;
    f.type=JKMathParser::functionC;
    if (f0) f.simpleFuncPointer[0]=(void*)f0;
    if (f1) f.simpleFuncPointer[1]=(void*)f1;
    if (f2) f.simpleFuncPointer[2]=(void*)f2;
    if (f3) f.simpleFuncPointer[3]=(void*)f3;
    environment.setFunction(name, f);
}

void JKMathParser::addFunction(const JKMP::string &name, JKMathParser::jkmpEvaluateFunc function, JKMathParser::jkmpEvaluateFuncSimple0ParamMP f0, JKMathParser::jkmpEvaluateFuncSimple1ParamMP f1, JKMathParser::jkmpEvaluateFuncSimple2ParamMP f2, JKMathParser::jkmpEvaluateFuncSimple3ParamMP f3)
{
    jkmpFunctionDescriptor f;
    f.function=function;
    f.name=name;
    f.type=JKMathParser::functionC;
    if (f0) f.simpleFuncPointer[100]=(void*)f0;
    if (f1) f.simpleFuncPointer[101]=(void*)f1;
    if (f2) f.simpleFuncPointer[102]=(void*)f2;
    if (f3) f.simpleFuncPointer[103]=(void*)f3;
    environment.setFunction(name, f);
}

void JKMathParser::addFunction(const JKMP::string &name, JKMathParser::jkmpEvaluateFuncRefReturn function, JKMathParser::jkmpEvaluateFuncSimple0Param f0, JKMathParser::jkmpEvaluateFuncSimple1Param f1, JKMathParser::jkmpEvaluateFuncSimple2Param f2, JKMathParser::jkmpEvaluateFuncSimple3Param f3)
{
    jkmpFunctionDescriptor f;
    f.functionRR=function;
    f.name=name;
    f.type=JKMathParser::functionCRefReturn;
    if (f0) f.simpleFuncPointer[0]=(void*)f0;
    if (f1) f.simpleFuncPointer[1]=(void*)f1;
    if (f2) f.simpleFuncPointer[2]=(void*)f2;
    if (f3) f.simpleFuncPointer[3]=(void*)f3;
    environment.setFunction(name, f);
}

void JKMathParser::addFunction(const JKMP::string &name, JKMathParser::jkmpEvaluateFuncRefReturn function, JKMathParser::jkmpEvaluateFuncSimple0ParamMP f0, JKMathParser::jkmpEvaluateFuncSimple1ParamMP f1, JKMathParser::jkmpEvaluateFuncSimple2ParamMP f2, JKMathParser::jkmpEvaluateFuncSimple3ParamMP f3)
{
    jkmpFunctionDescriptor f;
    f.functionRR=function;
    f.name=name;
    f.type=JKMathParser::functionCRefReturn;
    if (f0) f.simpleFuncPointer[100]=(void*)f0;
    if (f1) f.simpleFuncPointer[101]=(void*)f1;
    if (f2) f.simpleFuncPointer[102]=(void*)f2;
    if (f3) f.simpleFuncPointer[103]=(void*)f3;
    environment.setFunction(name, f);
}

void JKMathParser::addFunction(const JKMP::string &name, JKMathParser::jkmpEvaluateFromNodesFuncRefReturn function)
{
    jkmpFunctionDescriptor f;
    f.functionFN=function;
    f.name=name;
    f.type=JKMathParser::functionFromNode;
    environment.setFunction(name, f);

}


JKMathParser::jkmpTokenType JKMathParser::getToken(){
    char ch=0;
    while(getFromStream(program, ch) && std::isspace(ch)) {
		;
	}


    switch (ch) {
		case 0:
		case -1:
			return CurrentToken=END;
        case ';':
            return CurrentToken=PRINT;
        case '.':
            return CurrentToken=DOT;
        case ':':
            return CurrentToken=COLON;
        case '*':

            return CurrentToken=MUL;
            break;
        case '/':{
                char ch1=0;
                if (program) getFromStream(program, ch1);
                if (ch1=='/') {
                    eatSinglelineComment();
                    return getToken();
                }
                if (ch1=='*') {
                    eatMultilineComment();
                    return getToken();
                }
                putbackStream(program, ch1);
                return CurrentToken=DIV;
            }
            break;
		case '%':
			return CurrentToken=MODULO;
		case '+':
			return CurrentToken=PLUS;
		case '-':
			return CurrentToken=MINUS;
        case '(':
            return CurrentToken=LPARENTHESE;
        case ')':
            return CurrentToken=RPARENTHESE;
        case '[':
            return CurrentToken=LBRACKET;
        case ']':
            return CurrentToken=RBRACKET;
        case '{':
            return CurrentToken=LBRACE;
        case '}':
            return CurrentToken=RBRACE;
        case ',':
            return CurrentToken=COMMA;
		case '"':
			return CurrentToken=STRING_DELIM;
		case '^':
			return CurrentToken=POWER;
        case '~':
            return CurrentToken=TILDE;
        case '!':{
            char ch1=0;
            if (program) getFromStream(program, ch1);
			if (ch1=='=') return CurrentToken=COMP_UNEQUAL;
			// else
            putbackStream(program, ch1);
			return CurrentToken=FACTORIAL_LOGIC_NOT;
		}
		case '&':{
            char ch1=0;
            if (program) getFromStream(program, ch1);
			if (ch1=='&') return CurrentToken=LOGIC_AND;
			// else
            putbackStream(program, ch1);
            return CurrentToken=BINARY_AND;
            //jkmpError(JKMP::_("undefined operator '&'; Did you mean LOGICAL_AND ('&&' / 'and')?"));
            break;
		}
		case '|':{
            char ch1=0;
            if (program) getFromStream(program, ch1);
			if (ch1=='|') return CurrentToken=LOGIC_OR;
			// else
            putbackStream(program, ch1);
            return CurrentToken=BINARY_OR;
            //jkmpError(JKMP::_("undefined operator '|'; Did you mean LOGICAL_OR ('||' / 'or')?"));
            break;
		}
		case '=':{
            char ch1=0;
            if (program) getFromStream(program, ch1);
			if (ch1=='=') return CurrentToken=COMP_EQUALT;
			// else
            putbackStream(program, ch1);
			return CurrentToken=ASSIGN;
		}
		case '>':{
            char ch1=0;
            if (program) getFromStream(program, ch1);
			if (ch1=='=') return CurrentToken=COMP_GEQUAL;
			// else
            putbackStream(program, ch1);
			return CurrentToken=COMP_GREATER;
		}
		case '<':{
            char ch1=0;
            if (program) getFromStream(program, ch1);
			if (ch1=='=') return CurrentToken=COMP_SEQUAL;
			// else
            putbackStream(program, ch1);
			return CurrentToken=COMP_SMALLER;
		}
		case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            putbackStream(program, ch);
            //(*program) >> NumberValue;
            NumberValue=readNumber();
			return CurrentToken=NUMBER;
		}
		default:
            if (std::isalpha(ch, std::locale("C")) || (ch=='_')) { // try to recognize NAME, LOGIC_TRUE, LOGIC_FALSE, DIFF_LBRACKET
				StringValue=ch;
                while (getFromStream(program, ch) && (std::isdigit(ch, std::locale("C"))||std::isalpha(ch, std::locale("C")) || (ch=='_'))) {
                    if ((std::isdigit(ch, std::locale("C"))||std::isalpha(ch, std::locale("C"))) || (ch=='_')) {
                        StringValue<<ch;
                    }
				}

                putbackStream(program, ch); // now put the last thing read back int the stream, as it
									  // could belong to the next token

				if (StringValue=="true") return CurrentToken=LOGIC_TRUE;
				if (StringValue=="false") return CurrentToken=LOGIC_FALSE;
                if (StringValue.toLower()=="nan") {
                    NumberValue=std::numeric_limits<double>::quiet_NaN();
                    return CurrentToken=NUMBER;
                }
                if (StringValue.toLower()=="inf") {
                    NumberValue=std::numeric_limits<double>::infinity();
                    return CurrentToken=NUMBER;
                }
                if (StringValue.toLower()=="ninf") {
                    NumberValue=-std::numeric_limits<double>::infinity();
                    return CurrentToken=NUMBER;
                }
                if (StringValue=="and") return CurrentToken=LOGIC_AND;
                if (StringValue=="or") return CurrentToken=LOGIC_OR;
                if (StringValue=="band") return CurrentToken=BINARY_AND;
                if (StringValue=="bor") return CurrentToken=BINARY_OR;
                if (StringValue=="xor") return CurrentToken=LOGIC_XOR;
				if (StringValue=="not") return CurrentToken=LOGIC_NOT;
				if (StringValue=="nor") return CurrentToken=LOGIC_NOR;
				if (StringValue=="nand") return CurrentToken=LOGIC_NAND;

				return CurrentToken=NAME;
			}
			// the parser has found an unknown token. an exception will be thrown
			//std::cout<<StringValue<<",   "<<ch<<std::endl;
            jkmpError(JKMP::string("get_next_token: unknown token currentCharacter='%1', currentString='%2'").arg(JKMP::chartostr(ch)).arg(StringValue));
            break;
	}
    return END;
}

void JKMathParser::eatMultilineComment()
{
    char ch1=0;
    char ch2=0;
    char ch=0;
    while(getFromStream(program, ch)) {
        ch1=ch2;
        ch2=ch;
        if (ch1=='*' && ch2=='/') {
            break;
        }
    }
}

void JKMathParser::eatSinglelineComment()
{
    char ch=0;
    while(getFromStream(program, ch) && ch!='\n') {
        ;
    }
}


JKMathParser::jkmpNode* JKMathParser::parse(std::string prog){
    progStr=prog;
    program=new std::istringstream(progStr);
    JKMathParser::jkmpNode* res=NULL;
    JKMathParser::jkmpNodeList* resList=new JKMathParser::jkmpNodeList(this);
	while(true) {
		getToken();
        if (CurrentToken == END) {
            break;
        }
        res= logicalExpression(false);
        resList->add(res);
        //qDebug()<<"parse add nodelist item "<<resList->getCount()<<"  = "<<res->evaluate().toTypeString();
	}
	delete program;
    //qDebug()<<"parsed nodelist with "<<resList->getCount()<<" items";
    if (resList->getCount()==1) {
        //delete resList;
        //qDebug()<<"returning single item";
        res=resList->popLast(false);
        delete resList;
        return res;
    } else {
        //qDebug()<<"returning list";
        return resList;
    }
}

jkmpResult JKMathParser::evaluate(std::string prog) {
    JKMathParser::jkmpNode* res=parse(prog);
    jkmpResult r;
    res->evaluate(r);
    delete res;
    return r;
}

JKMathParser::jkmpNode* JKMathParser::logicalExpression(bool get){
    JKMathParser::jkmpNode* left=logicalTerm(get);

	for(;;) // forever, do until you find anything else than an expressions
		switch(CurrentToken) {
			case LOGIC_OR:
                left= (JKMathParser::jkmpNode*)new jkmpBinaryBoolNode(jkmpLOPor, left, logicalTerm(true), this, NULL);
				break;
            case LOGIC_XOR:
                left= (JKMathParser::jkmpNode*)new jkmpBinaryBoolNode(jkmpLOPxor, left, logicalTerm(true), this, NULL);
				break;
			case LOGIC_NOR:
                left= (JKMathParser::jkmpNode*)new jkmpBinaryBoolNode(jkmpLOPnor, left, logicalTerm(true), this, NULL);
				break;
			default:
				return left;
		}
}

JKMathParser::jkmpNode* JKMathParser::logicalTerm(bool get){
    JKMathParser::jkmpNode* left=compExpression(get);

	for(;;) // forever, do until you find anything else than an expressions
		switch(CurrentToken) {
            case LOGIC_AND:
                left= (JKMathParser::jkmpNode*)new jkmpBinaryBoolNode(jkmpLOPand, left, compExpression(true), this, NULL);
                break;
            case LOGIC_NAND:
                left= (JKMathParser::jkmpNode*)new jkmpBinaryBoolNode(jkmpLOPnand, left, compExpression(true), this, NULL);
				break;
			default:
				return left;
		}
}

JKMathParser::jkmpNode* JKMathParser::compExpression(bool get){
    JKMathParser::jkmpNode* left=mathExpression(get);

	for(;;) // forever, do until you find anything else than an expressions
		switch(CurrentToken) {
			case COMP_EQUALT:
                left= (JKMathParser::jkmpNode*)new jkmpCompareNode(jkmpCOMPequal, left, mathExpression(true), this, NULL);
				break;
			case COMP_UNEQUAL:
                left= (JKMathParser::jkmpNode*)new jkmpCompareNode(jkmpCOMPnequal, left, mathExpression(true), this, NULL);
				break;
			case COMP_GREATER:
                left= (JKMathParser::jkmpNode*)new jkmpCompareNode(jkmpCOMPgreater, left, mathExpression(true), this, NULL);
				break;
			case COMP_SMALLER:
                left= (JKMathParser::jkmpNode*)new jkmpCompareNode(jkmpCOMPlesser, left, mathExpression(true), this, NULL);
				break;
			case COMP_GEQUAL:
                left= (JKMathParser::jkmpNode*)new jkmpCompareNode(jkmpCOMPgreaterequal, left, mathExpression(true), this, NULL);
				break;
			case COMP_SEQUAL:
                left= (JKMathParser::jkmpNode*)new jkmpCompareNode(jkmpCOMPlesserequal, left, mathExpression(true), this, NULL);
				break;
			default:
				return left;
		}
}


JKMathParser::jkmpNode* JKMathParser::mathExpression(bool get){
    JKMathParser::jkmpNode* left=mathTerm(get);

	for(;;) // forever, do until you find anything else than an expressions
		switch(CurrentToken) {
			case PLUS:
                left= (JKMathParser::jkmpNode*)new jkmpBinaryArithmeticNode('+', left, mathTerm(true), this, NULL);
				break;
            case BINARY_OR:
                left= (JKMathParser::jkmpNode*)new jkmpBinaryArithmeticNode('|', left, mathTerm(true), this, NULL);
                break;
            case MINUS:
                left= (JKMathParser::jkmpNode*)new jkmpBinaryArithmeticNode('-', left, mathTerm(true), this, NULL);
				break;
			default:
				return left;
		}
}

JKMathParser::jkmpNode* JKMathParser::mathTerm(bool get){
    JKMathParser::jkmpNode* left=primaryLOp(get);

    for(;;) {
        // forever, do until you find anything else than a term
        switch(CurrentToken) {
        case MUL:
            left= (JKMathParser::jkmpNode*)new jkmpBinaryArithmeticNode('*', left, primaryLOp(true), this, NULL);
            break;
        case DIV:
            left= (JKMathParser::jkmpNode*)new jkmpBinaryArithmeticNode('/', left, primaryLOp(true), this, NULL);
            break;
        case MODULO:
            left= (JKMathParser::jkmpNode*)new jkmpBinaryArithmeticNode('%', left, primaryLOp(true), this, NULL);
            break;
        case BINARY_AND:
            left= (JKMathParser::jkmpNode*)new jkmpBinaryArithmeticNode('&', left, primaryLOp(true), this, NULL);
            break;
        default:
            return left;
        }
    }
}



JKMathParser::jkmpNode *JKMathParser::primaryLOp(bool get)
{
    JKMathParser::jkmpNode* res=NULL;

    if (get) getToken();

        switch(CurrentToken) {
            case MINUS:  // found primary:  - primary
                res= (JKMathParser::jkmpNode*)new jkmpUnaryNode('-', vectorPrimary(true), this, NULL);
                break;

            case FACTORIAL_LOGIC_NOT:// found primary:  !/not primary
            case LOGIC_NOT:
                res= (JKMathParser::jkmpNode*)new jkmpUnaryNode('!', vectorPrimary(true), this, NULL);
                break;

            case TILDE:// found primary:  ~ primary
                res= (JKMathParser::jkmpNode*)new jkmpUnaryNode('~', vectorPrimary(true), this, NULL);
                break;

            default:
                res=vectorPrimary(false);
                break;

        }

    if (!res) res=new jkmpInvalidNode(this, NULL);

    //qDebug()<<"primaryLOp";

    return res;

}



JKMathParser::jkmpNode* JKMathParser::vectorPrimary(bool get){

    JKMathParser::jkmpNode* res=NULL;
    //if (get) getToken();

      // vector construct start:delta:end and start:end


    res=primaryOp(get);

    if (CurrentToken==COLON) {
        JKMathParser::jkmpNode* v1=res;
        JKMathParser::jkmpNode* v2=primaryOp(true);
        if (CurrentToken==COLON) {
            //qDebug()<<"vectorPrimary ::";
            res=(JKMathParser::jkmpNode*)new jkmpVectorConstructionNode(v1, primaryOp(true), v2, this, NULL);
            //qDebug()<<"constructed 'a:d:e' current="<<tokentostring(CurrentToken);
        } else {
            //qDebug()<<"vectorPrimary :";
            res=(JKMathParser::jkmpNode*)new jkmpVectorConstructionNode(v1, v2, NULL, this, NULL);
            //qDebug()<<"constructed 'a:e' current="<<tokentostring(CurrentToken);
        }
    }

    return res;
}


JKMathParser::jkmpNode *JKMathParser::primaryOp(bool get)
{
    JKMathParser::jkmpNode* res=primary(get);

    for(;;) {

        switch(CurrentToken) {
            case POWER:  // found primary:  ^ primary
                //qDebug()<<"primaryOp ^";
                res=(JKMathParser::jkmpNode*)new jkmpBinaryArithmeticNode('^', res, primary(true), this, NULL);
                break;

            case LBRACKET: { // found '['
                    //qDebug()<<"primaryOp [";
                    JKMathParser::jkmpNode* parameter=logicalExpression(true);
                    if (CurrentToken == RBRACKET) {
                        res=new jkmpVectorAccessNode(res, parameter, this, NULL);
                        getToken();
                    } else {
                        if (res) delete res;
                        jkmpError(JKMP::string("parsing primary_op: ']' expected, but '%1' found").arg(currenttokentostring()));
                        return new jkmpInvalidNode(this, NULL);
                    }

                    //getToken();
                } break;
            case DOT: { // found '.'
                    //qDebug()<<"primaryOp [";
                    if (getToken()==NAME) {
                        res=new jkmpStructAccessNode(res, StringValue, this, NULL);
                        getToken();
                    } else {
                        if (res) delete res;
                        jkmpError(JKMP::string("parsing primary_op: 'NAME' expected in EXPR.NAME (struct access) construct, but '%1' found").arg(currenttokentostring()));
                        return new jkmpInvalidNode(this, NULL);
                    }

                    //getToken();
                } break;

            default:
                if (!res) res=new jkmpInvalidNode(this, NULL);
                //qDebug()<<"primaryOp";
                return res;

        }

    }

    if (!res) res=new jkmpInvalidNode(this, NULL);

    return res;

}

JKMathParser::jkmpNode* JKMathParser::primary(bool get){
    JKMathParser::jkmpNode* res=NULL;

	if (get) getToken();

	switch(CurrentToken) {
		case NUMBER: {
            jkmpResult val(NumberValue);
			getToken();
            res= (JKMathParser::jkmpNode*)new jkmpConstantNode(val, this, NULL);
			break;
		}

		case NAME: {
                //jkMathParser::jkmpNode* def=NULL;
                JKMP::string varname=StringValue;
                getToken();
                if (CurrentToken == ASSIGN) { // assign a variable name
                    res=new jkmpVariableAssignNode(varname, logicalExpression(true), this, NULL);
                } else if (CurrentToken == LBRACKET) { // vector element access found
                    JKMathParser::jkmpNode* parameter=logicalExpression(true);
                    if ( CurrentToken != RBRACKET ) {
                        jkmpError(JKMP::_("parsing primary: ']' expected after '[...', but '%1' found").arg(currenttokentostring()));
                        return NULL;
                    }
                    getToken();
                    if (CurrentToken == ASSIGN) { // assign a variable name
                        res=new jkmpVectorElementAssignNode(varname, parameter, logicalExpression(true), this, NULL);
                    } else {
                        res=new jkmpVariableVectorAccessNode(varname, parameter, this, NULL);
                    }
                } else if (CurrentToken == LPARENTHESE) { // function found
                    //JKMathParser::jkmpNode** params=(JKMathParser::jkmpNode**)qfMalloc(255*sizeof(JKMathParser::jkmpNode*));
                    JKMP::vector<JKMathParser::jkmpNode*> params;


                    getToken();
                    while ((CurrentToken != RPARENTHESE)&&(CurrentToken!=END)) {
                        JKMathParser::jkmpNode* parameter=logicalExpression(params.size()>0);
                        params.push_back(parameter);
                        if ((CurrentToken!=RPARENTHESE)&&(CurrentToken!=COMMA)&&(CurrentToken!=END)) {
                            jkmpError(JKMP::_("parsing primary: ')' or ',' expected after '(...', but '%1' found").arg(currenttokentostring()));
                            return NULL;
                        }

                    }

                    if ( CurrentToken != RPARENTHESE ) {
                        jkmpError(JKMP::_("parsing primary: ')' expected after '(...', but '%1' found").arg(currenttokentostring()));
                        return NULL;
                    }
                    getToken();
                    JKMP::string lvarname=varname.toLower();
#ifdef JKMATHPARSER_DEBUGFUNCTIONNAMES
                    if (lvarname=="if" || lvarname=="dbgif") {
#else
                    if (lvarname=="if") {
#endif
                        if (params.size()!=3) {
                            jkmpError(JKMP::_("parsing primary: 'if(decision, trueValue, falseValue)' expects 3 arguments, but '%1' found").arg(params.size()));
                            return NULL;
                        }
                        jkmpCasesNode* cn=new jkmpCasesNode(this, NULL);
                        cn->addCase(params[0], params[1]);
                        cn->setElse(params[2]);
                        res=cn;
#ifdef JKMATHPARSER_DEBUGFUNCTIONNAMES
                    } else if (lvarname=="cases" || lvarname=="dbgcases") {
#else
                    } else if (lvarname=="cases") {
#endif
                        if (params.size()<3 || (params.size()%2==0)) {
                            jkmpError(JKMP::_("parsing primary: 'cases(decision1, trueValue1, decision2, trueValue2, ..., elseValue)' expects 3, 5, 7 ... arguments, but '%1' found").arg(params.size()));
                            return NULL;
                        }
                        jkmpCasesNode* cn=new jkmpCasesNode(this, NULL);
                        for (size_t i=0; i<params.size()-1; i=i+2) {
                            cn->addCase(params[i], params[i+1]);
                        }
                        cn->setElse(params.back());
                        res=cn;
                    } else if (lvarname=="cumsum" || lvarname=="cumprod" || lvarname=="sum" || lvarname=="prod" || lvarname=="for" || lvarname=="defaultfor" || lvarname=="savefor" || lvarname=="filterfor" || lvarname=="savefilterfor") {
                        if (params.size()==1) {
                            if (lvarname!="for"&&lvarname!="savefor"&&lvarname!="defaultfor") {
                                res=new jkmpFunctionNode(varname, params, this, NULL);
                            } else {
                                jkmpError(JKMP::_("parsing primary: '%2(NAME, start[, delta], end, expression)' expects 3-5 arguments, but '%1' found").arg(params.size()).arg(lvarname));
                                return NULL;
                            }
                        } else if (params.size()==3){
                            JKMP::string iName="";
                            jkmpVariableNode* vn=dynamic_cast<jkmpVariableNode*>(params[0]);
                            if (lvarname!="defaultfor"){
                                if (vn) {
                                    iName=vn->getName();
                                } else {
                                    jkmpError(JKMP::_("parsing primary: '%1(NAME, ..., expression)' expects a variable name as first argument").arg(lvarname));
                                    return NULL;
                                }
                                res=new jkmpVectorOperationNode(lvarname, iName, params[1], params[2], this, NULL);
                            } else {
                                jkmpError(JKMP::_("parsing primary: '%1(NAME, ..., expression, defaultValue)' expects at least 4 arguments").arg(lvarname));
                                return NULL;
                            }
                        } else if (params.size()==4){
                            JKMP::string iName="";
                            jkmpVariableNode* vn=dynamic_cast<jkmpVariableNode*>(params[0]);
                            jkmpNode* defVal=NULL;
                            //jkmpNode* start=params[1];
                            //jkmpNode* delta=params[2];
                            //jkmpNode* end=params[3];
                            if (vn) {
                                iName=vn->getName();
                            } else {
                                jkmpError(JKMP::_("parsing primary: '%1(NAME, ..., expression)' expects a variable name as first argument").arg(lvarname));
                                return NULL;
                            }
                            if (lvarname=="defaultfor"){
                                //start=params[1];
                                //end=params[2];
                                //defVal=params[3];
                                //delta=NULL;
                                res=new jkmpVectorOperationNode(lvarname, iName, params[1], params[2], this, NULL, defVal);
                            } else {
                                res=new jkmpVectorOperationNode(lvarname, iName, params[1], params[2], NULL, params[3], this, NULL, defVal);
                            }


                        } else if (params.size()==5){
                            JKMP::string iName="";
                            jkmpVariableNode* vn=dynamic_cast<jkmpVariableNode*>(params[0]);
                            if (vn) {
                                iName=vn->getName();
                            } else {
                                jkmpError(JKMP::_("parsing primary: '%1(NAME, ..., expression)' expects a variable name as first argument").arg(lvarname));
                                return NULL;
                            }
                            if (lvarname=="defaultfor"){
                                res=new jkmpVectorOperationNode(lvarname, iName, params[1], params[2], NULL, params[3], this, NULL, params[4]);
                            } else {
                                res=new jkmpVectorOperationNode(lvarname, iName, params[1], params[3], params[2], params[4], this, NULL);
                            }
                        } else if (params.size()==6){
                            JKMP::string iName="";
                            jkmpVariableNode* vn=dynamic_cast<jkmpVariableNode*>(params[0]);
                            if (vn) {
                                iName=vn->getName();
                            } else {
                                jkmpError(JKMP::_("parsing primary: '%1(NAME, ..., expression)' expects a variable name as first argument").arg(lvarname));
                                return NULL;
                            }
                            if (lvarname=="defaultfor"){
                                res=new jkmpVectorOperationNode(lvarname, iName, params[1], params[3], params[2], params[4], this, NULL, params[5]);
                            } else {
                                jkmpError(JKMP::_("parsing primary: '%1(NAME, ..., expression)' expects at most 5 arguments").arg(lvarname));
                                return NULL;
                            }
                        } else {
                            jkmpError(JKMP::_("parsing primary: '%2(NAME, start[, delta], end, expression)' expects 4 or 5 arguments and 'sum|prod|for(NAME, value_list, expression)' expects 3 arguments, but '%1' found").arg(params.size()).arg(lvarname));
                            return NULL;
                        }
                    } else if (CurrentToken==ASSIGN) { // function assignment
                        bool allParamsAreNames=true;
                        JKMP::stringVector pnames;
                        for (size_t i=0; i<params.size(); i++) {
                            jkmpVariableNode* n=NULL;
                            if ((n=dynamic_cast<jkmpVariableNode*>(params[i]))) {
                                pnames<<n->getName();
                            } else {
                                allParamsAreNames=false;
                            }

                        }
                        //qDebug()<<"FASSIGN: "<<varname<<allParamsAreNames<<pnames<<currenttokentostring();
                        if (allParamsAreNames) {
                            res=new jkmpFunctionAssignNode(varname, pnames, logicalExpression(true)/* primary(true)*/, this, NULL);
                        } else {
                            jkmpError(JKMP::_("parsing primary: malformed function assignmentfound, expected this form: FNAME(P1, P2, ...)=expression").arg(currenttokentostring()));
                            return NULL;
                        }
                    } else {
                        //qDebug()<<"FNODE: "<<varname<<currenttokentostring();
                        res=new jkmpFunctionNode(varname, params, this, NULL);
                    }

                } else {
                  res=(JKMathParser::jkmpNode*)new jkmpVariableNode(varname, this, NULL);
                }
                //res= def;
                break;

		}

		case STRING_DELIM: {// found primary: "data" == string constant
            jkmpResult val(readDelim('"'));
            res= (JKMathParser::jkmpNode*)new jkmpConstantNode(val, this, NULL);
			getToken();
			break;
		}

        /*case MINUS:  // found primary:  - primary
            res= (JKMathParser::jkmpNode*)new jkmpUnaryNode('-', primary(true), this, NULL);
			break;

        case FACTORIAL_LOGIC_NOT:
        case LOGIC_NOT:
            res= (JKMathParser::jkmpNode*)new jkmpUnaryNode('!', primary(true), this, NULL);
			break;

        case TILDE:
            res= (JKMathParser::jkmpNode*)new jkmpUnaryNode('~', primary(true), this, NULL);
            break;*/

        case LPARENTHESE: { // found primary: ( expression )
            JKMathParser::jkmpNode* expr=logicalExpression(true);
            if (CurrentToken != RPARENTHESE) {
                jkmpError(JKMP::string("parsing primary: ')' expected, but '%1' found").arg(currenttokentostring()));
                return NULL;
            }
			getToken(); // swallow ")"
			res= expr;
			break;
		}
        case LOGIC_TRUE: {// found 'true'
            jkmpResult val(true);
            res= (JKMathParser::jkmpNode*)new jkmpConstantNode(val, this, NULL);
			getToken();
			break;
        }
		case LOGIC_FALSE: {// found 'false'
            jkmpResult val(false);
            res= (JKMathParser::jkmpNode*)new jkmpConstantNode(val, this, NULL);
			getToken();
			break;
        }




        case LBRACKET: { // found '['
                jkmpVectorMatrixConstructionList* vl=new jkmpVectorMatrixConstructionList(this, NULL);
                res=vl;
                getToken();
                while ((CurrentToken != RBRACKET)) {
                    JKMathParser::jkmpNode* parameter=logicalExpression(vl->getCount()>0);
                    vl->add(parameter);
                    if (CurrentToken==PRINT) {
                        vl->add(NULL);
                        vl->setIsMatrix(true);
                    }
                    if ((CurrentToken!=RBRACKET)&&(CurrentToken!=COMMA)&&(CurrentToken!=PRINT)) {
                        jkmpError(JKMP::string("parsing primary (vector construction node '[a,b;c,...]'): ']', ';' or ',' expected, but '%1' found after %2 elements").arg(currenttokentostring()).arg(vl->getCount()));
                        return new jkmpInvalidNode(this, NULL);
                    }
                    //qDebug()<<"adding to [...]: "<<vl->getCount()<<". element CurrentToke="<<currenttokentostring();
                }


                if ( CurrentToken != RBRACKET ) {
                    jkmpError(JKMP::string("parsing primary (vector construction node '[a,b,...]'): ']' expected, but '%1' found").arg(currenttokentostring()));
                    return new jkmpInvalidNode(this, NULL);
                }
                getToken();


            } break;

        case LBRACE: { // found '{'
                jkmpListConstruction* vl=new jkmpListConstruction(this, NULL);
                res=vl;
                getToken();
                while ((CurrentToken != RBRACE)) {
                    JKMathParser::jkmpNode* parameter=logicalExpression(vl->getCount()>0);
                    vl->add(parameter);
                    if ((CurrentToken!=RBRACE)&&(CurrentToken!=COMMA)) {
                        jkmpError(JKMP::string("parsing primary (list construction node '{a,b,...}'): '}' or ',' expected, but '%1' found after %2 elements").arg(currenttokentostring()).arg(vl->getCount()));
                        return new jkmpInvalidNode(this, NULL);
                    }
                    //qDebug()<<"adding to [...]: "<<vl->getCount()<<". element CurrentToke="<<currenttokentostring();
                }


                if ( CurrentToken != RBRACE ) {
                    jkmpError(JKMP::string("parsing primary (list construction node '{a,b,...}'): '}' expected, but '%1' found").arg(currenttokentostring()));
                    return new jkmpInvalidNode(this, NULL);
                }
                getToken();


            } break;
        default:
            jkmpError(JKMP::_("parsing primary: primary expected, but '%1' found").arg(currenttokentostring()));
            if (res) delete res;
            return new jkmpInvalidNode(this, NULL);

	}

    /*if (CurrentToken==POWER) {
        res=(JKMathParser::jkmpNode*)new jkmpBinaryArithmeticNode('^', res, primary(true), this, NULL);
    }*/


    if (!res) res=new jkmpInvalidNode(this, NULL);

    return res;

}

double JKMathParser::readNumber()
{


  double dfactor=1;
  bool isNumber=true;


  int i=0;
  char c;

  if (getFromStream(program, c)) {
   // check sign
    if (c=='-') { dfactor=-1; isNumber=getFromStream(program, c); i++; }
    else if (c=='+') { isNumber=getFromStream(program, c); i++; }

    if (isNumber && c=='0') {
        isNumber=getFromStream(program, c); i++;
        if (c=='x' || c=='X') return dfactor*readHex();
        if (c=='o' || c=='O') return dfactor*readOct();
        if (c=='b' || c=='B') return dfactor*readBin();
    }
    putbackStream(program, c);
    return dfactor*readDec();

  }
  return 0;
}

double JKMathParser::readDec()
{

  double current_double=0;

  bool isMantissa=true;
  double dfactor=1;
  int mantissaPos=0;
  bool isNumber=true;
  bool foundDot=false;


  JKMP::string num="";
  int i=0;
  char c;

  if (getFromStream(program, c)) {
   // check sign
    if (c=='-') { dfactor=-1; isNumber=getFromStream(program, c); i++; }
    else if (c=='+') { isNumber=getFromStream(program, c); i++; }

    while (isNumber) {
      if (!isMantissa) {
        switch(c) {
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            num+=c;
            break;
          case '-':
            if (i==mantissaPos) {
              num+=c;
            } else {
              isNumber=false;
              putbackStream(program, c);
            }
            break;
          case '+':
            if (i!=mantissaPos) {
              isNumber=false;
              putbackStream(program, c);
            }
            break;
          default:
            putbackStream(program, c);
            isNumber=false;
        }
      }
      if (isMantissa) {
        switch(c) {
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            num+=c;
            break;
          case '.':
            if (foundDot) {
              putbackStream(program, c);
              isNumber=false;
            } else {
              num+=c;
              foundDot=true;
            }
            break;
          case '-':
            if (i==mantissaPos) {
              dfactor=-1;
            } else {
              isNumber=false;
              putbackStream(program, c);
            }
            break;
          case '+':
            if (i==mantissaPos) {
              dfactor=1;
            } else {
              putbackStream(program, c);
              isNumber=false;
            }
            break;
          case 'e':
          case 'E':
            isMantissa=false;
            num+='e';
            mantissaPos=i+1;
            break;

          default:
            putbackStream(program, c);
            isNumber=false;
        }
      }
      i++;
      if (isNumber) isNumber=getFromStream(program, c);
    }
  }

    if (num.length()<=0) num="0";
    current_double = JKMP::stringtofloat(num);
    current_double=(current_double)*dfactor;
    return current_double;
}

double JKMathParser::readHex() {
    double current_double=0;
    double dfactor=1;
    bool isNumber=true;

    JKMP::string num="";
    int i=0;
    char c;

    if (getFromStream(program, c)) {
        // check sign
        if (c=='-') { dfactor=-1; isNumber=getFromStream(program, c); i++; }
        else if (c=='+') { isNumber=getFromStream(program, c); i++; }

        while (isNumber) {
            switch(c) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case 'A':
                case 'a':
                case 'B':
                case 'b':
                case 'C':
                case 'c':
                case 'D':
                case 'd':
                case 'E':
                case 'e':
                case 'F':
                case 'f':
                    num+=c;
                    break;
                default:
                    if (std::isdigit(c, std::locale("C")) || std::isalpha(c, std::locale("C"))) {
                        jkmpError(JKMP::_("read_hex: found unexpected character '%1'").arg(JKMP::string(c)));
                        return 0;
                    }
                    putbackStream(program, c);
                    isNumber=false;
            }
            i++;
            if (isNumber) isNumber=getFromStream(program, c);
        }
    }

      if (num.length()<=0) num="0";
      current_double=JKMP::hextoint(num);
      current_double=(current_double)*dfactor;
      return current_double;
}

double JKMathParser::readOct() {
    double current_double=0;
    double dfactor=1;
    bool isNumber=true;

    JKMP::string num="";
    int i=0;
    char c;

    if (getFromStream(program, c)) {
        // check sign
        if (c=='-') { dfactor=-1; isNumber=getFromStream(program, c); i++; }
        else if (c=='+') { isNumber=getFromStream(program, c); i++; }

        while (isNumber) {
            switch(c) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                    num+=c;
                    break;
                case '8':
                case '9':
                    jkmpError(JKMP::_("read_oct: found unexpected digit '%1'").arg(JKMP::string(c)));
                    return 0;
                    break;

              default:
                  putbackStream(program, c);
                  isNumber=false;
                  break;
            }
            i++;
            if (isNumber) isNumber=getFromStream(program, c);
        }
    }

      if (num.length()<=0) num="0";
      current_double=JKMP::octtoint(num);
      current_double=(current_double)*dfactor;
      return current_double;
}

double JKMathParser::readBin() {

  double current_double=0;
  double dfactor=1;
  bool isNumber=true;

  JKMP::string num="";
  int i=0;
  char c;

  if (getFromStream(program, c)) {
      // check sign
      if (c=='-') { dfactor=-1; isNumber=getFromStream(program, c); i++; }
      else if (c=='+') { isNumber=getFromStream(program, c); i++; }

      while (isNumber) {
          switch(c) {
              case '0':
              case '1':
                  num+=c;
                  break;
              case '2':
              case '3':
              case '4':
              case '5':
              case '6':
              case '7':
              case '8':
              case '9':
                  jkmpError(JKMP::_("read_binary: found unexpected digit '%1'").arg(JKMP::string(c)));
                  break;
            default:
              putbackStream(program, c);
              isNumber=false;
              break;
          }
          i++;
          if (isNumber) isNumber=getFromStream(program, c);
      }
  }

    if (num.size()<=0) num="0";
    double fac=1;
    current_double=0;
    for (int i=num.size()-1; i>=0; i--) {
        if (num[i]=='1') current_double=current_double+fac;
        fac=fac*2.0;
    }
    current_double=(current_double)*dfactor;
    return current_double;
}

JKMP::string JKMathParser::readDelim(char delimiter){
    JKMP::string res="";
    char ch=0;

    while(getFromStream(program, ch)) {
		if (ch==delimiter ) {
            char ch1=peekStream(program);
		    if (ch1==delimiter) {
                    getFromStream(program, ch);
		            res=res+delimiter;
            } else {
                break;
            }
        } else if (ch=='\\')  {
            char ch1=peekStream(program);
            if (ch1=='\"') {
                getFromStream(program, ch);
                res=res+"\"";
            } else if (ch1=='\'') {
                getFromStream(program, ch);
                res=res+"\'";
            } else if (ch1=='\t') {
                getFromStream(program, ch);
                res=res+"\t";
            } else if (ch1=='\n') {
                getFromStream(program, ch);
                res=res+"\n";
            } else if (ch1=='\r') {
                getFromStream(program, ch);
                res=res+"\r";
            } else if (ch1=='\\') {
                getFromStream(program, ch);
                res=res+"\\";
            } else if (ch1=='/') {
                getFromStream(program, ch);
                res=res+"/";
            } else {
                //jkmpError(JKMP::_("read_delmited_string: found unexpected escape sequence '\\%1'").arg(JKMP::string(ch1)));
                //return res;
                //break;
                res=res+JKMP::string(ch)+JKMP::string(ch1);
                getFromStream(program, ch);
            }
        } else if ((program) && (ch!=delimiter)) res=res+ch;
	}

	return res;
}









JKMathParser::jkmpUnaryNode::jkmpUnaryNode(char op, JKMathParser::jkmpNode* c, JKMathParser* p, JKMathParser::jkmpNode* par):
    jkmpNode(p, par)
{
  child=c;
  child->setParent(this);
  setParser(p);
  setParent(par);
  operation=op;
}

JKMathParser::jkmpUnaryNode::~jkmpUnaryNode()
{
    if (child) delete child;
}


void JKMathParser::jkmpUnaryNode::evaluate(jkmpResult &res)
{
    //jkmpResult c;
    child->evaluate(res);

    switch(operation) {
      case '!':
            if (res.type==jkmpBool) {
              res.boolean=!res.boolean;
              return ;
            } else if (res.type==jkmpBoolVector) {
                for (size_t i=0; i<res.boolVec.size(); i++) res.boolVec[i]=!res.boolVec[i];
                return ;
             } else parser->jkmpError(JKMP::_("'!' only defined for bool (argument type was %1)").arg(res.typeName()));
           break;
        case '-':
            if (res.type==jkmpDouble) {
                res.num=-res.num;
                return ;
            } else if (res.type==jkmpDoubleVector) {
                //res.setDoubleVec(res.numVec.size());
                for (size_t i=0; i<res.numVec.size(); i++) res.numVec[i]=-res.numVec[i];
                return ;
             } else parser->jkmpError(JKMP::_("'-' only defined for numbers (argument type was %1)").arg(res.typeName()));
             break;
        case '~':
             if (res.isInteger()) {
                 res.num=~res.toInteger();
                 return ;
             } else if (res.type==jkmpDoubleVector) {
                 for (size_t i=0; i<res.numVec.size(); i++) res.numVec[i]=double(~int32_t(res.numVec[i]));
                 return ;
             } else parser->jkmpError(JKMP::_("'~' only defined for integer numbers (argument type was %1)").arg(res.typeName()));
             break;
       default: parser->jkmpError(JKMP::_("unknown unary operation '%1'").arg(operation));
    }
    res.setInvalid();
}

JKMathParser::jkmpNode *JKMathParser::jkmpUnaryNode::copy(JKMathParser::jkmpNode *par)
{
    JKMathParser::jkmpNode *n= new JKMathParser::jkmpUnaryNode(operation, child->copy(), getParser(), par);
    return n;
}

bool JKMathParser::jkmpUnaryNode::createByteCode(JKMathParser::ByteCodeProgram &program, JKMathParser::ByteCodeEnvironment *environment)
{
    bool ok=true;
    if (child) ok=ok&&child->createByteCode(program, environment);

    if (!ok) return false;

    switch(operation) {
        case '!':
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicNot));
            break;
        case '-':
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcNeg));
            break;
        case '~':
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcBitNot));
            break;


        default:
            parser->jkmpError(JKMP::_("unknown unary operation"));
            ok=false;
            break;

    }
    return ok;
}

JKMP::string JKMathParser::jkmpUnaryNode::print() const
{
    return JKMP::string("%1(%2)").arg(JKMP::string(operation)).arg(child->print());
}

JKMP::string JKMathParser::jkmpUnaryNode::printTree(int level) const
{
    return JKMP::string(2*level, char(' '))+JKMP::string("UnaryNode %2\n%1").arg(child->printTree(level+1)).arg(JKMP::string(operation));
}







JKMathParser::jkmpBinaryArithmeticNode::jkmpBinaryArithmeticNode(char op, JKMathParser::jkmpNode* l, JKMathParser::jkmpNode* r, JKMathParser* p, JKMathParser::jkmpNode* par):
    jkmpNode(p, par)
{
  left=l;
  right=r;
  if (left) left->setParent(this);
  if (right) right->setParent(this);
  setParser(p);
  setParent(par);
  operation=op;
}

JKMathParser::jkmpBinaryArithmeticNode::~jkmpBinaryArithmeticNode()
{
    if (left) delete left;
    if (right) delete right;
}



void JKMathParser::jkmpBinaryArithmeticNode::evaluate(jkmpResult& res){
  jkmpResult l;
  if (left) left->evaluate(l);
  jkmpResult r;
  if (right) right->evaluate(r);
  //jkmpResult res;

  switch(operation) {
      case '+':
            jkmpResult::add(res, l, r, getParser());
          break;
      case '-':
            jkmpResult::sub(res, l, r, getParser());
          break;
      case '*':
            jkmpResult::mul(res, l, r, getParser());
          break;
      case '/':
            jkmpResult::div(res, l, r, getParser());
          break;
      case '%':
            jkmpResult::mod(res, l, r, getParser());
          break;
      case '^':
          jkmpResult::power(res, l, r, getParser());
          break;
      case '&':
          jkmpResult::bitwiseand(res, l, r, getParser());
          break;
      case '|':
          jkmpResult::bitwiseor(res, l, r, getParser());
          break;

      default:
          parser->jkmpError(JKMP::_("unknown arithmetic operation"));
          res.setInvalid();
          break;

  }
}

JKMathParser::jkmpNode *JKMathParser::jkmpBinaryArithmeticNode::copy(JKMathParser::jkmpNode *par)
{
    JKMathParser::jkmpNode *n= new JKMathParser::jkmpBinaryArithmeticNode(operation, left->copy(), right->copy(), getParser(), par);
    return n;
}

bool JKMathParser::jkmpBinaryArithmeticNode::createByteCode(JKMathParser::ByteCodeProgram &program, ByteCodeEnvironment *environment)
{
    bool ok=true;
    if (right) ok=ok&&right->createByteCode(program, environment);
    if (left) ok=ok&&left->createByteCode(program, environment);

    if (!ok) return false;

    switch(operation) {
        case '+':
            program.push_back(JKMathParser::ByteCodeInstruction(bcAdd));
            break;
        case '-':
            program.push_back(JKMathParser::ByteCodeInstruction(bcSub));
            break;
        case '*':
            program.push_back(JKMathParser::ByteCodeInstruction(bcMul));
            break;
        case '/':
            program.push_back(JKMathParser::ByteCodeInstruction(bcDiv));
            break;
        case '%':
            program.push_back(JKMathParser::ByteCodeInstruction(bcMod));
            break;
        case '^':
            program.push_back(JKMathParser::ByteCodeInstruction(bcPow));
            break;
        case '&':
            program.push_back(JKMathParser::ByteCodeInstruction(bcBitAnd));
            break;
        case '|':
            program.push_back(JKMathParser::ByteCodeInstruction(bcBitOr));
            break;

        default:
            parser->jkmpError(JKMP::_("unknown arithmetic operation"));
            ok=false;
            break;

    }
    return ok;
}

JKMP::string JKMathParser::jkmpBinaryArithmeticNode::print() const
{
    return JKMP::string("(%1) %2 (%3)").arg(left->print()).arg(JKMP::string(operation)).arg(right->print());
}

JKMP::string JKMathParser::jkmpBinaryArithmeticNode::printTree(int level) const
{
    return JKMP::string(2*level, char(' '))+JKMP::string("BinaryArithmeticNode %2\n%1\n%3").arg(left->printTree(level+1)).arg(JKMP::string(operation)).arg(right->printTree(level+1));
}







JKMathParser::jkmpCompareNode::jkmpCompareNode(char op, JKMathParser::jkmpNode* l, JKMathParser::jkmpNode* r, JKMathParser* p, JKMathParser::jkmpNode* par):
    jkmpNode(p, par)
{
  left=l;
  right=r;
  if (left) left->setParent(this);
  if (right) right->setParent(this);
  setParser(p);
  setParent(par);
  operation=op;
}

JKMathParser::jkmpCompareNode::~jkmpCompareNode()
{
    if (left) delete left;
    if (right) delete right;
}


void JKMathParser::jkmpCompareNode::evaluate(jkmpResult &res)
{
    jkmpResult l;
    if (left) left->evaluate(l);
    jkmpResult r;
    if (right) right->evaluate(r);

  /*if (l.type!=r.type) {
      parser->jkmpError(JKMP::_("you can't compare different datatypes"));
      res.setInvalid();
      return;
  }*/

  res.type=jkmpBool;
  switch(operation) {
        case jkmpCOMPequal:
            jkmpResult::compareequal(res, l, r, getParser());
            break;
        case jkmpCOMPnequal:
              jkmpResult::comparenotequal(res, l, r, getParser());
              break;
        case jkmpCOMPgreater:
              jkmpResult::comparegreater(res, l, r, getParser());
              break;

        case jkmpCOMPlesser:
              jkmpResult::comparesmaller(res, l, r, getParser());
              break;

        case jkmpCOMPgreaterequal:
              jkmpResult::comparegreaterequal(res, l, r, getParser());
              break;

        case jkmpCOMPlesserequal:
              jkmpResult::comparesmallerequal(res, l, r, getParser());
              break;


        default:
              parser->jkmpError(JKMP::_("compare operation between %1 and %2 not possible").arg(l.toTypeString()).arg(r.toTypeString()));
              res.setInvalid();
              break;
    }
}

JKMathParser::jkmpNode *JKMathParser::jkmpCompareNode::copy(JKMathParser::jkmpNode *par)
{
    JKMathParser::jkmpNode *n= new JKMathParser::jkmpCompareNode(operation, left->copy(), right->copy(), getParser(), par);
    return n;
}

bool JKMathParser::jkmpCompareNode::createByteCode(JKMathParser::ByteCodeProgram &program, JKMathParser::ByteCodeEnvironment *environment)
{
    bool ok=true;
    if (right) ok=ok&&right->createByteCode(program, environment);
    if (left) ok=ok&&left->createByteCode(program, environment);

    if (!ok) return false;

    switch(operation) {
        case jkmpCOMPequal:
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcCmpEqual));
            break;
        case jkmpCOMPnequal:
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcCmpEqual));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicNot));
            break;
        case jkmpCOMPlesser:
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcCmpLesser));
            break;
        case jkmpCOMPlesserequal:
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcCmpLesserEqual));
            break;
        case jkmpCOMPgreater:
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcCmpLesserEqual));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicNot));
            break;
        case jkmpCOMPgreaterequal:
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcCmpLesser));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicNot));
            break;

        default:
            parser->jkmpError(JKMP::_("unknown compare operation"));
            ok=false;
            break;

    }
    return ok;
}

JKMP::string JKMathParser::jkmpCompareNode::print() const
{
    return JKMP::string("(%1) %2 (%3)").arg(left->print()).arg(JKMP::string(opAsString())).arg(right->print());
}

JKMP::string JKMathParser::jkmpCompareNode::printTree(int level) const
{
    return JKMP::string(2*level, char(' '))+JKMP::string("CompareNode %2\n%1\n%3").arg(left->printTree(level+1)).arg(opAsString()).arg(right->printTree(level+1));
}

JKMP::string JKMathParser::jkmpCompareNode::opAsString() const
{
    switch(operation) {
        case jkmpCOMPequal:
            return "==";
            break;
        case jkmpCOMPnequal:
            return "!=";
            break;
        case jkmpCOMPlesser:
            return "<";
            break;
        case jkmpCOMPlesserequal:
            return "<=";
            break;
        case jkmpCOMPgreater:
            return ">";
            break;
        case jkmpCOMPgreaterequal:
            return ">=";
            break;

        default:
            break;

    }
    return JKMP::string("???");
}







JKMathParser::jkmpBinaryBoolNode::jkmpBinaryBoolNode(char op, JKMathParser::jkmpNode* l, JKMathParser::jkmpNode* r, JKMathParser* p, JKMathParser::jkmpNode* par):
    jkmpNode(p, par)
{
  left=l;
  right=r;
  if (left) left->setParent(this);
  if (right) right->setParent(this);
  setParser(p);
  setParent(par);
  operation=op;
}

JKMathParser::jkmpBinaryBoolNode::~jkmpBinaryBoolNode()
{
    if (left) delete left;
    if (right) delete right;
}


void JKMathParser::jkmpBinaryBoolNode::evaluate(jkmpResult &res)
{
    jkmpResult l;
    if (left) left->evaluate(l);
    jkmpResult r;
    if (right) right->evaluate(r);

  /*if ((l.type!=jkmpBool)||(r.type!=jkmpBool)) {
      res.setInvalid();
      parser->jkmpError(JKMP::_("logical operations only for bool"));
  } else {*/
      switch(operation) {
          case jkmpLOPand:
              jkmpResult::logicand(res, l, r, getParser());
              break;
          case jkmpLOPor:
              jkmpResult::logicor(res, l, r, getParser());
              break;
          case jkmpLOPnor:
              jkmpResult::logicnor(res, l, r, getParser());
              break;
          case jkmpLOPxor:
              jkmpResult::logicxor(res, l, r, getParser());
              break;
          case jkmpLOPnand:
              jkmpResult::logicnand(res, l, r, getParser());
              break;


          default: parser->jkmpError(JKMP::_("unknown error"));
              res.setInvalid();
              break;
      }
  //}
}

JKMathParser::jkmpNode *JKMathParser::jkmpBinaryBoolNode::copy(JKMathParser::jkmpNode *par)
{
    JKMathParser::jkmpNode *n= new JKMathParser::jkmpBinaryBoolNode(operation, left->copy(), right->copy(), getParser(), par);
    return n;
}

bool JKMathParser::jkmpBinaryBoolNode::createByteCode(JKMathParser::ByteCodeProgram &program, JKMathParser::ByteCodeEnvironment *environment)
{
    bool ok=true;
    if (right) ok=ok&&right->createByteCode(program, environment);
    if (left) ok=ok&&left->createByteCode(program, environment);

    if (!ok) return false;

    switch(operation) {
        case jkmpLOPand:
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicAnd));
            break;
        case jkmpLOPor:
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicOr));
            break;
        case jkmpLOPxor:
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicXor));
            break;
        case jkmpLOPnor:
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicOr));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicNot));
            break;
        case jkmpLOPnand:
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicAnd));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicNot));
            break;

        default:
            parser->jkmpError(JKMP::_("unknown logic operation"));
            ok=false;
            break;

    }
    return ok;
}

JKMP::string JKMathParser::jkmpBinaryBoolNode::print() const
{
    return JKMP::string("(%1) %2 (%3)").arg(left->print()).arg(JKMP::string(opAsString())).arg(right->print());
}

JKMP::string JKMathParser::jkmpBinaryBoolNode::printTree(int level) const
{
    return JKMP::string(2*level, char(' '))+JKMP::string("BinaryBoolNode %2\n%1\n%3").arg(left->printTree(level+1)).arg(opAsString()).arg(right->printTree(level+1));
}

JKMP::string JKMathParser::jkmpBinaryBoolNode::opAsString() const
{
    switch(operation) {
        case jkmpLOPand:
            return "and";
            break;
        case jkmpLOPor:
            return "or";
            break;
        case jkmpLOPxor:
            return "xor";
            break;
        case jkmpLOPnor:
            return "nor";
            break;
        case jkmpLOPnand:
            return "nand";
            break;
        default:
            break;

    }
    return JKMP::string("???");
}




JKMathParser::jkmpVariableNode::jkmpVariableNode(JKMP::string name, JKMathParser* p, JKMathParser::jkmpNode* par):
    jkmpNode(p, par)
 {
  var=name;
  setParser(p);
  setParent(par);
};

jkmpResult JKMathParser::jkmpVariableNode::evaluate() {
    return getParser()->getVariable(var);
}

void JKMathParser::jkmpVariableNode::evaluate(jkmpResult &result)
{
    result=getParser()->getVariable(var);
}

JKMathParser::jkmpNode *JKMathParser::jkmpVariableNode::copy(JKMathParser::jkmpNode *par)
{
    return new JKMathParser::jkmpVariableNode(var, getParser(), par);
}

bool JKMathParser::jkmpVariableNode::createByteCode(JKMathParser::ByteCodeProgram &program, JKMathParser::ByteCodeEnvironment* environment)
{
    JKMathParser::jkmpVariable def;

    if (environment->heapVariables.contains(var) && environment->heapVariables[var].size()>0) {
        if (environment->heapVariables[var].back()>=0) {
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapRead, environment->heapVariables[var].back()));
            return true;
        } else {
            getParser()->jkmpError(JKMP::_("heap-adress-error, tried to access heap item %1").arg(environment->heapVariables[var].back()));
            return false;
        }
    } else if (getParser()->environment.getVariableDef(var, def)) {
        int level=getParser()->environment.getVariableLevel(var);
        if (level>0) {
            getParser()->jkmpError(JKMP::_("only top-level variables allowed in byte-coded expressionen (variable '%1', level %2)").arg(var).arg(level));
            return false;
        }
        if (def.getType()==jkmpDouble) {
            double* varP=def.getNum();
            if (varP) {
                program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcVarRead, varP));
                return true;
            } else {
                getParser()->jkmpError(JKMP::_("variable '%1' points to NULL").arg(var));
                return false;
            }
        } else {
            getParser()->jkmpError(JKMP::_("only number-valued variables allowed in byte-coded expressions (variable '%1')").arg(var));
            return false;
        }
    } else {
        getParser()->jkmpError(JKMP::_("variable '%1' not found").arg(var));
        return false;
    }

}

JKMP::string JKMathParser::jkmpVariableNode::print() const
{
    return var;

}

JKMP::string JKMathParser::jkmpVariableNode::printTree(int level) const
{
    return JKMP::string(2*level, char(' '))+JKMP::string("VariableNode '%1'").arg(var);
}




void JKMathParser::jkmpNodeList::add(JKMathParser::jkmpNode* n){
    list.push_back(n);
}

JKMathParser::jkmpNode *JKMathParser::jkmpNodeList::popFirst(bool deleteObject)
{
    if (list.size()>0) {
        if (deleteObject) {
            delete list.front();
            list.erase(list.begin());
        } else {
            JKMathParser::jkmpNode * n=list.front();
            list.erase(list.begin());
            return n;
        }

    }
    return NULL;
}

JKMathParser::jkmpNode *JKMathParser::jkmpNodeList::popLast(bool deleteObject)
{
    if (list.size()>0) {
        if (deleteObject) {
            delete list.back();
            list.pop_back();
        } else {
            JKMathParser::jkmpNode * n=list.back();
            list.pop_back();
            return n;
        }

    }
    return NULL;
}



void JKMathParser::jkmpNodeList::evaluate(jkmpResult &res)
{
    int n=getCount();
    //qDebug()<<"evaluating() nodelist with "<<n<<" items";
    if (n>0) {
       for (int i=0; i<n; i++) {
          res.setInvalid();
          if (list[i]) list[i]->evaluate(res);
          //qDebug()<<"eval nodelist(), item"<<i+1<<"/"<<n<<":  "<<res.toTypeString();
       }
       return ;
    } else parser->jkmpError(JKMP::_("NodeList empty"));
    res.setInvalid();
}

void JKMathParser::jkmpNodeList::evaluate(JKMP::vector<jkmpResult> &result)
{
    int n=getCount();
    result.clear();
    //qDebug()<<"evaluating() nodelist with "<<n<<" items";
    if (n>0) {
       for (int i=0; i<n; i++) {
          if (list[i]) {
              jkmpResult res;
              list[i]->evaluate(res);
              result.push_back(res);
          }
          //qDebug()<<"eval nodelist(), item"<<i+1<<"/"<<n<<":  "<<res.toTypeString();
       }
       return ;
    } else parser->jkmpError(JKMP::_("NodeList empty"));
    result.clear();

}

JKMathParser::jkmpNode *JKMathParser::jkmpNodeList::copy(JKMathParser::jkmpNode *par)
{

    JKMathParser::jkmpNodeList* n= new JKMathParser::jkmpNodeList(getParser());
    if (list.size()>0) {
        for (size_t i=0; i<list.size(); i++) {
            n->add(list[i]->copy(n));
        }
    }

    n->setParent(par);
    return n;
}

bool JKMathParser::jkmpNodeList::createByteCode(JKMathParser::ByteCodeProgram &program, JKMathParser::ByteCodeEnvironment *environment)
{
    bool ok=true;
    for (size_t i=0; (ok&&i<list.size()); i++) {
        if (list[i]) {
            ok=ok&&list[i]->createByteCode(program, environment);
        }
    }

    return ok;
}

JKMP::string JKMathParser::jkmpNodeList::print() const
{
    JKMP::stringVector sl;
    if (list.size()>0) {
        for (size_t i=0; i<list.size(); i++) {
            sl<<list[i]->print();
        }
    }
    return sl.join("; ");
    //return JKMP::string("%1[%3] = %2").arg(variable).arg(child->print()).arg(index->print());
}

JKMP::string JKMathParser::jkmpNodeList::printTree(int level) const
{
    JKMP::stringVector sl;
    if (list.size()>0) {
        for (size_t i=0; i<list.size(); i++) {
            sl<<list[i]->printTree(level+1);
        }
    }
    return JKMP::string(2*level, char(' '))+JKMP::string("NodeList\n%1").arg(sl.join("\n"));
}

JKMathParser::jkmpNodeList::jkmpNodeList(JKMathParser *p, JKMathParser::jkmpNode *par):
    jkmpNode(p, par)
{
}

JKMathParser::jkmpNodeList::~jkmpNodeList() {

    if (list.size()>0) for (size_t i=0; i<list.size(); i++) {
        if (list[i]) delete list[i];
    }
    list.clear();
}


JKMathParser::jkmpVariableAssignNode::~jkmpVariableAssignNode()
{
    if (child) delete child;
}

JKMathParser::jkmpVariableAssignNode::jkmpVariableAssignNode(JKMP::string var, JKMathParser::jkmpNode* c, JKMathParser* p, JKMathParser::jkmpNode* par):
    jkmpNode(p, par)
{
  child=c;
  if (child) child->setParent(this);
  setParser(p);
  setParent(par);
  variable=var;
//  std::cout<<"assign: "<<var<<std::endl;
}



void JKMathParser::jkmpVariableAssignNode::evaluate(jkmpResult &result)
{
    if (child) child->evaluate(result);
    getParser()->setVariable(variable, result);
}

JKMathParser::jkmpNode *JKMathParser::jkmpVariableAssignNode::copy(JKMathParser::jkmpNode *par)
{
    if (child) return new JKMathParser::jkmpVariableAssignNode(variable, child->copy(NULL), getParser(), par);
    else return new JKMathParser::jkmpVariableAssignNode(variable, NULL, getParser(), par);
}

bool JKMathParser::jkmpVariableAssignNode::createByteCode(JKMathParser::ByteCodeProgram &program, JKMathParser::ByteCodeEnvironment *environment)
{
    JKMathParser::jkmpVariable def;

    bool ok=child->createByteCode(program, environment);

    if (ok) {
        if (environment->heapVariables.contains(variable) && environment->heapVariables[variable].size()>0) {
            if (environment->heapVariables[variable].back()>=0) {
                program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapWrite, environment->heapVariables[variable].back()));
                program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapRead, environment->heapVariables[variable].back()));
                return true;
            } else {
                getParser()->jkmpError(JKMP::_("heap-adress-error, tried to access heap item %1").arg(environment->heapVariables[variable].back()));
                return false;
            }
        } else if (getParser()->environment.getVariableDef(variable, def)) {
            int level=getParser()->environment.getVariableLevel(variable);
            if (level>0) {
                getParser()->jkmpError(JKMP::_("only top-level variables allowed in byte-coded expressionen (variable '%1', level %2)").arg(variable).arg(level));
                return false;
            }
            double* varP=NULL;
            if (def.getType()==jkmpDouble) {
                varP=def.getNum();
                if (varP) {
                    program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcVarWrite, varP));
                    program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcVarRead, varP));
                    return true;
                } else {
                    getParser()->jkmpError(JKMP::_("variable '%1' points to NULL").arg(variable));
                    return false;
                }
            } else {
                getParser()->jkmpError(JKMP::_("only number-values variables allowed in byte-coded expressions (variable '%1')").arg(variable));
                return false;
            }
        } else {
            /*getParser()->jkmpError(JKMP::_("variable '%1' not found").arg(var));
            return false;*/
            int varIdx=environment->pushVar(variable);
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapWrite, varIdx));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapRead, varIdx));
            return true;
        }
    }
    return ok;
}

JKMP::string JKMathParser::jkmpVariableAssignNode::print() const
{
    return JKMP::string("%1 = %2").arg(variable).arg(child->print());

}

JKMP::string JKMathParser::jkmpVariableAssignNode::printTree(int level) const
{
    return JKMP::string(2*level, char(' '))+JKMP::string("VariableAssigneNode '%2'\n%1").arg(child->printTree(level+1)).arg(variable);
}

JKMathParser::jkmpFunctionNode::jkmpFunctionNode(JKMP::string name, JKMP::vector<jkmpNode *> params, JKMathParser *p, jkmpNode *par):
    jkmpNode(p, par)
 {
  child=params;
  fun=name;
  setParser(p);
  setParent(par);
  if (child.size()>0) {
    for (size_t i=0; i<child.size(); i++) {
      if (child[i]) child[i]->setParent(this);
    }
  }
}

void JKMathParser::jkmpFunctionNode::evaluate(jkmpResult &result)
{
    /*JKMP::vector<jkmpResult> data(child.size(), JKMathParser::getInvalidResult());
    for (int i=0; i<child.size(); i++) {
        if (child[i]) child[i]->evaluate(data[i]);
    }*/
    result=parser->evaluateFunction(fun, child);
}

JKMathParser::jkmpNode *JKMathParser::jkmpFunctionNode::copy(JKMathParser::jkmpNode *par)
{
    JKMP::vector<jkmpNode *> params;
    if (child.size()>0) {
        for (size_t i=0; i<child.size(); i++) {
            params<<child[i]->copy(NULL);
        }
    }

    return new JKMathParser::jkmpFunctionNode(fun, params, getParser(), par);
}

bool JKMathParser::jkmpFunctionNode::createByteCode(JKMathParser::ByteCodeProgram &program, ByteCodeEnvironment *environment)
{
    bool ok=true;
    int params=0;
    for (int i=child.size()-1; i>=0; i--) {
        if (child[i]) {
            ok=ok&&child[i]->createByteCode(program, environment);
            params++;
        }
    }
    if (ok) {
        JKMathParser::jkmpFunctionDescriptor def;
        if (environment->functionDefs.contains(fun) && environment->functionDefs[fun].second) {
            if (environment->inFunctionCalls.find(fun)!=environment->inFunctionCalls.end()) {
                getParser()->jkmpError(JKMP::_("no recursive function calls allowed in byte-code (function '%1')").arg(fun));
                return false;
            } else if (environment->functionDefs[fun].first.size()==params) {
                for (int i=0; i<params; i++) {
                    int varID=environment->pushVar(environment->functionDefs[fun].first[i]);
                    program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapWrite, varID));
                }

                environment->inFunctionCalls.insert(fun);
                ok=ok&&environment->functionDefs[fun].second->createByteCode(program, environment);
                environment->inFunctionCalls.erase(fun);
                for (int i=0; i<params; i++) {
                    environment->popVar(environment->functionDefs[fun].first[i]);
                }
                return ok;
            } else {
                getParser()->jkmpError(JKMP::_("function '%1' defined with wrong number of parameters (required: %2, in definition: %3)").arg(fun).arg(params).arg(environment->functionDefs[fun].first.size()));
                return false;
            }
        } else if (getParser()->environment.getFunctionDef(fun, def)) {
            int level=getParser()->environment.getFunctionLevel(fun);
            if (level>0) {
                getParser()->jkmpError(JKMP::_("only top-level functions allowed in byte-coded expressionen (function '%1', level %2)").arg(fun).arg(level));
                return false;
            }
            jkmpFunctiontype ft=getParser()->environment.getFunctionType(fun);
            if (ft==functionFromNode || ft==functionInvalid) {
                getParser()->jkmpError(JKMP::_("only simple functions allowed in byte-coded expressionen (function '%1')").arg(fun));
                return false;
            }
            if ((!def.simpleFuncPointer.contains(params) && !def.simpleFuncPointer.contains(100+params)) || (!def.simpleFuncPointer[params] && !def.simpleFuncPointer[100+params])) {
                program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcCallResultFunction, fun, params));
                //getParser()->jkmpError(JKMP::_("no implementation of function '%1(...)' with %2 parameters found").arg(fun).arg(params));
                return true;
            }
            if (def.simpleFuncPointer.contains(params)) {
                void* fp=def.simpleFuncPointer[params];
                program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcCallCFunction, fp, params));
                return true;
            } else {
                void* fp=def.simpleFuncPointer[params+100];
                program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcCallCMPFunction, fp, params));
                return true;
            }
        }
    }
    return false;
}

JKMP::string JKMathParser::jkmpFunctionNode::print() const
{
    JKMP::stringVector sl;
    for (size_t i=0; i<child.size(); i++) {
        sl<<child[i]->print();
    }
    return JKMP::string("%1(%2)").arg(fun).arg(sl.join(", "));
}

JKMP::string JKMathParser::jkmpFunctionNode::printTree(int level) const
{
    JKMP::stringVector sl;
    if (child.size()>0) {
        for (size_t i=0; i<child.size(); i++) {
            sl<<child[i]->printTree(level+1);
        }
    }
    if (child.size()>0) return JKMP::string(2*level, char(' '))+JKMP::string("FunctionNode '%1(...)'\n%2").arg(fun).arg(sl.join("\n"));
    else return JKMP::string(2*level, char(' '))+JKMP::string("FunctionNode '%1()'").arg(fun);
}


JKMathParser::jkmpFunctionNode::~jkmpFunctionNode() {

    if (child.size()>0) {
      for (size_t i=0; i<child.size(); i++) {
        if (child[i]) delete child[i];
      }
      child.clear();
    }
}



JKMathParser::jkmpVariable::jkmpVariable()
{
    type=jkmpVoid;     /*!< \brief type of the variable */
    internal=false;           /*!< \brief this is an internal variable */
    str=NULL;        /*!< \brief this points to the variable data if \c type==jkmpString */
    num=NULL;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
    boolean=NULL;
    numVec=NULL;
    boolVec=NULL;
    strVec=NULL;
    structData=NULL;
    listData=NULL;
    matrix_columns=NULL;
}

JKMathParser::jkmpVariable::jkmpVariable(double *ref)
{
    type=jkmpDouble;     /*!< \brief type of the variable */
    internal=false;           /*!< \brief this is an internal variable */
    str=NULL;        /*!< \brief this points to the variable data if \c type==jkmpString */
    num=ref;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
    boolean=NULL;
    numVec=NULL;
    boolVec=NULL;
    strVec=NULL;
    structData=NULL;
    listData=NULL;
    matrix_columns=NULL;
}

JKMathParser::jkmpVariable::jkmpVariable(JKMP::string *ref)
{
    type=jkmpString;     /*!< \brief type of the variable */
    internal=false;           /*!< \brief this is an internal variable */
    str=ref;        /*!< \brief this points to the variable data if \c type==jkmpString */
    num=NULL;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
    boolean=NULL;
    numVec=NULL;
    boolVec=NULL;
    strVec=NULL;
    structData=NULL;
    listData=NULL;
    matrix_columns=NULL;
}

JKMathParser::jkmpVariable::jkmpVariable(bool *ref)
{
    type=jkmpBool;     /*!< \brief type of the variable */
    internal=false;           /*!< \brief this is an internal variable */
    str=NULL;        /*!< \brief this points to the variable data if \c type==jkmpString */
    num=NULL;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
    boolean=ref;
    numVec=NULL;
    boolVec=NULL;
    strVec=NULL;
    structData=NULL;
    listData=NULL;
    matrix_columns=NULL;
}

JKMathParser::jkmpVariable::jkmpVariable(JKMP::vector<double> *ref)
{
    type=jkmpDoubleVector;     /*!< \brief type of the variable */
    internal=false;           /*!< \brief this is an internal variable */
    str=NULL;        /*!< \brief this points to the variable data if \c type==jkmpString */
    num=NULL;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
    boolean=NULL;
    numVec=ref;
    boolVec=NULL;
    strVec=NULL;
    structData=NULL;
    listData=NULL;
    matrix_columns=NULL;
}

JKMathParser::jkmpVariable::jkmpVariable(JKMP::vector<bool> *ref)
{
    type=jkmpBoolVector;     /*!< \brief type of the variable */
    internal=false;           /*!< \brief this is an internal variable */
    str=NULL;        /*!< \brief this points to the variable data if \c type==jkmpString */
    num=NULL;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
    boolean=NULL;
    numVec=NULL;
    boolVec=ref;
    strVec=NULL;
    structData=NULL;
    listData=NULL;
    matrix_columns=NULL;
}

JKMathParser::jkmpVariable::jkmpVariable(JKMP::vector<double> *ref, int *columnref)
{
    type=jkmpDoubleMatrix;     /*!< \brief type of the variable */
    internal=false;           /*!< \brief this is an internal variable */
    str=NULL;        /*!< \brief this points to the variable data if \c type==jkmpString */
    num=NULL;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
    boolean=NULL;
    numVec=ref;
    boolVec=NULL;
    strVec=NULL;
    structData=NULL;
    listData=NULL;
    matrix_columns=columnref;

}

JKMathParser::jkmpVariable::jkmpVariable(JKMP::vector<bool> *ref, int *columnref)
{
    type=jkmpBoolMatrix;     /*!< \brief type of the variable */
    internal=false;           /*!< \brief this is an internal variable */
    str=NULL;        /*!< \brief this points to the variable data if \c type==jkmpString */
    num=NULL;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
    boolean=NULL;
    numVec=NULL;
    boolVec=ref;
    strVec=NULL;
    structData=NULL;
    listData=NULL;
    matrix_columns=columnref;
}

JKMathParser::jkmpVariable::jkmpVariable(JKMP::stringVector *ref)
{
    type=jkmpStringVector;     /*!< \brief type of the variable */
    internal=false;           /*!< \brief this is an internal variable */
    str=NULL;        /*!< \brief this points to the variable data if \c type==jkmpString */
    num=NULL;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
    boolean=NULL;
    numVec=NULL;
    boolVec=NULL;
    strVec=ref;
    structData=NULL;
    listData=NULL;
    matrix_columns=NULL;
}

JKMathParser::jkmpVariable::jkmpVariable(JKMP::map<JKMP::string, jkmpResult> *ref)
{
    type=jkmpStruct;     /*!< \brief type of the variable */
    internal=false;           /*!< \brief this is an internal variable */
    str=NULL;        /*!< \brief this points to the variable data if \c type==jkmpString */
    num=NULL;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
    boolean=NULL;
    numVec=NULL;
    boolVec=NULL;
    strVec=NULL;
    structData=ref;
    listData=NULL;
    matrix_columns=NULL;
}

JKMathParser::jkmpVariable::jkmpVariable(JKMP::vector<jkmpResult> *ref)
{
    type=jkmpList;     /*!< \brief type of the variable */
    internal=false;           /*!< \brief this is an internal variable */
    str=NULL;        /*!< \brief this points to the variable data if \c type==jkmpString */
    num=NULL;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
    boolean=NULL;
    numVec=NULL;
    boolVec=NULL;
    strVec=NULL;
    structData=NULL;
    listData=ref;
    matrix_columns=NULL;

}


JKMathParser::jkmpVariable::~jkmpVariable()
{

}

void JKMathParser::jkmpVariable::clearMemory()
{
    if (internal) {
        if (type==jkmpDouble && num) delete (num);
        if (type==jkmpBool && boolean) delete (boolean);
        if (type==jkmpString && str) delete (str);
        if (type==jkmpDoubleVector && numVec) delete (numVec);
        if (type==jkmpDoubleMatrix) {
            if (numVec) delete (numVec);
            if (matrix_columns) delete (matrix_columns);
        }
        if (type==jkmpBoolMatrix) {
            if (boolVec) delete (boolVec);
            if (matrix_columns) delete (matrix_columns);
        }
        if (type==jkmpBoolVector && boolVec) delete (boolVec);
        if (type==jkmpStringVector && strVec) delete (strVec);
        if (type==jkmpStruct && structData) delete (structData);
        if (type==jkmpList && listData) delete (listData);
        num=NULL;
        boolean=NULL;
        str=NULL;
        numVec=NULL;
        boolVec=NULL;
        strVec=NULL;
        structData=NULL;
        listData=NULL;
    }
    internal=false;
}

jkmpResult JKMathParser::jkmpVariable::toResult() const
{
    jkmpResult r;
    toResult(r);
    return r;
}

void JKMathParser::jkmpVariable::toResult(jkmpResult &r) const
{
    r.setInvalid();
    r.type=type;
    r.isValid=true;
    if (type==jkmpDouble && num) {
        r.num=*(num);
    } else if (type==jkmpString && str) {
        r.str=*(str);
    } else if (type==jkmpBool && boolean) {
        r.boolean=*(boolean);
    } else if (type==jkmpDoubleVector && numVec) {
        r.numVec=*(numVec);
    } else if (type==jkmpBoolVector && boolVec) {
        r.boolVec=*(boolVec);
    } else if (type==jkmpBoolMatrix && boolVec && matrix_columns) {
        r.boolVec=*(boolVec);
        r.matrix_columns=*matrix_columns;
    } else if (type==jkmpDoubleMatrix && numVec && matrix_columns) {
        r.numVec=*(numVec);
        r.matrix_columns=*matrix_columns;
    } else if (type==jkmpStringVector && strVec) {
        r.strVec=*(strVec);
    } else if (type==jkmpStruct && structData) {
        r.structData=*(structData);
    } else if (type==jkmpList && listData) {
        r.listData=*(listData);
    } else {
        r.isValid=false;
    }

}

bool JKMathParser::jkmpVariable::isInternal() const
{
    return internal;
}

void JKMathParser::jkmpVariable::set(const jkmpResult &result)
{
    //qDebug()<<"jkmpVariable::set("<<result.toTypeString()<<")   vartype="<<jkmpResultTypeToString(type)<<"  resultType="<<jkmpResultTypeToString(result.type);
    if (type==result.type) {
        switch (result.type) {
            case jkmpDouble:  if (!num) {num=new double; internal=true; } *num=result.num; break;
            case jkmpDoubleVector:  if (!numVec){ numVec=new JKMP::vector<double>; internal=true; } *numVec=result.numVec; break;
            case jkmpDoubleMatrix:  if (!numVec){ if (numVec) delete numVec; if (matrix_columns) delete matrix_columns; numVec=new JKMP::vector<double>; matrix_columns=new int; internal=true; } *matrix_columns=result.matrix_columns; *numVec=result.numVec; break;
            case jkmpStringVector:  if (!strVec){ strVec=new JKMP::stringVector; internal=true; } *strVec=result.strVec; break;
            case jkmpBoolVector:  if (!boolVec){ boolVec=new JKMP::vector<bool>; internal=true; } *boolVec=result.boolVec; break;
            case jkmpBoolMatrix:  if (!boolVec || !matrix_columns){ if (boolVec) delete boolVec; if (matrix_columns) delete matrix_columns; boolVec=new JKMP::vector<bool>; matrix_columns=new int; internal=true; } *boolVec=result.boolVec; *matrix_columns=result.matrix_columns;   break;
            case jkmpString:  if (!str) {str=new JKMP::string; internal=true; } *str=result.str; break;
            case jkmpBool:  if (!boolean) {boolean=new bool; internal=true; } *boolean=result.boolean; break;
            case jkmpStruct:  if (!structData) {structData=new JKMP::map<JKMP::string, jkmpResult>; internal=true; } *structData=result.structData; break;
            case jkmpList:  if (!listData) {listData=new JKMP::vector<jkmpResult>; internal=true; } *listData=result.listData; break;
            case jkmpVoid: break;
        }
    } else {
        clearMemory();
        internal=true;
        type=result.type;
        switch (result.type) {
            case jkmpDouble: num=new double; *num=result.num; break;
            case jkmpDoubleVector: numVec=new JKMP::vector<double>; *numVec=result.numVec; break;
            case jkmpDoubleMatrix: numVec=new JKMP::vector<double>; matrix_columns=new int; *matrix_columns=result.matrix_columns; *numVec=result.numVec; break;
            case jkmpStringVector: strVec=new JKMP::stringVector; *strVec=result.strVec; break;
            case jkmpBoolVector: boolVec=new JKMP::vector<bool>; *boolVec=result.boolVec; break;
            case jkmpBoolMatrix: boolVec=new JKMP::vector<bool>; matrix_columns=new int; *matrix_columns=result.matrix_columns; *boolVec=result.boolVec; break;
            case jkmpString: str=new JKMP::string; *str=result.str; break;
            case jkmpBool: boolean=new bool; *boolean=result.boolean; break;
            case jkmpStruct: structData=new JKMP::map<JKMP::string, jkmpResult>; *structData=result.structData; break;
            case jkmpList: listData=new JKMP::vector<jkmpResult>; *listData=result.listData; break;
            case jkmpVoid: break;
        }
    }
    //qDebug()<<"jkmpVariable::set("<<result.toTypeString()<<")   vartype="<<jkmpResultTypeToString(type)<<"  data="<<toResult().toTypeString();
}




JKMP::stringVector JKMathParser::getLastErrors() const
{
    return lastError;
}

int JKMathParser::getLastErrorCount()
{
    return errors;
}

void JKMathParser::resetErrors()
{
    lastError.clear();
    errors=0;
}


JKMP::string JKMathParser::getLastError() const
{
    if (lastError.is_empty()) return JKMP::string();
    return lastError.back();
}

JKMP::string JKMathParser::getFirstError() const
{
    if (lastError.is_empty()) return JKMP::string();
    return lastError.front();
}

JKMathParser::jkmpFunctionDescriptor::jkmpFunctionDescriptor()
{
    name="";
    type=JKMathParser::functionC;
    functionNode=NULL;
    parameterNames.clear();
}

void JKMathParser::jkmpFunctionDescriptor::clearMemory()
{
    if (type==JKMathParser::functionNode && functionNode) {
        delete functionNode;
        functionNode=NULL;
    }
}

void JKMathParser::jkmpFunctionDescriptor::evaluate(jkmpResult &r, const JKMP::vector<jkmpResult> &parameters, JKMathParser *parent) const
{
    r.isValid=true;
    if (type==JKMathParser::functionC) {
        r=function(parameters.data(), parameters.size(), parent);
        //qDebug()<<"fd.evalFunction C "<<name<<" = "<<r.toTypeString();
    } else if (type==JKMathParser::functionCRefReturn) {
        functionRR(r, parameters.data(), parameters.size(), parent);
            //qDebug()<<"fd.evalFunction C "<<name<<" = "<<r.toTypeString();
    } else if (type==JKMathParser::functionNode) {
        if (parameterNames.size()!=parameters.size()) {
            r.setInvalid();
            parent->jkmpError(JKMP::_("function '%1' takes %2 parameters, but %3 parameters were given").arg(name).arg(parameterNames.size()).arg(parameters.size()));
        } else {
            if (parent && parameterNames.size()>0) {
                parent->enterBlock();
                //qDebug()<<"enter block "<<parent->getBlockLevel();
                for (size_t i=0; i<parameterNames.size(); i++) {
                    parent->addVariable(parameterNames[i], parameters[i]);
                    //qDebug()<<"  adding "<<parameterNames[i]<<"="<<parameters[i].toString()<<"  levels="<<parent->getVariableLevels(parameterNames[i]);
                }
                //qDebug()<<"  eval";
                functionNode->evaluate(r);
                //qDebug()<<"leaving block "<<parent->getBlockLevel();
                parent->leaveBlock();
            } else {
                functionNode->evaluate(r);
            }
        }
    } else {
        r.setInvalid();
    }
    //qDebug()<<"fd.evalFunction "<<name<<" = "<<r.toTypeString();

}

void JKMathParser::jkmpFunctionDescriptor::evaluate(jkmpResult &r,  JKMP::vector<jkmpNode *> parameters, JKMathParser *parent) const
{

    r.setInvalid();
    if (type==JKMathParser::functionFromNode) {
        functionFN(r, parameters.data(), parameters.size(), parent);
    } else if (type==JKMathParser::functionNode) {
        if (parameterNames.size()!=parameters.size()) {
            r.setInvalid();
            parent->jkmpError(JKMP::_("function '%1' takes %2 parameters, but %3 parameters were given").arg(name).arg(parameterNames.size()).arg(parameters.size()));
        } else {
            if (parent && parameters.size()>0) {
                parent->enterBlock();
                //qDebug()<<"enter block "<<parent->getBlockLevel();
                for (size_t i=0; i<parameters.size(); i++) {
                    jkmpResult ri;
                    parameters[i]->evaluate(ri);
                    parent->addVariable(parameterNames[i], ri);
                    //qDebug()<<"  adding "<<parameterNames[i]<<"="<<ri.toString()<<"  levels="<<parent->getVariableLevels(parameterNames[i]);
                }
                //qDebug()<<"  eval";
                functionNode->evaluate(r);
                //qDebug()<<"leaving block "<<parent->getBlockLevel();
                parent->leaveBlock();
            } else {
                functionNode->evaluate(r);
            }
        }
    } else if (type==JKMathParser::functionC) {
        JKMP::vector<jkmpResult> ps;
        ps.resize(parameters.size());
        for (size_t i=0; i<parameters.size(); i++) {
            parameters[i]->evaluate(ps[i]);
        }
        r=function(ps.data(), ps.size(), parent);
    } else if (type==JKMathParser::functionCRefReturn) {
        JKMP::vector<jkmpResult> ps;
        ps.resize(parameters.size());
        for (size_t i=0; i<parameters.size(); i++) {
            parameters[i]->evaluate(ps[i]);
        }
        functionRR(r, ps.data(), ps.size(), parent);
    } else {
        r.setInvalid();
    }

}

JKMP::string JKMathParser::jkmpFunctionDescriptor::toDefString() const
{
    JKMP::string res="";
    if (type==JKMathParser::functionNode) res=JKMP::string("%1(%2): node").arg(name).arg(parameterNames.join(", "));
    else if (type==JKMathParser::functionC) res=JKMP::string("%1(?): C function (copy-return)").arg(name);
    else if (type==JKMathParser::functionCRefReturn) res=JKMP::string("%1(?): C function (return-by-reference)").arg(name);
    else if (type==JKMathParser::functionFromNode) res=JKMP::string("%1(?): C function of nodes (return-by-reference)").arg(name);
    else if (type==JKMathParser::functionInvalid) res=JKMP::string("%1(?): invalid function").arg(name);
    return res;
}


JKMathParser::executionEnvironment::executionEnvironment(JKMathParser *parent)
{
    //qDebug()<<"executionEnvironment constructed parent="<<parent;
    currentLevel=0;
    this->parent=parent;
}

JKMathParser::executionEnvironment::~executionEnvironment()
{
    clear();
}

void JKMathParser::executionEnvironment::setParent(JKMathParser *parent)
{
    this->parent=parent;
}



void JKMathParser::executionEnvironment::clear()
{
    clearVariables();
    clearFunctions();
    currentLevel=0;
}



void JKMathParser::executionEnvironment::addVariable(const JKMP::string &name, const JKMathParser::jkmpVariable &variable)
{
    //qDebug()<<"addVariable("<<name<<")";
    if (variables.contains(name) && variables[name].size()>0) {
        if (variables[name].back().first==currentLevel) {
            variables[name].back().second.clearMemory();
            int l=variables[name].back().first;
            variables[name].pop_back
();
            variables[name].push_back(std::make_pair(l, variable));
        } else {
            variables[name].push_back(std::make_pair(currentLevel, variable));
        }
    } else {
        JKMP::vector<std::pair<int, jkmpVariable> > l;
        l.push_back(std::make_pair(currentLevel, variable));
        variables[name]=l;
    }
    //if (name=="x") //qDebug()<<"**ADDED_VARIABLE "<<currentLevel<<": var:"<<name<<"   levels="<<variables[name].size();
}

int JKMathParser::executionEnvironment::getVariableLevels(const JKMP::string &name) const
{
    if (variables.contains(name)) {
        return variables[name].size();
    } else {
        return 0;
    }
}

void JKMathParser::executionEnvironment::setVariableDouble(const JKMP::string &name, double result)
{
    //qDebug()<<"executionEnvironment::setVariableDouble("<<name<<result<<")";
    jkmpResult r;
    r.setDouble(result);
    setVariable(name, r);
}

void JKMathParser::executionEnvironment::setVariableDoubleVec(const JKMP::string &name, const JKMP::vector<double> &result)
{
    jkmpResult r;
    r.setDoubleVec(result);
    setVariable(name, r);
}

void JKMathParser::executionEnvironment::setVariableStringVec(const JKMP::string &name, const JKMP::stringVector &result)
{
    jkmpResult r;
    r.setStringVec(result);
    setVariable(name, r);
}

void JKMathParser::executionEnvironment::setVariableBoolVec(const JKMP::string &name, const JKMP::vector<bool> &result)
{
    jkmpResult r;
    r.setBoolVec(result);
    setVariable(name, r);
}

void JKMathParser::executionEnvironment::setVariableString(const JKMP::string &name, const JKMP::string &result)
{
    jkmpResult r;
    r.setString(result);
    setVariable(name, r);
}

void JKMathParser::executionEnvironment::setVariableBoolean(const JKMP::string &name, bool result)
{
    jkmpResult r;
    r.setBoolean(result);
    setVariable(name, r);

}

void JKMathParser::executionEnvironment::addVariableDouble(const JKMP::string &name, double result)
{
    //qDebug()<<"executionEnvironment::addVariableDouble("<<name<<result<<")";
    jkmpResult r;
    r.setDouble(result);
    //qDebug()<<"  adding "<<r.toTypeString();
    addVariable(name, r);
}

void JKMathParser::executionEnvironment::addVariableDoubleVec(const JKMP::string &name, const JKMP::vector<double> &result)
{
    jkmpResult r;
    r.setDoubleVec(result);
    addVariable(name, r);
}

void JKMathParser::executionEnvironment::addVariableStringVec(const JKMP::string &name, const JKMP::stringVector &result)
{
    jkmpResult r;
    r.setStringVec(result);
    addVariable(name, r);
}

void JKMathParser::executionEnvironment::addVariableBoolVec(const JKMP::string &name, const JKMP::vector<bool> &result)
{
    jkmpResult r;
    r.setBoolVec(result);
    addVariable(name, r);
}

void JKMathParser::executionEnvironment::addVariableString(const JKMP::string &name, const JKMP::string &result)
{
    jkmpResult r;
    r.setString(result);
    addVariable(name, r);
}

void JKMathParser::executionEnvironment::addVariableBoolean(const JKMP::string &name, bool result)
{
    jkmpResult r;
    r.setBoolean(result);
    addVariable(name, r);
}

void JKMathParser::executionEnvironment::deleteVariable(const JKMP::string &name)
{
    if (variables.contains(name)) {
        for (size_t i=0; i<variables[name].size(); i++) {
            jkmpVariable v=variables[name].at(i).second;
            v.clearMemory();
        }
    }
    variables.erase(name);
}

JKMP::string JKMathParser::executionEnvironment::printVariables() const
{
    JKMP::string res="";

    if (variables.size()>0) {

        auto itV=variables.begin();
        while (itV!=variables.end()) {

            jkmpVariable v=itV->second.back().second;
            res+="'"+itV->first+"'"+"\t\t";
            if (v.isInternal()) res+="intern"; else res+="extern";
            res+="\t"+v.toResult().toTypeString();
            res+="\n";

            ++itV;

        }
    }
    return res;
}

JKMP::string JKMathParser::executionEnvironment::printFunctions() const
{
    JKMP::string res="";

    if (functions.size()>0) {


        for (auto itV=functions.begin(); itV!=functions.end(); ++itV) {
            if (res.size()>0) res+="\n";

            res+=itV->second.back().second.toDefString();

        }
    }
    return res;
}

JKMP::vector<std::pair<JKMP::string, JKMathParser::jkmpVariable> > JKMathParser::executionEnvironment::getVariables() const
{
    JKMP::vector<std::pair<JKMP::string, JKMathParser::jkmpVariable> > res;

    if (variables.size()>0) {
        for (auto itV=variables.begin(); itV!=variables.end(); ++itV) {
            res.push_back(std::make_pair(itV->first, itV->second.back().second));
        }
    }
    return res;
}

JKMP::vector<std::pair<JKMP::string, JKMathParser::jkmpFunctionDescriptor> > JKMathParser::executionEnvironment::getFunctions() const
{
    JKMP::vector<std::pair<JKMP::string, JKMathParser::jkmpFunctionDescriptor> > res;

    if (functions.size()>0) {
        for (auto itV=functions.begin(); itV!=functions.end(); ++itV) {
            res.push_back(std::make_pair(itV->first, itV->second.back().second));
        }
    }
    return res;
}

void JKMathParser::executionEnvironment::setFunction(const JKMP::string &name, const JKMathParser::jkmpFunctionDescriptor &function)
{
    if (functions.contains(name) && functions[name].size()>0) {
        if (functions[name].back().first==currentLevel) {
            functions[name].back().second.clearMemory();
            int l=functions[name].back().first;
            functions[name].pop_back
();
            functions[name].push_back(std::make_pair(l, function));
        } else {
            functions[name].push_back(std::make_pair(currentLevel, function));
        }
    } else {
        JKMP::vector<std::pair<int, jkmpFunctionDescriptor> > l;
        l.push_back(std::make_pair(currentLevel, function));
        functions.insert(std::make_pair(name, l));
    }
}

void JKMathParser::executionEnvironment::addFunction(const JKMP::string &name, const JKMP::stringVector &parameterNames, JKMathParser::jkmpNode *function)
{
    JKMathParser::jkmpFunctionDescriptor fd;
    fd.type=functionNode;
    fd.parameterNames=parameterNames;
    fd.functionNode=function;
    fd.name=name;
    setFunction(name, fd);
}






void JKMathParser::executionEnvironment::clearVariables()
{
    JKMP::stringVector keys=variables.keys();
    for (size_t j=0; j<keys.size(); j++) {
        for (size_t i=0; i<variables[keys[j]].size(); i++) {
            variables[keys[j]].operator[](i).second.clearMemory();
        }
    }

    variables.clear();
}

void JKMathParser::executionEnvironment::clearFunctions()
{
    functions.clear();
}

JKMathParser::jkmpFunctionAssignNode::~jkmpFunctionAssignNode()
{
    if (child) delete child;
}

JKMathParser::jkmpFunctionAssignNode::jkmpFunctionAssignNode(JKMP::string function, JKMP::stringVector parameterNames, JKMathParser::jkmpNode *c, JKMathParser *p, JKMathParser::jkmpNode *par):
    jkmpNode(p, par)

{
    child=c;
    if (child) child->setParent(this);
    setParser(p);
    setParent(par);
    this->function=function;
    this->parameterNames=parameterNames;
}

void JKMathParser::jkmpFunctionAssignNode::evaluate(jkmpResult &res)
{
    getParser()->addFunction(function, parameterNames, child->copy(NULL));
    res.setVoid();
}

JKMathParser::jkmpNode *JKMathParser::jkmpFunctionAssignNode::copy(JKMathParser::jkmpNode *par)
{
    if (child) return new JKMathParser::jkmpFunctionAssignNode(function, parameterNames, child->copy(NULL), getParser(), par);
    else return new JKMathParser::jkmpFunctionAssignNode(function, parameterNames, NULL, getParser(), par);
}

bool JKMathParser::jkmpFunctionAssignNode::createByteCode(JKMathParser::ByteCodeProgram &/*program*/, JKMathParser::ByteCodeEnvironment *environment)
{
    if (child) {
        environment->functionDefs[function]=std::make_pair(parameterNames, child);
        return true;
    }
    return false;
}

JKMP::string JKMathParser::jkmpFunctionAssignNode::print() const
{
    if (child) return JKMP::string("%1( %2 ) = %3").arg(function).arg(parameterNames.join(", ")).arg(child->print());
    else return JKMP::string("%1( %2 ) = %3").arg(function).arg(parameterNames.join(", ")).arg(JKMP::string());
}

JKMP::string JKMathParser::jkmpFunctionAssignNode::printTree(int level) const
{
    if (child) return JKMP::string(2*level, char(' '))+JKMP::string("FunctionAssigneNode '%2(%3)'\n%1").arg(child->printTree(level+1)).arg(function).arg(parameterNames.join(", "));
    else return JKMP::string(2*level, char(' '))+JKMP::string("FunctionAssigneNode '%1(%2)'").arg(function).arg(parameterNames.join(", "));
}

jkmpResult JKMathParser::jkmpConstantNode::evaluate()
{
    return data;
}

void JKMathParser::jkmpConstantNode::evaluate(jkmpResult &result)
{
    result=data;
}

JKMathParser::jkmpNode *JKMathParser::jkmpConstantNode::copy(JKMathParser::jkmpNode *par)
{
    return new JKMathParser::jkmpConstantNode(data, getParser(), par);
}

bool JKMathParser::jkmpConstantNode::createByteCode(JKMathParser::ByteCodeProgram &program, ByteCodeEnvironment */*environment*/)
{
    if (data.type==jkmpDouble) {
        program.push_back(JKMathParser::ByteCodeInstruction(bcPush, data.num));
    } else if (data.type==jkmpBool) {
        program.push_back(JKMathParser::ByteCodeInstruction(bcPush, (data.boolean)?1.0:0.0));
    } else {
        getParser()->jkmpError(JKMP::_("only numbers and booleans allowed in bytecoded expressions!"));
        return false;
    }
    return true;
}

JKMP::string JKMathParser::jkmpConstantNode::print() const
{
    return data.toString();
}

JKMP::string JKMathParser::jkmpConstantNode::printTree(int level) const
{
    return JKMP::string(2*level, char(' '))+JKMP::string("ConstantNode %1").arg(data.toTypeString());
}

jkmpResult JKMathParser::jkmpInvalidNode::evaluate()
{
    return jkmpResult::invalidResult();
}

void JKMathParser::jkmpInvalidNode::evaluate(jkmpResult &result)
{
    result.setInvalid();
}

JKMathParser::jkmpNode *JKMathParser::jkmpInvalidNode::copy(JKMathParser::jkmpNode *par)
{
    return new JKMathParser::jkmpInvalidNode(getParser(), par);
}

JKMP::string JKMathParser::jkmpInvalidNode::print() const
{
    return JKMP::string("INVALID");
}

JKMP::string JKMathParser::jkmpInvalidNode::printTree(int level) const
{
    return JKMP::string(2*level, char(' '))+JKMP::string("InvalidNode");
}

void JKMathParser::jkmpVectorMatrixConstructionList::evaluate(jkmpResult &res)
{
    jkmpResult r;
    res.setDoubleVec(0,0);
    if (isMatrix()) {
        int realitems=0;
        int nextSep=-1;
        int cols=-1;
        int row=1;
        int thisrow=0;
        for (size_t i=0; i<list.size(); i++) {
            if (list[i]==NULL) {
                if (nextSep<0 || cols<0) {
                    cols=realitems;
                    nextSep=realitems*2+1;
                    row++;
                    thisrow=0;
                } else {
                    if (i!=nextSep) {
                        res.setInvalid();
                        if (getParser()) getParser()->jkmpError(JKMP::_("error in matrix construct [Val1, Val2; Val3, ...]: the given rows do not have the same number of entries (%1 expected in row %2, but %3 found").arg(cols).arg(row).arg(thisrow));
                        return;
                    } else {
                        nextSep=nextSep+realitems+1;
                        row++;
                        thisrow=0;
                    }
                }
            } else {
                realitems++;
                thisrow++;
            }
        }
        int rows=realitems/cols;
        if (list.size()>0) {
            if (realitems!=rows*cols) {
                res.setInvalid();
                if (getParser()) getParser()->jkmpError(JKMP::_("error in matrix construct [Val1, Val2; Val3, ...]: wrong number of values given (%1 expected but %2 given): possibly too many or too few values in last row").arg(rows*cols).arg(realitems));
                return;
            }
        }


        if (list.size()>0) {
            int ri=0;
            for (size_t i=0; i<list.size(); i++) {
                if (list[i]) {
                    r.setInvalid();
                    list[i]->evaluate(r);
                    ri++;
                    if (i==0) {
                        if (r.type==jkmpDouble) {
                            res.setDoubleMatrix(0, cols, 0);
                        } else if (r.type==jkmpBool) {
                            res.setBoolMatrix(0, cols, false);
                        }
                    }
                    if (res.type==jkmpDoubleMatrix) {
                        if (r.isValid && r.type==jkmpDouble) {
                            res.numVec.push_back(r.num);
                        } else {
                            res.setInvalid();
                            if (getParser()) getParser()->jkmpError(JKMP::_("error in matrix construct [Val1, Val2; Val3, ...]: item %1 has the wrong type (not number, but %2!)").arg(ri+1).arg(r.typeName()));
                            break;
                        }
                    } else if (res.type==jkmpBoolMatrix) {
                        if (r.isValid && r.type==jkmpBool) {
                            res.boolVec.push_back(r.boolean);
                        } else {
                            res.setInvalid();
                            if (getParser()) getParser()->jkmpError(JKMP::_("error in matrix construct [Val1, Val2; Val3, ...]: item %1 has the wrong type (not boolean, but %2!)").arg(ri+1).arg(r.typeName()));
                            break;
                        }
                    }
                }
            }
        }
    } else {
        res.setDoubleVec(0,0);
        if (list.size()>0) {
            for (size_t i=0; i<list.size(); i++) {
                r.setInvalid();
                list[i]->evaluate(r);
                if (i==0) {
                    if (r.type==jkmpString||(r.type==jkmpStringVector)) {
                        res.setStringVec(0, JKMP::string());
                    } else if (r.type==jkmpBool||(r.type==jkmpBoolVector)) {
                        res.setBoolVec(0, false);
                    }
                }
                if (res.type==jkmpDoubleVector) {
                    if (r.isValid && r.type==jkmpDouble) {
                        res.numVec.push_back(r.num);
                    } else if (r.isValid && r.type==jkmpDoubleVector) {
                        res.numVec<<r.numVec;
                    } else {
                        res.setInvalid();
                        if (getParser()) getParser()->jkmpError(JKMP::_("error in vector construct [Val1, Val2, ...]: item %1 has the wrong type (not number or number vector, but %2!)").arg(i+1).arg(r.typeName()));
                        break;
                    }
                } else if (res.type==jkmpStringVector) {
                    if (r.isValid && r.type==jkmpString) {
                        res.strVec.push_back(r.str);
                    } else if (r.isValid && r.type==jkmpStringVector) {
                        res.strVec<<r.strVec;
                    } else {
                        res.setInvalid();
                        if (getParser()) getParser()->jkmpError(JKMP::_("error in vector construct [Val1, Val2, ...]: item %1 has the wrong type (not string or string vector, but %2!)").arg(i+1).arg(r.typeName()));
                        break;
                    }
                } else if (res.type==jkmpBoolVector) {
                    if (r.isValid && r.type==jkmpBool) {
                        res.boolVec.push_back(r.boolean);
                    } else if (r.isValid && r.type==jkmpBoolVector) {
                        res.boolVec<<r.boolVec;
                    } else {
                        res.setInvalid();
                        if (getParser()) getParser()->jkmpError(JKMP::_("error in vector construct [Val1, Val2, ...]: item %1 has the wrong type (not boolean or boolean vector, but %2!)").arg(i+1).arg(r.typeName()));
                        break;
                    }
                }
            }
        }
    }
}

JKMathParser::jkmpNode *JKMathParser::jkmpVectorMatrixConstructionList::copy(JKMathParser::jkmpNode *par)
{
    JKMathParser::jkmpNodeList* n= new JKMathParser::jkmpVectorMatrixConstructionList(getParser(), par);
    if (list.size()>0) {
        for (size_t i=0; i<list.size(); i++) {
            if (list[i]) n->add(list[i]->copy(n));
            else n->add(list[i]);
        }
    }

    n->setParent(par);
    return n;

}

bool JKMathParser::jkmpVectorMatrixConstructionList::createByteCode(JKMathParser::ByteCodeProgram &/*program*/, JKMathParser::ByteCodeEnvironment */*environment*/)
{
    if (getParser()) getParser()->jkmpError(JKMP::_("no vector constructs in byte-code allowed"));
    return false;
}

JKMP::string JKMathParser::jkmpVectorMatrixConstructionList::print() const
{
    JKMP::stringVector sl;
    for (size_t i=0; i<list.size(); i++) {
        if (list[i]) sl<<list[i]->print();
        else sl<<"; ";
    }
    return JKMP::string("[ %1 ]").arg(sl.join(", "));
}

JKMP::string JKMathParser::jkmpVectorMatrixConstructionList::printTree(int level) const
{
    JKMP::stringVector sl;
    if (list.size()>0) {
        for (size_t i=0; i<list.size(); i++) {
            if (list[i]) sl<<list[i]->printTree(level+1);
            else sl<<"; ";
        }
    }
    return JKMP::string(2*level, char(' '))+JKMP::string("VectorMatrixCOnstructionListNode\n%1").arg(sl.join("\n"));

}

JKMathParser::jkmpNode *JKMathParser::jkmpVectorConstructionNode::copy(JKMathParser::jkmpNode *par)
{
    if (step) {
        return new jkmpVectorConstructionNode(start->copy(NULL), end->copy(NULL), step->copy(NULL), getParser(), par);
    } else {
        return new jkmpVectorConstructionNode(start->copy(NULL), end->copy(NULL), NULL, getParser(), par);
    }
}

JKMP::string JKMathParser::jkmpVectorConstructionNode::print() const
{
    if (step)  return JKMP::string("(%1):(%3):(%2)").arg(start->print()).arg(end->print()).arg(step->print());
    else  return JKMP::string("(%1):(%2)").arg(start->print()).arg(end->print());
}

JKMP::string JKMathParser::jkmpVectorConstructionNode::printTree(int level) const
{
    if (step)  return JKMP::string(2*level, char(' '))+JKMP::string("VectorConstructionNode:\n%1\n%3\n%2").arg(start->printTree(level+1)).arg(end->printTree(level+1)).arg(step->printTree(level+1));
    else  return JKMP::string(2*level, char(' '))+JKMP::string("VectorConstructionNode:\n%1\n%2").arg(start->printTree(level+1)).arg(end->printTree(level+1));

}

JKMathParser::jkmpVectorConstructionNode::jkmpVectorConstructionNode(JKMathParser::jkmpNode *start, JKMathParser::jkmpNode *end, JKMathParser::jkmpNode *step, JKMathParser *p, JKMathParser::jkmpNode *par):
    jkmpNode(p, par)

{
    this->start=start;
    this->end=end;
    this->step=step;
    setParser(p);
    setParent(par);
}

JKMathParser::jkmpVectorConstructionNode::~jkmpVectorConstructionNode()
{
    if (start) delete start;
    if (step) delete step;
    if (end) delete end;
}



void JKMathParser::jkmpVectorConstructionNode::evaluate(jkmpResult &res)
{
    if (!start) {
        if (getParser()) getParser()->jkmpError(JKMP::_("error in vector construct 'start[:delta]:end' INTERNAL ERROR: start does not exist"));
        res.setInvalid();
        return;
    }
    if (!end) {
        if (getParser()) getParser()->jkmpError(JKMP::_("error in vector construct 'start[:delta]:end' INTERNAL ERROR: end does not exist"));
        res.setInvalid();
        return;
    }
     res.setDoubleVec();
     jkmpResult rstart;
     start->evaluate(rstart);
     jkmpResult rend;
     end->evaluate(rend);
     if (rstart.type!=jkmpDouble || rend.type!=jkmpDouble) {
         if (getParser()) getParser()->jkmpError(JKMP::_("error in vector construct 'start[:delta]:end'' start, delta and end have to be numbers (start=%1, end=%2)").arg(rstart.toTypeString()).arg(rend.toTypeString()));
         res.setInvalid();
         return;
     }
     double s=rstart.num;
     double e=rend.num;
     double d=1;
     if (step) {
         jkmpResult rstep;
         step->evaluate(rstep);
         if (rstep.type!=jkmpDouble) {
             if (getParser()) getParser()->jkmpError(JKMP::_("error in vector construct 'start:delta:end'' delta has to be numbers (start=%1, step=%2, end=%3)").arg(rstart.toTypeString()).arg(rstep.toTypeString()).arg(rend.toTypeString()));
             res.setInvalid();
             return;
         }
         d=rstep.num;
     }
     res.isValid=true;
     if (d>0) {
         for (double t=s; t<=e; t=t+d) {
             res.numVec<<t;
         }
     } else if (d<0) {
         for (double t=s; t>=e; t=t+d) {
             res.numVec<<t;
         }
     }
}

JKMathParser::jkmpCasesNode::jkmpCasesNode(JKMathParser *p, JKMathParser::jkmpNode *par):
    jkmpNode(p, par)

{
    setParent(par);
    setParser(p);
    elseNode=NULL;
}

JKMathParser::jkmpCasesNode::~jkmpCasesNode()
{
    if (elseNode) delete elseNode;
    for (size_t i=0; i<casesNodes.size(); i++) {
        if (casesNodes[i].first) delete casesNodes[i].first;
        if (casesNodes[i].second) delete casesNodes[i].second;
    }
    casesNodes.clear();
}

void JKMathParser::jkmpCasesNode::setElse(JKMathParser::jkmpNode *elseNode)
{
    this->elseNode=elseNode;
}

void JKMathParser::jkmpCasesNode::addCase(JKMathParser::jkmpNode *decision, JKMathParser::jkmpNode *value)
{
    casesNodes.push_back(std::make_pair(decision, value));
}



void JKMathParser::jkmpCasesNode::evaluate(jkmpResult &r)
{
    r.isValid=false;
    jkmpResult d;
    for (size_t i=0; i<casesNodes.size(); i++) {
        casesNodes[i].first->evaluate(d);
        if (d.type==jkmpBool) {
            if (d.boolean) {
                casesNodes[i].second->evaluate(r);
                return;
            }
        } else {
            if (getParser()) getParser()->jkmpError(JKMP::_("decision statement %1 does not have type boolean").arg(i+1));
            r.setInvalid();
            return;
        }
    }
    if (elseNode) elseNode->evaluate(r);
}

JKMathParser::jkmpNode *JKMathParser::jkmpCasesNode::copy(JKMathParser::jkmpNode *par)
{
    JKMathParser::jkmpCasesNode* res=new JKMathParser::jkmpCasesNode(getParser(), par);
    for (size_t i=0; i<casesNodes.size(); i++) {
        JKMathParser::jkmpNode* n1=NULL;
        JKMathParser::jkmpNode* n2=NULL;
        if (casesNodes[i].first) n1=casesNodes[i].first->copy();
        if (casesNodes[i].second) n2=casesNodes[i].second->copy();
        res->addCase(n1, n2);
    }
    if (elseNode) res->setElse(elseNode->copy(res));
    return res;
}

bool JKMathParser::jkmpCasesNode::createByteCode(JKMathParser::ByteCodeProgram &program, JKMathParser::ByteCodeEnvironment *environment)
{
    /*
        if (a1) return va1;
        else if (a2) return va2;
        else if (a3) return va3;
        else return vae;


          EVAL_A1
          LOGICNOT
          JMPCOND label1    # jump if (!a1)
          EVAL VA1
          JMP END
        label1:     # label 1
          EVAL_A2
          LOGICNOT
          JMPCOND label2    # jump if (!a2)
          EVAL VA2
          JMP END
        label2:     # label 2
          EVAL_A3
          LOGICNOT
          JMPCOND labelelse    # jump if (!a2)
          EVAL VA3
          JMP END
        labelelse:  # label 0
          EVAL VAE
        end:        # label -1
          NOP

      */
    bool ok=true;
    JKMP::vector<int> caseadresses;
    JKMP::map<int, int> jumpadresses;
    for (size_t i=0; (ok && i<casesNodes.size()); i++) {
        ok=ok&&casesNodes[i].first->createByteCode(program, environment);
        program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicNot));
        if (ok) {
            caseadresses<<program.size();
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcJumpCondRel, static_cast<int>(i+1)));
            ok=ok&&casesNodes[i].second->createByteCode(program, environment);
            caseadresses<<program.size();
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcJumpRel, -1));
            jumpadresses[i+1]=program.size();
        }
    }
    if (elseNode && ok) {
        jumpadresses[0]=program.size();
        ok=ok&&elseNode->createByteCode(program, environment);
    }
    jumpadresses[-1]=program.size();
    program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcNOP));
    for (size_t i=0; i<caseadresses.size(); i++) {
        int oldjmp=program[caseadresses[i]].intpar;
       // //qDebug()<<"   convert JMP "<<oldjmp<<"  ==>  JMP "<<jumpadresses[oldjmp]-caseadresses[i]<<" (new abs: "<<jumpadresses[oldjmp]<<")    @ "<<caseadresses[i];
        program[caseadresses[i]].intpar=jumpadresses[oldjmp]-caseadresses[i];
    }
    return ok;
}

JKMP::string JKMathParser::jkmpCasesNode::print() const
{
    JKMP::stringVector sl;
    for (size_t i=0; i<casesNodes.size(); i++) {
        sl<<casesNodes[i].first->print();
        sl<<casesNodes[i].second->print();
    }
    if (elseNode) sl<<elseNode->print();
    return JKMP::string("cases(%1)").arg(sl.join(", "));
}

JKMP::string JKMathParser::jkmpCasesNode::printTree(int level) const
{
    JKMP::stringVector sl;
    for (size_t i=0; i<casesNodes.size(); i++) {
        sl<<casesNodes[i].first->printTree(level+1);
        sl<<casesNodes[i].second->printTree(level+1);
    }
    if (elseNode) sl<<elseNode->printTree(level+1);
    return JKMP::string(2*level, char(' '))+JKMP::string("CasesNode\n%1").arg(sl.join("\n"));

}

JKMathParser::jkmpVectorOperationNode::jkmpVectorOperationNode(const JKMP::string &operationName, const JKMP::string &variableName, JKMathParser::jkmpNode *items, JKMathParser::jkmpNode *expression, JKMathParser *p, JKMathParser::jkmpNode *par, jkmpNode *defaultValue):
    jkmpNode(p, par)

{
    this->operationName=operationName.toLower();
    this->variableName=variableName;
    this->expression=expression;
    this->items=items;
    this->start=NULL;
    this->end=NULL;
    this->delta=NULL;
    this->defaultValue=defaultValue;
}

JKMathParser::jkmpVectorOperationNode::jkmpVectorOperationNode(const JKMP::string &operationName, const JKMP::string &variableName, JKMathParser::jkmpNode *start, JKMathParser::jkmpNode *end, JKMathParser::jkmpNode *delta, JKMathParser::jkmpNode *expression, JKMathParser *p, JKMathParser::jkmpNode *par, JKMathParser::jkmpNode *defaultValue):
    jkmpNode(p, par)
{
    this->operationName=operationName.toLower();
    this->variableName=variableName;
    this->expression=expression;
    this->defaultValue=defaultValue;
    this->items=NULL;
    this->start=start;
    this->end=end;
    this->delta=delta;
}

JKMathParser::jkmpVectorOperationNode::~jkmpVectorOperationNode()
{
    if (defaultValue) delete defaultValue;
    if (expression) delete expression;
    if (items) delete items;
    if (start) delete start;
    if (end) delete end;
    if (delta) delete delta;
}


void JKMathParser::jkmpVectorOperationNode::evaluate(jkmpResult &r)
{
     r.isValid=false;
     JKMP::stringVector strVec, itemValsS;
     JKMP::vector<bool> boolVec, itemValsB;
     JKMP::vector<double> numVec, itemVals;
     JKMP::vector<jkmpResult> itemList, itemListO;
     jkmpResultType resType=jkmpDouble;
     bool evalItems=true;
     bool ok=false;
     bool isString=false;
     bool isBool=false;
     bool isList=false;

     int cnt=0;
     if (!ok) {
         if (items) {
             jkmpResult ri;
             if (evalItems) {
                 items->evaluate(ri);
                 if (ri.type==jkmpDouble) {itemVals<<ri.num; cnt=itemVals.size(); }
                 else if (ri.type==jkmpDoubleVector) {itemVals=ri.numVec; cnt=itemVals.size(); }
                 else if (ri.type==jkmpString) {itemValsS<<ri.str; isString=true; cnt=itemValsS.size(); }
                 else if (ri.type==jkmpStringVector) {itemValsS=ri.strVec; isString=true; cnt=itemValsS.size();}
                 else if (ri.type==jkmpBool) {itemValsB<<ri.boolean; isBool=true; cnt=itemValsB.size();}
                 else if (ri.type==jkmpBoolVector) {itemValsB=ri.boolVec; isBool=true; cnt=itemValsB.size();}
                 else if (ri.type==jkmpList) {itemList=ri.listData; isList=true; cnt=itemList.size();}
                 else {
                     if (getParser()) getParser()->jkmpError(JKMP::_("%1(NAME, ITEMS, EXPRESSION) expects a list of numbers, booleans or strings as ITEMS").arg(operationName));
                     r.setInvalid();
                     return;
                 }
             }
         } else {
             jkmpResult rs;
             start->evaluate(rs);
             jkmpResult re;
             end->evaluate(re);
             double s=rs.num;
             double e=re.num;
             if (rs.type!=jkmpDouble) {
                 if (getParser()) getParser()->jkmpError(JKMP::_("%1(NAME, START, DELTA, END, EXPRESSION) expects a number for START").arg(operationName));
                 r.setInvalid();
                 return;
             }
             if (re.type!=jkmpDouble) {
                 if (getParser()) getParser()->jkmpError(JKMP::_("%1(NAME, START, DELTA, END, EXPRESSION) expects a number for END").arg(operationName));
                 r.setInvalid();
                 return;
             }
             double d=1;
             if (delta) {
                 jkmpResult rd;
                 delta->evaluate(rd);
                 if (rd.type==jkmpDouble) {
                     d=rd.num;
                 } else {
                     if (getParser()) getParser()->jkmpError(JKMP::_("%1(NAME, START, DELTA, END, EXPRESSION) expects a number for DELTA").arg(operationName));
                     r.setInvalid();
                     return;
                 }

             }
             //qDebug()<<"for: t="<<s<<"; t<="<<e<<"; t=t+"<<d;
             if (d>0) {
                 for (double t=s; t<=e; t=t+d) {
                     itemVals<<t;
                 }
             } else if (d<0) {
                 for (double t=s; t>=e; t=t+d) {
                     itemVals<<t;
                 }
             }
             cnt=itemVals.size();
         }
         if (cnt<=0) {
             if (operationName=="for"||operationName=="savefor") {
                 r.setDoubleVec(JKMP::vector<double>());
                 return;
             } else if (operationName=="defaultfor") {
                 if (defaultValue) {
                     defaultValue->evaluate(r);
                 } else {
                     r.setDoubleVec(JKMP::vector<double>());
                 }
                 return;
             } else {
                 r.setDouble(0.0);
                 return;
             }
         }
         jkmpResult defr;

         getParser()->enterBlock();
         jkmpResult thisr;
         bool isFilterFor=((operationName=="filterfor") || (operationName=="savefilterfor"));
         for (int i=0; i<cnt; i++) {
             if (isBool) {
                 getParser()->addVariable(variableName, jkmpResult(itemValsB[i]));
             } else if (isString) {
                 getParser()->addVariable(variableName, jkmpResult(itemValsS[i]));
             } else if (isList) {
                 getParser()->addVariable(variableName, jkmpResult(itemList[i]));
             } else {
                 getParser()->addVariable(variableName, jkmpResult(itemVals[i]));
             }
             expression->evaluate(thisr);
             if (isFilterFor) {
                 if (i==0) {
                     if (isBool) {
                         resType=jkmpBoolVector;
                         boolVec.clear();
                     } else if (isString) {
                         resType=jkmpStringVector;
                         strVec.clear();
                     } else if (isList) {
                         itemListO.clear();
                     } else {
                         resType=jkmpDoubleVector;
                         numVec.clear();
                     }
                 }
                 if (thisr.type==jkmpBool) {
                     if (thisr.boolean) {
                         if (isBool) {
                             boolVec.push_back(itemValsB[i]);
                         } else if (isString) {
                             strVec.push_back(itemValsS[i]);
                         } else if (isList) {
                             itemListO.push_back(itemList[i]);
                         } else {
                             numVec.push_back(itemVals[i]);
                         }
                     }
                 } else {
                     if (operationName!="savefilterfor" && operationName!="savefor") {
                         if (getParser()) getParser()->jkmpError(JKMP::_("EXPRESSION in %1(NAME, ..., EXPRESSION) has to evaluate to a boolean, but found %2 in iteration %3").arg(operationName).arg(resultTypeToString(thisr.type)).arg(i+1));
                         r.setInvalid();
                     }
                     return;
                 }
             } else {
                 if (i==0 && !isList) {
                     switch(thisr.type) {
                         case jkmpBool:
                         case jkmpBoolVector:
                             resType=jkmpBoolVector;
                             break;
                         case jkmpString:
                         case jkmpStringVector:
                             resType=jkmpStringVector;
                             break;
                         case jkmpDouble:
                         case jkmpDoubleVector:
                             resType=jkmpDoubleVector;
                             break;
                         default:
                             resType=jkmpDoubleVector;
                             if (operationName=="defaultfor") {
                                 defr.setInvalid();
                                 if (defaultValue) defaultValue->evaluate(defr);
                                 resType=defr.type;
                             }
                             break;
                     }

                     //resType=thisr.type;
                 } else if (!isList && ((resType==jkmpBoolVector && thisr.type!=jkmpBool && thisr.type!=jkmpBoolVector)
                            || (resType==jkmpStringVector && thisr.type!=jkmpString && thisr.type!=jkmpStringVector)
                            || (resType==jkmpDoubleVector && thisr.type!=jkmpDouble && thisr.type!=jkmpDoubleVector))) {
                     if (operationName=="defaultfor") {
                         defr.setInvalid();
                         if (defaultValue) defaultValue->evaluate(defr);
                         if (!defr.isValid) {
                             if (getParser()) getParser()->jkmpError(JKMP::_("DEFAULT in %1(NAME, ..., EXPRESSION, DEFAULT) evaluated to invalid!").arg(operationName));
                             r.setInvalid();
                             return;
                         }
                         if ((resType==jkmpBoolVector && (defr.type==jkmpBool || defr.type==jkmpBoolVector))
                                                     || (resType==jkmpStringVector && (defr.type==jkmpString || defr.type==jkmpStringVector))
                                                     || (resType==jkmpDoubleVector && (defr.type==jkmpDouble || defr.type==jkmpDoubleVector))) {
                             thisr=defr;
                         } else {
                             if (getParser()) getParser()->jkmpError(JKMP::_("EXPRESSION in %1(NAME, ..., EXPRESSION, DEFAULT): DEFAULT has to evaluate to a compatible type in every iteration: expected compaitble with %3 (single/vector), but found %2 in iteration %4").arg(operationName).arg(defr.toTypeString()).arg(resultTypeToString(resType)).arg(i+1));
                             r.setInvalid();
                             return;
                         }
                     } else {
                         if (operationName!="savefor") {
                             if (getParser()) getParser()->jkmpError(JKMP::_("EXPRESSION in %1(NAME, ..., EXPRESSION) has to evaluate to a compatible type in every iteration: expected compaitble with %3 (single/vector), but found %2 in iteration %4").arg(operationName).arg(defr.toTypeString()).arg(resultTypeToString(resType)).arg(i+1));
                             r.setInvalid();
                             return;
                         }
                     }
                 }
                 if (isList) {
                     itemListO<<thisr;
                 } else {

                     switch(resType) {
                         case jkmpDoubleVector:
                             if (thisr.type==jkmpDouble) numVec<<thisr.num;
                             else if (thisr.type==jkmpDoubleVector) numVec<<thisr.numVec;
                             break;
                         case jkmpStringVector:
                             if (operationName=="sum" || operationName=="for" || operationName=="savefor" || operationName=="defaultfor") {
                                 if (thisr.type==jkmpString) strVec<<thisr.str;
                                 else if (thisr.type==jkmpStringVector) strVec<<thisr.strVec;
                             } else  if (operationName!="savefor")  {
                                 if (getParser()) getParser()->jkmpError(JKMP::_("EXPRESSION in %1(NAME, ..., EXPRESSION) has to evaluate to number: but found %2 in iteration %3").arg(operationName).arg(resultTypeToString(resType)).arg(i+1));
                                 r.setInvalid();
                                 return;
                             }
                             break;
                         case jkmpBoolVector:
                             if (operationName=="for" || operationName=="savefor" || operationName=="defaultfor") {
                                 if (thisr.type==jkmpBool) boolVec<<thisr.boolean;
                                 else if (thisr.type==jkmpBoolVector) boolVec<<thisr.boolVec;
                             } else  if (operationName!="savefor")  {
                                 if (getParser()) getParser()->jkmpError(JKMP::_("EXPRESSION in %1(NAME, ..., EXPRESSION) has to evaluate to number: but found %2 in iteration %3").arg(operationName).arg(resultTypeToString(resType)).arg(i+1));
                                 r.setInvalid();
                                 return;
                             }
                             break;
                         default:
                             if (operationName!="savefor") {
                                 if (getParser()) getParser()->jkmpError(JKMP::_("EXPRESSION in %1(NAME, ..., EXPRESSION) has to evaluate to string or number: but found %2 in iteration %3").arg(operationName).arg(resultTypeToString(resType)).arg(i+1));
                                 r.setInvalid();
                             }
                             return;
                     }
                 }
             }
         }
         getParser()->leaveBlock();

         r.isValid=true;
         if (operationName=="for"||operationName=="savefor"||operationName=="filterfor"||operationName=="savefilterfor"||operationName=="defaultfor") {
             if (isList) {
                 r.setList(itemListO);
             } else {
                 if (resType==jkmpDoubleVector) r.setDoubleVec(numVec);
                 else if (resType==jkmpStringVector) r.setStringVec(strVec);
                 else if (resType==jkmpBoolVector) r.setBoolVec(boolVec);
                 else if (operationName=="savefor" || operationName=="savefilterfor") {
                     r.setDoubleVec(JKMP::vector<double>());
                 } else r.setInvalid();
             }

         } else if (operationName=="sum") {
             if (resType==jkmpDoubleVector) r.setDouble(statisticsSum(numVec.data(), numVec.size()));
             else if (resType==jkmpStringVector) r.setString(strVec.join(""));
             else r.setInvalid();
         } else if (operationName=="prod") {
             if (resType==jkmpDoubleVector) r.setDouble(statisticsProd(numVec.data(), numVec.size()));
             else r.setInvalid();
         }
     }
}

bool JKMathParser::jkmpVectorOperationNode::createByteCode(JKMathParser::ByteCodeProgram &program, JKMathParser::ByteCodeEnvironment *environment)
{
    /*

  # reserve variable __operationName__stop
  # reserve variable __operationName__delta
  # reserve variable __operationName__runvar
      PUSH 0
      EVAL END
      HEAPWRITE __operationName__end           # __operationName__stop = END
  #if (delta exists)
      EVAL DELTA
  #else
      PUSH 1
  #endif
      HEAPWRITE __operationName__delta         # __operationName__delta = DELTA
      EVAL START
      HEAPWRITE __operationName__runvar        # __operationName__runvar = START
  start:
      HEAPREAD __operationName__stop           # if (__operationName__runvar>__operationName__stop) jump end
      HEAPREAD __operationName__runvar
      CMPLESSEREQUAL
      LOGICNOT
      JUMPCONDREL end

      EVAL EXPRESSION

      HEAPREAD __operationName__runvar         # __operationName__runvar=__operationName__delta+DELTA
      HEAPREAD __operationName__delta
      ADD
      HEAPWRITE __operationName__runvar
      JUMPREL start
  end:
      NOP
      */
    bool ok=expression&&start&&end;
    if (operationName=="sum" || operationName=="prod") {
        if (ok) {
            if (operationName=="prod") {
                program.push_back(JKMathParser::ByteCodeInstruction( JKMathParser::bcPush, 1.0));
            } else {
                program.push_back(JKMathParser::ByteCodeInstruction( JKMathParser::bcPush, 0.0));
            }
            JKMP::string vnStop=JKMP::string("__%1__stop").arg(operationName);
            JKMP::string vnDelta=JKMP::string("__%1__delta").arg(operationName);
            JKMP::string vnRun=variableName;//JKMP::string("__%1__runvar").arg(operationName);
            int stopvar=environment->pushVar(vnStop);
            int deltavar=environment->pushVar(vnDelta);
            int runvar=environment->pushVar(vnRun);

            ok=ok&&end->createByteCode(program, environment);
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapWrite, stopvar));
            if (delta) {
                ok=ok&&delta->createByteCode(program, environment);
            } else {
                program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcPush, 1.0));
            }
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapWrite, deltavar));
            ok=ok&&start->createByteCode(program, environment);
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapWrite, runvar));
            int expr_adress=program.size();

            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapRead, stopvar));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapRead, runvar));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcCmpLesserEqual));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcLogicNot));
            int jmpAdress=program.size();
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcJumpCondRel, 0));

            ok=ok&&expression->createByteCode(program, environment);
            if (operationName=="prod") {
                program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcMul));
            } else {
                program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcAdd));
            }

            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapRead, deltavar));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapRead, runvar));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcAdd));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcHeapWrite, runvar));
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcBJumpRel, static_cast<int>(program.size()-expr_adress)));
            environment->popVar(vnRun);
            environment->popVar(vnDelta);
            environment->popVar(vnStop);
            int end_adress=program.size();
            program[jmpAdress].intpar=end_adress-jmpAdress;
            program.push_back(JKMathParser::ByteCodeInstruction(JKMathParser::bcNOP));
            return ok;
        } else {
            if (getParser()) getParser()->jkmpError(JKMP::_("%1(VAR, ITEMS, EXPRESSION) not usable in byte-code").arg(operationName));
            return false;
        }
    } else {
        if (getParser()) getParser()->jkmpError(JKMP::_("%1(...) not usable in byte-code").arg(operationName));
        return false;
    }

}

JKMathParser::jkmpNode *JKMathParser::jkmpVectorOperationNode::copy(JKMathParser::jkmpNode *par)
{
    if (items) {
        return new jkmpVectorOperationNode(operationName, variableName, items->copy(), expression->copy(), getParser(), par);
    } else {
        jkmpNode* def=NULL;
        if (defaultValue) def=defaultValue->copy();
        if (delta) return new jkmpVectorOperationNode(operationName, variableName, start->copy(), end->copy(), delta->copy(), expression->copy(), getParser(), par, def);
        else return new jkmpVectorOperationNode(operationName, variableName, start->copy(), end->copy(), NULL, expression->copy(), getParser(), par, def);
    }
}

JKMP::string JKMathParser::jkmpVectorOperationNode::print() const
{
    JKMP::stringVector sl;
    JKMP::string def="";
    if (items) sl<<items->print();
    else {
        if (delta) {
            sl<<start->print();
            sl<<delta->print();
            sl<<end->print();
        } else {
            sl<<start->print();
            sl<<end->print();
        }
    }
    if (defaultValue) {
        def=JKMP::string(", ")+defaultValue->print();
    }
    return JKMP::string("%2(%3, %1, %4%5)").arg(sl.join(", ")).arg(operationName).arg(variableName).arg(expression->print()).arg(def);
}

JKMP::string JKMathParser::jkmpVectorOperationNode::printTree(int level) const
{
    JKMP::stringVector sl;
    if (items) sl<<items->printTree(level+1);
    else {
        if (delta) {
            sl<<start->printTree(level+1);
            sl<<delta->printTree(level+1);
            sl<<end->printTree(level+1);
        } else {
            sl<<start->printTree(level+1);
            sl<<end->printTree(level+1);
        }
        if (defaultValue) {
            sl<<defaultValue->printTree(level+1);
        }
    }
    return JKMP::string(2*level, char(' '))+JKMP::string("VectorOperationNode %2, %3\n%1\n%4)").arg(sl.join("\n")).arg(operationName).arg(variableName).arg(expression->printTree(level+1));


}

JKMathParser::jkmpVectorElementAssignNode::~jkmpVectorElementAssignNode()
{
    if (index) delete index;
}

JKMathParser::jkmpVectorElementAssignNode::jkmpVectorElementAssignNode(JKMP::string var, JKMathParser::jkmpNode *index, JKMathParser::jkmpNode *expression, JKMathParser *p, JKMathParser::jkmpNode *par):
    jkmpVariableAssignNode(var, expression, p, par)
{
    this->index=index;
}

void JKMathParser::jkmpVectorElementAssignNode::evaluate(jkmpResult &res)
{

     jkmpResult exp, idx, var;
     res.isValid=true;
     if (child) child->evaluate(exp);
     if (index) index->evaluate(idx);

     getParser()->getVariable(var, variable);
     //qDebug()<<"got variable "<<variable<<var.toTypeString();
     if (idx.convertsToIntVector()) {
         JKMP::vector<int> ii=idx.asIntVector();
         if (ii.size()==0) {
             getParser()->jkmpError(JKMP::_("vector element assignment needs non-empty number vector as index, but index is %1").arg(idx.toTypeString()));
             res.setInvalid();
             return;
         }
         if (var.type==jkmpList) {
             if (ii.size()>=1) {
                 for (size_t i=0; i<ii.size(); i++) {
                     if (ii[i]>=0 && ii[i]<var.listData.size()) {
                         var.listData[ii[i]]=exp;
                         getParser()->setVariable(variable, var);
                         res=var;
                     } else {
                         getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but vector %2 has only %3 elements").arg(ii[0]).arg(variable).arg(var.numVec.size()));
                         res.setInvalid();
                         return;
                     }
                 }
                 getParser()->setVariable(variable, var);
             } else {
                 getParser()->jkmpError(JKMP::_("can only assign x[N elements]=(1 element)"));
                 res.setInvalid();
                 return;
             }
         } else if (var.type==jkmpDoubleVector && exp.convertsToVector()) {
             JKMP::vector<double> dat=exp.asVector();
             if (ii.size()==1 && dat.size()==1) {
                 if (ii[0]>=0 && ii[0]<var.numVec.size()) {
                     var.numVec[ii[0]]=dat[0];
                     getParser()->setVariable(variable, var);
                     res.setDouble(dat[0]);
                 } else {
                     getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but vector %2 has only %3 elements").arg(ii[0]).arg(variable).arg(var.numVec.size()));
                     res.setInvalid();
                     return;
                 }
                 getParser()->setVariable(variable, var);
             } else if (ii.size()>1 && dat.size()==1) {
                 res.type=jkmpDoubleVector;
                 res.numVec.clear();
                 const double t=dat[0];
                 for (size_t i=0; i<ii.size(); i++) {
                     if (ii[i]>=0 && ii[i]<var.numVec.size()) {
                         var.numVec[ii[i]]=t;
                         res.numVec<<t;
                     } else {
                         getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but vector %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.numVec.size()));
                         res.setInvalid();
                         return;
                     }
                 }
                 getParser()->setVariable(variable, var);
             } else if (ii.size()>1 && dat.size()==ii.size()) {
                 res.type=jkmpDoubleVector;
                 res.numVec.clear();
                 for (size_t i=0; i<ii.size(); i++) {
                     if (ii[i]>=0 && ii[i]<var.numVec.size()) {
                         const double t=dat[i];
                         var.numVec[ii[i]]=t;
                         res.numVec<<t;
                     } else {
                         getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but vector %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.numVec.size()));
                         res.setInvalid();
                         return;
                     }
                 }
                 getParser()->setVariable(variable, var);
             } else {
                 getParser()->jkmpError(JKMP::_("can only assign x[N elements]=(1 element), x[N elements]=(N elements), x[1 elements]=(1 elements)"));
                 res.setInvalid();
                 return;
             }

         } else if ((exp.type==jkmpString||exp.type==jkmpStringVector) && var.type==jkmpStringVector) {
             JKMP::stringVector dat=exp.asStrVector();
             if (ii.size()==1 && dat.size()==1) {
                 if (ii[0]>=0 && ii[0]<var.strVec.size()) {
                     var.strVec[ii[0]]=dat[0];
                     getParser()->setVariable(variable, var);
                     res=jkmpResult(dat[0]);
                 } else {
                     getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but vector %2 has only %3 elements").arg(ii[0]).arg(variable).arg(var.strVec.size()));
                     res.setInvalid();
                     return;
                 }
                 getParser()->setVariable(variable, var);
             } else if (ii.size()>1 && dat.size()==1) {
                 res.type=jkmpStringVector;
                 res.strVec.clear();
                 const JKMP::string t=dat[0];
                 for (size_t i=0; i<ii.size(); i++) {
                     if (ii[i]>=0 && ii[i]<var.strVec.size()) {
                         var.strVec[ii[i]]=t;
                         res.strVec<<t;
                     } else {
                         getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but vector %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.strVec.size()));
                         res.setInvalid();
                         return;
                     }
                 }
                 getParser()->setVariable(variable, var);
             } else if (ii.size()>1 && dat.size()==ii.size()) {
                 res.type=jkmpStringVector;
                 res.strVec.clear();
                 for (size_t i=0; i<ii.size(); i++) {
                     if (ii[i]>=0 && ii[i]<var.strVec.size()) {
                         const JKMP::string t=dat[i];
                         var.strVec[ii[i]]=t;
                         res.strVec<<t;
                     } else {
                         getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but vector %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.strVec.size()));
                         res.setInvalid();
                         return;
                     }
                 }
                 getParser()->setVariable(variable, var);
             } else {
                 getParser()->jkmpError(JKMP::_("can only assign x[N elements]=(1 element), x[N elements]=(N elements), x[1 elements]=(1 elements)"));
                 res.setInvalid();
                 return;
             }
         } else if ((exp.type==jkmpBool||exp.type==jkmpBoolVector) && var.type==jkmpBoolVector) {
             JKMP::vector<bool> dat=exp.asBoolVector();
             if (ii.size()==1 && dat.size()==1) {
                 if (ii[0]>=0 && ii[0]<var.boolVec.size()) {
                     var.boolVec[ii[0]]=dat[0];
                     getParser()->setVariable(variable, var);
                     res=jkmpResult(dat[0]);
                 } else {
                     getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but vector %2 has only %3 elements").arg(ii[0]).arg(variable).arg(var.boolVec.size()));
                     res.setInvalid();
                     return;
                 }
                 getParser()->setVariable(variable, var);
             } else if (ii.size()>1 && dat.size()==1) {
                 res.type=jkmpBoolVector;
                 res.boolVec.clear();
                 const bool t=dat[0];
                 for (size_t i=0; i<ii.size(); i++) {
                     if (ii[i]>=0 && ii[i]<var.boolVec.size()) {
                         var.boolVec[ii[i]]=t;
                         res.boolVec<<t;
                     } else {
                         getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but vector %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.boolVec.size()));
                         res.setInvalid();
                         return;
                     }
                 }
                 getParser()->setVariable(variable, var);
             } else if (ii.size()>1 && dat.size()==ii.size()) {
                 res.type=jkmpBoolVector;
                 res.boolVec.clear();
                 for (int i=0; i<ii.size(); i++) {
                     if (ii[i]>=0 && ii[i]<var.boolVec.size()) {
                         const bool t=dat[i];
                         var.boolVec[ii[i]]=t;
                         res.boolVec<<t;
                     } else {
                         getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but vector %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.boolVec.size()));
                         res.setInvalid();
                         return;
                     }
                 }
                 getParser()->setVariable(variable, var);
             } else {
                 getParser()->jkmpError(JKMP::_("can only assign x[N elements]=(1 element), x[N elements]=(N elements), x[1 elements]=(1 elements)"));
                 res.setInvalid();
                 return;
             }



         } else if (exp.type==jkmpString && var.type==jkmpString){
             JKMP::string dat=exp.asString();
             if (ii.size()==1 && dat.size()==1) {
                 if (ii[0]>=0 && ii[0]<var.str.size()) {
                     var.str[ii[0]]=dat[0];
                     getParser()->setVariable(variable, var);
                     res.setString(JKMP::string(dat[0]));
                 } else {
                     getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but string %2 has only %3 elements").arg(ii[0]).arg(variable).arg(var.str.size()));
                     res.setInvalid();
                     return;
                 }
                 getParser()->setVariable(variable, var);
             } else if (ii.size()>1 && dat.size()==1) {
                 res.type=jkmpString;
                 res.str.clear();
                 char t=dat[0];
                 for (size_t i=0; i<ii.size(); i++) {
                     if (ii[i]>=0 && ii[i]<var.str.size()) {
                         var.str[ii[i]]=t;
                         res.str+=t;
                     } else {
                         getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but string %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.str.size()));
                         res.setInvalid();
                         return;
                     }
                 }
                 getParser()->setVariable(variable, var);
             } else if (ii.size()>1 && dat.size()==ii.size()) {
                 res.type=jkmpString;
                 res.str.clear();
                 for (size_t i=0; i<ii.size(); i++) {
                     if (ii[i]>=0 && ii[i]<var.str.size()) {
                         char t=dat[i];
                         var.str[ii[i]]=t;
                         res.str+=t;
                     } else {
                         getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to assign to element %1, but string %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.str.size()));
                         res.setInvalid();
                         return;
                     }
                 }
                 getParser()->setVariable(variable, var);
             } else {
                 getParser()->jkmpError(JKMP::_("can only assign x[N elements]=(1 element), x[N elements]=(N elements), x[1 elements]=(1 elements)"));
                 res.setInvalid();
                 return;
             }
         } else {
             getParser()->jkmpError(JKMP::_("vector element assignment needs an expression which evaluates to the same type as the variable (var: %1, expression: %2)").arg(resultTypeToString(var.type)).arg(exp.toTypeString()));
             res.setInvalid();
             return;
         }
     } else {
         getParser()->jkmpError(JKMP::_("vector element assignment needs number or number vector as index, but index is %1").arg(resultTypeToString(idx.type)));
         res.setInvalid();
         return;
     }

}

JKMathParser::jkmpNode *JKMathParser::jkmpVectorElementAssignNode::copy(JKMathParser::jkmpNode *par)
{
    return new JKMathParser::jkmpVectorElementAssignNode(variable, index->copy(NULL), child->copy(NULL), getParser(), par);
}

JKMP::string JKMathParser::jkmpVectorElementAssignNode::print() const
{
    return JKMP::string("%1[%3] = %2").arg(variable).arg(child->print()).arg(index->print());
}

JKMP::string JKMathParser::jkmpVectorElementAssignNode::printTree(int level) const
{
    return JKMP::string(2*level, char(' '))+JKMP::string("VectorElementAssigneNode '%2'\n%1\n%3").arg(index->printTree(level+1)).arg(variable).arg(child->printTree(level+1));
}

JKMathParser::jkmpVariableVectorAccessNode::~jkmpVariableVectorAccessNode()
{
    if (index) delete index;
}

JKMathParser::jkmpVariableVectorAccessNode::jkmpVariableVectorAccessNode(JKMP::string var, JKMathParser::jkmpNode *index, JKMathParser *p, JKMathParser::jkmpNode *par):
    jkmpNode(p, par)
{
    variable=var;
    this->index=index;
    if(index) index->setParent(this);
}


void JKMathParser::jkmpVariableVectorAccessNode::evaluate(jkmpResult &res)
{
    jkmpResult  idx, var;
    if (index) index->evaluate(idx);
    res.isValid=true;

    var=getParser()->getVariable(variable);
    if (var.type==jkmpDoubleVector) {
        if (idx.convertsToBoolVector()) {
            JKMP::vector<bool> ii=idx.asBoolVector();
            if (ii.size()!=var.numVec.size()) {
                getParser()->jkmpError(JKMP::_("vector variable element access by boolean-vectors needs a boolean vector of the same size, as the vector variable, but: index: %1, data: %2 elements").arg(ii.size()).arg(var.numVec.size()));
                res.setInvalid();
                return;
            }
            int trues=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) trues++;
            }
            res.setDoubleVec(trues);
            int j=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) {
                    res.numVec[j]=var.numVec[i];
                    j++;
                }
            }
        } else if (idx.convertsToIntVector()) {
            JKMP::vector<int> ii=idx.asIntVector();
            if (ii.size()==0) {
                getParser()->jkmpError(JKMP::_("vector variable element access needs non-empty number vector as index, but index is %1").arg(idx.toTypeString()));
                res.setInvalid();
                return;
            }
            res.type=jkmpDoubleVector;
            res.numVec=JKMP::vector<double>(ii.size(), 0);
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]>=0 && ii[i]<var.numVec.size()) {
                    res.numVec[i]=var.numVec[ii[i]];
                } else {
                    getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to access element %1, but vector variable %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.numVec.size()));
                    res.setInvalid();
                    return;
                }
            }
            if (res.numVec.size()==1) res.setDouble(res.numVec[0]);

        } else {
            getParser()->jkmpError(JKMP::_("vector variable element access needs number/number vector or boolean/boolean vector as index, but index is %1").arg(resultTypeToString(idx.type)));
            res.setInvalid();
            return;
        }
    } else if (var.type==jkmpList) {
        if (idx.convertsToBoolVector()) {
            JKMP::vector<bool> ii=idx.asBoolVector();
            if (ii.size()!=var.listData.size()) {
                getParser()->jkmpError(JKMP::_("list element access by boolean-vectors needs a boolean vector of the same size, as the data vector, but: index: %1, data: %2 elements").arg(ii.size()).arg(var.listData.size()));
                res.setInvalid();
                return;
            }
            int trues=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) trues++;
            }
            res.setList(trues);
            int j=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) {
                    res.listData[j]=var.listData[i];
                    j++;
                }
            }
            if (res.listData.size()==1) res=res.listData[0];
        } else if (idx.convertsToIntVector()) {
            JKMP::vector<int> ii=idx.asIntVector();
            if (ii.size()==0) {
                getParser()->jkmpError(JKMP::_("list element access needs non-empty number vector as index, but index is %1").arg(idx.toTypeString()));
                res.setInvalid();
                return;
            }
            res.setList(ii.size());
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]>=0 && ii[i]<var.listData.size()) {
                    res.listData[i]=var.listData[ii[i]];
                } else {
                    getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to access element %1, but list %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.listData.size()));
                    res.setInvalid();
                    return;
                }
            }
            if (res.listData.size()==1) res=res.listData[0];
        } else {
            getParser()->jkmpError(JKMP::_("list element access needs number/number vector or boolean/boolean vector as index, but index is %1").arg(resultTypeToString(idx.type)));
            res.setInvalid();
            return;
        }
    } else if (var.type==jkmpStringVector) {
        if (idx.convertsToBoolVector()) {
            JKMP::vector<bool> ii=idx.asBoolVector();
            if (ii.size()!=var.strVec.size()) {
                getParser()->jkmpError(JKMP::_("vector element access by boolean-vectors needs a boolean vector of the same size, as the data vector, but: index: %1, data: %2 elements").arg(ii.size()).arg(var.numVec.size()));
                res.setInvalid();
                return;
            }
            int trues=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) trues++;
            }
            res.setStringVec(trues);
            int j=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) {
                    res.strVec[j]=var.strVec[i];
                    j++;
                }
            }
        } else if (idx.convertsToIntVector()) {
            JKMP::vector<int> ii=idx.asIntVector();
            if (ii.size()==0) {
                getParser()->jkmpError(JKMP::_("vector element access needs non-empty number vector as index, but index is %1").arg(idx.toTypeString()));
                res.setInvalid();
                return;
            }
            res.type=jkmpStringVector;
            res.strVec.clear();
            //res.strVec.resize(ii.size(), JKMP::string());
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]>=0 && ii[i]<var.strVec.size()) {
                    res.strVec<<var.strVec[ii[i]];
                } else {
                    getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to access element %1, but vector %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.strVec.size()));
                    res.setInvalid();
                    return;
                }
            }
            if (res.strVec.size()==1) res.setString(res.strVec[0]);
        } else {
            getParser()->jkmpError(JKMP::_("vector element access needs number/number vector or boolean/boolean vector as index, but index is %1").arg(resultTypeToString(idx.type)));
            res.setInvalid();
            return;

        }
    } else if (var.type==jkmpBoolVector) {
        if (idx.convertsToBoolVector()) {
            JKMP::vector<bool> ii=idx.asBoolVector();
            if (ii.size()!=var.boolVec.size()) {
                getParser()->jkmpError(JKMP::_("vector element access by boolean-vectors needs a boolean vector of the same size, as the data vector, but: index: %1, data: %2 elements").arg(ii.size()).arg(var.numVec.size()));
                res.setInvalid();
                return;
            }
            int trues=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) trues++;
            }
            res.setBoolVec(trues);
            int j=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) {
                    res.boolVec[j]=var.boolVec[i];
                    j++;
                }
            }
        } else if (idx.convertsToIntVector()) {
            JKMP::vector<int> ii=idx.asIntVector();
            if (ii.size()==0) {
                getParser()->jkmpError(JKMP::_("vector element access needs non-empty number vector as index, but index is %1").arg(idx.toTypeString()));
                res.setInvalid();
                return;
            }
            res.type=jkmpBoolVector;
            res.boolVec=JKMP::vector<bool>(ii.size(), 0);
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]>=0 && ii[i]<var.boolVec.size()) {
                    res.boolVec[i]=var.boolVec[ii[i]];
                } else {
                    getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to access element %1, but vector %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.boolVec.size()));
                    res.setInvalid();
                    return;
                }
            }
            if (res.boolVec.size()==1) res.setBoolean(res.boolVec[0]);

        } else {
            getParser()->jkmpError(JKMP::_("vector element access needs number/number vector or boolean/boolean vector as index, but index is %1").arg(resultTypeToString(idx.type)));
            res.setInvalid();
            return;
        }
    } else if (var.type==jkmpString) {
        if (idx.convertsToBoolVector()) {
            JKMP::vector<bool> ii=idx.asBoolVector();
            if (ii.size()!=var.str.size()) {
                getParser()->jkmpError(JKMP::_("vector element access by boolean-vectors needs a boolean vector of the same size, as the data vector, but: index: %1, data: %2 elements").arg(ii.size()).arg(var.numVec.size()));
                res.setInvalid();
                return;
            }
            int trues=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) trues++;
            }
            res.setString(trues);
            int j=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) {
                    res.str[j]=var.str[i];
                    j++;
                }
            }
        } else if (idx.convertsToIntVector()) {
            JKMP::vector<int> ii=idx.asIntVector();
            if (ii.size()==0) {
                getParser()->jkmpError(JKMP::_("vector element access needs non-empty number vector as index, but index is %1").arg(idx.toTypeString()));
                res.setInvalid();
                return;
            }
            res.type=jkmpString;
            res.str="";
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]>=0 && ii[i]<var.str.size()) {
                    res.str+=var.str[ii[i]];
                } else {
                    getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to access element %1, but string %2 has only %3 elements").arg(ii[i]).arg(variable).arg(var.str.size()));
                    res.setInvalid();
                    return;
                }
            }

        } else {
            getParser()->jkmpError(JKMP::_("string character access needs number/number vector or boolean/boolean vector as index, but index is %1").arg(resultTypeToString(idx.type)));
            res.setInvalid();
            return;
        }
    } else {
        getParser()->jkmpError(JKMP::_("vector element/string character access is only possible if the variable is a number/string/bool vector, or a string, but variable '%1' is of type %2").arg(variable).arg(resultTypeToString(var.type)));
        res.setInvalid();
        return;
    }
}

JKMathParser::jkmpNode *JKMathParser::jkmpVariableVectorAccessNode::copy(JKMathParser::jkmpNode *par)
{
    return new jkmpVariableVectorAccessNode(variable, index->copy(NULL), getParser(), par);
}

JKMP::string JKMathParser::jkmpVariableVectorAccessNode::print() const
{
    return JKMP::string("%1[%2]").arg(variable).arg(index->print());
}

JKMP::string JKMathParser::jkmpVariableVectorAccessNode::printTree(int level) const
{
return JKMP::string(2*level, char(' '))+JKMP::string("VariableVectorAccessNode: '%2'\n%1").arg(index->printTree(level+1)).arg(variable);
}

void JKMathParser::jkmpNode::evaluate(jkmpResult &result)
{
    result=evaluate();
}












//#define JKMATHPARSER_BYTECODESTACK_ERRORCHECKING





double JKMathParser::evaluateBytecode(const JKMathParser::ByteCodeProgram &program)
{
    JKMP::vector<double> resultStack;
    JKMP::vector<double> heap;
    //int heappointer=0;
    heap.resize(ByteCodeInitialHeapSize);
    resultStack.reserve(128);
    bool ok=true;
    JKMathParser::ByteCodeProgram::const_iterator itp=program.begin();
    while (ok && itp<program.end()) {
#ifdef JKMATHPARSER_BYTECODESTACK_DEBUGMESSAGES
        JKMathParser::ByteCodeProgram::const_iterator itp_old=itp;
#endif
        switch (itp->opcode) {
            case bcNOP:
                break;
            case bcPush:
                resultStack.push(itp->numpar);
                break;
            case bcPop:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.is_empty()) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in POP opcode: stack is empty"));
                    ok=false;
                } else
#endif
                    resultStack.pop();

                break;

            case bcVarRead:
                resultStack.push(*((double*)itp->pntpar));
                break;
            case bcVarWrite:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<1) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in VARWRITE opcode: stack is too small"));
                    ok=false;
                } else
#endif
                *((double*)itp->pntpar)=resultStack.pop();
                break;


            case bcHeapRead:
                resultStack.push(heap.value(itp->intpar, NAN));
#ifdef JKMATHPARSER_BYTECODESTACK_DEBUGMESSAGES
                //qDebug()<<"HEAPREAD  #"<<itp->intpar+heapoffset<<" == "<<heap.value(itp->intpar+heapoffset, NAN);
#endif
                break;
            case bcHeapWrite:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<1) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in HEAPWRITE opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    if (itp->intpar>heap.size()) heap.resize(itp->intpar+1);
                    heap[itp->intpar]=resultStack.pop();
#ifdef JKMATHPARSER_BYTECODESTACK_DEBUGMESSAGES
                    //qDebug()<<"HEAPWRITE  #"<<itp->intpar+heapoffset<<" <= "<<heap[itp->intpar+heapoffset];
#endif
                }
                break;



            case bcCallCFunction:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<itp->intpar) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in CALLCFUNC opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    switch (itp->intpar) {
                        case 0: *(it-1)=((jkmpEvaluateFuncSimple0Param)itp->pntpar)(); break;
                        case 1: *(it-1)=((jkmpEvaluateFuncSimple1Param)itp->pntpar)(*(it-1)); break;
                        case 2: *(it-2)=((jkmpEvaluateFuncSimple2Param)itp->pntpar)(*(it-1),*(it-2)); resultStack.pop(); break;
                        case 3: *(it-3)=((jkmpEvaluateFuncSimple3Param)itp->pntpar)(*(it-1),*(it-2),*(it-3)); resultStack.pop(); resultStack.pop(); break;
                        default: break;
                    }
                } break;
            case bcCallCMPFunction:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<itp->intpar) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in CALLCMPFUNC opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    switch (itp->intpar) {
                        case 0: *(it-1)=((jkmpEvaluateFuncSimple0ParamMP)itp->pntpar)(this); break;
                        case 1: *(it-1)=((jkmpEvaluateFuncSimple1ParamMP)itp->pntpar)(*(it-1), this); break;
                        case 2: *(it-2)=((jkmpEvaluateFuncSimple2ParamMP)itp->pntpar)(*(it-1),*(it-2), this); resultStack.pop(); break;
                        case 3: *(it-3)=((jkmpEvaluateFuncSimple3ParamMP)itp->pntpar)(*(it-1),*(it-2),*(it-3), this); resultStack.pop(); resultStack.pop(); break;
                        default: break;
                    }
                } break;
        case bcCallResultFunction:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
            if (resultStack.size()<itp->intpar) {
                jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in CALLRESULTFUNC opcode: stack is too small"));
                ok=false;
            } else
#endif
            {
               // const JKMP::vector<double>::iterator it=resultStack.end();
                JKMP::vector<jkmpResult> parameters;
                for (int i=0; i<itp->intpar; i++) {
                    parameters<<jkmpResult(resultStack.pop());
                }
                jkmpResult r;
                evaluateFunction(r, itp->strpar, parameters);
                if (r.type==jkmpDouble) resultStack.push(r.asNumber());
                else if (r.type==jkmpBool) resultStack.push((r.asBool())?1.0:0.0);
                else {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: result of function call ('%1'') was not a number!").arg(itp->strpar));
                    ok=false;
                }
            } break;

            case bcAdd:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in ADD opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = (*(it-1))+(*(it-2));
                    resultStack.pop();

                }
                break;
            case bcSub:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in SUB opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = (*(it-1))-(*(it-2));
                    resultStack.pop();

                }
                break;
            case bcMul:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in MUL opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = (*(it-1))*(*(it-2));
                    resultStack.pop();

                }
                break;

            case bcDiv:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in DIV opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = (*(it-1))/(*(it-2));
                    resultStack.pop();

                }
                break;
            case bcMod:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in MOD opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = int32_t(*(it-1))%int32_t(*(it-2));
                    resultStack.pop();

                }
                break;
            case bcPow:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in POW opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = pow((*(it-1)),(*(it-2)));
                    resultStack.pop();

                }

                break;
            case bcNeg:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<1) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in NEG opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-1) = -(*(it-1));

                }
                break;



            case bcBitAnd:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in BITAND opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = int32_t(*(it-1))&int32_t(*(it-2));
                    resultStack.pop();

                } break;
            case bcBitOr:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in BITOR opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = int32_t(*(it-1))|int32_t(*(it-2));
                    resultStack.pop();

                }

                break;
            case bcBitNot:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<1) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in BITNOT opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-1) = ~int32_t(*(it-1));

                }
                break;



            case bcLogicAnd:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in LOGICAND opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = ((*(it-1)!=0.0)&&(*(it-2)!=0.0))?1:0;
                    resultStack.pop();

                } break;
            case bcLogicOr:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in LOGICOR opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = ((*(it-1)!=0.0)||(*(it-2)!=0.0))?1:0;
                    resultStack.pop();

                }

                break;
            case bcLogicXor:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in LOGICOR opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = (((*(it-1)!=0.0)&&(*(it-2)==0.0))||((*(it-1)==0.0)&&(*(it-2)!=0.0)))?1:0;
                    resultStack.pop();

                }

                break;

            case bcLogicNot:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<1) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in LOGICNOT opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-1) = (!(*(it-1)!=0.0))?1:0;

                }
                break;

            case bcCmpEqual:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in CMPEQUAL opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = ((*(it-1))==(*(it-2)))?1:0;
                    resultStack.pop();

                }
                break;



            case bcCmpLesser:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in CMPLESSER opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = ((*(it-1))<(*(it-2)))?1:0;
                    resultStack.pop();

                }
                break;

            case bcCmpLesserEqual:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<2) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in CMPLESSEREQUAL opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    const JKMP::vector<double>::iterator it=resultStack.end();
                    *(it-2) = ((*(it-1))<=(*(it-2)))?1:0;
                    resultStack.pop();

                }
                break;

            case bcJumpRel:
                itp+=(itp->intpar-1);
#ifdef JKMATHPARSER_BYTECODESTACK_DEBUGMESSAGES
                //qDebug()<<"                                                    @"<<((int)itp_old-(int)program.begin())/int(sizeof(*itp_old));
                //qDebug()<<"                                                       jump to "<<1+((int)itp-(int)program.begin())/int(sizeof(*itp_old));
#endif
                break;
            case bcBJumpRel:
                itp-=(itp->intpar+1);
#ifdef JKMATHPARSER_BYTECODESTACK_DEBUGMESSAGES
                //qDebug()<<"                                                    @"<<((int)itp_old-(int)program.begin())/int(sizeof(*itp_old));
                //qDebug()<<"                                                       bjump to "<<1+((int)itp-(int)program.begin())/int(sizeof(*itp_old));
#endif
                break;

            case bcJumpCondRel:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<1) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in JMPCONDREL opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    if (resultStack.pop()!=0.0) {
#ifdef JKMATHPARSER_BYTECODESTACK_DEBUGMESSAGES
                        //qDebug()<<"                                                    @"<<((int)itp_old-(int)program.begin())/int(sizeof(*itp_old));
#endif
                        itp+=(itp->intpar-1);
#ifdef JKMATHPARSER_BYTECODESTACK_DEBUGMESSAGES
                        //qDebug()<<"                                                       jumpcond to "<<1+((int)itp-(int)program.begin())/int(sizeof(*itp_old));
#endif
                    }
                }
                break;
            case bcBJumpCondRel:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                if (resultStack.size()<1) {
                    jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error in BJMPCONDREL opcode: stack is too small"));
                    ok=false;
                } else
#endif
                {
                    if (resultStack.pop()!=0.0) {
#ifdef JKMATHPARSER_BYTECODESTACK_DEBUGMESSAGES
                        //qDebug()<<"                                                    @"<<((int)itp_old-(int)program.begin())/int(sizeof(*itp_old));
#endif
                        itp-=(itp->intpar+1);
#ifdef JKMATHPARSER_BYTECODESTACK_DEBUGMESSAGES
                        //qDebug()<<"                                                       bjumpcond to "<<1+((int)itp-(int)program.begin())/int(sizeof(*itp_old));
#endif
                    }
                }
                break;

            default:
#ifdef JKMATHPARSER_BYTECODESTACK_ERRORCHECKING
                jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: unknown opcode %1 encountered").arg(itp->opcode));
                ok=false;
#endif
                break;
        }

#ifdef JKMATHPARSER_BYTECODESTACK_DEBUGMESSAGES
        //qDebug()<<((int)itp_old-(int)program.begin())/int(sizeof(*itp_old))<<": "<<printBytecode(*itp_old)<<"   stack_after = "<<arrayToString(resultStack.data(), resultStack.size());
#endif

        ++itp;
    }
    if (ok) {
        if (resultStack.size()>0) {
            return resultStack.back();
        } else {
            jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: no result returned"));
            return NAN;
        }
    } else {
        jkmpError(JKMP::_("JKMathParser Bytecode Interpreter: error running program"));
        return NAN;
    }
}

JKMP::string JKMathParser::printBytecode(const JKMathParser::ByteCodeInstruction &inst)
{
    JKMP::string res="";

    switch (inst.opcode) {
        case bcNOP:
            res+=JKMP::string("NOP");
            break;
        case bcPush:
            res+=JKMP::string("PUSH %1").arg(inst.numpar);
            break;
        case bcCallCFunction:
            res+=JKMP::string("CALLCFUNCTION 0x%1, %2").arg((uint64_t)inst.pntpar,0,16).arg(inst.intpar);
            break;
        case bcCallCMPFunction:
            res+=JKMP::string("CALLCMPFUNCTION 0x%1, %2").arg((uint64_t)inst.pntpar,0,16).arg(inst.intpar);
            break;
        case bcCallResultFunction:
            res+=JKMP::string("CALLRESULTFUNCTION %1, %2").arg(inst.strpar).arg(inst.intpar);
            break;
        case bcJumpRel:
            res+=JKMP::string("JMPREL %1").arg(inst.intpar);
            break;
        case bcJumpCondRel:
            res+=JKMP::string("JMPCONDREL %1").arg(inst.intpar);
            break;
        case bcBJumpRel:
            res+=JKMP::string("BJMPREL %1").arg(inst.intpar);
            break;
        case bcBJumpCondRel:
            res+=JKMP::string("BJMPCONDREL %1").arg(inst.intpar);
            break;
        case bcPop:
            res+=JKMP::string("POP");
            break;
        case bcVarRead:
            res+=JKMP::string("VARREAD 0x%1").arg((uint64_t)inst.pntpar,0,16);
            break;
        case bcVarWrite:
            res+=JKMP::string("VARWRITE 0x%1").arg((uint64_t)inst.pntpar,0,16);
            break;
        case bcAdd:
            res+=JKMP::string("ADD");
            break;
        case bcSub:
            res+=JKMP::string("SUB");
            break;
        case bcMul:
            res+=JKMP::string("MUL");
            break;
        case bcDiv:
            res+=JKMP::string("DIV");
            break;
        case bcMod:
            res+=JKMP::string("MOD");
            break;
        case bcPow:
            res+=JKMP::string("POW");
            break;
        case bcBitAnd:
            res+=JKMP::string("BITAND");
            break;
        case bcBitOr:
            res+=JKMP::string("BITOR");
            break;
        case bcBitNot:
            res+=JKMP::string("BITNOT");
            break;
        case bcNeg:
            res+=JKMP::string("NEG");
            break;
        case bcLogicAnd: res+=JKMP::string("LOGICAND"); break;
        case bcLogicOr: res+=JKMP::string("LOGICOR"); break;
        case bcLogicNot: res+=JKMP::string("LOGICNOT"); break;
        case bcLogicXor: res+=JKMP::string("LOGICXOR"); break;
        case bcCmpEqual: res+=JKMP::string("CMPEQUAL"); break;
        case bcCmpLesser: res+=JKMP::string("CMPLESSER"); break;
        case bcCmpLesserEqual: res+=JKMP::string("CMPLESSEREQUAL"); break;
        case bcHeapRead: res+=JKMP::string("HEAPREAD %1").arg(inst.intpar); break;
        case bcHeapWrite: res+=JKMP::string("HEAPWRITE %1").arg(inst.intpar); break;
        default:
            res+=JKMP::string("*** UNKNOWN *** %1").arg(inst.opcode);
            break;
    }
    return res;
}

JKMP::string JKMathParser::printBytecode(const JKMathParser::ByteCodeProgram &program)
{
    JKMP::string res="";
    for (size_t i = 0; i < program.size(); ++i) {
        const ByteCodeInstruction& inst=program[i];
        res+=JKMP::string("%1: %2\n").arg(JKMP::inttostr(i, 10, char(' '))).arg(printBytecode(inst));
    }
    return res;
}


JKMathParser::ByteCodeInstruction::ByteCodeInstruction(JKMathParser::ByteCodes opcode)
{
    this->opcode=opcode;
}

JKMathParser::ByteCodeInstruction::ByteCodeInstruction(JKMathParser::ByteCodes opcode, double respar)
{
    this->opcode=opcode;
    this->numpar=respar;
}


JKMathParser::ByteCodeInstruction::ByteCodeInstruction(JKMathParser::ByteCodes opcode, int intpar)
{
    this->opcode=opcode;
    this->intpar=intpar;
}

JKMathParser::ByteCodeInstruction::ByteCodeInstruction(JKMathParser::ByteCodes opcode, void *pntpar)
{
    this->opcode=opcode;
    this->pntpar=pntpar;
}

JKMathParser::ByteCodeInstruction::ByteCodeInstruction(JKMathParser::ByteCodes opcode, void *pntpar, int intpar)
{
    this->opcode=opcode;
    this->pntpar=pntpar;
    this->intpar=intpar;
}

JKMathParser::ByteCodeInstruction::ByteCodeInstruction(JKMathParser::ByteCodes opcode, JKMP::string strpar, int intpar)
{
    this->opcode=opcode;
    this->strpar=strpar;
    this->intpar=intpar;
}

JKMathParser::ByteCodeEnvironment::ByteCodeEnvironment(JKMathParser *parser)
{
    this->parser=parser;
    heapItemPointer=0;
}

void JKMathParser::ByteCodeEnvironment::init(JKMathParser */*parser*/)
{
    heapItemPointer=0;
}

int JKMathParser::ByteCodeEnvironment::pushVar(const JKMP::string &name)
{
    heapVariables[name].push_back(heapItemPointer);
    heapItemPointer++;
    return heapItemPointer-1;
}

void JKMathParser::ByteCodeEnvironment::popVar(const JKMP::string &name)
{
    if (heapVariables[name].size()<=1) {
        heapVariables.erase(name);
    } else {
        heapVariables[name].pop_back();
    }
    heapItemPointer--;
}

jkmpResult JKMathParser::jkmpNode::evaluate()
{
    jkmpResult res;
    evaluate(res);
    return res;
}

void JKMathParser::jkmpNode::evaluate(JKMP::vector<jkmpResult> &result)
{
    result.clear();
    result.push_back(jkmpResult());
    evaluate(result[0]);
}

JKMP::string JKMathParser::jkmpNode::print() const
{
    return JKMP::string();
}

JKMP::string JKMathParser::jkmpNode::printTree(int /*level*/) const
{
    return JKMP::string();
}


JKMathParser::jkmpVectorAccessNode::~jkmpVectorAccessNode()
{
    if (index) delete index;
    if (left) delete left;
}

JKMathParser::jkmpVectorAccessNode::jkmpVectorAccessNode(JKMathParser::jkmpNode *left, JKMathParser::jkmpNode *index, JKMathParser *p, JKMathParser::jkmpNode *par):
    jkmpNode(p, par)
{
    this->left=left;
    this->index=index;
    if (index) index->setParent(this);
    if (left) left->setParent(this);
}

void JKMathParser::jkmpVectorAccessNode::evaluate(jkmpResult &res)
{
    jkmpResult  idx, var;
    if (index) index->evaluate(idx);
    res.isValid=true;

    if (left) left->evaluate(var);
    if (var.type==jkmpList) {
        if (idx.convertsToBoolVector()) {
            JKMP::vector<bool> ii=idx.asBoolVector();
            if (ii.size()!=var.listData.size()) {
                getParser()->jkmpError(JKMP::_("list element access by boolean-vectors needs a boolean vector of the same size, as the data vector, but: index: %1, data: %2 elements").arg(ii.size()).arg(var.listData.size()));
                res.setInvalid();
                return;
            }
            int trues=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) trues++;
            }
            res.setList(trues);
            int j=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) {
                    res.listData[j]=var.listData[i];
                    j++;
                }
            }
        } else if (idx.convertsToIntVector()) {
            JKMP::vector<int> ii=idx.asIntVector();
            if (ii.size()==0) {
                getParser()->jkmpError(JKMP::_("list element access needs non-empty number vector as index, but index is %1").arg(idx.toTypeString()));
                res.setInvalid();
                return;
            }
            res.setList(ii.size());
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]>=0 && ii[i]<var.listData.size()) {
                    res.listData[i]=var.listData[ii[i]];
                } else {
                    getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to access element %1, but list has only %2 elements").arg(ii[i]).arg(var.listData.size()));
                    res.setInvalid();
                    return;
                }
            }
            if (res.listData.size()==1) res=res.listData[0];

        } else {
            getParser()->jkmpError(JKMP::_("list element access needs number/number vector or boolean/boolean vector as index, but index is %1").arg(resultTypeToString(idx.type)));
            res.setInvalid();
            return;
        }
    } else if (var.type==jkmpDoubleVector) {
        if (idx.convertsToBoolVector()) {
            JKMP::vector<bool> ii=idx.asBoolVector();
            if (ii.size()!=var.numVec.size()) {
                getParser()->jkmpError(JKMP::_("vector element access by boolean-vectors needs a boolean vector of the same size, as the data vector, but: index: %1, data: %2 elements").arg(ii.size()).arg(var.numVec.size()));
                res.setInvalid();
                return;
            }
            int trues=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) trues++;
            }
            res.setDoubleVec(trues);
            int j=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) {
                    res.numVec[j]=var.numVec[i];
                    j++;
                }
            }
        } else if (idx.convertsToIntVector()) {
            JKMP::vector<int> ii=idx.asIntVector();
            if (ii.size()==0) {
                getParser()->jkmpError(JKMP::_("vector element access needs non-empty number vector as index, but index is %1").arg(idx.toTypeString()));
                res.setInvalid();
                return;
            }
            res.type=jkmpDoubleVector;
            res.numVec=JKMP::vector<double>(ii.size(), 0);
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]>=0 && ii[i]<var.numVec.size()) {
                    res.numVec[i]=var.numVec[ii[i]];
                } else {
                    getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to access element %1, but vector has only %2 elements").arg(ii[i]).arg(var.numVec.size()));
                    res.setInvalid();
                    return;
                }
            }
            if (res.numVec.size()==1) res.setDouble(res.numVec[0]);

        } else {
            getParser()->jkmpError(JKMP::_("vector element access needs number/number vector or boolean/boolean vector as index, but index is %1").arg(resultTypeToString(idx.type)));
            res.setInvalid();
            return;
        }
    } else if (var.type==jkmpStringVector) {
        if (idx.convertsToBoolVector()) {
            JKMP::vector<bool> ii=idx.asBoolVector();
            if (ii.size()!=var.strVec.size()) {
                getParser()->jkmpError(JKMP::_("vector element access by boolean-vectors needs a boolean vector of the same size, as the data vector, but: index: %1, data: %2 elements").arg(ii.size()).arg(var.numVec.size()));
                res.setInvalid();
                return;
            }
            int trues=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) trues++;
            }
            res.setStringVec(trues);
            int j=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) {
                    res.strVec[j]=var.strVec[i];
                    j++;
                }
            }
        } else if (idx.convertsToIntVector()) {
            JKMP::vector<int> ii=idx.asIntVector();
            if (ii.size()==0) {
                getParser()->jkmpError(JKMP::_("vector element access needs non-empty number vector as index, but index is %1").arg(idx.toTypeString()));
                res.setInvalid();
                return;
            }
            res.type=jkmpStringVector;
            res.strVec.clear();
            //res.strVec.resize(ii.size(), JKMP::string());
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]>=0 && ii[i]<var.strVec.size()) {
                    res.strVec<<var.strVec[ii[i]];
                } else {
                    getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to access element %1, but vector has only %2 elements").arg(ii[i]).arg(var.strVec.size()));
                    res.setInvalid();
                    return;
                }
            }
            if (res.strVec.size()==1) res.setString(res.strVec[0]);

        } else {
            getParser()->jkmpError(JKMP::_("vector element access needs number/number vector or boolean/boolean vector as index, but index is %1").arg(resultTypeToString(idx.type)));
            res.setInvalid();
            return;
        }
    } else if (var.type==jkmpBoolVector) {
        if (idx.convertsToBoolVector()) {
            JKMP::vector<bool> ii=idx.asBoolVector();
            if (ii.size()!=var.boolVec.size()) {
                getParser()->jkmpError(JKMP::_("vector element access by boolean-vectors needs a boolean vector of the same size, as the data vector, but: index: %1, data: %2 elements").arg(ii.size()).arg(var.numVec.size()));
                res.setInvalid();
                return;
            }
            int trues=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) trues++;
            }
            res.setBoolVec(trues);
            int j=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) {
                    res.boolVec[j]=var.boolVec[i];
                    j++;
                }
            }
        } else if (idx.convertsToIntVector()) {
            JKMP::vector<int> ii=idx.asIntVector();
            if (ii.size()==0) {
                getParser()->jkmpError(JKMP::_("vector element access needs non-empty number vector as index, but index is %1").arg(idx.toTypeString()));
                res.setInvalid();
                return;
            }
            res.type=jkmpBoolVector;
            res.boolVec=JKMP::vector<bool>(ii.size(), 0);
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]>=0 && ii[i]<var.boolVec.size()) {
                    res.boolVec[i]=var.boolVec[ii[i]];
                } else {
                    getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to access element %1, but vector has only %2 elements").arg(ii[i]).arg(var.boolVec.size()));
                    res.setInvalid();
                    return;
                }
            }
            if (res.boolVec.size()==1) res.setBoolean(res.boolVec[0]);

        } else {
            getParser()->jkmpError(JKMP::_("vector element access needs number/number vector or boolean/boolean vector as index, but index is %1").arg(resultTypeToString(idx.type)));
            res.setInvalid();
            return;
        }
    } else if (var.type==jkmpString) {
        if (idx.convertsToBoolVector()) {
            JKMP::vector<bool> ii=idx.asBoolVector();
            if (ii.size()!=var.str.size()) {
                getParser()->jkmpError(JKMP::_("vector element access by boolean-vectors needs a boolean vector of the same size, as the data vector, but: index: %1, data: %2 elements").arg(ii.size()).arg(var.numVec.size()));
                res.setInvalid();
                return;
            }
            int trues=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) trues++;
            }
            res.setString(trues);
            int j=0;
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]) {
                    res.str[j]=var.str[i];
                    j++;
                }
            }
        } else if (idx.convertsToIntVector()) {
            JKMP::vector<int> ii=idx.asIntVector();
            if (ii.size()==0) {
                getParser()->jkmpError(JKMP::_("vector element access needs non-empty number vector as index, but index is %1").arg(idx.toTypeString()));
                res.setInvalid();
                return;
            }
            res.type=jkmpString;
            res.str="";
            for (size_t i=0; i<ii.size(); i++) {
                if (ii[i]>=0 && ii[i]<var.str.size()) {
                    res.str+=var.str[ii[i]];
                } else {
                    getParser()->jkmpError(JKMP::_("OUT OF RANGE: trying to access element %1, but string has only %2 elements").arg(ii[i]).arg(var.str.size()));
                    res.setInvalid();
                    return;
                }
            }

        } else {
            getParser()->jkmpError(JKMP::_("string character access needs number/number vector or boolean/boolean vector as index, but index is %1").arg(resultTypeToString(idx.type)));
            res.setInvalid();
            return;
        }
    } else {
        getParser()->jkmpError(JKMP::_("vector element/string character access is only possible if the expression is a number/string/bool vector, or a string, but expression is of type %1").arg(resultTypeToString(var.type)));
        res.setInvalid();
        return;
    }
}

JKMathParser::jkmpNode *JKMathParser::jkmpVectorAccessNode::copy(JKMathParser::jkmpNode *par)
{
    return new jkmpVectorAccessNode(left->copy(NULL), index->copy(NULL), getParser(), par);
}

JKMP::string JKMathParser::jkmpVectorAccessNode::print() const
{
    return JKMP::string("(%1)[%2]").arg(left->print()).arg(index->print());
}

JKMP::string JKMathParser::jkmpVectorAccessNode::printTree(int level) const
{
    return JKMP::string(2*level, char(' '))+JKMP::string("VectorAccessNode:\n%1\n%2").arg(left->printTree(level+1)).arg(index->printTree(level+1));
}


JKMathParser::jkmpStructAccessNode::~jkmpStructAccessNode()
{
    if (left) delete left;
}

JKMathParser::jkmpStructAccessNode::jkmpStructAccessNode(JKMathParser::jkmpNode *left, const JKMP::string &index, JKMathParser *p, JKMathParser::jkmpNode *par):
    jkmpNode(p, par)
{
    this->left=left;
    this->index=index;
    if (left) left->setParent(this);
}

void JKMathParser::jkmpStructAccessNode::evaluate(jkmpResult &result)
{
    jkmpResult var;

    if (left) left->evaluate(var);
    if (var.type==jkmpStruct) {
        if (var.structData.contains(index)) {
            result=var.structData[index];
            return;
        } else {
            getParser()->jkmpError(JKMP::_("struct element access: item '%1' does not exist (available elements: %2).").arg(index).arg(JKMP::stringVector(var.structData.keys()).join(", ")));
            result.setInvalid();
            return;
        }

    } else {
        getParser()->jkmpError(JKMP::_("struct element access: left-hand-side expression hast to be a struct, but expression is of type %1").arg(resultTypeToString(var.type)));
        result.setInvalid();
        return;
    }
}

JKMathParser::jkmpNode *JKMathParser::jkmpStructAccessNode::copy(JKMathParser::jkmpNode *par)
{
    return new jkmpStructAccessNode(left->copy(NULL), index, getParser(), par);
}

JKMP::string JKMathParser::jkmpStructAccessNode::print() const
{
    return JKMP::string("(%1).%2").arg(left->print()).arg(index);
}

JKMP::string JKMathParser::jkmpStructAccessNode::printTree(int level) const
{
    return JKMP::string(2*level, char(' '))+JKMP::string("StructAccessNode '%2':\n%1").arg(left->printTree(level+1)).arg(index);}


void JKMathParser::jkmpListConstruction::evaluate(jkmpResult &res)
{
    res.setList();
    if (list.size()>0) {
        for (size_t i=0; i<list.size(); i++) {
            res.listData.push_back(jkmpResult::invalidResult());
            list[i]->evaluate(res.listData.back());
            if (!res.listData.back().isValid) {
                res.setInvalid();
                getParser()->jkmpError(JKMP::_("list construction node: trying to add invalid item (%1. item) to a list").arg(i+1));
                return;
            }
        }
    }
}

JKMathParser::jkmpNode *JKMathParser::jkmpListConstruction::copy(JKMathParser::jkmpNode *par)
{
    JKMathParser::jkmpNodeList* n= new JKMathParser::jkmpListConstruction(getParser(), par);
    if (list.size()>0) {
        for (size_t i=0; i<list.size(); i++) {
            n->add(list[i]->copy(n));
        }
    }

    n->setParent(par);
    return n;
}

bool JKMathParser::jkmpListConstruction::createByteCode(JKMathParser::ByteCodeProgram &/*program*/, JKMathParser::ByteCodeEnvironment */*environment*/)
{
    if (getParser()) getParser()->jkmpError(JKMP::_("no list constructs in byte-code allowed"));
    return false;
}

JKMP::string JKMathParser::jkmpListConstruction::print() const
{
    JKMP::stringVector sl;
    for (size_t i=0; i<list.size(); i++) {
        sl<<list[i]->print();
    }
    return JKMP::string("{ %1 }").arg(sl.join(", "));
}

JKMP::string JKMathParser::jkmpListConstruction::printTree(int level) const
{
    JKMP::stringVector sl;
    if (list.size()>0) {
        for (size_t i=0; i<list.size(); i++) {
            sl<<list[i]->printTree(level+1);
        }
    }
    return JKMP::string(2*level, char(' '))+JKMP::string("ListConstruction\n%1").arg(sl.join("\n"));
}
