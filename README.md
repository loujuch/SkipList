# Skip List

## 谓词封装

将输入的逻辑小于谓词封装为多种逻辑谓词

```c++
class Comparator {
public:
    inline bool less(const Key &left, const Key &right) {
        return m_c_(left, right);
    }

    inline bool greater(const Key &left, const Key &right) {
        return less(right, left);
    }

    inline bool less_or_equal(const Key &left, const Key &right) {
        return !greater(left, right);
    }

    inline bool greater_or_equal(const Key &left, const Key &right) {
        return !less(left, right);
    }

    inline bool not_equal(const Key &left, const Key &right) {
        return less(left, right) || right(left, right);
    }

    inline bool equal(const Key &left, const Key &right) {
        return !not_equal(left, right);
    }
private:
    Compare m_c_;
};
```

## 搜索

为了尽可能的利用构建的索引，从任意位置开始的向任意方向的搜索流程如下：

```c++
iterator search(iterator pos, const Key &key, std::function<bool(const Key &, const Key &)> func) {
    // 上升阶段：在此阶段只会发生楼层上升
    //   1. 每次都尝试跳到最远的地方
    //   2. 每次跳跃前判断目标是否符合条件
    iterator next = pos.get_max_next();
    while(next != end() && func(next, key)) {
        pos = next;
        next = pos.get_max_next();
    }

    // 下降阶段：此阶段只会发生楼层下降
    //   1. 不断进行平跳，直到下一跳不符合条件
    //   2. 降低一楼层
    floor_number_type fn = pos.floor_number() - 1;
    while(fn--) {
        next = pos.get_index_next(fn);

        while(next != end() && func(next, key)) {
            pos = next;
            next = pos.get_index_next(fn);
        }
    }

    return next;
}
```

## 