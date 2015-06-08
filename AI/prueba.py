import fuzzy.storage.fcl.Reader

system = fuzzy.storage.fcl.Reader.Reader().load_from_file("naves.fcl")
 
# preallocate input and output values
my_input = {
        "Our_Health" : 0.0,
        "Enemy_Health" : 0.0
        }
my_output = {
        "Aggressiveness" : 0.0
        }

# set input values
my_input["Our_Health"] = 99
my_input["Enemy_Health"] = 1
 
# calculate

system.calculate(my_input, my_output)

 
# now use outputs
print my_output

