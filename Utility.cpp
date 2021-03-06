#include "Utility.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

void trim_string(string & str) {
  if (str.size() == 0){
    return;
  }
  
  int start = 0, len = 0, i = 0;
  bool inword = false;
  vector<string> words;
  
  //Lambda function for get all word in a title
  for_each(str.begin(),str.end(), [&start,&len,&inword,&words,&str,&i](char elem){
    if(isspace(elem)&&inword){
      inword = false;
      words.push_back(str.substr(start,len));
    }else if(!isspace(elem)&&!inword){
      inword =true;
      start = i;
      len = 1;
    }else if(!isspace(elem)&&inword){
      len++;
    }
    i++;
  });
  
  if(inword){
    words.push_back(str.substr(start,len));
  }
  
  str = "";
  
  //only range for for concatane words together
  for(string word:words){
    str+=word+" ";
  }
  str.erase(str.size()-1,1);
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
    is.putback('\n');
    return;
}
