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

#include "jkmpfilenametools.h"
#include <sstream>
#include <locale>
#include <iomanip>
#include <unistd.h>
#ifdef __WINDOWS__
  #include <windows.h>
#endif

JKMP::stringType JKMP::extract_file_path(JKMP::stringType filename) {
  size_t p=filename.find_last_of(JKMP_PATHSEPARATOR_STRING);
  if (p==JKMP::stringType::npos) { // if there is not separator: return nothing. The filname
                              // only consists a filename, no path
    return "";
  } else {
    return include_trailing_backslash(filename.substr(0, p+1));
  }
}


JKMP::stringType JKMP::extract_file_name(JKMP::stringType filename) {
  size_t p=filename.find_last_of(JKMP_PATHSEPARATOR_STRING);
  if (p==JKMP::stringType::npos) { // if there is not separator: return the complete string,
                              // <filename> only consists of a filename
    return filename;
  } else {
    return filename.substr(p+1);
  }
}


JKMP::stringType JKMP::extract_file_ext(JKMP::stringType filename) {
  size_t p=filename.find_last_of(".");
  if (p==JKMP::stringType::npos) { // if there is not separator: there is no file extension
    return "";
  } else {
    return filename.substr(p+1);
  }
}


JKMP::stringType JKMP::change_file_ext(JKMP::stringType filename, JKMP::stringType ext) {
  size_t p=filename.find_last_of(".");
  JKMP::stringType ext1=ext;
  if (ext.size()>0)
    if (ext[0]!='.') ext1="."+ext;
  if (p==JKMP::stringType::npos) { // if there is not separator: there is no file extension
    return filename+ext1;
  } else {
    return filename.substr(0,p)+ext1;
  }
}


JKMP::stringType JKMP::include_trailing_backslash(JKMP::stringType filename) {
  JKMP::stringType ret=filename;
  if (filename.size()>0) {
    if (filename[filename.size()-1]!=JKMP_PATHSEPARATOR_CHAR) ret+=JKMP_PATHSEPARATOR_STRING;
  }
  return ret;
}


JKMP::stringType JKMP::exclude_trailing_backslash(JKMP::stringType filename) {
  JKMP::stringType ret=filename;
  if (filename.size()>0) {
    if (filename[filename.size()-1]==JKMP_PATHSEPARATOR_CHAR) ret.erase(filename.size()-1,1);
  }
  return ret;
}

JKMP::stringType JKMP::extend_file_path(JKMP::stringType filename, JKMP::stringType addpath) {
    JKMP::stringType path=extract_file_path(filename);
    JKMP::stringType file=extract_file_name(filename);
    return include_trailing_backslash(include_trailing_backslash(path)+exclude_trailing_backslash(addpath))+file;
}

JKMP::stringType JKMP::get_currentworkingdirectory() {
    size_t size = 1024;
    JKMP::string res="";
    bool ok=true;
    while (ok) {
        char *buffer = (char *)calloc(size, sizeof(char));
        if (_getcwd(buffer, size) == buffer) {
            res=buffer;
            ok=false;
        }

        free (buffer);
        if (errno != ERANGE) ok=false;
        size *= 2;
    }
    return res;
}

JKMP::stringType JKMP::get_full_filename(JKMP::stringType filename){
  #ifndef __WINDOWS__
    #warning("get_full_filename(JKMP::stringType filename) was not implemented for non-MS Windows OS!!!");
    return filename;
  #else
  JKMP::charType buffer[8192];
  long retval = GetFullPathNameA(filename.c_str(), 8192, buffer, NULL);
  if (retval>0)
    return buffer;
  else
    return "";
  #endif
}

JKMP::stringType JKMP::replace_to_system_pathseparator(JKMP::stringType filename) {
    JKMP::stringType fn;
    for (size_t i=0; i<filename.size(); i++) {
        if (filename[i]=='/'||filename[i]=='\\') fn=fn+JKMP_PATHSEPARATOR_STRING;
        else fn=fn+filename[i];
    }
    return fn;
}
