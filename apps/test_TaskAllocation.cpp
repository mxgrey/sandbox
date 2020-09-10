
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <queue>
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>

#include <cassert>
#include <iostream>

struct State
{
  std::array<double, 2> p;
  double finish_time = 0.0;

  static State make(std::array<double, 2> p_)
  {
    return State{p_};
  }
};

class TaskRequest;
using ConstTaskRequestPtr = std::shared_ptr<TaskRequest>;

class Candidates
{
public:

  struct Entry
  {
    std::size_t candidate;
    State state;
  };

  using Map = std::multimap<double, Entry>;

  static Candidates make(
      const std::vector<State>& initial_states,
      const TaskRequest& request);

  Candidates(const Candidates& other)
  {
    _value_map = other._value_map;
    _update_map();
  }

  Candidates& operator=(const Candidates& other)
  {
    _value_map = other._value_map;
    _update_map();
    return *this;
  }

  Candidates(Candidates&&) = default;
  Candidates& operator=(Candidates&&) = default;

  struct Range
  {
    Map::const_iterator begin;
    Map::const_iterator end;
  };

  Range best_candidates() const
  {
    assert(!_value_map.empty());

    Range range;
    range.begin = _value_map.begin();
    auto it = range.begin;
    while (it->first == range.begin->first)
      ++it;

    range.end = it;
    return range;
  }

  double best_finish_time() const
  {
    assert(!_value_map.empty());
    return _value_map.begin()->first;
  }

  void update_candidate(std::size_t candidate, State state)
  {
    const auto it = _candidate_map.at(candidate);
    _value_map.erase(it);
    _candidate_map[candidate] = _value_map.insert(
      {state.finish_time, Entry{candidate, state}});
  }


private:
  Map _value_map;
  std::vector<Map::iterator> _candidate_map;

  Candidates(Map candidate_values)
    : _value_map(std::move(candidate_values))
  {
    _update_map();
  }

  void _update_map()
  {
    for (auto it = _value_map.begin(); it != _value_map.end(); ++it)
    {
      const auto c = it->second.candidate;
      if (_candidate_map.size() <= c)
        _candidate_map.resize(c+1);

      _candidate_map[c] = it;
    }
  }
};

class TaskRequest
{
public:

  virtual State estimate(const State& initial_state) const = 0;

  virtual double invariant_duration() const = 0;

};

struct PendingTask
{
  PendingTask(
      std::vector<State> initial_states,
      ConstTaskRequestPtr request_)
    : request(std::move(request_)),
      candidates(Candidates::make(initial_states, *request))
  {
    // Do nothing
  }

  ConstTaskRequestPtr request;
  Candidates candidates;
};

struct Assignment
{
  std::size_t task_id;
  State state;
};

struct Invariant
{
  std::size_t task_id;
  double invariant_cost;
};

struct InvariantLess
{
  bool operator()(const Invariant& a, const Invariant& b) const
  {
    return a.invariant_cost < b.invariant_cost;
  }
};

struct Node
{
  using Assignments = std::vector<std::vector<Assignment>>;
  Assignments assignments;
  std::unordered_map<std::size_t, PendingTask> unassigned;

  using InvariantSet = std::multiset<Invariant, InvariantLess>;
  InvariantSet unassigned_invariants;
  double cost_estimate;

  void sort_invariants()
  {
    unassigned_invariants.clear();
    for (const auto& u : unassigned)
    {
      unassigned_invariants.insert(
            Invariant{u.first, u.second.request->invariant_duration()});
    }
  }

  void pop_unassigned(std::size_t task_id)
  {
    unassigned.erase(task_id);

    bool popped_invariant = false;
    for (auto it = unassigned_invariants.begin();
         it != unassigned_invariants.end(); ++it)
    {
      if (it->task_id == task_id)
      {
        popped_invariant = true;
        unassigned_invariants.erase(it);
      }
    }

    assert(popped_invariant);
  }
};

using NodePtr = std::shared_ptr<Node>;
using ConstNodePtr = std::shared_ptr<const Node>;

void print_node(const Node& node)
{
  std::cout << " -- " << node.cost_estimate << ": <";
  for (std::size_t a=0; a < node.assignments.size(); ++a)
  {
    if (a > 0)
      std::cout << ", ";

    std::cout << a << ": [";
    for (const auto i : node.assignments[a])
      std::cout << " " << i.task_id;
    std::cout << " ]";
  }

  std::cout << " -- ";
  bool first = true;
  for (const auto& u : node.unassigned)
  {
    if (first)
      first = false;
    else
      std::cout << ", ";

    std::cout << u.first << ":";
    const auto& range = u.second.candidates.best_candidates();
    for (auto it = range.begin; it != range.end; ++it)
      std::cout << " " << it->second.candidate;
  }

  std::cout << ">" << std::endl;
}

Candidates Candidates::make(
    const std::vector<State>& initial_states,
    const TaskRequest& request)
{
  Map initial_map;
  for (std::size_t s=0; s < initial_states.size(); ++s)
  {
    const auto& state = initial_states[s];
    const auto finish = request.estimate(state);
    initial_map.insert({finish.finish_time, Entry{s, finish}});
  }

  return Candidates(std::move(initial_map));
}

double g(const Node& n, bool details = false)
{
  double output = 0.0;

  std::size_t a = 0;
  for (const auto& agent : n.assignments)
  {
    if (details)
      std::cout << "Costs for agent " << a++ << std::endl;

    for (const auto& assignment : agent)
    {
      output += assignment.state.finish_time;

      if (details)
      {
        std::cout << " -- " << output << " <- " << assignment.state.finish_time
                  << std::endl;
      }
    }
  }

  return output;
}

class InvariantHeuristicQueue
{
public:

  InvariantHeuristicQueue(std::vector<double> initial_values)
  {
    assert(!initial_values.empty());
    std::sort(initial_values.begin(), initial_values.end());

    for (const auto value : initial_values)
      _stacks.push_back({value});
  }

  void add(double new_value)
  {
    // Add the new value to the smallest stack
    const double value = _stacks[0].back() + new_value;
    _stacks[0].push_back(value);

    // Find the largest stack that is still smaller than the current front
    const auto next_it = _stacks.begin() + 1;
    auto end_it = next_it;
    for (; end_it != _stacks.end(); ++end_it)
    {
      if (value <= end_it->back())
        break;
    }

    if (next_it != end_it)
    {
      // Rotate the vector elements to move the front stack to its new place
      // in the order
      std::rotate(_stacks.begin(), next_it, end_it);
    }
  }

  double compute_cost() const
  {
    double total_cost = 0.0;
    for (const auto& stack : _stacks)
    {
      // NOTE: We start iterating from i=1 because i=0 represents a component of
      // the cost that is already accounted for by g(n) and the variant
      // component of h(n)
      for (std::size_t i=1; i < stack.size(); ++i)
        total_cost += stack[i];
    }

    return total_cost;
  }

private:
  std::vector<std::vector<double>> _stacks;
};

double h(const Node& n, bool /*details*/ = false)
{
  std::vector<double> initial_queue_values;
  initial_queue_values.resize(
        n.assignments.size(), std::numeric_limits<double>::infinity());

  for (const auto& u : n.unassigned)
  {
    // We subtract the invariant duration here because otherwise its
    // contribution to the cost estimate will be duplicated in the next section,
    // which could result in an overestimate.
    const double variant_value =
        u.second.candidates.best_finish_time()
        - u.second.request->invariant_duration();

    const auto& range = u.second.candidates.best_candidates();
    for (auto it = range.begin; it != range.end; ++it)
    {
      const std::size_t candidate = it->second.candidate;
      if (variant_value < initial_queue_values[candidate])
        initial_queue_values[candidate] = variant_value;
    }
  }

  for (std::size_t i=0; i < initial_queue_values.size(); ++i)
  {
    auto& value = initial_queue_values[i];
    if (std::isinf(value))
    {
      // Clear out any infinity placeholders. Those candidates simply don't have
      // any unassigned tasks that want to use it.
      const auto& assignments = n.assignments[i];
      if (assignments.empty())
        value = 0.0;
      else
        value = assignments.back().state.finish_time;
    }
  }

  InvariantHeuristicQueue queue(std::move(initial_queue_values));
  // NOTE: It is crucial that we use the ordered set of unassigned_invariants
  // here. The InvariantHeuristicQueue expects the invariant costs to be passed
  // to it in order of smallest to largest. If that assumption is not met, then
  // the final cost that's calculated may be invalid.
  for (const auto& u : n.unassigned_invariants)
    queue.add(u.invariant_cost);

  return queue.compute_cost();
}

double f(const Node& n, bool details = false)
{
  return g(n, details) + h(n, details);
}

class Filter
{
public:

  enum class Type
  {
    Passthrough,
    Trie,
    Hash
  };

  Filter(Type type, const std::size_t N_tasks)
    : _type(type),
      _set(N_tasks, AssignmentHash(N_tasks))
  {
    // Do nothing
  }

  bool ignore(const Node& node);

private:

  struct TaskTable;

  struct AgentTable
  {
    std::unordered_map<std::size_t, std::unique_ptr<TaskTable>> agent;
  };

  struct TaskTable
  {
    std::unordered_map<std::size_t, std::unique_ptr<AgentTable>> task;
  };

  using Assignments = Node::Assignments;
  struct AssignmentHash
  {
    AssignmentHash(std::size_t N)
    {
      // We add 1 to N because
      _shift = std::ceil(std::log2(N+1));
    }

    std::size_t operator()(const Assignments& assignments) const
    {
      std::size_t output = 0;
      std::size_t count = 0;
      for (const auto& a : assignments)
      {
        for (const auto& s : a)
        {
          // We add 1 to the task_id to differentiate between task_id == 0 and
          // a task being unassigned.
          const std::size_t id = s.task_id + 1;
          output += id << (_shift * (count++));
        }
      }

      return output;
    }

    std::size_t _shift;
  };

  struct AssignmentEqual
  {
    bool operator()(const Assignments& A, const Assignments& B) const
    {
      if (A.size() != B.size())
        return false;

      for (std::size_t i=0; i < A.size(); ++i)
      {
        const auto& a = A[i];
        const auto& b = B[i];

        if (a.size() != b.size())
          return false;

        for (std::size_t j=0; j < a.size(); ++j)
        {
          if (a[j].task_id != b[j].task_id)
            return false;
        }
      }

      return true;
    }
  };

  using Set = std::unordered_set<Assignments, AssignmentHash, AssignmentEqual>;


  Type _type;
  AgentTable _root;
  Set _set;
};

bool Filter::ignore(const Node& node)
{
  if (_type == Type::Passthrough)
    return false;

  if (_type == Type::Hash)
    return !_set.insert(node.assignments).second;

  bool new_node = false;

  // TODO(MXG): Consider replacing this tree structure with a hash set

  AgentTable* agent_table = &_root;
  std::size_t a = 0;
  std::size_t t = 0;
  while(a < node.assignments.size())
  {
    const auto& current_agent = node.assignments.at(a);

    if (t < current_agent.size())
    {
      const auto& task_id = current_agent[t].task_id;
      const auto agent_insertion = agent_table->agent.insert({a, nullptr});
      if (agent_insertion.second)
        agent_insertion.first->second = std::make_unique<TaskTable>();

      auto* task_table = agent_insertion.first->second.get();

      const auto task_insertion = task_table->task.insert({task_id, nullptr});
      if (task_insertion.second)
      {
        new_node = true;
        task_insertion.first->second = std::make_unique<AgentTable>();
      }

      agent_table = task_insertion.first->second.get();
      ++t;
    }
    else
    {
      t = 0;
      ++a;
    }
  }

  return !new_node;
}

std::vector<ConstNodePtr> expand(ConstNodePtr parent, Filter& filter)
{
  std::vector<ConstNodePtr> new_nodes;
  new_nodes.reserve(parent->unassigned.size());
  for (const auto& u : parent->unassigned)
  {
    const auto& range = u.second.candidates.best_candidates();
    for (auto it = range.begin; it != range.end; ++it)
    {
      const auto& c = it->second;

      auto new_node = std::make_shared<Node>(*parent);
      new_node->assignments[c.candidate]
          .push_back(Assignment{u.first, c.state});

      new_node->pop_unassigned(u.first);

      for (auto& new_u : new_node->unassigned)
      {
        new_u.second.candidates.update_candidate(
              c.candidate, new_u.second.request->estimate(c.state));
      }

      new_node->cost_estimate = f(*new_node);

      if (filter.ignore(*new_node))
        continue;

      new_nodes.push_back(std::move(new_node));
    }
  }

  return new_nodes;
}

struct LowestCostEstimate
{
  bool operator()(const ConstNodePtr& a, const ConstNodePtr& b)
  {
    return b->cost_estimate < a->cost_estimate;
  }
};

ConstNodePtr solve(
    std::vector<State> initial_states,
    std::vector<ConstTaskRequestPtr> requests,
    const Filter::Type filter_type,
    const bool display = false)
{
  const auto start = std::chrono::steady_clock::now();
  auto initial_node = std::make_shared<Node>();

  initial_node->assignments.resize(initial_states.size());
  for (std::size_t i=0; i < requests.size(); ++i)
  {
    const auto r = requests[i];
    initial_node->unassigned.insert({i, PendingTask(initial_states, r)});
    initial_node->cost_estimate = f(*initial_node);
  }
  initial_node->sort_invariants();

  using Queue = std::priority_queue<
      ConstNodePtr,
      std::vector<ConstNodePtr>,
      LowestCostEstimate>;

  Queue queue;
  queue.push(initial_node);

  Filter filter(filter_type, requests.size());

  std::size_t total_queue_entries = 1;
  std::size_t total_queue_expansions = 0;
  while (!queue.empty())
  {
    if (display)
    {
      auto display_queue = queue;
      while (!display_queue.empty())
      {
        const auto top = display_queue.top();
        display_queue.pop();
        print_node(*top);
      }

      std::cout << "=========================" << std::endl;
    }

    const auto top = queue.top();
    queue.pop();

    if (top->unassigned.empty())
    {
      const auto finish = std::chrono::steady_clock::now();
      const auto elapsed = std::chrono::duration_cast<
          std::chrono::duration<double>>(finish - start);

      std::cout << "Time elapsed: " << elapsed.count() << std::endl;
      std::cout << "Winning cost: " << top->cost_estimate << std::endl;
      std::cout << "Final queue size: " << queue.size() << std::endl;
      std::cout << "Total queue expansions: " << total_queue_expansions << std::endl;
      std::cout << "Total queue entries: " << total_queue_entries << std::endl;

      // This is the solution criteria
      return top;
    }

    // We don't have a solution yet, so let's expand
    const auto new_nodes = expand(top, filter);
    ++total_queue_expansions;
    total_queue_entries += new_nodes.size();

    for (const auto& n : new_nodes)
      queue.push(n);
  }

  return nullptr;
}

double travel_time(
    std::array<double, 2> p0,
    std::array<double, 2> p1,
    const double speed = 1.0)
{
  const double dx = p1[0] - p0[0];
  const double dy = p1[1] - p0[1];
  const double dist = std::sqrt(dx*dx + dy*dy);
  return dist/speed;
}

class TravelTaskRequest : public TaskRequest
{
public:

  TravelTaskRequest(std::array<double, 2> p, double speed = 1.0)
    : _p(p),
      _speed(speed)
  {
    // Do nothing
  }

  static ConstTaskRequestPtr make(std::array<double, 2> p, double speed = 1.0)
  {
    return std::make_shared<TravelTaskRequest>(p, speed);
  }

  State estimate(const State& initial_state) const final
  {
    State output;
    output.p = _p;

    const auto& p0 = initial_state.p;
    output.finish_time =
        initial_state.finish_time + travel_time(p0, _p, _speed);

    return output;
  }

  double invariant_duration() const final
  {
    return 0.0;
  }

private:
  std::array<double, 2> _p;
  double _speed;
};

class DeliveryTaskRequest : public TaskRequest
{
public:

  DeliveryTaskRequest(
      std::array<double, 2> pickup,
      std::array<double, 2> dropoff)
    : _pickup(pickup),
      _dropoff(dropoff)
  {
    _invariant_duration = travel_time(_pickup, _dropoff);
  }

  static ConstTaskRequestPtr make(
      std::array<double, 2> pickup,
      std::array<double, 2> dropoff)
  {
    return std::make_shared<DeliveryTaskRequest>(pickup, dropoff);
  }

  State estimate(const State& initial_state) const final
  {
    State output;
    output.p = _dropoff;

    output.finish_time =
        initial_state.finish_time
        + travel_time(initial_state.p, _pickup)
        + _invariant_duration;

    return output;
  }

  double invariant_duration() const final
  {
    return _invariant_duration;
  }


private:
  std::array<double, 2> _pickup;
  std::array<double, 2> _dropoff;
  double _invariant_duration;
};

int main()
{
//  std::vector<ConstTaskRequestPtr> requests =
//  {
//    TravelTaskRequest::make({5, 5}),    // 0
//    TravelTaskRequest::make({-5, 5}),   // 1
//    TravelTaskRequest::make({10, -10}), // 2
//    TravelTaskRequest::make({1, 0}),    // 3
//    TravelTaskRequest::make({2, 30}),   // 4
//    TravelTaskRequest::make({12, 10})   // 5
//  };

  // We use the default seed for rng so that the delivery tasks are consistent
  std::default_random_engine rng;
  std::uniform_real_distribution<double> dist(-30, 30);

  const std::size_t N_requests = 15;
  std::vector<ConstTaskRequestPtr> requests;
  requests.reserve(N_requests);
  for (std::size_t i=0; i < N_requests; ++i)
  {
    std::array<double, 2> pickup = {dist(rng), dist(rng)};
    std::array<double, 2> dropoff = {dist(rng), dist(rng)};
    requests.emplace_back(DeliveryTaskRequest::make(pickup, dropoff));
  }

  std::vector<State> initial_states =
  {
    State::make({0, 0}),
    State::make({5, 0}),
    State::make({-5, 0})
  };

  const bool display = false;

  const auto solution = solve(
//        initial_states, requests, Filter::Type::Trie, display);
        initial_states, requests, Filter::Type::Hash, display);

  if (!solution)
  {
    std::cout << "No solution found! :(" << std::endl;
    return 0;
  }

  std::cout << "Assignments:\n";
  for (std::size_t i=0; i < solution->assignments.size(); ++i)
  {
    std::cout << i << ":";
    for (const auto& t : solution->assignments[i])
      std::cout << "  " << t.task_id;
    std::cout << "\n";
  }
}
