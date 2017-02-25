/*
 * bot.c - The simply bot module
 *
 * Copyright (c) 2017 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./bot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

static char* progname = "bot.c";

static int regist_bot(bot_t** bot, int (*botdef)(bot_res* func), pattern_t patterns[]);
static int talk_bot(bot_t** bot, char* str);
static void release_bot(bot_t* bot);

int init_bot(bot_t** bot)
{
    bot_t*  bt  = NULL;

    if ((bt = (bot_t*)
                malloc(sizeof(bot_t))) == NULL) {
        fprintf(stderr, "%s: init_bot(): malloc() failure\n",
                progname);

        return -1;
    } else {
        bt->init = init_bot;
        bt->regist = regist_bot;
        bt->talk = talk_bot;
        bt->release = release_bot;
    }
    *bot = bt;

    return 0;
}

static int regist_bot(bot_t** bot, int (*botdef)(bot_res* matches), pattern_t patterns[])
{
    int         i       = 0,
                cnt     = 0;

    dict_t**    dict    = NULL;

    if (bot == NULL)
        return -1;

    while (patterns[cnt].regex != NULL || patterns[cnt].func != NULL) {
        if (cnt == 0)
            cnt++;
        cnt++;
    }
    if (cnt == 0)
        return -1;

    if (botdef != NULL)
        (*bot)->botdef = botdef;

    if ((dict = (dict_t**)
                malloc(sizeof(dict_t*) * (cnt + 1))) == NULL) {
        fprintf(stderr, "%s: regist_bot(): malloc() failure\n",
                progname);

        goto ERR;
    }
    while (i < cnt) {
        if ((dict[i] = (dict_t*)
                    malloc(sizeof(dict_t))) == NULL) {
            fprintf(stderr, "%s: regist_bot(): malloc() failure\n",
                    progname);

            goto ERR;

        }
        if (regcomp(&dict[i]->regex, patterns[i].regex, REG_EXTENDED) != 0) {
            fprintf(stderr, "%s: regist_bot(): regcomp(): failure\n",
                    progname);

            goto ERR;
        }
        dict[i]->func = patterns[i].func;
        i++;
    }
    dict[i] = NULL;
    (*bot)->dict = dict;

    return 0;

ERR:
    if (dict != NULL) {
        while (i >= 0) {
            if (dict[i] != NULL) {
                if (&dict[i]->regex != NULL) {
                    regfree(&dict[i]->regex);
                }
                free(dict[i]);
                dict[i] = NULL;
            }
            i--;
        }
        free(dict);
        dict = NULL;
    }

    return -2;
}

static int talk_bot(bot_t** bot, char* str)
{
    int         i                           = 0,
                (*func)(bot_res* matches)   = NULL;

    bot_res     res;

    if (bot == NULL)
        return -1;

    res.nmatch = MAX_NMATCH;
    res.str = str;
    while ((*bot)->dict[i] != NULL) {
        if (regexec(&(*bot)->dict[i]->regex,
                    str, res.nmatch, res.pmatch, 0) == 0) {
            func = (*bot)->dict[i]->func;
            break;
        }
        i++;
    }
    if (func == NULL && (*bot)->botdef != NULL)
        func = (*bot)->botdef;

    if (func != NULL)
        return func(&res);

    return PAT_NOMATCH;
}

static void release_bot(bot_t* bot)
{
    int i   = 0;

    if (bot != NULL) {
        if (bot->dict != NULL) {
            while (bot->dict[i] != NULL) {
                if (&bot->dict[i]->regex != NULL) {
                    regfree(&bot->dict[i]->regex);
                }
                free(bot->dict[i]);
                bot->dict[i] = NULL;
                i++;
            }
            free(bot->dict);
            bot->dict = NULL;
        }
        free(bot);
        bot = NULL;
    }

    return;
}
