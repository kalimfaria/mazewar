// 1. WHY IS CLEAR RAT POSITION NOT WORKING?
// 2. WHY IS TIMEOUT NOT WORKING? - DONE
// Problem in this piece of code:
// TAGS BUT CANNOT SEE THE OTHER PLAYER. WHY? SADNESS. DONE
// 1) reappearing after tag - DONE
// 2) update score card - DONE
// 3) duplicates in tagging - DONE
// 4) See other rat - DONE
// 5) clear projectile and arrows - DONE
// 6) clear rat - DONE
// 7) Cross pressed - DONE
// 8) Convert incoming is screwing up :/

//// CLEAR POSITION IS NOT WORKING 
// Convert incoming is weird :/
#include "main.h"
#include <string>
#include "mazewar.h"
#include <map>
/*-------------------------------Variables----------------------------------*/
static bool updateView;	/* true if update needed */
MazewarInstance::Ptr M;
struct Projectile {
	bool present; // true if a missile is flying
	short x, y; // origin of sending
	short prev_x, prev_y; // for showMissile
	struct timeval shootTime; // time when shot
	int direction;
};
/* Use this socket address to send packets to the multi-cast group. */
static Sockaddr         groupAddr;
#define MAX_OTHER_RATS  (MAX_RATS - 1)
Projectile proj;
void makePacket(MW244BPacket*, char, int, bool);
void sendPacketToPlayers(MW244BPacket);
MW244BPacket *rat_array[MAX_RATS];
int my_seq_no,checkingzero,checking, kills,GLOBAL_ID, participation;
int prevseq_a[8], expected_seqno[8], participants[8];
bool join, updateSeqNo;
unsigned long start_time;
map <int,int> Mapping_idToIndex;
map <int,int> Mapping_indexToId;
/***************************************************************/
void setMapping()
{
	participation = 0;
	if (M->myRatId().value() == 7)
	{
		Mapping_idToIndex.insert(pair<int, int>(7,0));
		for(int i = 0; i < 7 ; i++)
			Mapping_idToIndex.insert(pair<int, int>(i,i+1));
		Mapping_indexToId.insert(pair<int, int>(0,7));		
		Mapping_indexToId.insert(pair<int, int>(1,0));
		Mapping_indexToId.insert(pair<int, int>(2,1));
		Mapping_indexToId.insert(pair<int, int>(3,2));
		Mapping_indexToId.insert(pair<int, int>(4,3));
		Mapping_indexToId.insert(pair<int, int>(5,4));
		Mapping_indexToId.insert(pair<int, int>(6,5));
		Mapping_indexToId.insert(pair<int, int>(7,6));	
	}
	else if (M->myRatId().value() == 6)
	{	
		Mapping_idToIndex.insert(pair<int, int>(6,0));
		Mapping_idToIndex.insert(pair<int, int>(7,1));
		for(int i = 0; i < 6; i++)
			Mapping_idToIndex.insert(pair<int, int>(i,i+2));
		Mapping_indexToId.insert(pair<int, int>(0,6));		
		Mapping_indexToId.insert(pair<int, int>(1,7));
		Mapping_indexToId.insert(pair<int, int>(2,0));
		Mapping_indexToId.insert(pair<int, int>(3,1));
		Mapping_indexToId.insert(pair<int, int>(4,2));
		Mapping_indexToId.insert(pair<int, int>(5,3));
		Mapping_indexToId.insert(pair<int, int>(6,4));
		Mapping_indexToId.insert(pair<int, int>(7,5));
	}
	else if (M->myRatId().value() == 5)
	{	
		Mapping_idToIndex.insert(pair<int, int>(5,0));
		Mapping_idToIndex.insert(pair<int, int>(6,1));
		Mapping_idToIndex.insert(pair<int, int>(7,2));
		for(int i = 0; i < 5; i++)
			Mapping_idToIndex.insert(pair<int, int>(i,i+3));
		Mapping_indexToId.insert(pair<int, int>(0,5));		
		Mapping_indexToId.insert(pair<int, int>(1,6));
		Mapping_indexToId.insert(pair<int, int>(2,7));
		Mapping_indexToId.insert(pair<int, int>(3,0));
		Mapping_indexToId.insert(pair<int, int>(4,1));
		Mapping_indexToId.insert(pair<int, int>(5,2));
		Mapping_indexToId.insert(pair<int, int>(6,3));
		Mapping_indexToId.insert(pair<int, int>(7,4));	
	}
	else if (M->myRatId().value() == 4)
	{	
		Mapping_idToIndex.insert(pair<int, int>(4,0));
		Mapping_idToIndex.insert(pair<int, int>(5,1));
		Mapping_idToIndex.insert(pair<int, int>(6,2));
		Mapping_idToIndex.insert(pair<int, int>(7,3));
		for(int i = 0; i < 4; i++)
			Mapping_idToIndex.insert(pair<int, int>(i,i+4));
		Mapping_indexToId.insert(pair<int, int>(0,4));		
		Mapping_indexToId.insert(pair<int, int>(1,5));
		Mapping_indexToId.insert(pair<int, int>(2,6));
		Mapping_indexToId.insert(pair<int, int>(3,7));
		Mapping_indexToId.insert(pair<int, int>(4,0));
		Mapping_indexToId.insert(pair<int, int>(5,1));
		Mapping_indexToId.insert(pair<int, int>(6,2));
		Mapping_indexToId.insert(pair<int, int>(7,3));	
	}
	else if (M->myRatId().value() == 3)
	{	
		Mapping_idToIndex.insert(pair<int, int>(3,0));		
		Mapping_idToIndex.insert(pair<int, int>(4,1));
		Mapping_idToIndex.insert(pair<int, int>(5,2));
		Mapping_idToIndex.insert(pair<int, int>(6,3));
		Mapping_idToIndex.insert(pair<int, int>(7,4));
		for(int i = 0; i < 3; i++)
			Mapping_idToIndex.insert(pair<int, int>(i,i+5));
		Mapping_indexToId.insert(pair<int, int>(0,3));		
		Mapping_indexToId.insert(pair<int, int>(1,4));
		Mapping_indexToId.insert(pair<int, int>(2,5));
		Mapping_indexToId.insert(pair<int, int>(3,6));
		Mapping_indexToId.insert(pair<int, int>(4,7));
		Mapping_indexToId.insert(pair<int, int>(5,0));
		Mapping_indexToId.insert(pair<int, int>(6,1));
		Mapping_indexToId.insert(pair<int, int>(7,2));	

	}
	else if (M->myRatId().value() == 2)
	{	
		Mapping_idToIndex.insert(pair<int, int>(2,0));		
		Mapping_idToIndex.insert(pair<int, int>(3,1));
		Mapping_idToIndex.insert(pair<int, int>(4,2));
		Mapping_idToIndex.insert(pair<int, int>(5,3));
		Mapping_idToIndex.insert(pair<int, int>(6,4));
		Mapping_idToIndex.insert(pair<int, int>(7,5));
		for(int i = 0; i < 2; i++)
			Mapping_idToIndex.insert(pair<int, int>(i,i+6));
		Mapping_indexToId.insert(pair<int, int>(0,2));		
		Mapping_indexToId.insert(pair<int, int>(1,3));
		Mapping_indexToId.insert(pair<int, int>(2,4));
		Mapping_indexToId.insert(pair<int, int>(3,5));
		Mapping_indexToId.insert(pair<int, int>(4,6));
		Mapping_indexToId.insert(pair<int, int>(5,7));
		Mapping_indexToId.insert(pair<int, int>(6,0));
		Mapping_indexToId.insert(pair<int, int>(7,1));	

	}
	else if (M->myRatId().value() == 1)
	{	
		Mapping_idToIndex.insert(pair<int, int>(1,0));		
		Mapping_idToIndex.insert(pair<int, int>(2,1));
		Mapping_idToIndex.insert(pair<int, int>(3,2));
		Mapping_idToIndex.insert(pair<int, int>(4,3));
		Mapping_idToIndex.insert(pair<int, int>(5,4));
		Mapping_idToIndex.insert(pair<int, int>(6,5));
		Mapping_idToIndex.insert(pair<int, int>(7,6));
		Mapping_idToIndex.insert(pair<int, int>(0,7));		

		Mapping_indexToId.insert(pair<int, int>(0,1));		
		Mapping_indexToId.insert(pair<int, int>(1,2));
		Mapping_indexToId.insert(pair<int, int>(2,3));
		Mapping_indexToId.insert(pair<int, int>(3,4));
		Mapping_indexToId.insert(pair<int, int>(4,5));
		Mapping_indexToId.insert(pair<int, int>(5,6));
		Mapping_indexToId.insert(pair<int, int>(6,7));
		Mapping_indexToId.insert(pair<int, int>(7,0));		

	}
	else if (M->myRatId().value() == 0)
		for(int i = 0; i < 8; i++)
		{			
			Mapping_idToIndex.insert(pair<int, int>(i,i));
			Mapping_indexToId.insert(pair<int, int>(i,i));
		}

	my_seq_no = M->myRatId().value(); // MY ID NO IS MY FIRST SEQ 
}
/* ----------------------------------------------------------------------- */
	void
UpdateScoreCard(RatIndexType ratIndex)
{
	ClearScoreLine(ratIndex);
	if (M->mazeRats_[ratIndex.value()].playing)
		WriteScoreString(ratIndex);
	if (Rats2Display[ratIndex.value()].visible == TRUE)
		InvertScoreLine(ratIndex);
}

void ConvertIntHtoN(unsigned long *p)
{
	*p = htonl(*p);
}

void ConvertIntNtoH(unsigned long *p)
{
	*p = ntohl(*p);
}

void ConvertIncoming(MW244BPacket *p)
{
	ConvertIntNtoH((unsigned long*)&p->ID);	
	ConvertIntNtoH((unsigned long*)&p->sequence_number);	
	ConvertIntNtoH((unsigned long*)&p->score);
	ConvertIntNtoH((unsigned long*)&p->x_pos);
	ConvertIntNtoH((unsigned long*)&p->y_pos);
	ConvertIntNtoH((unsigned long*)&p->dir);
	ConvertIntNtoH((unsigned long*)&p->tagged_rat);
	ConvertIntNtoH((unsigned long*)&p->globalID);
	ConvertIntNtoH(&p->time);	
	//exit(0);	
}

/* ----------------------------------------------------------------------- */

void ConvertOutgoing(MW244BPacket *p)
{
	ConvertIntHtoN((unsigned long*)&p->ID);
	ConvertIntHtoN((unsigned long*)&p->sequence_number);	
	ConvertIntHtoN((unsigned long*)&p->score);
	ConvertIntHtoN((unsigned long*)&p->x_pos);
	ConvertIntHtoN((unsigned long*)&p->y_pos);
	ConvertIntHtoN((unsigned long*)&p->dir);	
	ConvertIntHtoN((unsigned long*)&p->tagged_rat);
	ConvertIntHtoN((unsigned long*)&p->globalID);
	ConvertIntHtoN(&p->time);	
}

/* ----------------------------------------------------------------------- */
int time_diff(struct timeval &t1, struct timeval &t2)
{
	return ((t1.tv_sec - t2.tv_sec) * 1000000 + t1.tv_usec - t2.tv_usec) / 1000;
}
/* ----------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
	Loc x(1);
	Loc y(5);
	Direction dir(0);
	char *ratName;	
	signal(SIGHUP, quit);
	signal(SIGINT, quit);
	signal(SIGTERM, quit);
	struct timeval zero;
	struct timeval now;
	gettimeofday(&now, NULL);
	start_time = time_diff(now, zero);

	getName("Welcome to CS244B MazeWar!\n\nYour Name", &ratName);
	ratName[strlen(ratName)-1] = 0;

	M = MazewarInstance::mazewarInstanceNew(string(ratName));
	MazewarInstance* a = M.ptr();
	strncpy(M->myName_, ratName, NAMESIZE);
	free(ratName);

	MazeInit(argc, argv);
	NewPosition(M);
	play();

	return 0;
}
/* ----------------------------------------------------------------------- */
void play(void)
{	// initialisation
	cout << "Starting Play" << endl;
	//SetRatPosition(7,1,5,0);
	proj.present = false;
	proj.prev_x = proj.prev_y = proj.x = proj.y = 1;
	int turn = 0;
	
	updateSeqNo = true;
	join = false;
	
	for (int k = 0; k < 8; k++)
		participants[k] = prevseq_a[k] = 0;
	for (int k = 0; k < 8; k++)
		expected_seqno[k] = k+1;
	GLOBAL_ID = 0;
	checking = 1;
	checkingzero=1;
	MWEvent		event;
	MW244BPacket	incoming;

	event.eventDetail = &incoming;

	while (TRUE) {
		turn ++;
		NextEvent(&event, M->theSocket());
		if (!M->peeking())
			switch(event.eventType) {
				case EVENT_A:
					aboutFace();
					break;

				case EVENT_S:
					leftTurn();
					break;

				case EVENT_D:
					forward();
					break;

				case EVENT_F:
					rightTurn();
					break;


				case EVENT_LEFT_D:
					peekLeft();
					break;

				case EVENT_BAR:
					shoot();
					break;

				case EVENT_RIGHT_D:
					peekRight();
					break;

				case EVENT_NETWORK:
					processPacket(&event);
					break;

				case EVENT_TIMEOUT:

					checking = (++checking) % 25;					
					if (checking == 0 && !join){
						cout << "Setting ID to zero" << endl;
						join = true;
						setMapping();
					}		
					else {cout << "TIMEOUT"  << endl;
						participation = (++participation) % 30;
						manageMissiles();
						if (participation == 29)
						{ ratStates(); /* clean house */
							for (int i = 0 ; i < 8 ; i++)
								participants[i] = 0;
						}
					}
					break;

				case EVENT_INT:
					quit(0);
					break;

			}
		else
			switch (event.eventType) {
				case EVENT_RIGHT_U:
				case EVENT_LEFT_U:
					peekStop();
					break;
				case EVENT_NETWORK:
					processPacket(&event);
					break;
			}
		DoViewUpdate();
		checking ++;

		manageMissiles();
		if (join && checkingzero == 0)
		{	
			MW244BPacket p;
			makePacket(&p,'a',kills, updateSeqNo);
			sendPacketToPlayers(p);
		}

		else if (join && turn % 2 == 0)
		{
			MW244BPacket p;
			makePacket(&p,'t',-1, updateSeqNo);
			sendPacketToPlayers(p);
		}

	}
}
/* ----------------------------------------------------------------------- */
static	Direction	_aboutFace[NDIRECTION] ={SOUTH, NORTH, WEST, EAST};
static	Direction	_leftTurn[NDIRECTION] =	{WEST, EAST, NORTH, SOUTH};
static	Direction	_rightTurn[NDIRECTION] ={EAST, WEST, SOUTH, NORTH};

	void
aboutFace(void)
{
	M->dirIs(_aboutFace[MY_DIR]);
	updateView = TRUE;
}

/* ----------------------------------------------------------------------- */
	void
leftTurn(void)
{
	M->dirIs(_leftTurn[MY_DIR]);
	updateView = TRUE;
}

/* ----------------------------------------------------------------------- */
	void
rightTurn(void)
{
	M->dirIs(_rightTurn[MY_DIR]);
	updateView = TRUE;
}

/* ----------------------------------------------------------------------- */
void forward(void)
{	
	bool allow = true;
	register int tx = MY_X_LOC;
	register int ty = MY_Y_LOC;
	switch(MY_DIR) {
		case NORTH:
			{
				for (int i = 0; i < 8 ; i++)
				{
					if(rat_array[i] != NULL)
					{
						if (M->mazeRats_[i].x.value() == MY_X_LOC+1 && MY_Y_LOC == M->mazeRats_[i].y.value())	
						{

							allow = false;
							break;
						}
					} 
				}
				break; 
			}
		case SOUTH:	
			{
				for (int i = 0; i < 8 ; i++)
				{
					if(rat_array[i] != NULL)
					{	
						if (M->mazeRats_[i].x.value() == MY_X_LOC-1  && MY_Y_LOC == M->mazeRats_[i].y.value())	
						{

							allow = false;
							break;
						}
					} 
				}
				break; 
			}
		case EAST:
			{
				for (int i = 0; i < 8 ; i++)
				{
					if(rat_array[i] != NULL)
					{
						if (M->mazeRats_[i].x.value() == MY_X_LOC  && MY_Y_LOC+1 == M->mazeRats_[i].y.value())	
						{

							allow = false;
							break;
						}
					} 
				}
				break; 
			}
		case WEST:
			{
				for (int i = 0; i < 8 ; i++)
				{
					if(rat_array[i] != NULL)
					{
						if (M->mazeRats_[i].x.value() == MY_X_LOC  && MY_Y_LOC-1 == M->mazeRats_[i].y.value())	
						{

							allow = false;
							break;
						}
					} 
				}
				break; 
			}	
		default:
			exit(0);
	}
	if (allow){

		switch(MY_DIR) {
			case NORTH:	if (!M->maze_[tx+1][ty])	tx++; break;
			case SOUTH:	if (!M->maze_[tx-1][ty])	tx--; break;
			case EAST:	if (!M->maze_[tx][ty+1])	ty++; break;
			case WEST:	if (!M->maze_[tx][ty-1])	ty--; break;
			default:
						MWError("bad direction in Forward");
		}
		if ((MY_X_LOC != tx) || (MY_Y_LOC != ty)) {
			M->xlocIs(Loc(tx));
			M->ylocIs(Loc(ty));
			updateView = TRUE;
		}
	}
}
/* ----------------------------------------------------------------------- */

void backward()
{
	register int	tx = MY_X_LOC;
	register int	ty = MY_Y_LOC;

	switch(MY_DIR) {
		case NORTH:	if (!M->maze_[tx-1][ty])	tx--; break;
		case SOUTH:	if (!M->maze_[tx+1][ty])	tx++; break;
		case EAST:	if (!M->maze_[tx][ty-1])	ty--; break;
		case WEST:	if (!M->maze_[tx][ty+1])	ty++; break;
		default:
					MWError("bad direction in Backward");
	}
	if ((MY_X_LOC != tx) || (MY_Y_LOC != ty)) {
		M->xlocIs(Loc(tx));
		M->ylocIs(Loc(ty));
		updateView = TRUE;
	}
}

/* ----------------------------------------------------------------------- */

void peekLeft()
{	
	M->xPeekIs(MY_X_LOC);
	M->yPeekIs(MY_Y_LOC);
	M->dirPeekIs(MY_DIR);

	switch(MY_DIR) {
		case NORTH:	if (!M->maze_[MY_X_LOC+1][MY_Y_LOC]) {
					M->xPeekIs(MY_X_LOC + 1);
					M->dirPeekIs(WEST);
				}
				break;

		case SOUTH:	if (!M->maze_[MY_X_LOC-1][MY_Y_LOC]) {
					M->xPeekIs(MY_X_LOC - 1);
					M->dirPeekIs(EAST);
				}
				break;

		case EAST:	if (!M->maze_[MY_X_LOC][MY_Y_LOC+1]) {
					M->yPeekIs(MY_Y_LOC + 1);
					M->dirPeekIs(NORTH);
				}
				break;

		case WEST:	if (!M->maze_[MY_X_LOC][MY_Y_LOC-1]) {
					M->yPeekIs(MY_Y_LOC - 1);
					M->dirPeekIs(SOUTH);
				}
				break;

		default:
				MWError("bad direction in PeekLeft");
	}

	/* if any change, display the new view without moving! */

	if ((M->xPeek() != MY_X_LOC) || (M->yPeek() != MY_Y_LOC)) {
		M->peekingIs(TRUE);
		updateView = TRUE;
	}
}

/* ----------------------------------------------------------------------- */

void peekRight()
{
	M->xPeekIs(MY_X_LOC);
	M->yPeekIs(MY_Y_LOC);
	M->dirPeekIs(MY_DIR);

	switch(MY_DIR) {
		case NORTH:	if (!M->maze_[MY_X_LOC+1][MY_Y_LOC]) {
					M->xPeekIs(MY_X_LOC + 1);
					M->dirPeekIs(EAST);
				}
				break;

		case SOUTH:	if (!M->maze_[MY_X_LOC-1][MY_Y_LOC]) {
					M->xPeekIs(MY_X_LOC - 1);
					M->dirPeekIs(WEST);
				}
				break;

		case EAST:	if (!M->maze_[MY_X_LOC][MY_Y_LOC+1]) {
					M->yPeekIs(MY_Y_LOC + 1);
					M->dirPeekIs(SOUTH);
				}
				break;

		case WEST:	if (!M->maze_[MY_X_LOC][MY_Y_LOC-1]) {
					M->yPeekIs(MY_Y_LOC - 1);
					M->dirPeekIs(NORTH);
				}
				break;

		default:
				MWError("bad direction in PeekRight");
	}

	/* if any change, display the new view without moving! */

	if ((M->xPeek() != MY_X_LOC) || (M->yPeek() != MY_Y_LOC)) {
		M->peekingIs(TRUE);
		updateView = TRUE;
	}
}

/* ----------------------------------------------------------------------- */

void peekStop()
{
	M->peekingIs(FALSE);
	updateView = TRUE;
}

/* ----------------------------------------------------------------------- */

void shoot()
{
	if (proj.present) return;
	proj.present = true;
	proj.x = M->xloc().value();
	proj.y = M->yloc().value();
	proj.prev_x = proj.prev_y = 0;
	proj.direction = M->dir().value();
	gettimeofday(&proj.shootTime, NULL);
	M->scoreIs( M->score().value()-1 );
	UpdateScoreCard(0);
}

/* ----------------------------------------------------------------------- */

/*
 * Exit from game, clean up window
 */

void quit(int sig)
{	cout << "QUIT WAS CALLED"<< endl;
	MW244BPacket p;
	makePacket(&p,'f',-1, updateSeqNo);
	sendPacketToPlayers(p);
	StopWindow();
	exit(0);
}


/* ----------------------------------------------------------------------- */

void NewPosition(MazewarInstance::Ptr m)
{
	Loc newX(0);
	Loc newY(0);
	Direction dir(0); /* start on occupied square */

	while (M->maze_[newX.value()][newY.value()]) {
		/* MAZE[XY]MAX is a power of 2 */
		newX = Loc(random() & (MAZEXMAX - 1));
		newY = Loc(random() & (MAZEYMAX - 1));
		for (int i = 0; i < 8; i ++)

			if (M->mazeRats_[i].x.value() == newX.value() && newY.value() == M->mazeRats_[i].y.value())
			{
				newX = Loc(0);
				newY = Loc(0);
				break;
			}	
		/* In real game, also check that square is
		   unoccupied by another rat */
	}

	/* prevent a blank wall at first glimpse */

	if (!m->maze_[(newX.value())+1][(newY.value())]) dir = Direction(NORTH);
	if (!m->maze_[(newX.value())-1][(newY.value())]) dir = Direction(SOUTH);
	if (!m->maze_[(newX.value())][(newY.value())+1]) dir = Direction(EAST);
	if (!m->maze_[(newX.value())][(newY.value())-1]) dir = Direction(WEST);

	m->xlocIs(newX);
	m->ylocIs(newY);
	m->dirIs(dir);
}

/* ----------------------------------------------------------------------- */

void MWError(char *s)

{
	StopWindow();
	fprintf(stderr, "CS244BMazeWar: %s\n", s);
	perror("CS244BMazeWar");
	exit(-1);
}

/* ----------------------------------------------------------------------- */

/* This is just for the sample version, rewrite your own */
Score GetRatScore(RatIndexType ratId)
{
	if (ratId.value() == 0 )
	{ return(M->score()); }
	else 
	{
		if(rat_array[(ratId.value())]==NULL)
			return 0;
		else
			return rat_array[ratId.value()]->score; 
	}

}
/** ----------------------------------------------------------------------- */

/* This is just for the sample version, rewrite your own */
char *GetRatName(RatIndexType ratId)
{
	if (ratId.value() ==	0)
	{ return(M->myName_); }
	else { if (rat_array[ratId.value()] != NULL) {
		char * p = new char[20];
		p = rat_array[ratId.value()]->Name;
		return p;}
		else return "Dummy";}

}

/* ----------------------------------------------------------------------- */

void ratStates()
{
	/* In our sample version, we don't know about the state of any rats over
	   the net, so this is a no-op */
	for (int i = 0; i < 8 ; i++)
		if (i != M->myRatId().value() && participants[i] == 0 && M->mazeRats_[Mapping_idToIndex.find(i)->second].playing)// no packet from this dude
		{	cout << i << endl;
			//M->mazeRats_[Mapping_idToIndex.find(i)->second].playing = false;
			rat_array[Mapping_idToIndex.find(i)->second] = NULL;	
			RatIndexType ratId( Mapping_idToIndex.find(i)->second);			
			ClearRatPosition(ratId);
		}

	for (int i = 0; i < 8; i++)
		cout << "bool" << M->mazeRats_[i].playing << endl;
	if (!( M->mazeRats_[1].playing || M->mazeRats_[2].playing || M->mazeRats_[3].playing || M->mazeRats_[4].playing || M->mazeRats_[5].playing || M->mazeRats_[6].playing || M->mazeRats_[7].playing)) // this means that there are no other players :O
		{
			M->myRatIdIs(0); 
			M->scoreIs(0);
			SetMyRatIndexType(0);
			Mapping_idToIndex.clear();
			Mapping_indexToId.clear();			
			play();//main(0,NULL); // start over
		}



}

/* ----------------------------------------------------------------------- */

/* This is just for the sample version, rewrite your own */
// order: N S E W

void clearProjectile()
{
	proj.present = false;
	clearSquare(proj.prev_x, proj.prev_y);
	proj.x = 0;
	proj.y = 0;	proj.prev_x= 1;
	proj.prev_y= 1;

}


void makePacket(MW244BPacket *p, char status, int killed, bool UpdateSeqNo)
{	
	strncpy(p->Name,M->myName_,NAMESIZE ); 
	p-> ID = M->myRatId().value();
	cout << "ID from packet: " << M->myRatId().value() << endl;
	if (UpdateSeqNo) ++my_seq_no;
	p-> sequence_number = my_seq_no;
	p -> tagged_rat = killed; 
	p->score = M->score().value();
	p->x_pos = MY_X_LOC; 
	p->y_pos = MY_Y_LOC; 
	p-> dir = MY_DIR; 
	p-> status = status;
	p-> globalID = GLOBAL_ID; 
	p -> time = start_time;
}
void sendPacketToPlayers(MW244BPacket p)
{

	ConvertOutgoing(&p);

	if (sendto(M->theSocket(), &p, sizeof(MW244BPacket), 0, (struct sockaddr *)&groupAddr, sizeof(Sockaddr)) < 0)
		MWError("Sample error"); 

}

void manageMissiles()
{
	if (!proj.present)
		return;

	struct timeval now;
	gettimeofday(&now, NULL);
	int x = proj.x, y = proj.y;
	int elapsed_time = time_diff(now, proj.shootTime);
	int distance_travelled = elapsed_time * projectile_speed;
	if (proj.direction == 0)
		x += distance_travelled;
	else if (proj.direction == 1)
		x -= distance_travelled;
	else if (proj.direction == 2)
		y += distance_travelled;
	else if (proj.direction == 3)
		y -= distance_travelled;

	if (MY_X_LOC == x && MY_Y_LOC == y)
		return;

	if (M->maze_[x][y]) {
		// hit wall
		clearProjectile(); 
		return;
	}
	for (int i = 0; i < 8; i++) 
	{
		if (M->mazeRats_[i].playing && M->mazeRats_[i].x.value() == x && y == M->mazeRats_[i].y.value())	
		{
			MW244BPacket p;
			makePacket(&p,'a',Mapping_indexToId.find(i)->second, updateSeqNo);			
			proj.prev_x = x;
			proj.prev_y = y;
			sendPacketToPlayers(p);
			updateSeqNo = false;
			checkingzero = 0;
			RatIndexType ratId(i);
			kills = Mapping_indexToId.find(i)->second;
			clearProjectile(); 
			M->scoreIs( M->score().value() + 11 );
			UpdateScoreCard(0);
			DrawString("You have tagged a player!",25,200,250); 					
			ClearRatPosition(ratId);
		}	

	}
	if (proj.prev_x == 0 && proj.prev_y == 0) proj.prev_x = proj.prev_y = 1;
	showMissile(x, y, proj.direction,
			proj.prev_x, proj.prev_y,
			true);
	proj.prev_x = x;
	proj.prev_y = y;
	if (updateSeqNo && join)
	{
		MW244BPacket p;
		makePacket(&p,'t',-1, updateSeqNo);	
		sendPacketToPlayers(p);
	}
	

}

/* ----------------------------------------------------------------------- */

void DoViewUpdate()
{
	if (updateView) {	/* paint the screen */
		ShowPosition(MY_X_LOC, MY_Y_LOC, MY_DIR);
		if (M->peeking())
			ShowView(M->xPeek(), M->yPeek(), M->dirPeek());
		else
			ShowView(MY_X_LOC, MY_Y_LOC, MY_DIR);
		updateView = FALSE;
	}
}

/* ----------------------------------------------------------------------- */

/* Sample of processPacket. */


bool myPacket(MW244BPacket *p, Sockaddr *remote) {

	return Mapping_idToIndex.find(p->ID)->second == Mapping_idToIndex.find(M->myRatId().value())->second && p->time == start_time;
}


void processPacket (MWEvent *eventPacket)
{

	MW244BPacket *p = new MW244BPacket ;
	p = eventPacket->eventDetail;
	/*cout << p->ID<< endl;
	cout << p->sequence_number<< endl;
	cout << p->score<< endl;
	cout << p->x_pos<< endl;
	cout << p->y_pos<< endl;
	cout << p->dir<< endl;
	cout << p->tagged_rat<< endl;
	cout << p->globalID<< endl;
	ConvertIncoming(p);
	cout << p->ID<< endl;
	cout << p->sequence_number<< endl;
	cout << p->score<< endl;
	cout << p->x_pos<< endl;
	cout << p->y_pos<< endl;
	cout << p->dir<< endl;
	cout << p->tagged_rat<< endl;
	cout << p->globalID<< endl;
	exit(0);*/

	cout << "PP " << p->ID << endl;
	if (myPacket(p, &eventPacket->eventSource) && join)// BECAUSE WHILE YOU ARE STILL 0, you will get packets with ID 0
	{
		return;
	}
	else {
		if (!join && expected_seqno[p->ID] <= p->sequence_number){

			if (p->globalID == 7) // What if there is only player with global ID 7? :/
			{
				cout << "You are one player too many :(" << endl;
				cout << "Please try again later." << endl;
				exit(0);

			}	
			else
			{	for (int i = 0; i < 100; i++)
					cout << "Incrementing for no reason" << endl;
				join = true;
				GLOBAL_ID = p->globalID+1;
				M->myRatIdIs(GLOBAL_ID);
				setMapping();
				UpdateScoreCard(0);
				MW244BPacket q;
				makePacket(&q,'h',-1, updateSeqNo); 
				sendPacketToPlayers(q);				
				participants[p->ID]++;
				Rat r;
				r.playing = 1;
				cout << "NEW " << p->ID << endl;
				r.x=p->x_pos;
				r.y=p->y_pos;
				r.dir=(p->dir);
				M->mazeRats_[Mapping_idToIndex.find(p->ID)->second] = r;
				rat_array[p-> ID]=new MW244BPacket;
				rat_array[Mapping_idToIndex.find(p->ID)->second]=p;
				Loc x(p->x_pos);
				Loc y(p->y_pos);
				Direction dir(p-> dir);
				RatIndexType ratId(Mapping_idToIndex.find(p->ID)->second);
				SetRatPosition(ratId, x, y, dir);
				UpdateScoreCard(Mapping_idToIndex.find(p-> ID)->second);
				expected_seqno[p->ID] = p->sequence_number + 1;
				return;
			}
		}

		if (GLOBAL_ID < p->globalID  && expected_seqno[p->ID] <= p->sequence_number )
			GLOBAL_ID = p->globalID;

		if (p->status == 'a' && p->tagged_rat == M->myRatId().value())
		{ // cannot discard expected_seqno[p->ID] <= p->sequence_number 
			// because we're getting this packet because the other side didn't get b :(
			// MUST send b
			MW244BPacket q;
			makePacket(&q,'b',-1, updateSeqNo);
			sendPacketToPlayers(q);
			cout<<p->sequence_number<<"Seq number"<<endl;
			cout<<prevseq_a[p->ID]<<"Previous Seq number"<<endl;		

			if (p->sequence_number  > prevseq_a[p->ID])
			{ // dealing with duplicates
				DrawString("You have been tagged!",21,200,250); 
				M->scoreIs( M->score().value()-5 );
				UpdateScoreCard(0);
				NewPosition(M);
			}
			prevseq_a[p->ID] = p->sequence_number;

		}
		else if (p->status == 'a' && expected_seqno[p->ID] <= p->sequence_number){
			//RatIndexType ratId(p-> ID);
			//ClearRatPosition(ratId);
			RatIndexType ratId( Mapping_idToIndex.find(p->ID)->second);			
			ClearRatPosition(ratId);

		}

		if (p->status == 'b' && expected_seqno[p->ID] <= p->sequence_number)
		{ 
			checkingzero = 1;
			updateSeqNo = true;
		}

		if (p->status == 'f' && expected_seqno[p->ID] <= p->sequence_number)
		{ 
			//player has left
			cout << "Player has gone :(" << endl;
			participants[p->ID] = -1;// in next section, will be incremented to 0
			// automatically dealt with in ratstates
			RatIndexType ratId( Mapping_idToIndex.find(p->ID)->second);			
			ClearRatPosition(ratId); // WHY IS THIS NOT WORKING? :/
			clearSquare(p->x_pos, p->y_pos);
			cout << p->x_pos << "," << p->y_pos << endl;
			cout << M->mazeRats_[Mapping_idToIndex.find(p->ID)->second].x.value() << "," << M->mazeRats_[Mapping_idToIndex.find(p->ID)->second].y.value() << endl;
			
		}
	

		if (expected_seqno[p->ID] <= p->sequence_number)
		{ // if this condition is false, then we are discarding the packet.
			Rat r;
			r.playing = 1;
			cout << "IF " << p->ID << endl;
			r.x=p->x_pos;
			r.y=p->y_pos;
			r.dir=(p->dir);
			M->mazeRats_[Mapping_idToIndex.find(p->ID)->second] = r;
			rat_array[p-> ID]=new MW244BPacket;
			rat_array[Mapping_idToIndex.find(p->ID)->second]=p;
			Loc x(p->x_pos);
			Loc y(p->y_pos);
			Direction dir(p-> dir);
			RatIndexType ratId(Mapping_idToIndex.find(p->ID)->second);
			SetRatPosition(ratId, x, y, dir);
			UpdateScoreCard(Mapping_idToIndex.find(p-> ID)->second);
			expected_seqno[p->ID] = p->sequence_number + 1;
			participants[p->ID]++;
		}
	}
}
/* ----------------------------------------------------------------------- */

	void
netInit()
{
	Sockaddr		nullAddr;
	Sockaddr		*thisHost;
	char			buf[128];
	int				reuse;
	u_char          ttl;
	struct ip_mreq  mreq;

	/* MAZEPORT will be assigned by the TA to each team */
	M->mazePortIs(htons(MAZEPORT));

	gethostname(buf, sizeof(buf));
	if ((thisHost = resolveHost(buf)) == (Sockaddr *) NULL)
		MWError("who am I?");
	bcopy((caddr_t) thisHost, (caddr_t) (M->myAddr()), sizeof(Sockaddr));

	M->theSocketIs(socket(AF_INET, SOCK_DGRAM, 0));
	if (M->theSocket() < 0)
		MWError("can't get socket");

	/* SO_REUSEADDR allows more than one binding to the same
	   socket - you cannot have more than one player on one
	   machine without this */
	reuse = 1;
	if (setsockopt(M->theSocket(), SOL_SOCKET, SO_REUSEADDR, &reuse,
				sizeof(reuse)) < 0) {
		MWError("setsockopt failed (SO_REUSEADDR)");
	}

	nullAddr.sin_family = AF_INET;
	nullAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	nullAddr.sin_port = M->mazePort();
	if (bind(M->theSocket(), (struct sockaddr *)&nullAddr,
				sizeof(nullAddr)) < 0)
		MWError("netInit binding");

	/* Multicast TTL:
	   0 restricted to the same host
	   1 restricted to the same subnet
	   32 restricted to the same site
	   64 restricted to the same region
	   128 restricted to the same continent
	   255 unrestricted

	   DO NOT use a value > 32. If possible, use a value of 1 when
	   testing.
	 */

	ttl = 1;
	if (setsockopt(M->theSocket(), IPPROTO_IP, IP_MULTICAST_TTL, &ttl,
				sizeof(ttl)) < 0) {
		MWError("setsockopt failed (IP_MULTICAST_TTL)");
	}

	/* join the multicast group */
	mreq.imr_multiaddr.s_addr = htonl(MAZEGROUP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(M->theSocket(), IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)
				&mreq, sizeof(mreq)) < 0) {
		MWError("setsockopt failed (IP_ADD_MEMBERSHIP)");
	}

	/*
	 * Now we can try to find a game to join; if none, start one.
	 */

	printf("\n");

	/* set up some stuff strictly for this local sample */
	//int  temp = rand()%8;/*FIXME- REALLY REALLY RANDOM*/
	M->myRatIdIs(0); // FIXME
	//M->rat(M->myRatId().value()).playing=true;
	//cout <<  M->myRatId().value() << endl;
	//cout <<  temp << endl;
	M->scoreIs(0);
	SetMyRatIndexType(0);

	/* Get the multi-cast address ready to use in SendData()
	   calls. */
	memcpy(&groupAddr, &nullAddr, sizeof(Sockaddr));
	groupAddr.sin_addr.s_addr = htonl(MAZEGROUP);

}


/* ----------------------------------------------------------------------- */
