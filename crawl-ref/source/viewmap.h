#ifndef VIEWMAP_H
#define VIEWMAP_H

#include "format.h"

enum feature_list_group
{
    G_UP,
    G_DOWN,
    G_PORTAL,
    G_OTHER,
    G_NONE,
    NUM_GROUPS = G_NONE
};

class feature_list
{
    vector<cglyph_t> data[NUM_GROUPS];
    feature_list_group get_group(const coord_def& gc);
    void maybe_add(const coord_def& gc);

public:
    void init();
    formatted_string format() const;
    vector<formatted_string> formatttt() const;
    virtual ~feature_list();
};

struct level_pos;
bool travel_colour_override(const coord_def& p);
bool is_feature(ucs_t feature, const coord_def& where);
bool show_map(level_pos &spec_place,
              bool travel_mode, bool allow_esc, bool allow_offlevel);

bool emphasise(const coord_def& where);
#endif
