/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:31 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/28 01:13:49 by jorgonca         ###   ########.fr       */
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


static t_philosopher *allocate_philosophers(int number_of_philosophers)
{
    t_philosopher *philosophers = (t_philosopher *)ft_calloc(number_of_philosophers, sizeof(t_philosopher));
    if (!philosophers)
    {
        perror("Error allocating philosophers");
        exit(EXIT_FAILURE);
    }
    return philosophers;
}


static void initialize_mutex(pthread_mutex_t *mutex, t_data *data)
{
    if (pthread_mutex_init(mutex, NULL) != 0)
    {
        perror("Error initializing mutex");
        
        for (int j = 0; j < data->number_of_philosophers; j++)
        {
            pthread_mutex_destroy(&data->fork[j]);
        }
        pthread_mutex_destroy(&data->print_lock);
        pthread_mutex_destroy(&data->death);
        pthread_mutex_destroy(&data->dined);
        pthread_mutex_destroy(&data->meals_eaten_mutex);
        pthread_mutex_destroy(&data->completed_threads_mutex);
        pthread_mutex_destroy(&data->last_meal_timestamps_mutex);
        pthread_mutex_destroy(&data->fork_status_mutex);
        
        free(data->fork);
        exit(EXIT_FAILURE);
    }
}

static void initialize_mutexes(t_philosopher *philosophers, t_data *data)
{
    int i = 0;
    (void)philosophers;
    data->fork = (pthread_mutex_t *)ft_calloc(data->number_of_philosophers, sizeof(pthread_mutex_t));
    while (i < data->number_of_philosophers)
    {
        initialize_mutex(&data->fork[i], data);
        i++;
    }
    initialize_mutex(&data->print_lock, data);
    initialize_mutex(&data->death, data);
    initialize_mutex(&data->dined, data);
    initialize_mutex(&data->meals_eaten_mutex, data);
    initialize_mutex(&data->completed_threads_mutex, data);
    initialize_mutex(&data->last_meal_timestamps_mutex, data);
    initialize_mutex(&data->fork_status_mutex, data);
    initialize_mutex(&data->prioritized_mutex, data);
    
    data->completed_threads_count = 0;
}


void initialize_philosopher(t_philosopher *philosopher, int id, long start_time, t_data *data)
{
    philosopher->id = id;
    philosopher->last_meal_time = get_current_time();
    philosopher->meals_eaten = 0;
    philosopher->time_to_die = data->time_to_die;
    philosopher->data = data;
    philosopher->data->death_note = 0;
    philosopher->data->dined_enough = 0;
    philosopher->data->completed_threads_count = 0;
    philosopher->data->exiting = 0;
    philosopher->data->turn = 0;
    philosopher->starvation_counter = 0;
    philosopher->prioritize_eating = 0;

    pthread_mutex_lock(&data->last_meal_timestamps_mutex);
    data->last_meal_timestamps[id - 1] = start_time;
    pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
}


/**
 * Assigns the left and right forks to a philosopher based on their ID.
 * Philosophers with an odd ID will be assigned the fork to their left and the fork to their right.
 * Philosophers with an even ID will be assigned the fork to their right and the fork to their left.
 *
 * @param philosopher The philosopher to assign the forks to.
 * @param data The data structure containing the forks.
 */
void assign_forks(t_philosopher *philosopher, t_data *data)
{
    if (philosopher->id % 2 == 1)
    {
        philosopher->right_fork = &data->fork[philosopher->id - 1];
        philosopher->left_fork = &data->fork[philosopher->id % data->number_of_philosophers];
        philosopher->r_fork = &data->forks[philosopher->id - 1];
        philosopher->l_fork = &data->forks[philosopher->id % data->number_of_philosophers];
    }
    else
    {
        philosopher->right_fork = &data->fork[philosopher->id % data->number_of_philosophers];
        philosopher->left_fork = &data->fork[philosopher->id - 1];
        philosopher->r_fork = &data->forks[philosopher->id % data->number_of_philosophers];
        philosopher->l_fork = &data->forks[philosopher->id - 1];
    }
}


static void initialize_multiple_philosophers(t_philosopher *philosophers, t_data *data)
{
    int i;
    long start_time = get_current_time();
    
    i = 0;
    while (i < data->number_of_philosophers)
    {
        data->start_time = start_time;
        initialize_philosopher(&philosophers[i], i + 1, start_time, data);
        assign_forks(&philosophers[i], data);
        i++;
    }
}


int *init_array_forks(t_data *data)
{
    int number_of_philosophers = data->number_of_philosophers; 
        int* forks = (int*) malloc(number_of_philosophers * sizeof(int));
        if (!forks)
        {
            perror("Error allocating forks");
            exit(EXIT_FAILURE);
        }

    for (int i = 0; i < number_of_philosophers; i++)
    {
        forks[i] = 0;
    }

    return forks;
}

void initialize_philosophers(t_data *data)
{
    if (data->number_of_philosophers == 1)
    {
        initialize_single_philosopher_case(data);
        return;
    }
    t_philosopher *philosophers = allocate_philosophers(data->number_of_philosophers);
    data->philosophers = philosophers;
    if (data->number_of_philosophers > 1)
    {
       initialize_mutexes(philosophers, data);
       initialize_multiple_philosophers(philosophers, data);
    }
}



void create_threads(t_data *data)
{
    int i = 0;
    while (i < data->number_of_philosophers)
    {
        if (pthread_create(&(data->philosophers[i].thread), NULL, philosopher_routine, &(data->philosophers[i])) != 0)
        {
            perror("Error creating philosopher thread");
            for (int j = 0; j <= i; j++)
            {
                pthread_mutex_destroy(&(data)->fork[j]);
            }
            pthread_mutex_destroy(&data->print_lock);
            exit(EXIT_FAILURE);
        }
        random_delay(0, 150);
        //usleep(100);  // Slight delay to avoid timing issues
        i++;
    }
    
    // Create the monitor thread
    if (pthread_create(&(data->monitor), NULL, monitor_routine, data) != 0)
    {
        perror("Error creating monitor thread");
        exit(EXIT_FAILURE);
    }
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



int is_valid_number(char *str)
{
    if (*str == '0' && *(str + 1) != '\0')
        return 0;

    while (*str)
    {
        if (*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
}

int is_within_limits(char *str)
{
    long value = atol(str);
    if (value <= 0 || value > INT_MAX)
        return 0;
    return 1;
}

int validate_argument(char *arg, const char *error_message)
{
    if (!is_valid_number(arg) || !is_within_limits(arg))
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        return 1;
    }
    return 0;
}

int validate_number_of_philosophers(int number_of_philosophers)
{
    if (number_of_philosophers > MAX_PHILOSOPHERS)
    {
        write(STDERR_FILENO, "Error: number of philosophers exceeds the maximum limit\n", 56);
        return 1;
    }
    return 0;
}

int validate_times_must_eat(int times_must_eat)
{
    if (times_must_eat <= 0)
    {
        write(STDERR_FILENO, "Error: times_must_eat must be a positive integer\n", 49);
        return 1;
    }
    return 0;
}

int init_data(t_data *data, int argc, char **argv)
{
    if (validate_argument(argv[1], "Error: invalid number of philosophers\n") ||
        validate_argument(argv[2], "Error: invalid time_to_die\n") ||
        validate_argument(argv[3], "Error: invalid time_to_eat\n") ||
        validate_argument(argv[4], "Error: invalid time_to_sleep\n"))
        return 1;

    data->number_of_philosophers = atoi(argv[1]);
    if (validate_number_of_philosophers(data->number_of_philosophers))
        return 1;

    data->time_to_die = atol(argv[2]);
    data->time_to_eat = atol(argv[3]);
    data->time_to_sleep = atol(argv[4]);
    data->forks = init_array_forks(data);

    if (argc == 6)
    {
        if (validate_argument(argv[5], "Error: invalid argument for times_must_eat\n"))
            return 1;
        data->times_must_eat = atoi(argv[5]);
        if (validate_times_must_eat(data->times_must_eat))
            return 1;
    }
    else
        data->times_must_eat = -1;

    return 0;
}


int main(int argc, char **argv)
{
    t_data *data;
    
    data = ft_calloc(1, sizeof(t_data));
    if (argc != 5 && argc != 6)
    {
        write(STDERR_FILENO, "Usage:./philosophers number_of_philosophers time_to_die time_to_eat time_to_sleep [times_must_eat]\n", 75);
        return 1;
    }
    if (init_data(data, argc, argv) != 0)
    {
        write(STDERR_FILENO, "Error: invalid arguments\n", 25);
        return 1;
    }  
   
    initialize_philosophers(data);
    if (data->number_of_philosophers > 1)
    {
        create_threads(data);
        start_simulation(data);
        
    }
    clean_exit(data);
    return 0;
}
