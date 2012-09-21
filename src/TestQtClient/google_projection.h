#pragma once


class google_projection
{
public:
    typedef double real_t;
    struct zoom_pair_t
    {
        zoom_pair_t()
        {
            c[0] = c[1] = 0;
        }
        zoom_pair_t(const real_t x, const real_t y)
        {
            c[0] = x;
            c[1] = y;
        }
        real_t c[2];
    };
public:
    google_projection(const size_t levels)
    {
        real_t c(256);
        for (size_t i = 0; i != levels; ++i, c *= 2)
        {
            const real_t e = c / 2;
            const zoom_pair_t zoom_pair (e, e);

            Bc.push_back(c / real_t(360));
            Cc.push_back(c / (M_PI * 2));
            zc.push_back(zoom_pair);
            Ac.push_back(c);
        }
    }

/*
    zoom_pair_t fromLLtoPixel(const zoom_pair_tll,zoom):
    d = self.zc[zoom]
    e = round(d[0] + ll[0] * self.Bc[zoom])
        f = minmax(sin(DEG_TO_RAD * ll[1]),-0.9999,0.9999)
        g = round(d[1] + 0.5*log((1+f)/(1-f))*-self.Cc[zoom])
        return (e,g)
*/

    zoom_pair_t fromPixelToLL(const zoom_pair_t &px, const size_t zoom)
    {
        const real_t RAD_TO_DEG = real_t(180) / M_PI;
        const zoom_pair_t e = zc[zoom];
        const real_t f = (px.c[0] - e.c[0]) / Bc[zoom];
        const real_t g = (px.c[1] - e.c[1]) / -Cc[zoom];
        const real_t h = RAD_TO_DEG * (2 * atan(exp(g)) - 0.5 * M_PI);
        
        return zoom_pair_t (f, h);
    }
private:
    vector<real_t> Bc, Cc, Ac;
    vector<zoom_pair_t> zc;
};

