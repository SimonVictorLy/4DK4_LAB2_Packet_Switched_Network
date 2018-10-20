
/*
 * 
 * Simulation_Run of A Single Server Queueing System
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA,
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "simparameters.h"
#include "packet_arrival.h"
#include "cleanup_memory.h"
#include "trace.h"
#include "main.h"

/******************************************************************************/

/*
 * main.c declares and creates a new simulation_run with parameters defined in
 * simparameters.h. The code creates a fifo queue and server for the single
 * server queueuing system. It then loops through the list of random number
 * generator seeds defined in simparameters.h, doing a separate simulation_run
 * run for each. To start a run, it schedules the first packet arrival
 * event. When each run is finished, output is printed on the terminal.
 */

int
main(void)
{
  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data[NUMBER_OF_SERVERS];

  /*
   * Declare and initialize our random number generator seeds defined in
   * simparameters.h
   */

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned random_seed;
  int j=0;
  int i = 0;

  /* 
   * Loop for each random number generator seed, doing a separate
   * simulation_run run for each.
   */

  while ((random_seed = RANDOM_SEEDS[j++]) != 0) {

	//Create a new simulation run.
    simulation_run = simulation_run_new(); 

    //Set the simulation_run data pointer to our data object.
    simulation_run_attach_data(simulation_run, (void *) & data); // add pointer from simulation to data

    //Initialize the simulation_run data variables, declared in main.h.   
	for (i = 0; i < NUMBER_OF_SERVERS ; i++) {
		data[i].blip_counter = 0;
		data[i].arrival_count = 0;
		data[i].number_of_packets_processed = 0;
		data[i].accumulated_delay = 0.0;
		data[i].random_seed = random_seed;

		//Create the packet buffer and transmission link, declared in main.h.
		data[i].buffer = fifoqueue_new();
		data[i].link = server_new();
	}
	/*
	Simulation_Run_Data *x0 = &data;
	Simulation_Run_Data *x1 = x0+1;
	Simulation_Run_Data *x2 = &data[0];
	Simulation_Run_Data *x3 = &data[1];
	*/
    //Set the random number generator seed for this run.
    random_generator_initialize(random_seed);

	//========================================================================
    //Schedule the initial packet arrival for the current clock time (= 0).
	//========================================================================
	for (i = 0; i < NUMBER_OF_SERVERS; i++) {
		schedule_packet_arrival_event(simulation_run, data[i].link, simulation_run_get_time(simulation_run));
	}
   // Execute events until we are finished. 
	// Assume when the number of packets are processed for one server
    while(data[0].number_of_packets_processed < RUNLENGTH) {
      simulation_run_execute_event(simulation_run);
    }

    // Output and Cleanup
    output_results(simulation_run);
    cleanup_memory(simulation_run);
  }

  getchar();   /* Pause before finishing. */
  return 0;
}