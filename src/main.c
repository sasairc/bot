/*
 * app.c
 */

#include "./bot.h"
#include "./pattern.h"
#include <stdio.h>

int main(void)
{
    bot_t*      bot         = NULL;

    pattern_t   patterns[]  = {
        {"^(お?うどん|o?udon)$\0",      bot_oudon},
        {"^(お?さかな|o?sakana)$\0",    bot_osakana},
        {"(何時|なんじ)[？\?]?\0",        bot_time},
        {"(.+)[好す]き[？\?]\0",        bot_suki},
        {"^echo\\s(.+)",                bot_echo},
        {NULL, NULL},
    };

    if (init_bot(&bot) < 0) {
        fprintf(stderr, "init_bot() failure\n");

        return 1;
    }
    if (bot->regist(&bot, bot_default, patterns) < 0) {
        fprintf(stderr, "regist_bot() failure\n");

        return 2;
    }

    bot->talk(&bot, "おうどん");
    bot->talk(&bot, "おさかな");
    bot->talk(&bot, "今何時？");
    bot->talk(&bot, "高水準の皮を被った低水準好き？");
    bot->talk(&bot, "こうしてだらだら書くのも良いものですよ");
    bot->talk(&bot, "echo やすなちゃんはかわいいなあ！");

    bot->release(bot);

    return 0;
}
