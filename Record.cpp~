#include "Record.h"
#include <iostream>
#include <fstream>
#include "Utility.h"

using std::string;
using std::ostream;
using std::ifstream;
using std::endl;

int Record::ID_counter = 0;
int Record::ID_counter_save = 0;

Record::Record(const string& medium_,const string& title_):ID(++ID_counter),title(title_),medium(medium_),rating(0){}

Record::Record(const string& title_):ID(0), title(title_),medium(),rating(0){}

Record::Record(int ID_):ID(ID_),title(),medium(),rating(0){}

Record::Record(ifstream& is){
  if (!(is >> ID))
    throw Error("Invalid data found in file!");
    if (!(is >> medium))
      throw Error("Invalid data found in file!");
    if (!(is >> rating) || (rating > 5 || rating < 0))
      throw Error("Invalid data found in file!");
    read_title_file(title, is);
    if (ID > ID_counter)
      ID_counter = ID;
}

void Record::set_rating(int rating_){
  if(rating_>5||rating_<=0){
    throw Error("Rating is out of range!");
  }
  rating = rating_;
}

void Record::save(ostream& os) const{
  os << ID << " " << medium << " " << rating << " " << title << endl;
}

ostream& operator<<(ostream& os, const Record& record){
  if (record.rating == 0){
    os << record.ID << ": " << record.medium << " u " << record.title << endl;
  }else{
    os << record.ID << ": " << record.medium << " " << record.rating << " " << record.title << endl;
  }
  return os; 
}
