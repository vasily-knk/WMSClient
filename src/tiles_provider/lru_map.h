#pragma once

template<typename K, typename M>
class lru_map
{
public:
    typedef K key_type;
    typedef M mapped_type;
    typedef std::pair<const typename key_type, typename mapped_type> value_type;

private:
    typedef list<value_type> list_t;
    typedef typename list_t::iterator list_it_t;

    typedef unordered_map<key_type, list_it_t> map_t;
    typedef typename map_t::iterator map_it_t;

public:
    typedef typename list_t::const_iterator const_iterator;
    typedef typename list_t::iterator iterator;
    typedef typename list_t::const_reverse_iterator const_reverse_iterator;
    typedef typename list_t::reverse_iterator reverse_iterator;
    typedef value_type &reference;
    typedef const value_type &const_reference;

public:
    size_t count(const key_type& key) const
    {
        return map_.count(key);
    }

    void insert(const value_type &value)
    {
        if (count(value.first) != 0)
        {
            // TODO: dublication
            throw std::runtime_error("dumb_cache key dublication");
        }
        const list_it_t pos = list_.insert(list_.begin(), value);
        map_.insert(make_pair(value.first, pos));
    }

    
    iterator erase(iterator pos)
    {
        map_.erase(pos->first);
        return list_.erase(pos);
    }

    size_t erase(const key_type &key)
    {
        const map_it_t map_it = map_.find(key);
        if (map_it == map_.end())
            return 0;

        list_.erase(map_it.second);
        map_.erase(map_it);
        return 1;
    }

    mapped_type &request(const key_type &key)
    {
        list_it_t &it_ref = map_.at(key);
        list_.splice(list_.begin(), list_, it_ref);
        it_ref = list_.begin();
        return it_ref->second;
    }

    const_iterator find(const key_type &key) const
    {
        return map_.at(key);
    }

    iterator find(const key_type &key)
    {
        return map_.at(key);
    }

    size_t size() const
    {
        return map_.size();
    }

public:
    const_iterator begin() const
    {
        return list_.begin();
    }

    const_iterator end() const
    {
        return list_.end();
    }

    iterator begin()
    {
        return list_.begin();
    }

    iterator end()
    {
        return list_.end();
    }

    const_reverse_iterator rbegin() const
    {
        return list_.rbegin();
    }

    const_reverse_iterator rend() const
    {
        return list_.rend();
    }

    reverse_iterator rbegin()
    {
        return list_.rbegin();
    }

    reverse_iterator rend()
    {
        return list_.rend();
    }

private:
    list_t list_;
    map_t map_;
};
