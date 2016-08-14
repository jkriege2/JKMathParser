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

#include "jkmathparsertools.h"
#include "jkmathparser.h"
#include <typeinfo>




jkmpResult::jkmpResult()
{
    setInvalid();
}

jkmpResult::~jkmpResult()
{
}

jkmpResult::jkmpResult(double value)
{
    setDouble(value);
}

jkmpResult::jkmpResult(unsigned int value)
{
    setDouble(value);
}

jkmpResult::jkmpResult(int value)
{
    setDouble(value);
}

jkmpResult::jkmpResult(JKMP::string value)
{
    setString(value);
}

jkmpResult::jkmpResult(bool value)
{
    setBoolean(value);
}

jkmpResult::jkmpResult(const JKMP::vector<double>& value)
{
    setDoubleVec(value);
}

jkmpResult::jkmpResult(const JKMP::vector<bool> &value)
{
    setBoolVec(value);
}

jkmpResult::jkmpResult(const JKMP::stringVector &value)
{
    setStringVec(value);
}

jkmpResult::jkmpResult(const jkmpResult &value)
{
    set(value);

}



jkmpResult &jkmpResult::operator=(const jkmpResult &value)
{
    set(value);
    return *this;
}

void jkmpResult::set(const jkmpResult &value)
{
    setInvalid();
    isValid=value.isValid;
    type=value.type;
    num=value.num;
    boolean=value.boolean;
    matrix_columns=value.matrix_columns;
    if (type==jkmpString) this->str=value.str;
    else if (type==jkmpDoubleVector) this->numVec=value.numVec;
    else if (type==jkmpDoubleMatrix) this->numVec=value.numVec;
    else if (type==jkmpStringVector) this->strVec=value.strVec;
    else if (type==jkmpBoolVector) this->boolVec=value.boolVec;
    else if (type==jkmpBoolMatrix) this->boolVec=value.boolVec;
    else if (type==jkmpStruct) this->structData=value.structData;
    else if (type==jkmpList) this->listData=value.listData;
}

void jkmpResult::setInvalid()
{
    matrix_columns=1;
    isValid=false;
    numVec.clear();
    boolVec.clear();
    structData.clear();
    listData.clear();
    strVec.clear();
    str.clear();
    type=jkmpVoid;
}

void jkmpResult::setVoid()
{
    setInvalid();
    isValid=true;
    type=jkmpVoid;
}

JKMP::string jkmpResult::toString(int precision) const
{
    switch(type) {
        case jkmpDouble: return doubleToJKMP::string(num, precision);
        case jkmpDoubleVector: return JKMP::string("[ ")+doubleVecToJKMP::string(numVec, precision, 'g', '.', ", ")+JKMP::string(" ]");
        case jkmpDoubleMatrix: return JKMP::string("[ ")+doubleMatrixToJKMP::string(numVec, matrix_columns, precision,'g', '.', ", ", ";\n")+JKMP::string(" ]");
        case jkmpStringVector: return JKMP::string("[ ")+strVec.join(", ")+JKMP::string(" ]");
        case jkmpBoolVector: return JKMP::string("[ ")+boolvectorToJKMP::string(boolVec, ", ", " true", "false")+JKMP::string(" ]");
        case jkmpBoolMatrix: return JKMP::string("[ ")+boolMatrixToJKMP::string(boolVec, matrix_columns, ", ", ";\n", " true", "false")+JKMP::string(" ]");
        case jkmpString: return str;
        case jkmpBool: return boolToJKMP::string(boolean);
        case jkmpStruct: {
                JKMP::stringVector sl;
                JKMP::mapIterator<JKMP::string,jkmpResult> it(structData);
                while (it.hasNext()) {
                    it.next();
                    sl<<JKMP::string("%1: %2").arg(it.key()).arg(it.value().toString(precision));
                }
                return JKMP::_("{ %1 }").arg(sl.join(", "));
            }
        case jkmpList: {
                JKMP::stringVector sl;
                JKMP::vectorIterator<jkmpResult> it(listData);
                while (it.hasNext()) {
                    sl<<it.next().toString(precision);
                }
                return JKMP::_("{ %1 }").arg(sl.join(", "));
            }
        case jkmpVoid: break;
    }
    return "";

}

JKMP::string jkmpResult::toTypeString(int precision) const
{
    if (!isValid) return JKMP::_("[INVALID]");
    switch(type) {
        case jkmpDouble: return doubleToJKMP::string(num, precision)+JKMP::_(" [number]");
        case jkmpDoubleVector: return JKMP::string("[ ")+doubleVecToJKMP::string(numVec, precision, 'g', '.', ", ")+JKMP::string(" ] [number vector]");
        case jkmpDoubleMatrix: return JKMP::string("[ ")+doubleMatrixToJKMP::string(numVec, matrix_columns, precision,'g', '.', ", ", ";\n")+JKMP::string(" ] [number matrix]");
        case jkmpStringVector: return JKMP::string("[ ")+strVec.join(", ")+JKMP::string(" ] [string vector]");
        case jkmpBoolVector: return JKMP::string("[ ")+boolvectorToJKMP::string(boolVec, ", ", "true", "false")+JKMP::string(" ] [boolean vector]");
        case jkmpBoolMatrix: return JKMP::string("[ ")+boolMatrixToJKMP::string(boolVec, matrix_columns, ", ", ";\n", "true", "false")+JKMP::string(" ] [boolean matrix]");
        case jkmpString: return str+JKMP::_(" [string]");
        case jkmpBool: return boolToJKMP::string(boolean)+JKMP::_(" [bool]");
        case jkmpVoid: return JKMP::_(" [void]");
        case jkmpStruct: {
                JKMP::stringVector sl;
                JKMP::mapIterator<JKMP::string,jkmpResult> it(structData);
                while (it.hasNext()) {
                    it.next();
                    sl<<JKMP::string("%1: %2").arg(it.key()).arg(it.value().toTypeString(precision));
                }
                return JKMP::_("{ %1 } [struct]").arg(sl.join(", "));
            }
        case jkmpList: {
                JKMP::stringVector sl;
                JKMP::vectorIterator<jkmpResult> it(listData);
                while (it.hasNext()) {
                    sl<<it.next().toTypeString(precision);
                }
                return JKMP::_("{ %1 } [list]").arg(sl.join(", "));
            }

    }
    return JKMathParser::resultTypeToString(type);

}

int32_t jkmpResult::toInteger() const
{
    if (type==jkmpDouble) return num;
    if (type==jkmpBool) return (boolean)?1:0;
    return 0;
}

uint32_t jkmpResult::toUInt() const
{
    if (type==jkmpDouble && num>=0) return num;
    if (type==jkmpBool) return (boolean)?1:0;
    return 0;
}

JKMP::vector<uint32_t> jkmpResult::toUIntVector() const
{
    JKMP::vector<uint32_t> l;
    if (type==jkmpDoubleVector) {
        for (int i=0; i<numVec.size(); i++) {
            l<<uint32_t(numVec[i]);
        }
    }
    return l;
}

JKMP::vector<int32_t> jkmpResult::toIntVector() const
{
    JKMP::vector<int32_t> l;
    if (type==jkmpDoubleVector) {
        for (int i=0; i<numVec.size(); i++) {
            l<<int32_t(numVec[i]);
        }
    }
    return l;
}



int jkmpResult::length() const
{
    if (!isValid) return 0;
    switch(type) {
        case jkmpDouble: return 1;
        case jkmpDoubleVector: return numVec.size();
        case jkmpDoubleMatrix: return numVec.size();
        case jkmpBoolVector: return boolVec.size();
        case jkmpBoolMatrix: return boolVec.size();
        case jkmpStringVector: return strVec.size();
        case jkmpString: return str.size();
        case jkmpBool: return 1;
        case jkmpVoid: return 0;
        case jkmpStruct: return structData.size();
        case jkmpList: return listData.size();
    }
    return 0;
}


int jkmpResult::sizeX() const
{
    //if (type==jkmp) return ;
    if (isMatrix()) return matrix_columns;
    return length();
}


int jkmpResult::sizeY() const
{
    //if (type==jkmp) return ;
    if (isMatrix()) return length()/matrix_columns;
    return (length()>0)?1:0;
}


int jkmpResult::dimensions() const
{
    if (!isValid) return 0;
    switch(type) {
        case jkmpDouble: return 1;
        case jkmpDoubleVector: return 1;
        case jkmpDoubleMatrix: return 2;
        case jkmpBoolVector: return 1;
        case jkmpBoolMatrix: return 2;
        case jkmpStringVector: return 1;
        case jkmpString: return 1;
        case jkmpBool: return 1;
        case jkmpVoid: return 0;
        case jkmpStruct: return 1;
        case jkmpList: return 1;
    }
    return 0;
}

bool jkmpResult::isMatrix() const
{
    if (!isValid) return false;
    switch(type) {
        case jkmpDoubleMatrix:
        case jkmpBoolMatrix: return true;
        default: return false;
    }
    return false;
}

bool jkmpResult::isUIntVector() const
{
    if (type==jkmpDoubleVector) {
        for (int i=0; i<numVec.size(); i++) {
            double num=numVec[i];
            if (!(fabs(num)<2147483648.0)&&(num>=0)&&(num==round(num))) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool jkmpResult::isIntVector() const
{
    if (type==jkmpDoubleVector) {
        for (int i=0; i<numVec.size(); i++) {
            double num=numVec[i];
            if (!(fabs(num)<2147483648.0)&&(num==round(num))) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void jkmpResult::setDouble(double val)
{
    setInvalid();
    type=jkmpDouble;
    num=val;
    isValid=true;
}

void jkmpResult::setBoolean(bool val)
{
    setInvalid();
    type=jkmpBool;
    boolean=val;
    isValid=true;
}

void jkmpResult::setString(const JKMP::string &val)
{
    setInvalid();
    type=jkmpString;
    str=val;
    isValid=true;
}

void jkmpResult::setString(int size, char defaultChar)
{
    setInvalid();
    type=jkmpString;
    str=JKMP::string(size, defaultChar);
    isValid=true;
}

void jkmpResult::setDoubleVec(const JKMP::vector<double> &val)
{
    setInvalid();
    isValid=true;
    type=jkmpDoubleVector;
    numVec=val;
}

void jkmpResult::setDoubleVec(int size, double defaultVal)
{
    setInvalid();
    isValid=true;
    type=jkmpDoubleVector;
    if (size>0) {
        numVec.resize(size);
        for (int i=0; i<size; i++) numVec[i]=defaultVal;
    }
}

void jkmpResult::setDoubleMatrix(int size, int cols, double defaultVal)
{
    setInvalid();
    isValid=true;
    type=jkmpDoubleMatrix;
    matrix_columns=cols;
    if (size>0) {
        numVec.resize(size);
        for (int i=0; i<size; i++) numVec[i]=defaultVal;
    }
    //qDebug()<<"double matrix "<<rows()<<columns()<<"  size="<<size<<length()<<" cols="<<cols;
}

void jkmpResult::setStruct(const JKMP::stringVector &items)
{
    setInvalid();
    isValid=true;
    type=jkmpStruct;
    for (int i=0; i<items.size(); i++) {
        structData.insert(std::make_pair(items[i], jkmpResult::invalidResult()));
    }
}

void jkmpResult::setList(int items)
{
    setInvalid();
    isValid=true;
    type=jkmpList;
    for (int i=0; i<items; i++) {
        listData.push_back(jkmpResult::invalidResult());
    }
}

void jkmpResult::setList(const JKMP::vector<jkmpResult> &dat)
{
    setInvalid();
    isValid=true;
    type=jkmpList;
    listData=dat;
}

void jkmpResult::setBoolVec(const JKMP::vector<bool> &val)
{
    setInvalid();
    isValid=true;
    type=jkmpBoolVector;
    boolVec=val;
}

void jkmpResult::setBoolVec(int size, bool defaultVal)
{
    setInvalid();
    isValid=true;
    type=jkmpBoolVector;
    if (size>0) {
        boolVec.resize(size);
        for (int i=0; i<size; i++) boolVec[i]=defaultVal;
    }
}

void jkmpResult::setBoolMatrix(int size, int cols, bool defaultVal)
{
    setInvalid();
    isValid=true;
    type=jkmpBoolMatrix;
    matrix_columns=cols;
    if (size>0) {
        boolVec.resize(size);
        for (int i=0; i<size; i++) boolVec[i]=defaultVal;
    }
}

void jkmpResult::setStringVec(const JKMP::stringVector &val)
{
    setInvalid();
    isValid=true;
    type=jkmpStringVector;
    strVec=val;
}

void jkmpResult::setStringVec(int size, const JKMP::string &defaultVal)
{
    setInvalid();
    isValid=true;
    type=jkmpStringVector;
    if (size>0) {
        strVec.clear();
        for (int i=0; i<size; i++) strVec<<defaultVal;
    }
}

JKMP::vector<double> jkmpResult::asVector() const
{
    if (type==jkmpDoubleVector) return numVec;
    else if (type==jkmpBoolVector) return boolvectorToNumVec(boolVec, 1.0, 0.0);
    else if (type==jkmpDouble) return JKMP::vector<double>(1, num);
    else if (type==jkmpList) {
        bool ok=true;
        JKMP::vector<double> v;
        for (JKMP::vector<jkmpResult>::const_iterator it=listData.begin(); it!=listData.end(); it++) {
            if (it->convertsToDouble()) {
                v<<it->asNumber();
            } else {
                ok=false;
                break;
            }
        }
        if (ok) return v;
    }
    return JKMP::vector<double>();
}


JKMP::stringVector jkmpResult::asStrVector() const
{
    if (type==jkmpStringVector) return strVec;
    else if (type==jkmpString)  return JKMP::stringVector(str);
    return JKMP::stringVector();
}

JKMP::vector<bool> jkmpResult::asBoolVector() const
{
    if (type==jkmpBoolVector) return boolVec;
    else if (type==jkmpBool) return JKMP::vector<bool>(1, boolean);

    return JKMP::vector<bool>();
}

bool jkmpResult::convertsToVector() const
{
    if (type==jkmpDoubleVector || type==jkmpBoolVector || type==jkmpDouble) return true;
    return false;
}

bool jkmpResult::convertsToBoolVector() const
{
    if (type==jkmpBool || type==jkmpBoolVector) return true;
    return false;
}

bool jkmpResult::convertsToStringVector() const
{
    if (type==jkmpStringVector || type==jkmpString) return true;
    return false;
}








jkmpResult jkmpResult::invalidResult()
{
    jkmpResult r;
    r.isValid=false;
    return r;
}

jkmpResult jkmpResult::voidResult()
{
    jkmpResult r;
    r.isValid=true;
    r.type=jkmpVoid;
    return r;
}

jkmpResult jkmpResult::getStructItem(const JKMP::string &item)
{
    jkmpResult res;
    if (type==jkmpStruct) {
        if (structData.contains(item)) res=structData[item];
    }
    return res;
}

void jkmpResult::setStructItem(const JKMP::string &item, const jkmpResult &value)
{
    if (type==jkmpStruct) {
        if (structData.contains(item)) structData[item]=value;
        else structData.insert(std::make_pair(item, value));
    }
}

void jkmpResult::setStruct(const JKMP::map<JKMP::string, jkmpResult> &data)
{
    setStruct();
    structData=data;
}


jkmpResult jkmpResult::getListItem(int item) const
{
    if (type==jkmpList) {
        if (item>=0 && item<listData.size() ) return listData[item];
    }
    return jkmpResult::invalidResult();
}

jkmpResult jkmpResult::getListItem(int item, const jkmpResult &defaultResult) const
{
    if (type==jkmpList) {
        if (item>=0 && item<listData.size() ) return listData[item];
    }
    return defaultResult;
}

void jkmpResult::removeListItem(int item) {
    if (type==jkmpList) {
        if (item>=0 && item<listData.size() ) listData.removeAt(item);
    }
}

void jkmpResult::appendListItem(const jkmpResult& item) {
    if (type==jkmpList) {
        listData.push_back(item);
    }
}

void jkmpResult::insertListItem(int i, const jkmpResult& item) {
    if (type==jkmpList) {
        if (i>=0 && i<listData.size() ) listData.insert(i, item);
        else if (i<0) listData.prepend(item);
        else if (i>=listData.size()) listData.push_back(item);
    }
}


bool jkmpResult::operator!=(const jkmpResult &other) const
{
    return !operator==(other);
}


bool jkmpResult::operator==(const jkmpResult &r) const
{
    if (!isValid && !r.isValid) return true;
    switch((uint32_t(type)<<16)+uint32_t(r.type)) {
        case (jkmpVoid<<16)+jkmpVoid:
            return  true;
            break;
        case (jkmpDouble<<16)+jkmpDouble:
            return(num==r.num);
            break;
        case (jkmpDoubleVector<<16)+jkmpDoubleVector:
            return(numVec.size()==r.numVec.size() && numVec==r.numVec);
            break;
        case (jkmpDoubleMatrix<<16)+jkmpDoubleMatrix:
            return(matrix_columns==r.matrix_columns && numVec.size()==r.numVec.size() && numVec==r.numVec);
            break;
        case (jkmpString<<16)+jkmpString:
            return(str==r.str);
            break;
        case (jkmpBool<<16)+jkmpBool:
            return(boolean==r.boolean);
            break;
        case (jkmpBoolVector<<16)+jkmpBoolVector:
            return(boolVec.size()==r.boolVec.size() && boolVec==r.boolVec);
            break;
        case (jkmpBoolMatrix<<16)+jkmpBoolMatrix:
            return(matrix_columns==r.matrix_columns && boolVec.size()==r.boolVec.size() && boolVec==r.boolVec);
            break;
        case (jkmpStringVector<<16)+jkmpStringVector:
            return(strVec==r.strVec);
            break;
        case (jkmpStruct<<16)+jkmpStruct:
            return(structData==r.structData);
            break;
        case (jkmpList<<16)+jkmpList:
            return(listData==r.listData);
            break;
        default:
            return false;
    }
    return false;
}



//================================================================================================================


void jkmpResult::add(jkmpResult& re, const jkmpResult &l, const jkmpResult &r, JKMathParser* p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    /*qDebug()<<JKMP::string::number(tt, 16);
    //qDebug()<<JKMP::string::number((uint32_t(jkmpDouble)<<16)+jkmpDouble, 16);
    //qDebug()<<JKMP::string::number((uint32_t(jkmpDouble)<<16)+jkmpDoubleVector, 16);
    //qDebug()<<JKMP::string::number((uint32_t(jkmpDoubleVector<<16))+jkmpDoubleVector, 16);
    //qDebug()<<JKMP::string::number((uint32_t(jkmpString)<<16)+jkmpString, 16);*/
    switch(tt) {
        case (uint32_t(jkmpDouble)<<16)+jkmpDouble:
            re.setDouble(l.num+r.num);
            break;
        case (jkmpDoubleVector<<16)+jkmpDouble:
        case (jkmpDoubleMatrix<<16)+jkmpDouble:
            {
            re=l;
            for (int i=0; i<l.numVec.size(); i++) {
                re.numVec[i]=l.numVec[i]+r.num;
            }
            break;
        }
        case (uint32_t(jkmpDouble)<<16)+jkmpDoubleVector:
        case (uint32_t(jkmpDouble)<<16)+jkmpDoubleMatrix:
            {
            re=r;
            for (int i=0; i<r.numVec.size(); i++) {
                re.numVec[i]=l.num+r.numVec[i];
            }
            break;
        }
        case (uint32_t(jkmpDoubleVector<<16))+jkmpDoubleVector:
        case (uint32_t(jkmpDoubleMatrix<<16))+jkmpDoubleMatrix:
            {
                if (l.isMatrix()) {
                    if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                        if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        re.setInvalid();
                        return;
                    }
                } else {
                    if (l.length()!=r.length()) {
                        if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                        re.setInvalid();
                        return;
                    }
                }
                re=r;
                for (int i=0; i<r.numVec.size(); i++) {
                    re.numVec[i]=l.numVec[i]+r.numVec[i];
                }
                break;
            }
        case (uint32_t(jkmpString)<<16)+jkmpString:
            re.setString(l.str+r.str);
            break;
        default:
{
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be added").arg(l.typeName()).arg(r.typeName()));
                re.setInvalid();
            }
            break;
    }
}




void jkmpResult::sub(jkmpResult& re, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    //qDebug()<<JKMP::string::number(tt, 16);
    switch(tt) {
        case (uint32_t(jkmpDouble)<<16)+jkmpDouble:
            re.setDouble(l.num-r.num);
            break;
        case (jkmpDoubleVector<<16)+jkmpDouble:
        case (jkmpDoubleMatrix<<16)+jkmpDouble:
            {
            re=l;
            for (int i=0; i<l.numVec.size(); i++) {
                re.numVec[i]=l.numVec[i]-r.num;
            }
            break;
        }
        case (uint32_t(jkmpDouble)<<16)+jkmpDoubleVector:
        case (uint32_t(jkmpDouble)<<16)+jkmpDoubleMatrix:
            {
            re=r;
            for (int i=0; i<r.numVec.size(); i++) {
                re.numVec[i]=l.num-r.numVec[i];
            }
            break;
        }
        case (uint32_t(jkmpDoubleVector<<16))+jkmpDoubleVector:
        case (uint32_t(jkmpDoubleMatrix<<16))+jkmpDoubleMatrix:
            {
                if (l.isMatrix()) {
                    if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                        if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        re.setInvalid();
                        return;
                    }
                } else {
                    if (l.length()!=r.length()) {
                        if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                        re.setInvalid();
                        return;
                    }
                }
                re=r;
                for (int i=0; i<r.numVec.size(); i++) {
                    re.numVec[i]=l.numVec[i]-r.numVec[i];
                }
                break;
            }
        default:
            {
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be subtracted").arg(l.typeName()).arg(r.typeName()));
                re.setInvalid();
            }
            break;
    }
}

void jkmpResult::mul(jkmpResult& re, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    //qDebug()<<JKMP::string::number(tt, 16);
    switch(tt) {
        case (uint32_t(jkmpDouble)<<16)+jkmpDouble:
            re.setDouble(l.num*r.num);
            break;
        case (jkmpDoubleVector<<16)+jkmpDouble:
        case (jkmpDoubleMatrix<<16)+jkmpDouble:
            {
            re=l;
            for (int i=0; i<l.numVec.size(); i++) {
                re.numVec[i]=l.numVec[i]*r.num;
            }
            break;
        }
        case (uint32_t(jkmpDouble)<<16)+jkmpDoubleVector:
        case (uint32_t(jkmpDouble)<<16)+jkmpDoubleMatrix:
            {
            re=r;
            for (int i=0; i<r.numVec.size(); i++) {
                re.numVec[i]=l.num*r.numVec[i];
            }
            break;
        }
        case (uint32_t(jkmpDoubleVector<<16))+jkmpDoubleVector:
        case (uint32_t(jkmpDoubleMatrix<<16))+jkmpDoubleMatrix:
            {
                if (l.isMatrix()) {
                    if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                        if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        re.setInvalid();
                        return;
                    }
                } else {
                    if (l.length()!=r.length()) {
                        if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                        re.setInvalid();
                        return;
                    }
                }
                re=r;
                for (int i=0; i<r.numVec.size(); i++) {
                    re.numVec[i]=l.numVec[i]*r.numVec[i];
                }
                break;
            }
        default:
            {
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be multiplied").arg(l.typeName()).arg(r.typeName()));
                re.setInvalid();
            }
            break;
    }
}

void jkmpResult::div(jkmpResult& re, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    //qDebug()<<JKMP::string::number(tt, 16);
    switch(tt) {
        case (uint32_t(jkmpDouble)<<16)+jkmpDouble:
            re.setDouble(l.num/r.num);
            break;
        case (jkmpDoubleVector<<16)+jkmpDouble:
        case (jkmpDoubleMatrix<<16)+jkmpDouble:
            {
            re=l;
            for (int i=0; i<l.numVec.size(); i++) {
                re.numVec[i]=l.numVec[i]/r.num;
            }
            break;
        }
        case (uint32_t(jkmpDouble)<<16)+jkmpDoubleVector:
        case (uint32_t(jkmpDouble)<<16)+jkmpDoubleMatrix:
            {
            re=r;
            for (int i=0; i<r.numVec.size(); i++) {
                re.numVec[i]=l.num/r.numVec[i];
            }
            break;
        }
        case (uint32_t(jkmpDoubleVector<<16))+jkmpDoubleVector:
        case (uint32_t(jkmpDoubleMatrix<<16))+jkmpDoubleMatrix:
            {
                if (l.isMatrix()) {
                    if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                        if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        re.setInvalid();
                        return;
                    }
                } else {
                    if (l.length()!=r.length()) {
                        if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                        re.setInvalid();
                        return;
                    }
                }
                re=r;
                for (int i=0; i<r.numVec.size(); i++) {
                    re.numVec[i]=l.numVec[i]/r.numVec[i];
                }
                break;
            }
        default:
            {
                re.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be divided").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}

void jkmpResult::mod(jkmpResult& re, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    //qDebug()<<JKMP::string::number(tt, 16);
    switch(tt) {
        case (jkmpDouble<<16)+jkmpDouble:
            re.setDouble(double(l.toInteger()%r.toInteger()));
            break;
        case (jkmpDoubleVector<<16)+jkmpDouble:
        case (jkmpDoubleMatrix<<16)+jkmpDouble:
            {
            re=l;
            for (int i=0; i<l.numVec.size(); i++) {
                re.numVec[i]=int32_t(l.numVec[i])%r.toInteger();
            }
            break;
        }
        case (jkmpDouble<<16)+jkmpDoubleVector:
        case (jkmpDouble<<16)+jkmpDoubleMatrix:
            {
            re=r;
            for (int i=0; i<r.numVec.size(); i++) {
                re.numVec[i]=l.toInteger()%int32_t(r.numVec[i]);
            }
            break;
        }
        case (jkmpDoubleVector<<16)+jkmpDoubleVector:
        case (jkmpDoubleMatrix<<16)+jkmpDoubleMatrix:
            {
            if (l.isMatrix()) {
                if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                    if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                    re.setInvalid();
                    return;
                }
            } else {
                if (l.length()!=r.length()) {
                    if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                    re.setInvalid();
                    return;
                }
            }
            re=l;
            for (int i=0; i<l.numVec.size(); i++) {
                re.numVec[i]=int32_t(l.numVec[i])%int32_t(r.numVec[i]);
            }
            break;
        }
        default:
            {
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be used in mod operation").arg(l.typeName()).arg(r.typeName()));
                re.setInvalid();
            }
            break;
    }
}

void jkmpResult::power(jkmpResult& re, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    //qDebug()<<JKMP::string::number(tt, 16);
    switch(tt) {
        case (jkmpDouble<<16)+jkmpDouble:
            re.setDouble(pow(l.num,r.num));
            break;
        case (jkmpDoubleVector<<16)+jkmpDouble:
        case (jkmpDoubleMatrix<<16)+jkmpDouble:
            {
            re=l;
            for (int i=0; i<l.numVec.size(); i++) {
                re.numVec[i]=pow(l.numVec[i],r.num);
            }
            break;
        }
        case (jkmpDouble<<16)+jkmpDoubleVector:
        case (jkmpDouble<<16)+jkmpDoubleMatrix:
            {
            re=r;
            for (int i=0; i<r.numVec.size(); i++) {
                re.numVec[i]=pow(l.num,r.numVec[i]);
            }
            break;
        }
        case (jkmpDoubleVector<<16)+jkmpDoubleVector:
        case (jkmpDoubleMatrix<<16)+jkmpDoubleMatrix:
            {
                if (l.isMatrix()) {
                    if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                        if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        re.setInvalid();
                        return;
                    }
                } else {
                    if (l.length()!=r.length()) {
                        if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                        re.setInvalid();
                        return;
                    }
                }

            re=r;
            for (int i=0; i<r.numVec.size(); i++) {
                re.numVec[i]=pow(l.numVec[i],r.numVec[i]);
            }
            break;
        }
        default:
            {
                re.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be used in power operation").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}

void jkmpResult::bitwiseand(jkmpResult& re, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    //qDebug()<<JKMP::string::number(tt, 16);
    switch(tt) {
        case (jkmpDouble<<16)+jkmpDouble:
            re.setDouble(double(l.toInteger()&r.toInteger()));
            break;
        case (jkmpDoubleVector<<16)+jkmpDouble:
        case (jkmpDoubleMatrix<<16)+jkmpDouble:
            {
            re=l;
            for (int i=0; i<l.numVec.size(); i++) {
                re.numVec[i]=int32_t(l.numVec[i])&r.toInteger();
            }
            break;
        }
        case (jkmpDouble<<16)+jkmpDoubleVector:
        case (jkmpDouble<<16)+jkmpDoubleMatrix:
            {
            re=r;
            for (int i=0; i<r.numVec.size(); i++) {
                re.numVec[i]=l.toInteger()&int32_t(r.numVec[i]);
            }
            break;
        }
        case (jkmpDoubleVector<<16)+jkmpDoubleVector:
        case (jkmpDoubleMatrix<<16)+jkmpDoubleMatrix:
            {
                if (l.isMatrix()) {
                    if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                        if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        re.setInvalid();
                        return;
                    }
                } else {
                    if (l.length()!=r.length()) {
                        if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                        re.setInvalid();
                        return;
                    }
                }
            re=r;
            for (int i=0; i<r.numVec.size(); i++) {
                re.numVec[i]=int32_t(l.numVec[i])&int32_t(r.numVec[i]);
            }
            break;
        }
        default:
            {
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be used in bitwise and operation").arg(l.typeName()).arg(r.typeName()));
                re.setInvalid();
            }
            break;
    }
}

void jkmpResult::bitwiseor(jkmpResult& re, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    //qDebug()<<JKMP::string::number(tt, 16);
    switch(tt) {
        case (jkmpDouble<<16)+jkmpDouble:
            re.setDouble(double(l.toInteger()|r.toInteger()));
            break;
        case (jkmpDoubleVector<<16)+jkmpDouble:
        case (jkmpDoubleMatrix<<16)+jkmpDouble:
            {
            re=r;
            for (int i=0; i<l.numVec.size(); i++) {
                re.numVec[i]=int32_t(l.numVec[i])|r.toInteger();
            }
            break;
        }
        case (jkmpDouble<<16)+jkmpDoubleVector:
        case (jkmpDouble<<16)+jkmpDoubleMatrix:
            {
            re=l;
            for (int i=0; i<r.numVec.size(); i++) {
                re.numVec[i]=l.toInteger()|int32_t(r.numVec[i]);
            }
            break;
        }
        case (jkmpDoubleVector<<16)+jkmpDoubleVector:
        case (jkmpDoubleMatrix<<16)+jkmpDoubleMatrix:
            {
            if (l.isMatrix()) {
                if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                    if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                    re.setInvalid();
                    return;
                }
            } else {
                if (l.length()!=r.length()) {
                    if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                    re.setInvalid();
                    return;
                }
            }

            re=r;
            for (int i=0; i<r.numVec.size(); i++) {
                re.numVec[i]=int32_t(l.numVec[i])|int32_t(r.numVec[i]);
            }
            break;
        }
        default:
            {
                re.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be used in bitwise or operation").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}

void jkmpResult::logicand(jkmpResult& re, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    //qDebug()<<JKMP::string::number(tt, 16);
    switch(tt) {
        case (jkmpBool<<16)+jkmpBool:
            re.setBoolean(l.boolean&&r.boolean);
            break;
        case (jkmpBoolVector<<16)+jkmpBoolVector:
        case (jkmpBoolMatrix<<16)+jkmpBoolMatrix:
            {
                if (l.isMatrix()) {
                    if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                        if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        re.setInvalid();
                        return;
                    }
                } else {
                    if (l.length()!=r.length()) {
                        if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                        re.setInvalid();
                        return;
                    }
                }
            re=r;
            for (int i=0; i<r.boolVec.size(); i++) {
                re.boolVec[i]=l.boolVec[i]&&r.boolVec[i];
            }
            break;
        }
        case (jkmpBoolVector<<16)+jkmpBool:
        case (jkmpBoolMatrix<<16)+jkmpBool:
            {
            re=l;
            for (int i=0; i<l.boolVec.size(); i++) {
                re.boolVec[i]=l.boolVec[i]&&r.boolean;
            }
            break;
        }

        default:
            {
                re.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be used in logic and operation").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}

void jkmpResult::logicor(jkmpResult& re, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    //qDebug()<<JKMP::string::number(tt, 16);
    switch(tt) {
        case (jkmpBool<<16)+jkmpBool:
            re.setBoolean(l.boolean||r.boolean);
            break;
        case (jkmpBoolVector<<16)+jkmpBoolVector:
        case (jkmpBoolMatrix<<16)+jkmpBoolMatrix:
            {
                if (l.isMatrix()) {
                    if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                        if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        re.setInvalid();
                        return;
                    }
                } else {
                    if (l.length()!=r.length()) {
                        if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                        re.setInvalid();
                        return;
                    }
                }
            re=r;
            for (int i=0; i<r.boolVec.size(); i++) {
                re.boolVec[i]=l.boolVec[i]||r.boolVec[i];
            }
            break;
        }
        case (jkmpBoolVector<<16)+jkmpBool:
        case (jkmpBoolMatrix<<16)+jkmpBool:
            {
            re=l;
            for (int i=0; i<l.boolVec.size(); i++) {
                re.boolVec[i]=l.boolVec[i]||r.boolean;
            }
            break;
        }
        default:
            {
                re.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be used in logic or operation").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}

void jkmpResult::logicnot(jkmpResult& re, const jkmpResult &l, JKMathParser *p)
{
    switch(l.type) {
        case jkmpBool: re.setBoolean(!l.boolean); break;
        case jkmpBoolVector:
        case jkmpBoolMatrix:
            {
            re=l;
            for (int i=0; i<l.boolVec.size(); i++) {
                re.boolVec[i]=!l.boolVec[i];
            }
            break;
        }
        default:
            {
                re.setInvalid();
                if (p) p->jkmpError(JKMP::_("argument of type %1 may not be used in logic not operation").arg(l.typeName()));
            }
            break;

    }

}

void jkmpResult::logicnand(jkmpResult &re, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    //qDebug()<<JKMP::string::number(tt, 16);
    switch(tt) {
        case (jkmpBool<<16)+jkmpBool:
            re.setBoolean(!(l.boolean&&r.boolean));
            break;
        case (jkmpBoolVector<<16)+jkmpBoolVector:
        case (jkmpBoolMatrix<<16)+jkmpBoolMatrix:
            {
                if (l.isMatrix()) {
                    if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                        if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        re.setInvalid();
                        return;
                    }
                } else {
                    if (l.length()!=r.length()) {
                        if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                        re.setInvalid();
                        return;
                    }
                }
            re=r;
            for (int i=0; i<r.boolVec.size(); i++) {
                re.boolVec[i]=!(l.boolVec[i]&&r.boolVec[i]);
            }
            break;
        }
        case (jkmpBoolVector<<16)+jkmpBool:
        case (jkmpBoolMatrix<<16)+jkmpBool:
            {
            re=l;
            for (int i=0; i<l.boolVec.size(); i++) {
                re.boolVec[i]=!(l.boolVec[i]&&r.boolean);
            }
            break;
        }
        default:
            {
                re.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be used in logic nand operation").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}

void jkmpResult::logicnor(jkmpResult &re, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    //qDebug()<<JKMP::string::number(tt, 16);
    switch(tt) {
        case (jkmpBool<<16)+jkmpBool:
            re.setBoolean(!(l.boolean||r.boolean));
            break;
        case (jkmpBoolVector<<16)+jkmpBoolVector:
        case (jkmpBoolMatrix<<16)+jkmpBoolMatrix:
            {
                if (l.isMatrix()) {
                    if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                        if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        re.setInvalid();
                        return;
                    }
                } else {
                    if (l.length()!=r.length()) {
                        if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                        re.setInvalid();
                        return;
                    }
                }
            re=r;
            for (int i=0; i<r.boolVec.size(); i++) {
                re.boolVec[i]=!(l.boolVec[i]||r.boolVec[i]);
            }
            break;
        }
        case (jkmpBoolVector<<16)+jkmpBool:
        case (jkmpBoolMatrix<<16)+jkmpBool: {
            re=l;
            for (int i=0; i<l.boolVec.size(); i++) {
                re.boolVec[i]=!(l.boolVec[i]||r.boolean);
            }
            break;
        }
        default:
            {
                re.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be used in logic nor operation").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}

void jkmpResult::logicxor(jkmpResult &re, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    uint32_t lt=l.type;
    uint32_t rt=r.type;
    uint32_t tt=(lt<<16)+rt;
    //qDebug()<<JKMP::string::number(tt, 16);
    switch(tt) {
        case (jkmpBool<<16)+jkmpBool:
            re.setBoolean((l.boolean&&!r.boolean)||(!l.boolean&&r.boolean));
            break;
        case (jkmpBoolVector<<16)+jkmpBoolVector:
        case (jkmpBoolMatrix<<16)+jkmpBoolVector:
            {
                if (l.isMatrix()) {
                    if (l.length()!=r.length() || r.matrix_columns!=l.matrix_columns) {
                        if (p) p->jkmpError(JKMP::_("matrix arguments have to have the same size, but sizes were left=%1x%2, right=%3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        re.setInvalid();
                        return;
                    }
                } else {
                    if (l.length()!=r.length()) {
                        if (p) p->jkmpError(JKMP::_("arguments have to have the same length, but lengthes were left=%1, right=%2").arg(l.length()).arg(r.length()));
                        re.setInvalid();
                        return;
                    }
                }
            re=r;
            for (int i=0; i<r.boolVec.size(); i++) {
                re.boolVec[i]=(!l.boolVec[i]&&r.boolVec[i])||(l.boolVec[i]&&!r.boolVec[i]);
            }
            break;
        }
        case (jkmpBoolVector<<16)+jkmpBool:
        case (jkmpBoolMatrix<<16)+jkmpBool:
            {
            re=l;
            for (int i=0; i<l.boolVec.size(); i++) {
                re.boolVec[i]=(!l.boolVec[i]&&r.boolean)||(l.boolVec[i]&&!r.boolean);
            }
            break;
        }
        default:
            {
                re.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be used in logic xor operation").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}

void jkmpResult::neg(jkmpResult& re, const jkmpResult &l, JKMathParser *p)
{
    switch(l.type) {
        case jkmpDouble: re.setDouble(-l.num); break;
        case jkmpDoubleVector:
        case jkmpDoubleMatrix:
            {
            re=l;
            for (int i=0; i<l.numVec.size(); i++) {
                re.numVec[i]=l.numVec[i];
            }
            break;
        }
        default:
            {
                re.setInvalid();
                if (p) p->jkmpError(JKMP::_("argument of type %1 may not be used in negation operation").arg(l.typeName()));
            }
            break;

    }
}

void jkmpResult::bitwisenot(jkmpResult& re, const jkmpResult &l, JKMathParser *p)
{
    switch(l.type) {
        case jkmpDouble: re.setDouble(double(~l.toInteger())); break;
        case jkmpDoubleVector:
        case jkmpDoubleMatrix:
            {
            re=l;
            for (int i=0; i<l.numVec.size(); i++) {
                re.numVec[i]=~int32_t(l.numVec[i]);
            }
            break;
        }
        default:
            {
                re.setInvalid();
                if (p) p->jkmpError(JKMP::_("argument of type %1 may not be used in bitwise not operation").arg(l.typeName()));
            }
            break;

    }

}

void jkmpResult::compareequal(jkmpResult& res, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    switch((uint32_t(l.type)<<16)+uint32_t(r.type)) {
        case (jkmpDouble<<16)+jkmpDouble:
            res.setBoolean(l.num==r.num);
            break;
        case (jkmpDoubleVector<<16)+jkmpDoubleVector:
        case (jkmpDoubleMatrix<<16)+jkmpDoubleMatrix:
            {
                if (l.isMatrix()) {
                    if (l.numVec.size()!=r.numVec.size() || l.matrix_columns!=r.matrix_columns) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("matrix in comparison '==' have different sizes (%1x%2 and %3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        return;
                    }
                    res.setBoolMatrix(l.numVec.size(), l.matrix_columns);
                } else {
                    if (l.numVec.size()!=r.numVec.size()) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("vectors in comparison '==' have different length (%1 and %2)").arg(l.length()).arg(r.length()));
                        return;
                    }
                    res.setBoolVec(l.numVec.size());
                }

            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]==r.numVec[i]);
            }
            } break;
        case (jkmpDoubleVector<<16)+jkmpDouble: {
            res.setBoolVec(l.numVec.size());
            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]==r.num);
            }
            } break;
        case (jkmpDouble<<16)+jkmpDoubleVector: {
            res.setBoolVec(r.numVec.size());
            for (int i=0; i<r.numVec.size(); i++) {
                res.boolVec[i]=(r.numVec[i]==l.num);
            }
            } break;
        case (jkmpDoubleMatrix<<16)+jkmpDouble: {
            res.setBoolMatrix(l.numVec.size(), l.matrix_columns);
            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]==r.num);
            }
            } break;
        case (jkmpDouble<<16)+jkmpDoubleMatrix: {
            res.setBoolMatrix(r.numVec.size(), r.matrix_columns);
            for (int i=0; i<r.numVec.size(); i++) {
                res.boolVec[i]=(r.numVec[i]==l.num);
            }
            } break;

        case (jkmpList<<16)+jkmpList:
            res.setBoolean(l.listData==r.listData);
            break;
        case (jkmpStruct<<16)+jkmpStruct:
            res.setBoolean(l.structData==r.structData);
            break;
        case (jkmpString<<16)+jkmpString:
            res.setBoolean(l.str==r.str);
            break;
        case (jkmpStringVector<<16)+jkmpStringVector: {
            if (l.strVec.size()!=r.strVec.size()) {
                res.setInvalid();
                if (p) p->jkmpError(JKMP::_("vectors in comparison '==' have different length"));
                return;
            }
            res.setBoolVec(l.strVec.size());
            for (int i=0; i<l.strVec.size(); i++) {
                res.boolVec[i]=(l.strVec[i]==r.strVec[i]);
            }
            } break;
        case (jkmpStringVector<<16)+jkmpString: {
            res.setBoolVec(l.strVec.size());
            for (int i=0; i<l.strVec.size(); i++) {
                res.boolVec[i]=(l.strVec[i]==r.str);
            }
            } break;
        case (jkmpString<<16)+jkmpStringVector: {
            res.setBoolVec(r.strVec.size());
            for (int i=0; i<r.strVec.size(); i++) {
                res.boolVec[i]=(r.strVec[i]==l.str);
            }
            } break;


        case (jkmpBool<<16)+jkmpBool:
            res.setBoolean(l.boolean==r.boolean);
            break;
        case (jkmpBoolVector<<16)+jkmpBoolVector:
        case (jkmpBoolMatrix<<16)+jkmpBoolMatrix:
            {
                if (l.isMatrix()) {
                    if (l.boolVec.size()!=r.boolVec.size() || l.matrix_columns!=r.matrix_columns) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("matrix in comparison '==' have different sizes (%1x%2 and %3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        return;
                    }
                    res.setBoolMatrix(l.boolVec.size(), l.matrix_columns);
                } else {
                    if (l.boolVec.size()!=r.boolVec.size()) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("vectors in comparison '==' have different length (%1 and %2)").arg(l.length()).arg(r.length()));
                        return;
                    }
                    res.setBoolVec(l.boolVec.size());
                }
            for (int i=0; i<l.boolVec.size(); i++) {
                res.boolVec[i]=(l.boolVec[i]==r.boolVec[i]);
            }
            } break;
        case (jkmpBoolVector<<16)+jkmpBool: {
            res.setBoolVec(l.boolVec.size());
            for (int i=0; i<l.boolVec.size(); i++) {
                res.boolVec[i]=(l.boolVec[i]==r.boolean);
            }
            } break;
        case (jkmpBool<<16)+jkmpBoolVector: {
            res.setBoolVec(r.boolVec.size());
            for (int i=0; i<r.boolVec.size(); i++) {
                res.boolVec[i]=(r.boolVec[i]==l.boolean);
            }
            } break;
        case (jkmpBoolMatrix<<16)+jkmpBool: {
            res.setBoolMatrix(l.boolVec.size(), l.matrix_columns);
            for (int i=0; i<l.boolVec.size(); i++) {
                res.boolVec[i]=(l.boolVec[i]==r.boolean);
            }
            } break;
        case (jkmpBool<<16)+jkmpBoolMatrix: {
            res.setBoolMatrix(r.boolVec.size(), r.matrix_columns);
            for (int i=0; i<r.boolVec.size(); i++) {
                res.boolVec[i]=(r.boolVec[i]==l.boolean);
            }
            } break;
        default:
            {
                res.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be compared '=='").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}

void jkmpResult::comparenotequal(jkmpResult& res, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    switch((uint32_t(l.type)<<16)+uint32_t(r.type)) {
        case (jkmpDouble<<16)+jkmpDouble:
            res.setBoolean(l.num!=r.num);
            break;
        case (jkmpDoubleVector<<16)+jkmpDoubleVector:
        case (jkmpDoubleMatrix<<16)+jkmpDoubleMatrix:
            {
                if (l.isMatrix()) {
                    if (l.numVec.size()!=r.numVec.size() || l.matrix_columns!=r.matrix_columns) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("matrix in comparison '!=' have different sizes (%1x%2 and %3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        return;
                    }
                    res.setBoolMatrix(l.numVec.size(), l.matrix_columns);
                } else {
                    if (l.numVec.size()!=r.numVec.size()) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("vectors in comparison '!=' have different length (%1 and %2)").arg(l.length()).arg(r.length()));
                        return;
                    }
                    res.setBoolVec(l.numVec.size());
                }

            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]!=r.numVec[i]);
            }
            } break;
        case (jkmpDoubleVector<<16)+jkmpDouble: {
            res.setBoolVec(l.numVec.size());
            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]!=r.num);
            }
            } break;
        case (jkmpDouble<<16)+jkmpDoubleVector: {
            res.setBoolVec(r.numVec.size());
            for (int i=0; i<r.numVec.size(); i++) {
                res.boolVec[i]=(r.numVec[i]!=l.num);
            }
            } break;
        case (jkmpDoubleMatrix<<16)+jkmpDouble: {
            res.setBoolMatrix(l.numVec.size(), l.matrix_columns);
            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]!=r.num);
            }
            } break;
        case (jkmpDouble<<16)+jkmpDoubleMatrix: {
            res.setBoolMatrix(r.numVec.size(), r.matrix_columns);
            for (int i=0; i<r.numVec.size(); i++) {
                res.boolVec[i]=(r.numVec[i]!=l.num);
            }
            } break;

        case (jkmpList<<16)+jkmpList:
            res.setBoolean(l.listData!=r.listData);
            break;
        case (jkmpStruct<<16)+jkmpStruct:
            res.setBoolean(l.structData!=r.structData);
            break;
        case (jkmpString<<16)+jkmpString:
            res.setBoolean(l.str!=r.str);
            break;
        case (jkmpStringVector<<16)+jkmpStringVector: {
            if (l.strVec.size()!=r.strVec.size()) {
                res.setInvalid();
                if (p) p->jkmpError(JKMP::_("vectors in comparison '!=' have different length"));
                return;
            }
            res.setBoolVec(l.strVec.size());
            for (int i=0; i<l.strVec.size(); i++) {
                res.boolVec[i]=(l.strVec[i]!=r.strVec[i]);
            }
            } break;
        case (jkmpStringVector<<16)+jkmpString: {
            res.setBoolVec(l.strVec.size());
            for (int i=0; i<l.strVec.size(); i++) {
                res.boolVec[i]=(l.strVec[i]!=r.str);
            }
            } break;
        case (jkmpString<<16)+jkmpStringVector: {
            res.setBoolVec(r.strVec.size());
            for (int i=0; i<r.strVec.size(); i++) {
                res.boolVec[i]=(r.strVec[i]!=l.str);
            }
            } break;


        case (jkmpBool<<16)+jkmpBool:
            res.setBoolean(l.boolean!=r.boolean);
            break;
        case (jkmpBoolVector<<16)+jkmpBoolVector:
        case (jkmpBoolMatrix<<16)+jkmpBoolMatrix:
            {
                if (l.isMatrix()) {
                    if (l.boolVec.size()!=r.boolVec.size() || l.matrix_columns!=r.matrix_columns) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("matrix in comparison '!=' have different sizes (%1x%2 and %3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        return;
                    }
                    res.setBoolMatrix(l.boolVec.size(), l.matrix_columns);
                } else {
                    if (l.boolVec.size()!=r.boolVec.size()) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("vectors in comparison '!=' have different length (%1 and %2)").arg(l.length()).arg(r.length()));
                        return;
                    }
                    res.setBoolVec(l.boolVec.size());
                }
            for (int i=0; i<l.boolVec.size(); i++) {
                res.boolVec[i]=(l.boolVec[i]!=r.boolVec[i]);
            }
            } break;
        case (jkmpBoolVector<<16)+jkmpBool: {
            res.setBoolVec(l.boolVec.size());
            for (int i=0; i<l.boolVec.size(); i++) {
                res.boolVec[i]=(l.boolVec[i]!=r.boolean);
            }
            } break;
        case (jkmpBool<<16)+jkmpBoolVector: {
            res.setBoolVec(r.boolVec.size());
            for (int i=0; i<r.boolVec.size(); i++) {
                res.boolVec[i]=(r.boolVec[i]!=l.boolean);
            }
            } break;
        case (jkmpBoolMatrix<<16)+jkmpBool: {
            res.setBoolMatrix(l.boolVec.size(), l.matrix_columns);
            for (int i=0; i<l.boolVec.size(); i++) {
                res.boolVec[i]=(l.boolVec[i]!=r.boolean);
            }
            } break;
        case (jkmpBool<<16)+jkmpBoolMatrix: {
            res.setBoolMatrix(r.boolVec.size(), r.matrix_columns);
            for (int i=0; i<r.boolVec.size(); i++) {
                res.boolVec[i]=(r.boolVec[i]!=l.boolean);
            }
            } break;
        default:
            {
                res.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be compared '!='").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}


void jkmpResult::comparegreater(jkmpResult& res, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    switch((uint32_t(l.type)<<16)+uint32_t(r.type)) {
        case (jkmpDouble<<16)+jkmpDouble:
            res.setBoolean(l.num>r.num);
            break;
        case (jkmpDoubleVector<<16)+jkmpDoubleVector:
        case (jkmpDoubleMatrix<<16)+jkmpDoubleMatrix:
            {
                if (l.isMatrix()) {
                    if (l.numVec.size()!=r.numVec.size() || l.matrix_columns!=r.matrix_columns) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("matrix in comparison '>' have different sizes (%1x%2 and %3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        return;
                    }
                    res.setBoolMatrix(l.numVec.size(), l.matrix_columns);
                } else {
                    if (l.numVec.size()!=r.numVec.size()) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("vectors in comparison '>' have different length (%1 and %2)").arg(l.length()).arg(r.length()));
                        return;
                    }
                    res.setBoolVec(l.numVec.size());
                }

            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]>r.numVec[i]);
            }
            } break;
        case (jkmpDoubleVector<<16)+jkmpDouble: {
            res.setBoolVec(l.numVec.size());
            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]>r.num);
            }
            } break;
        case (jkmpDouble<<16)+jkmpDoubleVector: {
            res.setBoolVec(r.numVec.size());
            for (int i=0; i<r.numVec.size(); i++) {
                res.boolVec[i]=(r.numVec[i]>l.num);
            }
            } break;
        case (jkmpDoubleMatrix<<16)+jkmpDouble: {
            res.setBoolMatrix(l.numVec.size(), l.matrix_columns);
            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]>r.num);
            }
            } break;
        case (jkmpDouble<<16)+jkmpDoubleMatrix: {
            res.setBoolMatrix(r.numVec.size(), r.matrix_columns);
            for (int i=0; i<r.numVec.size(); i++) {
                res.boolVec[i]=(r.numVec[i]>l.num);
            }
            } break;


        case (jkmpString<<16)+jkmpString:
            res.setBoolean(l.str>r.str);
            break;
        case (jkmpStringVector<<16)+jkmpStringVector: {
            if (l.strVec.size()!=r.strVec.size()) {
                res.setInvalid();
                if (p) p->jkmpError(JKMP::_("vectors in comparison '>' have different length"));
                return;
            }
            res.setBoolVec(l.strVec.size());
            for (int i=0; i<l.strVec.size(); i++) {
                res.boolVec[i]=(l.strVec[i]>r.strVec[i]);
            }
            } break;
        case (jkmpStringVector<<16)+jkmpString: {
            res.setBoolVec(l.strVec.size());
            for (int i=0; i<l.strVec.size(); i++) {
                res.boolVec[i]=(l.strVec[i]>r.str);
            }
            } break;
        case (jkmpString<<16)+jkmpStringVector: {
            res.setBoolVec(r.strVec.size());
            for (int i=0; i<r.strVec.size(); i++) {
                res.boolVec[i]=(r.strVec[i]>l.str);
            }
            } break;





        default:
            {
                res.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be compared '>'").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}



void jkmpResult::comparegreaterequal(jkmpResult& res, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    switch((uint32_t(l.type)<<16)+uint32_t(r.type)) {
        case (jkmpDouble<<16)+jkmpDouble:
            res.setBoolean(l.num>=r.num);
            break;
        case (jkmpDoubleVector<<16)+jkmpDoubleVector:
        case (jkmpDoubleMatrix<<16)+jkmpDoubleMatrix:
            {
                if (l.isMatrix()) {
                    if (l.numVec.size()!=r.numVec.size() || l.matrix_columns!=r.matrix_columns) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("matrix in comparison '>=' have different sizes (%1x%2 and %3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        return;
                    }
                    res.setBoolMatrix(l.numVec.size(), l.matrix_columns);
                } else {
                    if (l.numVec.size()!=r.numVec.size()) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("vectors in comparison '>=' have different length (%1 and %2)").arg(l.length()).arg(r.length()));
                        return;
                    }
                    res.setBoolVec(l.numVec.size());
                }

            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]>=r.numVec[i]);
            }
            } break;
        case (jkmpDoubleVector<<16)+jkmpDouble: {
            res.setBoolVec(l.numVec.size());
            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]>=r.num);
            }
            } break;
        case (jkmpDouble<<16)+jkmpDoubleVector: {
            res.setBoolVec(r.numVec.size());
            for (int i=0; i<r.numVec.size(); i++) {
                res.boolVec[i]=(r.numVec[i]>=l.num);
            }
            } break;
        case (jkmpDoubleMatrix<<16)+jkmpDouble: {
            res.setBoolMatrix(l.numVec.size(), l.matrix_columns);
            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]>=r.num);
            }
            } break;
        case (jkmpDouble<<16)+jkmpDoubleMatrix: {
            res.setBoolMatrix(r.numVec.size(), r.matrix_columns);
            for (int i=0; i<r.numVec.size(); i++) {
                res.boolVec[i]=(r.numVec[i]>=l.num);
            }
            } break;


        case (jkmpString<<16)+jkmpString:
            res.setBoolean(l.str>=r.str);
            break;
        case (jkmpStringVector<<16)+jkmpStringVector: {
            if (l.strVec.size()!=r.strVec.size()) {
                res.setInvalid();
                if (p) p->jkmpError(JKMP::_("vectors in comparison '>=' have different length"));
                return;
            }
            res.setBoolVec(l.strVec.size());
            for (int i=0; i<l.strVec.size(); i++) {
                res.boolVec[i]=(l.strVec[i]>=r.strVec[i]);
            }
            } break;
        case (jkmpStringVector<<16)+jkmpString: {
            res.setBoolVec(l.strVec.size());
            for (int i=0; i<l.strVec.size(); i++) {
                res.boolVec[i]=(l.strVec[i]>=r.str);
            }
            } break;
        case (jkmpString<<16)+jkmpStringVector: {
            res.setBoolVec(r.strVec.size());
            for (int i=0; i<r.strVec.size(); i++) {
                res.boolVec[i]=(r.strVec[i]>=l.str);
            }
            } break;


        default:
            {
                res.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be compared '>='").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}





void jkmpResult::comparesmaller(jkmpResult& res, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    switch((uint32_t(l.type)<<16)+uint32_t(r.type)) {
        case (jkmpDouble<<16)+jkmpDouble:
            res.setBoolean(l.num<r.num);
            break;
        case (jkmpDoubleVector<<16)+jkmpDoubleVector:
        case (jkmpDoubleMatrix<<16)+jkmpDoubleMatrix:
            {
                if (l.isMatrix()) {
                    if (l.numVec.size()!=r.numVec.size() || l.matrix_columns!=r.matrix_columns) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("matrix in comparison '<' have different sizes (%1x%2 and %3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        return;
                    }
                    res.setBoolMatrix(l.numVec.size(), l.matrix_columns);
                } else {
                    if (l.numVec.size()!=r.numVec.size()) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("vectors in comparison '<' have different length (%1 and %2)").arg(l.length()).arg(r.length()));
                        return;
                    }
                    res.setBoolVec(l.numVec.size());
                }

            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]<r.numVec[i]);
            }
            } break;
        case (jkmpDoubleVector<<16)+jkmpDouble: {
            res.setBoolVec(l.numVec.size());
            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]<r.num);
            }
            } break;
        case (jkmpDouble<<16)+jkmpDoubleVector: {
            res.setBoolVec(r.numVec.size());
            for (int i=0; i<r.numVec.size(); i++) {
                res.boolVec[i]=(r.numVec[i]<l.num);
            }
            } break;
        case (jkmpDoubleMatrix<<16)+jkmpDouble: {
            res.setBoolMatrix(l.numVec.size(), l.matrix_columns);
            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]<r.num);
            }
            } break;
        case (jkmpDouble<<16)+jkmpDoubleMatrix: {
            res.setBoolMatrix(r.numVec.size(), r.matrix_columns);
            for (int i=0; i<r.numVec.size(); i++) {
                res.boolVec[i]=(r.numVec[i]<l.num);
            }
            } break;


        case (jkmpString<<16)+jkmpString:
            res.setBoolean(l.str<r.str);
            break;
        case (jkmpStringVector<<16)+jkmpStringVector: {
            if (l.strVec.size()!=r.strVec.size()) {
                res.setInvalid();
                if (p) p->jkmpError(JKMP::_("vectors in comparison '<' have different length"));
                return;
            }
            res.setBoolVec(l.strVec.size());
            for (int i=0; i<l.strVec.size(); i++) {
                res.boolVec[i]=(l.strVec[i]<r.strVec[i]);
            }
            } break;
        case (jkmpStringVector<<16)+jkmpString: {
            res.setBoolVec(l.strVec.size());
            for (int i=0; i<l.strVec.size(); i++) {
                res.boolVec[i]=(l.strVec[i]<r.str);
            }
            } break;
        case (jkmpString<<16)+jkmpStringVector: {
            res.setBoolVec(r.strVec.size());
            for (int i=0; i<r.strVec.size(); i++) {
                res.boolVec[i]=(r.strVec[i]<l.str);
            }
            } break;


        default:
            {
                res.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be compared '<'").arg(l.typeName()).arg(r.typeName()));
                break;            }

    }
}


void jkmpResult::comparesmallerequal(jkmpResult& res, const jkmpResult &l, const jkmpResult &r, JKMathParser *p)
{
    switch((uint32_t(l.type)<<16)+uint32_t(r.type)) {
        case (jkmpDouble<<16)+jkmpDouble:
            res.setBoolean(l.num<=r.num);
            break;
        case (jkmpDoubleVector<<16)+jkmpDoubleVector:
        case (jkmpDoubleMatrix<<16)+jkmpDoubleMatrix:
            {
                if (l.isMatrix()) {
                    if (l.numVec.size()!=r.numVec.size() || l.matrix_columns!=r.matrix_columns) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("matrix in comparison '<=' have different sizes (%1x%2 and %3x%4").arg(l.rows()).arg(l.columns()).arg(r.rows()).arg(r.columns()));
                        return;
                    }
                    res.setBoolMatrix(l.numVec.size(), l.matrix_columns);
                } else {
                    if (l.numVec.size()!=r.numVec.size()) {
                        res.setInvalid();
                        if (p) p->jkmpError(JKMP::_("vectors in comparison '<=' have different length (%1 and %2)").arg(l.length()).arg(r.length()));
                        return;
                    }
                    res.setBoolVec(l.numVec.size());
                }

            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]<=r.numVec[i]);
            }
            } break;
        case (jkmpDoubleVector<<16)+jkmpDouble: {
            res.setBoolVec(l.numVec.size());
            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]<=r.num);
            }
            } break;
        case (jkmpDouble<<16)+jkmpDoubleVector: {
            res.setBoolVec(r.numVec.size());
            for (int i=0; i<r.numVec.size(); i++) {
                res.boolVec[i]=(r.numVec[i]<=l.num);
            }
            } break;
        case (jkmpDoubleMatrix<<16)+jkmpDouble: {
            res.setBoolMatrix(l.numVec.size(), l.matrix_columns);
            for (int i=0; i<l.numVec.size(); i++) {
                res.boolVec[i]=(l.numVec[i]<=r.num);
            }
            } break;
        case (jkmpDouble<<16)+jkmpDoubleMatrix: {
            res.setBoolMatrix(r.numVec.size(), r.matrix_columns);
            for (int i=0; i<r.numVec.size(); i++) {
                res.boolVec[i]=(r.numVec[i]<=l.num);
            }
            } break;


        case (jkmpString<<16)+jkmpString:
            res.setBoolean(l.str<=r.str);
            break;
        case (jkmpStringVector<<16)+jkmpStringVector: {
            if (l.strVec.size()!=r.strVec.size()) {
                res.setInvalid();
                if (p) p->jkmpError(JKMP::_("vectors in comparison '<=' have different length"));
                return;
            }
            res.setBoolVec(l.strVec.size());
            for (int i=0; i<l.strVec.size(); i++) {
                res.boolVec[i]=(l.strVec[i]<=r.strVec[i]);
            }
            } break;
        case (jkmpStringVector<<16)+jkmpString: {
            res.setBoolVec(l.strVec.size());
            for (int i=0; i<l.strVec.size(); i++) {
                res.boolVec[i]=(l.strVec[i]<=r.str);
            }
            } break;
        case (jkmpString<<16)+jkmpStringVector: {
            res.setBoolVec(r.strVec.size());
            for (int i=0; i<r.strVec.size(); i++) {
                res.boolVec[i]=(r.strVec[i]<=l.str);
            }
            } break;


        default:
            {
                res.setInvalid();
                if (p) p->jkmpError(JKMP::_("arguments of type %1 and %2 may not be compared '<='").arg(l.typeName()).arg(r.typeName()));
            }
            break;
    }
}

















//================================================================================================================



JKMP::string jkmpResult::typeName() const
{
    if (!isValid) return JKMP::string("invalid");
    return JKMathParser::resultTypeToString(type);
}


JKMP::vector<int> jkmpResult::asIntVector() const
{
    JKMP::vector<double> dbl=asVector();
    JKMP::vector<int> ii(dbl.size(), 0);
    for (int i=0; i<dbl.size(); i++) {
        ii[i]=dbl[i];
    }
    return ii;
}























JKMP::string jkmpResultTypeToString(jkmpResultType type)
{
    switch(type) {
        case jkmpDouble: return JKMP::_("number");
        case jkmpString: return JKMP::_("string");
        case jkmpBool: return JKMP::_("bool");
        case jkmpDoubleVector: return JKMP::_("number vector");
        case jkmpDoubleMatrix: return JKMP::_("number matrix");
        case jkmpStringVector: return JKMP::_("string vector");
        case jkmpBoolVector: return JKMP::_("bool vector");
        case jkmpBoolMatrix: return JKMP::_("bool matrix");
        case jkmpVoid: return JKMP::_("void");
        case jkmpStruct: return JKMP::_("struct");
        case jkmpList: return JKMP::_("list");
    }
    return JKMP::_("invalid");
}
