# Path-Prediction-using-A*-Algorithm-in-C

## This code implements the A* search algorithm to find the shortest path between a start position and a goal position in a grid. The grid is represented as a two-dimensional array, where obstacles can be placed. The user is prompted to enter the dimensions of the grid, the positions of obstacles, as well as the start and goal positions.
## The A* algorithm uses the Manhattan distance heuristic to estimate the cost of reaching the goal from each node. It maintains an open list of nodes to explore, sorted by their estimated total cost. The algorithm continues until either the goal is reached or there are no more nodes to explore.
## If a path is found, it is printed on the grid, showing the obstacles, the path, and the coordinates of each node in the path. If no path is found, a corresponding message is displayed.
## The code is structured using several functions to perform specific tasks, such as creating nodes, inserting nodes into a priority queue, popping nodes from the priority queue, checking for node containment, adding obstacles to the grid, calculating the Manhattan distance, and printing the path.
## Overall, this code provides a clear and concise implementation of the A* search algorithm for pathfinding in a grid, allowing users to visualize the path and obstacles.






