/*
 * pattern.c
 */

#include "./bot.h"
#include "./pattern.h"
#include <stdio.h>
#include <time.h>

int bot_default(bot_res* matches)
{
    fprintf(stdout, "%s => なるほど\n",
            matches->str);

    return 0;
}

int bot_oudon(bot_res* matches)
{
    fprintf(stdout, "%s => おうどん\n",
            matches->str);
    return 0;
}

int bot_osakana(bot_res* matches)
{
    fprintf(stdout, "%s => おさかな\n",
            matches->str);
    return 0;
}

int bot_time(bot_res* matches)
{
    time_t  t = time(NULL);

    char    date[64] = {'\0'};

    strftime(date, sizeof(date), "%H時%M分をお知らせします", localtime(&t));
    fprintf(stdout, "%s => %s\n",
            matches->str, date);

    return 0;
}

int bot_suki(bot_res* matches)
{
    int i   = 0;

    fprintf(stdout, "わーい！");
    while (i < (matches->pmatch[1].rm_eo - 2))
        putchar(matches->str[i++]);
    fprintf(stdout, "大好きー！私が見つけたんだから私のだよー！\n");

    return 0;
}

int bot_echo(bot_res* matches)
{
    int i   = 0,
        j   = 0;

    fprintf(stdout, "*** echo ***\n");
    while (i < matches->nmatch) {
        fprintf(stdout, "pos = %d, %d, str = ",
                matches->pmatch[i].rm_so, matches->pmatch[i].rm_eo);

        if (matches->pmatch[i].rm_so >= 0 && matches->pmatch[i].rm_eo >= 0) {
            j = matches->pmatch[i].rm_so;
            while (j < matches->pmatch[i].rm_eo)
                putchar(matches->str[j++]);
        }
        putchar('\n');
        i++;
    }

    return 0;
}
