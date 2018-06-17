//
// Created by irina on 17.06.18.
//

#ifndef SET_SET_H
#define SET_SET_H

#include <iterator>
#include <cassert>
#include <experimental/optional>

namespace my_Set {
    struct set;

    template<typename T>
    void swap(set<T> &a, set<T> &b) noexcept {
        std::swap(a.end_fake, b.end_fake);
        std::swap(a.root, b.root);
    }

    template<typename T>
    struct set {
    private:
        struct node {
            node *left;
            node *right;
            node *parent;
            std::experimental::optional<T> x;

            explicit node(T const &x) : left(nullptr), right(nullptr), parent(nullptr), x(x) {}

            explicit node() : left(nullptr), right(nullptr), parent(nullptr), x() {}
        };

        node *root;
        node *start;
        node end_fake;
    public:
        template<typename V>
        struct iterator_set : public std::iterator<std::bidirectional_iterator_tag, V> {
            friend set;

            template<typename W>
            iterator_set(const iterator_set<W> &other) : {
                it = other.it;
            }

            iterator_set &operator++() {
                if (it->right) {
                    it = it->right;

                    while (it->left) {
                        it = it->left;
                    }
                } else {
                    while (it->parent && (it->parent->x < it->x)) {
                        it = it->parent;
                    }

                    assert(it->parent && "can't ++it");

                    it = it->parent;
                }

                return *this;
            }

            iterator_set operator++(int) {
                iterator_set in(it);
                ++(*this);
                return in;
            }

            iterator_set &operator--() {
                if (it->left) {
                    it = it->left;

                    while (it->right) {
                        it = it->right;
                    }
                } else {
                    while (it->parent && (it->x < it->parent->x)) {
                        it = it->parent;
                    }

                    assert(it->parent && "can't --it");

                    it = it->parent;
                }

                return *this;
            };

            const iterator_set operator--(int) {
                iterator_set in(it);
                --(*this);
                return in;
            }

            V const &operator*() const {
                return it->x;
            }

            bool operator!=(const iterator_set &other) const {
                return it != other.it;
            }

            bool operator==(const iterator_set &other) const {
                return it == other.it;
            }

            // todo private:
            iterator_set(node *it) : it(it) {}

            node *get() {
                return it;
            }

            node *it;
        };

        using iterator = iterator_set<T>;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        set() noexcept : root(&end_fake), end_fake{} {
            end_fake.left = &end_fake;
            end_fake.right = &end_fake;
            end_fake.parent = &end_fake;
            start = &end_fake;
        }

        const_iterator copy_dfs(const_iterator v) {
            if (not *v->left && not *v->right) {
                node *ret = new node(v.it->x);
                ret->left = ret->right = nullptr;
                return iterator(ret);
            }

            node *ret = new node(v.it->x);

            if (*v->left) {
                const_iterator cur_l = copy_dfs(v->left);
                cur_l.it->parent = ret;
                ret->left = cur_l.it;
            }

            if (*v->right) {
                const_iterator cur_r = copy_dfs(v->left);
                cur_r.it->parent = ret;
                ret->left = cur_r.it;
            }

            return ret;
        }

        set(set const &other) : set() {
            root = *copy_dfs(iterator(other.root));
            start = root;
        }

        set &operator=(set rhs) {
            swap(rhs);
            return *this;
        }

        ~set() {
            clear();
        }

        void swap(set const &rhs) {
            std::swap(root, rhs.root);
            std::swap(end_fake, rhs.end_fake);
            std::swap(start, rhs.start);
        }

        void dfs_with_delete(node *v) {
            if (!v->left && !v->right) {
                delete v;
                return;
            }

            if (v->left) {
                dfs_with_delete(v->left);
            }

            if (v->right) {
                dfs_with_delete(v->right);
            }
        }

        void clear() {
            dfs_with_delete(root);
        }

        bool empty() const noexcept {
            return start != &end_fake;
        }

        const_iterator lower_bound(node *v = root, T const &value) const noexcept {
            if (empty()) { return end(); }

            if (v->x == value || (v->left == nullptr && v->x > value)) {
                return iterator(v);
            }

            if (v->x > value) {
                lower_bound(v->left);
            } else {
                if (v->right == nullptr) {
                    return ++(iterator(v));
                } else {
                    lower_bound(v->right, value);
                }
            }
        }

        const_iterator upper_bound(node *v = root, T const &value) const noexcept {
            if (empty()) { return end(); }

            const_iterator lower = lower_bound(v, value);
            if (*lower == value) {
                ++lower;
            }

            return  lower;
        }

        const_iterator find(T const &value) const noexcept {
            const_iterator lower = lower_bound(root, value);

            if (*lower == value) {
                return lower;
            } else {
                return end();
            }
        }

        iterator erase(const_iterator pos) {
            assert(pos != end() && "can't erase end element");

            bool flag_delete_root = (pos == root);
            iterator ret;

            if (not pos.it->left && not pos.it->right) {
                ret = pos.it->parent;

                delete pos.it;
            } else {
                ret = std::next(pos);

                pos.it->x = ret.it->x;
                ret.it->parent->left = ret.it->right;
                ret.it->right->parent = ret.it->parent->left;

                delete ret.it;

                ret = pos;
            }

            if (flag_delete_root) {
                root = *ret;
            }

            return ret;
        }

        std::pair<iterator, bool> insert(T const& v) {
            if (find(v) != end()) {
                return {end(), false};
            }


        }

        const_iterator end() const {
            return iterator(&end_fake);
        }

        const_iterator begin() const {
            return start;
        }

        friend void swap(set &a, set &b);
    };
}

#endif //SET_SET_H
