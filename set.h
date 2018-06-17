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

    template <typename T>
    void swap(set<T> &a, set<T> &b) noexcept {
        std::swap(a.end_fake, b.end_fake);
        std::swap(a.root, b.root);
    }

    template <typename T>
    struct set {
    private:
        struct node {
            node *left;
            node *right;
            node *parent;
            std::experimental::optional<T> x;

            explicit node(T const  &x) : left(nullptr), right(nullptr), parent(nullptr), x(x) {}

            explicit node() : left(nullptr), right(nullptr), parent(nullptr), x() {}
        };

        node *root;
        node *start;
        node end_fake;
    public:
        template <typename V>
        struct iterator_set : public std::iterator<std::bidirectional_iterator_tag, V> {
            friend struct set<T>;

            template <typename W>
            iterator_set(const iterator_set<W> &other) : {
                it = other.it;
            }

            iterator_set& operator++() {
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

            iterator_set&operator--() {
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

            V &operator*() {
                return it->x;
            }

            bool operator!=(const iterator_set &other) const {
                return it != other.it;
            }

            bool operator==(const iterator_set &other) const {
                return it == other.it;
            }

        private:
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

        ~set() {
            clear();
        }

        void dfs_with_delete(node *v) {
            if (!v->left && !v->right) {
                delete(v);
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

        bool empty() {
            return start != &end_fake;
        }

        const_iterator lower_bound(T const&) const {

        }

        friend void swap(set &a, set &b);
    };
}

#endif //SET_SET_H
