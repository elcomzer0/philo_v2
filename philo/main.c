/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:31 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/18 20:56:02 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_philosopher *allocate_philosophers(int number_of_philosophers)
{
    t_philosopher *philosophers;
    
    philosophers = (t_philosopher *)ft_calloc(number_of_philosophers, sizeof(t_philosopher));
    if (!philosophers)
    {
        write(2, "Error allocating philosophers\n", 35);
        return NULL;
    }
    return (philosophers);
}

static int initialize_mutex(pthread_mutex_t *mutex, t_data *data)
{
    int j;

    if (pthread_mutex_init(mutex, NULL) != 0)
    {
        write(2, "Error: mutex_init\n", 18);
        j = 0;
        while (j < data->number_of_philosophers)
        {
            pthread_mutex_destroy(&data->fork[j]);
            j++;
        }
        pthread_mutex_destroy(&data->print_lock);
        pthread_mutex_destroy(&data->death);
        pthread_mutex_destroy(&data->dined);
        pthread_mutex_destroy(&data->meals_eaten_mutex);
        pthread_mutex_destroy(&data->last_meal_timestamps_mutex);
        pthread_mutex_destroy(&data->dining_mutex);

        free(data->fork);
        return (-1);
    }
    return (0);
}

static void initialize_mutexes(t_philosopher *philosophers, t_data *data)
{
    int i = 0;
    (void)philosophers;
    
    data->fork = (pthread_mutex_t *)ft_calloc(data->number_of_philosophers, 
    sizeof(pthread_mutex_t));
    while (i < data->number_of_philosophers)
    {
        initialize_mutex(&data->fork[i], data);
        i++;
    }
    initialize_mutex(&data->print_lock, data);
    initialize_mutex(&data->death, data);
    initialize_mutex(&data->dined, data);
    initialize_mutex(&data->meals_eaten_mutex, data);
    initialize_mutex(&data->last_meal_timestamps_mutex, data);
    initialize_mutex(&data->dining_mutex, data);
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
    pthread_mutex_lock(&data->last_meal_timestamps_mutex);
    data->last_meal_timestamps[id - 1] = start_time;
    pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
}

void assign_forks(t_philosopher *philosopher, t_data *data)
{
    if (philosopher->id % 2 == 1)
    {
        philosopher->right_fork = &data->fork[philosopher->id - 1];
        philosopher->left_fork = &data->fork[philosopher->id % data->number_of_philosophers];
    }
    else
    {
        philosopher->right_fork = &data->fork[philosopher->id % data->number_of_philosophers];
        philosopher->left_fork = &data->fork[philosopher->id - 1];
    }
}

//keep
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

int create_monitor_threads(t_data *data)
{ 
    if (pthread_create(&(data->monitor_eat), NULL, monitor_eat, data) != 0)
    {
        write(2, "Error creating monitor thread\n", 30);
        return (-1);
    }
    if (pthread_create(&(data->monitor_death), NULL, monitor_death, data) != 0)
    {  
        write(2, "Error creating monitor thread\n", 30);
        return (-1);
    }
    return (0);
}

int create_threads(t_data *data)
{
    int i;
    int j;

    i = 0;
    while (i < data->number_of_philosophers)
    {
        if (pthread_create(&(data->philosophers[i].thread), NULL, philosopher_routine,
        &(data->philosophers[i])) != 0)
        {
            write(2, "Error creating philosopher thread\n", 31);
            j = 0;
            while (j <= i)
            {
                pthread_mutex_destroy(&(data)->fork[j]);
                j++;
            }
            pthread_mutex_destroy(&data->print_lock);
            return (-1);
        }
        usleep(100);
        i++;
    }
    if (create_monitor_threads(data) != 0)
        return (-1);
    return 0;
}

int is_valid_number(char *str)
{
    if (*str == '0' && *(str + 1) != '\0')
        return (0);
    while (*str)
    {
        if (*str < '0' || *str > '9')
            return (0);
        str++;
    }
    return (1);
}

int is_within_limits(char *str)
{
    long value;
    
    value = atol(str);
    if (value <= 0 || value > INT_MAX)
        return (0);
    return (1);
}

int validate_argument(char *arg, const char *error_message)
{
    if (!is_valid_number(arg) || !is_within_limits(arg))
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        return (1);
    }
    return (0);
}

int validate_number_of_philosophers(int number_of_philosophers)
{
    if (number_of_philosophers > MAX_PHILOSOPHERS)
    {
        write(STDERR_FILENO, "Error: number of philosophers exceeds the maximum limit\n", 56);
        return (1);
    }
    return (0);
}

int validate_times_must_eat(int times_must_eat)
{
    if (times_must_eat <= 0)
    {
        write(STDERR_FILENO, "Error: times_must_eat must be a positive integer\n", 49);
        return (1);
    }
    return (0);
}

int init_data(t_data *data, int argc, char **argv)
{
    if (validate_argument(argv[1], "Error: invalid number of philosophers\n") ||
        validate_argument(argv[2], "Error: invalid time_to_die\n") ||
        validate_argument(argv[3], "Error: invalid time_to_eat\n") ||
        validate_argument(argv[4], "Error: invalid time_to_sleep\n"))
        return (1);

    data->number_of_philosophers = atoi(argv[1]);
    if (validate_number_of_philosophers(data->number_of_philosophers))
        return (1);
    data->time_to_die = atol(argv[2]);
    data->time_to_eat = atol(argv[3]);
    data->time_to_sleep = atol(argv[4]);
    if (argc == 6)
    {
        if (validate_argument(argv[5], "Error: invalid argument for times_must_eat\n"))
            return (1);
        data->times_must_eat = atoi(argv[5]);
        if (validate_times_must_eat(data->times_must_eat))
            return (1);
    }
    else
        data->times_must_eat = -1;

    return (0);
}


int main(int argc, char **argv)
{
    t_data *data;
    
    data = ft_calloc(1, sizeof(t_data));
    if (argc != 5 && argc != 6)
    {
        write(STDERR_FILENO, "Usage: ./philosophers number_of_philosophers", 38);
        write(STDERR_FILENO, "time_to_die time_to_eat time_to_sleep [times_must_eat]\n", 37);
        return (1);
    }
    if (init_data(data, argc, argv) != 0)
    {
        write(STDERR_FILENO, "Error: invalid arguments\n", 25);
        return (1);
    }  
    initialize_philosophers(data);
    if (data->number_of_philosophers > 1)
    {
        create_threads(data);   
    }
    clean_exit(data);
    return (0);
}
