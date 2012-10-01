#pragma once

#include "png_provider.h"

class wms_png_provider : public png_provider
{
public:
    wms_png_provider(boost::asio::io_service &io, const string &host);

public:
    bool request_png(const tile_id_t &id, const png_callback &callback);

private:
    void proceed_with_request(const tile_id_t &id, const png_callback &callback);

private:
    class request;

private:
    boost::asio::io_service *io_;
    const string host_;

};

