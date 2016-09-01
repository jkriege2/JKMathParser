#include "../jkmathparser.h"
#include "../jkmptools.h"
#include "../jkmpmathtools.h"
#include "../StatisticsTools/statistics_tools.h"
#include "../libTermcolor/include/termcolor/termcolor.hpp"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "ticktock.h"

#ifdef _WINDOWS_
#  include <stdio.h>
#  include <wchar.h>
#  include <windows.h>
#  include <Wincon.h>
#  ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#    define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#  endif



//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString()
{
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

#else
std::string GetLastErrorAsString()
{
    return std::string();
}
#endif

template <class T, typename F>
T foreach_apply(const T& input, F f) {
    T res=input;
    for (auto it=res.begin(); it!=res.end(); ++it) {
        *it=f(*it);
    }
    return res;
}


#define qDebug() std::cout<<std::endl


#define TEST_CPP(expr, result, cnt, cntPASS, cntFAIL) {\
    qDebug()<<"-------------------------------------------------------------------------------------"; \
    auto res=expr; \
    qDebug()<<#expr<<"       =  "<<res<<"\n"; \
    cnt++;\
    if (res!=result) { \
            qDebug()<<"   ERROR: result was "<<res<<", but expected "<<result<<"" ;\
            qDebug()<<"                                                                       "<<termcolor::red<<"FAILED!!!"<<termcolor::reset<<"\n\n" ;\
            cntFAIL++; \
    } else {\
            qDebug()<<"                                                                       "<<termcolor::green<<"PASSED!!!"<<termcolor::reset<<"\n\n" ;\
            cntPASS++; \
    }\
  }

#define TEST_CPP_TRUE(expr, cnt, cntPASS, cntFAIL) TEST_CPP(expr, true, cnt, cntPASS, cntFAIL)
#define TEST_CPP_FALSE(expr, cnt, cntPASS, cntFAIL) TEST_CPP(expr, false, cnt, cntPASS, cntFAIL)


#define TEST_BOOL(expr, boolres, cnt, cntPASS, cntFAIL) {\
    parser.resetErrors(); \
    JKMathParser::jkmpNode* n=parser.parse(expr); \
    jkmpResult r=n->evaluate(); \
    qDebug()<<"-------------------------------------------------------------------------------------"; \
    qDebug()<<expr<<"       =  "<<r.toTypeString()<<"\n"; \
    cnt++;\
    if (parser.hasErrorOccured()) { \
            qDebug()<<"   "<<parser.getLastErrorCount()<<" ERROR: "<<parser.getLastErrors().join("\n    ")<<"" ;\
            qDebug()<<"                                                                       "<<termcolor::red<<"FAILED!!!"<<termcolor::reset<<"\n\n" ;\
            cntFAIL++; \
    } else {\
        if (r.getType()==jkmpBool && r.boolean==boolres) {\
            qDebug()<<"                                                                       "<<termcolor::green<<"PASSED!!!"<<termcolor::reset<<"\n\n" ;\
            cntPASS++; \
        } else {\
            qDebug()<<"   ERROR: result was "<<r.toTypeString()<<", but expected true [bool]" ;\
            qDebug()<<"                                                                       "<<termcolor::red<<"FAILED!!!"<<termcolor::reset<<"\n\n" ;\
            cntFAIL++; \
        }\
    }\
  }

#define TEST_TRUE(expr, cnt, cntPASS, cntFAIL) TEST_BOOL(expr, true, cnt, cntPASS, cntFAIL)
#define TEST_FALSE(expr, cnt, cntPASS, cntFAIL) TEST_BOOL(expr, false, cnt, cntPASS, cntFAIL)


#define TEST_CMPDBL_FULL(expr, expectedresult, cnt, cntPASS, cntFAIL, JKMPTYPE, CPPTYPE, GETFUN) {\
    parser.resetErrors(); \
    JKMathParser::jkmpNode* n=parser.parse(expr); \
    jkmpResult r=n->evaluate(); \
    qDebug()<<"-------------------------------------------------------------------------------------"; \
    qDebug()<<expr<<"       =  "<<r.toTypeString()<<"\n"; \
    const CPPTYPE expected=static_cast<CPPTYPE>(expectedresult); \
    cnt++;\
    if (parser.hasErrorOccured()) { \
            qDebug()<<"   "<<parser.getLastErrorCount()<<" ERROR: "<<parser.getLastErrors().join("\n    ")<<"" ;\
            qDebug()<<"                                                                       "<<termcolor::red<<"FAILED!!!"<<termcolor::reset<<"\n\n" ;\
            cntFAIL++; \
    } else {\
        if (r.getType()==JKMPTYPE && r.GETFUN==expected) {\
            qDebug()<<"                                                                       "<<termcolor::green<<"PASSED!!!"<<termcolor::reset<<"\n\n" ;\
            cntPASS++; \
        } else {\
            qDebug()<<"   ERROR: result was "<<r.toTypeString()<<", but expected "<<expected<<" [" << #JKMPTYPE << "]" ;\
            qDebug()<<"                                                                       "<<termcolor::red<<"FAILED!!!"<<termcolor::reset<<"\n\n" ;\
            cntFAIL++; \
        }\
    }\
  }

#define TEST_CMPDBL(expr, expectedresult, cnt, cntPASS, cntFAIL) TEST_CMPDBL_FULL(expr, expectedresult, cnt, cntPASS, cntFAIL, jkmpDouble, double, num)
#define TEST_CMPDBL1(expr, cnt, cntPASS, cntFAIL) TEST_CMPDBL(#expr, expr, cnt, cntPASS, cntFAIL)
#define TEST_CMPBOOL(expr, expectedresult, cnt, cntPASS, cntFAIL) TEST_CMPDBL_FULL(expr, expectedresult, cnt, cntPASS, cntFAIL, jkmpBool, bool, boolean)
#define TEST_CMPBOOL1(expr, cnt, cntPASS, cntFAIL) TEST_CMPBOOL(#expr, expr, cnt, cntPASS, cntFAIL)
#define TEST_CMPSTR(expr, expectedresult, cnt, cntPASS, cntFAIL) TEST_CMPDBL_FULL(expr, expectedresult, cnt, cntPASS, cntFAIL, jkmpString, std::string, str)
#define TEST_CMPSTR1(expr, cnt, cntPASS, cntFAIL) TEST_CMPSTR(#expr, expr, cnt, cntPASS, cntFAIL)
#define TEST_CMPSTRVEC(expr, expectedresult, cnt, cntPASS, cntFAIL) TEST_CMPDBL_FULL(expr, expectedresult, cnt, cntPASS, cntFAIL, jkmpStringVector, JKMP::stringVector, strVec)
#define TEST_CMPBOOLVEC(expr, expectedresult, cnt, cntPASS, cntFAIL) TEST_CMPDBL_FULL(expr, expectedresult, cnt, cntPASS, cntFAIL, jkmpBoolVector, JKMP::vector<bool>, boolVec)
#define TEST_CMPDBLVEC(expr, expectedresult, cnt, cntPASS, cntFAIL) TEST_CMPDBL_FULL(expr, expectedresult, cnt, cntPASS, cntFAIL, jkmpDoubleVector, JKMP::vector<double>, numVec)


#define TEST_VOID(expr, cnt, cntPASS, cntFAIL) {\
    parser.resetErrors(); \
    JKMathParser::jkmpNode* n=parser.parse(expr); \
    jkmpResult r=n->evaluate(); \
    qDebug()<<"-------------------------------------------------------------------------------------"; \
    qDebug()<<expr<<"       =  "<<r.toTypeString()<<"\n"; \
    cnt++;\
    if (parser.hasErrorOccured()) { \
            qDebug()<<"   "<<parser.getLastErrorCount()<<" ERROR: "<<parser.getLastErrors().join("\n    ")<<"" ;\
            qDebug()<<"                                                                       "<<termcolor::red<<"FAILED!!!"<<termcolor::reset<<"\n\n" ;\
            cntFAIL++; \
    } else {\
        if (r.getType()==jkmpVoid) {\
            qDebug()<<"                                                                       "<<termcolor::green<<"PASSED!!!"<<termcolor::reset<<"\n\n" ;\
            cntPASS++; \
        } else {\
            qDebug()<<"   ERROR: result was "<<r.toTypeString()<<", but expected void [void]" ;\
            qDebug()<<"                                                                       "<<termcolor::red<<"FAILED!!!"<<termcolor::reset<<"\n\n" ;\
            cntFAIL++; \
        }\
    }\
  }


#define TEST_ERROR(expr, cnt, cntPASS, cntFAIL) {\
    parser.resetErrors(); \
    JKMathParser::jkmpNode* n=parser.parse(expr); \
    jkmpResult r=n->evaluate(); \
    qDebug()<<"-------------------------------------------------------------------------------------"; \
    qDebug()<<expr<<"       =  "<<r.toTypeString()<<"\n"; \
    cnt++;\
    if (parser.hasErrorOccured()) { \
            qDebug()<<"   "<<parser.getLastErrorCount()<<" ERROR: "<<parser.getLastErrors().join("\n    ")<<"" ;\
            qDebug()<<"                                                                       "<<termcolor::green<<"PASSED!!!"<<termcolor::reset<<"\n\n" ;\
            cntPASS++; \
    } else {\
            qDebug()<<"   ERROR expected, but result was "<<r.toTypeString()<<"" ;\
            qDebug()<<"                                                                       "<<termcolor::red<<"FAILED!!!"<<termcolor::reset<<"\n\n" ;\
            cntFAIL++; \
    }\
  }


int main(int /*argc*/, JKMP::charType */*argv*/[])
{

#ifdef _WINDOWS_
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        std::cerr<<"GetStdHandle: "<< GetLastError()<<": "<<GetLastErrorAsString();
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        std::cerr<<"GetConsoleMode: "<< GetLastError()<<": "<<GetLastErrorAsString();
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        std::cerr<<"SetConsoleMode: "<< GetLastError()<<": "<<GetLastErrorAsString();
    }
#endif
    JKMathParser parser;
    const double pi=M_PI;
    int cnt=0;
    int cntFAIL=0;
    int cntPASS=0;

    JKMP::stringVector sv;
    TEST_CPP_TRUE(sv.size()==0, cnt, cntPASS, cntFAIL);
    sv<<"Hello";
    TEST_CPP_TRUE(sv.size()==1, cnt, cntPASS, cntFAIL);
    sv<<"World!";
    TEST_CPP_TRUE(sv.size()==2, cnt, cntPASS, cntFAIL);
    TEST_CPP_TRUE(sv.contains("Hello", true), cnt, cntPASS, cntFAIL);
    TEST_CPP_TRUE(sv.contains("hello", false), cnt, cntPASS, cntFAIL);
    TEST_CPP_FALSE(sv.contains("hello", true), cnt, cntPASS, cntFAIL);
    TEST_CPP(sv.join(""), "HelloWorld!", cnt, cntPASS, cntFAIL);
    TEST_CPP(sv.join(" "), "Hello World!", cnt, cntPASS, cntFAIL);

    JKMP::string s;
    TEST_CPP_TRUE(s.size()==0, cnt, cntPASS, cntFAIL);
    s="1234567890";
    TEST_CPP_TRUE(s.contains('0'), cnt, cntPASS, cntFAIL);
    TEST_CPP_TRUE(s.contains('2'), cnt, cntPASS, cntFAIL);
    TEST_CPP_TRUE(s.contains('5'), cnt, cntPASS, cntFAIL);
    TEST_CPP_FALSE(s.contains('\0'), cnt, cntPASS, cntFAIL);
    TEST_CPP_FALSE(s.contains('a'), cnt, cntPASS, cntFAIL);
    s="Hello";
    TEST_CPP_TRUE(s.size()==5, cnt, cntPASS, cntFAIL);
    s<<" "<<"World!";
    TEST_CPP(s, "Hello World!", cnt, cntPASS, cntFAIL);
    TEST_CPP(s.toLower(), "hello world!", cnt, cntPASS, cntFAIL);
    TEST_CPP(s.toUpper(), "HELLO WORLD!", cnt, cntPASS, cntFAIL);
    s="Text%1%2 with 2 args";
    TEST_CPP(s, "Text%1%2 with 2 args", cnt, cntPASS, cntFAIL);
    TEST_CPP(s.arg(1).arg(2), "Text12 with 2 args", cnt, cntPASS, cntFAIL);
    TEST_CPP(s.arg(2).arg(1), "Text21 with 2 args", cnt, cntPASS, cntFAIL);
    s="1,2,3,4,5";
    TEST_CPP(s.split(","), JKMP::stringVector::construct("1", "2", "3", "4", "5"), cnt, cntPASS, cntFAIL);
    TEST_CPP(s.split(",").join(","), s, cnt, cntPASS, cntFAIL);
    TEST_CPP(s.split("."), JKMP::stringVector::construct("1,2,3,4,5"), cnt, cntPASS, cntFAIL);
    s="1,2,3";
    TEST_CPP(s.split(","), JKMP::stringVector::construct("1", "2", "3"), cnt, cntPASS, cntFAIL);
    s="1,2";
    TEST_CPP(s.split(","), JKMP::stringVector::construct("1", "2"), cnt, cntPASS, cntFAIL);
    s="1,2,";
    TEST_CPP(s.split(","), JKMP::stringVector::construct("1", "2", ""), cnt, cntPASS, cntFAIL);
    s="1";
    TEST_CPP(s.split(","), JKMP::stringVector::construct("1"), cnt, cntPASS, cntFAIL);

    qDebug()<<"\n\n========================================================================";
    qDebug()<<" LIBRARY-TEST";
    qDebug()<<" #TESTS    = "<<cnt;
    qDebug()<<"   #PASSED = "<<cntPASS;
    qDebug()<<"   #FAILED = "<<cntFAIL;
    qDebug()<<"========================================================================\n\n\n";

    //return 0;
    cnt=0;
    cntFAIL=0;
    cntPASS=0;

    TEST_CMPDBL("1+2+pi", 1.0+2.0+M_PI, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("1+2*pi", 1.0+2.0*M_PI, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("1*2+pi", 1.0*2.0+M_PI, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(1*(2+pi), cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(sin(2.5*pi), cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(sqrt(sin(2.5*pi)), cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(-2.5e-45, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(2.5e-45, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(000.001, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(2.8e45, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(0b11010, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(~0b11010, cnt, cntPASS, cntFAIL);
    TEST_ERROR("0b112010", cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(0xFF, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(0xFF&0x0F, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(0xFF&0x0F&0x02, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(0x01&0x0F&0x02, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(0x01|0x0F&0x02, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1((0x01|0x0F)&0x02, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(0x01|(0x0F&0x02), cnt, cntPASS, cntFAIL);
    TEST_TRUE("1==1", cnt, cntPASS, cntFAIL);
    TEST_TRUE("1!=2", cnt, cntPASS, cntFAIL);
    TEST_TRUE("1<2", cnt, cntPASS, cntFAIL);
    TEST_TRUE("1<=2", cnt, cntPASS, cntFAIL);
    TEST_TRUE("1<=1", cnt, cntPASS, cntFAIL);
    TEST_FALSE("1>2", cnt, cntPASS, cntFAIL);
    TEST_FALSE("1>=2", cnt, cntPASS, cntFAIL);
    TEST_TRUE("1>=1", cnt, cntPASS, cntFAIL);
    TEST_TRUE("1.0000001>=1", cnt, cntPASS, cntFAIL);
    TEST_TRUE("1.0000001>1", cnt, cntPASS, cntFAIL);
    TEST_TRUE("10.0>1", cnt, cntPASS, cntFAIL);
    TEST_TRUE("10.0>=1", cnt, cntPASS, cntFAIL);
    TEST_TRUE("10.0>=10.0", cnt, cntPASS, cntFAIL);
    TEST_TRUE("true==true", cnt, cntPASS, cntFAIL);
    TEST_TRUE("true!=false", cnt, cntPASS, cntFAIL);
    TEST_ERROR("true>false", cnt, cntPASS, cntFAIL);
    TEST_ERROR("true<false", cnt, cntPASS, cntFAIL);
    TEST_ERROR("true>=false", cnt, cntPASS, cntFAIL);
    TEST_ERROR("true<=false", cnt, cntPASS, cntFAIL);
    TEST_TRUE("\"blabla\"==\"blabla\"", cnt, cntPASS, cntFAIL);
    TEST_FALSE("\"Blabla\"==\"blabla\"", cnt, cntPASS, cntFAIL);
    TEST_TRUE("\"blabla\"==\"blabla\"", cnt, cntPASS, cntFAIL);
    TEST_FALSE("\"Blabla\"==\"blabla\"", cnt, cntPASS, cntFAIL);
    TEST_TRUE("\"abc\"!=\"cba\"", cnt, cntPASS, cntFAIL);
    TEST_TRUE("\"abc\"<\"cba\"", cnt, cntPASS, cntFAIL);
    TEST_TRUE("\"abc\"<=\"abc\"", cnt, cntPASS, cntFAIL);
    TEST_TRUE("\"cba\">\"abc\"", cnt, cntPASS, cntFAIL);
    TEST_TRUE("\"abc\">=\"abc\"", cnt, cntPASS, cntFAIL);
    TEST_TRUE("\"abc\"+\"cba\"==\"abccba\"", cnt, cntPASS, cntFAIL);
    TEST_ERROR("'abc'==\"abc\"", cnt, cntPASS, cntFAIL);
    TEST_ERROR("\"abc\"-\"cba\"", cnt, cntPASS, cntFAIL);
    TEST_ERROR("\"abc\"*\"cba\"", cnt, cntPASS, cntFAIL);
    TEST_ERROR("\"abc\"/\"cba\"", cnt, cntPASS, cntFAIL);
    TEST_ERROR("\"abc\"&\"cba\"", cnt, cntPASS, cntFAIL);
    TEST_ERROR("\"abc\"&&\"cba\"", cnt, cntPASS, cntFAIL);
    TEST_ERROR("\"abc\"|\"cba\"", cnt, cntPASS, cntFAIL);
    TEST_ERROR("\"abc\"||\"cba\"", cnt, cntPASS, cntFAIL);
    TEST_TRUE("true&&true", cnt, cntPASS, cntFAIL);
    TEST_ERROR("true&true", cnt, cntPASS, cntFAIL);
    TEST_FALSE("true&&false", cnt, cntPASS, cntFAIL);
    TEST_ERROR("true&false", cnt, cntPASS, cntFAIL);
    TEST_TRUE("!(true&&false)", cnt, cntPASS, cntFAIL);
    TEST_CMPBOOL1(true&&false||true, cnt, cntPASS, cntFAIL);
    TEST_CMPBOOL1((true&&false)||true, cnt, cntPASS, cntFAIL);
    TEST_CMPBOOL1((!true&&!false)||!true, cnt, cntPASS, cntFAIL);
    TEST_CMPBOOL1(!true&&!false||!true, cnt, cntPASS, cntFAIL);
    TEST_CMPBOOL1(true&&false||false, cnt, cntPASS, cntFAIL);
    TEST_CMPBOOL1(true&&(false||false), cnt, cntPASS, cntFAIL);
    TEST_CMPBOOL1((true&&false)||false, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1((1+2)*3, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(1+(2*3), cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(1+2*3, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1((1-2)/3.0, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(1-(2/3.0), cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(1-2/3.0, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(3*(1-2), cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(3*1-2, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1((3*1)-2, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("2^8", 256, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("2^(8-1)", 128, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("2^8-1", 255, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("2^7*2", 256, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("2^7+2", 130, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("pi^7.5+2", pow(M_PI, 7.5)+2.0, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(1e-300, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(1e-300+1, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(1e-300+1e-100, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL1(-1e300+1, cnt, cntPASS, cntFAIL);
    TEST_TRUE("a=1; b=2; a!=b", cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("a=1; b=2; a+b", 3, cnt, cntPASS, cntFAIL);
    TEST_ERROR("1&true", cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("a=1; 1&a", 1, cnt, cntPASS, cntFAIL);
    TEST_TRUE("a=true; a",  cnt, cntPASS, cntFAIL);
    TEST_TRUE("a=true; a||false",  cnt, cntPASS, cntFAIL);
    TEST_FALSE("a=true; a&&false",  cnt, cntPASS, cntFAIL);
    TEST_ERROR("1+\"abc\"",  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("a=\"abc\"; a", "abc",  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("a=\"abc\"; a+a", "abcabc",  cnt, cntPASS, cntFAIL);
    TEST_ERROR("a=\"abc\"; a*a",  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("a=\"abc\"; toupper(a)", "ABC",  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("a=\"aBc\"; toupper(a)+tolower(a)", "ABCabc",  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("[1,2,pi,4,-5.5]", JKMP::vector<double>(1,2,M_PI,4,-5.5),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("a=[1,2,pi,4,-5.5]; a[0]", 1,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("a=[1,2,pi,4,-5.5]; a[1]", 2,  cnt, cntPASS, cntFAIL);
    TEST_ERROR("a=[1,2,pi,4,-5.5]; a[0,1]",  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("a=[1,2,pi,4,-5.5]; a[[0,1]]", JKMP::vector<double>(1.0,2.0),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("a=[1,2,pi,4,-5.5]; a[0:2]", JKMP::vector<double>(1,2, M_PI),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("a=[1,2,pi,4,-5.5]; a[[true,true,false,false,true]]", JKMP::vector<double>(1,2,-5.5),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("a=[1,2,pi,4,-5.5]; b=[true,true,false,false,true]; a[b]", JKMP::vector<double>(1,2,-5.5),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("a=[1,2,pi,4,-5.5]; a[[false,false,false,false,false]]", JKMP::vector<double>(),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("1:5", JKMP::vector<double>(1.0,2.0,3.0,4.0,5.0),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("1:2:5", JKMP::vector<double>(1.0,3.0,5.0),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("1:2.2:6", JKMP::vector<double>(1.0,3.2,5.4),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("1:(-2):(-5)", JKMP::vector<double>(1.0,-1,-3,-5),  cnt, cntPASS, cntFAIL);
    TEST_CMPSTRVEC("a=\"a,B,c\"; split(a, \",\")", JKMP::stringVector::construct("a", "B", "c"),  cnt, cntPASS, cntFAIL);
    TEST_CMPSTRVEC("a=\"a,B,c\"; split(a, \".\")", JKMP::stringVector::construct("a,B,c"),  cnt, cntPASS, cntFAIL);
    TEST_ERROR("a=[\"a,B,c\"]; split(a, \",\")",  cnt, cntPASS, cntFAIL);
    TEST_ERROR("a=[1,2,3]; split(a, \",\")",  cnt, cntPASS, cntFAIL);
    TEST_VOID("ff(x)=x^2",  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("ff(5)", 25,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("ff(1:5)", JKMP::vector<double>(1,4,9,16,25),  cnt, cntPASS, cntFAIL);
    TEST_ERROR("ff(true)",  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("ff(5+5)", 100,  cnt, cntPASS, cntFAIL);
    TEST_VOID("fib(x)=if(x<=1, 1, fib(x-1)+fib(x-2))",  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("fib(8)", 34,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("fib(9)", 55,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("fib(10)", 89,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("fib(5+5)", 89,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("for(i,1,10,fib(i))", JKMP::vector<double>(1,2,3,5,8,13,21,34,55,89),  cnt, cntPASS, cntFAIL);
    TEST_VOID("sf(x,y)=if(x>y, x+x+x, y+y)",  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("sf(\"aa\", \"bb\")", "bbbb",  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("sf(\"cc\", \"bb\")", "cccccc",  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("sf(1,2)", 4, cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("2*sf(9,1)", 54, cnt, cntPASS, cntFAIL);
    TEST_ERROR("sf(\"9\",1)", cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("if(pi==3, \"yes\", \"no\")", "no",  cnt, cntPASS, cntFAIL);
    TEST_CMPBOOL("if(pi>=3, true, \"no\")", true,  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("\"1\"+cases(1>1, \" is >1\", 1>0.5, \" is >0.5\", \"else\")", "1 is >0.5",  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("x=5; cases(x>0,\"0<x<10\", x>10 && x<100,\"10<x<100\", x>100,\"100<x<1000\", \">1000\")", "0<x<10",  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("x=101; cases(x>0,\"0<x<10\", x>10,\"10<x<100\", x>100,\"100<x<1000\", \">1000\")", "0<x<10",  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("x=5; cases((x>0) && (x<10),\"0<x<10\", (x>10) && (x<100),\"10<x<100\", (x>100) && (x<1000),\"100<x<1000\", \">1000\")", "0<x<10",  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("x=101; cases((x>0) && (x<10),\"0<x<10\", (x>10) && (x<100),\"10<x<100\", (x>100) && (x<1000),\"100<x<1000\", \">1000\")", "100<x<1000",  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("x=100; cases((x>0) && (x<10),\"0<x<10\", (x>10) && (x<100),\"10<x<100\", (x>100) && (x<1000),\"100<x<1000\", \">1000\")", ">1000",  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("x=10000; cases((x>0) && (x<10),\"0<x<10\", (x>10) && (x<100),\"10<x<100\", (x>100) && (x<1000),\"100<x<1000\", \">1000\")", ">1000",  cnt, cntPASS, cntFAIL);
    TEST_CMPBOOLVEC("x=[5,99,100,101,1000,10000]; (x>10) && (x<=100)", JKMP::vector<bool>(false,true,true,false,false,false), cnt, cntPASS, cntFAIL);
    TEST_CMPBOOLVEC("x=[5,99,100,101,1000,10000]; x>10 && x<=100", JKMP::vector<bool>(false,true,true,false,false,false), cnt, cntPASS, cntFAIL);
    TEST_ERROR("x=[5,99,100,101,1000,10000]; !x>10 && x<=100", cnt, cntPASS, cntFAIL);
    TEST_CMPBOOLVEC("x=[5,99,100,101,1000,10000]; !(x>10) && x<=100", JKMP::vector<bool>(true,false,false,false,false,false), cnt, cntPASS, cntFAIL);
    TEST_CMPBOOLVEC("x=[5,99,100,101,1000,10000]; (!(x>10)) && (x<=100)", JKMP::vector<bool>(true,false,false,false,false,false), cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("sin(1:10)", foreach_apply(JKMP::construct_vector_range<double>(1,10,1), sin),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("for(i,1,10,sin(i))", foreach_apply(JKMP::construct_vector_range<double>(1,10,1), sin),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("for(i,1:10,sin(i))", foreach_apply(JKMP::construct_vector_range<double>(1,10,1), sin),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("for(i,1,2,10,cos(i))", foreach_apply(JKMP::construct_vector_range<double>(1,10,2), cos),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("cos(1:2:10)", foreach_apply(JKMP::construct_vector_range<double>(1,10,2), cos),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("[]", JKMP::vector<double>(),  cnt, cntPASS, cntFAIL);
    TEST_ERROR("vec=1:5; for(vec)", cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("vec=1:5; sum(vec)", 1+2+3+4+5,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("sum(i,1,5)", 5,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("vec=1:0.5:5; sum(vec)", 1.0+1.5+2.0+2.5+3.0+3.5+4.0+4.5+5.0,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("sum(i,1,0.5,5)", 0,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("sum(i,1:0.5:5,i)", 1.0+1.5+2.0+2.5+3.0+3.5+4.0+4.5+5.0,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("sum(i,1,0.5,5,i)", 1.0+1.5+2.0+2.5+3.0+3.5+4.0+4.5+5.0,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("sum(i,1,0.5,5,i^2)", JKMP::sqr(1.0)+JKMP::sqr(1.5)+JKMP::sqr(2.0)+JKMP::sqr(2.5)+JKMP::sqr(3.0)+JKMP::sqr(3.5)+JKMP::sqr(4.0)+JKMP::sqr(4.5)+JKMP::sqr(5.0),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("vec=1:5; prod(vec)", 1*2*3*4*5,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("prod(i,1,5)", 5,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("vec=1:0.5:5; prod(vec)", 1.0*1.5*2.0*2.5*3.0*3.5*4.0*4.5*5.0,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("prod(i,1,0.5,5)", 0,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("prod(i,1:0.5:5,i)", 1.0*1.5*2.0*2.5*3.0*3.5*4.0*4.5*5.0,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("prod(i,1,0.5,5,i)", 1.0*1.5*2.0*2.5*3.0*3.5*4.0*4.5*5.0,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("prod(i,1,0.5,5,i^2)", JKMP::sqr(1.0)*JKMP::sqr(1.5)*JKMP::sqr(2.0)*JKMP::sqr(2.5)*JKMP::sqr(3.0)*JKMP::sqr(3.5)*JKMP::sqr(4.0)*JKMP::sqr(4.5)*JKMP::sqr(5.0),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("0:(pi/5):(2*pi)", JKMP::construct_vector_range<double>(0,2.0*M_PI, M_PI/5.0),  cnt, cntPASS, cntFAIL);
    TEST_ERROR("0:pi/5:2*pi", cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("0:pi+2*(0:pi)", foreach_apply(JKMP::construct_vector_range<double>(0,M_PI), [](double f) { return f*3.0;}),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("x=0:0.3:10; sum(x)", statisticsSumV(JKMP::construct_vector_range<double>(0,10,0.3)),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("x=0:0.3:10; prod(x)", statisticsProdV(JKMP::construct_vector_range<double>(0,10,0.3)),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("x=0:0.3:10; mean(x)", statisticsAverageV(JKMP::construct_vector_range<double>(0,10,0.3)),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("x=0:0.3:10; var(x)", statisticsVarianceV(JKMP::construct_vector_range<double>(0,10,0.3)),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("x=0:0.3:10; std(x)", sqrt(statisticsVarianceV(JKMP::construct_vector_range<double>(0,10,0.3))),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("x=0:0.3:10; corrcoeff(x, 0:(-0.3):(-10))", statisticsCorrelationCoefficientV(JKMP::construct_vector_range<double>(0,10,0.3), JKMP::construct_vector_range<double>(0,-10,-0.3)),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("median([1])", 1,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("median([1,2])", 1.5,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("median([1,2,3])", 2,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("quantile(1:10,0.25)", 4,  cnt, cntPASS, cntFAIL);
    TEST_CMPDBL("quantile(1:10,0.75)", 9,  cnt, cntPASS, cntFAIL);
    TEST_ERROR("quantile(0.75)", cnt, cntPASS, cntFAIL);
    TEST_ERROR("quantile([]],0.25)", cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("removeall([1,1,1,2,2,3,3,4,5,1], -1)", JKMP::vector<double>(1,1,1,2,2,3,3,4,5,1),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("removeall([1,1,1,2,2,3,3,4,5,1], 1)", JKMP::vector<double>(2,2,3,3,4,5),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("removeall([1,1,1,2,2,3,3,4,5,1], 3)", JKMP::vector<double>(1,1,1,2,2,4,5,1),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("remove(10:(-1):1, 1:3)", JKMP::vector<double>(10,6,5,4,3,2,1),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("remove(10:(-1):1, 0:3)", JKMP::vector<double>(6,5,4,3,2,1),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("reverse(1:10)", JKMP::construct_vector_range<double>(10,1,-1),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("sort(reverse(1:10))", JKMP::construct_vector_range<double>(1,10),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("dsort(reverse(1:10))", JKMP::construct_vector_range<double>(10,1,-1),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("sort([3,2,1])", JKMP::vector<double>::construct(1,2,3),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("sort([3,2])", JKMP::vector<double>::construct(2,3),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("sort([2])", JKMP::vector<double>::construct(2),  cnt, cntPASS, cntFAIL);
    TEST_ERROR("sort()", cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("sort([])", JKMP::vector<double>(), cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("sort([1,2,3])", JKMP::vector<double>(1,2,3),  cnt, cntPASS, cntFAIL);
    TEST_CMPSTRVEC("sort([\"a\",\"bb\",\"ccc\"])", JKMP::stringVector::construct("a","bb","ccc"),  cnt, cntPASS, cntFAIL);
    TEST_CMPSTRVEC("sort([\"bb\",\"a\",\"ccc\"])", JKMP::stringVector::construct("a","bb","ccc"),  cnt, cntPASS, cntFAIL);
    TEST_CMPSTRVEC("sort([\"bb\",\"a\",\"ccc\"])", JKMP::stringVector::construct("a","bb","ccc"),  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("sort(\"bbaaccc\")", JKMP::string("aabbccc"),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("concat(1:2:4, [18,19,20], [3], [1, pi])", JKMP::vector<double>::construct(1,3,18,19,20,3,1,M_PI),  cnt, cntPASS, cntFAIL);
    TEST_CMPDBLVEC("[1:2:4, [18,19,20],  1, pi, 3:(-1):1]", JKMP::vector<double>::construct(1,3,18,19,20,1,M_PI,3,2,1),  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("xx=1:20; sum(i,0,length(xx)-1,int2bin(xx[i])+\"   \")", JKMP::string("1   10   11   100   101   110   111   1000   1001   1010   1011   1100   1101   1110   1111   10000   10001   10010   10011   10100   "),  cnt, cntPASS, cntFAIL);
    TEST_CMPSTR("xx=1:20; sum(i,0,length(xx)-1,int2hex(xx[i])+\"   \")", JKMP::string("1   2   3   4   5   6   7   8   9   a   b   c   d   e   f   10   11   12   13   14   "),  cnt, cntPASS, cntFAIL);

    qDebug()<<"\n\n========================================================================";
    qDebug()<<" PARSER-TEST";
    qDebug()<<" #TESTS    = "<<cnt;
    qDebug()<<"   #PASSED = "<<cntPASS;
    qDebug()<<"   #FAILED = "<<cntFAIL;
    qDebug()<<"========================================================================\n\n\n";

        /*qDebug()<<"\n\n"<<parser.printVariables()<<"\n\n";


        TEST("for(i,1,5,if(i<=3,-i,i^3))");
        TEST("xx=~(1:20)");
        TEST("sum(i,0,length(xx)-1,int2bin(xx[i])+\"   \")");
        TEST("sum(i,~xx,int2hex(i)+\"   \")");
        TEST("sum(i,~xx,int2oct(i)+\"   \")");
        TEST("x=1:5;y=5:(-1):1");
        TEST("x+y");
        TEST("x*y");
        TEST("x/y");
        TEST("x&y");
        TEST("x|y");
        TEST("x%y");
        TEST("-x");
        TEST("x^y");
        TEST("x^2");

        TEST("x=1:5;y=5:(-1):1");
        TEST("x=1:5; y=5:(-1):1");
        TEST("1:5;5:(-1):1");
        TEST("1:1:5;5:(-1):1");
        TEST("[1,2,5];[5,4,3]");
        TEST("x=[1,2,5]; y=[5,4,3]");
        TEST("x=[1,2,5]; [5,4,3]");
        TEST("1; 5");
        TEST("x=1; 5");
        TEST("1; y=5");
        TEST("x=1; y=5");
        TEST("5:(-1):1;1:5");
        TEST("5:(-1):1;1:1:5");
        TEST("1:1:5;5:(-1):1");
        TEST("[1,2,5];[5,4,3]");
        TEST("[1,2,5];[5,4,3];[4,3,pi]");
        TEST("[1,2,5];1:5;[4,3,pi]");
        TEST("[1,2,5];[5,4,3];1:5");
        TEST("x=[1,2,5]; y=[5,4,3]");
        TEST("x=[1,2,5]; [5,4,3]");
        TEST("x=[1,2,5]");
        TEST("[1,2,5]");




        TEST("prod(i,1,5,i)");
        TEST("prod(i,1,2,5,i)");
        TEST("sum(i,1,5,i)");
        TEST("sum(i,1,2,5,i)");
        TEST("sum(i,0,1,5,sum(j,0,3,j^2))");
        TEST("sum(i,0,1,5,sum(i,0,3,j^2))");
        TEST("sum(i,0,1,5,sum(j,0,i,j^2))");
        TEST("sum(i,0,1,5,sum(j,0,3,j^2))");
        TEST("sum(i,0,1,5,sum(i,0,3,i^2))");

        TEST("f(x)=x^2;f(23+a)");
        TEST("fib(x)=if(x<=1, 1, fib(x-1)+fib(x-2)); fib(5)");
        TEST("xx=a; j0(xx)-j0(a)");
        TEST("j1(a)");
        TEST("jn(1,2)");
    }

    if (DO_VECTORS) {
        TEST("x=1:10");
        TEST("x[5]=-1");
        TEST("x");
        TEST("x[1:3]=nan");
        TEST("x");
        TEST("x[[7,9]]=inf");
        TEST("x");
        TEST("[-7,-9]");
        TEST("x[[7,9]]=[-7,-9]");
        TEST("x");
        TEST("x[[1,3,5,7,9]]");
        TEST("x[0:2:9]");
        TEST("x=[\"a\", \"b\", \"c\", \"d\", \"e\", \"f\", \"g\", \"h\", \"i\", \"j\", \"k\", \"l\", \"m\", \"n\"]");
        TEST("x[5]=-1");
        TEST("x");
        TEST("x[5]=\"x\"");
        TEST("x");
        TEST("x[1:3]=\"inf\"");
        TEST("x");
        TEST("x[[7,9]]=\"inf\"");
        TEST("x");
        TEST("x[[7,9]]=[\"7\",\"9\"]");
        TEST("x");
        TEST("x[[1,3,5,7,9]]");
        TEST("x[0:2:9]");
        TEST("x=\"abcdefghijklmnop\"");
        TEST("x[5]=-1");
        TEST("x");
        TEST("x[5]=\"x\"");
        TEST("x");
        TEST("x[1:3]=\"z\"");
        TEST("x");
        TEST("x[[7,9]]=\"z\"");
        TEST("x");
        TEST("x[[7,9]]=[\"7\",\"9\"]");
        TEST("x");
        TEST("x[[1,3,5,7,9]]");
        TEST("x[0:2:9]");
        TEST("x=[true,false,true,false,true,false,true,false,true,false,true,false,true,false,true,false]");
        TEST("x[5]=-1");
        TEST("x");
        TEST("x[1:3]=false");
        TEST("x");
        TEST("x[[7,9]]=false");
        TEST("x");
        TEST("x[[7,9]]=[false,true]");
        TEST("x");
        TEST("x[[1,3,5,7,9]]");
        TEST("x[0:2:9]");
        TEST("concat(1,2,3,4)");
        TEST("concat(\"1\",\"1\",\"1\",\"1\")");
        TEST("removeall([\"1\",\"2\",\"3\",\"1\"], \"1\")");
        TEST("removeall([\"1\",\"2\",\"3\",\"1\"], \"0\")");
        TEST("removeall([\"1\",\"2\",\"3\",\"1\"], \"3\")");
        TEST("concat(true, false, true, false)");
        TEST("concat(1, false, \"a\", false)");
        TEST("concat(true, false, \"a\", false)");
        TEST("x=[1,2,3,4,5,6,7,8,9]");
        TEST("y=reverse(x)");
        TEST("idxb=(trunc(x%2)==0)");
        TEST("idx=find(trunc(x%2)==0)");
        TEST("idx=find(trunc(x%2), 0)");
        TEST("x[idx]");
        TEST("y[idx]");
        TEST("select(y, x%2==0)");
        TEST("num2str(x)");
        TEST("tosystempathseparator(\"bla\\\\bla/bla\")");
        TEST("tosystempathseparator([\"bla\\\\bla/bla\", \"bla/bla\\\\bla\"])");
        TEST("shuffle(x)");
        TEST("shuffle([true, true, true, true, false, false, false, false])");
        TEST("shuffle(num2str(x))");
        TEST("x==x");
        TEST("x==y");
        TEST("x=[1,2,3,4,5,6,7,8,9]");
        TEST("y=[1,8,3.1,4,5,3,7,2,3.1]");
        TEST("unique(x)");
        TEST("unique(y)");
        TEST("idx=[1,3,2,1,2,3,3,2,1]");
        TEST("unique(idx)");
        TEST("idx=[1,2,3,1,2,3,1,2,3]");
        TEST("unique(idx)");
        TEST("indexedmean(x, idx)");
        TEST("indexedsum(x, idx)");
        TEST("indexedsum2(x, idx)");
        TEST("indexedvar(x, idx)");
        TEST("indexedstd(x, idx)");
        TEST("indexedmin(x, idx)");
        TEST("indexedmax(x, idx)");
        TEST("indexedmedian(x, idx)");
        TEST("indexedquantile(x, idx, 0.25)");
        TEST("indexedquantile(x, idx, 0.75)");
        TEST("indexedskewness(x, [1,2,1,2,1,2,1,2,1])");
        TEST("indexedprod(x, idx)");
        TEST("indexedcount(x, idx)");
        TEST("indexedmoment(x, idx, 3)");
        TEST("indexedncmoment(x, idx, 3)");
        TEST("indexedcorrcoeff(x, y, idx)");

        TEST("last(x)");
        TEST("first(x)");
        TEST("last(\"abc\")");
        TEST("first(\"abc\")");
        TEST("last([\"abc\",\"def\",\"ghi\"])");
        TEST("first([\"abc\",\"def\",\"ghi\"])");
        TEST("last([true,true,false])");
        TEST("first([true,true,false])");
        TEST("first(true)");
        TEST("last(false)");
        TEST("returnfirst(a=1,a=a+1,a=a+1,a*2)");
        TEST("returnlast(a=1,a=a+1,a=a+1,a*2)");
        TEST("a");
        TEST("item([1,2,3,4,5,6,7,8,9], [1,3,5])");
        TEST("item([1,2,3,4,5,6,7,8,9], [true,true,true,false,false,false,true,false,true])");
        TEST("item(\"abcdefghij\", [1,3,5])");
        TEST("item(\"abcdefghi\", [true,true,true,false,false,false,true,false,true])");
        TEST("p=[1,2,3,4,5]");
        TEST("polyval(0,p)");
        TEST("polyval([0,1,2],p)");
        TEST("polyder(p)");


        TEST("cumsum(i,1,5,i)");
        TEST("cumsum(i,1:5,i)");
        TEST("cumsum(i,1,2,5,i)");
        TEST("cumsum(i,1:2:5,i)");

        TEST("cumprod(i,1,5,i)");
        TEST("cumprod(i,1:5,i)");
        TEST("cumprod(i,1,2,5,i)");
        TEST("cumprod(i,1:2:5,i)");

        TEST("x=1:10");
        TEST("x[2]");
        TEST("1:10[2]");
        TEST("(1:10)[2]");
        TEST("(2*x)[2]");
        TEST("2*x[2]");
        TEST("(1:10)[x>5]");
        TEST("x[x>5]");
        TEST("\"blablabla\"[[1,2,3]]");
        TEST("\"blablabla\"[1:3]");
        TEST("x[0]:x[2]");
        TEST("1:x[2]");
        TEST("x[0]:5[2]");
        TEST("(x[0]:5)[2]");
        TEST("(x[0]):5[2]");
        TEST("((x[0]):5)[2]");

    }

    if (DO_DATES) {
        TEST("date2str(now())");
        TEST("datetime2str(now())");
        TEST("datetimenum(\"2014-05-01T10:00:00\")");
        TEST("datetimenum(\"2014-05-01T08:00:00\")");
        TEST("datetimenum(\"2014-05-01T10:00:00\")-datetimenum(\"2014-05-01T08:00:00\")");
        TEST("datediff2secs(datetimenum(\"2014-05-01T10:00:00\")-datetimenum(\"2014-05-01T08:00:00\"))");
        TEST("datediff2mins(datetimenum(\"2014-05-01T10:00:00\")-datetimenum(\"2014-05-01T08:00:00\"))");
        TEST("datediff2hours(datetimenum(\"2014-05-03T10:00:00\")-datetimenum(\"2014-05-01T08:00:00\"))");
        TEST("datediff2days(datetimenum(\"2014-05-03T10:00:00\")-datetimenum(\"2014-05-01T08:00:00\"))");
        //
    }

    if (DO_SPECIALS) {
        TEST("varname(pi, p, i, not_existent)");
        parser.addVariable("res1", result);
        parser.addVariable("res2", result2);
        qDebug()<<"\n\n"<<parser.printVariables()<<"\n\n";
        TEST("res1");
        TEST("res2");
        TEST("res1+res2");
        TEST("res3=res1");
        qDebug()<<"\n\n"<<parser.printVariables()<<"\n\n";
        TEST("x=1:10");
        TEST("printexpression(5+5+3*2*x))");
        TEST("printexpressiontree(5+5+3*2*x)");
        TEST("f(x)=if(x<=1, 0, [f(x-1), 1])");
        TEST("printexpression(f(x)=if(x<=1, 0, [f(x-1), 1]))");
        TEST("printexpressiontree(f(x)=if(x<=1, 0, [f(x-1), 1]))");
        TEST("f(5)");
        TEST("fib(x)=if(x<=1, 1, fib(x-1)+fib(x-2))");
        TEST("printexpression(fib(x)=if(x<=1, 1, fib(x-1)+fib(x-2)))");
        TEST("printexpressiontree(fib(x)=if(x<=1, 1, fib(x-1)+fib(x-2)))");
        TEST("for(i,1,10,fib(i))");
        TEST("fib(1)");
        TEST("fib(2)");
        TEST("fib(3)");
        TEST("fib(4)");
    }

    if (DO_STRUCT) {
        TEST("st=struct(\"a\", 1, \"b\", true)");
        TEST("st=struct(st, \"c\", 1:3, \"d\", \"blabla\")");
        TEST("st=struct(st, \"a\", 10, \"b\", 20)");
        TEST("st");
        TEST("printexpressiontree(st.a)");
        TEST("st.a");
        TEST("printexpressiontree(st.d[1:3])");
        TEST("st.d[1:3]");
        TEST("stt=struct(\"a\", struct(\"a\", 1, \"b\", true), \"b\", struct(\"a\", 1, \"b\", true, \"c\", 1:5))");
        TEST("stt.a");
        TEST("stt.a.a");
        TEST("stt.b.c[2:3]");
        TEST("stt.b[2:3]");
        TEST("stt[2:3]");
        TEST("st=struct(\"a\", 1, \"b\", true); st=struct(st, \"c\", 1:3, \"d\", \"blabla\")");
        TEST("structkeys(struct(\"a\", 1, \"b\", true))");
        TEST("structget(struct(\"a\", 1, \"b\", true), \"a\")");
        TEST("structsaveget(struct(\"a\", 1, \"b\", true), \"a\")");
    }

    if (DO_LIST) {
        TEST("l=list()");
        TEST("l2=l=list(\"a\", 1, \"b\", true)");
        TEST("l=listappend(l, \"a\", 1)");
        TEST("l=listremove(l, 1)");
        TEST("l=listremove(l, 0:1)");
        TEST("l=listremove(l, [false,true,false,true,true])");
        TEST("l=listremove(l, [true,false])");
        TEST("l=listinsert(l2, 1, 111)");
        TEST("l=listinsert(l2, 1, 222,333,444)");
        TEST("l=listremove(l2, [false,true,false])");
        TEST("listget(l2, [false,true,false,true])");
        TEST("listget(l2, [1,3])");
        TEST("listget(l2, 0:2)");
        TEST("listget(l2, 1)");
        TEST("listgetsave(l2, [false,true,false,true], \"NON\")");
        TEST("listgetsave(l2, [1,3,5], \"NON\")");
        TEST("listgetsave(l2, -1:2, \"NON\")");
        TEST("listgetsave(l2, 25, \"NON\")");
        TEST("l2=list(\"a\", 1, \"b\", true)");
        TEST("l2[[1,3]]");
        TEST("l2[[false,true,false,true]]");
        TEST("l2[0:2]");
        TEST("l2[0]=\"NON\";");
        TEST("l2[1:2]=\"NON\";");
        TEST("{1,2,4:5,true,[false,true],\"hello\",[\"World\",\"!\"], {\"Hello\",\" World\", \"!\"}}")
        TEST("l={{1},{2},{3}}; for(i,l,listappend(i,pi))")

    }

    if (DO_MATRIX) {
        TEST("m=doublematrix()");
        TEST("m=doublematrix(2)");
        TEST("m=doublematrix(2,3)");
        TEST("m=doublematrix(2,3,pi)");
        TEST("m2=doublematrix(2,3,1)");
        TEST("m3=doublematrix(3,2,1)");
        TEST("size(m)");
        TEST("sizerows(m)");
        TEST("sizecolumns(m)");
        TEST("m+m2");
        TEST("m3+m2");
        TEST("m+1");
        TEST("m==m3");
        TEST("m!=m2");
        TEST("m=[1,2,3;4,5,6;6,7,8]");
        TEST("{size(m), sizerows(m), sizecolumns(m), dimensions(m),ismatrix(m),isvector(m)}");
        TEST("m=[1,2,3;4,5,6;6,7,8,9]");
        TEST("m=[1,2,3,4;4,5,6;6,7,8,9]");
        TEST("m=[1,2,3;4,5;6,7,8]");

        TEST("m=boolmatrix()");
        TEST("m=boolmatrix(2)");
        TEST("m=boolmatrix(2,3)");
        TEST("m=boolmatrix(2,3,true)");
        TEST("m=[true,false,true;false,true,false;true,false,true]");
        TEST("{size(m), sizerows(m), sizecolumns(m), dimensions(m),ismatrix(m),isvector(m)}");
    }*/

    return 0;
}


