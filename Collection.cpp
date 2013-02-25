#include "Collection.h"
#include "Record.h"
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

Collection::Collection(std::ifstream& is,const vector<Record*>& library){
  int total_members = 0;
  if (!(is >> name))
    throw Error("Invalid data found in file!");
  if (!(is >> total_members))
    throw Error("Invalid data found in file!");
  for (int i = 0; i < total_members; i++) {
     while (is.get() != '\n');
    string title;
    read_title_file(title, is);
    Record record(title);
    Record_compare_title comparator;
    auto it = lower_bound(library.begin(), library.end(), &record, comparator);
    if (it == library.end()||comparator(&record,*it))
      throw Error("Invalid data found in file!");
    add_member(*it);
  }
}

void Collection::add_member(Record* record_ptr) {
  Record_compare_title comparator;
  auto it = lower_bound(list.begin(), list.end(), record_ptr, comparator);
  if(it!=list.end()&&!comparator(record_ptr,*it)){
    throw Error("Record is already a member in the collection!");
  }
  list.insert(it,record_ptr);
  return;
}

bool Collection::is_member_present(Record* record_ptr) const{
  Record_compare_title comparator;
  auto it = lower_bound(list.begin(), list.end(), record_ptr, comparator);
  return it != list.end()&&!comparator(record_ptr,*it);
}

void Collection::remove_member(Record* record_ptr) {
  Record_compare_title comparator;
  auto it = lower_bound(list.begin(), list.end(), record_ptr, comparator);
  if (it == list.end()||comparator(record_ptr,*it))
    throw Error("Record is not a member in the collection!");
  list.erase(it);
  return;
}

void Collection::save(std::ostream& os) const{
  os << name << " " << list.size() << endl;
  for (Record * record: list)
    os << record->get_title() << endl;
}

std::ostream& operator<< (std::ostream& os, const Collection& Collection){
  os << "Collection " << Collection.name << " contains:";
  if (Collection.list.empty())
    os << " None" << endl;
  else {
    os << endl;
    for (Record * record: Collection.list)
      os << * record;
  }
  return os;
}
