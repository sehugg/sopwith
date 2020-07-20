/*
	swtitle  -	SW perform animation on the title screen

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
			87-03-10	Microsoft compiler.
			87-03-11	Title reformatting.
			87-04-01	Version 7.F15
			96-12-27	New network version.
			99-01-24	1999 copyright.
			2000-10-29	Copyright update.
			2003-01-27	GNU General Public License
*/
#include	"sw.h"


extern	int		savemode;		/* Saved PC video mode	  */
extern	BOOL		hires;			/* High res debug flag	  */
extern	BOOL		titleflg;		/* Title flag		  */
extern	int		tickmode;		/* Tick action to be done */
extern	int		counttick, countmove;	/* Performance counters   */
extern	int		movetick;		/* Move synchronization   */
extern	int	displx, disprx; 	/* Display left and right bounds  */
extern	BOOL	dispinit;		/* Initalized display flag.	  */
extern	GRNDTYPE ground[];		/* Ground height by pixel	  */




swtitln()
{
OBJECTS 	ob;
extern	 char	swplnsym[][ANGLES][SYMBYTES];
extern	 char	swtrgsym[][TARGBYTES];
extern	 char	swoxsym[][OXBYTES];
extern	 char	swhitsym[][SYMBYTES];
extern	 char	swwinsym[][WINBYTES];
register int	 i, h;

	savemode = get_type();
	set_type ( ( hires ) ? 6 : 4 );

	if ( titleflg )
		return;

	tickmode = 1;

	sound( S_TITLE, 0, NULL );
	swsound();

/*---------------- Original BMB Version---------------

	swcolour( 3 );
	swposcur( 13, 6 );
	puts( "S O P W I T H" );

	swcolour( 1 );
	swposcur( 12, 8 );
	puts( "(Version 7.F15)" );

	swcolour( 3 );
	swposcur( 5, 11 );
	puts( "(c) Copyright 1984, 1985, 1987" );

	swcolour( 1 );
	swposcur( 6, 12 );
	puts( "BMB " );
	swcolour( 3 );
	puts( "Compuscience Canada Ltd." );

------------------ Original BMB Version---------------*/

/*---------------- New Network Version ---------------*/

	swcolour( 3 );
	swposcur( 13, 4 );
	puts( "S O P W I T H" );

	swcolour( 1 );
	swposcur( 9, 6 );
	puts( "(The Author's Edition)" );

	swcolour( 3 );
	swposcur( 5, 9 );
	puts( "(c) Copyright 1984, 1985, 1987" );

	swcolour( 1 );
	swposcur( 6, 10 );
	puts( "BMB " );
	swcolour( 3 );
	puts( "Compuscience Canada Ltd." );

	swcolour( 3 );
	swposcur( 1, 12 );
	puts( "Copyright (C) 1984-2003 David L. Clark" );

/*---------------- New Network Version-----------------*/

	setvdisp();

	displx = 700;
	dispinit = TRUE;
	swground();

	ob.ob_type = PLANE;
	ob.ob_symhgt = ob.ob_symwdt = 16;
	ob.ob_clr = 1;
	ob.ob_newsym = swplnsym[0][0];
	swputsym( 260, 180, &ob );

	ob.ob_newsym = swwinsym[3];
	swputsym( 50, 180, &ob );

	ob.ob_type = OX;
	ob.ob_newsym = swoxsym[0];
	swputsym( 100, ground[800] + 16, &ob );

	ob.ob_type = TARGET;
	ob.ob_clr = 2;
	ob.ob_newsym = swtrgsym[3];
	swputsym( 234, ground[934] + 16, &ob );

	ob.ob_type = PLANE;
	ob.ob_newsym = swhitsym[0];
	swputsym( 20, 160, &ob );

	ob.ob_type = SMOKE;
	ob.ob_symhgt = ob.ob_symwdt = 1;
	ob.ob_newsym = (char *)0x82;
	h = 150;
	for ( i = 9; i; --i )
		swputsym( 30, h += 5, &ob );
}




swtitlf()
{

	if ( titleflg )
		return;

	sound( 0, 0, NULL );
	swsound();
	tickmode = 0;
}



swtickc()
{

	++counttick;
	/*--- DLC 96/12/27
	++movetick;
	----------------*/
	movetick+=10;
#ifdef	IBMPC
	soundadj();
#endif
}
