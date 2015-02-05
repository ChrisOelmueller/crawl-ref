/**
 * @file
 * @brief Functions used to print player related info.
**/

#include "AppHdr.h"

#include "output.h"

#include <cmath>
#include <cstdlib>
#include <sstream>

#include "ability.h"
#include "abyss.h" // is_level_incorruptible
#include "branch.h"
#include "colour.h"
#include "describe.h"
#ifndef USE_TILE_LOCAL
#include "directn.h"
#endif
#include "dgn-overview.h"
#include "english.h"
#include "env.h"
#include "exclude.h"
#include "files.h"
#include "godabil.h"
#include "godpassive.h"
#include "initfile.h"
#include "itemname.h"
#include "jobs.h"
#include "lang-fake.h"
#include "libutil.h"
#include "menu.h"
#include "message.h"
#include "misc.h"
#include "mutation.h"
#include "notes.h"
#include "prompt.h"
#include "religion.h"
#include "showsymb.h"
#include "skills.h"
#include "state.h"
#include "status.h"
#include "stringutil.h"
#include "throw.h"
#include "transform.h"
#include "viewchar.h"
#include "viewmap.h" // feature_list
#include "view.h"
#include "xom.h"


#ifdef USE_TILE_LOCAL
#include "tilesdl.h"

/*
 * this glorious piece of code works by:
    - overriding cgotoxy and cprintf
    - mapping the x,y coordinate of each part of the HUD to a
      value in the touchui_states enum and storing the current value
    - using the current state to decide what and where to actually
      render each part of the HUD

  12345678901234567890
1 Nameaname
2 TrDk(Mak)
3  St Dx In
4  nn nn nn
5  AC SH EV
6  nn nn nn
7 W: foobar
8 Q: 20 baz
9 XXXXXXXXX      status lights
.
y  HPP MPP
 */
#include <stdarg.h>
#define CGOTOXY _cgotoxy_touchui
#define CPRINTF _cprintf_touchui
#define NOWRAP_EOL_CPRINTF _nowrap_eol_cprintf_touchui

enum touchui_states
{
    TOUCH_S_INIT  = 0x0,
    TOUCH_S_NULL  = 0x1,
    TOUCH_T_MP    = 0x0104,
    TOUCH_T_AC    = 0x0105,
    TOUCH_T_EV    = 0x0106,
    TOUCH_T_SH    = 0x0107,
    TOUCH_T_STR   = 0x1305,
    TOUCH_T_INT   = 0x1306,
    TOUCH_T_DEX   = 0x1307,
    TOUCH_V_PLACE = 0x1308,
    TOUCH_T_HP    = 0x0103,
    TOUCH_V_HP    = 0x0203, // dummy location
    TOUCH_V_MP    = 0x0904,
    TOUCH_V_AC    = 0x0505,
    TOUCH_V_EV    = 0x0506,
    TOUCH_V_SH    = 0x0507,
    TOUCH_V_STR   = 0x1805,
    TOUCH_V_INT   = 0x1806,
    TOUCH_V_DEX   = 0x1807,
    TOUCH_V_XL    = 0x0108,
    TOUCH_T_WP    = 0x0109,
    TOUCH_T_QV    = 0x010A,
    TOUCH_V_WP    = 0x0209, // dummy
    TOUCH_V_QV    = 0x020A, // dummy
    TOUCH_V_TITLE = 0x0101,
    TOUCH_V_TITL2 = 0x0102,
    TOUCH_V_LIGHT = 0x010B,
};
touchui_states TOUCH_UI_STATE = TOUCH_S_INIT;
static void _cgotoxy_touchui(int x, int y, GotoRegion region = GOTO_CRT)
{
//    printf("go to (%d,%d): ",x,y);
    if (tiles.is_using_small_layout())
        TOUCH_UI_STATE = (touchui_states)((x<<8)+y);
//    printf("[%x]: ",TOUCH_UI_STATE);
    switch (TOUCH_UI_STATE)
    {
        case TOUCH_V_HP:
        case TOUCH_T_MP:
        case TOUCH_V_TITLE:
        case TOUCH_V_TITL2:
        case TOUCH_V_XL:
        case TOUCH_V_PLACE:
        case TOUCH_S_NULL:
            // no special behaviour for these
            break;
        case TOUCH_T_STR:
            x = 1; y = 3;
            break;
        case TOUCH_T_INT:
            x = 4; y = 3;
            break;
        case TOUCH_T_DEX:
            x = 7; y = 3;
            break;
        case TOUCH_T_AC:
            x = 1; y = 5;
            break;
        case TOUCH_T_EV:
            x = 4; y = 5;
            break;
        case TOUCH_T_SH:
            x = 7; y = 5;
            break;
        case TOUCH_V_STR:
            x = 1; y = 4;
            break;
        case TOUCH_V_INT:
            x = 4; y = 4;
            break;
        case TOUCH_V_DEX:
            x = 7; y = 4;
            break;
        case TOUCH_V_AC:
            x = 2; y = 6;
            break;
        case TOUCH_V_EV:
            x = 5; y = 6;
            break;
        case TOUCH_V_SH:
            x = 8; y = 6;
            break;
        case TOUCH_T_WP:
            x = 1; y = 7;
            break;
        case TOUCH_T_QV:
            x = 1; y = 8;
            break;
        case TOUCH_V_WP:
            x = 4; y = 7;
            break;
        case TOUCH_V_QV:
            x = 4; y = 8;
            break;
        case TOUCH_V_LIGHT:
            x = 1; y = 9;
            break;
        case TOUCH_T_HP:
            x = 2; y = crawl_view.hudsz.y;
            break;
        case TOUCH_V_MP:
            x = 6; y = crawl_view.hudsz.y;
            break;
        default:
            // reset state
            TOUCH_UI_STATE = TOUCH_S_INIT;
    }
//    printf("(%d,%d): ",x,y);
    cgotoxy(x,y,region);
}

static void _cprintf_touchui(const char *format, ...)
{
    va_list args;
    string  buf;
    va_start(args, format);
    buf = vmake_stringf(format, args);

    switch (TOUCH_UI_STATE)
    {
        case TOUCH_T_MP:
        case TOUCH_V_TITL2:
        case TOUCH_V_XL:
        case TOUCH_V_PLACE:
        case TOUCH_S_NULL:
            // don't draw these
//            printf("X! %s\n",buf.c_str());
            break;
        case TOUCH_T_HP:
            TOUCH_UI_STATE = TOUCH_V_HP;
            break;
        case TOUCH_V_TITLE:
            cprintf(you.your_name.c_str());
            break;
        case TOUCH_V_HP:
        case TOUCH_V_MP:
            // suppress everything after initial print; rjustify
            cprintf("%3s", buf.c_str());
            TOUCH_UI_STATE = TOUCH_S_NULL;
            break;
        case TOUCH_V_STR:
        case TOUCH_V_INT:
        case TOUCH_V_DEX:
            // rjustify to 3 chars on these
            cprintf("%3s", buf.c_str());
            break;
        case TOUCH_T_WP:
            TOUCH_UI_STATE = TOUCH_V_WP;
            cprintf(buf.c_str());
            break;
        case TOUCH_T_QV:
            TOUCH_UI_STATE = TOUCH_V_QV;
            cprintf(buf.c_str());
            break;
        case TOUCH_V_WP:
        case TOUCH_V_QV:
            // get rid of the hotkey; somewhat pointless in a touch-screen ui :)
            cprintf(buf.substr(3,10).c_str());
            break;

        default:
//            printf("p: %s\n",buf.c_str());
            cprintf(buf.c_str());
    }
    va_end(args);
}

static void _nowrap_eol_cprintf_touchui(const char *format, ...)
{
    va_list args;
    string  buf;
    va_start(args, format);
    buf = vmake_stringf(format, args);

    // N.B. this should really be factored out and merged with the other switch-case above
    switch (TOUCH_UI_STATE)
    {
        case TOUCH_S_NULL:
            // don't print these
            break;
        case TOUCH_V_TITL2:
            cprintf("%s%s %.4s", get_species_abbrev(you.species),
                                 get_job_abbrev(you.char_class),
                                 god_name(you.religion).c_str());
            TOUCH_UI_STATE = TOUCH_S_NULL; // suppress whatever else it was going to print
            break;
        default:
//            printf("q: %s\n",buf.c_str());
            nowrap_eol_cprintf("%s", buf.c_str());
    }
    va_end(args);
}

#else
#define CGOTOXY cgotoxy
#define CPRINTF cprintf
#define NOWRAP_EOL_CPRINTF nowrap_eol_cprintf
#endif

static string _god_powers();
static string _god_asterisks();
static int _god_status_colour(int default_colour);

// Colour for captions like 'Health:', 'Str:', etc.
#define HUD_CAPTION_COLOUR Options.status_caption_colour
#define HUD_COMPACT Options.compact_hud
#define HUD_CRAMPED (Options.compact_hud && Options.compact_hud_xs)

// Column layout constants
#define HUD_WIDTH crawl_view.hudsz.x
#define FIRST_COL  1
#define BAR_START (13 + (2*HUD_CRAMPED))

#define CLASSIC_A FIRST_COL
#define CLASSIC_B 19
#define CLASSIC_C 30 // Hack: only used for ZotDef

#define COMPACT_A FIRST_COL
#define COMPACT_B  9
#define COMPACT_C 19
#define COMPACT_D 29

#define CRAMPED_A FIRST_COL
#define CRAMPED_B  8
#define CRAMPED_C 15 // 15 would be better
#define CRAMPED_D 25 // 24?  ??? what to do with remaining space ???
// 26 + '-----------------' 3 space, 24 would be 5
//      'Orcish Mines:2|||'
#define CRAMPED_E 35 // 24?  ??? what to do with remaining space ???

// Some hackish stuff to move around HUD for obsolete content
#if TAG_MAJOR_VERSION == 34
#define temp (you.species == SP_LAVA_ORC)
#else
// Todo: rewrite #defines below to no longer contain temp TAG_MAJOR_VERSION > 34
// Fairly sure spamming ifdef-protected alternate codepaths would not help
// readability much more. Removing temp when possible should be rather quick.
#define temp 0
#endif

#define name_pos_x (CLASSIC_A)
#define name_pos_y 1

#define god_pos_x (CLASSIC_A)
#define god_pos_y 2

// Real HUD layout data
#define hp_pos_x (CLASSIC_A)
#define hp_pos_y (HUD_CRAMPED ? 2 : 3)
#define hp_bar_y (HUD_CRAMPED ? 2 : 3)

#define mp_pos_x (HUD_CRAMPED ? CRAMPED_B + 1 : CLASSIC_A)
#define mp_pos_y (HUD_CRAMPED ? 2 : 4)
#define mp_bar_y (HUD_CRAMPED ? 3 : 4)

#if TAG_MAJOR_VERSION == 34
#define contam_pos_x mp_pos_x
#define contam_pos_y mp_pos_y
#define contam_bar_y mp_bar_y

#define temperature_pos_x (CLASSIC_A)
#define temperature_pos_y (HUD_CRAMPED ? 3 : 5)
#define temperature_bar_y (HUD_CRAMPED ? 4 : 5)
#endif

#define ac_pos_x (CLASSIC_A)
#define ac_pos_y ((HUD_CRAMPED ? 3 : 5) + temp)

#define ev_pos_x (CLASSIC_A)
#define ev_pos_y (ac_pos_y + 1)

#define sh_pos_x (CLASSIC_A)
#define sh_pos_y (ev_pos_y + 1)


#define stat_pos_x (HUD_CRAMPED ? CRAMPED_B : HUD_COMPACT ? COMPACT_B : CLASSIC_B)
#define str_pos_x (stat_pos_x)
#define int_pos_x (stat_pos_x)
#define dex_pos_x (stat_pos_x)

#define str_pos_y ((HUD_CRAMPED ? 3 : 5) + temp)
#define int_pos_y ((HUD_CRAMPED ? 4 : 6) + temp)
#define dex_pos_y ((HUD_CRAMPED ? 5 : 7) + temp)

#define place_pos_x (HUD_CRAMPED ? CRAMPED_D : COMPACT_D)
#define place_pos_y ((HUD_CRAMPED ? 4 : 6) + temp)

#define xl_pos_x (HUD_CRAMPED ? CRAMPED_C : HUD_COMPACT ? COMPACT_C : CLASSIC_A)
#define xl_pos_y ((HUD_CRAMPED ? 4 : HUD_COMPACT ? 6 : 8) + temp)

#define turn_pos_x (HUD_CRAMPED ? CRAMPED_D : HUD_COMPACT ? COMPACT_D : CLASSIC_B)
#define turn_pos_y ((HUD_CRAMPED ? 5 : HUD_COMPACT ? 5 : 9) + temp)
#define last_pos_x (HUD_CRAMPED ? CRAMPED_C : HUD_COMPACT ? COMPACT_C : CLASSIC_C)
#define last_pos_y (turn_pos_y)

#define piety_pos_x (HUD_CRAMPED ? CRAMPED_E : COMPACT_C)
#define piety_pos_y (HUD_CRAMPED ? xl_pos_y : place_pos_y)

#define defenses_pos_x (HUD_CRAMPED ? CRAMPED_D : COMPACT_C)
#define defenses_pos_y ((HUD_CRAMPED ? 5 : 7) + temp)

#define hunger_pos_x (HUD_CRAMPED ? CRAMPED_E : COMPACT_C)
#define hunger_pos_y (HUD_CRAMPED ? 5 : defenses_pos_y)

#define realtime_pos_x hunger_pos_x
#define realtime_pos_y hunger_pos_y

#define sid_pos_x realtime_pos_x
#define sid_pos_y realtime_pos_y

#define resists_pos_x sid_pos_x
#define resists_pos_y sid_pos_y

#define stairs_pos_x resists_pos_x
#define stairs_pos_y resists_pos_y

#define fcrawl_pos_x resists_pos_x
#define fcrawl_pos_y resists_pos_y

#define gold_pos_x (HUD_COMPACT ? COMPACT_D : CLASSIC_A)
#define gold_pos_y ((HUD_COMPACT ? 7 : 9) + temp)

#define zp_pos_x (HUD_COMPACT ? COMPACT_D : CLASSIC_C)
#define zp_pos_y ((HUD_COMPACT ? 7 :  8) + temp)

#define wp_pos_x (CLASSIC_A)
#define wp_pos_y ((HUD_CRAMPED ? 6 : HUD_COMPACT ? 8 : 10) + temp)
#define qv_pos_x (HUD_CRAMPED ? CRAMPED_D : CLASSIC_A)
#define qv_pos_y (wp_pos_y + (you.species != SP_FELID) - HUD_CRAMPED)

#define skills_pos_x (CLASSIC_A)
#define skills_pos_y (qv_pos_y + 1)

#define status_pos_x (CLASSIC_A)
#define status_pos_y (skills_pos_y + Options.show_skill_bar)

// Colour for values, which come after captions.
static const short HUD_VALUE_COLOUR = LIGHTGREY;

// ----------------------------------------------------------------------
// colour_bar
// ----------------------------------------------------------------------

class colour_bar
{
    typedef unsigned short colour_t;
 public:
    colour_t m_default;
    colour_t m_change_pos;
    colour_t m_change_neg;
    colour_t m_empty;

    colour_bar(colour_t default_colour,
               colour_t change_pos,
               colour_t change_neg,
               colour_t empty,
               bool round = false)
        : m_default(default_colour), m_change_pos(change_pos),
          m_change_neg(change_neg), m_empty(empty),
          m_old_disp(-1),
          m_request_redraw_after(0)
    {
        // m_old_disp < 0 means it's invalid and needs to be initialised.
    }

    bool wants_redraw() const
    {
        return m_request_redraw_after
               && you.num_turns >= m_request_redraw_after;
    }

#if TAG_MAJOR_VERSION == 34
    void draw(int ox, int oy, int val, int max_val, bool is_temp = false, int sub_val = 0)
#else
    void draw(int ox, int oy, int val, int max_val, int sub_val = 0)
#endif
    {
        ASSERT(val <= max_val);
        if (max_val <= 0)
        {
            m_old_disp = -1;
            return;
        }
#if TAG_MAJOR_VERSION == 34
        const colour_t temp_colour = temperature_colour(temperature());
#endif
        const int width = HUD_WIDTH - (ox - 1);
        const int sub_disp = (width * val / max_val);
        int disp  = width * max(0, val - sub_val) / max_val;
        const int old_disp = (m_old_disp < 0) ? sub_disp : m_old_disp;
        m_old_disp = sub_disp;

        // Always show at least one sliver of the sub-bar, if it exists
        if (sub_val)
            disp = max(0, min(sub_disp - 1, disp));

        CGOTOXY(ox, oy, GOTO_STAT);

        textcolour(BLACK);
        for (int cx = 0; cx < width; cx++)
        {
#ifdef USE_TILE_LOCAL
            // Maybe this should use textbackground too?
            textcolour(BLACK + m_empty * 16);

            if (cx < disp)
            {
#if TAG_MAJOR_VERSION == 34
                textcolour(BLACK + (temp) ? temp_colour * 16 : m_default * 16);
#else
                textcolour(BLACK + m_default * 16);
#endif
            }
            else if (cx < sub_disp)
                textcolour(BLACK + YELLOW * 16);
            else if (old_disp >= sub_disp && cx < old_disp)
                textcolour(BLACK + m_change_neg * 16);
            putwch(' ');
#else
            if (cx < disp && cx < old_disp)
            {
#if TAG_MAJOR_VERSION == 34
                textcolour(temp ? temp_colour : m_default);
#else
                textcolour(m_default);
#endif
                putwch('=');
            }
            else if (cx < disp)
            {
                textcolour(m_change_pos);
                putwch('=');
            }
            else if (cx < sub_disp)
            {
                textcolour(YELLOW);
                putwch('=');
            }
            else if (cx < old_disp)
            {
                textcolour(m_change_neg);
                putwch('-');
            }
            else
            {
                textcolour(m_empty);
                putwch('-');
            }
#endif

            // If some change colour was rendered, redraw in a few
            // turns to clear it out.
            if (old_disp != disp)
                m_request_redraw_after = you.num_turns + 4;
            else
                m_request_redraw_after = 0;
        }

        textcolour(LIGHTGREY);
        textbackground(BLACK);
    }

    void vdraw(int ox, int oy, int val, int max_val)
    {
        // ox is width from l/h edge; oy is height from top
        // bars are 3chars wide and render down to hudsz.y-1
        const int bar_width = 3;
        const int obase     = crawl_view.hudsz.y-1;

        ASSERT(val <= max_val);
        if (max_val <= 0)
        {
            m_old_disp = -1;
            return;
        }

        const int height   = bar_width * (obase-oy+1);
        const int disp     = height * val / max_val;
        const int old_disp = (m_old_disp < 0) ? disp : m_old_disp;

        CGOTOXY(ox, obase, GOTO_STAT);

        textcolour(WHITE);
        for (int cx = 0; cx < height; cx++)
        {
            // Maybe this should use textbackground too?
            textcolour(BLACK + m_empty * 16);

            if (cx < disp)
                textcolour(BLACK + m_default * 16);
            else if (old_disp > disp && cx < old_disp)
                textcolour(BLACK + m_change_neg * 16);
            putwch(' ');

            // move up a line if we've drawn this bit of the bar
            if ((cx+1) % bar_width == 0)
                CGOTOXY(ox, obase-cx/bar_width, GOTO_STAT);

            // If some change colour was rendered, redraw in a few
            // turns to clear it out.
            if (old_disp != disp)
                m_request_redraw_after = you.num_turns + 4;
            else
                m_request_redraw_after = 0;
        }

        textcolour(LIGHTGREY);
        textbackground(BLACK);
    }

 private:
    int m_old_disp;
    int m_request_redraw_after; // force a redraw at this turn count
};

static colour_bar HP_Bar(LIGHTGREEN, GREEN, LIGHTRED, DARKGREY);
static colour_bar EP_Bar(LIGHTMAGENTA, MAGENTA, BLUE, DARKGREY);

#ifdef USE_TILE_LOCAL
static colour_bar MP_Bar(BLUE, BLUE, LIGHTBLUE, DARKGREY);
#else
static colour_bar MP_Bar(LIGHTBLUE, BLUE, LIGHTMAGENTA, DARKGREY);
#endif

colour_bar Contam_Bar(DARKGREY, DARKGREY, DARKGREY, DARKGREY);
colour_bar Temp_Bar(RED, LIGHTRED, LIGHTBLUE, DARKGREY);

// ----------------------------------------------------------------------
// Status display
// ----------------------------------------------------------------------

static bool _boosted_hp()
{
    return you.duration[DUR_DIVINE_VIGOUR]
           || you.berserk();
}

static bool _boosted_mp()
{
    return you.duration[DUR_DIVINE_VIGOUR];
}

static bool _boosted_ac()
{
    return you.duration[DUR_ICY_ARMOUR]
           || player_stoneskin()
           || player_icemail_armour_class()
           || you.duration[DUR_QAZLAL_AC]
           || you.attribute[ATTR_BONE_ARMOUR] > 0;
}

static bool _boosted_ev()
{
    return you.duration[DUR_PHASE_SHIFT]
           || you.duration[DUR_AGILITY];
}

static bool _boosted_sh()
{
    return you.duration[DUR_CONDENSATION_SHIELD]
           || you.duration[DUR_MAGIC_SHIELD]
           || you.duration[DUR_DIVINE_SHIELD]
           || qazlal_sh_boost() > 0
           || you.attribute[ATTR_BONE_ARMOUR] > 0;
}

#ifdef DGL_SIMPLE_MESSAGING
void update_message_status()
{
    static const char *msg = "(Hit _)";
    static const int len = strwidth(msg);
    static const string spc(len, ' ');

    textcolour(LIGHTBLUE);

    const int dgl_pos_x = FIRST_COL + HUD_WIDTH - len;
    const int dgl_pos_y = 1;
    CGOTOXY(dgl_pos_x, dgl_pos_y, GOTO_STAT);
    if (SysEnv.have_messages)
        CPRINTF(msg);
    else
        you.redraw_title = true;
        //CPRINTF(spc.c_str());
    textcolour(LIGHTGREY);
}
#endif

void update_turn_count()
{
    if (crawl_state.game_is_arena())
        return;

    // Don't update turn counter when running/resting/traveling to
    // prevent pointless screen updates.
    if (you.running > 0
        || you.running < 0 && Options.travel_delay == -1)
    {
        return;
    }
    //                    =========
    //                    100083===
    //     this should be 999999999 but isn't .. oh Turns/Turn yes
    // if (you.num_turns > 99999999)

    textcolour(HUD_CAPTION_COLOUR);
    CGOTOXY(turn_pos_x, turn_pos_y, GOTO_STAT);
    if (HUD_COMPACT)
    {
        CGOTOXY(last_pos_x, last_pos_y, GOTO_STAT);
        //CPRINTF("LastTurn");
        //CGOTOXY(last_pos_x, last_pos_y+1, GOTO_STAT);
        // ⌛ // Basil
        // "@ "
        // ""
        //CPRINTF("⌛ "); // CPRINTF("dt"); // mauris // ⌛ // Basil // @
        CPRINTF("Δt"); // CPRINTF("dt"); // mauris // ⌛ // Basil // @
        textcolour(HUD_VALUE_COLOUR);
        string last_turn = make_stringf("%.1f",
            (you.elapsed_time - you.elapsed_time_at_last_input) / 10.0);
        CPRINTF("%5s", last_turn.c_str());

        textcolour(HUD_CAPTION_COLOUR);
        CGOTOXY(turn_pos_x, turn_pos_y, GOTO_STAT);
        //CPRINTF(HUD_CRAMPED ? "Turns" : "Turns");
        CPRINTF(HUD_CRAMPED ? "T" : "Turns"); // "⌚" clock
        textcolour(HUD_VALUE_COLOUR);
        CPRINTF(" %d", you.num_turns);
    }
    else if (Options.show_game_turns)
    {
        CPRINTF("Time: ");
        textcolour(HUD_VALUE_COLOUR);
        CPRINTF("%.1f (%.1f)", you.elapsed_time / 10.0,
                (you.elapsed_time - you.elapsed_time_at_last_input) / 10.0);
        clear_to_end_of_line();
    }
    else
    {
        CPRINTF("Turn: ");
        textcolour(HUD_VALUE_COLOUR);
        CPRINTF("%d", you.num_turns);
    }
}

#if TAG_MAJOR_VERSION == 34
static void _print_temperature(const int x = temperature_pos_x,
                               const int y = temperature_pos_y,
                               const int bar_y = temperature_bar_y)
{
    you.redraw_temperature = false;
    cgotoxy(x, y, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);
    cprintf("Temperature: ");

    Temp_Bar.draw(BAR_START, bar_y, temperature(), TEMP_MAX, true);
}
#endif

static void _print_stats_mp(const int x = mp_pos_x, const int y = mp_pos_y,
                            const int bar_y = mp_bar_y)
{
    you.redraw_magic_points = false;
#if TAG_MAJOR_VERSION == 34
    if (you.species == SP_DJINNI)
        return;

#endif
    // Calculate colour
    short mp_colour = HUD_VALUE_COLOUR;

    const bool boosted = _boosted_mp();

    if (boosted)
        mp_colour = LIGHTBLUE;
    else
    {
        int mp_percent = (you.max_magic_points == 0
                          ? 100
                          : (you.magic_points * 100) / you.max_magic_points);

        for (const auto &entry : Options.mp_colour)
            if (mp_percent <= entry.first)
                mp_colour = entry.second;
    }

    bool three_digits = (you.hp_max > 99 || you.max_magic_points > 99);
    // if HP: xx/yy   =====  if HP: xxx/yyy =====
    //  | MP: xx/yy   =====   | MP:  xx/yy  =====
    CGOTOXY(x, y, GOTO_STAT);
    if (HUD_CRAMPED)
        three_digits = you.max_magic_points < 10;
        //three_digits = false;
    else
    {
        textcolour(HUD_CAPTION_COLOUR);
        CPRINTF(HUD_COMPACT ? "MP " : "MP: ");
    }
    textcolour(mp_colour);
    CPRINTF(three_digits ? "%3d" : "%2d", you.magic_points);
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF("/");
    if (boosted)
        textcolour(LIGHTBLUE);
    //CPRINTF("%-3d", you.max_magic_points);
    CPRINTF("%d", you.max_magic_points);
    textcolour(HUD_VALUE_COLOUR);

#ifdef TOUCH_UI
    if (tiles.is_using_small_layout())
        MP_Bar.vdraw(6, 10, you.magic_points, you.max_magic_points);
    else
#endif
#if TAG_MAJOR_VERSION == 34
    const int temp_shift = (you.species == SP_LAVA_ORC) ? 1 : 0;
    MP_Bar.draw(BAR_START, bar_y + temp_shift, you.magic_points, you.max_magic_points);
#else
    MP_Bar.draw(BAR_START, bar_y, you.magic_points, you.max_magic_points);
#endif
}

#if TAG_MAJOR_VERSION == 34
static void _print_stats_contam(const int x = contam_pos_x,
                                const int y = contam_pos_y,
                                const int bar_y = contam_bar_y)
{
    if (you.species != SP_DJINNI)
        return;

    const int max_contam = 8000;
    int contam = min(you.magic_contamination, max_contam);

    // Calculate colour
    if (you.magic_contamination > 15000)
    {
        Contam_Bar.m_default = RED;
        Contam_Bar.m_change_pos = Contam_Bar.m_change_neg = RED;
    }
    else if (you.magic_contamination > 5000) // harmful
    {
        Contam_Bar.m_default = LIGHTRED;
        Contam_Bar.m_change_pos = Contam_Bar.m_change_neg = RED;
    }
    else if (you.magic_contamination > 3333)
    {
        Contam_Bar.m_default = YELLOW;
        Contam_Bar.m_change_pos = Contam_Bar.m_change_neg = BROWN;
    }
    else if (you.magic_contamination > 1666)
    {
        Contam_Bar.m_default = LIGHTGREY;
        Contam_Bar.m_change_pos = Contam_Bar.m_change_neg = DARKGREY;
    }
    else
    {
#ifdef USE_TILE_LOCAL
        Contam_Bar.m_default = LIGHTGREY;
#else
        Contam_Bar.m_default = DARKGREY;
#endif
        Contam_Bar.m_change_pos = Contam_Bar.m_change_neg = DARKGREY;
    }
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(Contam_Bar.m_default);
    CPRINTF(HUD_CRAMPED ? "Contm" : HUD_COMPACT ? "Contam" : "Contam: ");

#ifdef TOUCH_UI
    if (tiles.is_using_small_layout())
        Contam_Bar.vdraw(6, 10, contam, max_contam);
    else
#endif
    Contam_Bar.draw(BAR_START, bar_y, contam, max_contam);
}
#endif
static void _print_stats_hp(const int x = hp_pos_x, const int y = hp_pos_y,
                            const int bar_y = hp_bar_y)
{
    you.redraw_hit_points = false;
    int max_max_hp = get_real_hp(true, true);
#if TAG_MAJOR_VERSION == 34
    if (you.species == SP_DJINNI)
        max_max_hp += get_real_mp(true);
#endif

    // Calculate colour
    short hp_colour = HUD_VALUE_COLOUR;
    const bool boosted = _boosted_hp();

    if (boosted)
        hp_colour = LIGHTBLUE;
    else
    {
        const int hp_percent =
            (you.hp * 100) / get_real_hp(true, false);

        for (const auto &entry : Options.hp_colour)
            if (hp_percent <= entry.first)
                hp_colour = entry.second;
    }

    bool three_digits = (you.hp_max > 99 || you.max_magic_points > 99);
    // HP: xx/yy   =====  |  HP: xxx/yyy =====  |  HP:xxxx/yyyy=====
    CGOTOXY(x, y, GOTO_STAT);
    if (Options.compact_hud_xs)
        three_digits = false;
    else
    {
        textcolour(HUD_CAPTION_COLOUR);
#if TAG_MAJOR_VERSION == 34
        if (you.species == SP_DJINNI)
            CPRINTF(HUD_COMPACT ? "EP" : "EP:");
        else
#endif
        CPRINTF(HUD_COMPACT ? "HP" : "HP:");
    }
    textcolour(hp_colour);
    CPRINTF(three_digits ? "%4d" : (HUD_CRAMPED && you.hp_max < 100) ? "%2d" : "%3d", you.hp);
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF("/");
    // Indicate rot by coloring max hp (% screen shows exact rot amount)
    if (boosted)
        textcolour(LIGHTBLUE);
    else if (max_max_hp != you.hp_max)
        textcolour(YELLOW);
    CPRINTF("%-4d", you.hp_max);
    textcolour(HUD_VALUE_COLOUR);

#ifdef USE_TILE_LOCAL
    if (tiles.is_using_small_layout())
    {
#if TAG_MAJOR_VERSION == 34
        if (you.species == SP_DJINNI)
            EP_Bar.vdraw(2, 10, you.hp, you.hp_max);
        else
#endif
        HP_Bar.vdraw(2, 10, you.hp, you.hp_max);
    }
    else
#endif
#if TAG_MAJOR_VERSION == 34
    if (you.species == SP_DJINNI)
        EP_Bar.draw(BAR_START, bar_y, you.hp, you.hp_max);
    else
        HP_Bar.draw(BAR_START, bar_y, you.hp, you.hp_max, false, you.hp - max(0, poison_survival()));
#else
        HP_Bar.draw(BAR_START, bar_y, you.hp, you.hp_max, you.hp - max(0, poison_survival()));
#endif
}

static short _get_stat_colour(stat_type stat, bool yellow_rotted = false)
{
    if (you.stat_zero[stat])
        return LIGHTRED;

    // Check the stat_colour option for warning thresholds.
    for (const auto &entry : Options.stat_colour)
        if (you.stat(stat) <= entry.first)
            return entry.second;

    // Stat is magically increased.
    if (you.duration[DUR_DIVINE_STAMINA]
        || stat == STAT_STR && you.duration[DUR_MIGHT]
        || stat == STAT_STR && you.duration[DUR_BERSERK]
        || stat == STAT_STR && you.duration[DUR_FORTITUDE]
        || stat == STAT_INT && you.duration[DUR_BRILLIANCE]
        || stat == STAT_DEX && you.duration[DUR_AGILITY])
    {
        return LIGHTBLUE;  // no end of effect warning
    }

    // Stat is degenerated and we are explicitly asked to show here
    if (yellow_rotted && you.stat_loss[stat] > 0)
        return YELLOW;

    return HUD_VALUE_COLOUR;
}

static void _print_stat(stat_type stat, const char* statname,
                        const int x, const int y)
{
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF(statname);
    // Traditional "Str:  8" | Compact "Str  8"
    if (!HUD_COMPACT)
        CPRINTF(":");
    textcolour(_get_stat_colour(stat, HUD_CRAMPED));
    // Compact "Str  8", "Int 21", "Dex104" | Traditional same with colons
    CPRINTF("%3d", you.stat(stat, false));

    // Drained stats
    // Traditional: "Int: 20 (21) "
    // Compact:     "Int 20/21 "

    // TODO RHS should include: base stat, mutations, rot.
    // should not include: temp boosts.
    // so might + 1 rot shows "Str 15/11" with `15` blue, `/11` yellow
    textcolour(YELLOW);
    if (!HUD_CRAMPED && you.stat_loss[stat] > 0)
        CPRINTF(HUD_COMPACT ? "/%-3d" : " (%d) ", you.max_stat(stat));
}

static void _print_str(const int x = str_pos_x, const int y = str_pos_y)
{
    _print_stat(STAT_STR, "Str", x, y);
}

static void _print_int(const int x = int_pos_x, const int y = int_pos_y)
{
    _print_stat(STAT_INT, "Int", x, y);
}

static void _print_dex(const int x = dex_pos_x, const int y = dex_pos_y)
{
    _print_stat(STAT_DEX, "Dex", x, y);
}

static void _print_sid(const int x = sid_pos_x, const int y = sid_pos_y)
{
    const colour_t str_colour = _get_stat_colour(STAT_STR, true);
    const colour_t int_colour = _get_stat_colour(STAT_INT, true);
    const colour_t dex_colour = _get_stat_colour(STAT_DEX, true);
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(str_colour);
    CPRINTF("%2d", you.stat(STAT_STR, false));
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF(",");
    textcolour(int_colour);
    CPRINTF("%2d", you.stat(STAT_INT, false));
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF(",");
    textcolour(dex_colour);
    CPRINTF("%2d", you.stat(STAT_DEX, false));
}
static const colour_t _ac_colour()
{
    if (_boosted_ac())
        return LIGHTBLUE;
    else if (you.duration[DUR_CORROSION])
        return RED;
    else
        return HUD_VALUE_COLOUR;
}

static const colour_t _ev_colour()
{
    const bool incapacitated = you.duration[DUR_PETRIFYING]
                               || you.duration[DUR_GRASPING_ROOTS]
                               || you.cannot_move();
    if (incapacitated)
        return RED;
    else if (_boosted_ev())
        return LIGHTBLUE;
    else
        return HUD_VALUE_COLOUR;
}

static const colour_t _sh_colour()
{
    if (you.incapacitated() && you.shielded())
        return RED;
    else if (_boosted_sh())
        return LIGHTBLUE;
    else
        return HUD_VALUE_COLOUR;
}

static void _print_stats_ac(const int x = ac_pos_x, const int y = ac_pos_y)
{
    you.redraw_armour_class = false;
    colour_t ac_colour = _ac_colour();
    // Display AC (red if corroded, lightblue if enhanced)
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF(HUD_COMPACT ? "AC" : "AC:");
    textcolour(ac_colour);
    CPRINTF("%3d", you.armour_class());

    // Display GDR percentage
    if (!HUD_COMPACT)
    {
#ifdef WIZARD
        if (you.wizard)
            CPRINTF("(%d%%)", you.gdr_perc());
        else
            CPRINTF("     ");
#endif
        CPRINTF("     ");
    }
}

static void _print_stats_ev(const int x = ev_pos_x, const int y = ev_pos_y)
{
    you.redraw_evasion = false;
    const colour_t ev_colour = _ev_colour();
    // Display EV (red if incapacitated, lightblue if enhanced)
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF(HUD_COMPACT ? "EV" : "EV:");
    textcolour(ev_colour);
    CPRINTF("%3d", player_evasion());
}

static void _print_stats_sh(const int x = sh_pos_x, const int y = sh_pos_y)
{
    you.redraw_shield_class = false;
    const colour_t sh_colour = _sh_colour();
    // Display SH (red if incapacitated, lightblue if enhanced)
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF(HUD_COMPACT ? "SH" : "SH:");
    textcolour(sh_colour);
    CPRINTF("%3d", player_displayed_shield_class());
}

static void _print_fcrawl(const int x = fcrawl_pos_x, const int y = fcrawl_pos_y)
{
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF("~fcrawl~");
}

static void _resists_helper(const colour_t char_colour, const int resist)
{
    textcolour(char_colour);
    if (resist < 0)
        CPRINTF("x"); // or - ?
    else if (resist == 0)
        CPRINTF(".");
    else if (char_colour == MAGENTA && resist < 10) // MR
        CPRINTF("%s", to_string(resist).c_str());
    else if (resist == 1)
        CPRINTF("+");
    else if (resist == 2)
        CPRINTF("*");
    else if (resist == 3 && char_colour == LIGHTGREEN) // rPois
        CPRINTF("!"); // I'd rather die before printing ∞
    else if (char_colour == MAGENTA && you.form == TRAN_SHADOW) // MR immune
        CPRINTF("!");
    else if (resist == 3)
        CPRINTF("!");
    else // TODO
        CPRINTF("?");
}

static void _print_stairs(const int x = stairs_pos_x, const int y = stairs_pos_y)
{
    CGOTOXY(x, y, GOTO_STAT);
    ////Three upstairs
    //textcolour(GREEN);
    //CPRINTF("<");
    //textcolour(WHITE);
    //CPRINTF("<");
    //textcolour(DARKGREY);
    //CPRINTF("<");

    feature_list features;
    features.init();
    //feature_list displayed_features;
    //for (const auto &f : features)
    //{
    //}
    formatted_string fs = features.format();
    fs = fs.chop(8);
    /////TODO
    fs = fs.chop(3);
    fs.display();

    // Desired order
    // Three downstairs (grey if not found, white unvisited, green)
    // Two of the following, in order by preference:
    // - Timed portals
    // - Single-use portals
    // - Branch entry stairs
    // - Multi-use portals
    // - Uphatch
    // - Downhatch
    // - Shaft?
    if (coinflip())
    {
        textcolour(BLUE);
        CPRINTF(" ");
    }
    else
    {
        textcolour(LIGHTGREEN);
        CPRINTF(" ");
    }

    const bool has_br_entry = coinflip();
    if (has_br_entry)
    {
        textcolour(YELLOW);
        CPRINTF(" ");
    }
    else
    {
        textcolour(BROWN);
        CPRINTF(" ");
    }
    // Is there a travel-excluded area on the level?
    // Display its center if so.
    textcolour(RED);
    string exclusion = " ";
    for (const auto &exc : curr_excludes)
    {
        exclusion = get_cell_glyph(exc.first).ch;
        break;
    }
    // Make floor tiles with no monster or item on them more visible
    if (exclusion == ".")
        textcolour(LIGHTRED);

    CPRINTF(exclusion.c_str());

    //CPRINTF(coinflip() ? "." : "E");

    // Is the level annotated, and is the annotation scary?
    // Skip [runed doors] exclusions(?) uniques(?) 
    //string annotation = get_level_annotation(level_id::current(), true);
    string annotation = get_level_annotation();
    if (annotation == "")
        annotation = " ";
    else if (level_annotation_has("runed door"))
    {
        textcolour(LIGHTBLUE);
        annotation = "+";
    }
    else
    {
        textcolour(LIGHTGREY);
        annotation = "!";
    }
    if (level_annotation_has("!"))
        textcolour(LIGHTRED);

    CPRINTF(annotation.c_str());

    // TODO HACK
    // Are runed doors present on the level?
    //textcolour(LIGHTBLUE);
    //CPRINTF(level_annotation_has("runed door") ? "+" : " ");

    // Is the level corruptible?
    // Only display indicator when it matters.
    textcolour(MAGENTA);
    if (you_worship(GOD_LUGONU) || one_chance_in(10))
    {
        if (is_level_incorruptible(true))
            textcolour(DARKGREY);
        CPRINTF("C");
    }
    else
        CPRINTF(" ");
}

static void _print_resists(const int x = resists_pos_x, const int y = resists_pos_y)
{
    const bool calc_unid = false;
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);

    _resists_helper(LIGHTRED, player_res_fire(calc_unid));
    _resists_helper(LIGHTBLUE, player_res_cold(calc_unid));
    _resists_helper(DARKGREY, player_prot_life(calc_unid));
    _resists_helper(LIGHTGREEN, player_res_poison(calc_unid));
    _resists_helper(WHITE, player_res_electricity(calc_unid));
    _resists_helper(YELLOW, you.res_corr(calc_unid));
    _resists_helper(LIGHTMAGENTA,
        (you.rmut_from_item(calc_unid)
         || player_mutation_level(MUT_MUTATION_RESISTANCE) == 3));
    _resists_helper(MAGENTA, player_res_magic(calc_unid) / MR_PIP);
}


static void _print_defenses(const int x = defenses_pos_x,
                            const int y = defenses_pos_y)
{
    const colour_t ac_colour = _ac_colour();
    const colour_t ev_colour = _ev_colour();
    const colour_t sh_colour = _sh_colour();
    const int sh = player_displayed_shield_class();
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(ac_colour);
    CPRINTF("%02d", you.armour_class());
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF("/");
    textcolour(ev_colour);
    CPRINTF("%02d", player_evasion());
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF("/");
    textcolour(sh_colour);
    if (sh)
        CPRINTF("%02d", player_displayed_shield_class());
    else
        CPRINTF("--");
}

static void _print_piety(const int x = piety_pos_x, const int y = piety_pos_y)
{

    if (you_worship(GOD_NO_GOD))
    {
        _print_fcrawl(piety_pos_x, piety_pos_y);
        return;
    }
    CGOTOXY(x, y, GOTO_STAT);

    const char godkey =
        you_worship(GOD_SHINING_ONE) ? '1' : god_name(you.religion).at(0);

    textcolour(_god_status_colour(god_colour(you.religion)));

    CPRINTF("%c %s", godkey, _god_asterisks().c_str());
}

static void _print_rot(const int x, const int y)
{
    const int max_max_hp = get_real_hp(true, true);
    // TODO real ghoul hunger meter + rot count
    if (max_max_hp == you.hp_max)
        return;
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF("Rot");
    textcolour(YELLOW);
    CPRINTF("%4d  ", max_max_hp);
}

static void _print_hunger(const int x = hunger_pos_x, const int y = hunger_pos_y)
{
    // Mummy / lichform
    if (you.undead_state() == US_UNDEAD)
        return;
#if TAG_MAJOR_VERSION == 34
    // Let's not speak about these.
    if (you.species == SP_DJINNI)
        return;
#endif

    if (you.undead_state() == US_HUNGRY_DEAD)
    {
        _print_rot(x, y);
        return;
    }

    CGOTOXY(x, y, GOTO_STAT);

    map<hunger_state_t, pair<colour_t, colour_t> > hunger_bar = {
      {HS_STARVING,      {RED,        DARKGREY,   }},
    //{HS_NEAR_STARVING, {RED,        DARKGREY,   }}, // YELLOW actually
      {HS_VERY_HUNGRY,   {YELLOW,     LIGHTGREY,  }},
      {HS_HUNGRY,        {YELLOW,     LIGHTGREY,  }},
      {HS_SATIATED,      {LIGHTGREY,  LIGHTGREY,  }}, // has two bars
      {HS_FULL,          {GREEN,      GREEN,      }},
      {HS_VERY_FULL,     {GREEN,      GREEN,      }},
      {HS_ENGORGED,      {LIGHTGREEN, LIGHTGREEN, }},
    };// // TODO option to hide status lights below a threshold state

    // TODO check if Vp special coloring actually helps
    //const bool vp = you.undead_state() == US_SEMI_UNDEAD;
    const bool vp = false;
    colour_t rightmost = RED;
    for (const auto &snack : hunger_bar)
    {
        if (snack.first > you.hunger_state)
            continue;
        // Vampires get special coloring, as to not imply misleading
        // things about their hunger states with segment colors.
        rightmost = vp ? snack.second.second : snack.second.first;
        textcolour(rightmost);
    }
    for (const auto &snack : hunger_bar)
    {
        const char ch = (snack.first <= you.hunger_state
                         && you.hunger_state != HS_STARVING) ? '=' : '-';
        if (ch == '-')
            rightmost = vp ? snack.second.second : snack.second.first;
        textcolour(rightmost);
        NOWRAP_EOL_CPRINTF("%c", ch);
        if (snack.first == HS_SATIATED)
            NOWRAP_EOL_CPRINTF("%c", ch);
    }
}

static void _print_realtime(const int x = realtime_pos_x,
                            const int y = realtime_pos_y)
{
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(HUD_VALUE_COLOUR);
    CPRINTF(make_time_string(you.real_time, true).c_str());
}

/**
 * Get the appropriate colour for the UI text describing the player's weapon.
 * (Or hands/ice fists/etc, as appropriate.)
 *
 * @return     A colour enum for the player's weapon.
 */
static int _wpn_name_colour()
{
    if (you.duration[DUR_CORROSION])
        return RED;

    if (you.weapon())
    {
        const item_def& wpn = *you.weapon();

        const string prefix = item_prefix(wpn);
        const int prefcol = menu_colour(wpn.name(DESC_INVENTORY), prefix, "stats");
        if (prefcol != -1)
            return prefcol;
        return LIGHTGREY;
    }

    return get_form()->uc_colour;
}

static bool _best_skills(int i, int j)
{
    if (skill_focused(i) != skill_focused(j))
        return skill_focused(i);
    else
        return you.skills[i] > you.skills[j];
}

/**
 * Print a brief one-line overview of currently trained skills to the HUD.
 * Kinda sorta relies on `short_skill_name` only rarely returning four chars.
 *
 * @param x The x-coordinate to print the skill bar at.
 * @param y The y-coordinate to print the skill bar at.
 */
static void _print_stats_skills(const int x = skills_pos_x,
                                const int y = skills_pos_y) 
{
    // you.redraw_skills = false; ////TODO implement this perhaps?
    vector<skill_type> trained_skills;
    for (int i = 0; i < NUM_SKILLS; ++i)
        if (skill_trained(i))
            trained_skills.push_back(static_cast<skill_type>(i));

    sort(trained_skills.begin(), trained_skills.end(), _best_skills);

    int long_len = -2; // Don't count trailing two spaces.
    int short_len = -2; // Don't count trailing two spaces.
    for (unsigned int i = 0; i < trained_skills.size(); ++i)
    {
        const skill_type sk = trained_skills[i];
        long_len += strwidth(long_skill_name(sk));
        long_len += 3 + (you.skill(sk, 10) > 99 ? 4 : 3);
        if (i > 3)
            continue;
        short_len += strwidth(short_skill_name(sk));
        short_len += 3 + (you.skill(sk, 10) > 99 ? 4 : 3);
    }

    CGOTOXY(x, y, GOTO_STAT);
    const bool long_names_fit = long_len <= 42; // HUD_WIDTH
    const bool shorten_ellipsis = short_len > 39; // HUD_WIDTH - 3

    for (unsigned int i = 0; i < trained_skills.size(); ++i)
    {
        textcolour(HUD_CAPTION_COLOUR);
        if (i > 3)
        {
            textbackground(COLFLAG_REVERSE);
            if (shorten_ellipsis)
            {
                CGOTOXY(crawl_view.mlistsz.x - 1, y, GOTO_STAT);
                CPRINTF("…");
            }
            else
            {
                CGOTOXY(crawl_view.mlistsz.x - 3, y, GOTO_STAT);
                CPRINTF("(…)");
            }
            textbackground(BLACK);
            break;
        }
        if (long_names_fit)
            CPRINTF("%s", long_skill_name(trained_skills[i]));
        else
            CPRINTF("%s", short_skill_name(trained_skills[i]).c_str());

        if (skill_focused(trained_skills[i]))
            CPRINTF("*");
        else
            CPRINTF(" ");

        textcolour(HUD_VALUE_COLOUR);
        CPRINTF("%.1f  ", (double_t) you.skill(trained_skills[i], 10) / 10.0);
        clear_to_end_of_line();
    }
}

/**
 * Print a description of the player's weapon (or lack thereof) to the UI.
 *
 * @param x     The x-coordinate to print the description at.
 * @param y     The y-coordinate to print the description at.
 */
static void _print_wp(const int x = wp_pos_x, const int y = wp_pos_y)
{
    string text;
    if (you.weapon())
    {
        item_def wpn = *you.weapon();

        if (you.duration[DUR_CORROSION])
        {
            if (wpn.base_type == OBJ_RODS)
                wpn.special -= 3 * you.props["corrosion_amount"].get_int();
            else
                wpn.plus -= 3 * you.props["corrosion_amount"].get_int();
        }
        text = wpn.name(DESC_PLAIN, true, false, true);
    }
    else
        text = you.unarmed_attack_name();

    CGOTOXY(x, y, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);
    const char slot_letter = you.weapon() ? index_to_letter(you.weapon()->link)
                                          : '-';
    const string slot_name = make_stringf("%c) ", slot_letter);
    CPRINTF("%s", slot_name.c_str());
    textcolour(_wpn_name_colour());
    const int max_name_width = HUD_WIDTH - slot_name.size();
    CPRINTF("%s", chop_string(text, max_name_width).c_str());
    textcolour(LIGHTGREY);
}

/**
 * Print a description of the player's quivered item stack (or lack thereof)
 * to the UI.
 *
 * @param x     The x-coordinate to print the description at.
 * @param y     The y-coordinate to print the description at.
 */
static void _print_qv(const int x = qv_pos_x, const int y = qv_pos_y)
{
    you.redraw_quiver = false;
    int col;
    string text;
    const int q = you.m_quiver->get_fire_item();
    ASSERT_RANGE(q, -1, ENDOFPACK);
    char hud_letter = '-';
    if (q != -1 && !fire_warn_if_impossible(true))
    {
        const item_def& quiver = you.inv[q];
        hud_letter = index_to_letter(quiver.link);
        const string prefix = item_prefix(quiver);
        const int prefcol =
            menu_colour(quiver.name(DESC_PLAIN), prefix, "stats");
        if (prefcol != -1)
            col = prefcol;
        else
            col = LIGHTGREY;
        text = quiver.name(DESC_PLAIN, true);
    }
    else
    {
        if (fire_warn_if_impossible(true))
        {
            col  = DARKGREY;
            text = "Quiver unavailable";
        }
        else
        {
            col  = LIGHTGREY;
            text = "Nothing quivered";
        }
    }
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF("%c) ", hud_letter);
    textcolour(col);
    CPRINTF("%s", chop_string(text, HUD_WIDTH).c_str());
    textcolour(LIGHTGREY);
}

struct status_light
{
    status_light(int c, string t) : colour(c), text(t) {}
    colour_t colour;
    string text;
};

static void _add_status_light_to_out(int i, vector<status_light>& out)
{
    status_info inf;

    if (fill_status_info(i, &inf) && !inf.light_text.empty())
    {
        status_light sl(inf.light_colour, inf.light_text);
        out.push_back(sl);
    }
}

// The colour scheme for these flags is currently:
//
// - yellow, "orange", red      for bad conditions
// - light grey, white          for god based conditions
// - green, light green         for good conditions
// - blue, light blue           for good enchantments
// - magenta, light magenta     for "better" enchantments (deflect, fly)
//
// Prints hunger,
// pray, holy, teleport, regen, fly/lev, invis, silence,
//   conf. touch, sage
// confused, mesmerised, fire, poison, disease, rot, held, glow, swift,
//   fast, slow, breath
//
// Note the usage of bad_ench_colour() correspond to levels that
// can be found in player.cc, ie those that the player can tell by
// using the '@' command.  Things like confusion and sticky flame
// hide their amounts and are thus always the same colour (so
// we're not really exposing any new information). --bwr
static void _get_status_lights(vector<status_light>& out)
{
#ifdef DEBUG_DIAGNOSTICS
    {
        static char static_pos_buf[80];
        snprintf(static_pos_buf, sizeof(static_pos_buf),
                 "%2d,%2d", you.pos().x, you.pos().y);
        out.emplace_back(LIGHTGREY, static_pos_buf);

        static char static_hunger_buf[80];
        snprintf(static_hunger_buf, sizeof(static_hunger_buf),
                 "(%d:%d)", you.hunger - you.old_hunger, you.hunger);
        out.emplace_back(LIGHTGREY, static_hunger_buf);
    }
#endif

    // We used to have to hardcode every status, now we just hardcode the
    // statuses important enough to appear first. (Rightmost)
    const unsigned int important_statuses[] =
    {
        STATUS_STR_ZERO, STATUS_INT_ZERO, STATUS_DEX_ZERO,
        STATUS_HUNGER,
        DUR_PARALYSIS,
        DUR_CONF,
        DUR_PETRIFYING,
        DUR_PETRIFIED,
        DUR_BERSERK,
        DUR_TELEPORT,
        DUR_HASTE,
        DUR_SLOW,
        STATUS_SPEED,
        DUR_DEATHS_DOOR,
        DUR_EXHAUSTED,
        DUR_QUAD_DAMAGE,
    };

    bitset<STATUS_LAST_STATUS + 1> done;
    for (unsigned important : important_statuses)
    {
        _add_status_light_to_out(important, out);
        done.set(important);
    }

    for (unsigned status = 0; status <= STATUS_LAST_STATUS ; ++status)
        if (!done[status])
            _add_status_light_to_out(status, out);
}

static void _print_status_lights(const int x = status_pos_x,
                                 const int y = status_pos_y) 
{
    you.redraw_status_flags = 0;
    vector<status_light> lights;
    static int last_number_of_lights = 0;
    _get_status_lights(lights);
    if (lights.empty() && last_number_of_lights == 0)
        return;
    last_number_of_lights = lights.size();

    const size_t line_end = crawl_view.hudsz.y+1;

    CGOTOXY(x, y, GOTO_STAT);
#ifdef ASSERTS
    if (wherex() != crawl_view.hudp.x)
    {
        save_game(false); // should be safe
        die("misaligned HUD (is %d, should be %d)", wherex(), crawl_view.hudp.x);
    }
#endif

#ifdef USE_TILE_LOCAL
    if (!tiles.is_using_small_layout())
    {
#endif
    size_t line_cur = y;
    size_t i_light = 0;
    while (true)
    {
        const int end_x = (wherex() - crawl_view.hudp.x)
                + (i_light < lights.size() ? strwidth(lights[i_light].text)
                                           : 10000);

        if (end_x <= HUD_WIDTH)
        {
            textcolour(lights[i_light].colour);
            CPRINTF("%s", lights[i_light].text.c_str());
            if (end_x < HUD_WIDTH)
                CPRINTF(" ");
            ++i_light;
        }
        else
        {
            clear_to_end_of_line();
            ++line_cur;
            // Careful not to trip the )#(*$ CGOTOXY ASSERT
            if (line_cur == line_end)
                break;
            CGOTOXY(x, line_cur, GOTO_STAT);
        }
    }
#ifdef USE_TILE_LOCAL
    }
    else
    {
        size_t i_light = 0;
        if (lights.size() == 1)
        {
            textcolour(lights[0].colour);
            CPRINTF("%s", lights[0].text.c_str());
        }
        else
        {
            while (i_light < lights.size() && (int)i_light < HUD_WIDTH - 1)
            {
                textcolour(lights[i_light].colour);
                if (i_light == lights.size() - 1
                    && strwidth(lights[i_light].text) < HUD_WIDTH - wherex())
                {
                    CPRINTF("%s",lights[i_light].text.c_str());
                }
                else if ((int)lights.size() > HUD_WIDTH / 2)
                    CPRINTF("%.1s",lights[i_light].text.c_str());
                else
                    CPRINTF("%.1s ",lights[i_light].text.c_str());
                ++i_light;
            }
        }
        clear_to_end_of_line();
    }
#endif
}

#ifdef USE_TILE_LOCAL
static bool _need_stats_printed()
{
    return you.redraw_title
           || you.redraw_hit_points
           || you.redraw_magic_points
           || you.redraw_armour_class
           || you.redraw_evasion
           || you.redraw_shield_class
           || you.redraw_stats[STAT_STR]
           || you.redraw_stats[STAT_INT]
           || you.redraw_stats[STAT_DEX]
           || you.redraw_experience
           || you.wield_change
           || you.redraw_quiver;
}
#endif

static void _draw_wizmode_flag(const char *word, const int y)
{
    // -2 for the "**"
    int x = FIRST_COL + HUD_WIDTH - strlen(word) - 2;
    if (HUD_CRAMPED)
        x += strlen(word) -1;
    textcolour(LIGHTMAGENTA);
    CGOTOXY(x, y, GOTO_STAT);
    if (HUD_CRAMPED)
        CPRINTF("*%c*", word[0]);
    else
        CPRINTF("*%s*", word);
}

// Minotaur of Okawaru **....
static void _stats_god(const int x = god_pos_x, const int y = god_pos_y)
{
    if (HUD_CRAMPED)
        return;

    CGOTOXY(x, y, GOTO_STAT);
    string species = species_name(you.species);
    textcolour(YELLOW);
    NOWRAP_EOL_CPRINTF("%s", species.c_str());
    if (!you_worship(GOD_NO_GOD))
    {
        string god = " of ";
        god += you_worship(GOD_JIYVA) ? god_name_jiyva(true)
                                      : god_name(you.religion);
        NOWRAP_EOL_CPRINTF("%s", god.c_str());

        string piety = _god_asterisks();
        textcolour(_god_status_colour(YELLOW));
        const int linelength = 
            FIRST_COL + strwidth(species) + strwidth(god) + strwidth(piety);
        // "Mottled Draconian of the Shining One ***..." doesn't fit
        // by one char. Trim to "the Shining One***..." in that case.
        if (linelength <= HUD_WIDTH)
            NOWRAP_EOL_CPRINTF(" %s", piety.c_str());
        else if (linelength == HUD_WIDTH + 1)
            NOWRAP_EOL_CPRINTF("%s", piety.c_str());
    }
    else if (you.char_class == JOB_MONK && you.species != SP_DEMIGOD
             && !had_gods())
    {
        string piety = "**....";
        textcolour(DARKGREY);
        const int linelength = FIRST_COL + strwidth(species) + strwidth(piety);
        if (linelength <= HUD_WIDTH)
            NOWRAP_EOL_CPRINTF(" %s", piety.c_str());
    }

    clear_to_end_of_line(); ////TODO // penance stars shorter / abandonment
}

// Line 1: Foo the Fighter        *WIZARD*
static void _redraw_title(const string &your_name, const string &job_name,
                          const int x = name_pos_x, const int y = name_pos_y)
{
    you.redraw_title = false;
    string title = your_name + " " + job_name;

    // Player name, shortened if necessary, and player title
    // [Foo the Fighter]        *WIZARD*
    // [VeryLongNameF, Fighter] *WIZARD*
#ifdef USE_TILE_LOCAL
    if (tiles.is_using_small_layout())
        title = your_name;
    else
#endif
    {
        int in_len = strwidth(title);
        if (you.wizard || you.explore)
            in_len += HUD_CRAMPED ? 3 : 8; // *W* *E* | *WIZARD* *EXPLORE*
        if (you.wizard || you.explore)
            in_len += 1; // flags being placed 1 col from border: *W* |
        if (HUD_CRAMPED)
            in_len += 5; // "[MiFi] "
        if (HUD_CRAMPED || in_len > HUD_WIDTH)
        {
            // What we're getting back from replacing " the " with ", "
            in_len -= 3;
            const int name_len = strwidth(your_name);
            string trimmed_name = your_name;
            // Squeeze name if required, the "- 8" is to not squeeze too much.
            if (in_len > HUD_WIDTH && (name_len + 2) > (in_len - HUD_WIDTH))
            {
                trimmed_name = chop_string(trimmed_name,
                                           name_len - (in_len - HUD_WIDTH) - 1);
            }

            title = trimmed_name + ", " + job_name.substr(4);
        }
    }

    // [[MiFi] ]Foo the Fighter        *WIZARD*
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(YELLOW);
    if (HUD_CRAMPED)
    {
        CPRINTF("[%s%s] ",
            get_species_abbrev(you.species),
            get_job_abbrev(you.char_class));
    }
#ifdef USE_TILE_LOCAL
    if (tiles.is_using_small_layout() && you.wizard) textcolour(LIGHTMAGENTA);
#endif
    CPRINTF("%s", chop_string(title, HUD_WIDTH).c_str());

    // Wizmode / Explore mode flags
    // Foo the Fighter       [*WIZARD* ]
    const char* wizmode = you.wizard ? "WIZARD" : you.explore ? "EXPLORE" : "";
    if ((you.wizard || you.explore)
#ifdef USE_TILE_LOCAL
        && !tiles.is_using_small_layout()
#endif
    )
        _draw_wizmode_flag(wizmode, y);

    // DGL message hint
    // Foo the Fighter        [(Hit _) ]
#ifdef DGL_SIMPLE_MESSAGING
    update_message_status();
#endif
    _stats_god();
}

static string _level_description_string_hud()
{
    const PlaceInfo& place = you.get_place_info();
    string short_name;
    if (HUD_COMPACT)
        short_name = branches[place.branch].abbrevname;
    else
        short_name = branches[place.branch].shortname;

    if (brdepth[place.branch] > 1)
        short_name += make_stringf(":%d", you.depth);
    // Indefinite articles
    else if (place.branch != BRANCH_PANDEMONIUM && !is_connected_branch(place.branch))
        short_name = article_a(short_name);
    return short_name;
}

static void _print_place(const int x = place_pos_x, const int y = place_pos_y)
{
    //you.redraw_experience = false;
    CGOTOXY(x, y, GOTO_STAT);

    if (HUD_COMPACT && !crawl_state.game_is_zotdef())
    {
        textcolour(HUD_CAPTION_COLOUR);
        // align w/actions (hudswappy): usually single-digit turn
        //CPRINTF("In  ");
        // This looks less bad while also eathing three chars
        //CPRINTF("Lvl"); // but is bad since XL</>Lvl in same column
        //CPRINTF("Plog "); // Site Camp  Location*
        if (!HUD_CRAMPED)
            CPRINTF("Place "); // Site Camp  Location*

        textcolour(HUD_VALUE_COLOUR);
        // 8: "Vaults:3"
        CPRINTF("%-8s", _level_description_string_hud().c_str());
    }
}

static void _print_xl(const int x = xl_pos_x, const int y = xl_pos_y)
{
    you.redraw_experience = false;
    CGOTOXY(x, y, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF("XL");

    if (!HUD_COMPACT)
        CPRINTF(":");
    textcolour(HUD_VALUE_COLOUR);
    CPRINTF("%3d", you.experience_level);
    if (you.experience_level >= you.get_max_xl())
        CPRINTF("%4s", "");
    else
    {
        textcolour(HUD_CAPTION_COLOUR);
        CPRINTF(".%02d ", get_exp_progress());
    }
}

static void _print_zp(const int x = zp_pos_x, const int y = zp_pos_y)
{
    if (!crawl_state.game_is_zotdef())
        return;

    CGOTOXY(x, y, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF(HUD_COMPACT ? "ZP" : "ZP:");
    textcolour(HUD_VALUE_COLOUR);
    CPRINTF(" %d", you.zot_points);
}

static void _print_gold(const bool terse=false,
                        const int x = gold_pos_x, const int y = gold_pos_y)
{
#ifdef USE_TILE_LOCAL
    if (tiles.is_using_small_layout())
        return;
#endif
    textcolour(HUD_CAPTION_COLOUR);
    CGOTOXY(x, y, GOTO_STAT);
    // glyph_to_tagstr(get_item_glyph(item))
    CPRINTF(terse ? "$ " : HUD_COMPACT ? "Gold  " : "Gold: "); // ⌛
    textcolour(HUD_VALUE_COLOUR);
    CPRINTF("%d", you.gold);
}

void print_stats()
{
    cursor_control coff(false);
    textcolour(LIGHTGREY);

    // Displayed evasion is tied to dex/str.
    if (you.redraw_stats[STAT_DEX] || you.redraw_stats[STAT_STR])
        you.redraw_evasion = true;

    if (HP_Bar.wants_redraw())
        you.redraw_hit_points = true;
    if (MP_Bar.wants_redraw())
        you.redraw_magic_points = true;
#if TAG_MAJOR_VERSION == 34
    if (Temp_Bar.wants_redraw() && you.species == SP_LAVA_ORC)
        you.redraw_temperature = true;
#endif

    // Poison display depends on regen rate, so should be redrawn every turn.
    if (you.duration[DUR_POISONING])
    {
        you.redraw_hit_points = true;
        you.redraw_status_flags |= REDRAW_POISONED;
    }

#ifdef USE_TILE_LOCAL
    bool has_changed = _need_stats_printed();
#endif

    if (you.redraw_title)
        _redraw_title(you.your_name, filtered_lang(player_title()));
    if (you.redraw_hit_points)
        _print_stats_hp();
    if (you.redraw_magic_points)
        _print_stats_mp();
#if TAG_MAJOR_VERSION == 34
    _print_stats_contam();

    if (you.redraw_temperature)
        _print_temperature();
#endif
    //if (you.redraw_armour_class || you.redraw_evasion || you.redraw_shield_class)
    //    _print_defenses();
    // For historic reasons, these two are still combined //// TODO
    if (you.redraw_armour_class || you.redraw_shield_class)
        _print_stats_sh();
    if (you.redraw_armour_class)
        _print_stats_ac();
    if (you.redraw_evasion)
        _print_stats_ev();

    if (you.redraw_stats[STAT_STR])
        _print_str();
    if (you.redraw_stats[STAT_INT])
        _print_int();
    if (you.redraw_stats[STAT_DEX])
        _print_dex();
    you.redraw_stats.init(false);

    if (you.redraw_experience)
        _print_xl();

    _print_place();

    // Gold
    if (!HUD_CRAMPED)
        _print_gold();

    if (HUD_CRAMPED)
        _print_piety();

    _print_hunger();
    //_print_defenses();
    //_print_fcrawl();
    //_print_sid();
    //_print_realtime();
    //_print_resists();
    //_print_stairs();

    _print_zp();

    if (you.wield_change)
    {
        // wield_change is set in a billion places; probably not all
        // of them actually mean the user changed their weapon.  Calling
        // on_weapon_changed redundantly is normally OK; but if the user
        // is wielding a bow and throwing javelins, the on_weapon_changed
        // will switch them back to arrows, which is annoying.
        // Perhaps there should be another bool besides wield_change
        // that's set in fewer places?
        // Also, it's a little bogus to change simulation state in
        // render code.  We should find a better place for this.
        you.m_quiver->on_weapon_changed();
        _print_wp();
    }
    you.wield_change = false;

    // There are no circumstances under which Felids could quiver something.
    if (you.species != SP_FELID && (you.redraw_quiver || you.wield_change))
        _print_qv();

    if (Options.show_skill_bar)
        _print_stats_skills();

    if (you.redraw_status_flags)
        _print_status_lights();

    textcolour(LIGHTGREY);
    //_print_fcrawl( 8, 5);
    //_print_fcrawl( 8, 6);
    //_print_fcrawl( 8, 7);

    //_print_fcrawl(17, 5);
    //_print_fcrawl(17, 6);
    //_print_fcrawl(17, 7);

    //_print_fcrawl(26, 5);
    //_print_fcrawl(26, 6);
    //_print_fcrawl(26, 7);

    //_print_fcrawl(35, 5);
    //_print_fcrawl(35, 6);
    //_print_fcrawl(35, 7);

    //_print_fcrawl( 1, 5);
    //_print_fcrawl( 1, 6);
    //_print_fcrawl( 1, 7);
    //_print_fcrawl( 8, 5);
    //_print_fcrawl( 8, 6);
    //_print_fcrawl( 8, 7);
    //_print_fcrawl(15, 5);
    //_print_fcrawl(15, 6);
    //_print_fcrawl(15, 7);
    //_print_fcrawl(22, 5);
    //_print_fcrawl(22, 6);
    //_print_fcrawl(22, 7);
    //_print_fcrawl(29, 5);
    //_print_fcrawl(29, 6);
    //_print_fcrawl(29, 7);
    //_print_fcrawl(36, 5);
    //_print_fcrawl(36, 6);
    //_print_fcrawl(36, 7);

#ifdef USE_TILE_LOCAL
    if (has_changed)
        update_screen();
#else
    update_screen();
#endif
}

void print_stats_level()
{
    if (HUD_COMPACT)
    {
        print_stats();
        return;
    }
    int ypos = 8;
#if TAG_MAJOR_VERSION == 34
    if (you.species == SP_LAVA_ORC)
        ypos++;
#endif
    cgotoxy(CLASSIC_B, ypos, GOTO_STAT);
    textcolour(HUD_CAPTION_COLOUR);
    CPRINTF("Place: ");

    textcolour(HUD_VALUE_COLOUR);
#ifdef DEBUG_DIAGNOSTICS
    CPRINTF("(%d) ", env.absdepth0 + 1);
#endif
    CPRINTF("%s", _level_description_string_hud().c_str());
    clear_to_end_of_line();
}

void draw_border()
{
    clrscr();

    textcolour(HUD_CAPTION_COLOUR);

    // Traditional HUD layout:                         Compact HUD layout:
    //  | 1 3 5 7 9. . . . .  1 3 5 7 9. . . . .  1 3|  | 1 3 5 7 9. . . . .  1 3 5 7 9. . . . .  1 3|
    //  |. . . . .  1 3 5 7 9. . . . .  1 3 5 7 9. . |  |. . . . .  1 3 5 7 9. . . . .  1 3 5 7 9. . |
    // 1| wizard the Spellbinder            *WIZARD* | 1| wizard the Spellbinder            *WIZARD* |
    //  | Mottled Draconian of the Shining One**.... |  | Mottled Draconian of the Shining One**.... |
    // 3| Health: 113/181   ==============---------- | 3| H 113/181 M 16/16 ==============---------- |
    //  | Magic:  16/16     ======================== |  | AC  3   Str  4    ======================== |
    // 5| AC:  3            Str: 4                   | 5| EV 18   Int 15/16 XL 20.81  Orcish Mines:3 |
    //  | EV: 18            Int: 15 (16)             |  | SH  0   Dex 16    @   0.7   Turns 19704    |
    // 7| SH:  0            Dex: 16                  | 7| a) +6 spear {reap, Hex rPois rN+ HP-6}     |
    //  | XL: 20 Next: 81%  Place: Orcish Mines:1    |  | f) 10 stones                               |
    // 9| Gold: 2436        Time: 17190.2 (0.7)      | 9| Dodging*1.7  Stealth 3.2  Fighting 0.0 (…) |
    //  | a) +6 spear {reap, Hex rPois rN+ HP-6}     |  | Engorged                                   |
    //11| -) Nothing quivered                        |11|                                            |
    //  | Engorged                                   |  | ee  2 deep elf blademasters                |
    //13|                                            |13| ee  2 deep elf master archers              |
    //  | ee  2 deep elf blademasters                |  | ee  2 deep elf annihilators                |
    //15| ee  2 deep elf master archers              |15| ee  3 deep elf demonologists               |
    //  | ee  2 deep elf annihilators                |  | e  5 deep elf knights                      |
    //17| ee  3 deep elf demonologists           (…) |17| e  5 deep elf summoners                (…) |
}

void set_redraw_status(uint64_t flags)
{
    you.redraw_status_flags |= flags;
}

void redraw_screen()
{
    if (!crawl_state.need_save)
    {
        // If the game hasn't started, don't do much.
        clrscr();
        return;
    }

#ifdef USE_TILE_WEB
    tiles.close_all_menus();
#endif

    draw_border();

    you.redraw_title        = true;
    you.redraw_hit_points   = true;
    you.redraw_magic_points = true;
#if TAG_MAJOR_VERSION == 34
    if (you.species == SP_LAVA_ORC)
        you.redraw_temperature = true;
#endif
    you.redraw_stats.init(true);
    you.redraw_armour_class = true;
    you.redraw_evasion      = true;
    you.redraw_shield_class = true;
    you.redraw_experience   = true;
    you.wield_change        = true;
    you.redraw_quiver       = true;
    //you.redraw_skills       = true;

    /////////////////// TODO // this is completely, horrifyingly outdated
    set_redraw_status(
                      REDRAW_LINE_1_MASK | REDRAW_LINE_2_MASK | REDRAW_LINE_3_MASK);

    print_stats();

    bool note_status = notes_are_active();
    activate_notes(false);
    if (!HUD_COMPACT)
        print_stats_level();
#ifdef DGL_SIMPLE_MESSAGING
    update_message_status();
#endif
    update_turn_count();
    activate_notes(note_status);

    viewwindow();

    // Display the message window at the end because it places
    // the cursor behind possible prompts.
    display_message_window();
    update_screen();
}

// ----------------------------------------------------------------------
// Monster pane
// ----------------------------------------------------------------------

static string _get_monster_name(const monster_info& mi, int count, bool fullname)
{
    string desc = "";

    const char * const adj = mi.attitude == ATT_FRIENDLY ? "friendly"
                           : mi.attitude == ATT_HOSTILE  ? nullptr
                                                         : "neutral";

    string monpane_desc;
    int col;
    mi.to_string(count, monpane_desc, col, fullname, adj);

    if (count == 1)
    {
        if (!mi.is(MB_NAME_THE))
            desc = (is_vowel(monpane_desc[0]) ? "an " : "a ") + desc;
        else if (adj || !mi.is(MB_NAME_UNQUALIFIED))
            desc = "the " + desc;
    }

    desc += monpane_desc;
    return desc;
}

// If past is true, the messages should be printed in the past tense
// because they're needed for the morgue dump.
string mpr_monster_list(bool past)
{
    // Get monsters via the monster_pane_info, sorted by difficulty.
    vector<monster_info> mons;
    get_monster_info(mons);

    string msg = "";
    if (mons.empty())
    {
        msg  = "There ";
        msg += (past ? "were" : "are");
        msg += " no monsters in sight!";

        return msg;
    }

    vector<string> describe;

    int count = 0;
    for (unsigned int i = 0; i < mons.size(); ++i)
    {
        if (i > 0 && monster_info::less_than(mons[i-1], mons[i]))
        {
            describe.push_back(_get_monster_name(mons[i-1], count, true).c_str());
            count = 0;
        }
        count++;
    }

    describe.push_back(_get_monster_name(mons[mons.size()-1], count, true).c_str());

    msg = "You ";
    msg += (past ? "could" : "can");
    msg += " see ";

    if (describe.size() == 1)
        msg += describe[0];
    else
        msg += comma_separated_line(describe.begin(), describe.end());
    msg += ".";

    return msg;
}

#ifndef USE_TILE_LOCAL
static void _print_next_monster_desc(const vector<monster_info>& mons,
                                     int& start, bool zombified = false,
                                     int idx = -1)
{
    // Skip forward to past the end of the range of identical monsters.
    unsigned int end;
    for (end = start + 1; end < mons.size(); ++end)
    {
        // Array is sorted, so if !(m1 < m2), m1 and m2 are "equal".
        if (monster_info::less_than(mons[start], mons[end], zombified, zombified))
            break;
    }
    // Postcondition: all monsters in [start, end) are "equal"

    // Print info on the monsters we've found.
    {
        int printed = 0;

        // for targeting
        if (idx >= 0)
        {
            textcolour(WHITE);
            CPRINTF(stringize_glyph(mlist_index_to_letter(idx)).c_str());
            CPRINTF(" - ");
            printed += 4;
        }

        // One glyph for each monster.
        for (unsigned int i_mon = start; i_mon < end; i_mon++)
        {
            monster_info mi = mons[i_mon];
            cglyph_t g = get_mons_glyph(mi);
            textcolour(g.col);
            CPRINTF("%s", stringize_glyph(g.ch).c_str());
            ++printed;

            // Printing too many looks pretty bad, though.
            if (i_mon > 6)
                break;
        }
        textcolour(LIGHTGREY);

        const int count = (end - start);

        if (count == 1)  // Print an icon representing damage level.
        {
            CPRINTF(" ");

            monster_info mi = mons[start];
#ifdef TARGET_OS_WINDOWS
            textcolour(real_colour(dam_colour(mi) | COLFLAG_ITEM_HEAP));
#else
            textcolour(real_colour(dam_colour(mi) | COLFLAG_REVERSE));
#endif
            CPRINTF(" ");
            textbackground(BLACK);
            textcolour(LIGHTGREY);
            CPRINTF(" ");
            printed += 3;
        }
        else
        {
            textcolour(LIGHTGREY);
            CPRINTF("  ");
            printed += 2;
        }

        if (printed < crawl_view.mlistsz.x)
        {
            int desc_colour;
            string desc;
            mons[start].to_string(count, desc, desc_colour, zombified);
            textcolour(desc_colour);
            desc.resize(crawl_view.mlistsz.x-printed, ' ');
            CPRINTF("%s", desc.c_str());
        }
    }

    // Set start to the next un-described monster.
    start = end;
    textcolour(LIGHTGREY);
}
#endif

#ifndef USE_TILE_LOCAL
// #define BOTTOM_JUSTIFY_MONSTER_LIST
// Returns -1 if the monster list is empty, 0 if there are so many monsters
// they have to be consolidated, and 1 otherwise.
int update_monster_pane()
{
    if (!map_bounds(you.pos()) && !crawl_state.game_is_arena())
        return -1;

    const int max_print = crawl_view.mlistsz.y;
    textbackground(BLACK);

    if (max_print <= 0)
        return -1;

    vector<monster_info> mons;
    get_monster_info(mons);

    // Count how many groups of monsters there are.
    unsigned int lines_needed = mons.size();
    for (unsigned int i = 1; i < mons.size(); i++)
        if (!monster_info::less_than(mons[i-1], mons[i]))
            --lines_needed;

    bool full_info = true;
    if (lines_needed > (unsigned int) max_print)
    {
        full_info = false;

        // Use type names rather than full names ("small zombie" vs
        // "rat zombie") in order to take up fewer lines.

        lines_needed = mons.size();
        for (unsigned int i = 1; i < mons.size(); i++)
            if (!monster_info::less_than(mons[i-1], mons[i], false, false))
                --lines_needed;
    }

#ifdef BOTTOM_JUSTIFY_MONSTER_LIST
    const int skip_lines = max<int>(0, crawl_view.mlistsz.y-lines_needed);
#else
    const int skip_lines = 0;
#endif

    // Print the monsters!
    string blank;
    blank.resize(crawl_view.mlistsz.x, ' ');
    int i_mons = 0;
    for (int i_print = 0; i_print < max_print; ++i_print)
    {
        CGOTOXY(1, 1 + i_print, GOTO_MLIST);
        // i_mons is incremented by _print_next_monster_desc
        if (i_print >= skip_lines && i_mons < (int) mons.size())
        {
             int idx = crawl_state.mlist_targeting ? i_print - skip_lines : -1;
             _print_next_monster_desc(mons, i_mons, full_info, idx);
        }
        else
            CPRINTF("%s", blank.c_str());
    }

    if (i_mons < (int)mons.size())
    {
        // Didn't get to all of them.
        CGOTOXY(crawl_view.mlistsz.x - 3, crawl_view.mlistsz.y, GOTO_MLIST);
        textbackground(COLFLAG_REVERSE);
        CPRINTF("(…)");
        textbackground(BLACK);
    }

    if (mons.empty())
        return -1;

    return full_info;
}
#else
// FIXME: Implement this for Tiles!
int update_monster_pane()
{
    return false;
}
#endif

// Converts a numeric resistance to its symbolic counterpart.
// Can handle any maximum level. The default is for single level resistances
// (the most common case). Negative resistances are allowed.
// Resistances with a maximum of up to 4 are spaced (arbitrary choice), and
// starting at 5 levels, they are continuous.
// params:
//  level : actual resistance level
//  max : maximum number of levels of the resistance
static string _itosym(int level, int max = 1)
{
    if (max < 1)
        return "";

    string sym;
    bool spacing = (max >= 5) ? false : true;

    while (max > 0)
    {
        if (level == 0)
            sym += ".";
        else if (level > 0)
        {
            sym += "+";
            --level;
        }
        else // negative resistance
        {
            sym += "x";
            ++level;
        }
        sym += (spacing) ? " " : "";
        --max;
    }
    return sym;
}

static const char *s_equip_slot_names[] =
{
    "Weapon", "Cloak",  "Helmet", "Gloves", "Boots",
    "Shield", "Armour", "Left Ring", "Right Ring", "Amulet",
    "First Ring", "Second Ring", "Third Ring", "Fourth Ring",
    "Fifth Ring", "Sixth Ring", "Seventh Ring", "Eighth Ring",
    "Amulet Ring"
};

const char *equip_slot_to_name(int equip)
{
    COMPILE_CHECK(ARRAYSZ(s_equip_slot_names) == NUM_EQUIP);

    if (equip == EQ_RINGS
        || equip == EQ_LEFT_RING || equip == EQ_RIGHT_RING
        || equip >= EQ_RING_ONE && equip <= EQ_RING_EIGHT
        || equip == EQ_RING_AMULET)
    {
        return "Ring";
    }

    if (equip == EQ_BOOTS
        && (you.species == SP_CENTAUR || you.species == SP_NAGA))
    {
        return "Barding";
    }

    if (equip < 0 || equip >= NUM_EQUIP)
        return "";

    return s_equip_slot_names[equip];
}

int equip_name_to_slot(const char *s)
{
    for (int i = 0; i < NUM_EQUIP; ++i)
        if (!strcasecmp(s_equip_slot_names[i], s))
            return i;

    return -1;
}

// Colour the string according to the level of an ability/resistance.
// Take maximum possible level into account.
static const char* _determine_colour_string(int level, int max_level)
{
    // No colouring for larger bars.
    if (max_level > 3)
        return "<lightgrey>";

    switch (level)
    {
    case 3:
    case 2:
        if (max_level > 1)
            return "<lightgreen>";
        // else fall-through
    case 1:
        return "<green>";
    case -2:
    case -3:
        if (max_level > 1)
            return "<lightred>";
        // else fall-through
    case -1:
        return "<red>";
    default:
        return "<lightgrey>";
    }
}

static int _stealth_breakpoint(int stealth)
{
    if (stealth == 0)
        return 0;
    else if (stealth >= 500)
        return 10;
    else if (stealth >= 450)
        return 9;
    else
        return 1 + stealth / STEALTH_PIP;
}

static string _stealth_bar(int sw)
{
    string bar;
    //no colouring
    bar += _determine_colour_string(0, 5);
    bar += "Stlth  ";
    const int stealth_num = _stealth_breakpoint(check_stealth());
    for (int i = 0; i < stealth_num; i++)
        bar += "+";
    for (int i = 0; i < 10 - stealth_num; i++)
        bar += ".";
    bar += "\n";
    linebreak_string(bar, sw);
    return bar;
}
static string _status_mut_abilities(int sw);

// helper for print_overview_screen
static void _print_overview_screen_equip(column_composer& cols,
                                         vector<char>& equip_chars,
                                         int sw)
{
    const int e_order[] =
    {
        EQ_WEAPON, EQ_BODY_ARMOUR, EQ_SHIELD, EQ_HELMET, EQ_CLOAK,
        EQ_GLOVES, EQ_BOOTS, EQ_AMULET, EQ_RIGHT_RING, EQ_LEFT_RING,
        EQ_RING_ONE, EQ_RING_TWO, EQ_RING_THREE, EQ_RING_FOUR,
        EQ_RING_FIVE, EQ_RING_SIX, EQ_RING_SEVEN, EQ_RING_EIGHT,
        EQ_RING_AMULET,
    };

    sw = min(max(sw, 79), 640);

    char buf[641];
    for (int i = 0; i < NUM_EQUIP; i++)
    {
        int eqslot = e_order[i];

        if (you.species == SP_OCTOPODE
            && e_order[i] != EQ_WEAPON
            && !you_can_wear(e_order[i], true))
        {
            continue;
        }

        if (you.species == SP_OCTOPODE && (eqslot == EQ_RIGHT_RING
                                       || eqslot == EQ_LEFT_RING))
        {
            continue;
        }

        if (you.species != SP_OCTOPODE
            && eqslot >= EQ_RING_ONE && eqslot <= EQ_RING_EIGHT)
        {
            continue;
        }

        if (eqslot == EQ_RING_AMULET && !you_can_wear(eqslot))
            continue;

        const string slot_name_lwr = lowercase_string(equip_slot_to_name(eqslot));

        char slot[15] = "";

        if (you.equip[ e_order[i] ] != -1)
        {
            // The player has something equipped.
            const int item_idx   = you.equip[e_order[i]];
            const item_def& item = you.inv[item_idx];
            const bool melded    = !player_wearing_slot(e_order[i]);
            const string prefix = item_prefix(item);
            const int prefcol = menu_colour(item.name(DESC_INVENTORY), prefix);
            const int col = prefcol == -1 ? LIGHTGREY : prefcol;

            // Colour melded equipment dark grey.
            const char* colname  = melded ? "darkgrey"
                                          : colour_to_str(col).c_str();

            const char equip_char = index_to_letter(item_idx);

            snprintf(buf, sizeof buf,
                     "%s<w>%c</w> - <%s>%s%s</%s>",
                     slot,
                     equip_char,
                     colname,
                     melded ? "melded " : "",
                     chop_string(item.name(DESC_PLAIN, true),
                                 melded ? sw - 43 : sw - 36, false).c_str(),
                     colname);
            equip_chars.push_back(equip_char);
        }
        else if (e_order[i] == EQ_WEAPON
                 && you.skill(SK_UNARMED_COMBAT))
        {
            snprintf(buf, sizeof buf, "%s  - Unarmed", slot);
        }
        else if (e_order[i] == EQ_WEAPON
                 && you.form == TRAN_BLADE_HANDS)
        {
            const bool plural = !player_mutation_level(MUT_MISSING_HAND);
            snprintf(buf, sizeof buf, "%s  - Blade Hand%s", slot,
                     plural ? "s" : "");
        }
        else if (e_order[i] == EQ_BOOTS
                 && (you.species == SP_NAGA || you.species == SP_CENTAUR))
        {
            snprintf(buf, sizeof buf,
                     "<darkgrey>(no %s)</darkgrey>", slot_name_lwr.c_str());
        }
        else if (!you_can_wear(e_order[i], true))
        {
            snprintf(buf, sizeof buf,
                     "<darkgrey>(%s unavailable)</darkgrey>", slot_name_lwr.c_str());
        }
        else if (!you_tran_can_wear(e_order[i], true))
        {
            snprintf(buf, sizeof buf,
                     "<darkgrey>(%s currently unavailable)</darkgrey>",
                     slot_name_lwr.c_str());
        }
        else if (!you_can_wear(e_order[i]))
        {
            snprintf(buf, sizeof buf,
                     "<darkgrey>(%s restricted)</darkgrey>", slot_name_lwr.c_str());
        }
        else
        {
            snprintf(buf, sizeof buf,
                     "<darkgrey>(no %s)</darkgrey>", slot_name_lwr.c_str());
        }
        cols.add_formatted(2, buf, false);
    }
}

static string _overview_screen_title(int sw)
{
    string title = make_stringf(" %s ", player_title().c_str());

    string species_job = make_stringf("(%s %s)",
                                      species_name(you.species).c_str(),
                                      you.class_name.c_str());

    handle_real_time();
    string time_turns = make_stringf(" Turns: %d, Time: ", you.num_turns)
                      + make_time_string(you.real_time, true);

    const int char_width = strwidth(species_job);
    const int title_width = strwidth(title);

    int linelength = strwidth(you.your_name) + title_width
                   + char_width + strwidth(time_turns);

    if (linelength >= sw)
    {
        species_job = make_stringf("(%s%s)", get_species_abbrev(you.species),
                                             get_job_abbrev(you.char_class));
        linelength -= (char_width - strwidth(species_job));
    }

    // Still not enough?
    if (linelength >= sw)
    {
        title = " ";
        linelength -= (title_width - 1);
    }

    string text;
    text = "<yellow>";
    text += you.your_name;
    text += title;
    text += species_job;

    const int num_spaces = sw - linelength - 1;
    if (num_spaces > 0)
        text += string(num_spaces, ' ');

    text += time_turns;
    text += "</yellow>\n";

    return text;
}

#ifdef WIZARD
static string _wiz_god_powers()
{
    string godpowers = god_name(you.religion);
    return make_stringf("%s %d (%d)", god_name(you.religion).c_str(),
                                      you.piety,
                                      you.duration[DUR_PIETY_POOL]);
}
#endif

static string _god_powers()
{
    if (you_worship(GOD_NO_GOD))
        return "";

    const string name = god_name(you.religion);
    if (you_worship(GOD_GOZAG))
        return colour_string(name, _god_status_colour(god_colour(you.religion)));

    return colour_string(chop_string(name, 20, false)
              + " [" + _god_asterisks() + "]",
              _god_status_colour(god_colour(you.religion)));
}

static string _god_asterisks()
{
    if (you_worship(GOD_NO_GOD))
        return "";

    if (player_under_penance())
        return "* :( *";

    if (you_worship(GOD_GOZAG))
        return "$$$$$$";

    if (you_worship(GOD_XOM))
    {
        const int p_rank = xom_favour_rank() - 1;
        if (p_rank >= 0)
            return string(p_rank, '.') + "*" + string(5 - p_rank, '.');
        else
            return " >:)  "; // very special plaything
    }
    else
    {
        const int prank = piety_rank() - 1;
        return string(prank, '*') + string(6 - prank, '.');
    }
}

/**
 * What colour should the god status display be?
 *
 * @param default_colour   The default colour, if not under penance or boredom.
 * @return                 A colour for the god status display.
 */
static int _god_status_colour(int default_colour)
{
    if (player_under_penance())
        return RED;

    if (you_worship(GOD_XOM) && you.gift_timeout <= 1)
        return you.gift_timeout ? RED : LIGHTRED;

    return default_colour;
}

static bool _player_statrotted()
{
    return you.strength(false) != you.max_strength()
        || you.intel(false) != you.max_intel()
        || you.dex(false) != you.max_dex();
}

static vector<formatted_string> _get_overview_stats()
{
    formatted_string entry;

    // 4 columns
    int col1 = 20;
    int col2 = 10;
    int col3 = 11;

    if (player_rotted())
        col1 += 1;

    if (_player_statrotted())
        col3 += 2;

    column_composer cols(4, col1, col1 + col2, col1 + col2 + col3);

    entry.textcolour(HUD_CAPTION_COLOUR);
    if (player_rotted())
        entry.cprintf("HP:   ");
    else
        entry.cprintf("Health: ");

    if (_boosted_hp())
        entry.textcolour(LIGHTBLUE);
    else
        entry.textcolour(HUD_VALUE_COLOUR);

    entry.cprintf("%d/%d", you.hp, you.hp_max);
    if (player_rotted())
        entry.cprintf(" (%d)", get_real_hp(true, true));

    cols.add_formatted(0, entry.to_colour_string(), false);
    entry.clear();

    entry.textcolour(HUD_CAPTION_COLOUR);
    if (player_rotted())
        entry.cprintf("MP:   ");
    else
        entry.cprintf("Magic:  ");

    if (_boosted_mp())
        entry.textcolour(LIGHTBLUE);
    else
        entry.textcolour(HUD_VALUE_COLOUR);

    entry.cprintf("%d/%d", you.magic_points, you.max_magic_points);

    cols.add_formatted(0, entry.to_colour_string(), false);
    entry.clear();

    entry.textcolour(HUD_CAPTION_COLOUR);
    if (player_rotted())
        entry.cprintf("Gold: ");
    else
        entry.cprintf("Gold:   ");

    entry.textcolour(HUD_VALUE_COLOUR);

    entry.cprintf("%d", you.gold);

    cols.add_formatted(0, entry.to_colour_string(), false);
    entry.clear();

    entry.textcolour(HUD_CAPTION_COLOUR);
    entry.cprintf("AC: ");

    if (_boosted_ac())
        entry.textcolour(LIGHTBLUE);
    else
        entry.textcolour(HUD_VALUE_COLOUR);

    entry.cprintf("%2d", you.armour_class());
    cols.add_formatted(1, entry.to_colour_string(), false);
    entry.clear();

    entry.textcolour(HUD_CAPTION_COLOUR);
    entry.cprintf("EV: ");

    if (_boosted_ev())
        entry.textcolour(LIGHTBLUE);
    else
        entry.textcolour(HUD_VALUE_COLOUR);

    entry.cprintf("%2d", player_evasion());

    cols.add_formatted(1, entry.to_colour_string(), false);
    entry.clear();

    entry.textcolour(HUD_CAPTION_COLOUR);
    entry.cprintf("SH: ");

    if (_boosted_sh())
        entry.textcolour(LIGHTBLUE);
    else
        entry.textcolour(HUD_VALUE_COLOUR);

    entry.cprintf("%2d", player_displayed_shield_class());

    cols.add_formatted(1, entry.to_colour_string(), false);
    entry.clear();

    entry.textcolour(HUD_CAPTION_COLOUR);
    entry.cprintf("Str: ");

    entry.textcolour(_get_stat_colour(STAT_STR));

    entry.cprintf("%2d", you.strength(false));
    if (you.strength(false) != you.max_strength())
        entry.cprintf(" (%d)", you.max_strength());

    cols.add_formatted(2, entry.to_colour_string(), false);
    entry.clear();

    entry.textcolour(HUD_CAPTION_COLOUR);
    entry.cprintf("Int: ");

    entry.textcolour(_get_stat_colour(STAT_INT));

    entry.cprintf("%2d", you.intel(false));
    if (you.intel(false) != you.max_intel())
        entry.cprintf(" (%d)", you.max_intel());

    cols.add_formatted(2, entry.to_colour_string(), false);
    entry.clear();

    entry.textcolour(HUD_CAPTION_COLOUR);
    entry.cprintf("Dex: ");

    entry.textcolour(_get_stat_colour(STAT_DEX));

    entry.cprintf("%2d", you.dex(false));
    if (you.dex(false) != you.max_dex())
        entry.cprintf(" (%d)", you.max_dex());

    cols.add_formatted(2, entry.to_colour_string(), false);
    entry.clear();

    entry.textcolour(HUD_CAPTION_COLOUR);
    entry.cprintf("XL:     ");

    entry.textcolour(HUD_VALUE_COLOUR);
    entry.cprintf("%d", you.experience_level);

    if (you.experience_level < you.get_max_xl())
    {
        entry.textcolour(HUD_CAPTION_COLOUR);
        entry.cprintf("   Next: ");

        entry.textcolour(HUD_VALUE_COLOUR);
        entry.cprintf("%d%%", get_exp_progress());
    }

    cols.add_formatted(3, entry.to_colour_string(), false);
    entry.clear();

    entry.textcolour(HUD_CAPTION_COLOUR);
    entry.cprintf("God:    ");

    entry.textcolour(HUD_VALUE_COLOUR);

    string godpowers = _god_powers();
#ifdef WIZARD
    if (you.wizard)
        godpowers = _wiz_god_powers();
#endif
    entry += formatted_string::parse_string(godpowers);

    cols.add_formatted(3, entry.to_colour_string(), false);
    entry.clear();

    entry.textcolour(HUD_CAPTION_COLOUR);
    entry.cprintf("Spells: ");

    entry.textcolour(HUD_VALUE_COLOUR);
    entry.cprintf("%d memorised, %d level%s left",
                  you.spell_no, player_spell_levels(),
                  (player_spell_levels() == 1) ? "" : "s");

    cols.add_formatted(3, entry.to_colour_string(), false);
    entry.clear();

    if (you.species == SP_FELID)
    {
        entry.textcolour(HUD_CAPTION_COLOUR);
        entry.cprintf("Lives:  ");

        entry.textcolour(HUD_VALUE_COLOUR);
        entry.cprintf("%d", you.lives);

        entry.textcolour(HUD_CAPTION_COLOUR);
        entry.cprintf("   Deaths: ");

        entry.textcolour(HUD_VALUE_COLOUR);
        entry.cprintf("%d", you.deaths);

        cols.add_formatted(3, entry.to_colour_string(), false);
        entry.clear();
    }

    return cols.formatted_lines();
}

// generator of resistance strings:
// params :
//      name : name of the resist, correct spacing is handled here
//      spacing : width of the name column
//      value : actual value of the resistance (can be negative)
//      max : maximum value of the resistance (for colour AND representation),
//          default is the most common case (1)
//      pos_resist : false for "bad" resistances (no tele, random tele, *Rage),
//          inverts the value for the colour choice
static string _resist_composer(
    const char * name, int spacing, int value, int max = 1, bool pos_resist = true)
{
    string out;
    out += _determine_colour_string(pos_resist ? value : -value, max);
    out += chop_string(name, spacing);
    out += _itosym(value, max);

    return out;
}

static vector<formatted_string> _get_overview_resistances(
    vector<char> &equip_chars, bool calc_unid, int sw)
{
    // 3 columns, splits at columns 18, 33
    column_composer cols(3, 18, 33);
    // First column, resist name is 7 chars
    int cwidth = 7;
    string out;

    const int rfire = player_res_fire(calc_unid);
    out += _resist_composer("rFire", cwidth, rfire, 3) + "\n";

    const int rcold = player_res_cold(calc_unid);
    out += _resist_composer("rCold", cwidth, rcold, 3) + "\n";

    const int rlife = player_prot_life(calc_unid);
    out += _resist_composer("rNeg", cwidth, rlife, 3) + "\n";

    const int rpois = player_res_poison(calc_unid);
    string rpois_string = _resist_composer("rPois", cwidth, rpois) + "\n";
    //XXX
    if (rpois == 3)
    {
       rpois_string = replace_all(rpois_string, "+", "∞");
       rpois_string = replace_all(rpois_string, "green", "lightgreen");
    }
    out += rpois_string;

    const int relec = player_res_electricity(calc_unid);
    out += _resist_composer("rElec", cwidth, relec) + "\n";

    const int rcorr = you.res_corr(calc_unid);
    out += _resist_composer("rCorr", cwidth, rcorr) + "\n";

    const int rmuta = (you.rmut_from_item(calc_unid)
                       || player_mutation_level(MUT_MUTATION_RESISTANCE) == 3);
    out += _resist_composer("rMut", cwidth, rmuta) + "\n";

    const int rmagi = player_res_magic(calc_unid) / MR_PIP;
    out += _resist_composer("MR", cwidth, rmagi, 5) + "\n";

    out += _stealth_bar(get_number_of_cols()) + "\n";

    cols.add_formatted(0, out, false);

    // Second column, resist name is 9 chars
    out.clear();
    cwidth = 9;
    const int rinvi = you.can_see_invisible(calc_unid);
    out += _resist_composer("SeeInvis", cwidth, rinvi) + "\n";

    const int rclar = you.clarity(calc_unid);
    const int stasis = you.stasis(calc_unid);
    // TODO: what about different levels of anger/berserkitis?
    const bool show_angry = (you.angry(calc_unid)
                             || player_mutation_level(MUT_BERSERK))
                            && !rclar && !stasis
                            && !you.is_lifeless_undead();
    out += show_angry ? _resist_composer("Rnd*Rage", cwidth, 1, 1, false) + "\n"
                      : _resist_composer("Clarity", cwidth, rclar) + "\n";

    const int rsust = player_sust_abil(calc_unid);
    out += _resist_composer("SustAb", cwidth, rsust) + "\n";

    const int gourmand = you.gourmand(calc_unid);
    out += _resist_composer("Gourm", cwidth, gourmand, 1) + "\n";

    const int rspir = you.spirit_shield(calc_unid);
    out += _resist_composer("Spirit", cwidth, rspir) + "\n";
    const int rward = you.warding(calc_unid);
    out += _resist_composer("Warding", cwidth, rward) + "\n";

    const int notele = you.no_tele(calc_unid);
    const int rrtel = !!player_teleport(calc_unid);
    if (notele && !stasis)
        out += _resist_composer("NoTele", cwidth, 1, 1, false) + "\n";
    else if (rrtel && !stasis)
        out += _resist_composer("Rnd*Tele", cwidth, 1, 1, false) + "\n";
    else
        out += _resist_composer("Stasis", cwidth, stasis) + "\n";
    cols.add_formatted(1, out, false);

    const int no_cast = you.no_cast(calc_unid);
    if (no_cast)
    {
        out.clear();
        out += _resist_composer("NoCast", cwidth, 1, 1, false);
        cols.add_formatted(1, out, false);
    }

    _print_overview_screen_equip(cols, equip_chars, sw);

    return cols.formatted_lines();
}

// New scrollable status overview screen, including stats, mutations etc.
static char _get_overview_screen_results()
{
    bool calc_unid = false;
    formatted_scroller overview;

    overview.set_flags(MF_SINGLESELECT | MF_ALWAYS_SHOW_MORE | MF_NOWRAP);
    overview.set_more();
    overview.set_tag("resists");

    overview.add_text(_overview_screen_title(get_number_of_cols()));

    for (const formatted_string &bline : _get_overview_stats())
        overview.add_item_formatted_string(bline);
    overview.add_text(" ");

    {
        vector<char> equip_chars;
        vector<formatted_string> blines =
            _get_overview_resistances(equip_chars, calc_unid, get_number_of_cols());

        for (unsigned int i = 0; i < blines.size(); ++i)
        {
            // Kind of a hack -- we don't really care what items these
            // hotkeys go to.  So just pick the first few.
            const char hotkey = (i < equip_chars.size()) ? equip_chars[i] : 0;
            overview.add_item_formatted_string(blines[i], hotkey);
        }
    }

    overview.add_text(" ");
    overview.add_text(_status_mut_abilities(get_number_of_cols()));

    vector<MenuEntry *> results = overview.show();
    return (!results.empty()) ? results[0]->hotkeys[0] : 0;
}

string dump_overview_screen(bool full_id)
{
    string text = formatted_string::parse_string(_overview_screen_title(80));
    text += "\n";

    for (const formatted_string &bline : _get_overview_stats())
    {
        text += bline;
        text += "\n";
    }
    text += "\n";

    vector<char> equip_chars;
    for (const formatted_string &bline
            : _get_overview_resistances(equip_chars, full_id, 640))
    {
        text += bline;
        text += "\n";
    }
    text += "\n";

    text += formatted_string::parse_string(_status_mut_abilities(80));
    text += "\n";

    return text;
}

void print_overview_screen()
{
    while (true)
    {
        char c = _get_overview_screen_results();
        if (!c)
        {
            redraw_screen();
            break;
        }

        item_def& item = you.inv[letter_to_index(c)];
        describe_item(item, true);
        // loop around for another go.
    }
}

static const char* stealth_words[11] =
{
    "extremely un", "very un", "un", "fairly ", "", "quite ", "very ",
    "extremely ", "extraordinarily ", "incredibly ", "uncannily "
};

string stealth_desc(int stealth)
{
    return make_stringf("%sstealthy", stealth_words[_stealth_breakpoint(stealth)]);
}

string magic_res_adjective(int mr)
{
    if (mr == MAG_IMMUNE)
        return "immune";

    string prefix =
            (mr <  40) ? "not" :
            (mr <  80) ? "somewhat" :
            (mr < 120) ? "very" :
            (mr < 160) ? "extremely" :
            (mr < 200) ? "incredibly"
                       : "almost entirely";
    return prefix + " resistant";
}

static string _annotate_form_based(string desc, bool suppressed)
{
    if (suppressed)
        return "<darkgrey>(" + desc + ")</darkgrey>";
    else
        return desc;
}

static string _dragon_abil(string desc)
{
    const bool supp = form_changed_physiology() && you.form != TRAN_DRAGON;
    return _annotate_form_based(desc, supp);
}

// Creates rows of short descriptions for current
// status, mutations and abilities.
static string _status_mut_abilities(int sw)
{
    //----------------------------
    // print status information
    //----------------------------
    string text = "<w>@:</w> ";
    vector<string> status;

    // A hard-coded duration/status list used to be used here. This list is no
    // longer hard-coded. May 2014. -reaverb
    status_info inf;
    for (unsigned i = 0; i <= STATUS_LAST_STATUS; ++i)
    {
        if (fill_status_info(i, &inf) && !inf.short_text.empty())
            status.emplace_back(inf.short_text);
    }

    int move_cost = (player_speed() * player_movement_speed()) / 10;
    if (move_cost != 10)
    {
        const char *help = (move_cost <   8) ? "very quick" :
                           (move_cost <  10) ? "quick" :
                           (move_cost <  13) ? "slow"
                                             : "very slow";
        status.emplace_back(help);
    }

    status.push_back(magic_res_adjective(player_res_magic(false))
                     + " to hostile enchantments");

    // character evaluates their ability to sneak around:
    status.push_back(stealth_desc(check_stealth()));

    text += comma_separated_line(status.begin(), status.end(), ", ", ", ");
    text += "\n";

    //----------------------------
    // print mutation information
    //----------------------------
    text += "<w>A:</w> ";

    int AC_change  = 0;

    vector<string> mutations;

    switch (you.species)   //mv: following code shows innate abilities - if any
    {
    case SP_MERFOLK:
        mutations.push_back(_annotate_form_based("change form in water",
                                                 form_changed_physiology()));
        mutations.push_back(_annotate_form_based("swift swim",
                                                 form_changed_physiology()));
        break;

    case SP_MINOTAUR:
        mutations.push_back(_annotate_form_based("retaliatory headbutt",
                                                 !form_keeps_mutations()));
        break;

    case SP_NAGA:
        // breathe poison replaces spit poison:
        if (!player_mutation_level(MUT_BREATHE_POISON))
            mutations.emplace_back("spit poison");

        if (you.experience_level > 12)
        {
            mutations.push_back(_annotate_form_based("constrict 1",
                                                     !form_keeps_mutations()));
        }
        AC_change += you.experience_level / 3;
        break;

    case SP_GHOUL:
        mutations.emplace_back("rotting body");
        break;

    case SP_TENGU:
        if (you.experience_level > 4)
        {
            string help = "able to fly";
            if (you.experience_level > 14)
                help += " continuously";
            mutations.push_back(help);
        }
        break;

    case SP_MUMMY:
        mutations.emplace_back("no food or potions");
        mutations.emplace_back("fire vulnerability");
        if (you.experience_level > 12)
        {
            string help = "in touch with death";
            if (you.experience_level > 25)
                help = "strongly " + help;
            mutations.push_back(help);
        }
        mutations.emplace_back("restore body");
        break;

    case SP_VAMPIRE:
        mutations.emplace_back("bottle blood");
        break;

    case SP_DEEP_DWARF:
        mutations.emplace_back("damage resistance");
        mutations.emplace_back("recharge devices");
        break;

    case SP_FELID:
        mutations.emplace_back("paw claws");
        break;

    case SP_RED_DRACONIAN:
        mutations.push_back(_dragon_abil("breathe fire"));
        break;

    case SP_WHITE_DRACONIAN:
        mutations.push_back(_dragon_abil("breathe frost"));
        break;

    case SP_GREEN_DRACONIAN:
        mutations.push_back(_dragon_abil("breathe noxious fumes"));
        break;

    case SP_YELLOW_DRACONIAN:
        mutations.push_back(_dragon_abil("spit acid"));
        mutations.push_back(_annotate_form_based("acid resistance",
                                                 !form_keeps_mutations()
                                                  && you.form != TRAN_DRAGON));
        break;

    case SP_GREY_DRACONIAN:
        mutations.emplace_back("walk through water");
        AC_change += 5;
        break;

    case SP_BLACK_DRACONIAN:
        mutations.push_back(_dragon_abil("breathe lightning"));
        if (you.experience_level >= 14)
            mutations.emplace_back("able to fly continuously");
        break;

    case SP_PURPLE_DRACONIAN:
        mutations.push_back(_dragon_abil("breathe power"));
        break;

    case SP_MOTTLED_DRACONIAN:
        mutations.push_back(_dragon_abil("breathe sticky flames"));
        break;

    case SP_PALE_DRACONIAN:
        mutations.push_back(_dragon_abil("breathe steam"));
        break;

    case SP_FORMICID:
        mutations.emplace_back("permanent stasis");
        mutations.emplace_back("dig shafts and tunnels");
        mutations.emplace_back("four strong arms");
        break;

    case SP_GARGOYLE:
        if (you.experience_level >= 14)
            mutations.emplace_back("able to fly continuously");
        AC_change += 2 + you.experience_level * 2 / 5
                       + max(0, you.experience_level - 7) * 2 / 5;
        break;

#if TAG_MAJOR_VERSION == 34
    case SP_DJINNI:
        mutations.emplace_back("fire immunity");
        mutations.emplace_back("cold vulnerability");
        break;

#endif
    default:
        break;
    }                           //end switch - innate abilities

    // a bit more stuff
    if (you.species == SP_OGRE || you.species == SP_TROLL
        || player_genus(GENPC_DRACONIAN) || you.species == SP_SPRIGGAN)
    {
        mutations.emplace_back("unfitting armour");
    }

    if (player_genus(GENPC_DRACONIAN))
    {
        // The five extra points for grey draconians were handled above.
        AC_change += 4 + you.experience_level / 3;
    }

    if (you.species == SP_FELID)
    {
        mutations.emplace_back("no armour");
        mutations.emplace_back("no weapons or thrown items");
    }

    if (you.species == SP_OCTOPODE)
    {
        mutations.emplace_back("almost no armour");
        mutations.emplace_back("amphibious");
        mutations.push_back(_annotate_form_based(
            make_stringf("%d rings", you.has_tentacles(false)),
            !get_form()->slot_available(EQ_RING_EIGHT)));
        mutations.push_back(_annotate_form_based(
            make_stringf("constrict %d", you.has_tentacles(false)),
            !form_keeps_mutations()));
    }

    if (beogh_water_walk())
        mutations.emplace_back("walk on water");

    string current;
    for (unsigned i = 0; i < NUM_MUTATIONS; ++i)
    {
        if (!you.mutation[i])
            continue;

        const mutation_type mut = (mutation_type) i;
        const int level = player_mutation_level(mut);
        const bool lowered = level < you.mutation[mut];

        current = mutation_name(mut);

        if (mutation_max_levels(mut) > 1)
        {
            ostringstream ostr;
            ostr << ' ' << level;

            current += ostr.str();
        }

        if (!current.empty())
        {
            if (level == 0)
                current = "(" + current + ")";
            if (lowered)
                current = "<darkgrey>" + current + "</darkgrey>";
            mutations.push_back(current);
        }
    }

    // Statue form does not get AC benefits from scales etc.  It does
    // get changes to EV and SH.
    if (AC_change && you.form != TRAN_STATUE)
    {
        const string ac_mut = make_stringf("AC %s%d",
                                           (AC_change > 0 ? "+" : ""),
                                           AC_change);
        mutations.push_back(ac_mut);
    }

    if (mutations.empty())
        text +=  "no striking features";
    else
    {
        text += comma_separated_line(mutations.begin(), mutations.end(),
                                     ", ", ", ");
    }

    //----------------------------
    // print ability information
    //----------------------------

    text += print_abilities();

    //--------------
    // print the Orb
    //--------------
    if (player_has_orb())
        text += "\n<w>0:</w> Orb of Zot";

    //--------------
    // print runes
    //--------------
    vector<string> runes;
    for (int i = 0; i < NUM_RUNE_TYPES; i++)
        if (you.runes[i])
            runes.emplace_back(rune_type_name(i));
    if (!runes.empty())
    {
        text += make_stringf("\n<w>%s:</w> %d/%d rune%s: %s",
                    stringize_glyph(get_item_symbol(SHOW_ITEM_MISCELLANY)).c_str(),
                    (int)runes.size(), you.obtainable_runes,
                    you.obtainable_runes == 1 ? "" : "s",
                    comma_separated_line(runes.begin(), runes.end(),
                                         ", ", ", ").c_str());
    }

    linebreak_string(text, sw);

    return text;
}
