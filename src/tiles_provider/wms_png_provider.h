#pragma once

#include "png_provider.h"

class wms_png_provider : public png_provider
{
public:
    wms_png_provider(const string &host, boost::asio::io_service &io);

public:
    bool request_png(const tile_id_t &id, const png_callback &callback);
    void cache_png(const tile_id_t &id, shared_ptr<const png_t> png);

private:
    void proceed_with_request(const tile_id_t &id, const png_callback &callback);


private:
    class request;

private:
    const string host_;
    boost::asio::io_service *io_;

};

