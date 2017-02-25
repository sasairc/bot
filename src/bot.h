/*
 * bot.h - The simply bot module
 *
 * Copyright (c) 2017 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef BOT_H
#define BOT_H

#include <regex.h>

#define MAX_NMATCH  8
#define PAT_NOMATCH 1

typedef struct BOT_RES {
    char*       str;
    size_t      nmatch;
    regmatch_t  pmatch[MAX_NMATCH];
} bot_res;

typedef struct PATTERN_T {
    char*       regex;
    int         (*func)(bot_res* matches);
} pattern_t;

typedef struct DICT_T {
    regex_t     regex;
    int         (*func)(bot_res* matches);
} dict_t;

typedef struct BOT_T {
    dict_t**    dict;
    int         (*botdef)(bot_res* matches);
    int         (*init)(struct BOT_T** bot);
    int         (*regist)(struct BOT_T** bot, int (*botdef)(bot_res* matches), pattern_t patterns[]);
    int         (*talk)(struct BOT_T** bot, char* str);
    void        (*release)(struct BOT_T* bot);
} bot_t;

extern int init_bot(bot_t** bot);

/* BOT_H */
#endif
