          											COL216 ASSIGNMENT-1 						
RAVI SRI TEJA -2019CS10369
GATTU KARTHIK -2019CS10348

Approach used to solve the problem:

We have calculated the area bounded by the two consecutive points and summed up them to get the complete area which is required.

Detailed Procedure and Design Approach:

1)We took all the inputs from the keyboard.After running the code,the console shows a message to enter the number of points.
2)We ensured that the program exits if the input provided in the number of points is less than 2.
3)Otherwise the console shows a message to input x-coordinate followed by y-coordinate,n times where n is number of points entered earlier in the input i.e.,the total number of points for which we need to find the required area bounded by X-axis.
4)The taken input point say P1 has X and Y coordinates X1,Y1 stored in two $f registers say $f2,$f3 and next point say P2 has X and Y coordinates X2,Y2 which are stored in two $f registers say $f4,$f5.
5)Since we only need two points for computing area in each step we only store two points i.e., after each iteration the data corresponding to P2 is moved to register containing data of P1 and we calculated the area bounded by them.
6)We initialized a float register say $f1 with 0.00 for storing area which will be incremented in each iteration.
7)In the calculation of area there will be 2 cases
	Case-1:The two points are on same side of X-axis
			==>Here the area bounded will be the area of the trapezium formed by the ordinates of 2 points and line joining these 2 points and line joining the feet of ordinates on x axis which is equal to 0.5*|x2-x1|*|y1+y2|

	Case-2:The two points are on opposite sides of X-axis
			==>Here the area bounded will be sum of areas of 2 triangles which are formed by ordinates of the two points,X-axis and the line joining the given points which is given by 0.5*|x2-x1|*|y1*y1+y2*y2|/(|y1|+|y2|).

8)This calculated area is added to the total area variable that we are maintaining.
9)The content of P2 is moved to P1 
10)Further new point is taken and stored in P2 and the above procedure is repeated till we take up all the points.

						

Assumptions In Design:

1.Input is to be given in increasing order of X-coordinates.
2.Used floats to store the intermediate values so that the precision is only 32-bits.

Testing cases and Testing strategy:

1)Checked the cases which contain mixture of points which are above and below the x-axis
2)Checked the case in which the 2 points form a vertical line
3)Checked the values in the registers after running the asm file and confirmed it by the manual calculations














