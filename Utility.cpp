#include "Utility.h"
#include "String.h"
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

void trim_string(String & string) {
  if (string.size() == 0){
    return;
  }
  int i = 0;
  while (isspace(string[i])){
    i++;
  }
  if (i > 0){
    string.remove(0, i);
  }
  i = 1;
  int space_start = 0;
  while (i < string.size()) {
    if (isspace(string[i]) && !isspace(string[i - 1]))
      space_start = i;
    if (!isspace(string[i]) && isspace(string[i - 1]) && i - space_start >= 2) {
      string.remove(space_start, i - space_start - 1);
      string[space_start] = ' ';
      i = space_start;
    }
    i++;
  }
  
  if (isspace(string[string.size() - 1])){
    string.remove(space_start, string.size() - space_start);
  }
  return;
}

void read_integer(int& id, std::istream & is){
  if(!(is>>id)){
    throw Error("Could not read an integer value!");
  }
}

void read_title_stdin(String & title) {
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

void read_title_file(String & title, std::istream & is) {
    if (!getline(is, title) || title == "")
        throw Error("Invalid data found in file!");
    trim_string(title);
    return;
}
