/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:31 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/17 20:48:24 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

/**
 * Handles the case where a philosopher dies.
 *
 * This function is called when a philosopher has taken a fork and is about to die.
 * It locks the left fork, prints the philosopher's status, waits for the time to die,
 * prints the philosopher's death, and then unlocks the left fork before exiting.
 *
 * @param philo The philosopher who is about to die.
 */
static void *sokrates_case(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    
    pthread_mutex_lock(&philo->left_fork);
    print_status(philo, "has taken a fork");
    ft_usleep(philo->data->time_to_die * 1000);
    print_status(philo, "died");
    pthread_mutex_unlock(&philo->left_fork);
    exit(EXIT_SUCCESS);
}


/**
 * Initializes the philosophers in the dining philosophers problem.
 *
 * This function allocates memory for the philosophers, sets their initial state,
 * and links them together in a circular list. If there is only one philosopher,
 * a special case is handled.
 *
 * @param data A pointer to the t_data struct containing the number of philosophers.
 */
void initialize_philosophers(t_data *data)
{
    t_philosopher *philosophers = ft_calloc(data->number_of_philosophers, sizeof(t_philosopher));
    if (!philosophers)
        exit(EXIT_FAILURE);
    
    //initialize the print_lock mutex
    if (pthread_mutex_init(&data->print_lock, NULL)!= 0)
    {
        write(STDERR_FILENO, "Error initializing print_lock mutex\n", 34);
        exit(EXIT_FAILURE);
    }

    if (data->number_of_philosophers == 1)
    {
        philosophers[0].id = 1;
        philosophers[0].last_meal_time = get_current_time();
        philosophers[0].meals_eaten = 0;
        pthread_mutex_init(&philosophers[0].left_fork, NULL);
        philosophers[0].right_fork = NULL;
        philosophers[0].data = data;
        philosophers[0].next = NULL;
        philosophers[0].prev = NULL;
        data->philosophers = philosophers;
        pthread_create(&philosophers[0].thread, NULL, sokrates_case, &philosophers[0]);
        return;
    }

    int i = 0;
    while (i < data->number_of_philosophers)
    {
        philosophers[i].id = i + 1;
        philosophers[i].last_meal_time = get_current_time();
        philosophers[i].meals_eaten = 0;
        pthread_mutex_init(&philosophers[i].left_fork, NULL);

        if (i == data->number_of_philosophers - 1)
            philosophers[i].right_fork = &philosophers[0].left_fork;
        else
            philosophers[i].right_fork = &philosophers[i + 1].left_fork;

        philosophers[i].data = data;
        philosophers[i].next = &philosophers[(i + 1) % data->number_of_philosophers];
        philosophers[i].prev = &philosophers[(i - 1 + data->number_of_philosophers) % data->number_of_philosophers];
        i++;
    }
    data->philosophers = philosophers;
}

int init_data(t_data *data, int argc, char **argv)
{
    data->number_of_philosophers = atoi(argv[1]);
    data->time_to_die = atol(argv[2]);
    data->time_to_eat = atol(argv[3]);
    data->time_to_sleep = atol(argv[4]);
    if (argc == 6)
    {
        data->times_must_eat = atoi(argv[5]);
        if (data->times_must_eat < 0)
        {
            printf("Error: times_must_eat must be a positive integer\n");
            return (1);
        }
    }
    return (0);
}



int main(int argc, char **argv)
{
    t_data data;
    if (argc != 5 && argc != 6)
    {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [times_must_eat]\n", argv[0]);
        return 1;
    }
    if (init_data(&data, argc, argv) != 0)
    {
        printf("Error: invalid arguments\n");
        return 1;
    }  
    initialize_philosophers(&data);
    //check if the simulation should be started if there is more than one philosopher
    if (data.number_of_philosophers > 1)
        start_simulation(&data);
    //free(data.philosophers);
    clean_exit(&data);
    return 0;
}

