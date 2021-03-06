#include "Collection.h"
#include "Record.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <limits>
#include <iterator>

using std::vector;
using std::ifstream;
using std::string;
using std::ostream;
using std::streamsize;
using std::numeric_limits;
using std::endl;
using std::ostream_iterator;

Collection::Collection(ifstream& is,const vector<Record*>& library){
  int total_members = 0;
  if (!(is >> name))
    throw Error("Invalid data found in file!");
  if (!(is >> total_members))
    throw Error("Invalid data found in file!");
  for (int i = 0; i < total_members; i++) {
    is.ignore(numeric_limits<streamsize>::max(), '\n');
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

void Collection::save(ostream& os) const{
  os << name << " " << list.size() << endl;
  for (Record * record: list)
    os << record->get_title() << endl;
}

void Collection::combine(Collection* collection1_ptr, Collection* collection2_ptr){
  list.assign(collection1_ptr->list.size()+collection2_ptr->list.size(), nullptr);
  //using set_union to combine two collection to the list
  auto it = set_union(collection1_ptr->list.begin(), collection1_ptr->list.end(), collection2_ptr->list.begin(), collection2_ptr->list.end(), list.begin(), Record_compare_title());
  list.resize(it-list.begin());
}

void Collection::collection_sort(){
  sort(list.begin(), list.end(), Record_compare_title());
}

std::ostream& operator<< (ostream& os, const Collection& collection){
  os << "Collection " << collection.name << " contains:";
  if (collection.list.empty())
    os << " None" << endl;
  else {
    os << endl;
    ostream_iterator<Record*> outiter(os, "");
    copy(collection.list.begin(), collection.list.end(), outiter);
  }
  return os;
}

std::ostream& operator<< (ostream& os, const Record* record){
  os<<* record;
  return os;
}
