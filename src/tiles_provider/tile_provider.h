#pragma once

#include "png_provider.h"
#include "png_cache.h"
#include "png_converter.h"

class tile_provider
{
public:
    tile_provider();
    ~tile_provider();

    shared_ptr<const tile_t> request_tile(const tile_id_t &id);

    //void add_provider(shared_ptr<png_provider> provider, shared_ptr<png_cache> cache);
    void add_provider(shared_ptr<png_provider> provider);

    boost::asio::io_service &io_service() 
    {
        return asio_;
    }

public:
    bool debug_is_set() 
    {
        return (debug_ != NULL);
    }

    std::ostream& debug() 
    {
        return *debug_;
    }

    void set_debug(std::ostream &stream)
    {
        debug_ = &stream;
    }

private:
    void png_ready_callback(const tile_id_t &id, shared_ptr<tile_t> tile, shared_ptr<const png_t> png, shared_ptr<png_provider> source_provider);

private:
    boost::asio::io_service asio_;
    boost::asio::io_service::work work_;
    boost::thread runner_thread_;

    list<shared_ptr<png_provider>> loaders_;
    png_converter converter_;
    
    unordered_map<tile_id_t, shared_ptr<const tile_t>> tiles_in_progress_;
    boost::mutex tiles_in_progress_mutex_;

    std::ostream *debug_;
};

