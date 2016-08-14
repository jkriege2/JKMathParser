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
    size_t pos=0;
    do {
        size_t it=this->find(sep, pos);
        if (it!=npos) {
            if (it==pos) {
                res.push_back("");
            } else {
                res.push_back(this->substr(pos, it));
            }
            it+=sep.size();
        }
        pos=it;
    } while (it!=npos);
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
    return tolower(*this);
}

JKMP::string JKMP::string::toUpper() const
{
    return toupper(*this);
}

JKMP::string JKMP::booltostr(bool v) {
    return v?_("true"):_("false");
}

JKMP::string JKMP::inttostr(int64_t v, int fieldLEngth, char fillc) {
    std::stringstream s;
    s.imbue(std::locale("C"));
    if (fieldLEngth>0) s<<std::setw(fieldLEngth);
    s<<std::setfill(fillc);
    s<<v;
    return s.str();
}

JKMP::string JKMP::uinttostr(uint64_t v, int fieldLEngth, char fillc) {
    std::stringstream s;
    s.imbue(std::locale("C"));
    if (fieldLEngth>0) s<<std::setw(fieldLEngth);
    s<<std::setfill(fillc);
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


JKMP::string JKMP::inttohex(uint64_t v, int fieldLEngth, char fillc)
{
    std::stringstream s;
    s.imbue(std::locale("C"));
    s<<std::hex;
    if (fieldLEngth>0) s<<std::setw(fieldLEngth);
    s<<std::setfill(fillc);
    s<<v;
    return s.str();
}

JKMP::string JKMP::inttobin(uint64_t v, int fieldLEngth, char fillc)
{
    std::stringstream s;
    s.imbue(std::locale("C"));
    s<<std::bin<<v;
    if (fieldLEngth>0) s<<std::setw(fieldLEngth);
    s<<std::setfill(fillc);
    s<<;
    return s.str();
}

JKMP::string JKMP::inttooct(uint64_t v, int fieldLEngth, char fillc)
{
    std::stringstream s;
    s.imbue(std::locale("C"));
    s<<std::oct;
    if (fieldLEngth>0) s<<std::setw(fieldLEngth);
    s<<std::setfill(fillc);
    s<<v;
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


std::string JKMP::escapify(std::string text){
  std::string res="";
  if (text.size()>0) {
    for (size_t i=0; i<text.size(); i++)
      switch((char)text[i]) {
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
          if ((unsigned char)text[i]<32) {
            res+="\\x"+inttohex((unsigned char)text[i], 2, '0');
          } else res+=text[i];
          break;
      };
  }
  return res;
}


std::string JKMP::deescapify(std::string text){
  std::string res="";
  if (text.size()>0) {
    unsigned int i=0;
    while (i<text.size()) {
      if (text[i]!='\\') {
        res+=text[i];
      } else {
        if (i+1<text.size()) {
          char next=text[i+1];
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
                std::string num=text.substr(i+2,2);
                i+=3;
                res+=(char)strtol(num.c_str(), NULL, 16);
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

JKMP::string JKMP::chartostr(char v)
{
    std::ostringstream ost;
    ost<<v;
    return ost.str();
}

std::string JKMP::stringVector::join(const std::string &sep) const
{
    std::string res;
    if (size()>0) {
        res=(*this)[0];
    }
    for (size_t i=1; i<this->size(); i++) {
        res+=sep;
        res+=(*this)[i];
    }
    return res;
}

std::string JKMP::readFile(const std::string &fn)
{
    std::string res;
    std::ifstream is(fn);     // open file
    if (is) {
        char c;
        while (is.get(c))   {        // loop getting single characters
            res.push_back(c);
        }

        is.close();
    }
    return res;
}

int64_t JKMP::hextoint(const std::string &data)
{
    return strtol(data.c_str(), NULL, 16);
}

int64_t JKMP::bintoint(const std::string &data)
{
    return strtol(data.c_str(), NULL, 2);
}

int64_t JKMP::octtoint(const std::string &data)
{
    return strtol(data.c_str(), NULL, 8);
}
