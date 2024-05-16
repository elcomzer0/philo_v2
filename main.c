/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:31 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/16 10:07:08 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

/**
 * Starts the philosopher threads by creating a new thread for each philosopher
 * and recursively starting the threads for the left and right philosophers.
 *
 * @param philo The philosopher to start the thread for.
 */
void start_philosopher_threads(t_philo *philo)
{
    if (!philo)
        return;

    pthread_create(&philo->thread, NULL, philosopher_routine, philo);
    start_philosopher_threads(philo->left);
    start_philosopher_threads(philo->right);
}

// Function to join philosopher threads
void join_philosopher_threads(t_philo *philo)
{
    if (!philo)
        return;

    pthread_join(philo->thread, NULL);
    join_philosopher_threads(philo->left);
    join_philosopher_threads(philo->right);
}

/**
 * The main entry point of the program.
 *
 * This function is responsible for:
 * - Ensuring the correct number of command-line arguments are provided
 * - Initializing the program data
 * - Creating a monitoring thread
 * - Starting the philosopher threads
 * - Joining the philosopher threads
 * - Cleaning up resources
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 * @return 0 on successful execution, 1 on error.
 */
int main(int argc, char **argv)
{
    t_data data;

    // Ensure correct number of arguments
    if (argc != 5 && argc != 6)
    {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return 1;
    }

    // Initialize data and handle initialization failure
    if (init_data(&data, argc, argv) != 0)
    {
        printf("Error initializing data.\n");
        return 1;
    }

    // Create a monitoring thread
    pthread_t monitor_thread;
    pthread_create(&monitor_thread, NULL, monitor_routine, &data);

    // Start philosopher threads
    start_philosopher_threads(data.root);

    // Join philosopher threads
    join_philosopher_threads(data.root);
    pthread_join(monitor_thread, NULL);

    // Clean up resources
    destroy_philosopher_tree(data.root);
    pthread_mutex_destroy(&data.print_mutex);

    return 0;
}