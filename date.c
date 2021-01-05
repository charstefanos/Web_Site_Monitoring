/* **************************************************************************************
   *                                                                                    *
   *   Stefanos Charalamobus, 2315792c, SP Exercise 1a                                  *
   *   This is my own work as defined in the Academic Ethics agreement I have signed.   *
   *                                                                                    *
   ************************************************************************************** */

#include "date.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct date
{
    int day, month, year;
};

int check_date(char *temp)
{

    int dd, mm, yy;
    sscanf(temp, "%d/%d/%d", &dd, &mm, &yy);

    //check year
    if (yy >= 1000 && yy <= 9999)
    {
        //check month
        if (mm >= 1 && mm <= 12)
        {
            //check days
            if ((dd >= 1 && dd <= 31) && (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12))
                return 1;

            else if ((dd >= 1 && dd <= 30) && (mm == 4 || mm == 6 || mm == 9 || mm == 11))
                return 1;

            else if ((dd >= 1 && dd <= 28) && (mm == 2))
                return 1;

            else if (dd == 29 && mm == 2 && (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0)))
                return 1;
        }
    }

    return 0;
}

/*
* date_create creates a Date structure from `datestr`
 * * `datestr' is expected to be of the form "dd/mm/yyyy"
 * * returns pointer to Date structure if successful,
 * *         NULL if not (syntax error)*/
Date *date_create(char *datestr)
{

    /* checks datestr is a valid date */
    if (check_date(datestr))
    {
        Date *date = (Date *)malloc(sizeof(Date));
        /* get day,month,year using sscanf*/
        int day, month, year;
        sscanf(datestr, "%d/%d/%d", &day, &month, &year);

        if (date != NULL)
        {
            date->day = day;
            date->month = month;
            date->year = year;
        }
        return date;
    }
    return NULL;
}
/** date_duplicate creates a duplicate of `d'
 * * returns pointer to new Date structure if successful,*
 *          NULL if not (memory allocation failure)*/
Date *date_duplicate(Date *d)
{
    Date *duplicate = (Date *)malloc(sizeof(Date));

    if (duplicate != NULL)
    {
        duplicate->day = d->day;
        duplicate->month = d->month;
        duplicate->year = d->year;
    }
    return duplicate;
}
/** date_compare compares two dates, returning <0, 0, >0 if
 * * date1<date2, date1==date2, date1>date2, respectively*/
int date_compare(Date *date1, Date *date2)
{
    if (date1->year != date2->year)
        return (date1->year < date2->year) ? -1 : 1;

    else if (date1->month != date2->month)
        return (date1->month < date2->month) ? -1 : 1;

    else if (date1->day != date2->day)
        return (date1->day < date2->day) ? -1 : 1;

    return 0;
}
/** date_destroy returns any storage associated with `d' to the system*/
void date_destroy(Date *d)
{
    free(d);
}
