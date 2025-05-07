# TODO

context stays its own object unreleated to dmg, 
it just gets a copy of it and tells it to start by setting the 'quit' variable 
in dmg (passed by reference). no more while loop either,
the 'loop' function is a single iteration of the loop
as the loop happens in 'main.cpp' now instead.