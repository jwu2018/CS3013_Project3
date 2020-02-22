README
CS 3013 Project 3
Jeffrey Huang and Jyalu Wu

In this project, we could use either semaphores or mutex locks
and condition variables for each part; however, if we used
semaphores for one, we could not use sempahores for the other.

----------------------------------------------------------------
Phase 1: cats_dogs.c
----------------------------------------------------------------
Functions:						Structs:
	main							args
	cat
	dog

In this phase, we created a simulation with 13 cats, 9 dogs, and 2
refillable water bowls in the kitchen. The pets want to drink
water at random intervals for random amounts of time, but the
two types can't go into the kitchen together i.e. only one or two
pets of the same type are allowed in the kitchen at a time.

We decided to use semaphores for this phase because they would
give greater control over the "refilling bowls" part. Since you
can easily customize the value of a semaphore to reflect the
limit, this approach seemed more straightforward than using locks
and we were able to control the number of pets in the kitchen
more easily.

----------------------------------------------------------------
Phase 2: study.c
----------------------------------------------------------------
Functions:						Structs:
	main							list
	grad_specs						grad_specs
	make_random_list
	
In this phase, we created a simulation with 4 grad students in a lab
with 4 stations. Each student is given a bucket of random objects.
Each time they pick up an object, they will recieve a list of at
most 4 instructions in a random order that can be any of the
following: soak it at station 1, shock it at station 2, squeeze 
it at station 3, and scorch it at station 4. The instructions will
not have repeats. No two students can be at the same station 
twice.

An error that we are not sure how to resolve is that sometimes,
the program will stop after printing 5-10 lines and show the message:


	study: malloc.c:2394: sysmalloc: Assertion `(old_top == initial_top
	(av) && old_size == 0) || ((unsigned long) (old_size) >= MINSIZE &&
	prev_inuse (old_top) && ((unsigned long) old_end & (pagesize - 1))
	== 0)' failed.
	I need to go to stations Aborted (core dumped)


or something similar to it. If you run ./study again it should work
and the error message will disappear.

To test this phase, we wrote a bash file. It runs the simulation
10 times and stores the output in the file study_output.txt. In
order to run the bash file, use the command "sh test_file.sh".
