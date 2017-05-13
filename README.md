# CPSC 259
Computer Science 259 at the University of British Columbia
### Maze Solver

#### Project Specifications
Note: the framework for the code was given as part of a lab for the CPSC 259 course. 

*Brief description:* Files containing symmetrical mazes of different dimensions made up by integers from 0 to 9 as the possible paths and * as barriers. The program generates all the possible paths, and determines the short and least expensive path.

*Subjects:* recursion, depth first search, mazes, pointers, memory allocation, file processing

*Language:* C

*IDE:* Microsoft Visual Studio 2012

*Detailed description:*
The mazes that the program can solve are given in the form of text files. A file can contain a singular and symmetrical (square) mazes of a certain dimension. The maze is made up of integers from 0 to 9 as the path and * as barriers. The program can determine the number of paths found, the cheapest path, the cost of the cheapest path, and the shortest path.
To begin the process, a maze file is opened. Then, the dimension of the maze is retrieved by determining the length of the first line in the file. Since the mazes are square, this is the only dimension needed. Next, space is allocated for a double pointer with the dimension of the maze, and then the elements are filled with the contents of the maze to create an array-like pointer depiction of the maze file. This allows for the maze to processed. </br></br>
The array is fed to a function that generates all the possible valid paths. This is done by [depth first search](https://www.cs.usfca.edu/~galles/visualization/DFS.html), wherein the program will search as far as possible in one direction or branch of a maze until barriers obstruct all the possible moves except for backtracking. As a path is determined, the visited array cells are marked so that the path is visible in the event of backtracking. This analogous to the purpose of Hansel and Gretel’s trail of breadcrumbs. As a path is traversed, the path - a series of integers from 0 to 9 - is stored in a pointer and is altered with the search process. </br></br>
Once an exit is found, space is added to a double pointer containing all the paths generated for that maze and the current path string is copied in. Also, a variable is incremented that counts the number of individual paths found. 
Next, the shortest path is determined by comparing the string lengths of the paths in the paths double pointer. Then, the cheapest path is found by adding the series of integers that make up a given path within all the determined paths. The shortest path and cheapest path do not have to be the same path, since the integer path numbers vary, which is why both functions are necessary.

### PageRank
#### Project Specifications
Note: no framework was given
*Brief description:* Computes the page ranks of a web of hyperlinked pages using the MATLAB engine

*Subjects:* [PageRank]( http://www.cs.princeton.edu/~chazelle/courses/BIB/pagerank.htm), linear algebra, MATLAB, [dynamic memory allocation](https://www.programiz.com/c-programming/c-dynamic-memory-allocation), [file processing](http://www.mycplus.com/tutorials/c-programming-tutorials/file-handling/), probability, [pointers](https://www.tutorialspoint.com/cprogramming/c_pointers.htm)

*Language:* C

*IDE:* Microsoft Visual Studio 2012

*Additional Links:*</br>
[Google's PageRank with MATLAB](https://www.mathworks.com/content/dam/mathworks/mathworks-dot-com/moler/exm/chapters/pagerank.pdf)</br>
[Video about the PageRank algorithm](https://www.youtube.com/watch?v=v7n7wZhHJj8&t=278s)</br>
[PageRank Analogy](http://www.bbc.co.uk/programmes/p032rf5d),</br>
[The Secret Rules of Modern Living: Algorithms](http://www.bbc.co.uk/programmes/p030s6b3) is a documentary I highly recommend. The PageRank analogy video above is a clip from the documentary. Also, the documentary outlines many other amazing algorithms like sorting algorithms and the Nobel Prize winning [Stable Marriage algorithm](https://www.youtube.com/watch?v=Qcv1IqHWAzg).

*Detailed description:*</br>
A [connectivity matrix]( https://www.google.ca/url?sa=i&rct=j&q=&esrc=s&source=images&cd=&cad=rja&uact=8&ved=0ahUKEwj1kaXbvu3TAhVU5mMKHc-YCxkQjRwIBw&url=https%3A%2F%2Fpeople.hofstra.edu%2Fgeotrans%2Feng%2Fmethods%2Fconnectivitymatrix2.html&psig=AFQjCNHOla3_CQuynqjRGiUsS7I--rZh4A&ust=1494786211026381), a web of hyperlinked pages, is provided as equal rows and columns of data within a text file. To calculate the ranks of the pages in the web, the file is opened and the dimension is determined. Since the data in the file is square, only the top dimension is required. Next, the contents of the file are copied into a two-dimension array using a double pointer to the file contents. A double pointer was not used to contain the data because the function that creates matrices for MATLAB processing requires a matrix as the input. To continue, the MATLAB engine is started. Then, the contents of the two-dimensional matrix are copied into a one-dimensional matrix, which is then used to create the connectivity matrix that is acceptable for MATLAB. Once the matrix is printed, the PageRank calculations using MATLAB begin. Although the process of using both a two- and one-dimensional matrices, the use of the two-dimensional allowed for simple printing and acted as a matrix representation of the file data which aided in the debugging process. 
</br></br>
Firstly, the matrix is placed into the MATLAB workspace and the dimensions of the matrix are determined. Then, the sum of each column of the connectivity matrix is calculated and stored in a new matrix. Then, the probability factor is initialized. This value, also know as the [damping factor]( https://www.google.ca/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&cad=rja&uact=8&ved=0ahUKEwij5en6tu3TAhVD72MKHfvaAgIQygQIKzAA&url=https%3A%2F%2Fen.wikipedia.org%2Fwiki%2FPageRank%23Damping_factor&usg=AFQjCNGpJLglaVDhCwr3vdDZ99p8fm6oYA) is around 0.85. Next, the indices of the columns whose sums are not zero are found. Continuing, the inverse of the column sum matrix is calculated. Then the [stochastic matrix]( http://mathworld.wolfram.com/StochasticMatrix.html) is determined by multiplying the connectivity and inverse column sum matrices together. </br></br>
To proceed, the zero columns of the connectivity matrix are found. Then, a matrix, Q, is generated that possesses equal dimensions and entries of the inverse of the column sums matrix. Next, the transition matrix is calculated: TransitionMatrix = p * StochasticMatrix + (1 - p) * (Q/dimension). Note: p is the probability/damping factor and Q is the inverse column sums matrix.
To get a matrix containing the page ranks, a square matrix filled with ones possessing the same dimension as the connectivity matrix is generated. Then, for 100 iterations, the transition matrix is multiplied with the matrix of ones. This essentially multiplies column vector by transition matrix by itself until values cease to change. The result is a matrix containing the page ranks. However, the values must be normalized for the data to be interpreted. This is done by dividing the matrix contents by the sum of the matrix values.

