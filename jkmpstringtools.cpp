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

#include "jkmpstringtools.h"
#include <fstream>
#include <locale>
#include <iomanip>
#include <iostream>
#include <cmath>

JKMP::string JKMP::_(const string &s)
{
    return s;
}

JKMP::string JKMP::string::arg(const JKMP::string &s_param) const
{
    JKMP::string s1=*this;
    //std::cout<<"\n\n\n### '"<<*this<<"'.arg("<<s1<<")\n";
    stringVector res;
    res.push_back("");
    std::vector<size_t> nums;
    size_t lowestNum=100;
    JKMP::charType dig1=0;
    JKMP::charType dig2=0;
    const string digs="0123456789";
    for (size_t i=0; i<s1.size(); i++) {
        //std::cout<<"   ### s1["<<i<<"] = '"<<s1[i]<<"'\n";
        if (s1[i]=='%' && i+1<s1.size() && digs.contains(s1[i+1])) {
            dig1=s1[i+1];
            //std::cout<<"   ### dig1 = "<<dig1<<"\n";
            i++;
            if (i+1<s1.size() && digs.contains(s1[i+1])) {
                dig2=s1[i+1];
                //std::cout<<"   ### dig2 = "<<dig2<<"\n";
                i++;
            }
            size_t n=dig1-'0';
            if (dig2>0) {
                n=dig1*10+dig2-'0';
            }
            //std::cout<<"   ### n = '"<<n<<"\n";
            if (n<lowestNum) {
                lowestNum=n;
            }
            nums.push_back(n);
            res.push_back("%");
            res.push_back("");
        } else {
            res.back().push_back(s1[i]);
        }
    }

    /*for (size_t i=0; i<res.size(); i++) {
        std::cout<<"### res["<<i<<"] = '"<<res[i]<<"'\n";
    }
    for (size_t i=0; i<nums.size(); i++) {
        std::cout<<"### nums["<<i<<"] = "<<nums[i]<<"\n";
    }
    std::cout<<"### lowestNum = "<<lowestNum<<"\n";*/

    if (nums.size()==0) return s1;

    string ress;
    size_t jn=0;
    for (size_t i=0; i<res.size(); i++) {
        if (res[i]=="%") {
            if (nums[jn]==lowestNum) {
                ress+=s_param;
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

JKMP::string &JKMP::string::replace(const JKMP::string &before, const JKMP::string &after, bool caseSensitive)
{
    size_t it;
    size_t pos=0;
    do {
        if (caseSensitive) it=this->find(before, pos);
        else it=this->toLower().find(before.toLower());
        if (it!=npos) {
            replace(it, it+before.size(), after);
            pos+=after.size();
        }
    } while (it!=npos);
    return *this;
}

JKMP::stringVector JKMP::string::split(const JKMP::string &sep) const
{
    JKMP::stringVector res;
    res.push_back("");
    for (size_t i=0; i<size(); i++) {
        //std::cout<<"a["<<i<<"] = '"<<(*this)[i]<<"'\n";
        //std::cout<<"substr("<<i<<", "<<sep.size()<<") = '"<<substr(i, sep.size())<<"'\n";
        if (this->substr(i, sep.size())==sep) {
            res.push_back("");
            i+=(sep.size()-1);
        } else {
            res.back().push_back((*this)[i]);
        }
        if (i>=size()) break;
    }
    return res;
}


JKMP::string JKMP::string::trimmed() const
{
    JKMP::string res=*this;
    size_t i0=0;
    size_t N=res.size();
    while (i0<res.size() && isspace(res[i0])) {
        i0++;
        N--;
    }
    size_t i1=res.size()-1;
    while (i1<res.size() && isspace(res[i1])) {
        i1--;
        N--;
    }

    return res.substr(i0, N);
}

JKMP::string JKMP::string::toLower() const
{
    return JKMP::toLower(*this);
}

JKMP::string JKMP::string::toUpper() const
{
    return JKMP::toUpper(*this);
}

JKMP::string JKMP::boolToStr(bool v) {
    return v?_("true"):_("false");
}

JKMP::string JKMP::intToStr(int64_t v, int fieldLEngth, JKMP::charType fillc) {
    JKMP::stringstreamType s;
    s.imbue(std::locale("C"));
    if (fieldLEngth>0) s<<std::setw(fieldLEngth);
    s<<std::setfill(fillc);
    s<<v;
    return s.str();
}

JKMP::string JKMP::uintToStr(uint64_t v, int fieldLEngth, JKMP::charType fillc) {
    JKMP::stringstreamType s;
    s.imbue(std::locale("C"));
    if (fieldLEngth>0) s<<std::setw(fieldLEngth);
    s<<std::setfill(fillc);
    s<<v;
    return s.str();
}

JKMP::string JKMP::floatToStr(double data, int past_comma, int fieldLEngth, JKMP::charType fillc, bool remove_trail0, double belowIsZero)  {
    if (belowIsZero>0) {
        if (std::fabs(data)<belowIsZero) return string("0");
    }
    JKMP::stringstreamType s;
    s.imbue(std::locale("C"));
    if (past_comma>0) s.precision(past_comma);
    if (fieldLEngth>0) s<<std::setw(fieldLEngth);
    s<<std::setfill(fillc);
    s<<data;
    string r=s.str();

    if (remove_trail0 && (toLower(r).find('e')==string::npos)) {
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


JKMP::string JKMP::intToHex(uint64_t v, int fieldLEngth, JKMP::charType fillc)
{
    JKMP::stringstreamType s;
    s.imbue(std::locale("C"));
    s<<std::hex;
    if (fieldLEngth>0) s<<std::setw(fieldLEngth);
    s<<std::setfill(fillc);
    s<<v;
    return s.str();
}

JKMP::string JKMP::intToBin(uint64_t v, int fieldLEngth, JKMP::charType fillc)
{
    JKMP::stringstreamType s;
    s.imbue(std::locale("C"));
    JKMP::string r;
    if (v==0) r="v";
    else {
        while(v!=0) {r=(v%2==0 ?JKMP::string("0"):JKMP::string("1"))+r; v/=2;}
    }
    if (fieldLEngth>0) s<<std::setw(fieldLEngth);
    s<<std::setfill(fillc);
    s<<r;
    return s.str();
}

JKMP::string JKMP::intToOct(uint64_t v, int fieldLEngth, JKMP::charType fillc)
{
    JKMP::stringstreamType s;
    s.imbue(std::locale("C"));
    s<<std::oct;
    if (fieldLEngth>0) s<<std::setw(fieldLEngth);
    s<<std::setfill(fillc);
    s<<v;
    return s.str();
}


bool JKMP::strToBool(const JKMP::stringType& data){
    JKMP::stringType d=toLower(data);
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


double JKMP::strToFloat(const JKMP::stringType& data){
    std::istringstream s(data);
    s.imbue(std::locale("C"));
    double v;
    s>>v;
    if (s.bad() || !s.eof()) {
        throw std::runtime_error("could not convert '"+data+"' to an integer");
    }
    return v;
}

JKMP::stringType JKMP::toLower(const JKMP::stringType& s){
  JKMP::stringType d;
  d="";
  std::locale loc;
  if (s.length()>0) {
    for (unsigned long i=0; i<s.length(); i++) {
        d+=std::tolower(s[i],loc);
    }
  }
  return d;
}

JKMP::stringType JKMP::toUpper(const JKMP::stringType& s){
  JKMP::stringType d;
  d="";
  std::locale loc;
  if (s.length()>0) {
    for (unsigned long i=0; i<s.length(); i++) {
        d+=std::toupper(s[i], loc);
    }
  }
  return d;
}

int64_t JKMP::strToInt(const JKMP::stringType &data) {
    std::istringstream s(data);
    s.imbue(std::locale("C"));
    int64_t v;
    s>>v;
    if (s.bad() || !s.eof()) {
        throw std::runtime_error("could not convert '"+data+"' to an integer");
    }
    return v;

}


JKMP::stringType JKMP::escapify(JKMP::stringType text){
  JKMP::stringType res="";
  if (text.size()>0) {
    for (size_t i=0; i<text.size(); i++)
      switch((JKMP::charType)text[i]) {
        case '\0': res+="\\0"; break;
        case '\n': res+="\\n"; break;
        case '\r': res+="\\r"; break;
        case '\t': res+="\\t"; break;
        case '\\': res+="\\\\"; break;
        case '"': res+="\\\""; break;
        case '\'': res+="\\'"; break;
        case '\a': res+="\\a"; break;
        case '\b': res+="\\b"; break;
        case '\v': res+="\\v"; break;
        case '\f': res+="\\f"; break;
        case '\e': res+="\\e"; break;
        case '\?': res+="\\?"; break;
        default:
          if ((unsigned JKMP::charType)text[i]<32) {
            res+="\\x"+intToHex((unsigned JKMP::charType)text[i], 2, '0');
          } else res+=text[i];
          break;
      };
  }
  return res;
}


JKMP::stringType JKMP::deescapify(JKMP::stringType text){
  JKMP::stringType res="";
  if (text.size()>0) {
    unsigned int i=0;
    while (i<text.size()) {
      if (text[i]!='\\') {
        res+=text[i];
      } else {
        if (i+1<text.size()) {
          JKMP::charType next=text[i+1];
          switch(next) {
            case '0': res+='\0'; i++; break;
            case 'n': res+='\n'; i++; break;
            case 'r': res+='\r'; i++; break;
            case 't': res+='\t'; i++; break;
            case 'a': res+='\a'; i++; break;
            case 'b': res+='\b'; i++; break;
            case 'v': res+='\v'; i++; break;
            case 'f': res+='\f'; i++; break;
            case 'e': res+='\e'; i++; break;
            case '\\': res+='\\'; i++; break;
            case '"': res+='\"'; i++; break;
            case '?': res+='\?'; i++; break;
            case '\'': res+='\''; i++; break;
            case 'x':
            case 'X':
              if (i+3<text.size()) {
                JKMP::stringType num=text.substr(i+2,2);
                i+=3;
                res+=(JKMP::charType)strtol(num.c_str(), NULL, 16);
              } else i++;
              break;
          }
        }
      }
      i++;
    }
  }
  return res;
}

JKMP::string JKMP::charToStr(JKMP::charType v)
{
    std::ostringstream ost;
    ost<<v;
    return ost.str();
}

JKMP::stringType JKMP::stringVector::join(const JKMP::stringType &sep) const
{
    JKMP::stringType res;
    if (size()>0) {
        res=(*this)[0];
    }
    for (size_t i=1; i<this->size(); i++) {
        res+=sep;
        res+=(*this)[i];
    }
    return res;
}

JKMP::stringType JKMP::readFile(const JKMP::stringType &fn)
{
    JKMP::stringType res;
    std::ifstream is(fn);     // open file
    if (is) {
        JKMP::charType c;
        while (is.get(c))   {        // loop getting single characters
            res.push_back(c);
        }

        is.close();
    }
    return res;
}

int64_t JKMP::hexToInt(const JKMP::stringType &data)
{
    return strtol(data.c_str(), NULL, 16);
}

int64_t JKMP::binToInt(const JKMP::stringType &data)
{
    return strtol(data.c_str(), NULL, 2);
}

int64_t JKMP::octToInt(const JKMP::stringType &data)
{
    return strtol(data.c_str(), NULL, 8);
}

JKMP::stringType JKMP::doubleVecToStr(const std::vector<double> &value, int prec, const JKMP::stringType itemSeparator)
{
    JKMP::stringType out;
    for (size_t i=0; i<value.size(); i++) {
        if (i>0) out+=itemSeparator;
        const JKMP::stringType res=floatToStr(value[i], prec, -1, ' ', false, 1e-307);
        out+=res;
    }
    return out;
}

JKMP::stringType JKMP::doubleMatrixToStr(const std::vector<double> &value, size_t columns, int prec, const JKMP::stringType &itemSeparator, const JKMP::stringType &columnSeparator)
{
    JKMP::stringType out;
    bool first=true;
    for (size_t i=0; i<value.size(); i++) {
        if (!first) out+=itemSeparator;
        first=false;
        const JKMP::stringType res=floatToStr(value[i], prec, 2*prec, ' ', false, 1e-307);
        out+= res;
        if (i<value.size()-1 && (i%columns)==(columns-1)) {
            out+=columnSeparator;
            first=true;
        }
    }
    return out;
}

JKMP::stringType JKMP::boolMatrixToStr(const std::vector<bool> &value, size_t columns, const JKMP::stringType &itemSeparator, const JKMP::stringType &columnSeparator, const JKMP::stringType &trueName, const JKMP::stringType &falseName)
{
    JKMP::stringType out;
    bool first=true;
    for (size_t i=0; i<value.size(); i++) {
        if (!first) out+=itemSeparator;
        first=false;
        const JKMP::stringType res=value[i]?trueName:falseName;
        out+= res;
        if (i<value.size()-1 && (i%columns)==(columns-1)) {
            out+=columnSeparator;
            first=true;
        }
    }
    return out;
}

JKMP::stringType JKMP::boolVecToStr(const std::vector<bool> &value, const JKMP::stringType itemSeparator, const JKMP::stringType &trueName, const JKMP::stringType &falseName)
{
    JKMP::stringType out;
    for (size_t i=0; i<value.size(); i++) {
        if (i>0) out+=itemSeparator;
        const JKMP::stringType res=value[i]?trueName:falseName;
        out+=res;
    }
    return out;
}
