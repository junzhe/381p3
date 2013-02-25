#ifndef COLLECTION_H
#define COLLECTION_H
#include <string>
#include <vector>
#include "Utility.h"
#include "Record.h"
/* Collections contain a name and a container of members,
represented as pointers to Records.
Collection objects manage their own Record container. 
The container of Records is not available to clients.

*/
class Collection{
public:
  // Construct a collection with the specified name and no members
 Collection(const std::string& name_)
   :name(name_),
    list(){}
  /*fill this in*/
  
  // Construct a Collection from an input file stream in save format, using the record list,
  // restoring all the Record information. 
  // Record list is needed to resolve references to record members.
  // No check made for whether the Collection already exists or not.
  // Throw Error exception if invalid data discovered in file.
  // String data input is read directly into the member variable.
  Collection(std::ifstream& is, const std::vector<Record*>& library);
  
  // Accessors
  std::string get_name() const
  {return name;}
  
  // Add the Record, throw exception if there is already a Record with the same title.
  void add_member(Record* record_ptr);
  // Return true if there are no members; false otherwise
  bool empty() const{return list.empty();}
  // Return true if the record is present, false if not.
  bool is_member_present(Record* record_ptr) const;
  // Remove the specified Record, throw exception if the record was not found.
  void remove_member(Record* record_ptr);
  void save(std::ostream& os) const;
  // discard all members
  void clear()
  {list.clear();}
  
  void combine(Collection* collection1_ptr, Collection* collection2_ptr);
  
  // Write a Collections's data to a stream in save format, with endl as specified.
  
  // This operator defines the order relation between Collections, based just on the name
  bool operator< (const Collection& rhs) const
  {return name<rhs.name;}
  
   friend std::ostream& operator<< (std::ostream& os, const Collection& Collection);
		
 private:
  std::string name;
  std::vector<Record*> list;
};

// Print the Collection data
std::ostream& operator<< (std::ostream& os, const Collection& Collection);

struct Collection_compare_name{
  bool operator()(const Collection* collection1_ptr, const Collection* collection2_ptr)const{
    return *collection1_ptr< *collection2_ptr;
  }
};

#endif
