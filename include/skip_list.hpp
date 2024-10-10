#ifndef _SKIP_LIST_HPP__
#define _SKIP_LIST_HPP__

#include <random>

namespace bit {

namespace __detail {

struct SkipListNodeBase {
    using floor_number_type = typename std::default_random_engine::result_type;
};

struct SkipListNodeFloor {
    SkipListNodeBase *prev;
    SkipListNodeBase *next;
};

template <typename T>
struct SkipListNode : public SkipListNodeBase {
public:
    T value;
    const floor_number_type floor_number;
    SkipListNodeFloor floor[0];

    template <typename ... Args>
    explicit SkipListNode(floor_number_type fn, Args&& ... args) :
        value(std::forward<Args>(args)...),
        floor_number(fn) {
        std::uninitialized_fill_n(floor, floor_number, SkipListNodeFloor{ this, this });
    }
};

template <>
struct SkipListNode<void> : public SkipListNodeBase {
public:
    floor_number_type floor_number;
    SkipListNodeFloor floor[0];

    template <typename ... Args>
    explicit SkipListNode(floor_number_type fn) :
        floor_number(fn) {
        std::uninitialized_fill_n(floor, floor_number, SkipListNodeFloor{ this, this });
    }
};


} // namespace __detail

template <typename Key, typename Value, typename KeyOfValue, typename Compare,
    typename __detail::SkipListNodeBase::floor_number_type MaxFloorNumber = 32>
class SkipList {
private:
    class Comparer {
    public:
        inline bool equal(const Key &left, const Key &right) const {
            return !(m_c_(left, right) || m_c_(right, left));
        }

        inline bool not_equal(const Key &left, const Key &right) const {
            return !equal(left, right);
        }

        inline bool less(const Key &left, const Key &right) const {
            return m_c_(left, right);
        }

        inline bool greater(const Key &left, const Key &right) const {
            return m_c_(right, left);
        }

        inline bool less_or_equal(const Key &left, const Key &right) const {
            return !greater(left, right);
        }

        inline bool greater_or_equal(const Key &left, const Key &right) const {
            return !less(left, right);
        }
    private:
        Compare m_c_;
    };
public:
    class SkipListIterator {
    private:
        friend SkipList<Key, Value, KeyOfValue, Compare, MaxFloorNumber>;
    public:
        using value_type = Value;
        using pointer = value_type *;
        using reference = value_type &;

        using base_type = __detail::SkipListNodeBase *;
        using self = SkipListIterator;
        using skip_list_head = __detail::SkipListNode<void>;
        using skip_list_node = __detail::SkipListNode<value_type>;

        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;

        using iterator_category = std::bidirectional_iterator_tag;

        using floor_number_type =
            typename __detail::SkipListNodeBase::floor_number_type;
        using floor_type = __detail::SkipListNodeFloor;
    public:
        explicit SkipListIterator(base_type pBase = nullptr, base_type m_head_ = nullptr) :
            m_head_(m_head_),
            m_ptr_(pBase) {
        }

        SkipListIterator(const self &) = default;
        self &operator=(const self &) = default;

        SkipListIterator(self &&it) : SkipListIterator(it) {
            it.m_head_ = nullptr;
            it.m_ptr_ = nullptr;
        }

        self &operator=(self &&it) {
            if (this != &it) {
                *this = it;

                it.m_head_ = nullptr;
                it.m_ptr_ = nullptr;
            }
            return *this;
        }

        self &operator++() {
            m_ptr_ = floor()->next;
            return *this;
        }

        self operator++(int) {
            self tmp = *this;
            m_ptr_ = floor()->next;
            return tmp;
        }

        self &operator--() {
            m_ptr_ = floor()->prev;
            return *this;
        }

        self operator--(int) {
            self tmp = *this;
            m_ptr_ = floor()->prev;
            return tmp;
        }

        reference operator*() const {
            return to_node()->value;
        }

        pointer operator->() const {
            return &(this->operator*());
        }

        explicit operator bool() const {
            return m_ptr_;
        }

        base_type base() const {
            return m_ptr_;
        }

        friend inline bool operator==(const self &a, const self &b) {
            return a.m_ptr_ == b.m_ptr_;
        }

        friend inline bool operator!=(const self &a, const self &b) {
            return !operator==(a, b);
        }
    private:
        inline void set_next_floor(floor_number_type fn, self it) {
            floor()[fn].next = it.m_ptr_;
        }

        inline void set_prev_floor(floor_number_type fn, self it) {
            floor()[fn].prev = it.m_ptr_;
        }

        inline floor_number_type floor_number() const {
            return is_head() ? to_head()->floor_number : to_node()->floor_number;
        }

        inline floor_type *floor() const {
            return is_head() ? to_head()->floor : to_node()->floor;
        }

        inline self get_index_prev(floor_number_type fn) const {
            return self(floor()[fn].prev, m_head_);
        }

        inline self get_index_next(floor_number_type fn) const {
            return self(floor()[fn].next, m_head_);
        }

        inline self get_max_link() const {
            return get_index_next(floor_number() - 1);
        }

        inline self get_min_link() const {
            return get_index_prev(floor_number() - 1);
        }

        inline skip_list_node *to_node() const {
            return (skip_list_node *)m_ptr_;
        }

        inline skip_list_head *to_head() const {
            return (skip_list_head *)m_ptr_;
        }

        inline bool is_head() const {
            return m_head_ == m_ptr_;
        }
    private:
        base_type m_head_;
        base_type m_ptr_;
    };
public:
    class NodeHandler {
    public:
        using key_type = Key;
        using value_type = Value;

        using size_type = std::size_t;

        using iterator = SkipListIterator;
    public:
        NodeHandler() : NodeHandler(iterator(), 0) {
        }

        explicit NodeHandler(iterator it) : NodeHandler(it, 1) {
        }

        explicit NodeHandler(iterator it, size_type s) : m_it_(it), m_size_(s) {
        }

        NodeHandler(NodeHandler &&nh) {
        }

        NodeHandler &operator=(NodeHandler &&nh) {
        }

        ~NodeHandler() {
        }

        void swap(NodeHandler &nh) {
        }



        NodeHandler(const NodeHandler &) = delete;
        NodeHandler &operator=(const NodeHandler &) = delete;
    private:
        iterator m_it_;
        size_type m_size_;
    };
public:
    using key_type = Key;
    using value_type = Value;

    using pointer = value_type *;
    using reference = value_type &;

    using key_of_value = KeyOfValue;
    using comparer = Comparer;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using floor_number_type = typename __detail::SkipListNodeBase::floor_number_type;
    using floor_type = __detail::SkipListNodeFloor;

    using skip_list_head = __detail::SkipListNode<void>;
    using skip_list_node = __detail::SkipListNode<value_type>;

    using self = SkipList<Key, Value, KeyOfValue, Compare, MaxFloorNumber>;

    using iterator = SkipListIterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
public:
    iterator begin() const {
        iterator tmp = m_it_;
        return ++tmp;
    }

    iterator end() const {
        return m_it_;
    }

    reverse_iterator rbegin() const {
        return reverse_iterator(begin);
    }

    reverse_iterator rend() const {
        return reverse_iterator(end);
    }

    size_type size() const {
        return m_size_;
    }

    bool empty() const {
        return 0 == size();
    }
public:
    size_type count(const key_type &key) const {
        size_type c = 0;
        iterator it = lower_bound(key);
        while (it != end() && m_c_.less_or_equal(m_kov_(*it), key)) {
            ++it;
            ++c;
        }
        return c;
    }

    bool contain(const key_type &key) const {
        return end() != find(key);
    }

    void clear() {
        erase(begin(), end());
    }
public:
    explicit SkipList(double p = 0.5) :
        m_gd_(1.0 - p), m_size_(0), m_it_(create_head()) {
        static_assert(MaxFloorNumber > 0, "MaxFloorNumber should greater than zero!");
    }

    explicit SkipList(iterator first, iterator last, double p = 0.5) :
        SkipList(p) {
        insert_equal(first, last);
    }

    explicit SkipList(std::initializer_list<value_type> ilist, double p = 0.5) :
        SkipList(p) {
        insert_equal(ilist);
    }

    SkipList(const self &sl) :
        m_c_(sl.m_c_),
        m_dre_(sl.m_dre_),
        m_gd_(sl.m_gd_),
        m_kov_(sl.m_kov_),
        m_size_(0),
        m_it_(create_head()) {
        insert_equal(sl.begin(), sl.end());
    }

    self &operator=(const self &sl) {
        if (this != &sl) {
            clear();

            m_c_ = sl.m_c_;
            m_dre_ = sl.m_dre_;
            m_gd_ = sl.m_gd_;
            m_kov_ = sl.m_kov_;

            insert_equal(sl.begin(), sl.end());
        }
        return *this;
    }

    SkipList(self &&sl) :
        m_c_(std::move(sl.m_c_)),
        m_dre_(std::move(sl.m_dre_)),
        m_gd_(std::move(sl.m_gd_)),
        m_kov_(std::move(sl.m_kov_)),
        m_size_(sl.m_size_),
        m_it_(std::move(sl.m_it_)) {
        sl.m_size_ = 0;
        sl.m_it_ = create_head();
    }

    self &operator=(self &&sl) {
        if (this != &sl) {
            clear();

            m_c_ = std::move(sl.m_c_);
            m_dre_ = std::move(sl.m_dre_);
            m_gd_ = std::move(sl.m_gd_);
            m_kov_ = std::move(sl.m_kov_);
            m_size_ = sl.m_size_;
            m_it_ = std::move(sl.m_it_);

            sl.m_size_ = 0;
            sl.m_it_ = create_head();
        }
        return *this;
    }

    ~SkipList() {
        clear();
        destory_head(m_it_);
    }

    iterator lower_bound(const key_type &key) const {
        floor_number_type fn = MaxFloorNumber;
        iterator it = end();
        iterator next = it;
        while (fn--) {
            next = it.get_index_next(fn);
            while (next != end() && m_c_.less(m_kov_(*next), key)) {
                it = next;
                next = it.get_index_next(fn);
            }
        }
        return next;
    }

    iterator upper_bound(const key_type &key) const {
        floor_number_type fn = MaxFloorNumber;
        iterator it = end();
        while (fn--) {
            iterator tmp = it.get_index_next(fn);
            while (tmp != end() && m_c_.less_or_equal(m_kov_(*tmp), key)) {
                it = tmp;
                tmp = it.get_index_next(fn);
            }
        }
        return ++it;
    }

    iterator find(const key_type &key) const {
        iterator pos = lower_bound(key);
        return (pos == end() || m_c_.greater(m_kov_(*pos), key)) ? end() : pos;
    }

    iterator insert_equal(const value_type &value) {
        return emplace_equal(value);
    }

    iterator insert_equal(size_type n, const value_type &value) {
        iterator ret = end();
        while (n--) {
            ret = insert_equal(value);
        }
        return ret;
    }

    iterator insert_equal(iterator it) {
        return insert_equal(*it);
    }

    void insert_equal(iterator first, iterator last) {
        while (first != last) {
            insert_equal(first++);
        }
    }

    void insert_equal(std::initializer_list<value_type> ilist) {
        for (const auto &i : ilist) {
            insert_equal(i);
        }
    }

    template <typename ... Args>
    iterator emplace_equal(Args&& ... args) {
        iterator it = create_node(std::forward<Args>(args)...);
        iterator pos = lower_bound(m_kov_(*it));
        return insert_unsafe(pos, it);
    }

    std::pair<iterator, bool> insert_unique(const value_type &value) {
        iterator pos = lower_bound(m_kov_(value));
        if (pos != end() && m_c_.less_or_equal(m_kov_(*pos), m_kov_(value))) {
            return { iterator(), false };
        }
        return { insert_unsafe(pos, create_node(value)), true };
    }

    std::pair<iterator, bool> insert_unique(value_type &&value) {
        iterator pos = lower_bound(m_kov_(value));
        if (pos != end() && m_c_.less_or_equal(m_kov_(*pos), m_kov_(value))) {
            return { iterator(), false };
        }
        return { insert_unsafe(pos, create_node(std::move(value))), true };
    }

    std::pair<iterator, bool> insert_unique(iterator it) {
        return insert_unique(*it);
    }

    std::pair<iterator, bool> insert_unique(size_type n, const value_type &value) {
        return std::min(static_cast<size_type>(1), n) ?
            insert_unique(value) : std::make_pair(iterator(), true);
    }

    void insert_unique(iterator first, iterator last) {
        while (first != last) {
            insert_unique(first++);
        }
    }

    void insert_unique(std::initializer_list<value_type> ilist) {
        for (const auto &i : ilist) {
            insert_equal(i);
        }
    }

    template <typename ... Args>
    std::pair<iterator, bool> emplace_unique(Args&& ... args) {
        iterator it = create_node(std::forward<Args>(args)...);
        iterator pos = lower_bound(m_kov_(*it));
        if (pos != end() && m_c_.less_or_equal(m_kov_(*pos), m_kov_(*it))) {
            destory_node(it);
            return { iterator(), false };
        }
        return { insert_unsafe(pos, create_node(it)), true };
    }

    iterator erase(iterator pos) {
        iterator it = pos++;
        it = extract(it);
        destory_node(it);
        return pos;
    }

    iterator erase(iterator first, iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return last;
    }

    iterator erase(const key_type &key) {
        iterator it = lower_bound(key);
        while (it != end() && m_c_.less_or_equal(m_kov_(*it), key)) {
            it = erase(it);
        }
        return it;
    }

    iterator extract(iterator it) {
        floor_number_type fn = it.floor_number();
        floor_type *fnp = it.floor();
        for (floor_number_type ifn = 0; ifn < fn; ++ifn) {
            iterator npos = it.get_index_next(ifn);
            iterator ppos = it.get_index_prev(ifn);

            npos.set_prev_floor(ifn, ppos);
            ppos.set_next_floor(ifn, npos);

            it.set_prev_floor(ifn, it);
            it.set_next_floor(ifn, it);
        }
        it.m_head_ = nullptr;
        --m_size_;
        return it;
    }

    iterator extract(iterator first, iterator last) {
        if (first == last) {
            return end();
        }

        iterator pBegin = first;
        iterator pEnd = last;
        --pEnd;

        // 双指针，尽可能的走索引加快统计
        floor_number_type fn = 0;
        while (pBegin != pEnd) {

            // 获取的pBegin为前向节点，pEnd为后向节点
            iterator pp = pBegin.get_index_prev(fn);
            iterator np = pEnd.get_index_next(fn);

            np.set_prev_floor(fn, pp);
            pp.set_next_floor(fn, np);

            pBegin.set_prev_floor(fn, pEnd);
            pEnd.set_next_floor(fn, pBegin);

            ++fn;

            while (pBegin != pEnd && pBegin.floor_number() <= fn) {
                pBegin = pBegin.get_index_next(fn);
            }
            while (pBegin != pEnd && pEnd.floor_number() <= fn) {
                pEnd = pEnd.get_index_prev(fn);
            }
        }

        if (pBegin == pEnd) {
            floor_type *p = pBegin.floor();
            floor_number_type pfn = pBegin.floor_number();
            while (fn < pfn) {
                pBegin.set_prev_floor(fn, pEnd);
                pEnd.set_next_floor(fn, pBegin);
                ++fn;
            }
        }

        // 进行统计
        iterator p = first;
        while (p != last) {
            p.m_head_ = nullptr;
            --m_size_;
            ++p;
        }

        return first;
    }

    iterator extract(const key_type &key) {
        iterator first = lower_bound(key);
        iterator last = upper_bound(key);
        return extract(first, last);
    }

    void merge(iterator it) {
        iterator pos = lower_bound(m_kov_(*it));
        insert_unsafe(pos, it);
        ++m_size_;
    }

    // 先找到插入前点，在进行插入
    void merge(iterator first, iterator last) {
        if (first == last) {
            return;
        }

        iterator pos = lower_bound(m_kov_(*first++));


    }

    void merge(self &sl, iterator it) {
        merge(sl.extract(it));
    }

    void merge(self &sl, iterator first, iterator last) {
        merge(sl.extract(first, last));
    }

    void merge(self &sl) {
        merge(sl.extract(sl.begin(), sl.end()));
    }
private:
    // 从pos（包括pos）向后第一个小于等于key
    iterator lower_bound_hint(iterator pos, const key_type &key) {
        // 上升阶段，其够到的最远节点仍小于key
        floor_number_type fn = pos.floor_number();
        iterator next = pos.get_max_link();
        while (next != end() && m_c_.less(m_kov_(*next), key)) {
            fn = next.floor_number();
            pos = next;
            next = pos.get_max_link();
        }
        // 下降阶段
        while (fn--) {
            next = pos.get_index_next(fn);
            while (next != end() && m_c_.less(m_kov_(*next), key)) {
                pos = next;
                next = pos.get_index_next(fn);
            }
        }
        return next;
    }

    // 从pos（包括pos）向后第一个小于等于key
    iterator upper_bound_hint(iterator pos, const key_type &key) {
        // 上升阶段，其够到的最远节点仍小于key
        floor_number_type fn = pos.floor_number();
        iterator next = pos.get_max_link();
        while (next != end() && m_c_.less_or_equal(m_kov_(*next), key)) {
            fn = next.floor_number();
            pos = next;
            next = pos.get_max_link();
        }
        // 下降阶段
        while (fn--) {
            next = pos.get_index_next(fn);
            while (next != end() && m_c_.less_or_equal(m_kov_(*next), key)) {
                pos = next;
                next = pos.get_index_next(fn);
            }
        }
        return next;
    }

    // 合并一段[first, last]到pos前
    iterator merge_unsafe(iterator pos, iterator first, iterator last) {
        iterator ret = pos;

        iterator prev = pos;
        --prev;

        // 自底向上，逐层连接

        floor_number_type fn = 0;

        while (first != last) {
            // 解开前向
            prev.set_next_floor(fn, first);
            first.set_prev_floor(fn, prev);

            // 解开后向
            pos.set_prev_floor(fn, last);
            last.set_next_floor(fn, pos);

            ++fn;

            while (fn >= first.floor_number()) {
                first = first.get_max_link();
            }

            while (fn >= last.floor_number()) {
                last = last.get_min_link();
            }

            while (fn >= pos.floor_number()) {
                pos = pos.get_max_link();
            }

            while (fn >= prev.floor_number()) {
                prev = prev.get_min_link();
            }
        }

        if (first == last) {
            floor_number_type pfn = first.floor_number();

            while (fn < pfn) {
                // 解开前向
                prev.set_next_floor(fn, first);
                first.set_prev_floor(fn, prev);

                // 解开后向
                pos.set_prev_floor(fn, last);
                last.set_next_floor(fn, pos);

                ++fn;

                while (fn >= pos.floor_number()) {
                    pos = pos.get_max_link();
                }

                while (fn >= prev.floor_number()) {
                    prev = prev.get_min_link();
                }
            }
        }

        return ret;
    }

    iterator insert_unsafe(iterator pos, iterator it) {
        floor_number_type itfn = it.floor_number();
        floor_number_type posfn = pos.floor_number();
        floor_number_type fn = 0;

        while (fn < itfn) {
            iterator ppos = pos.get_index_prev(fn);

            it.set_prev_floor(fn, ppos);
            it.set_next_floor(fn, pos);
            ppos.set_next_floor(fn, it);
            pos.set_prev_floor(fn, it);

            ++fn;

            while (fn >= posfn) {
                pos = pos.get_max_link();
                posfn = pos.floor_number();
            }
        }

        return it;
    }
private:
    iterator create_head() {
        std::allocator<skip_list_head> alloc;
        skip_list_head *p = (skip_list_head *)malloc(
            sizeof(skip_list_head) + MaxFloorNumber * sizeof(floor_type));
        std::allocator_traits<std::allocator<skip_list_head>>().construct(
            alloc, p, MaxFloorNumber);
        return iterator{ p, p };
    }

    template <typename ... Args>
    iterator create_node(Args&& ... args) {
        std::allocator<skip_list_node> alloc;
        floor_number_type fn = floor_number();
        skip_list_node *p = (skip_list_node *)malloc(
            sizeof(skip_list_node) + fn * sizeof(floor_type));
        std::allocator_traits<std::allocator<skip_list_node>>().construct(
            alloc, p, fn, std::forward<Args>(args)...);
        ++m_size_;
        return iterator{ p, m_it_.m_head_ };
    }

    void destory_head(iterator it) {
        free(it.to_head());
    }

    void destory_node(iterator it) {
        std::allocator<skip_list_node> alloc;
        std::allocator_traits<std::allocator<skip_list_node>>().destroy(
            alloc, it.to_node());
        free(it.to_node());
        --m_size_;
    }

    inline floor_number_type floor_number() {
        return std::min(MaxFloorNumber, m_gd_(m_dre_) + 1);
    }
private:
    key_of_value m_kov_;
    comparer m_c_;
    std::default_random_engine m_dre_;
    std::geometric_distribution<floor_number_type> m_gd_;
    size_type m_size_;
    iterator m_it_;
};

}

#endif // _SKIP_LIST_HPP__