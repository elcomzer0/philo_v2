/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:06:05 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/16 10:07:49 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

/**
 * Initializes the data structure for the dining philosophers problem.
 *
 * @param data Pointer to the data structure to be initialized.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return 0 on success, -1 on failure.
 */
int init_data(t_data *data, int argc, char **argv)
{
    data->num_philos = atoi(argv[1]);
    data->time_to_die = atoi(argv[2]);
    data->time_to_eat = atoi(argv[3]);
    data->time_to_sleep = atoi(argv[4]);
    if (argc == 6)
    {
        data->num_meals = atoi(argv[5]);
    }
    else
    {
        data->num_meals = -1;
    }


    if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
        return -1;

    data->root = create_philosopher_tree(1, data->num_philos, data);
    if (!data->root)
    {
        pthread_mutex_destroy(&data->print_mutex);
        return -1;
    }

    data->start_time = get_timestamp();
    return 0;
}

/**
 * Creates a tree of philosopher nodes, where each node represents a philosopher.
 *
 * @param id The ID of the current philosopher node.
 * @param max_id The maximum ID of the philosophers.
 * @param data A pointer to the shared data for the philosophers.
 * @return A pointer to the created philosopher node, or NULL if an error occurred.
 */
t_philo *create_philosopher_tree(int id, int max_id, t_data *data)
{
    if (id > max_id)
        return NULL;

    t_philo *node = malloc(sizeof(t_philo));
    if (!node)
        return NULL;

    node->id = id;
    node->meals_eaten = 0;
    node->last_meal = data->start_time;
    node->data = data;
    if (pthread_mutex_init(&node->left_fork, NULL) != 0 || pthread_mutex_init(&node->right_fork, NULL) != 0)
    {
        free(node);
        return NULL;
    }

    node->left = create_philosopher_tree(2 * id, max_id, data);
    node->right = create_philosopher_tree(2 * id + 1, max_id, data);

    return node;
}

/**
 * Recursively destroys a philosopher tree by freeing the memory allocated for each philosopher.
 *
 * @param philo The root philosopher of the tree to be destroyed.
 */
void destroy_philosopher_tree(t_philo *philo)
{
    if (!philo)
        return;

    destroy_philosopher_tree(philo->left);
    destroy_philosopher_tree(philo->right);

    pthread_mutex_destroy(&philo->left_fork);
    pthread_mutex_destroy(&philo->right_fork);
    free(philo);
}

/**
 * Returns the current timestamp in milliseconds.
 *
 * This function uses the `gettimeofday()` system call to retrieve the current
 * time, and then converts the result to milliseconds by multiplying the
 * seconds by 1000 and dividing the microseconds by 1000.
 *
 * @return The current timestamp in milliseconds.
 */
long get_timestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

/**
 * Prints the status of a philosopher to the console.
 *
 * @param philo The philosopher whose status is being printed.
 * @param status The status message to be printed.
 */
void print_status(t_philo *philo, const char *status)
{
    long timestamp = get_timestamp() - philo->data->start_time;
    pthread_mutex_lock(&philo->data->print_mutex);
    printf("%ld %d %s\n", timestamp, philo->id, status);
    pthread_mutex_unlock(&philo->data->print_mutex);
}