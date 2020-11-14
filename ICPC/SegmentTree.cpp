#include <iostream>
#include <vector>

template <typename T>
class SegmentTree {
public:
    SegmentTree(std::vector<T> a);
    T Sum(int left, int right) {sum(1, left, right);};

private:
    struct Segment {
        Segment(int left, int right, T data) : left(left), right(right), data(data) {};
        Segment(int left, int right): left(left), right(right), data(0) {};
        int left;
        int right;
        T data;
    };
    void build(std::vector<T> a, int v, int left, int right);
    T sum(int v, int left, int right);
    std::vector<Segment> _data;
};

template <typename T>
SegmentTree<T>::SegmentTree(std::vector<T> a) {
    _data.assign(a.size() * 4, Segment(0, 0));
    build(a, 1, 0, a.size() - 1);
}

template <typename T>
void SegmentTree<T>::build(std::vector<T> a, int v, int left, int right) {
    _data[v].left = left;
    _data[v].right = right;
    if(left == right) {
        _data[v].data = a[left];
    } else {
        int mid = (left + right) / 2;
        build(a, 2 * v, left, mid);
        build(a, 2 * v + 1, mid + 1, right);
        _data[v].data = _data[v * 2].data + _data[v * 2 + 1].data;
    }
}

template <typename T>
T SegmentTree<T>::sum(int v, int left, int right) {
    if (_data[v].left == left && _data[v].right == right) {
        return _data[v].data;
    }
    int mid = (_data[v].left + _data[v].right) / 2;
    if(right <= mid) {
        return sum(v * 2, left, right);
    }
    if(left > mid) {
        return sum(v * 2 + 1, left, right);
    }
    return sum(v * 2, left, mid) + sum(v * 2 + 1, mid + 1, right);
}

int main() {
    std::vector a = {7, 9, 4, 8, 2, 5, 3, 2, 9, 0};
    SegmentTree<int> st(a);
    int left, right;
    while(1) {
        std::cin >> left >> right;
        std::cout << st.Sum(left, right);
    }
}

