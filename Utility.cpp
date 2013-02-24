#include "Utility.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

void trim_string(string & str) {
  if (str.size() == 0){
    return;
  }
  unsigned int i = 0;
  while (isspace(str[i])){
    i++;
  }
  if (i > 0){
    str.erase(0, i);
  }
  i = 1;
  int space_start = 0;
  while (i < str.size()) {
    if (isspace(str[i]) && !isspace(str[i - 1]))
      space_start = i;
    if (!isspace(str[i]) && isspace(str[i - 1]) && i - space_start >= 2) {
      str.erase(space_start, i - space_start - 1);
      str[space_start] = ' ';
      i = space_start;
    }
    i++;
  }
  
  if (isspace(str[str.size() - 1])){
    str.erase(space_start, str.size() - space_start);
  }
  return;
}

void read_integer(int& id, std::istream & is){
  if(!(is>>id)){
    throw Error("Could not read an integer value!");
  }
}

void read_title_stdin(string & title) {
  try{
    read_title_file(title,cin);
  }catch(Error& error){
    if(strcmp(error.msg, "Invalid data found in file!")==0){
      throw Error("Could not read a title!");
    }
  }
  trim_string(title);
  return;
}

void read_title_file(string & title, std::istream & is) {
    if (!getline(is, title) || title == "")
        throw Error("Invalid data found in file!");
    trim_string(title);
    return;
}
