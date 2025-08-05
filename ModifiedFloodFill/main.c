#include <stdio.h>
#include <stdbool.h>
#include "API.h"

typedef struct manhattanDistances
{
    int coordDist[5][5];
}mDist;

typedef struct wallCoordinates
{
    int hWalls[6][5];
    int vWalls[5][6];

}wallCoords;

typedef struct cellInformation
{
    char *cellCardinal;
    int cellCoordsY;
    int cellCoordsX;
    int cellValue;
}cellInfo;

typedef struct circularArrayData
{
    #define	SIZE_OF_ARRAY 18	// Maximum size of buffer
    int	read_Index;	    	    // Index of the read pointer should be set to 0
    int	write_Index;	   	    // Index of the write pointer should be set to 0
    int	array_Length;		    // Number of values in circular buffer should be set to 0
}cArrayData;


cellInfo botPosition;
cellInfo endPosition;

wallCoords wallData;

/*//mDist cellDistances = {4,3,2,3,4,
                       3,2,1,2,3,
                       2,1,0,1,2,
                       3,2,1,2,3,
                       4,3,2,3,4};//variable for manhattan distances*/


cellInfo flood_CArray[30] = { 0 };


void log(char * text) {
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}

void ilog(int * text) {
    fprintf(stderr, "%d\n", text);
    fflush(stderr);
}

//this is update walls function

/*need to update the variables to the struct ones*/
void checkWalls(void)
{

    int x = botPosition.cellCoordsX;
    int y = botPosition.cellCoordsY;

    printf("X %d ", x);
    printf("Y %d ", y);
    printf("direction %s\n", botPosition.cellCardinal);



    char *currentDirection = botPosition.cellCardinal;

    printf("direction 2 %s\n", currentDirection);
    switch (*currentDirection)
    {
        case 'N':
            if (API_wallFront() == true)
            {
                //printf("front wall: %d\n",wallData.hWalls[y][x]);//north
                wallData.hWalls[y][x] = 1;

/*                fprintf(stderr, "Y:%d X:%d\n", y,x);
                fprintf(stderr, "North value %d\n", wallData.hWalls[y][x]);
                fflush(stderr);*/

                log("North wall blocked\n\n");
            }
            if (API_wallRight() == true)
            {
                //printf("right wall: %d\n",API_wallRight());//east
                wallData.vWalls[y][x+1] = 1;

/*                fprintf(stderr, "Y:%d X:%d\n", y,x);
                fprintf(stderr, "East value %d\n",  wallData.vWalls[y][x+1]);
                log("East wall blocked\n\n");

                fflush(stderr);*/
            }
            if (API_wallLeft() == true)
            {
                wallData.vWalls[y][x] = 1;//west

     /*           fprintf(stderr, "Y:%d X:%d\n", y,x);
                fprintf(stderr, "West value %d\n",  wallData.vWalls[y][x]);
                log("West wall blocked\n\n");


                fflush(stderr);*/
            }
            break;
        case 'E':
            if (API_wallFront() == true)
            {
                wallData.vWalls[y][x+1] = 1;//east

  /*              fprintf(stderr, "Y:%d X:%d\n", y,x);
                fprintf(stderr, "East value %d\n",  wallData.vWalls[y][x+1]);
                log("East wall blocked\n\n");*/


                fflush(stderr);
            }

            if (API_wallRight() == true)
            {
                wallData.hWalls[y+1][x] = 1;//south
    /*            fprintf(stderr, "Y:%d X:%d\n", y,x);
                fprintf(stderr, "South value %d\n", wallData.hWalls[y+1][x]);
                log("South wall blocked\n\n");*/


                fflush(stderr);
            }
            if (API_wallLeft() == 1)
            {
                wallData.hWalls[y][x] = 1;//north

           /*     fprintf(stderr, "Y:%d X:%d\n", y,x);
                fprintf(stderr, "North value %d\n", wallData.hWalls[y+1][x]);
                log("North wall blocked\n\n");
*/

                fflush(stderr);
            }
            break;
        case 'W':
            if (API_wallFront() == true)
            {
                wallData.vWalls[y][x] = 1;//west

    /*            fprintf(stderr, "Y:%d X:%d\n", y,x);
                fprintf(stderr, "West value %d\n",  wallData.vWalls[y][x]);
                log("West wall blocked\n\n");
*/
            }

            if (API_wallRight() == true)
            {
                wallData.hWalls[y][x] = 1;//north

  /*              fprintf(stderr, "Y:%d X:%d\n", y,x);
                fprintf(stderr, "North value %d\n", wallData.hWalls[y+1][x]);
                log("North wall blocked\n\n");*/
            }
            if (API_wallLeft() == true)
            {
                wallData.hWalls[y+1][x] = 1;//south

//                fprintf(stderr, "Y:%d X:%d\n", y,x);
//                fprintf(stderr, "South value %d\n", wallData.hWalls[y+1][x]);
//                log("South wall blocked\n\n");

            }
            break;
        case 'S':
            if (API_wallFront() == true)
            {
                wallData.hWalls[y+1][x] = 1;//south

/*                fprintf(stderr, "Y:%d X:%d\n", y,x);
                fprintf(stderr, "South value %d\n", wallData.hWalls[y+1][x]);
                log("South wall blocked\n\n");*/

            }

            if (API_wallRight() == true)
            {
                wallData.vWalls[y][x] = 1;//west
/*                fprintf(stderr, "Y:%d X:%d\n", y,x);
                fprintf(stderr, "West value %d\n",  wallData.vWalls[y][x]);
                log("West wall blocked\n\n");*/
            }
            if (API_wallLeft() == true)
            {
                wallData.vWalls[y][x+1] = 1;//east
/*                fprintf(stderr, "Y:%d X:%d\n", y,x);
                fprintf(stderr, "East value %d\n",  wallData.vWalls[y][x+1]);
                log("East wall blocked\n\n");*/
            }
            break;
        default:
            printf("Check walls went wrong");
            break;
    }

}


/*
I've just realised that instead of the tail variable i could've used array_length instead,
too late to change now, if i have spare time i will. By doing so it will improve readability
*/

void Cell_Sort(cArrayData *sortArrayData, cellInfo *sortArray)
{
    cellInfo temp;//Initialising temp struct to store whilst sorting

    int i, j, l;


    i = sortArrayData->read_Index;
    l = sortArrayData->array_Length;
    /*i = *head;//set first position item as i
    l = *tail;//set the last position item as l*/
    //l -= 1;//l needs to be one increment less because the tail is pointing at empty space

    //Nested for loop re-orders struct array in the smallest to largest
    //I begins at the start number, j starts on the second number, this loops until all numbers are sorted

    for (; i < l - 1; i++)
    {
        for (j = 0 ; j < l - i - 1 ; j++)
        {
            //If the current value is bigger than the next value
            //make the current value the next one
            if (sortArray[j].cellValue > sortArray[j+1].cellValue)
            {
                temp = sortArray[j];
                sortArray[j] = sortArray[j+1];
                sortArray[j+1] = temp;
            }
        }
    }

/*        for (; i < l - 1; i++)
        {
            for (j = i + 1; j < l; j++)
            {
                //If the current value is bigger than the next value
                //make the current value the next one
                if (sortArray[i].cellValue > sortArray[j].cellValue)
                {
                    temp = sortArray[i];
                    sortArray[i] = sortArray[j];
                    sortArray[j] = temp;
                }
            }
        }*/

    //This isn't coded for the edge case of the circular array looping back around
}

void Push_Var(cellInfo cellData, cArrayData *arrayData, cellInfo *pushArray)
{
    if (arrayData->array_Length == SIZE_OF_ARRAY) {
        printf("\n    Buffer is full!\n\n    ");
        log("\n    Buffer is full!\n\n    ");
    }

    pushArray[arrayData->write_Index] = cellData;	 // Write cellData to address of buffer index


    arrayData->array_Length++;	 //	Increase buffer size after writing
    arrayData->write_Index++;	 //	Increase writeIndex position to prepare for next write

    //printf("%d\n", arrayData->array_Length);

    // If at last index in buffer, set writeIndex back to 0
    if (arrayData->write_Index == SIZE_OF_ARRAY)
    {
        arrayData->write_Index = 0;
    }
}

cellInfo Pop_Var(int arraySize, cArrayData *arrayData, cellInfo *popArray)
{
    if (arrayData->array_Length == 0)
    {
        printf("\n    Buffer is empty!\n\n    ");
        log("\n    Buffer is empty!\n\n    ");
    }

  /*  printf("\n    The output value is %d\n\n    ", popArray[arrayData->read_Index]);*/

    cellInfo cellData =  popArray[arrayData->read_Index];



    arrayData->array_Length--;	 //	Decrease buffer size after reading
    arrayData->read_Index++;	 //	Increase readIndex position to prepare for next read

    // If at last index in buffer, set readIndex back to 0
    if (arrayData->read_Index == arraySize)
    {
        arrayData->read_Index = 0;
    }

    return cellData;
}




/* [Note to self] 22/02/23 J.KO

 When checking if walls are blocked, potentially check e.g.
 wallNorth instead of the accessing the wall structure

*/

//Check accessible neighbours

cArrayData checkNeighbours(const int *cellCoordinateY, const int *cellCoordinateX, cellInfo *checkArray, mDist *cellDistances)
{

    int x = *cellCoordinateX;
    int y = *cellCoordinateY;


    printf("X:%d", x);
    printf("Y:%d\n", y);

    cArrayData neighbourArrayData;

    neighbourArrayData.read_Index	    =	0;	// Index of the read pointer
    neighbourArrayData.write_Index	    =	0;	// Index of the write pointer
    neighbourArrayData.array_Length	    =   0;	// Number of values in circular buffer



    if (wallData.hWalls[y][x] == 0)//if north wall is not blocked
    {
        cellInfo northCell;//Initialise north cell data

        northCell.cellCoordsX = x;     //set the array element for the x coordinate
        northCell.cellCoordsY = y - 1; //set the array element for the y coordinate
        northCell.cellValue = cellDistances->coordDist[y-1][x];//set the manhattan distance for the north cell
        northCell.cellCardinal = "N";

        Push_Var(northCell ,&neighbourArrayData, checkArray);

/*        printf("northCell X %d", northCell.cellCoordsX);
        printf(" northCell y %d", northCell.cellCoordsY);
        printf(" northCell card %s\n", northCell.cellCardinal);*/


    /*    printf("1checkcell X %d", checkArray[0].cellCoordsX);
        printf(" 1checkcell y %d", checkArray[0].cellCoordsX);
        printf(" 1checkcell card %s\n", checkArray[0].cellCardinal);*/
        printf("North %d\n", wallData.hWalls[x][y]);
    }



    if (wallData.vWalls[y][x+1] == 0)//if east wall is not blocked
    {
        cellInfo eastCell;

        eastCell.cellCoordsX = x + 1; //set the array element for the x coordinate
        eastCell.cellCoordsY = y;     //set the array element for the y coordinate
        eastCell.cellValue = cellDistances->coordDist[y][x+1];
        eastCell.cellCardinal = "E";

        Push_Var(eastCell ,&neighbourArrayData, checkArray);
        printf("East %d\n", wallData.vWalls[y][x+1]);
    }



    if (wallData.hWalls[y+1][x] == 0)//if south wall is not blocked
    {
        cellInfo southCell;

        southCell.cellCoordsX = x;     //set the array element for the x coordinate
        southCell.cellCoordsY = y + 1; //set the array element for the y coordinate
        southCell.cellValue = cellDistances->coordDist[y+1][x];
        southCell.cellCardinal = "S";

        Push_Var(southCell ,&neighbourArrayData, checkArray);
        printf("South %d\n",wallData.hWalls[y+1][x]);
    }


    if (wallData.vWalls[y][x] == 0)//if west wall is not blocked
    {
        cellInfo westCell;

        westCell.cellCoordsX = x - 1;   //set the array element for the x coordinate
        westCell.cellCoordsY = y; //set the array element for the y coordinate
        westCell.cellValue = cellDistances->coordDist[y][x-1];
        westCell.cellCardinal = "W";

        Push_Var(westCell ,&neighbourArrayData, checkArray);

        printf("West %d\n",wallData.vWalls[y][x]);
    }



    Cell_Sort(&neighbourArrayData,
               checkArray);

/*    printf("checkcell X %d", checkArray[0].cellCoordsX);
    printf(" checkcell y %d", checkArray[0].cellCoordsY);
    printf(" checkcell card %s\n", checkArray[0].cellCardinal);*/

    return neighbourArrayData;
}


void Flood_Fill(mDist *cellDistances)
{


    cellInfo cNeighbourArray[20] = { 0 };


    cArrayData floodArrayData;

    cellInfo minimumCell;

    floodArrayData.read_Index	    =	0;	// Index of the read pointer
    floodArrayData.write_Index	    =	0;	// Index of the write pointer
    floodArrayData.array_Length	    =   0;	// Number of values in circular buffer


    Push_Var(botPosition, &floodArrayData,  flood_CArray);

    while(floodArrayData.array_Length != 0)
    {



        //Pop the variable from the circular array and set it as cell_To_Check
        cellInfo cell_To_Check = Pop_Var(SIZE_OF_ARRAY, &floodArrayData, flood_CArray);



        int cellCheckY = cell_To_Check.cellCoordsY;//set the Y coordinate of the checked cell
        int cellCheckX = cell_To_Check.cellCoordsX;//set the X coordinate of the checked cell

        int cellCheckValue = cellDistances->coordDist[cellCheckY][cellCheckX];//manhattan value of the cell of interest

        cArrayData cNeighbourArrayData;

        cNeighbourArrayData = checkNeighbours(&cellCheckY, &cellCheckX, cNeighbourArray, cellDistances);

        int cNeighbourArrayLength = cNeighbourArrayData.array_Length;

        minimumCell = *cNeighbourArray;



        if (cellCheckValue <= minimumCell.cellValue)
        {
            //This corrects the cell to be the correct floodfill number
            cellDistances->coordDist[cellCheckY][cellCheckX] = 1 + minimumCell.cellValue;

            fprintf(stderr, "Y:%d X:%d\n", cellCheckY,cellCheckX);
            fprintf(stderr, "Manhattan value %d\n",  cellDistances->coordDist[cellCheckY][cellCheckX]);


            printf("min val %d\n", cellDistances->coordDist[cellCheckY][cellCheckX]);
            //This loops and pushes all cardinal cells onto the stack to be checked

            for(int i = 0; i < cNeighbourArrayLength; i++)
            {
                printf("stackX: %d", cNeighbourArray[i].cellCoordsX);
                printf("stackY: %d\n", cNeighbourArray[i].cellCoordsY);


                Push_Var(cNeighbourArray[i],  &floodArrayData,  flood_CArray);
            }
        }

    }

/*    printf("mincell X %d", minimumCell.cellCoordsX);
    printf(" mincell y %d", minimumCell.cellCoordsY);
    printf(" mincell card %s\n", minimumCell.cellCardinal);*/


    //Code to move to lowest distance
    //robot moves to cNeighbourArray[0].cellCoords[0] (y value), cNeighbourArray[0].cellCoords[1]

}


//This function assigns values based on the direction the bot is facing

/* log 1: this function is completely useless*
 *
 * log 2:i can repurpose this function as directional movement
 */
void Cardinal_Value_Labelling(cellInfo *cellData)
{




    printf("card X %d", cellData->cellCoordsX);
    printf(" card y %d", cellData->cellCoordsY);
    printf(" card card %s\n", cellData->cellCardinal);

    char *cellDirection = cellData->cellCardinal;

    char *currentDirection = botPosition.cellCardinal;


    printf("Cell Direction %s ", cellDirection);
    printf("Current Direction %s\n", currentDirection);



    switch(*currentDirection)
    {
        //This assigns values for which direction the bot is facing
        //e.g. facing north starting from 0 N=0, E=1, S=2, W=3.
        case 'N':

            if ('N' == *cellDirection)
            {
                API_moveForward();
                botPosition.cellCoordsY -=1;
                botPosition.cellCardinal = "N";
                printf("bot Y %s ", botPosition.cellCardinal);


            }
            else if ('E' == *cellDirection)
            {
                API_turnRight();
                API_moveForward();
                botPosition.cellCoordsX +=1;
                botPosition.cellCardinal = "E";


            }
            else if ('W' == *cellDirection)
            {
                API_turnLeft();
                API_moveForward();
                botPosition.cellCoordsX -=1;
                botPosition.cellCardinal = "W";



            }
            else if ('S' == *cellDirection)
            {
                API_turnRight();
                API_turnRight();
                API_moveForward();

                botPosition.cellCoordsY +=1;
                botPosition.cellCardinal = "S";
                //Turn 180 degrees

            }
            break;

        case 'E':
            if ('N' == *cellDirection)
            {
                API_turnLeft();
                API_moveForward();
                botPosition.cellCoordsY -=1;
                botPosition.cellCardinal = "N";

            }
            else if ('E' == *cellDirection)
            {

                API_moveForward();
                botPosition.cellCoordsX +=1;
                botPosition.cellCardinal = "E";


            }
            else if ('W' == *cellDirection)
            {
                API_turnRight();
                API_turnRight();
                API_moveForward();
                botPosition.cellCoordsX -=1;
                botPosition.cellCardinal = "W";

            } else if ('S' == *cellDirection)
            {
                API_turnRight();
                API_moveForward();
                botPosition.cellCoordsY +=1;
                botPosition.cellCardinal = "S";


            }
            break;

        case 'S':
            if ('N' == *cellDirection)
            {
                API_turnRight();
                API_turnRight();
                API_moveForward();
                botPosition.cellCoordsY -=1;
                botPosition.cellCardinal = "N";


            } else if ('E' == *cellDirection)
            {
                API_turnLeft();
                API_moveForward();
                botPosition.cellCoordsX +=1;
                botPosition.cellCardinal = "E";



            } else if ('W' == *cellDirection)
            {
                API_turnRight();
                API_moveForward();
                botPosition.cellCoordsX -=1;
                botPosition.cellCardinal = "W";


            } else if ('S' == *cellDirection)
            {
                API_moveForward();
                botPosition.cellCoordsY +=1;
                botPosition.cellCardinal = "S";


            }
            break;

        case 'W':
            if ('N' == *cellDirection)
            {
                API_turnRight();
                API_moveForward();

                botPosition.cellCoordsY -=1;
                botPosition.cellCardinal = "N";

            } else if ('E' == *cellDirection)
            {
                API_turnRight();
                API_turnRight();
                API_moveForward();
                botPosition.cellCoordsX +=1;
                botPosition.cellCardinal = "E";


            } else if ('W' == *cellDirection)
            {

                API_moveForward();
                botPosition.cellCoordsX -=1;
                botPosition.cellCardinal = "W";

            } else if ('S' == *cellDirection)
            {
                API_turnLeft();
                API_moveForward();
                botPosition.cellCoordsY +=1;
                botPosition.cellCardinal = "S";

            }
            break;

        default:
            printf("Error Case 1!");
            break;
    }
}

void Run_Algo(mDist *cellDistances)
{
    cellInfo cNeighbourArray[20] = { 0 };


    cArrayData floodArrayData;

    cellInfo minimumCell;

    floodArrayData.read_Index	    =	0;	// Index of the read pointer
    floodArrayData.write_Index	    =	0;	// Index of the write pointer
    floodArrayData.array_Length	    =   0;	// Number of values in circular buffer


    Push_Var(botPosition, &floodArrayData,  flood_CArray);

    while(floodArrayData.array_Length != 0)
    {


        //Pop the variable from the circular array and set it as cell_To_Check
        cellInfo cell_To_Check = Pop_Var(SIZE_OF_ARRAY, &floodArrayData, flood_CArray);



        int cellCheckY = botPosition.cellCoordsY;//set the Y coordinate of the checked cell
        int cellCheckX = botPosition.cellCoordsY;//set the X coordinate of the checked cell

        int cellCheckValue = cellDistances->coordDist[cellCheckY][cellCheckX];//manhattan value of the cell of interest

        cArrayData cNeighbourArrayData;

        cNeighbourArrayData = checkNeighbours(&cellCheckY, &cellCheckX, cNeighbourArray, cellDistances);

        int cNeighbourArrayLength = cNeighbourArrayData.array_Length;

        minimumCell = *cNeighbourArray;






        printf("min val %d\n", cellDistances->coordDist[cellCheckY][cellCheckX]);
        //This loops and pushes all cardinal cells onto the stack to be checked

        for(int i = 0; i < cNeighbourArrayLength; i++)
        {
            printf("stackX: %d", cNeighbourArray[i].cellCoordsX);
            printf("stackY: %d\n", cNeighbourArray[i].cellCoordsY);


            Push_Var(cNeighbourArray[i],  &floodArrayData,  flood_CArray);
        }


    }
}
void Solve_Maze(mDist *cellDistances)
{
    while ((botPosition.cellCoordsY != endPosition.cellCoordsY) || (botPosition.cellCoordsX != endPosition.cellCoordsX))
    {


        cellInfo botNeighbourArray[20] = {0};

        cellInfo minCell;

        checkWalls();

        Flood_Fill(cellDistances);


        checkNeighbours(&botPosition.cellCoordsY, &botPosition.cellCoordsX, botNeighbourArray, cellDistances);

        minCell = botNeighbourArray[0];

        Cardinal_Value_Labelling(&minCell);

        //Now move to lowest distance e.g. minimumCell
    }

}



void Run_Maze(mDist *cellDistances)
{
    while ((botPosition.cellCoordsY != endPosition.cellCoordsY) ||  (botPosition.cellCoordsX != endPosition.cellCoordsX))
    {


        cellInfo botNeighbourArray[20] = { 0 };

        cellInfo minCell;

        //Run_Algo(cellDistances);


        checkNeighbours(&botPosition.cellCoordsY, &botPosition.cellCoordsX, botNeighbourArray, cellDistances);

        minCell = botNeighbourArray[0];

        Cardinal_Value_Labelling(&minCell);

        //Now move to lowest distance e.g. minimumCell
    }
}


int main()
{
    //Initialise end position
    endPosition.cellCoordsY = 2;
    endPosition.cellCoordsX = 2;
    //Initialise start position
    botPosition.cellCoordsY = 4;
    botPosition.cellCoordsX = 0;
    botPosition.cellCardinal = "N";

//set manhattan distances to 1



    for (int j = 0; j < 5; j++)
    {

        wallData.vWalls[j][0] = 1;
        wallData.vWalls[j][5] = 1;

        wallData.hWalls[0][j] = 1;
        wallData.hWalls[5][j] = 1;
    }


    //Flood_Fill();
    mDist cellDistances = {4,3,2,3,4,
                           3,2,1,2,3,
                           2,1,0,1,2,
                           1,2,1,2,3,
                           0,1,2,3,4};

    Solve_Maze(&cellDistances);//Path to the middle


    endPosition.cellCoordsY = 4;
    endPosition.cellCoordsX = 0;


    mDist cellDistancesBack = {4,5,6,7,4,
                           3,4,5,6,7,
                           2,3,4,5,6,
                           1,2,3,4,5,
                           0,1,2,3,4};



    Solve_Maze(&cellDistancesBack);



    endPosition.cellCoordsY = 2;
    endPosition.cellCoordsX = 2;

    Run_Maze(&cellDistances);//Path to the beginning

    /*while (botPosition.cellCoords[0] != endPosition.cellCoords[0] & botPosition.cellCoords[1] != endPosition.cellCoords[1])
    {
        //move to end position
    }*/


    return 0;
}
