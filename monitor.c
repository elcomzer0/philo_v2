/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:43 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/16 10:07:24 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

/**
 * Checks the status of a philosopher and takes appropriate action if necessary.
 *
 * This function is responsible for monitoring the state of a philosopher and
 * taking action if the philosopher has been starving for too long or has
 * reached the required number of meals.
 *
 * @param philo The philosopher to check.
 * @param data The global simulation data.
 * @param all_fed A pointer to a flag indicating whether all philosophers have been fed.
 */
static void check_philosopher(t_philo *philo, t_data *data, int *all_fed)
{
    if (!philo)
        return;

    long time_since_last_meal = get_timestamp() - philo->last_meal;
    if (time_since_last_meal > data->time_to_die)
    {
        print_status(philo, "died");
        exit(1); // End the simulation if any philosopher dies
    }

    if (data->num_meals > 0 && philo->meals_eaten < data->num_meals)
    {
        *all_fed = 0;
    }

    check_philosopher(philo->left, data, all_fed);
    check_philosopher(philo->right, data, all_fed);
}

/**
 * The monitor_routine function is responsible for monitoring the state of the philosophers and determining when the simulation should end.
 * It checks the state of each philosopher and keeps track of whether all philosophers have eaten the required number of meals.
 * If all philosophers have eaten enough, the function returns to end the simulation.
 * Otherwise, it sleeps for a short period before checking the philosopher states again.
 *
 * @param arg A pointer to a t_data struct containing the data needed for the simulation.
 * @return NULL when the simulation is complete.
 */
void *monitor_routine(void *arg)
{
    t_data *data = (t_data *)arg;
    while (1)
    {
        int all_fed = 1;
        check_philosopher(data->root, data, &all_fed);

        if (data->num_meals > 0 && all_fed)
        {
            return NULL; // End the simulation when all philosophers have eaten enough times
        }

        usleep(1000); // Sleep for a short period before checking again
    }
}