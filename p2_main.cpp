#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include "Utility.h"
#include "Record.h"
#include "Collection.h"
#include <exception>
#include <functional>
#include <ctype.h>
#include <map>
using namespace std;
using namespace std::placeholders;

typedef void (*input_func)();

struct Record_compare_rating{
  bool operator()(const Record* record1_ptr, const Record* record2_ptr)const{
    return record1_ptr->get_rating()<record2_ptr->get_rating();
  }
};

//global variables for libraries and catalog, count
vector<Record *> library_title;
vector<Record *> library_id;
vector<Collection*> catalog;
int record_count;
int collection_count;

//Error handler, read remaning inputs and print error message
void error_handler(const char * const err_msg);

//Add record to the libraries
void add_record(Record * record);
//Add collection to catalog
void add_collection(Collection * collection);
//Delete record from libraries
void delete_record(Record* record);
//Delete collection from catalog
void delete_collection(Collection* collection);

//Helper for clear library
void library_clear_helper();
//Helper for clear catalog
void catalog_clear_helper();

//Read title from stdin, and find the record, throw Error if not found
Record* read_title_find_record();
//Read id from stdin, and find the record, throw Error if not found
Record* read_id_find_record();
//Read name from stdin, and find the collection, throw Error if not found
Collection* read_name_find_collection();
//Read title from stdin, check record exists, throw if doesn't exist
Record * check_record_title_exist();
//Read name from stdin, check collection exists, throw if doesn't exist
Collection* check_collection_name_exist();
//Given a name, return the pointer to the collection if found, throw Errors if not found
Collection* find_collection_by_name(string& name);
//Given a title, return the pointer to the record if found, throw Errors if not found
Record* find_record_by_title(const string& title);
//Given a id, return the pointer to the record if found, throw Errors if not found
Record* find_record_by_id(const int id);

//Check whether a record is a member of collection
bool record_is_member(const Collection * collection,Record * record);
//Check whether a collection is empty
bool collection_not_empty(const Collection * collection);

//Function for find record
void input_find_record_by_title();
//Function for print record
void input_print_record();
//Function for print collection
void input_print_collection();
//Function for print library
void input_print_Library();
//Fucntion for print Catalog
void input_print_Catalog();
//Function for print allocation
void input_print_allocation();
//Function for modify rating
void input_modify_rating();
//Function for add record
void input_add_record();
//Fucntion for add collection
void input_add_collection();
//Function for add member
void input_add_member();
//Function for delete record
void input_delete_record();
//Function for delete collection
void input_delete_collection();
//Function for delete member
void input_delete_member();
//Function for clear library
void input_clear_Library();
//Function for clear catalog
void input_clear_Catalog();
//Function for clear All
void input_clear_All();
//Function for save All
void input_save_All();
//Function for read saved file
void input_restore_All();
//Function for quit and destroy all
void input_quit();
void input_find_record_by_string();
void input_list_rating();
void input_collection_statics();

void error_handler(const char * const err_msg) {
    cout << err_msg << endl;
    cin.clear();
    if(strcmp(err_msg, "Could not read a title!")!=0){
      while ((char)cin.get() != '\n');
    }
    return;
}

void add_record(Record * record) {
    auto it = lower_bound(library_title.begin(),library_title.end(),record, Record_compare_title());
    library_title.insert(it,record);
    it = lower_bound(library_id.begin(),library_id.end(),record, Record_compare_id());
    library_id.insert(it,record);
    record_count++;
}

void add_collection(Collection * collection) {
    auto it = lower_bound(catalog.begin(), catalog.end(), collection, Collection_compare_name());
    catalog.insert(it,collection);
    collection_count++;
}

void delete_record(Record* record){
  auto it = lower_bound(library_title.begin(),library_title.end(),record, Record_compare_title());
  library_title.erase(it);
  it = lower_bound(library_id.begin(),library_id.end(),record, Record_compare_id());
  library_id.erase(it);
  delete record;
  record_count--;
}

void delete_collection(Collection* collection){
  auto it = lower_bound(catalog.begin(), catalog.end(), collection, Collection_compare_name());
  catalog.erase(it);
  delete collection;
  collection_count--;
}

void input_find_record_by_title(){
  Record* record_ptr = read_title_find_record();
  cout<< *record_ptr;
}

void input_print_record(){
  Record* record_ptr = read_id_find_record();
  cout<< *record_ptr;
}

Record* read_title_find_record(){
  string title;
  read_title_stdin(title);
  Record* record_ptr = find_record_by_title(title);
  return record_ptr;
}

Record* read_id_find_record(){
  int id;
  read_integer(id,cin);
  Record* record_ptr = find_record_by_id(id);
  return record_ptr;
}

Record* find_record_by_title(const string& title){
  Record probe_record(title);
  Record_compare_title comparator;
  auto it = lower_bound(library_title.begin(),library_title.end(),&probe_record, comparator);
  if(it==library_title.end()||comparator(&probe_record,*it)){
    throw Error("No record with that title!");
  }
  return *it;
}

Record* find_record_by_id(const int id){
  Record probe_record(id);
  Record_compare_id comparator;
  auto it = lower_bound(library_id.begin(),library_id.end(),&probe_record, comparator);
  if(it==library_id.end()||comparator(&probe_record,*it)){
    throw Error("No record with that ID!");
  }
  return *it;
}

void input_print_collection(){
  Collection * collection = read_name_find_collection();
  cout << * collection;
}

Collection* read_name_find_collection(){
  string name;
  cin >> name;
  return find_collection_by_name(name);
}

Collection* find_collection_by_name(string& name){
  Collection probe_collection(name);
  Collection_compare_name comparator;
  auto it = lower_bound(catalog.begin(), catalog.end(), &probe_collection, comparator);
  if (it == catalog.end()||comparator(&probe_collection,*it)){
    throw Error("No collection with that name!");
  }
  return (* it);
}

void input_print_Library(){
  if (library_title.empty())
    cout << "Library is empty" << endl;
  else {
    cout << "Library contains " << library_title.size() << " records:" << endl;
    for (Record * record: library_title)
      cout << * record;
  }
  return;
}

void input_print_Catalog(){
  if (catalog.empty())
    printf("Catalog is empty\n");
  else {
    cout << "Catalog contains " << catalog.size() << " collections:" << endl;
    for (Collection* collection: catalog)
      cout << *collection;
  }
  return;
}

void input_print_allocation() {
  cout << "Memory allocations:" << endl;
  cout << "Records: " << record_count << endl;
  cout << "Collections: " << collection_count << endl;
  return;
}

void input_modify_rating(){
  Record * record = read_id_find_record();
  int rating = 0;
  if (!(cin >> rating))
    throw Error("Could not read a title!");
  record->set_rating(rating);
  cout << "Rating for record " << record->get_ID() << " changed to " << rating << endl;
  return;
}

void input_add_record(){
  Record * record = check_record_title_exist();
  add_record(record);
  cout << "Record " << record->get_ID() << " added" << endl;
  return;
}

Record * check_record_title_exist() {
  string medium;
  cin >> medium;
  string title;
  read_title_stdin(title);
  try{
    find_record_by_title(title);
  }catch(Error& error){
    if(strcmp(error.msg,"No record with that title!")!=0){
      throw error;
    }else{
      return new Record(medium, title);
    }
  }
  throw Error("Library already has a record with this title!");
}

void input_add_collection(){
  Collection* collection = check_collection_name_exist();
  add_collection(collection);
  cout << "Collection " << collection->get_name() << " added" << endl;
  return;
}

Collection* check_collection_name_exist(){
  string name;
  cin>>name;
  try{
    find_collection_by_name(name);
  }catch(Error& error){
    if(strcmp(error.msg,"No collection with that name!")!=0){
      throw error;
    }else{
      return new Collection(name);
    }
  }
  throw Error("Catalog already has a collection with this name!");
}

void input_add_member(){
  Collection * collection = read_name_find_collection();
  Record * record = read_id_find_record();
  collection->add_member(record);
  cout << "Member " << record->get_ID() << " " << record->get_title() << " added" << endl;
}

void input_delete_record(){
  Record * record = read_title_find_record();
  if (find_if(catalog.begin(), catalog.end(), bind(record_is_member,_1, record))!=catalog.end())
    throw Error("Cannot delete a record that is a member of a collection!");
  cout << "Record " << record->get_ID() << " " << record->get_title() << " deleted" << endl;
  delete_record(record);
  return;
}

bool record_is_member(const Collection * collection, Record * record) {
    return collection->is_member_present(record);
}

bool collection_not_empty(const Collection * collection) {
    return !collection->empty();
}

void input_delete_collection(){
  Collection * collection = read_name_find_collection();
  cout << "Collection " << collection->get_name() << " deleted" << endl;
  delete_collection(collection);
}

void input_delete_member() {
  Collection * collection = read_name_find_collection();
  Record * record = read_id_find_record();
  collection->remove_member(record);
  cout << "Member " << record->get_ID() << " " << record->get_title() << " deleted" << endl;
  return;
}

void input_clear_Library(){
  library_clear_helper();
  cout << "All records deleted" << endl;
  return;
}

void library_clear_helper(){
  if (find_if(catalog.begin(), catalog.end(), collection_not_empty)!=catalog.end())
    throw Error("Cannot clear all records unless all collections are empty!");
  //for (Record * record: library_id)
    //delete record;
  library_id.clear();
  library_title.clear();
  record_count = 0;
  Record::reset_ID_counter();
  return;
}

void input_clear_Catalog() {
  catalog_clear_helper();
  cout << "All collections deleted" << endl;
  return;
}

void catalog_clear_helper(){
  for(Collection* collection: catalog)
    delete collection;
  catalog.clear();
  collection_count = 0;
  return;
}

void input_clear_All() {
  catalog_clear_helper();
  library_clear_helper();
  cout << "All data deleted" << endl;
  return;
}

void input_save_All(){
  string file_name;
  cin >> file_name;
  ofstream output(file_name.c_str());
  if (!output){
    throw Error("Could not open file!");
}
  output << library_title.size() << endl;
  for (Record * record: library_title)
    record->save(output);
  output << catalog.size() << endl;
  for(Collection* collection: catalog)
    collection->save(output);
  output.close();
  cout << "Data saved" << endl;
}

void input_restore_All(){
  string file_name;
  cin >> file_name;
  ifstream input(file_name.c_str());
  if (!input){
    throw Error("Could not open file!");
  }
  vector<Record *> library_title_sub(library_title);
  vector<Record *> library_id_sub(library_id);
  vector<Collection*> catalog_sub(catalog);
  Record::save_ID_counter();
  catalog.clear();
  library_title.clear();
  library_id.clear();
  record_count = 0;
  collection_count = 0;
  Record::reset_ID_counter();
  try {
    int num_records = 0;
    if (!(input >> num_records))
      throw Error("Invalid data found in file!");
    for (int i = 0; i < num_records; i++) {
      Record * record = new Record(input);
      add_record(record);
    }
    int num_collections = 0;
    if (!(input >> num_collections))
      throw Error("Invalid data found in file!");
    for (int i = 0; i < num_collections; i++) {
      Collection* collection = new Collection(input,library_title);
      add_collection(collection);
    }
    input.close();
    cout<<"Data loaded"<<endl;
    for (Record * record: library_title_sub)
      delete record;
    for (auto it = catalog_sub.begin();it!=catalog_sub.end();it++)
      delete (*it);
    library_title_sub.clear();
    library_id_sub.clear();
    catalog_sub.clear();
    return;
  }
  catch (Error & error) {
    catalog_clear_helper();
    library_clear_helper();
    library_title = library_title_sub;
    library_id = library_id_sub;
    catalog = catalog_sub;
    record_count = library_title_sub.size();
    collection_count = catalog.size();
    Record::restore_ID_counter();
    throw error;
  }
}

void input_quit(){
  catalog_clear_helper();
  library_clear_helper();
  cout << "All data deleted" << endl;
  cout << "Done" << endl;
  exit(0);
}

void input_find_record_by_string(){
  string key;
  bool found = false;
  cin>>key;
  transform(key.begin(), key.end(),key.begin(),::tolower);
  for(Record* record:library_title){
     string title_tmp = record->get_title();
     transform(title_tmp.begin(), title_tmp.end(),title_tmp.begin(),::tolower);
     if(title_tmp.find(key)!=string::npos){
       found = true;
       cout<< *record;
     }
   }
   if(!found){
     throw Error("No records contain that string!");
   }
}

void input_list_rating(){
  if(library_id.empty()){
    cout << "Library is empty" << endl;
  }
  vector<Record*> library_rating = library_title;
  auto compare_func = [](Record* p1, Record* p2){return p1->get_rating()>p2->get_rating();};
  sort(library_rating.begin(), library_rating.end(),compare_func);
  for(Record* record:library_rating){
    cout<< *record;
  }
  return;
}

void input_collection_statics(){
  map<Record*, int, Record_compare_id> record_map;
  int total_records = library_id.size();
  int more_than_one = 0;
  for(Record* record: library_id){
    for(Collection* collection: catalog){
      if(collection->is_member_present(record)){
	record_map[record]++;
      }
    }
  }
  int contained_record_size = record_map.size();
  cout<<contained_record_size<<" out of "<<total_records<<" Records appear in at least one Collection"<<endl;
  for(Record* record:library_id){
    if(record_map[record]>1){
      more_than_one++;
    }
  }
  cout<<more_than_one<<" out of "<<total_records<<" Records appear in at more than one Collection"<<endl;
  cout<<"Collections contain a total of "<<contained_record_size<<" Records"<<endl;
}

void input_combine_collection(){
  string name1;
  string name2;
  string name_combined;
  cin>>name1;
  cin>>name2;
  cin>>name_combined;
  Collection* collection1_ptr = find_collection_by_name(name1);
  Collection* collection2_ptr = find_collection_by_name(name2);
  try{
    find_collection_by_name(name_combined);
  }catch(Error& error){
    Collection* collection_combined = new Collection(name_combined);
    collection_combined->combine(collection1_ptr, collection2_ptr);
    cout<<"Collections "<<name1<<" and "<<name2<<" combined into new collection "<<name_combined<<endl;
    return;
  }
  throw Error("Catalog already has a collection with this name!");
}

void input_modify_title(){
  int id = 0;
  string title;
  read_integer(id, cin);
  Record probe_record_id(id);
  Record_compare_id comparator_id;
  auto it_id = lower_bound(library_id.begin(),library_id.end(),&probe_record_id, comparator_id);
  if(it_id==library_id.end()||comparator_id(&probe_record_id,*it_id)){
    throw Error("No record with that ID!");
  }
  string prev_title = (*it_id)->get_title();
  read_title_stdin(title);
  Record probe_record_title(title);
  Record_compare_title comparator_title;
  auto it_title = lower_bound(library_title.begin(),library_title.end(),&probe_record_title, comparator_title);
  if(it_title!=library_title.end()&&!comparator_title(&probe_record_title,*it_title)){
    throw Error("Library already has a record with this title!");
  }
  Record probe_record_prev_title(prev_title);
  auto it_prev_title = lower_bound(library_title.begin(),library_title.end(),&probe_record_prev_title, comparator_title);
  Record* record_new = (*it_id);
  library_title.erase(it_prev_title);
  record_new->set_title(title);
  for(Collection* collection:catalog){
    if(collection->is_member_present(&probe_record_prev_title)){
      collection->remove_member(&probe_record_prev_title);
      collection->add_member(record_new);
    }
  }
  add_record(record_new);
  cout<<"Title for record "<<id<<" changed to "<<title<<endl;
  return;
}

int main() {
    
    char input1 = 0;
    char input2 = 0;
   
    record_count = 0;
    collection_count = 0;
    map<string, input_func> func_map;
    func_map["fr"] = input_find_record_by_title;
    func_map["fs"] = input_find_record_by_string;
    func_map["pr"] = input_print_record;
    func_map["pc"] = input_print_collection;
    func_map["pL"] = input_print_Library;
    func_map["pC"] = input_print_Catalog;
    func_map["pa"] = input_print_allocation;
    func_map["mr"] = input_modify_rating;
    func_map["ar"] = input_add_record;
    func_map["ac"] = input_add_collection;
    func_map["am"] = input_add_member;
    func_map["dr"] = input_delete_record;
    func_map["dc"] = input_delete_collection;
    func_map["dm"] = input_delete_member;
    func_map["cL"] = input_clear_Library;
    func_map["cC"] = input_clear_Catalog;
    func_map["cA"] = input_clear_All;
    func_map["sA"] = input_save_All;
    func_map["rA"] = input_restore_All;
    func_map["lr"] = input_list_rating;
    func_map["cs"] = input_collection_statics;
    func_map["cc"] = input_combine_collection;
    func_map["mt"] = input_modify_title;
    func_map["qq"] = input_quit;
    
    while (true) {
        printf("\nEnter command: ");
        while (isspace(input1 = (char)cin.get()));
        while (isspace(input2= (char)cin.get()));
	string input_command;
	input_command+=input1;
	input_command+=input2;
        
	
        try {
          if(func_map.find(input_command)==func_map.end()){
	    throw Error("Unrecognized command!");
	  }
	  func_map[input_command]();
        }
        catch (Error & error) {
	  error_handler(error.msg);
        }
        catch (exception & exc) {
	  error_handler(exc.what());
        }
    }
    return 0;
}
