#include <queue>
#include <unordered_set>

template<class T>
class MyPriorityQueue {
public:
   MyPriorityQueue() {}

   void push(T item) {
     if (!contains(item)){
       pq_.push(item);
       set_.emplace(item);
     }
   }
   void pop() {
     if (!empty()) {
       T top = pq_.top();
       set_.erase(top);
       pq_.pop();
     }
   }
   T top() { return pq_.top(); }
   bool contains(T item) { return set_.find(item) != set_.end(); }
   bool empty() const { return set_.empty(); }

 private:
   std::priority_queue<T> pq_;
   std::unordered_set<T> set_;
};