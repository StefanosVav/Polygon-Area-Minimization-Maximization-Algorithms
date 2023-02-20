# Assignment 3: App for comparative assessment of results from Initilization Algorithms Paired with Local Search and Simulated Annealing

Assignment by:
Stefanos Vavoulas
Nikolaos Konstas

# How to Compile and Run

# Compilation (where ... your path to CGAL-5.5):
> cmake -DCGAL_DIR=/.../CGAL-5.5
> make

# Execution:
> ./evaluate -i PointSetPath -o output.txt

Example when PointSetPath is a directory inside the assignment directory with multiple Files-PointSets:
> ./evaluate -i PointSetPath -o output.txt
 
# Comparative Assessment App
Our algorithm compares the pairs of Incremental + Local Search and Convex Hull + Local Search for each Point Set, and saves the score of the best combination of algorithms
Next, it does the same for all the combinations for Simulated annealing: Incremental + Simulated Annealing local/global and Convex Hull + Simulated annealing local/global, 
and, again, holds the best Simulated Annealing score for that Point Set 
Finally, the worst score from the Point Sets is held as min/max bound, and the scores are added to find score min and score max

In my local search algorithm I included very simple "preprocessing" for choosing the right parameters based on the images(Input) given for the assignment

# System info
Compiler: gcc 11.3.0
OS: Ubuntu 22.04.1
Developed using library CGAL

# Problems and Assumptions
We understood that the requirement of the assignment was for the evaluation algorithm to take multiple files-point sets as input,
compare and find the combination of algorithms that yields the best result for each point set, and add the score of that combination to
min score/max score respectively.

# Results: Comparisons and Comments
- For point sets of size 10 (from images): us-night-0000010.instance, euro-night-0000010.instance, london-0000010.instance:

||                Local Search                   ||               Simulated Annealing             ||
|| min score | max score | min bound | max bound || min score | max score | min bound | max bound ||
|| 0.794072  | 2.680835  | 0.342303  | 0.889423  || 0.772513  | 2.680835  | 0.342303  | 0.889423  ||

As you can observe here, our algorithms yield very good results, and the consistency is proven by the min/max bounds

- For point sets of size 20 (from images): us-night-0000020.instance, euro-night-0000020.instance, london-0000020.instance:

||                Local Search                   ||               Simulated Annealing             ||
|| min score | max score | min bound | max bound || min score | max score | min bound | max bound ||
|| 0.565356  | 2.668037  | 0.240695  | 0.875317  || 0.624233  | 2.662166  | 0.284892  | 0.859938  ||

The results get better progressively with the amount of Points in the point sets, which is to be expected since more points equal more options for changes to maximize/minimize the area

- For point sets of size 50 (from images): us-night-0000050.instance, euro-night-0000050.instance, london-0000050.instance:

||                Local Search                   ||               Simulated Annealing             ||
|| min score | max score | min bound | max bound || min score | max score | min bound | max bound ||
|| 0.592365  | 2.700495  | 0.222327  | 0.866525  || 0.719525  | 2.605051  | 0.267035  | 0.846049  ||

- For point sets of size 100 (from images): us-night-0000100.instance, euro-night-0000100.instance, london-0000100.instance: (roughly 1 minute per instance for 100 points)

||                Local Search                   ||               Simulated Annealing             ||
|| min score | max score | min bound | max bound || min score | max score | min bound | max bound ||
|| 0.432693  | 2.720078  | 0.156139  | 0.879944  || 0.644577  | 2.626106  | 0.235666  | 0.856750  ||

- For point sets of size 500 (from images): us-night-0000500.instance, euro-night-0000500.instance, paris-0000500.instance:

||                Local Search                   ||               Simulated Annealing             ||
|| min score | max score | min bound | max bound || min score | max score | min bound | max bound ||
|| 0.499514  | 2.654032  | 0.205372  | 0.855067  || 0.544341  | 2.650336  | 0.218801  | 0.853431  ||








