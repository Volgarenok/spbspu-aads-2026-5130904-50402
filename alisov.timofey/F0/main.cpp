#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace alisov
{

  enum class BucketState { Empty, Occupied, Deleted };

  struct Mail
  {
    std::string track_id;
    std::string current_office;
    std::string current_post;
    double weight = 0.0;
  };

  struct Link
  {
    std::string to_post;
    std::string to_office;
    double distance = 0.0;
    double cost = 0.0;
    double time = 0.0;
  };

  struct Office
  {
    std::string name;
    std::vector< Link > links;
    std::vector< std::string > local_mail_ids;

    Office() = default;
    Office(const Office &) = default;
    Office &operator=(const Office &) = default;
    Office(Office &&) noexcept = default;
    Office &operator=(Office &&) noexcept = default;
  };

  template < class K, class V >
  struct Bucket
  {
    K key;
    V value;
    int psl = -1;
    BucketState state = BucketState::Empty;

    Bucket() = default;
    Bucket(const Bucket &) = default;
    Bucket &operator=(const Bucket &) = default;
    Bucket(Bucket &&) noexcept = default;
    Bucket &operator=(Bucket &&) noexcept = default;
  };

  template < class K, class V >
  class RobinHoodHashMap
  {
  private:
    std::vector< Bucket< K, V > > table;
    size_t table_size = 0;
    size_t table_capacity = 0;
    static constexpr double MAX_LOAD_FACTOR = 0.7;

    size_t get_hash(const K &key) const
    {
      if (table_capacity == 0)
        return 0;
      return std::hash< K >{}(key) % table_capacity;
    }

    void insert_no_resize(const K &key, V value)
    {
      Bucket< K, V > entry;
      entry.key = key;
      entry.value = std::move(value);
      entry.psl = 0;
      entry.state = BucketState::Occupied;

      size_t idx = get_hash(key);

      while (true) {
        if (table[idx].state == BucketState::Empty || table[idx].state == BucketState::Deleted) {
          table[idx] = std::move(entry);
          table_size++;
          return;
        }

        if (table[idx].state == BucketState::Occupied && table[idx].key == entry.key) {
          table[idx].value = std::move(entry.value);
          return;
        }

        if (entry.psl > table[idx].psl) {
          std::swap(entry, table[idx]);
        }

        idx = (idx + 1) % table_capacity;
        entry.psl++;
      }
    }

    void rehash()
    {
      size_t old_capacity = table_capacity;
      auto old_table = std::move(table);

      table_capacity = (old_capacity == 0) ? 8 : old_capacity * 2;
      table.assign(table_capacity, Bucket< K, V >());
      table_size = 0;

      for (size_t i = 0; i < old_capacity; ++i) {
        if (old_table[i].state == BucketState::Occupied) {
          insert_no_resize(old_table[i].key, std::move(old_table[i].value));
        }
      }
    }

  public:
    struct Pair
    {
      const K &first;
      V &second;
    };

    struct ConstPair
    {
      const K &first;
      const V &second;
    };

    class Iterator
    {
    private:
      std::vector< Bucket< K, V > > *table = nullptr;
      size_t index = 0;

      void skip_empty()
      {
        if (!table)
          return;
        while (index < table->size() && (*table)[index].state != BucketState::Occupied) {
          index++;
        }
      }

    public:
      Iterator(std::vector< Bucket< K, V > > *tbl, size_t idx):
        table(tbl),
        index(idx)
      {
        skip_empty();
      }

      Pair operator*() const
      {
        return {(*table)[index].key, (*table)[index].value};
      }

      Iterator &operator++()
      {
        index++;
        skip_empty();
        return *this;
      }

      bool operator!=(const Iterator &other) const
      {
        return index != other.index || table != other.table;
      }

      bool operator==(const Iterator &other) const
      {
        return !(*this != other);
      }
    };

    class ConstIterator
    {
    private:
      const std::vector< Bucket< K, V > > *table = nullptr;
      size_t index = 0;

      void skip_empty()
      {
        if (!table)
          return;
        while (index < table->size() && (*table)[index].state != BucketState::Occupied) {
          index++;
        }
      }

    public:
      ConstIterator(const std::vector< Bucket< K, V > > *tbl, size_t idx):
        table(tbl),
        index(idx)
      {
        skip_empty();
      }

      ConstPair operator*() const
      {
        return {(*table)[index].key, (*table)[index].value};
      }

      ConstIterator &operator++()
      {
        index++;
        skip_empty();
        return *this;
      }

      bool operator!=(const ConstIterator &other) const
      {
        return index != other.index || table != other.table;
      }

      bool operator==(const ConstIterator &other) const
      {
        return !(*this != other);
      }
    };

    Iterator begin()
    {
      return Iterator(&table, 0);
    }

    Iterator end()
    {
      return Iterator(&table, table.size());
    }

    ConstIterator begin() const
    {
      return ConstIterator(&table, 0);
    }

    ConstIterator end() const
    {
      return ConstIterator(&table, table.size());
    }

    RobinHoodHashMap() = default;
    RobinHoodHashMap(const RobinHoodHashMap &) = default;
    RobinHoodHashMap &operator=(const RobinHoodHashMap &) = default;
    RobinHoodHashMap(RobinHoodHashMap &&) noexcept = default;
    RobinHoodHashMap &operator=(RobinHoodHashMap &&) noexcept = default;

    RobinHoodHashMap(size_t initial_capacity):
      table_capacity(initial_capacity)
    {
      table.resize(table_capacity);
    }

    size_t size() const
    {
      return table_size;
    }

    bool empty() const
    {
      return table_size == 0;
    }

    void insert(const K &key, V value)
    {
      if (table_capacity == 0 || (double)table_size / table_capacity >= MAX_LOAD_FACTOR) {
        rehash();
      }
      insert_no_resize(key, std::move(value));
    }

    V *find(const K &key)
    {
      if (table_capacity == 0)
        return nullptr;
      size_t idx = get_hash(key);
      int current_psl = 0;

      while (true) {
        if (table[idx].state == BucketState::Empty)
          return nullptr;
        if (current_psl > table[idx].psl)
          return nullptr;

        if (table[idx].state == BucketState::Occupied && table[idx].key == key) {
          return &table[idx].value;
        }

        idx = (idx + 1) % table_capacity;
        current_psl++;
      }
    }

    bool remove(const K &key)
    {
      if (table_capacity == 0)
        return false;
      size_t idx = get_hash(key);
      int current_psl = 0;

      while (true) {
        if (table[idx].state == BucketState::Empty)
          return false;
        if (current_psl > table[idx].psl)
          return false;

        if (table[idx].state == BucketState::Occupied && table[idx].key == key) {
          table[idx].state = BucketState::Deleted;
          table[idx].psl = -1;
          table_size--;
          return true;
        }

        idx = (idx + 1) % table_capacity;
        current_psl++;
      }
    }
  };

  struct PostSystem
  {
    std::string name;
    RobinHoodHashMap< std::string, Office > offices;

    PostSystem() = default;
    PostSystem(const PostSystem &) = default;
    PostSystem &operator=(const PostSystem &) = default;
    PostSystem(PostSystem &&) noexcept = default;
    PostSystem &operator=(PostSystem &&) noexcept = default;
  };

  class PostManager
  {
  private:
    RobinHoodHashMap< std::string, PostSystem > systems;
    RobinHoodHashMap< std::string, Mail > global_mails;

  public:
    void make_post(const std::string &post_name)
    {
      if (systems.find(post_name) != nullptr) {
        throw std::invalid_argument("Already exist");
      }
      PostSystem new_system;
      new_system.name = post_name;
      systems.insert(post_name, std::move(new_system));
    }

    void show_post(const std::string &post_name)
    {
      PostSystem *sys = systems.find(post_name);
      if (sys == nullptr) {
        throw std::invalid_argument("Post not found");
      }

      size_t mail_count = 0;
      for (auto off_pair : sys->offices) {
        mail_count += off_pair.second.local_mail_ids.size();
      }

      std::cout << "<OFFICES: " << sys->offices.size() << ", MAILS: " << mail_count << ">\n";
    }

    void add_office(const std::string &post_name, const std::string &office_name)
    {
      PostSystem *sys = systems.find(post_name);
      if (sys == nullptr || sys->offices.find(office_name) != nullptr) {
        throw std::invalid_argument("Invalid post or duplicate");
      }
      Office off;
      off.name = office_name;
      sys->offices.insert(office_name, std::move(off));
    }

    void show_office(const std::string &post_name, const std::string &office_name)
    {
      PostSystem *sys = systems.find(post_name);
      if (sys == nullptr) {
        throw std::invalid_argument("Post not found");
      }
      Office *off = sys->offices.find(office_name);
      if (off == nullptr) {
        throw std::invalid_argument("Office not found");
      }
      std::cout << "<NAME: " << off->name << ", MAILS: " << off->local_mail_ids.size() << ">\n";
    }

    void add_mail(const std::string &post_name, const std::string &track_id, const std::string &office_name,
                  double weight)
    {
      PostSystem *sys = systems.find(post_name);
      if (sys == nullptr || weight <= 0) {
        throw std::invalid_argument("Invalid weight or post");
      }
      Office *off = sys->offices.find(office_name);
      if (off == nullptr || global_mails.find(track_id) != nullptr) {
        throw std::invalid_argument("Invalid office or duplicate ID");
      }

      Mail m;
      m.track_id = track_id;
      m.current_office = office_name;
      m.current_post = post_name;
      m.weight = weight;

      global_mails.insert(track_id, std::move(m));
      off->local_mail_ids.push_back(track_id);
    }

    void show_mail(const std::string &post_name, const std::string &track_id)
    {
      PostSystem *sys = systems.find(post_name);
      Mail *m = global_mails.find(track_id);
      if (sys == nullptr || m == nullptr || m->current_post != post_name) {
        throw std::invalid_argument("Mail not found in this post");
      }
      std::cout << "<TRACK: " << m->track_id << ", OFFICE: " << m->current_office << ", WEIGHT: " << m->weight << ">\n";
    }

    void move_mail(const std::string &post_name, const std::string &track_id, const std::string &new_office_name)
    {
      PostSystem *sys = systems.find(post_name);
      Mail *m = global_mails.find(track_id);
      if (sys == nullptr || m == nullptr || m->current_post != post_name) {
        throw std::invalid_argument("Invalid system or mail ownership");
      }
      Office *old_off = sys->offices.find(m->current_office);
      Office *new_off = sys->offices.find(new_office_name);
      if (old_off == nullptr || new_off == nullptr) {
        throw std::invalid_argument("Invalid source or target office");
      }

      auto &v = old_off->local_mail_ids;
      v.erase(std::remove(v.begin(), v.end(), track_id), v.end());

      new_off->local_mail_ids.push_back(track_id);
      m->current_office = new_office_name;
    }

    void find_weight(const std::string &post_name, double left, double right)
    {
      if (systems.find(post_name) == nullptr || left > right) {
        throw std::invalid_argument("Invalid bounds or post");
      }

      std::vector< Mail > result;
      for (const auto pair : global_mails) {
        if (pair.second.current_post == post_name) {
          if (pair.second.weight >= left && pair.second.weight <= right) {
            result.push_back(pair.second);
          }
        }
      }

      std::sort(result.begin(), result.end(), [](const Mail &a, const Mail &b) {
        return a.track_id < b.track_id;
      });

      std::cout << "<MAILS:";
      for (size_t i = 0; i < result.size(); ++i) {
        std::cout << " " << result[i].track_id << " (" << result[i].weight << ")";
        if (i + 1 < result.size())
          std::cout << ",";
      }
      std::cout << ">\n";
    }

    void link_offices(const std::string &post_name, const std::string &off1, const std::string &off2, double dist)
    {
      PostSystem *sys1 = systems.find(post_name);
      if (sys1 == nullptr || dist < 0) {
        throw std::invalid_argument("Invalid distance or post");
      }
      Office *o1 = sys1->offices.find(off1);
      if (o1 == nullptr) {
        throw std::invalid_argument("Office 1 not found");
      }

      PostSystem *sys2 = nullptr;
      Office *o2 = nullptr;

      for (auto pair : systems) {
        Office *found = pair.second.offices.find(off2);
        if (found != nullptr) {
          sys2 = &pair.second;
          o2 = found;
          break;
        }
      }

      if (o2 == nullptr) {
        throw std::invalid_argument("Office 2 not found");
      }

      o1->links.push_back({sys2->name, off2, dist, dist * 1.5, dist * 0.1});
      o2->links.push_back({sys1->name, off1, dist, dist * 1.5, dist * 0.1});

      std::cout << "<LINKED: " << off1 << " - " << off2 << ", DISTANCE: " << dist << ">" << std::endl;
    }

    struct RouteResult
    {
      std::vector< std::string > path;
      double total_metric = -1;
    };

    RouteResult calculate(const std::string &start_post, const std::string &start_office,
                          const std::string &target_office, int mode)
    {
      struct NodeState
      {
        std::string post;
        std::string office;
        double dist = 1e9;
        std::string parent_post = "";
        std::string parent_office = "";
        bool visited = false;
      };

      std::vector< NodeState > states;
      std::string target_post = "";

      for (auto sys_pair : systems) {
        for (auto off_pair : sys_pair.second.offices) {
          states.push_back({sys_pair.first, off_pair.first, 1e9, "", "", false});
          if (off_pair.first == target_office) {
            target_post = sys_pair.first;
          }
        }
      }

      auto get_state = [&states](const std::string &p, const std::string &o) -> NodeState * {
        for (auto &s : states)
          if (s.post == p && s.office == o)
            return &s;
        return nullptr;
      };

      NodeState *start_state = get_state(start_post, start_office);
      if (!start_state)
        return {{}, -1};

      start_state->dist = 0;

      for (size_t i = 0; i < states.size(); ++i) {
        NodeState *min_node = nullptr;
        for (auto &s : states) {
          if (!s.visited && (min_node == nullptr || s.dist < min_node->dist)) {
            min_node = &s;
          }
        }

        if (min_node == nullptr || min_node->dist >= 1e9)
          break;
        min_node->visited = true;

        if (min_node->office == target_office)
          break;

        PostSystem *sys = systems.find(min_node->post);
        if (!sys)
          continue;
        Office *off = sys->offices.find(min_node->office);
        if (!off)
          continue;

        for (const auto &link : off->links) {
          NodeState *to_state = get_state(link.to_post, link.to_office);
          if (!to_state || to_state->visited)
            continue;

          double weight = link.distance;
          if (mode == 1)
            weight = link.cost;
          if (mode == 2)
            weight = link.time;

          if (min_node->dist + weight < to_state->dist) {
            to_state->dist = min_node->dist + weight;
            to_state->parent_post = min_node->post;
            to_state->parent_office = min_node->office;
          }
        }
      }

      NodeState *target_state = get_state(target_post, target_office);
      if (!target_state || target_state->dist >= 1e9)
        return {{}, -1};

      std::vector< std::string > path;
      NodeState *curr = target_state;
      while (curr && curr->office != "") {
        path.push_back(curr->office);
        curr = get_state(curr->parent_post, curr->parent_office);
      }
      std::reverse(path.begin(), path.end());

      return {path, target_state->dist};
    }

    void route_mail(const std::string &post_name, const std::string &track_id, const std::string &target_office,
                    const std::string &type)
    {
      PostSystem *sys = systems.find(post_name);
      Mail *m = global_mails.find(track_id);
      if (sys == nullptr || m == nullptr || m->current_post != post_name) {
        throw std::invalid_argument("System or mail mismatch");
      }

      int mode = 0;
      std::string metric_name = "DISTANCE";
      if (type == "cheap") {
        mode = 1;
        metric_name = "COST";
      } else if (type == "fast") {
        mode = 2;
        metric_name = "TIME";
      } else if (type != "short") {
        throw std::invalid_argument("Invalid Command");
      }

      RouteResult res = calculate(m->current_post, m->current_office, target_office, mode);
      if (res.path.empty()) {
        throw std::runtime_error("Path not found");
      }

      std::cout << "<ROUTED: " << track_id << ", PATH: ";
      for (size_t i = 0; i < res.path.size(); ++i) {
        std::cout << res.path[i];
        if (i + 1 < res.path.size())
          std::cout << " -> ";
      }
      std::cout << ", TOTAL " << metric_name << ": " << res.total_metric << ">" << std::endl;

      std::string final_post = m->current_post;
      for (auto pair : systems) {
        if (pair.second.offices.find(target_office) != nullptr) {
          final_post = pair.first;
          break;
        }
      }

      Office *old_off = sys->offices.find(m->current_office);
      if (old_off) {
        auto &v = old_off->local_mail_ids;
        v.erase(std::remove(v.begin(), v.end(), track_id), v.end());
      }

      m->current_post = final_post;
      m->current_office = target_office;

      PostSystem *final_sys = systems.find(final_post);
      if (final_sys) {
        Office *new_off = final_sys->offices.find(target_office);
        if (new_off) {
          new_off->local_mail_ids.push_back(track_id);
        }
      }
    }
  };

  void process_commands()
  {
    PostManager manager;
    std::string cmd;

    std::unordered_map< std::string, std::function< void() > > command_handlers = {
        {"make-post",
         [&]() {
           std::string name;
           std::cin >> name;
           manager.make_post(name);
         }},
        {"show-post",
         [&]() {
           std::string name;
           std::cin >> name;
           manager.show_post(name);
         }},
        {"add-office",
         [&]() {
           std::string p_name, o_name;
           std::cin >> p_name >> o_name;
           manager.add_office(p_name, o_name);
         }},
        {"show-office",
         [&]() {
           std::string p_name, o_name;
           std::cin >> p_name >> o_name;
           manager.show_office(p_name, o_name);
         }},
        {"add-mail",
         [&]() {
           std::string p_name, t_id, o_name;
           double w;
           std::cin >> p_name >> t_id >> o_name >> w;
           manager.add_mail(p_name, t_id, o_name, w);
         }},
        {"show-mail",
         [&]() {
           std::string p_name, t_id;
           std::cin >> p_name >> t_id;
           manager.show_mail(p_name, t_id);
         }},
        {"move-mail",
         [&]() {
           std::string p_name, t_id, new_o;
           std::cin >> p_name >> t_id >> new_o;
           manager.move_mail(p_name, t_id, new_o);
         }},
        {"find-weight",
         [&]() {
           std::string p_name;
           double l, r;
           std::cin >> p_name >> l >> r;
           manager.find_weight(p_name, l, r);
         }},
        {"link-offices",
         [&]() {
           std::string p_name, o1, o2;
           double d;
           std::cin >> p_name >> o1 >> o2 >> d;
           manager.link_offices(p_name, o1, o2, d);
         }},
        {"route-mail", [&]() {
           std::string p_name, t_id, t_off, type;
           std::cin >> p_name >> t_id >> t_off >> type;
           manager.route_mail(p_name, t_id, t_off, type);
         }}};
    while (std::cin >> cmd) {
      auto it = command_handlers.find(cmd);
      if (it != command_handlers.end()) {
        try {
          it->second();
        } catch (const std::exception &e) {
          std::cout << e.what() << "\n";
        }
      } else {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
  }
}

int main()
{
  alisov::process_commands();
}