#include "Collection.h"
#include <iostream>
#include <fstream>
using namespace std;

Collection::Collection(std::ifstream& is,const Ordered_list<Record*, Less_than_ptr<Record*> >& library){
  int total_members = 0;
  String name_tmp;
  if (!(is >> name_tmp))
    throw Error("Invalid data found in file!");
  name = name_tmp;
  if (!(is >> total_members))
    throw Error("Invalid data found in file!");
  for (int i = 0; i < total_members; i++) {
    while (is.get() != '\n');
    String title;
    read_title_file(title, is);
    Record record(title);
    Ordered_list<Record*, Less_than_ptr<Record*> >::Iterator it = library.find(&record);
    if (it == library.end())
      throw Error("Invalid data found in file!");
    add_member(*it);
  }
}

void Collection::add_member(Record* record_ptr) {
  Ordered_list<Record*, Less_than_ptr<Record*> >::Iterator it = list.find(record_ptr);
  if(it!=list.end()){
    throw Error("Record is already a member in the collection!");
  }
  list.insert(record_ptr);
  return;
}

bool Collection::is_member_present(Record* record_ptr) const{
  Ordered_list<Record*, Less_than_ptr<Record*> >::Iterator it = list.find(record_ptr);
  return it != list.end();
}

void Collection::remove_member(Record* record_ptr) {
  Ordered_list<Record*, Less_than_ptr<Record*> >::Iterator it = list.find(record_ptr);
  if (it == list.end())
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
