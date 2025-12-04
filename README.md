# MOTOR DESIGN USING GENETIC ALGIRHTM

This project focuses on developing an intelligent, generalized motor-design framework capable of automatically designing different classes of electric machines—specifically Brushless DC (BLDC) motors, Induction motors, and Switched Reluctance Motors (SRMs). The core of the system is a generic motor-design algorithm that rates the machines based on a given objecive function and cross-breeds the top performing motors in a generation to get new generation, this is repeated over several generations.

The algorithm accepts key performance targets—such as torque, speed, efficiency, power rating and then iteratively synthesizes an optimal motor geometry and winding configuration. By integrating analytical models, and evolutionary optimization techniques, the tool can automatically converge on feasible designs that satisfy the required specifications.

The framework is built to be modular and extensible. Ultimately, the project provides a unified platform that can design multiple motor technologies with minimal user intervention—reducing development time, enabling rapid comparison between machine types, and accelerating the process of selecting the optimal motor topology for a given application.

---

### FLOW OF DESIGN PROCESS

 ~~~mermaid
flowchart TD
    A(get motor desgin paramters, type of motor, voltage rating, power rating, rated speed)
    B(get simulatio paramters from user, no. of generations, no. of children per generation, optimizing function, cross-breeding factor) -->C
    A --> C(initialise children of the generation)
    C -->D(guess design variables depending on the type of motor, magnetic loading, current density, electric loading etc.)
    D -->E(perform design calculation based on the design variables from the set method of motor design)
    E -->F(analyse the design by conducting analytic performance estimation)
    F -->G(motors are given points based on optimization function, and sorted according to their optimization points)
    G -->I{if no. generations = required no. of generation}
    I -->|no| H(cros-breed top performing motor design variables based cross-breeding factor along with some randomly generated children. this gets us new generation of motors)
    H -->D
    I -->|yes| J(compare the best performing motors using simplifies FEM model and rank them based on the performance)
    J --> K(generate a design report of best performing motors and give it as an output to the user)
    K --> END
~~~

---

### DESIGN PROCESS OF BLDC
--to-be-filled--

### DESIGN PROCESS OF SRM
--to-be-filled--

### DESIGN PROCESS OF INDUCTION MOTOR
--to-be-filled--

---

### CROSS BREEDING PROCESS

This function is responsible for producing a new set of design variables for the next generation of motors by recombining characteristics from the best-performing designs of the current generation. After the optimization algorithm ranks all candidate motors based on objective metrics such as efficiency, torque ripple, losses, or cost, the top-performing designs are selected as “parents.”

The function then randomly samples and mixes their design variables—such as magnetic loading, electric loading, aspect ration, no. of poles-to form a new child motor configuration. For each design variable, the function selects a value from one of the parent motors with a controlled degree of randomness, ensuring adequate diversity while still inheriting high-quality traits. In cases where variables are continuous, it may interpolate or perturb the parent values; for discrete variables, it selects from the parent set probabilistically.

By combining attributes from two or more elite designs, this crossover function promotes exploration of the design space while preserving the advantageous characteristics discovered in previous generations. The generated child design is then passed into the next evaluation cycle of the optimization process, enabling evolutionary improvement of motor designs across generations.

~~~C++
for variable in motor.variables{
 variable = random_selection_between(motor_a, motor_b, random_between(variable.max, variable.min));
} 
~~~

this is a psuedo-code that represents the working of the function.

---

### SIMPLIFIES FEM MODEL
--to-be-filled--

