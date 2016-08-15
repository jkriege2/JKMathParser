#include "../jkmathparser.h"
#include "../jkmptools.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ticktock.h"



#define qDebug() std::cout<<std::endl


#define TEST_BOOL(expr, boolres, cnt, cntPASS, cntFAIL) {\
    parser.resetErrors(); \
    JKMathParser::jkmpNode* n=parser.parse(expr); \
    jkmpResult r=n->evaluate(); \
    qDebug()<<"-------------------------------------------------------------------------------------"; \
    qDebug()<<expr<<"       =  "<<r.toTypeString()<<"\n"; \
    cnt++;\
    if (parser.hasErrorOccured()) { \
            qDebug()<<"                                                                       FAILED!!!" ;\
            qDebug()<<"   ERROR "<<parser.getLastErrors().join("\n    ")<<"\n\n" ;\
            cntFAIL++; \
    } else {\
        if (r.getType()==jkmpBool && r.boolean==boolres) {\
            qDebug()<<"                                                                       PASSED!!!\n\n" ;\
            cntPASS++; \
        } else {\
            qDebug()<<"                                                                       FAILED!!!" ;\
            qDebug()<<"   ERROR: result was "<<r.toTypeString()<<", but expected true [bool]\n\n" ;\
            cntFAIL++; \
        }\
    }\
  }

#define TEST_TRUE(expr, cnt, cntPASS, cntFAIL) TEST_BOOL(expr, true, cnt, cntPASS, cntFAIL)
#define TEST_FALSE(expr, cnt, cntPASS, cntFAIL) TEST_BOOL(expr, false, cnt, cntPASS, cntFAIL)


#define TEST_CMPDBL(expr, expectedresult, cnt, cntPASS, cntFAIL) {\
    parser.resetErrors(); \
    JKMathParser::jkmpNode* n=parser.parse(expr); \
    jkmpResult r=n->evaluate(); \
    qDebug()<<"-------------------------------------------------------------------------------------"; \
    qDebug()<<expr<<"       =  "<<r.toTypeString()<<"\n"; \
    const double expected=static_cast<double>(expectedresult); \
    cnt++;\
    if (parser.hasErrorOccured()) { \
            qDebug()<<"                                                                       FAILED!!!" ;\
            qDebug()<<"   ERROR "<<parser.getLastErrors().join("\n    ")<<"\n\n" ;\
            cntFAIL++; \
    } else {\
        if (r.getType()==jkmpDouble && r.num==expected) {\
            qDebug()<<"                                                                       PASSED!!!\n\n" ;\
            cntPASS++; \
        } else {\
            qDebug()<<"                                                                       FAILED!!!" ;\
            qDebug()<<"   ERROR: result was "<<r.toTypeString()<<", but expected "<<expected<<" [number]\n\n" ;\
            cntFAIL++; \
        }\
    }\
  }

#define TEST_CMPDBL1(expr, cnt, cntPASS, cntFAIL) TEST_CMPDBL(#expr, expr, cnt, cntPASS, cntFAIL)



#define TEST_CMPERROR(expr, cnt, cntPASS, cntFAIL) {\
    parser.resetErrors(); \
    JKMathParser::jkmpNode* n=parser.parse(expr); \
    jkmpResult r=n->evaluate(); \
    qDebug()<<"-------------------------------------------------------------------------------------"; \
    qDebug()<<expr<<"       =  "<<r.toTypeString()<<"\n"; \
    cnt++;\
    if (parser.hasErrorOccured()) { \
            qDebug()<<"                                                                       PASSED!!!" ;\
            qDebug()<<"   ERROR "<<parser.getLastErrors().join("\n    ")<<"\n\n" ;\
            cntPASS++; \
    } else {\
            qDebug()<<"                                                                       FAILED!!!" ;\
            qDebug()<<"   ERROR expected, but result was "<<r.toTypeString()<<"\n\n" ;\
            cntFAIL++; \
    }\
  }

int main(int argc, JKMP::charType *argv[])
{
    JKMathParser parser;
    const double pi=M_PI;
    int cnt=0;
    int cntFAIL=0;
    int cntPASS=0;


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
    TEST_CMPERROR("0b112010", cnt, cntPASS, cntFAIL);
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
    TEST_TRUE("10.0>1", cnt, cntPASS, cntFAIL);
    TEST_TRUE("10.0>=1", cnt, cntPASS, cntFAIL);
    TEST_TRUE("10.0>=10.0", cnt, cntPASS, cntFAIL);
    TEST_TRUE("true==true", cnt, cntPASS, cntFAIL);
    TEST_TRUE("true!=false", cnt, cntPASS, cntFAIL);
    TEST_CMPERROR("true>false", cnt, cntPASS, cntFAIL);
    TEST_CMPERROR("true<false", cnt, cntPASS, cntFAIL);
    TEST_CMPERROR("true>=false", cnt, cntPASS, cntFAIL);
    TEST_CMPERROR("true<=false", cnt, cntPASS, cntFAIL);

    qDebug()<<"\n\n========================================================================";
    qDebug()<<" #TESTS    = "<<cnt;
    qDebug()<<"   #PASSED = "<<cntPASS;
    qDebug()<<"   #FAILED = "<<cntFAIL;
    qDebug()<<"========================================================================\n\n\n";

        /*qDebug()<<"\n\n"<<parser.printVariables()<<"\n\n";
        TEST("a=5*5+0.1");
        TEST("a=5*5+0.2");
        TEST("b=true&&false");
        TEST("s1=\"blablaBLA\"");
        TEST("s2=\"_BLUB12\"");
        TEST("s=tolower(\"blablaBLA\"+s2)");
        qDebug()<<"\n\n"<<parser.printVariables()<<"\n\n";

        qDebug()<<"\n\n"<<parser.printFunctions()<<"\n\n";
        TEST("ff(x)=x^2");
        TEST("f(x)=ff(x)^2");
        TEST("s(x)=if(x>0, x+s(x-1), 0)");
        TEST("fib(x)=if(x<=1, 1, fib(x-1)+fib(x-2))");
        qDebug()<<"\n\n"<<parser.printFunctions()<<"\n\n";
        TEST("ff(2)+f(2)");

        TEST("s(0)");
        TEST("s(1)");
        TEST("s(2)");
        TEST("s(5)");
        TEST("fib(0)");
        TEST("fib(1)");
        TEST("fib(2)");
        TEST("fib(3)");
        TEST("fib(4)");
        TEST("fib(5)");

        TEST("if(pi==3, \"yes\", \"no\")");
        TEST("if(pi>3, \"is pi\", false)");
        TEST("\"1\"+cases(1>1, \" is >1\", 1>0.5, \" is >0.5\", \"else\")");

        TEST("[]");
        TEST("[1]");
        TEST("[1,2]");
        TEST("[f(2),ff(1),pi^2]");
        TEST("[1,\"bla\"]");
        TEST("1:1");
        TEST("vec=1:5");
        TEST("1:0.5:5");
        TEST("sum(vec)");
        TEST("for(vec)");

        TEST("for(i,0,5,i)");
        TEST("for(i,0:5,i)");
        TEST("sum(i,0,5,i)");
        TEST("sum(i,0:5,i)");
        TEST("for(i,0,2,5,i)");
        TEST("for(i,0:2:5,i)");
        TEST("sum(i,0,2,5,i)");
        TEST("sum(i,0:2:5,i)");

        TEST("for(i,0,5,i^2)");
        TEST("for(i,0:5,i^2)");
        TEST("sum(i,0,5,i^2)");
        TEST("sum(i,0:5,i^2)");
        TEST("for(i,0,2,5,i^2)");
        TEST("for(i,0:2:5,i^2)");
        TEST("sum(i,0,2,5,i^2)");
        TEST("sum(i,0:2:5,i^2)");

        TEST("prod(i,1,5,i)");
        TEST("prod(i,1,5,i^2)");
        TEST("prod(i,1,2,5,i)");
        TEST("prod(i,1,2,5,i^2)");
        TEST("for(i,0,5,i^2)");
        TEST("x=0:(pi/5):(2*pi)");
        TEST("y=x");
        TEST("sin(1)");
        TEST("sin(1:5)");
        TEST("sin(x)");
        TEST("cos(x)");
        TEST("atan2(x,y)");
        TEST("x=1:10");
        TEST("sum(x)");
        TEST("count(x)");
        TEST("mean(x)");
        TEST("var(x)");
        TEST("sqrt(var(x))");
        TEST("std(x)");
        TEST("median(x)");
        TEST("min(x)");
        TEST("max(x)");
        TEST("quantile(x,0.25)");
        TEST("quantile(x,0.75)");


        TEST("removeall(x, -1)");
        TEST("remove(x, 1:3)");
        TEST("x=1:20");
        TEST("reverse(x)");
        TEST("shuffle(x)");
        TEST("dsort(x)");
        TEST("sort(x)");
        TEST("concat(1:2:4, [18,19,20], 3, [1, pi])");
        TEST("[1:2:4, [18,19,20], 3, 1, pi, 3:(-1):1]");
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


