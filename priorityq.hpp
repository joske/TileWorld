#include <functional>
#include <queue>
#include <unordered_set>
#include <vector>

template <class T, class Container = std::vector<T>,
          class Compare = std::less<typename Container::value_type>,
          class Hash = std::hash<T>>
class MyPriorityQueue {
public:
  MyPriorityQueue(const Compare &compare, const Hash &hash)
      : pq_(compare, Container()), set_(50, hash) {}

  void push(T item) {
    if (!contains(item)) {
      pq_.push(item);
      set_.emplace(item);
    }
  }
  T pop() {
    T top = pq_.top();
    set_.erase(top);
    pq_.pop();
    return top;
  }
  T top() { return pq_.top(); }
  bool contains(T item) { return set_.find(item) != set_.end(); }
  bool empty() const { return pq_.empty(); }

private:
  std::priority_queue<T, Container, Compare> pq_;
  std::unordered_set<T, Hash> set_;
};
