#ifndef AALTO_ELEC_CPP_RESTRICTED_PTR_CLASS
#define AALTO_ELEC_CPP_RESTRICTED_PTR_CLASS

#include "restricted_ref_counter.hpp"
#include <iostream>

/*
    Implement a class RestrictedPtr that holds a pointer of any type with at least the following members:
        - default constructor (takes no parameters)
        - constructor with a raw pointer parameter
        - copy constructor
        - destructor
        - copy assignment operator
        - function GetData that takes no parameters and returns the data the raw pointer refers to.
            The data returned should be such that modifying it modifies the data the pointer refers to.
        - function GetPointer that takes no parameters and returns the raw pointer
        - function GetRefCount that takes no parameters and returns an int with the reference count
    
    At any time the objects should keep track of how many references there are to a pointer
    and not allow more than 3 references. If the reference count drops to 0, the referenced
    pointer's memory should be released. If the reference count is already 3 when copying,
    the copier should set it's pointer to nullptr and reference count to 1.
    NOTE: GetPointer, GetData and GetRefCount are needed for the tests to run
*/

template <typename T>
class RestrictedPtr {
public:
    explicit RestrictedPtr() : j_(nullptr), count_(0) {
        count_ = new Restricted_Ref_Counter();
        count_->Counter_increase();
    }

    

    RestrictedPtr(T *ptr) :j_(ptr), count_(0) {
        count_ = new Restricted_Ref_Counter();
        if (j_) {
             count_->Counter_increase();
        }
    }

    RestrictedPtr(RestrictedPtr<T>& ptr) : j_(ptr.j_), count_(ptr.count_) {
                if (GetRefCount() != 3) {
                    count_->Counter_increase();
                } 
                else {
                    j_ = nullptr;
                    count_ = new Restricted_Ref_Counter();
                    count_->Counter_increase();  
                }
    }
    RestrictedPtr& operator= (const RestrictedPtr<T>& ptr){
    if (this != &ptr) {
        count_->Counter_decrease();
        if (GetRefCount() == 0) {
            delete j_;
             delete count_;
        }

        if (GetRefCount() != 3) {
            j_ = ptr.j_;
            count_ = ptr.count_;
            count_->Counter_increase();
        } 
        else {
            j_ = nullptr;
            count_ = new Restricted_Ref_Counter();
            count_->Counter_increase();    
        }
    }
    return *this;
    }

    ~RestrictedPtr<T>() {
        count_->Counter_decrease();
        if (GetRefCount() == 0)
        {
            delete j_;
            delete count_;
        }
    }

    

        

    T& GetData() const {
        return *j_;
    }

    T* operator->() const {
       return j_;
    }

    T& operator*() const {
        return *j_;
    }

    T* GetPointer() const {
        return j_;
    }

    int GetRefCount() const {
        return count_->GetCounter();
    }

private:
    T* j_;
    Restricted_Ref_Counter* count_;
};

#endif