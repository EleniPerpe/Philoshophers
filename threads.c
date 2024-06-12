/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eperperi <eperperi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:49:54 by eperperi          #+#    #+#             */
/*   Updated: 2024/06/12 18:54:27 by eperperi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *routine(void *temp_philo);
void printing_move(t_data *data, int philo_id, char *string);
void eating_time(t_data *data);
void finish_program(t_data *data, t_philosopher *philo);

int threads(t_data *data)
{
	int i;
	t_philosopher *philo;

	i = 0;
	philo = data->philosophers;
	printf("Time eat : %d\n", data->time_to_eat);
	printf("Time sleep : %d\n", data->time_to_sleep);
	printf("Time die : %d\n", data->time_to_die);
	while (i < data->number_of_philo)
	{
		if (pthread_create(&(philo[i].thread_id), NULL, routine, &(philo[i])))
			return (3);
		philo[i].last_meal_time = get_time();
		i++;
	}
	finish_program(data, philo);
	return (0);
}

void *routine(void *temp_philo)
{
	t_philosopher *philo = (t_philosopher *)temp_philo;
	t_data *data = philo->data;    

	philo = (t_philosopher *)temp_philo;
	philo = data->philosophers;
	printf("Time eat : %d\n", data->time_to_eat);
	printf("Time sleep : %d\n", data->time_to_sleep);
	printf("Time die : %d\n", data->time_to_die);
	if (philo->id % 2)
		usleep(15000);
	data->dead = 0;
	int i = 0;
	while (!(data->dead) && i < 5)
	{
		eating_time(data);
		if (data->all_ate)
			break ;
		printing_move(data, philo->id, "is sleeping");
		ft_usleep(data->time_to_sleep, data);
		printing_move(data, philo->id, "is thinking");
		i++;
	}
	return (NULL);
}

void eating_time(t_data *temp_data)
{
	t_philosopher *philo;
	t_data *data;

	data = (t_data *)temp_data;
	philo = data->philosophers;
	pthread_mutex_lock(&(data->forks[philo->left_fork]));
	printing_move(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&(data->forks[philo->right_fork]));
	printing_move(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&(data->moves_check));
	printing_move(data, philo->id, "is eating");
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&(data->moves_check));
	ft_usleep(data->time_to_eat, data);
	philo->times_ate++;
	pthread_mutex_unlock(&(data->forks[philo->left_fork]));
	pthread_mutex_unlock(&(data->forks[philo->right_fork]));
}

//probably I'll remove it, I have to check if the extra control for dead philo os needed

void printing_move(t_data *data, int philo_id, char *string)
{
	pthread_mutex_lock(&(data->printing));
	if (!(data->dead))
	{
		// printf("%lli ", get_time() - data->first_timestamp);
		// printf("%i ", philo_id + 1);
		// printf("%s\n", string);
		printf("%lli %d %s\n", get_time() - data->first_timestamp, philo_id, string);
	}
	pthread_mutex_unlock(&(data->printing));

}

void finish_program(t_data *data, t_philosopher *philo)
{
	int i;
	
	i = 0;
	while (i < data->number_of_philo)
    {
        pthread_join(philo[i].thread_id, NULL);
		pthread_mutex_destroy(&(data->forks[i]));
		i++;
    }
	pthread_mutex_destroy((&data->printing));
	pthread_mutex_destroy((&data->moves_check));
	
}
