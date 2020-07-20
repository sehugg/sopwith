/*
	sw.h	 -	SW  Manifest Definitions

	Copyright (C) 1984-2003 David L. Clark.
	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free Software
	Foundation; either version 2 of the License, or (at your option) any later
	version. This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
	or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
	more details. You should have received a copy of the GNU General Public
	License along with this program; if not, write to the Free Software Foundation,
	Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. 

			Author: Dave Clark

	Modification History:
			84-02-02	Development
			84-06-12	PCjr Speed-up
			85-04-02	Asynch Mode
			85-10-31	Atari
			87-03-09	Microsoft compiler
			87-03-12	Wounded airplanes
			87-03-12	Crashed planes stay longer at home.
			87-03-13	Splatted bird symbol
			87-03-30	Novice Player
			87-03-31	Missiles
			87-04-04	Missile and starburst support
			87-04-08	Delay between starbursts
			2003-01-27	GNU General Public License
*/
#include	"std.h"
#include	"sysint.h"
#include	"setjmp.h"
#include	"swdeve.h"
#include	"swmach.h"


/*  Constants  */

#define MAX_X		3000		/*  Maximum X coordinate	    */
#define MAX_Y		200		/*  Maximum Y coordinate	    */
#define MIN_SPEED	4		/*  Minimum plane speed 	    */
#define MAX_SPEED	8		/*  Maximum plane speed 	    */
#define MAX_THROTTLE	4		/*  Maximum acceleration	    */

#define MAXCRCOUNT	10		/*  Number of turns as crashed	    */
#define FALLCOUNT	10		/*  Moves between falling plane adj */
#define STALLCOUNT	6		/*  Moves between stalling plane adj*/
#define TARGHITCOUNT	10		/*  Target hit count before explod'n*/

#define SCR_WDTH	320		/*  Screen Width		    */
#define SCR_HGHT	200		/*  Screen Height		    */
#define SCR_CENTR	152		/*  Centre column of screen	    */
#define SCR_SEGM	0xB800		/*  Screen Video segment	    */
#define SCR_ROFF	0x2000		/*  Screen odd rastor line offset   */
#define SCR_LINW	80		/*  Screen line width in bytes	    */
#define SCR_MNSH	16		/*  Minumum line number to shift    */
#define SCR_MXSH	75		/*  Maximum line number to shift    */
#define SCR_LIMIT	180		/*  Screen shift limits 	    */

#define WRLD_RSX	(MAX_X / SCR_WDTH * 2 + 1) /*  World display X and  */
#define WRLD_RSY	(MAX_Y / SCR_MNSH + 1)	   /*  Y divisors	    */

#define MINFLCKX	( 0	+ SCR_WDTH + 50 )  /*  Bird flock travel    */
#define MAXFLCKX	( MAX_X - SCR_WDTH - 50 )  /*	 limits 	    */

#define CGUAGEX 	(SCR_CENTR - 25)/*  Crash, fuel, bomb, shot, missile*/
#define FGUAGEX 	(SCR_CENTR - 22)/*     and starburst guage	    */
#define BGUAGEX 	(SCR_CENTR - 19)/*     X-coordinates		    */
#define SGUAGEX 	(SCR_CENTR - 16)
#define MGUAGEX 	(SCR_CENTR - 13)
#define SBGUAGEX	(SCR_CENTR - 10)
#define GHOSTX		(SCR_CENTR - 21)/*  Ghost face display x-coodinate  */

#ifdef	IBMPC
#define 	PRINTINT	0x05	/* Shift print screen interrupt     */
#define 	BREAKINT	0x1B	/* Control break interrupt	    */
#define 	CLOCKINT	0x1C	/* Clock interrupt		    */
#define 	KEYINT		0x09	/* Keyboard interrupt		    */
#endif
#ifdef	ATARI
#define 	PRINTINT	0x05	/* Shift print screen interrupt     */
#define 	BREAKINT	0x05	/* Control break interrupt	    */
#define 	CLOCKINT	0x100	/* Clock interrupt		    */
#define 	KEYINT		0x46	/* Keyboard interrupt		    */
#endif


#define HITSYMS 	2		/*  Number of hit symbols per plane */
#define ANGLES		16		/*  Number of angle increments	    */
#define ORIENTS 	2		/*  Number of plane orientations    */
#define SYMBYTES	64		/*  Bytes in a symbol		    */
#define WINSIZES	4		/*  Number of winner plane sizes    */
#define WINBYTES	64		/*  Bytes in a winner symbol	    */
#define SYM_WDTH	16		/*  Symbol width in pixels	    */
#define SYM_HGHT	16		/*  Symbol height in pixels	    */
#define BOMBBYTES	16		/*  Bytes in a bomb symbol	    */
#define BOMBANGS	8		/*  Number of bomb angles	    */
#define TARGBYTES	64		/*  Bytes in a target symbol	    */
#define TARGORIENTS	4		/*  Number of target types	    */
#define EXPLSYMS	8		/*  Number of explosion symbols     */
#define EXPBYTES	16		/*  Bytes in an explosion symbol    */
#define FLCKSYMS	2		/*  Number of flock symbols	    */
#define FLKBYTES	64		/*  Bytes in a flock symbol	    */
#define BIRDSYMS	2		/*  Number of bird symbols	    */
#define BRDBYTES	2		/*  Bytes in a bird symbol	    */
#define OXSYMS		2		/*  Number of ox symbols	    */
#define OXBYTES 	64		/*  Bytes in an ox symbol	    */
#define GHSTBYTES	16		/*  Bytes in a ghost symbol	    */
#define SHOTBYTES	64		/*  Bytes in a shot window symbol   */
#define SPLTBYTES	256		/*  Bytes in a splatted bird symbol */
#define MISCBYTES	16		/*  Bytes in a missile symbol	    */
#define MISCANGS	16		/*  Number of missile angles	    */
#define BRSTBYTES	16		/*  Bytes in a starburst symbol     */
#define BRSTSYMS	2		/*  Number of starburst symbols     */

#define PCDOS		0		/*  Operating environments	    */
#define JRROMDOS	1
#define JRROM		2

#define SINGLE		0		/*  Playmodes			    */
#define MULTIPLE	1
#define COMPUTER	2
#define ASYNCH		3
#define NOVICE		4

#define MAXROUNDS	200		/* Maximum number of rounds of shot */
#define MAXBOMBS	5		/* Maximum number of bombs available*/
#define MAXMISSILES	5		/* Maximum number of missiles	    */
#define MAXBURSTS	5		/* Maximum nuber of starbursts	    */
#define MAXFUEL 	( 3 * MAX_X )	/* Maximum Number of turns between  */
					/*  refuellings 		    */
#define MAXCRASH	5		/* Mximum number of crashes allowed */

#define MAX_PLYR	4		/* Maximum number of players	    */
#define MAX_TARG	20		/* Maximum number of targets	    */
#define MAX_OBJS	100		/* Maximum number of objects	    */
#define MAX_FLCK	4		/* Maximum number of flocks	    */
#define MAX_BIRD	1		/* Maximum number of stray birds    */
					/*    per flock 		    */
#define MAX_GAME	7		/* Maximum number of games	    */
#define MAX_OXEN	2		/* Maximum number of oxen	    */

#define BULSPEED	10		/* Bullet speed    */
#define BULLIFE 	10		/* Bullet life	   */
#define BOMBLIFE	5		/* Moves between bomb course adj   */
#define MISSLIFE	50		/* Missile life 		   */
#define BURSTLIFE	20		/* Starburst life		   */
#define EXPLLIFE	3		/* Moves between explosion "  "    */
#define SMOKELIFE	10		/* Smoke life			   */
#define BIRDLIFE	4		/* Moves between bird flaps	   */
#define FLOCKLIFE	5		/* Moves between flop flaps	   */

#define WAITING 	0		/*  Player states  */
#define FLYING		1
#define HIT		2
#define CRASHED 	4
#define FALLING 	5
#define STANDING	6
#define STALLED 	7
#define REBUILDING	8
#define WOUNDED 	9
#define WOUNDSTALL	10
#define FINISHED	91
#define GHOST		92
#define GHOSTCRASHED	93
#define GHOSTSTALLED	94

#define WINNER		1		/*  End of game status		   */
#define LOSER		2

#define GROUND		0		/*  Object types		   */
#define PLANE		1
#define BOMB		2
#define SHOT		3
#define TARGET		4
#define EXPLOSION	5
#define SMOKE		6
#define FLOCK		7
#define BIRD		8
#define OX		9
#define MISSILE 	10
#define STARBURST	11
#define DUMMYTYPE	99

#define NEAR		( 150 * 150 )	/* Computer control distances	     */
#define NEARAPPROACH	200
#define CLOSE		32
#define DEADON		( 4 * 4 )
#define HOME		16
#define SAFERESET	32

#define QUIT		-5000		/* Plane life value when quitting    */

#define S_TITLE 	05
#define S_EXPLOSION	10		/*  Sound priorities		    */
#define S_BOMB		20
#define S_SHOT		30
#define S_FALLING	40
#define S_HIT		50
#define S_PLANE 	60

#define K_ACCEL 	0x0001		/* Keyboard word masks		     */
#define K_DEACC 	0x0002
#define K_FLAPU 	0x0004
#define K_FLAPD 	0x0008
#define K_FLIP		0x0010
#define K_SHOT		0x0020
#define K_BOMB		0x0100
#define K_HOME		0x0200
#define K_SOUND 	0x0400
#define K_BREAK 	0x0800
#define K_MISSILE	0x1000
#define K_STARBURST	0x2000

#define K_ASYNACK	0x40


#define COMM_FILE	"c:sopwith?.dta" /*  Multi user communications file  */
#define COMM_CMD	"c:semaphor\0   "/*  Multi-user semaphor file        */



typedef struct tt {			/*  Continuous tone table entry    */
	unsigned  tt_tone;
	unsigned  tt_chng;
	struct tt *tt_next;
	struct tt *tt_prev;
}	TONETAB;


typedef struct obj {				/*  Object list 	    */
	int	   ob_state;
	int	   ob_x, ob_y;
	int	   ob_dx, ob_dy;
	int	   ob_angle;
	int	   ob_orient;
	int	   ob_speed;
	int	   ob_accel;
	int	   ob_flaps;
	struct obj *ob_firing;
	int	   ob_score;
	int	   ob_rounds;
	int	   ob_hitcount;
	int	   ob_updcount;
	int	   ob_life;
	struct obj *ob_owner;
	int	   ob_symhgt;
	int	   ob_symwdt;
	int	   ob_bombing;
	int	   ob_bombs;
	int	   ob_clr;
	int	   ob_lx, ob_ly;
	int	   ob_ldx, ob_ldy;
	struct obj *ob_next;
	struct obj *ob_prev;
	int	   ob_index;
	int	   ob_oldx;
	int	   ob_oldy;
	int	   ob_drwflg;
	int	   ob_delflg;
	char	   *ob_oldsym;
	int	   ( *ob_drawf ) ();
	int	   ( *ob_movef ) ();
	struct obj *ob_xnext;
	struct obj *ob_xprev;
	int	   ob_crashcnt;
	char	   *ob_newsym;
	int	   ob_bdelay;
	int	   ob_home;
	int	   ob_hx[3], ob_hy[3];
	int	   ob_type;
	struct obj *ob_dnext;
	int	   ob_athome;
	struct tt  *ob_sound;
	int	   ob_missiles;
	struct obj *ob_mfiring;
	int	   ob_mdelay;
	struct obj *ob_target;
	int	   ob_bursts;
	int	   ob_bfiring;
	int	   ob_bsdelay;
}	OBJECTS;

typedef struct {				/*  Game structure	    */
	int	 gm_x[MAX_PLYR*2];
	int	 gm_orient[MAX_PLYR*2];
	unsigned ( *gm_randf ) ();
	unsigned gm_rseed;
	int	 ( *gm_specf ) ();
	int	 gm_xtarg[MAX_TARG];
	int	 gm_ttarg[MAX_TARG];
}	GAMES;

typedef struct {				/*  Communications buffer   */
	unsigned mu_maxplyr;
	unsigned mu_numplyr;
	unsigned mu_lstplyr;
	unsigned mu_key[MAX_PLYR];
	unsigned mu_state[MAX_PLYR];
	unsigned mu_explseed;
}	MULTIO;

typedef struct {				/*  Output message record   */
	char	 msgo_cmd;
	char	 msgo_port;
	char	 msgo_ssgnl;
	char	 msgo_rsgnl;
	char	 msgo_1fill[6];
	MULTIO	 msgo_buff;
	char	 msgo_2fill[1];
}	MSGOUT;

typedef struct {				/*  Input message record    */
	char	 msgi_port;
	char	 msgi_1fill[2];
	char	 msgi_myport;
	char	 msgi_2fill[6];
	MULTIO	 msgi_buff;
	char	 msgi_3fill[1];
}	MSGIN;



typedef struct {			/*  Old display parameters for	  */
	int	ow_xorplot;		/*  each object 		  */
	int	ow_x, ow_y;
}	OLDWDISP;


#define COS(x)	sintab[(x+(ANGLES/4))%ANGLES]
#define SIN(x)	sintab[x%ANGLES]
#define BOOL	int
#define BIOFD	int

					/*  Internal representation of ground */
					/*  due to non-implementation of      */
					/*  unsigned char on ATARI	      */
#ifdef	IBMPC
#define GRNDTYPE	unsigned char
#endif
#ifdef	ATARI
#define GRNDTYPE	int
#endif
