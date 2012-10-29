Teng Lu
1320150551 (1320-1505-51)
tenglu@usc.edu

CSCI 561 - Foundations of Articial Intelligence - Assignment 3
Dr. Narayanaswamy
TA: Luenin Barrios and Chanin Laohaphan

=======================================
DESCRIPTION: In this assignment we are to write a program which takes in an input text file and interpret it as a 8x8 checkerboard. We should then create a game tree up to a depth specified by the file and run the minimax algorithm with alpha-beta pruning and produce a statement every step detailing what is going on. In the end we should print out the first move player A should make based on alpha and beta score.

The rules of the game:
- Each player takes a turn to move
- Jumps are prioritized over moves so if there are any jumps you must perform them even if they are not to your advantage
- Only single jumps so you cannot perform any chaining jumps
- Pieces can only move forward (A pieces move up and B pieces move down)
- Pieces become kings if they reach the end row (A pieces hit the bottom row and B pieces hit the top row)
- Kings can move forward and backward
- Players lose if:
	- There are no more pieces left
	- You cannot perform any legal moves

The heuristics score is simple:
- Each piece for player A adds 1 to the score while each piece for player B subtracts 1
- King pieces is worth 2 points instead of 1
- Infinity for a winning state for A and negative infinity for B

=======================================
TOOLS: Microsoft Visual Studio 2010, G++ (on Aludra)

=======================================
HOW TO RUN THE PROGRAM:
On Aludra, type "make" to compile the program.
Type "tenglua3" to execute the program.

=======================================
A BRIEF DESCRIPTION OF THE PROGRAM STRUCTURE + OTHERS
1. The basic program outline is this:
	a. Grab the textfile as input and store it as a board in a GameNode object.
	b. Create a game tree BFS style up to the depth specified by the input.
	c. Run Minimax with alpha-beta pruning.
	d. Read the alpha and beta score of the first node and check the score along its successors to get an optimal move.

2. I made a GameNode class to represent a single node in the game tree. Notable variables include:
	a. gameBoard, a 8x8 array that stores the current board
	b. heuristics, a value of the current board
	c. alpha and beta, the values needed for Minimax
	d. A vector of GameNode pointers for successors. I picked a vector because pruning pretty much only happens at the children in the back unless the entire node is pruned so no shifting occurs. What I mean is if there are ten successors and a child at index 5 is pruned that means every child after index 5 will be pruned as well so there will not be any shifting.
	
	The original thought was to store moves, or rather changes instead of an 8x8 array to save memory. Since differences between two boards that have a difference of one in depth is just one move this would cut down memory size. However I quickly discovered that this method doesn't work with a BFS style of traversal through the boards. It works in DFS which I'll talk about below.

3. A parent pointer was not needed. We will need to backtrack but the Minimax algorithm and the nature of its recursion does it automatically.

4. I strayed from the traditional algorithm a bit by calculating the heuristics score when I create new child nodes when creating the game tree. This is because it's extremely cheap to do it during that. As mentioned above, boards with depth difference of one only differs by one move so all I need to do is check if:
	- Whether the piece moved became a king
	- Whether the move was a jump because if so the enemy lost a piece
	- And if the enemy lost a piece if that piece was a king or not
	
	However this is not the entire heuristic checking of course. You still need to check for the two losing conditions but that's also very cheap:
		- You only check if the enemy has any pieces left if that player has lost a piece because if he hasn't the numbers haven't changed. The checking itself has a O(8x8) time but it returns false as soon as any enemy piece is encountered which makes the amortized cost low.
		- To check if the player has any move left is left for next depth because you have to find out all the moves for the players as part of the algorithm anyway so this requires no extra effort at all.

=======================================
FUTURE WORKS
Making the algorithm DFS is a lot better for both time and space complexity. Some changes:

The basic program outline would change to:
	a. Grab the textfile as input and store it as a board in a GameNode object.
	b. Run Minimax with alpha-beta pruning and construct the game tree as we go. This is better because we will not be deleting any nodes when pruning - we will simply not create it.
	d. Read the alpha and beta score of the first node and check the score along its successors to get an optimal move.
	
The time and space complexity would be pretty much the same for the first few nodes because pruning doesn't usually occur in the beginning since you still need data. The time complexity would be severely reduced because you do not need to create nodes you've pruned. The space complexity is also reduced because you do not need to keep all the nodes in memory since this is DFS natured.

Another note to make here is that you could possibly store the board state by just storing a move. We can have one board which we change as we traverse nodes. Because our algorithm is DFS in nature this makes it easy to keep track of changes. However this will incur a slight cost in time complexity so it may not be desired depending on your scenario.
	
=======================================
SOURCES:
1. http://en.wikipedia.org/wiki/Minimax
2. http://en.wikipedia.org/wiki/Alpha-beta_pruning