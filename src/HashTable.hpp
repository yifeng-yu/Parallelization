#pragma once
#include <string>
#include <utility>
#include <functional>
#include <iostream>

template <typename ValueType, 
	typename HashFuncType = std::hash<std::string>>
class HashTable
{
public:
	// Constructs the hash table with the given capacity (size of underlying array)
	HashTable(size_t capacity)
		:mTableData(new std::pair<std::string, ValueType>[capacity])
		,mCapacity(capacity)
		,mSize(0)
	{ }

	// TODO: Implement "Rule of five" functions
    // copy constructor deep copy
    HashTable(const HashTable& ht)
    {
        mTableData = new std::pair<std::string, ValueType>[ht.Capacity()];
        mCapacity = ht.Capacity();
        mSize = ht.Size();
//        //std::memcpy(mTableData, ht.mTableData, mSize+1);
        for(int i = 0; i < mCapacity; i++){
            mTableData[i] = ht.mTableData[i];
        }
    }
    
    //destructor
    ~HashTable()
    {
        delete[] mTableData;
        mTableData = nullptr;
        
        //delete(mTableData);
        mCapacity = 0;
        mSize = 0;
    }
    
    // move constructor
    HashTable(HashTable&& ht)
    :mTableData(std::move(ht.mTableData))
    ,mCapacity(std::move(ht.mCapacity))
    ,mSize(std::move(ht.mSize))
    {
        ht.mSize = 0;
        ht.mCapacity = 0;
        ht.mTableData = nullptr;
    }
    
    // assignment opeartor
    HashTable& operator=(const HashTable& ht)
    {
        if(this != &ht){
            delete[] mTableData;
            // Deep copy
            mSize = ht.mSize;
            mCapacity = ht.mCapacity;
            mTableData = new std::pair<std::string, ValueType>[mCapacity];
            for(int i = 0; i < mCapacity; i++){
                mTableData[i].first = ht.mTableData[i].first;
                mTableData[i].second = ht.mTableData[i].second;
            }
        }
        return *this;
    }
    
    // move assignment operator
    HashTable& operator=(HashTable&& ht)
    {
        if(this != &ht){
            delete[] mTableData;
            // Shallow copy
            mSize = ht.mSize;
            mCapacity = ht.mCapacity;
            mTableData = ht.mTableData;

            // MUST set other mData to null
            ht.mTableData = nullptr;
            // Optional but a good idea:
            ht.mSize = 0;
            ht.mCapacity = 0;
        }
        return *this;
    }
	// Returns overall capacity of hash table (size of underlying array)
	size_t Capacity() const
	{
		return mCapacity;
	}

	// Returns the number of elements currently inserted in the hash table
	size_t Size() const
	{
		return mSize;
	}

	// Tries to insert (key, value) into hash table
	// Returns true if insert succeeds, false if not
	bool Insert(const std::string& key, const ValueType& value)
	{
		// TODO: Implement
        int index = mHashFunc(key) % Capacity();
        bool find = false;
        int sizeSoFar = 0;
        // means we have iterated through all the possibility in hashtable
        while(sizeSoFar < Capacity()){
            if(mTableData[index].first.empty()){
                find = true;
                break;
            }
            else {
                sizeSoFar++;
                if(index == Capacity()-1){
                    // wrap to 0
                    index = 0;
                }
                else {
                    index++;
                }
            }
        }
        
        if(find){
            mTableData[index].first = key;
            mTableData[index].second = value;
            mSize++;
            return true;
        }
		return false;
	}

	// Returns pointer to value for given key, or nullptr
	// if not in hash table
	ValueType* Find(const std::string& key) const
	{
		// TODO: Implement

        
        int index = mHashFunc(key) % Capacity();
        int sizeSoFar = 0;
        // means we have iterated through all the possibility in hashtable
        while(sizeSoFar < Capacity()){
            if(mTableData[index].first.empty()){
                return nullptr;
            }
            if(mTableData[index].first == key){
                return &mTableData[index].second;
            }
            else {
                sizeSoFar++;
                if(index == Capacity()-1){
                    // wrap to 0
                    index = 0;
                }
                else {
                    index++;
                }
            }
        }
		return nullptr;
	}

	// Executes provided lambda expression on each element inserted into the table
	void ForEach(std::function<void(const std::string& key, ValueType& value)> func)
	{
		// TODO: Implement
        int index = 0;
        int sizeSoFar = 0;
        while(index < Capacity()){
            if(!mTableData[index].first.empty()){
                func(mTableData[index].first, mTableData[index].second);
                sizeSoFar++;
                index++;
            }
            else {
                index++;
            }
            if(sizeSoFar == Size()){
                break;
            }
        }
	}

	// Gets a const pointer to underlying data (for testing)
	const std::pair<std::string, ValueType>* Data() const
	{
		return mTableData;
	}
private:
	// NOTE: you should not need any more member data!
	
	// Underlying hash table array
	std::pair<std::string, ValueType>* mTableData;
	// Allocated capacity of hash table array
	size_t mCapacity;
	// Number of elements in HashTable
	size_t mSize;
	// Hash function
	HashFuncType mHashFunc;
};
