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
#ifdef __WINDOWS__
  #include <windows.h>
#endif

std::string JKMP::extract_file_path(std::string filename) {
  size_t p=filename.find_last_of(JKMP_PATHSEPARATOR_STRING);
  if (p==std::string::npos) { // if there is not separator: return nothing. The filname
                              // only consists a filename, no path
    return "";
  } else {
    return include_trailing_backslash(filename.substr(0, p+1));
  }
}


std::string JKMP::extract_file_name(std::string filename) {
  size_t p=filename.find_last_of(JKMP_PATHSEPARATOR_STRING);
  if (p==std::string::npos) { // if there is not separator: return the complete string,
                              // <filename> only consists of a filename
    return filename;
  } else {
    return filename.substr(p+1);
  }
}


std::string JKMP::extract_file_ext(std::string filename) {
  size_t p=filename.find_last_of(".");
  if (p==std::string::npos) { // if there is not separator: there is no file extension
    return "";
  } else {
    return filename.substr(p+1);
  }
}


std::string JKMP::change_file_ext(std::string filename, std::string ext) {
  size_t p=filename.find_last_of(".");
  std::string ext1=ext;
  if (ext.size()>0)
    if (ext[0]!='.') ext1="."+ext;
  if (p==std::string::npos) { // if there is not separator: there is no file extension
    return filename+ext1;
  } else {
    return filename.substr(0,p)+ext1;
  }
}


std::string JKMP::include_trailing_backslash(std::string filename) {
  std::string ret=filename;
  if (filename.size()>0) {
    if (filename[filename.size()-1]!=PATHSEPARATOR_CHAR) ret+=JKMP_PATHSEPARATOR_STRING;
  }
  return ret;
}


std::string JKMP::exclude_trailing_backslash(std::string filename) {
  std::string ret=filename;
  if (filename.size()>0) {
    if (filename[filename.size()-1]==PATHSEPARATOR_CHAR) ret.erase(filename.size()-1,1);
  }
  return ret;
}

std::string JKMP::extend_file_path(std::string filename, std::string addpath) {
    std::string path=extract_file_path(filename);
    std::string file=extract_file_name(filename);
    return include_trailing_backslash(include_trailing_backslash(path)+exclude_trailing_backslash(addpath))+file;
}

std::string JKMP::get_currentworkingdirectory() {
    size_t size = 1024;
    std::string res="";
    bool ok=true;
    while (ok) {
        char *buffer = (char *)calloc(size, sizeof(char));
        if (getcwd(buffer, size) == buffer) {
            res=buffer;
            ok=false;
        }
        //printf("\n--%s--\n", buffer);
        free (buffer);
        if (errno != ERANGE) ok=false;
        size *= 2;
    }
    return res;
}

std::string JKMP::get_full_filename(std::string filename){
  #ifndef __WINDOWS__
    #warning("get_full_filename(std::string filename) was not implemented for non-MS Windows OS!!!");
    return filename;
  #else
  char buffer[8192];
  long retval = GetFullPathNameA(filename.c_str(), 8192, buffer, NULL);
  if (retval>0)
    return buffer;
  else
    return "";
  #endif
}

std::string JKMP::replace_to_system_pathseparator(std::string filename) {
    std::string fn;
    for (size_t i=0; i<filename.size(); i++) {
        if (filename[i]=='/'||filename[i]=='\\') fn=fn+JKMP_PATHSEPARATOR_STRING;
        else fn=fn+filename[i];
    }
    return fn;
}
