# personal_projects

This repository contains 4 of my major projects: 

1. An academic review of the paper Approximating Graph TSP by Matchings by Mönke and Svensson. The paper is concerned with cutting edge techniques used in the graphic variant of a well known problem called the Traveling Salesman Problem. The purpose of my review is to provide an introduction and present key ideas from the paper. 

2. A neural network written in python. It is written from scratch using python and NumPy, with the exception of sklearn and matplotlib for data pre-processing and visualization, respectively. The model provides control over many neural network hyperparameters such as: learning rate, regularization, batch size used in gradient descent, number of epochs, as well as the shape and activation function. I implemented the gradient calculation using my own take on the math, and will be adding a detailed explanation soon. A toy example of how to work with the neural network is shown in modeltests.ipynb. Download miniconda and follow the instructions in the file maketensorflowenv for a suitable python environment. 

3. A shell and process scheduler written in C. Done as an assignment for the course comp 310 (operating systems) at McGill University. The shell simulates system memory using an array, and the processes to be executed are shell scripts. Sample shell scripts are contained in the files named "prog". The shell is capable of executing basic commands such as echo and setting environment variables. The scheduler is capable of executing up to 3 processes at once using round robin scheduling. Pages are loaded into system memory dynamically from a program store, and the least recently used page gets evicted from memory if the memory is full.

Compile using "make clean; make mysh framesize=18 varmemsize=10". Further details are given by the help command.
   
4. A database system that can be used by healthcare practitioners, coming soon.
