#include <iostream>
#include "json.hpp"

#include "priorityqueue.h"

PriorityQueue::PriorityQueue(std::size_t max_size) :
	nodes_(max_size + 1, KeyValuePair()),
	max_size_(max_size),
	size_(0) {
}

void PriorityQueue::insert(Key k) {
	insert(std::make_pair(k, std::make_pair(0, 0)));
}


void PriorityQueue::insert(KeyValuePair kv) {
	// TODO: complete this function
    size_ += 1;                            //size is increased by one
    nodes_[size_] = kv;                    //Adding the keyvaluepair kv as bottom most right most node
    // nodes_.push_back(kv);                  
    heapifyUp(size_);                      //heapify up the new node to update the heap
    return; 
}

KeyValuePair PriorityQueue::min() {
	// TODO: complete this function
    return nodes_[ROOT];                     //the minimum keyvaluepair is returned
}

KeyValuePair PriorityQueue::removeMin() {
	// TODO: complete this function
    auto min = nodes_[ROOT];       //setting a variable to hold onto the minimum keyvaluepair 
    removeNode(ROOT);                 //removing the root node
    return min;                    //the minimum keyvaluepair is returned
}

bool PriorityQueue::isEmpty() const {
	// TODO: complete this function
    if (size_ == 0){    //size_ shows number of keyvaluepairs. if 0, then it is empty
        return true;   //Returning true for empty priority queue
    }
    else{                //If size is not == 0, then it's not empty
        return false;    //returning false for a not empty priority queue
    }
}

size_t PriorityQueue::size() const {
	// TODO: complete this function
    return size_;                             //returns gthe size of the priority queue
}

nlohmann::json PriorityQueue::JSON() const {
	nlohmann::json result;
  for (size_t i = 1; i <= size_; i++) {
      nlohmann::json node;
      KeyValuePair kv = nodes_[i];
      node["key"] = kv.first;
      node["value"] = kv.second;
      if (i != ROOT) {
          node["parent"] = std::to_string(i / 2);
      }
      if (2 * i <= size_) {
          node["leftChild"] = std::to_string(2 * i);
      }
      if (2 * i + 1 <= size_) {
          node["rightChild"] = std::to_string(2 * i + 1);
      }
      result[std::to_string(i)] = node;
  }
  result["metadata"]["max_size"] = max_size_;        //Added this to print out the extra maxsize in metadata
  result["metadata"]["maxHeapSize"] = max_size_;    //Added this to print out the extra maxsize in metadata
  result["metadata"]["size"] = size_;
	return result;
}

void PriorityQueue::heapifyUp(size_t i) {
	// TODO: complete this function
    if (i == 1){                    //node is the root, then nothing needs to be done
        return;                     //The function ends as you cannot heapify up past the root
    }
    else if (nodes_[i].first < nodes_[i/2].first){    //If the child is less than the parent, the two swap. Parent = i/2
        std::swap(nodes_[i], nodes_[i/2]);      //This swaps the child and parent
        heapifyUp(i/2);                        //This recursively calls heapifyup again using the parent node's location
    }
    else{                                   //This happens if the child is greater than the parent
        return;                         //Nothing is to be done if the child is greater than the parent
    }
    return;                         //heapify up is done
}

void PriorityQueue::heapifyDown(size_t i) {
	// TODO: complete this function
    if (((2*i)+1) <= size_                              //if statement checks if the right child exists, is less than the parent, and is less than the left child
        && nodes_[i].first > nodes_[((2*i)+1)].first 
        && nodes_[((2*i)+1)].first < nodes_[(2*i)].first){
            std::swap(nodes_[i], nodes_[((2*i)+1)]);        //swaps the parent with the right child
            heapifyDown(((2*i)+1));                         //recursively calls heapify down for right child
    }
    else if ((2*i) <= size_ && nodes_[i].first > nodes_[2*i].first){     //if statement checks if the left child exists and is less than the parent
        std::swap(nodes_[i], nodes_[(2*i)]);                            //swaps the parent with the left child
        heapifyDown((2*i));                                            //recursively calls heapify down for the left child
    }
    else {                                  //returns if there is no need to swap
        return;
    }
    return;                   //heapify down is done
}

void PriorityQueue::removeNode(size_t i) {
	// TODO: complete this function
    if (size_ == 0){    //if the heap is empty, there is nothing to remove
        return;         //remove node ends
    }
    else {
        nodes_[i] = nodes_[size_];           //This replaces the want to be deleted node with the last node of the heap
        nodes_.pop_back();                 //Deleting the last node
        size_ -= 1;                      //size is decreased by one
        heapifyUp(i);                   //The replaced node undergoes heapify up  to update the heap
        heapifyDown(i);                  //The replaced node undergoes heapify down to update the heap
    }
    return;                              //Node has been removed successfully
}

Key PriorityQueue::getKey(size_t i) {
	// TODO: complete this function
    return nodes_[i].first;        //This returns the key at node i 
                                //May need to implement protection for if there is no keys
}
