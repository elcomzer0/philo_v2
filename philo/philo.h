/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:06:36 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/19 22:39:40 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>
# include <stdint.h>

# define MAX_PHILOSOPHERS 201

typedef struct s_philosopher
{
	int				id;
	long			last_meal_time;
	int				meals_eaten;
	long			time_to_die;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}					t_philosopher;

typedef struct s_data
{
	long			start_time;
	int				number_of_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				times_must_eat;
	int				death_note;
	int				dined_enough;
	long long		last_meal_timestamps[201];
	pthread_t		monitor_eat;
	pthread_t		monitor_death;
	pthread_mutex_t	*fork;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	death;
	pthread_mutex_t	dined;
	pthread_mutex_t	meals_eaten_mutex;
	pthread_mutex_t	last_meal_timestamps_mutex;
	pthread_mutex_t	dining_mutex;
	t_philosopher	*philosophers;
}					t_data;

// Function prototypes
void				*philosopher_routine(void *arg);
void				ft_usleep(unsigned long long int time_value,
						t_philosopher *philo);

// single_philosopher
void				*sokrates_case(void *arg);
void				initialize_single_philosopher_case(t_data *data);
void				create_single_philosopher_thread(t_data *data);

/* utils */
void				*ft_calloc(size_t num_elements, size_t element_size);
long				get_current_time(void);
void				print_status(t_philosopher *philo, const char *status);
void				clean_exit(t_data *data);
// bool is_death_note(t_philosopher *philo);

/*monitor*/
void				*monitor_eat(void *arg);
void				*monitor_death(void *arg);
int					dining_checker(t_philosopher *philo);

/*monitor_utils*/
bool				stop_simulation(t_data *data);
int					check_eaten_status(t_data *data);
int					death_note_check(t_philosopher *philo);
int					dined_enough_check(t_data *data);

/*actions*/
void				action_eat(t_philosopher *philo);
void				action_sleep(t_philosopher *philo);
void				action_think(t_philosopher *philo);
void				print_philosopher(t_philosopher *philosopher);
void				print_data(const t_data *data);
void				random_delay(int min_ms, int max_ms);

/*main*/
t_philosopher		*allocate_philosophers(int number_of_philosophers);
void				assign_forks(t_philosopher *philosopher, t_data *data);
int					init_data(t_data *data, int argc, char **argv);
int					main(int argc, char **argv);

/*main_init*/
int					initialize_mutex(pthread_mutex_t *mutex, t_data *data);
int					initialize_mutexes(t_philosopher *philosophers,
						t_data *data);
void				initialize_philosopher(t_philosopher *philosopher, int id,
						long start_time, t_data *data);
void				initialize_multiple_philosophers(t_philosopher *philosophers,
						t_data *data);
int					initialize_philosophers(t_data *data);

/*main_create*/
int					create_monitor_threads(t_data *data);
int					create_threads(t_data *data);

/*main_valid*/
int					is_valid_number(char *str);
int					is_within_limits(char *str);
int					validate_argument(char *arg, const char *error_message);
int					validate_number_of_philosophers(int number_of_philosophers);
int					validate_times_must_eat(int times_must_eat);

/*utils_2*/
size_t				ft_strlen(const char *str);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_atoi(const char *str);
long				ft_atol(const char *str);

/* FUNCTIONS */
void				acquire_forks(t_philosopher *philo);
void				release_forks(t_philosopher *philo);
void				update_last_meal_time(t_philosopher *philo);
void				action_think(t_philosopher *philo);
void				action_sleep(t_philosopher *philo);
void				action_eat(t_philosopher *philo);

#endif
