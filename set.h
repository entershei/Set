//
// Created by irina on 17.06.18.
//

#ifndef SET_SET_H
#define SET_SET_H

#include <iterator>
#include <cassert>
#include <experimental/optional>

namespace my_Set {
    template<typename T>
    struct set;

    template<typename R>
    void swap(set<R> &a, set<R> &b) noexcept {
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

            bool less (node const &other) const {
                if (not x && not other.x) {
                    assert("less two not create obj");
                    return false;
                } else if (x && not other.x) {
                    return true;
                } else if (not x && other.x) {
                    return false;
                } else {
                    return x.value() < other.x.value();
                }
            }
        };

        mutable node end_fake;
        node *root;
        node *start;
    public:
        struct iterator_set {
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = int;
            using pointer = node *;
            using reference = value_type const &;

            friend set;

            iterator_set(const iterator_set &other) {
                it = other.it;
            }

            iterator_set &operator++() {
                if (it->right) {
                    it = it->right;

                    while (it->left) {
                        it = it->left;
                    }
                } else {
                    while (it->parent && (it->parent->less(*it))) {
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
                    while (it->parent && (it->less(*it->parent))) {
                        it = it->parent;
                    }

                    assert(it->parent && "can't --it");

                    it = it->parent;
                }

                return *this;
            };

            iterator_set operator--(int) {
                iterator_set in(it);
                --(*this);
                return in;
            }

            T const &operator*() const {
                return it->x.value();
            }

            T const &operator->() const {
                return *(operator*());
            }

            bool operator!=(const iterator_set &other) const {
                return it != other.it;
            }

            bool operator==(const iterator_set &other) const {
                return it == other.it;
            }

            iterator_set() : it(nullptr) {}

        //todo private:
            explicit iterator_set(node *it) : it(it) {}

            node* get_node() { return it; }

            pointer it;
        };

        using iterator = iterator_set;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        set() noexcept : end_fake{}, root(&end_fake), start(&end_fake) {
            end_fake.left = nullptr;
            end_fake.right = nullptr;
            end_fake.parent = nullptr;
        }

        const_iterator copy_dfs(const_iterator v) {
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

            return iterator(ret);
        }

        set(set const &other) : set() {
            root = copy_dfs(iterator(other.root)).get_node();

            start = root;
            while (start->left) {
                start = start->left;
            }
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
            if ((not v->left || v->left == &end_fake) && (!v->right || v->right == &end_fake)) {
                if (v != &end_fake) {
                    delete v;
                }

                return;
            }

            if (v->left && v != &end_fake) {
                dfs_with_delete(v->left);
            }

            if (v->right && v->right != &end_fake) {
                dfs_with_delete(v->right);
            }

            delete v;
        }

        void clear() {
            if (not empty()) {
                dfs_with_delete(root);

                root = &end_fake;
                start = &end_fake;
            }
        }

        bool empty() const noexcept {
            return start == &end_fake;
        }

        const_iterator help_lower_bound(node *v, T const &value) const noexcept {
            if (v->x == value || (v->left == nullptr && v->x > value)) {
                return iterator(v);
            }

            if (v->x > value) {
                return help_lower_bound(v->left, value);
            } else {
                if (v->right == nullptr) {
                    return ++(iterator(v));
                } else {
                    return help_lower_bound(v->right, value);
                }
            }
        }

        const_iterator lower_bound(T const &value) const noexcept {
            if (empty()) { return end(); }

            return help_lower_bound(root, value);
        }

        const_iterator upper_bound(T const &value) const noexcept {
            if (empty()) { return end(); }

            const_iterator lower = help_lower_bound(root, value);
            if (*lower == value) {
                ++lower;
            }

            return  lower;
        }

        const_iterator find(T const &value) const noexcept {
            const_iterator lower = lower_bound(value);

            if (lower != end() && *lower == value) {
                return lower;
            } else {
                return end();
            }
        }

        iterator erase(const_iterator pos) noexcept {
            assert(pos != end() && "can't erase end element");

            bool flag_delete_root = (pos.it == root);
            iterator ret = std::next(pos);;

            if (not pos.it->left && not pos.it->right) {
                delete pos.it;
            } else {
                pos.it->x = ret.it->x;
                ret.it->parent->left = ret.it->right;
                ret.it->right->parent = ret.it->parent->left;

                delete ret.it;

                ret = pos;
            }

            if (flag_delete_root) {
                root = &(*ret.it);
            }

            return ret;
        }

        std::pair<iterator, bool> insert(T const& value) {
            const_iterator fd = find(value);
            if (fd != end() && *fd == value) {
                return {end(), false};
            }

            node *ret = new node(value);

            if (fd == begin()) {
                start = ret;
            }

            ret->left = fd.it->left;
            fd.it->left = ret;

            return {iterator(ret), true};
        }

        const_iterator end() const {
            return const_iterator(&end_fake);
        }

        const_iterator begin() const {
            return iterator(start);
        }

        const_reverse_iterator rbegin() const {
            return reverse_iterator(end());
        }

        const_reverse_iterator rend() const {
            return reverse_iterator(begin());
        }

        template <typename R>
        friend void swap(set<R> &a, set<R> &b) noexcept;
    };
}

#endif //SET_SET_H
