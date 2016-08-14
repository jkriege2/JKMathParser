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

#include "jkmathparserstringtools.h"
#include <sstream>
#include <locale>
#include <iomanip>

JKMP::string JKMP::_(const string &s)
{
    argstring res=s;

    return res;
}

JKMP::string JKMP::string::arg(const JKMP::string &s1) const
{
    stringVector res;
    res.push_back("");
    std::vector<size_t> nums;
    size_t lowestNum=100;
    char dig1=0;
    char dig2=0;
    const string digs="0123456789";
    for (size_t i=0; i<s1.size(); i++) {
        if (s1[i]=='%' && i+1<s1.size() && digs.contains(s1[i+1])) {
            const size_t p=i;
            dig1=s1[i];
            i++;
            if (i+1<s1.size() && digs.contains(s1[i+1])) {
                dig2=s1[i];
                i++;
            }
            size_t n=dig1;
            if (dig2>0) {
                n=dig1*10+dig2;
            }
            if (n<lowestNum) {
                lowestNum=n;
                nums.push_back(n);
                res.push_back("%");
                res.push_back("");
            }
        } else {
            res.back().push_back(c);
        }
    }

    if (nums.size()==0) return s1;

    string ress;
    size_t jn=0;
    for (size_t i=0; i<res.size(); i++) {
        if (res[i]=="%") {
            if (nums[jn]==lowestNum) {
                ress+=s1;
            } else {
                ress+=("%"+std::to_string(nums[jn]));
            }
            jn++;
        } else {
            ress+=res[i];
        }
    }
    return ress;
}

JKMP::string JKMP::booltostr(bool v) {
    return v?_("true"):_("false");
}

JKMP::string JKMP::inttostr(int64_t v) {
    std::stringstream s;
    s.imbue(std::locale("C"));
    s<<v;
    return s.str();
}

JKMP::string JKMP::inttostr(uint64_t v) {
    std::stringstream s;
    s.imbue(std::locale("C"));
    s<<v;
    return s.str();
}

JKMP::string JKMP::floattostr(double v, int past_comma, bool remove_trail0, double belowIsZero)  {
    if (belowIsZero>0) {
        if (fabs(data)<belowIsZero) return string("0");
    }
    std::stringstream s;
    s.imbue(std::locale("C"));
    s.precision(past_comma);
    s<<v;
    string r=s.str();

    if (remove_trail0 && (tolower(r).find('e')==string::npos)) {
        if (data==0) return "0";
        //size_t cp=r.find(".");
        //if (cp<r.size()) return r;
        string re;
        size_t dpos=r.find('.');
        if (dpos==string::npos) {
            return r;
        } else {
            long i=r.size()-1;
            bool nonz=false;
            while (i>=0) {
                //std::cout<<i<<"\n";
                if (r[i]!='0') {
                    nonz=true;
                }
                if (nonz || (i<long(dpos))) {
                    if (re.size()==0 && r[i]=='.') {
                        // swallow decimal dot, if only 0 folowed
                    } else {
                        re=r[i]+re;
                    }
                }
                i--;
            }
            return re;
        }

    }
    return r;
}


JKMP::string JKMP::inttohex(uint64_t v)
{
    std::stringstream s;
    s.imbue(std::locale("C"));
    s<<std::hex<<v;
    return s.str();
}

JKMP::string JKMP::inttobin(uint64_t v)
{
    std::stringstream s;
    s.imbue(std::locale("C"));
    s<<std::bin<<v;
    return s.str();
}

JKMP::string JKMP::inttooct(uint64_t v)
{
    std::stringstream s;
    s.imbue(std::locale("C"));
    s<<std::oct<<v;
    return s.str();
}


bool JKMP::strtobool(const std::string& data){
    std::string d=tolower(data);
    if (d=="true") return true;
    if (d=="t") return true;
    if (d=="1") return true;
    if (d=="j") return true;
    if (d=="y") return true;
    if (d=="yes") return true;
    if (d=="ja") return true;
    if (d=="on") return true;
    return false;
}


double JKMP::stringtofloat(const std::string& data){
    std::istringstream s(data);
    s.imbue(std::locale("C"));
    double v;
    s>>v;
    if (s.bad() || !s.eof()) {
        throw std::runtime_error("could not convert '"+data+"' to an integer");
    }
    return v;
}

std::string JKMP::tolower(const std::string& s){
  std::string d;
  d="";
  std::locale loc;
  if (s.length()>0) {
    for (unsigned long i=0; i<s.length(); i++) {
        d+=std::tolower(s[i],loc);
    }
  }
  return d;
}

std::string JKMP::toupper(const std::string& s){
  std::string d;
  d="";
  std::locale loc;
  if (s.length()>0) {
    for (unsigned long i=0; i<s.length(); i++) {
        d+=std::toupper(s[i], loc);
    }
  }
  return d;
}

int64_t JKMP::stringtoint(const std::string &data) {
    std::istringstream s(data);
    s.imbue(std::locale("C"));
    int64_t v;
    s>>v;
    if (s.bad() || !s.eof()) {
        throw std::runtime_error("could not convert '"+data+"' to an integer");
    }
    return v;

}
