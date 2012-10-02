#pragma once

template<typename K, typename V>
class dumb_cache
{
public:
    typedef K key_type;
    typedef V value_type;

private:
    typedef std::list<value_type> list_t;
    typedef typename list_t::iterator list_it_t;
    
    typedef std::unordered_map<key_type, list_it_t> map_t;
    typedef typename map_t::iterator map_it_t;

public:
    typedef typename list_t::const_iterator const_iterator;
    typedef typename list_t::iterator iterator;
    typedef typename list_t::const_reverse_iterator const_reverse_iterator;
    typedef typename list_t::reverse_iterator reverse_iterator;

public:
    bool contains(const key_type& key) const
    {
        return map_.count(key) != 0;
    }

    void add(const key_type &key, const value_type &value)
    {
        if (contains(key))
        {
            // TODO: dublication
            throw std::runtime_error("dumb_cache key dublication");
        }
        list_.push_front(value);
        map_.insert(map_t::value_type(key, list_.begin()));        
    }

    void erase(const key_type &key)
    {
        const map_it_t map_it = map_.find(key);
        list_.erase(map_it.second);
        map_.erase(map_it);
    }

    value_type &request(const key_type &key)
    {
        list_it_t &it_ref = map_.at(key);
        list_.splice(list_.begin(), list_, it_ref);
        it_ref = list_.begin();
        return *it_ref;
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