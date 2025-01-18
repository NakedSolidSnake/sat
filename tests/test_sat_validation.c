#include <sat.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define CUSTOMER_NAME_SIZE          100

typedef struct 
{
    char name [CUSTOMER_NAME_SIZE];
    double credit;
    int stars;
} customer_t;

sat_status_t criteria_customer_credit (void *object);
sat_status_t criteria_customer_stars (void *object);

int main (int argc, char *argv[])
{
    sat_validation_t validation;

    sat_status_t status = sat_validation_init (&validation);
    assert (sat_status_get_result (&status) == true);

    status = sat_validation_add (&validation, criteria_customer_credit);
    assert (sat_status_get_result (&status) == true);

    status = sat_validation_add (&validation, criteria_customer_stars);
    assert (sat_status_get_result (&status) == true);

    customer_t customer_success = 
    {
        .name = "Customer success",
        .credit = 500.0,
        .stars = 4
    };

    customer_t customer_fail = 
    {
        .name = "Customer fail",
        .credit = 230.0,
        .stars = 4
    };

    status = sat_validation_verify (&validation, &customer_success);
    assert (sat_status_get_result (&status) == true);

    status = sat_validation_verify (&validation, &customer_fail);
    assert (sat_status_get_result (&status) == false);
    assert (strcmp (sat_status_get_motive (&status), "criteria customer credit error") == 0);

    return 0;
}

sat_status_t criteria_customer_credit (void *object)
{
    sat_status_t status = sat_status_set (&status, false, "criteria customer credit error");
    customer_t *customer = (customer_t *)object;

    if (customer->credit > 300.0)
        sat_status_set (&status, true, "");

    return status;
}

sat_status_t criteria_customer_stars (void *object)
{
    sat_status_t status = sat_status_set (&status, false, "criteria customer stars error");
    customer_t *customer = (customer_t *)object;

    if (customer->stars > 3)
        sat_status_set (&status, true, "");

    return status;
}